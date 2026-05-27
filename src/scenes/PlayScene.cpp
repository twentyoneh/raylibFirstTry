#include "PlayScene.h"
#include "UpgradeScene.h"
#include "UpgradeSceneContext.h"
#include "PauseScene.h"
#include "PauseSceneContext.h"
#include "RunSummaryScene.h"
#include "RunSummarySceneContext.h"
#include "MenuScene.h"
#include "MenuSceneContext.h"
#include "../weapons/BasicGun.h"
#include "../abilities/Ability.h"
#include "../progression/UpgradePool.h"
#include "../progression/XpTable.h"
#include "../util/Log.h"
#include "raymath.h"
#include <algorithm>
#include <cmath>

// Утилита: возвращает Transition, который свопает текущую сцену на новую MenuScene
// с указанным menu-контекстом (тем, что живёт в main.cpp на стеке).
static Transition swapToMenu(MenuScheneContext* menuCtx) {
    return Transition::Swap([menuCtx]() -> std::unique_ptr<Scene> {
        return std::make_unique<MenuScene>(*menuCtx);
    });
}

PlayScene::PlayScene(std::unique_ptr<PlaySceneContext> ctx)
    : TypedScene<PlaySceneContext>(*ctx),
      ownedCtx_(std::move(ctx)),
      player_(ownedCtx_->textures, "Hero", Vector2{ 200.f, 200.f })
{
    player_.equip(std::make_unique<BasicGun>());
    LOG_I("SCENE", "PlayScene constructed");
}

PlayScene::~PlayScene() { LOG_I("SCENE", "PlayScene destroyed"); }

void PlayScene::onEnterT(PlaySceneContext& ctx)
{
    cam_.target   = player_.position();
    cam_.offset   = { ctx.screenW * 0.5f, ctx.screenH * 0.5f };
    cam_.rotation = 0.0f;
    cam_.zoom     = 2.0f;
    LOG_I("SCENE", "PlayScene entered (screen %dx%d)", ctx.screenW, ctx.screenH);
}

void PlayScene::onExitT(PlaySceneContext& /*ctx*/) { LOG_I("SCENE", "PlayScene exited"); }

void PlayScene::handleInputT(PlaySceneContext& ctx)
{
    ctx.input.poll();

    moveWish_ = { 0.f, 0.f };
    if (ctx.input.down(Action::Right)) moveWish_.x += 1.f;
    if (ctx.input.down(Action::Left))  moveWish_.x -= 1.f;
    if (ctx.input.down(Action::Down))  moveWish_.y += 1.f;
    if (ctx.input.down(Action::Up))    moveWish_.y -= 1.f;

    fireHeld_ = ctx.input.down(Action::Fire);
    aimWorld_ = GetScreenToWorld2D(GetMousePosition(), cam_);

    if (ctx.input.pressed(Action::Pause)) {
        // Раньше Esc сразу выходил — теперь пушит PauseScene.
        pauseRequested_ = true;
    }
}

