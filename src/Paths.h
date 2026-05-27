#pragma once
// Paths — единое место для всех путей к ресурсам.
// Если папка переедет — правишь здесь, а не лазишь по cpp-файлам.
//
// Соглашение: пути относительные к cwd, в которой запускается бинарь
// (обычно — корень проекта). raylib LoadTexture/LoadSound примут такой
// путь без модификаций.
//
// Расширение: добавь новую константу, не плодя константы внутри cpp.

namespace paths {

namespace tex {
    inline constexpr const char* player        = "resources/textures/units/player/Player0001.png";
}

namespace sfx {
    // Заготовка под звуки — добавляй по мере появления .wav в resources/sounds/
    // inline constexpr const char* shotBasic  = "resources/sounds/shot_basic.wav";
}

namespace font {
    // inline constexpr const char* uiPrimary  = "resources/fonts/PressStart2P.ttf";
}

} // namespace paths
