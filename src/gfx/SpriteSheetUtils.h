#pragma once
#include "SpriteSheet.h"

// Ќарезает атлас пр€моугольниками фиксированного размера по сетке.
//
// sheet.tex            Ч должна указывать на валидную Texture2D
// tileW, tileH         Ч размеры тайла/кадра в пиксел€х
// startX, startY       Ч отступ (margin) слева/сверху
// spacingX, spacingY   Ч промежуток между тайлами (если есть)
// maxCols, maxRows     Ч ограничить количество столбцов/строк (-1 = до кра€ текстуры)
//
// ѕример: BuildGridFrames(sheet, 32, 32); // вс€ текстура по 32x32
inline void BuildGridFrames(SpriteSheet& sheet,
    int tileW, int tileH,
    int startX = 0, int startY = 0,
    int spacingX = 0, int spacingY = 0,
    int maxCols = -1, int maxRows = -1)
{
    sheet.frames.clear();

    if (!sheet.tex || tileW <= 0 || tileH <= 0) return;

    const int texW = sheet.tex->width;
    const int texH = sheet.tex->height;

    // —колько реально влезает по ширине/высоте с учЄтом отступов и промежутков
    auto countAlong = [](int total, int start, int size, int space)->int {
        if (start >= total) return 0;
        int avail = total - start;
        //  аждый тайл занимает size + space, кроме последнего Ч space не критичен
        int n = 0;
        int used = 0;
        while (used + size <= avail) {
            ++n;
            used += size + space;
        }
        return n;
        };

    int cols = (maxCols > 0) ? maxCols : countAlong(texW, startX, tileW, spacingX);
    int rows = (maxRows > 0) ? maxRows : countAlong(texH, startY, tileH, spacingY);

    for (int r = 0; r < rows; ++r) {
        int y = startY + r * (tileH + spacingY);
        for (int c = 0; c < cols; ++c) {
            int x = startX + c * (tileW + spacingX);
            if (x + tileW <= texW && y + tileH <= texH) {
                sheet.frames.push_back(Rectangle{ (float)x, (float)y, (float)tileW, (float)tileH });
            }
        }
    }
}
