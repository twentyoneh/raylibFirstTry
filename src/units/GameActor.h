#pragma once
// GameActor — базовый класс для всего, что имеет позицию на сцене:
// игрок, враги, пикапы. Хранит только позицию и предоставляет общий
// интерфейс update/draw, чтобы PlayScene могла работать с ними через
// единый указатель base-класса (полиморфизм).
//
// Раньше позиция была int — это не давало плавного движения по dt.
// Теперь Vector2 (float), движение — через move(delta) где delta уже
// посчитана как направление * скорость * dt.

#include "raylib.h"
#include "raymath.h"

class GameActor
{
public:
    GameActor(float x = 0.f, float y = 0.f) : pos_{ x, y } {}
    virtual ~GameActor() = default;

    Vector2 position() const { return pos_; }
    void    setPosition(Vector2 p) { pos_ = p; }

    // Прибавляет уже посчитанный сдвиг. Скорость/dt считается выше.
    void    move(Vector2 delta) { pos_ = Vector2Add(pos_, delta); }

    virtual void update(float /*dt*/) {}
    virtual void draw() const {}

    // Радиус для круговых коллизий. Подклассы могут переопределить.
    virtual float radius() const { return 8.f; }

    bool   alive() const { return alive_; }
    void   kill() { alive_ = false; }

protected:
    Vector2 pos_{};
    bool    alive_ = true;
};