Transition PlayScene::updateT(PlaySceneContext& ctx, float dt)
{
    // Запросы из подсцен — обрабатываем первыми, чтобы не тратить кадр на
    // обновление в неконсистентном состоянии (мёртвый игрок и т.п.).
    if (exitToMenuRequested_) {
        LOG_I("SCENE", "exit to menu requested by sub-scene");
        return swapToMenu(ownedCtx_->menuCtx);
    }
    if (restartRequested_) {
        LOG_I("SCENE", "restart requested — swapping to fresh PlayScene");
        MenuScheneContext* menuCtxPtr = ownedCtx_->menuCtx;
        return Transition::Swap([menuCtxPtr]() -> std::unique_ptr<Scene> {
            auto playCtx = std::make_unique<PlaySceneContext>();
            playCtx->screenW = GetScreenWidth();
            playCtx->screenH = GetScreenHeight();
            playCtx->menuCtx = menuCtxPtr;
            return std::make_unique<PlayScene>(std::move(playCtx));
        });
    }

    // Esc / Pause → push PauseScene поверх (играет роль паузы).
    if (pauseRequested_) {
        pauseRequested_ = false;
        bool* exitFlag = &exitToMenuRequested_;
        int sw = ownedCtx_->screenW, sh = ownedCtx_->screenH;
        return Transition::Push([exitFlag, sw, sh]() -> std::unique_ptr<Scene> {
            auto pctx = std::make_unique<PauseSceneContext>();
            pctx->screenW = sw;
            pctx->screenH = sh;
            pctx->exitToMenuFlag = exitFlag;
            return std::make_unique<PauseScene>(std::move(pctx));
        });
    }

    // Смерть игрока → один раз пушим RunSummary с итогами.
    if (!player_.alive() && !summaryShown_) {
        summaryShown_ = true;
        LOG_W("SCENE", "player dead — pushing RunSummaryScene");
        // Снэпшот статистики в локалку для лямбды.
        RunStats snap = stats_;
        snap.wave  = wave_.wave();
        snap.level = player_.level();
        snap.xp    = player_.xp();
        snap.score = score_;
        bool* exitFlag    = &exitToMenuRequested_;
        bool* restartFlag = &restartRequested_;
        int sw = ownedCtx_->screenW, sh = ownedCtx_->screenH;
        return Transition::Push([snap, exitFlag, restartFlag, sw, sh]() -> std::unique_ptr<Scene> {
            auto rctx = std::make_unique<RunSummarySceneContext>();
            rctx->screenW = sw;
            rctx->screenH = sh;
            rctx->stats = snap;
            rctx->exitToMenuFlag = exitFlag;
            rctx->restartFlag    = restartFlag;
            return std::make_unique<RunSummaryScene>(std::move(rctx));
        });
    }
    if (!player_.alive()) {
        // Уже показали summary — ничего не делаем, ждём действия игрока.
        return Transition::None();
    }

    // Если накопились level-up'ы — пушим UpgradeScene по одному за раз.
    if (pendingUpgrades_ > 0) {
        return tryPushUpgrade_();
    }

    // ВНИМАНИЕ: gameDt — это «игровое» время с учётом slow-mo/паузы.
    // Реальное dt используется только там, где скорость экрана важна
    // (камера, UI-анимации). Подсистемы геймплея тикаются по gameDt.
    const float gameDt = dt * gameTimeMul_;

    // Накапливаем "время жизни" — используется в RunSummary.
    stats_.timeAlive += gameDt;

    updatePlayer_(gameDt);
    updateBullets_(gameDt);
    updateEnemies_(gameDt);
    updatePickups_(gameDt);
    updateAbilities_(gameDt);
    resolveCollisions_();

    // Волны: тикаем менеджер, спавним моба или босса по сигналу.
    wave_.update(gameDt);
    maybeSpawnEnemy_(gameDt);
    maybeSpawnBoss_();

    // Камера плавно тянется к игроку — реальное dt, чтобы не лагать на slow-mo.
    Vector2 target = player_.position();
    float k = 1.0f - powf(0.001f, dt);
    cam_.target.x += (target.x - cam_.target.x) * k;
    cam_.target.y += (target.y - cam_.target.y) * k;

    return Transition::None();
}

void PlayScene::updatePlayer_(float dt)
{
    Vector2 wish = moveWish_;
    if (Vector2LengthSqr(wish) > 0.0001f) {
        wish = Vector2Normalize(wish);
        player_.setMoving(true);
    } else {
        player_.setMoving(false);
    }
    player_.move(Vector2Scale(wish, player_.effectiveSpeed() * dt));
    player_.update(dt);

    if (fireHeld_ && player_.weapon()) {
        Vector2 origin = player_.position();
        Vector2 dir    = Vector2Subtract(aimWorld_, origin);
        if (Vector2LengthSqr(dir) > 0.0001f) {
            dir = Vector2Normalize(dir);
            float dmgBuff = player_.buffs().multiplier(Buff::Stat::DamageMul);
            player_.weapon()->tryFire(origin, dir, bullets_,
                                      ownedCtx_->bullets, dmgBuff);
        }
    }
}

// Обновление одного списка пуль (см. updateBullets_).
// Объявлено перед использованием, чтобы не зависеть от порядка методов.
static void updateBulletList_(std::vector<Bullet>& list, float dt) {
    for (auto& b : list) if (b.active) {
        b.pos.x += b.vel.x * dt;
        b.pos.y += b.vel.y * dt;
        b.ttl   -= dt;
        if (b.ttl <= 0.f) b.active = false;
    }
    list.erase(
        std::remove_if(list.begin(), list.end(),
            [](const Bullet& b){ return !b.active; }),
        list.end());
}

