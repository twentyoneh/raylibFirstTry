#pragma once
// SpatialGrid — однородная сетка для ускорения пространственных запросов.
//
// Использование: каждый кадр (перед resolveCollisions_) делаем clear() и
// insert(idx, pos) для каждого активного врага. При проверке пули — вместо
// перебора всех enemies_ зовём queryNeighborhood(bullet.pos, fn) — fn вызовут
// только для врагов в 3x3 ячейках вокруг пули. Сложность O(N+M) вместо O(N*M).
//
// Параметры:
//  - cellSize — должна быть >= 2 * max(radius), чтобы 3x3-окрестность гарантированно
//    покрывала все возможные пересечения. Босс r=28, пуля r~4 → cellSize=64 даёт
//    запас. Если в игре появятся более крупные сущности — увеличь cellSize.
//
// Расширение: queryRadius(pos, r, fn) можно добавить, если потребуется явный
// радиус (например, для AoE-способностей).

#include "raylib.h"
#include <cmath>
#include <cstdint>
#include <unordered_map>
#include <vector>

class SpatialGrid {
public:
    explicit SpatialGrid(float cellSize = 64.f) : cellSize_(cellSize) {}

    void clear() { cells_.clear(); }

    void insert(int idx, Vector2 pos) {
        int cx, cy;
        cellOf_(pos, cx, cy);
        cells_[key_(cx, cy)].push_back(idx);
    }

    // Вызывает fn(int idx) для каждого элемента в 3x3 ячейках вокруг pos.
    // fn может быть лямбдой с захватом, что-нибудь делать с индексом.
    template <class Fn>
    void queryNeighborhood(Vector2 pos, Fn&& fn) const {
        int cx, cy;
        cellOf_(pos, cx, cy);
        for (int dy = -1; dy <= 1; ++dy) {
            for (int dx = -1; dx <= 1; ++dx) {
                auto it = cells_.find(key_(cx + dx, cy + dy));
                if (it == cells_.end()) continue;
                for (int idx : it->second) fn(idx);
            }
        }
    }

    float cellSize() const { return cellSize_; }

private:
    void cellOf_(Vector2 p, int& cx, int& cy) const {
        cx = (int)std::floor(p.x / cellSize_);
        cy = (int)std::floor(p.y / cellSize_);
    }

    int64_t key_(int cx, int cy) const {
        // Упаковываем 2 int32 в int64 — стабильный ключ для unordered_map.
        return ((int64_t)(uint32_t)cx << 32) | (uint32_t)cy;
    }

    float cellSize_;
    std::unordered_map<int64_t, std::vector<int>> cells_;
};
