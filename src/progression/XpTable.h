#pragma once
// XpTable — формула нужного количества XP для следующего уровня.
// Можно заменить на массив "level -> xp", если захочется ручной баланс.
// Сейчас квадратичный рост: 5, 9, 15, 23, 33, 45...
// (5 + 4*(L-1) + (L-1)^2)

class XpTable {
public:
    static int xpForLevel(int currentLevel) {
        int L = currentLevel < 1 ? 1 : currentLevel;
        return 5 + 4 * (L - 1) + (L - 1) * (L - 1);
    }
};
