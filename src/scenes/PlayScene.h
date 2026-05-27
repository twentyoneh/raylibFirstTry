#pragma once
// PlayScene — основной игровой экран.
//
// Что живёт здесь:
//  - игрок (Player) с оружием, бафами, способностями и опытом
//  - bullets_ — активные пули
//  - enemies_ — текущие враги, в т.ч. босс
//  - pickups_ — выпавшие оружия (только weapon drops; бафы стали апгрейдами)
//  - wave_    — WaveManager (волна, таймер, спавн босса)
//  - камера, score (визуально), pending levelups
//
// Поток уровней: каждое убийство → player.gainXp(...) → если levelUps>0,
// PlayScene пушит UpgradeScene (на каждый поднятый уровень — один пуш).
// UpgradeScene сама замораживает PlayScene и pop'ается после выбора.

#include "Scene.h"
#include "TypedScene.h"
#include "PlaySceneContext.h"
#include "../units/Player.h"
#include "../units/Enemy.h"
#include "../units/Bullet.h"
#include "../pickups/Pickup.h"
#include "../waves/WaveManager.h"
#include <memory>
#include <vector>

class PlayScene : public TypedScene<PlaySceneContext>
{
public:
    explicit PlayScene(std::unique_ptr<PlaySceneContext> ctx);
    ~PlayScene() override;

    void onEnterT(PlaySceneContext& ctx) override;
    void onExitT(PlaySceneContext& ctx) override;
    void handleInputT(PlaySceneContext& ctx) override;
    void drawT(PlaySceneContext& ctx) const override;
    Transition updateT(PlaySceneContext& ctx, float dt) override;

private:
    std::unique_ptr<PlaySceneContext> ownedCtx_;

    Player    player_;
    Camera2D  cam_{};
    bool      wantExit_{ false };

    // Намерение за кадр
    Vector2 moveWish_{};
    bool    fireHeld_{ false };
    Vector2 aimWorld_{};

    WaveManager wave_;
    int   pendingUpgrades_ = 0;  // сколько раз надо показать UpgradeScene подряд
    int   score_ = 0;            // косметическое (на HUD)

    // Множитель «игрового времени». 1.0 — обычная скорость, 0.0 — пауза,
    // 0.3 — slow-mo. Влияет на ВСЕ подсистемы через единый gameDt в updateT.
    // UI/камера используют реальный dt (не зависят от slow-mo).
    float gameTimeMul_ = 1.0f;
public:
    void setGameTimeMul(float m) { gameTimeMul_ = m; }
    float gameTimeMul() const { return gameTimeMul_; }
private:

    std::vector<Bullet>                  bullets_;
    std::vector<Enemy>                   enemies_;
    std::vector<std::unique_ptr<Pickup>> pickups_;

    void updatePlayer_(float dt);
    void updateBullets_(float dt);
    void updateEnemies_(float dt);
    void updatePickups_(float dt);
    void updateAbilities_(float dt);
    void resolveCollisions_();
    void maybeSpawnEnemy_(float dt);
    void maybeSpawnBoss_();
    Transition tryPushUpgrade_();

    void drawWorld_() const;
    void drawHud_(const PlaySceneContext& ctx) const;
};
