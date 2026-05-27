#pragma once
// WaveManager — таймер волн и точка управления сложностью.
//
// Поток (см. PlayScene::updateT):
//   update(dt): тикает время. Если waveTimeLeft_ закончилось — переходим
//               в состояние WaitingBoss и сообщаем сцене shouldSpawnBoss().
//               Сцена сама спавнит босса и зовёт notifyBossSpawned().
//               Когда босс умер — сцена вызывает notifyBossKilled(), и
//               менеджер переходит к следующей волне.
//
// state diagram:
//   Active  ─ timer<=0 ─▶ WaitingBoss
//   WaitingBoss ─ bossSpawned ─▶ BossFight
//   BossFight ─ bossKilled ─▶ Active(wave+1)

#include "../util/Log.h"
#include <cmath>

class WaveManager {
public:
    enum class State { Active, WaitingBoss, BossFight };

    void update(float dt) {
        if (state_ == State::Active) {
            waveTimeLeft_ -= dt;
            if (waveTimeLeft_ <= 0.f) {
                state_ = State::WaitingBoss;
                LOG_I("WAVE", "wave %d timer ended → boss incoming", wave_);
            }
        }
        // Тики спавна обычных врагов идут только в Active.
        if (state_ == State::Active) {
            spawnTimer_ -= dt;
        }
    }

    bool shouldSpawnEnemy() {
        if (state_ != State::Active) return false;
        if (spawnTimer_ > 0.f) return false;
        spawnTimer_ = currentSpawnInterval();
        return true;
    }

    bool shouldSpawnBoss() const { return state_ == State::WaitingBoss; }

    void notifyBossSpawned() {
        state_ = State::BossFight;
        LOG_I("WAVE", "boss spawned for wave %d", wave_);
    }

    void notifyBossKilled() {
        LOG_I("WAVE", "boss killed; wave %d cleared", wave_);
        wave_++;
        waveTimeLeft_ = waveDuration_;
        spawnTimer_ = 0.f;
        state_ = State::Active;
        LOG_I("WAVE", "wave %d started (duration=%.1fs)", wave_, waveDuration_);
    }

    // ---- getters для HUD ----
    int   wave()         const { return wave_; }
    float timeLeft()     const { return waveTimeLeft_; }
    float waveDuration() const { return waveDuration_; }
    State state()        const { return state_; }

private:
    // Интервал спавна врагов уменьшается с ростом волны (быстрее → сложнее).
    float currentSpawnInterval() const {
        // 1.5 сек на 1й волне, ~0.4 на 10й
        return std::fmax(0.35f, 1.5f - 0.12f * (wave_ - 1));
    }

    int   wave_         = 1;
    float waveDuration_ = 30.f;
    float waveTimeLeft_ = 30.f;
    float spawnTimer_   = 0.f;
    State state_        = State::Active;
};