void PlayScene::updateBullets_(float dt)
{
    updateBulletList_(bullets_, dt);
    updateBulletList_(enemyBullets_, dt);
}

void PlayScene::updateEnemies_(float dt)
{
    // Собираем "контекст тика" — общий для всех врагов в этом кадре.
    EnemyTick tick;
    tick.playerPos    = player_.position();
    tick.enemyBullets = &enemyBullets_;
    tick.factory      = &ownedCtx_->bullets;

    for (auto& e : enemies_) if (e.alive()) e.tick(dt, tick);

    enemies_.erase(
        std::remove_if(enemies_.begin(), enemies_.end(),
            [](const Enemy& e){ return !e.alive(); }),
        enemies_.end());
}


void PlayScene::updatePickups_(float /*dt*/)
{
    pickups_.erase(
        std::remove_if(pickups_.begin(), pickups_.end(),
            [](const std::unique_ptr<Pickup>& p){ return !p || p->consumed(); }),
        pickups_.end());
}

void PlayScene::updateAbilities_(float dt)
{
    for (auto& a : player_.abilities()) {
        if (a) a->update(dt, player_, enemies_);
    }
}

void PlayScene::resolveCollisions_()
{
    // Перестраиваем spatial-сетку по живым врагам. O(N).
    enemyGrid_.clear();
    for (int i = 0; i < (int)enemies_.size(); ++i) {
        if (enemies_[i].alive()) enemyGrid_.insert(i, enemies_[i].position());
    }

    // --- Пуля × Враг (через сетку: O(bullets * average_neighbors)) ---
    for (auto& b : bullets_) {
        if (!b.active) continue;
        bool hit = false;
        enemyGrid_.queryNeighborhood(b.pos, [&](int idx){
            if (hit) return;            // пуля уже использовалась — пропускаем
            auto& e = enemies_[idx];
            if (!e.alive()) return;
            float rr = b.radius + e.radius();
            if (Vector2DistanceSqr(b.pos, e.position()) > rr * rr) return;

            e.damage(b.damage);
            b.active = false;
            hit = true;

            if (!e.alive()) {
                int xpGain = e.xpValue();
                score_ += e.isBoss() ? 100 : 10;
                stats_.kills++;
                if (e.isBoss()) stats_.bossKills++;
                int ups = player_.gainXp(xpGain);
                pendingUpgrades_ += ups;

                float r = (float)GetRandomValue(0, 100000) / 100000.f;
                if (r < e.weaponDropChance()) {
                    if (auto p = ownedCtx_->dropTable.roll(wave_.wave(), e.position())) {
                        pickups_.push_back(std::move(p));
                    }
                }
                if (e.isBoss()) wave_.notifyBossKilled();
            }
        });
    }

    // --- Игрок × Враг (контактный урон) ---
    for (auto& e : enemies_) {
        if (!e.alive() || !player_.alive()) continue;
        float rr = player_.radius() + e.radius();
        if (Vector2DistanceSqr(player_.position(), e.position()) <= rr * rr) {
            player_.damage(e.touchDamage());
            // Hp=1 мобы и так умрут от тарана при kill(); Tank/Shooter/Boss выживают.
            if (e.kind() == EnemyKind::Chaser || e.kind() == EnemyKind::Runner) {
                e.kill();
            }
        }
    }

    // --- Вражеская пуля × Игрок ---
    for (auto& b : enemyBullets_) {
        if (!b.active || !player_.alive()) continue;
        float rr = b.radius + player_.radius();
        if (Vector2DistanceSqr(b.pos, player_.position()) <= rr * rr) {
            player_.damage(b.damage);
            b.active = false;
        }
    }

    // --- Игрок × Пикап ---
    for (auto& p : pickups_) {
        if (!p || p->consumed()) continue;
        float rr = player_.radius() + p->radius();
        if (Vector2DistanceSqr(player_.position(), p->position()) <= rr * rr) {
            p->onPickup(player_);
        }
    }
}

