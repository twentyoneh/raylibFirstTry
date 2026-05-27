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
    // Звуковые ключи + пути. Если файл не появится — AudioCache просто
    // молчит и кидает варнинг один раз (см. audio/AudioCache.h).
    inline constexpr const char* shotBasic   = "resources/sounds/shot.wav";
    inline constexpr const char* hit         = "resources/sounds/hit.wav";
    inline constexpr const char* enemyDeath  = "resources/sounds/enemy_death.wav";
    inline constexpr const char* playerHurt  = "resources/sounds/player_hurt.wav";
    inline constexpr const char* levelUp     = "resources/sounds/level_up.wav";
    inline constexpr const char* pickup      = "resources/sounds/pickup.wav";
    inline constexpr const char* bossWarn    = "resources/sounds/boss_warn.wav";
    inline constexpr const char* bossDeath   = "resources/sounds/boss_death.wav";
}

namespace font {
    // inline constexpr const char* uiPrimary  = "resources/fonts/PressStart2P.ttf";
}

} // namespace paths
