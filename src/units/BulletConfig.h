#pragma once

struct BulletConfig {
    float speed = 500.f;
    float ttl = 1.5f;
    float radius = 4.f;
    int   damage = 1;
    int   pierce = 0;    // сколько целей можно пробить (0 = не пробивает)
	//TODO: добавить текстуры, анимации и эффекты
};