void PlayScene::maybeSpawnEnemy_(float /*dt*/)
{
    if (!wave_.shouldSpawnEnemy()) return;

    float angle = (float)GetRandomValue(0, 360) * DEG2RAD;
    float dist  = (float)GetRandomValue(280, 420);
    Vector2 pos = {
        player_.position().x + cosf(angle) * dist,
        player_.position().y + sinf(angle) * dist
    };
    Enemy::Stats st = Enemy::randomForWave(wave_.wave());
    // С волной все враги чуть быстрее: +5 пикс/сек за каждую волну сверх первой.
    st.speed += 5.f * (wave_.wave() - 1);
    enemies_.emplace_back(pos, st);
    LOG_T("SCENE", "enemy spawned kind=%d at (%.0f, %.0f); total=%d",
          (int)st.kind, pos.x, pos.y, (int)enemies_.size());
}

void PlayScene::maybeSpawnBoss_()
{
    if (!wave_.shouldSpawnBoss()) return;
    // Спавним босса по фиксированному смещению от игрока.
    Vector2 pos = {
        player_.position().x + 220.f,
        player_.position().y + 220.f
    };
    enemies_.emplace_back(pos, Enemy::bossStats(wave_.wave()));
    wave_.notifyBossSpawned();
    LOG_I("SCENE", "BOSS spawned at (%.0f, %.0f)", pos.x, pos.y);
}

Transition PlayScene::tryPushUpgrade_()
{
    pendingUpgrades_--;
    LOG_I("SCENE", "push UpgradeScene (pending after: %d)", pendingUpgrades_);

    // Захватываем сырой указатель на Player — UpgradeScene не владеет.
    // Это безопасно, потому что PlayScene живёт всё время, пока UpgradeScene
    // лежит сверху (SceneStack не уничтожает нижние сцены при Push).
    Player* playerPtr = &player_;
    int screenW = ownedCtx_->screenW;
    int screenH = ownedCtx_->screenH;

    return Transition::Push([playerPtr, screenW, screenH]() -> std::unique_ptr<Scene> {
        auto ctx = std::make_unique<UpgradeSceneContext>();
        ctx->screenW = screenW;
        ctx->screenH = screenH;
        ctx->player  = playerPtr;
        ctx->options = UpgradePool::pickRandom(3);
        return std::make_unique<UpgradeScene>(std::move(ctx));
    });
}

void PlayScene::drawT(PlaySceneContext& ctx) const
{
    ClearBackground(Color{ 18,18,22,255 });
    BeginMode2D(cam_);
    drawWorld_();
    EndMode2D();
    drawHud_(ctx);
}

void PlayScene::drawWorld_() const
{
    // Сетка только в видимой зоне камеры.
    float zoomInv = 1.f / cam_.zoom;
    float halfW = cam_.offset.x * zoomInv;
    float halfH = cam_.offset.y * zoomInv;
    int minX = (int)floorf((cam_.target.x - halfW) / 64.f) * 64;
    int maxX = (int)ceilf ((cam_.target.x + halfW) / 64.f) * 64;
    int minY = (int)floorf((cam_.target.y - halfH) / 64.f) * 64;
    int maxY = (int)ceilf ((cam_.target.y + halfH) / 64.f) * 64;
    Color grid{ 40,40,48,255 };
    for (int x = minX; x <= maxX; x += 64) DrawLine(x, minY, x, maxY, grid);
    for (int y = minY; y <= maxY; y += 64) DrawLine(minX, y, maxX, y, grid);

    for (const auto& e : enemies_) if (e.alive()) e.draw();
    for (const auto& p : pickups_) if (p && !p->consumed()) p->draw();

    player_.draw();

    // Визуал способностей (поверх врагов, но за пулями).
    for (const auto& a : player_.abilities()) if (a) a->draw(player_);

    // Игроцкие пули — жёлтые.
    for (const auto& b : bullets_) if (b.active)
        DrawCircleV(b.pos, b.radius, YELLOW);
    // Вражеские — красные с тёмной обводкой (визуально отличить от пикапов).
    for (const auto& b : enemyBullets_) if (b.active) {
        DrawCircleV(b.pos, b.radius, RED);
        DrawCircleLines((int)b.pos.x, (int)b.pos.y, b.radius, MAROON);
    }
}

