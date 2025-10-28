#pragma once
#include "raylib.h"
#include <unordered_map>
#include <string>
#include <vector>

struct Animation {
    std::vector<int> frames;   // индексы в SpriteSheet::frames
    float fps = 8.0f;
    bool loop = true;
};

class Animator {
public:

    void add(const std::string& name, Animation anim) { anims_[name] = std::move(anim); }

    void play(const std::string& name, bool restart = false) {
        if (current_ == name && !restart) return;
        current_ = name; time_ = 0.f; frameIdx_ = 0;
    }

    void update(float dt) {
        auto it = anims_.find(current_);
        if (it == anims_.end()) return;
        const auto& a = it->second;
        time_ += dt;
        float frameTime = 1.f / a.fps;
        while (time_ >= frameTime) {
            time_ -= frameTime;
            frameIdx_++;
            if (frameIdx_ >= (int)a.frames.size()) {
                if (a.loop) frameIdx_ = 0;
                else { frameIdx_ = (int)a.frames.size() - 1; break; }
            }
        }
    }

    int frame() const {
        auto it = anims_.find(current_);
        if (it == anims_.end() || it->second.frames.empty()) return 0;
        return it->second.frames[frameIdx_];
    }
    const std::string& state() const { return current_; }

private:
    std::unordered_map<std::string, Animation> anims_;
    std::string current_;
    float time_ = 0.f;
    int frameIdx_ = 0;
};