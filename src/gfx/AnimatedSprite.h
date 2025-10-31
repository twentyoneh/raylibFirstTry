#pragma once
#include <vector>
#include <raylib.h>
#include "Sprite.h"
#include "SpriteSheet.h"

struct Animation {
    std::vector<int> frameIndices;
    float fps = 10.0f;
    bool loop = true;   // «ацикливать анимацию
};

class AnimatedSprite {
public:
    AnimatedSprite() = default;

    void setSheet(const SpriteSheet* sheet);

    // ”правление анимаци€ми
    void add(const std::string& name, const Animation& anim);
    bool has(const std::string& name) const;
    void play(const std::string& name, bool restart = false);
    void stop();
    bool isPlaying() const { return playing_; }
    std::string current() const { return currentName_; }

    void update(float dt);

    // –ендер
    void draw() const;

    // ѕараметры геометрии/внешнего вида (проксируем во внутренний Sprite)
    void setPosition(Vector2 p) { sprite_.setPosition(p); }
    void setScale(Vector2 s) { sprite_.setScale(s); }
    void setRotation(float deg) { sprite_.setRotation(deg); }
    void setOrigin(Vector2 o) { sprite_.setOrigin(o); }
    void centerOrigin() { sprite_.centerOrigin(); }
    void setTint(Color c) { sprite_.setTint(c); }
    void setFlip(bool fx, bool fy) { sprite_.setFlip(fx, fy); }
    Vector2 position() const { return sprite_.position(); }
    Vector2 scale() const { return sprite_.scale(); }
    float   rotation() const { return sprite_.rotation(); }

private:
    void applyFrame(); // примен€ем текущий кадр к внутреннему Sprite

    const SpriteSheet* sheet_ = nullptr;
    Sprite sprite_;

    std::unordered_map<std::string, Animation> animations_;
    std::string currentName_;
    int frameIndex_ = 0;
    float timeAcc_ = 0.0f;      // накопитель времени
    bool playing_ = false;
};