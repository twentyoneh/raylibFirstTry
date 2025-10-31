#pragma once
#include <raylib.h>
#include <vector>
#include <unordered_map>
#include <string>

// SpriteSheet хранит ссылку на текстуру и список прямоугольников-кадров
// в текстурных координатах (пикселях).
struct SpriteSheet {
    // Владеет ли текстурой? — нет. Следи, чтобы Texture2D жил дольше SpriteSheet.
    const Texture2D* tex = nullptr;

    // Кадры (прямоугольники) в координатах текстуры.
    std::vector<Rectangle> frames;

    // Необязательные "имена кадров" -> индекс в frames (удобно для UI/эффектов).
    std::unordered_map<std::string, int> named;

    void clear() { frames.clear(); named.clear(); tex = nullptr; }

    // Добавить кадр вручную, вернуть его индекс.
    int addFrame(Rectangle r) {
        frames.push_back(r);
        return static_cast<int>(frames.size() - 1);
    }

    // Присвоить имя существующему кадру.
    void nameFrame(const std::string& name, int index) {
        if (index >= 0 && index < (int)frames.size()) named[name] = index;
    }

    // Получить индекс кадра по имени; -1 если не найден.
    int indexOf(const std::string& name) const {
        auto it = named.find(name);
        return (it == named.end()) ? -1 : it->second;
    }
};