void PlayScene::drawHud_(const PlaySceneContext& ctx) const
{
    int x = 16, y = 16;

    // HP бар
    int hpW = 220, hpH = 16;
    DrawRectangle(x, y, hpW, hpH, Color{40,40,40,200});
    float pct = (float)player_.health() / (float)player_.maxHealth();
    pct = (pct < 0.f) ? 0.f : pct;
    DrawRectangle(x, y, (int)(hpW * pct), hpH, RED);
    DrawRectangleLines(x, y, hpW, hpH, RAYWHITE);
    DrawText(TextFormat("HP %d/%d", player_.health(), player_.maxHealth()),
             x + 6, y - 1, 16, RAYWHITE);
    y += hpH + 4;

    // XP бар
    int xpW = 220, xpH = 10;
    DrawRectangle(x, y, xpW, xpH, Color{40,40,40,200});
    int xpForLvl = XpTable::xpForLevel(player_.level());
    float xpct = xpForLvl > 0 ? (float)player_.xp() / (float)xpForLvl : 0.f;
    DrawRectangle(x, y, (int)(xpW * xpct), xpH, SKYBLUE);
    DrawRectangleLines(x, y, xpW, xpH, GRAY);
    DrawText(TextFormat("Lv %d   XP %d/%d", player_.level(),
                        player_.xp(), xpForLvl),
             x + xpW + 8, y - 3, 16, LIGHTGRAY);
    y += xpH + 8;

    DrawText(TextFormat("Score: %d", score_), x, y, 18, RAYWHITE);
    y += 22;

    // Wave
    const char* stateStr = "ACTIVE";
    if (wave_.state() == WaveManager::State::WaitingBoss) stateStr = "BOSS INCOMING";
    if (wave_.state() == WaveManager::State::BossFight)   stateStr = "BOSS";
    Color stCol = (wave_.state() == WaveManager::State::Active) ? RAYWHITE : GOLD;
    DrawText(TextFormat("Wave %d  [%s]  %.1fs",
                        wave_.wave(), stateStr,
                        wave_.state() == WaveManager::State::Active
                          ? wave_.timeLeft() : 0.f),
             x, y, 18, stCol);
    y += 22;

    // Оружие
    if (const Weapon* w = player_.weapon()) {
        const char* nm = w->name();
        if (w->config().magazine > 0) {
            if (w->isReloading()) {
                DrawText(TextFormat("%s [reload %d%%]", nm,
                    (int)(w->reloadProgress() * 100)), x, y, 18, YELLOW);
            } else {
                DrawText(TextFormat("%s %d/%d", nm, w->ammo(),
                    w->config().magazine), x, y, 18, RAYWHITE);
            }
        } else {
            DrawText(TextFormat("%s [inf]", nm), x, y, 18, RAYWHITE);
        }
        y += 22;
    }

    // Способности
    if (!player_.abilities().empty()) {
        DrawText("Abilities:", x, y, 16, LIGHTGRAY); y += 18;
        for (const auto& a : player_.abilities()) if (a) {
            DrawText(TextFormat("  %s Lv%d", a->name(), a->level()),
                     x, y, 14, PURPLE);
            y += 16;
        }
    }

    // Бафы
    const auto& buffs = player_.buffs().list();
    if (!buffs.empty()) {
        DrawText("Buffs:", x, y, 16, LIGHTGRAY); y += 18;
        for (const auto& b : buffs) {
            const char* tag = "?";
            switch (b.stat) {
                case Buff::Stat::DamageMul:   tag = "DMG";  break;
                case Buff::Stat::FireRateMul: tag = "RoF";  break;
                case Buff::Stat::SpeedMul:    tag = "SPD";  break;
                case Buff::Stat::MaxHpAdd:    tag = "HP+";  break;
            }
            if (b.duration > 0.f)
                DrawText(TextFormat("  %s x%.2f (%.1fs)", tag, b.value, b.remaining),
                         x, y, 14, ORANGE);
            else
                DrawText(TextFormat("  %s x%.2f", tag, b.value), x, y, 14, ORANGE);
            y += 16;
        }
    }

    // Подсказка
    DrawText("WASD — move, LMB — fire, Esc — exit",
             16, ctx.screenH - 28, 18, LIGHTGRAY);
}
