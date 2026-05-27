#pragma once
// BulletConfig — параметры пули по её типу. Используется в BulletFactory.
// Здесь только данные; стрельбу (cooldown, кол-во в залпе, разброс) задаёт
// WeaponConfig в weapons/.

struct BulletConfig {
    float speed  = 500.f;  // пикс/сек
    float ttl    = 1.5f;   // сек, после чего пуля гаснет
    float radius = 4.f;    // для отрисовки и коллизий
    int   damage = 1;
    int   pierce = 0;      // сколько целей может пройти насквозь (0 = одну)
};
