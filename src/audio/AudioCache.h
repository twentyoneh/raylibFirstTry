#pragma once
// AudioCache — ленивый кеш Sound (raylib).
//
// Использование:
//   audio.play("shot", paths::sfx::shotBasic);
// При первом вызове по ключу — пытается LoadSound(path). Если файла нет —
// логирует один варнинг и сохраняет "пустой" звук (frameCount=0).
// Дальше play() для отсутствующего звука — no-op.
//
// Поэтому можно добавить звуковые хуки в код ДО того, как появятся .wav
// файлы — игра не упадёт, просто будет тишина по отсутствующим ключам.
//
// Деструктор сам выгружает все загруженные звуки.
// Требование: InitAudioDevice() должен быть вызван до использования (см. main.cpp).

#include "raylib.h"
#include "../util/Log.h"
#include <string>
#include <unordered_map>

class AudioCache {
public:
    AudioCache() = default;
    ~AudioCache() { clear(); }

    AudioCache(const AudioCache&) = delete;
    AudioCache& operator=(const AudioCache&) = delete;

    // Воспроизвести звук с переданным ключом. Если ещё не загружен — ленивая загрузка.
    void play(const std::string& key, const std::string& path, float volume = 1.0f) {
        Sound& s = ensure_(key, path);
        if (s.frameCount > 0) {
            SetSoundVolume(s, volume);
            PlaySound(s);
        }
    }

    // Прелоад нескольких звуков заранее (не обязателен).
    void preload(const std::string& key, const std::string& path) {
        (void)ensure_(key, path);
    }

private:
    Sound& ensure_(const std::string& key, const std::string& path) {
        auto it = map_.find(key);
        if (it != map_.end()) return it->second;

        Sound s{};
        if (FileExists(path.c_str())) {
            s = LoadSound(path.c_str());
            LOG_D("AUDIO", "loaded '%s' from %s", key.c_str(), path.c_str());
        } else {
            LOG_W("AUDIO", "missing file: %s — sound '%s' will be silent",
                  path.c_str(), key.c_str());
        }
        map_.emplace(key, s);
        return map_.at(key);
    }

    void clear() {
        for (auto& [k, s] : map_) if (s.frameCount > 0) UnloadSound(s);
        map_.clear();
    }

    std::unordered_map<std::string, Sound> map_;
};
