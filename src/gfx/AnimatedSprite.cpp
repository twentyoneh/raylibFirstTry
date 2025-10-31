#include "AnimatedSprite.h"

void AnimatedSprite::setSheet(const SpriteSheet* sheet) {
    sheet_ = sheet;
    if (!sheet_ || !sheet_->tex) {
        throw std::runtime_error("AnimatedSprite::setSheet: sheet or texture is null");
    }
    // ѕрив€жем текстуру к базовому Sprite и нарисуем весь кадр по умолчанию
    sprite_.setTexture(*sheet_->tex);
    // ≈сли есть кадры в sheet_->frames Ч включим первый пр€моугольник
    if (!sheet_->frames.empty()) {
        Rectangle r = sheet_->frames[0];
        sprite_.setSourceRect(r);
    }
}

void AnimatedSprite::add(const std::string& name, const Animation& anim) {
    if (anim.fps <= 0.0f) {
        throw std::runtime_error("AnimatedSprite::add: fps must be > 0");
    }
    if (anim.frameIndices.empty()) {
        throw std::runtime_error("AnimatedSprite::add: frameIndices must not be empty");
    }
    animations_[name] = anim;
}

bool AnimatedSprite::has(const std::string& name) const {
    return animations_.find(name) != animations_.end();
}

void AnimatedSprite::play(const std::string& name, bool restart) {
    auto it = animations_.find(name);
    if (it == animations_.end()) {
        throw std::runtime_error("AnimatedSprite::play: animation not found: " + name);
    }

    // —мена анимации
    if (name != currentName_ || restart) {
        currentName_ = name;
        frameIndex_ = 0;
        timeAcc_ = 0.0f;
        playing_ = true;
        applyFrame();
    }
    else {
        // та же анимаци€ Ч просто продолжаем
        playing_ = true;
    }
}

void AnimatedSprite::stop() {
    playing_ = false;
}

void AnimatedSprite::update(float dt) {
    if (!playing_ || !sheet_) return;

    const Animation& anim = animations_.at(currentName_);

    // —колько времени кадр длитс€
    const float frameTime = 1.0f / anim.fps;
    timeAcc_ += dt;

    while (timeAcc_ >= frameTime) {
        timeAcc_ -= frameTime;
        frameIndex_++;

        if (frameIndex_ >= (int)anim.frameIndices.size()) {
            if (anim.loop) {
                frameIndex_ = 0;
            }
            else {
                frameIndex_ = (int)anim.frameIndices.size() - 1;
                playing_ = false; // дошли до конца Ч стопим
                break;
            }
        }
        applyFrame();
    }
}

void AnimatedSprite::draw() const {
    sprite_.draw();
}

void AnimatedSprite::applyFrame() {
    if (!sheet_) return;
    const Animation& anim = animations_.at(currentName_);
    int idxInSheet = anim.frameIndices[frameIndex_];

    if (idxInSheet < 0 || idxInSheet >= (int)sheet_->frames.size()) {
        // подстрахуемс€
        idxInSheet = 0;
    }
    sprite_.setSourceRect(sheet_->frames[idxInSheet]);
}