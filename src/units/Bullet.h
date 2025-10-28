#pragma once

enum class ProjectileKind : uint8_t {
    BasicBullet,
    KnightBasic,   
    Heavy,         
    Fast,          
    ShotgunPellet  
};

struct Bullet {
    Vector2 pos{};
    Vector2 vel{};
    float   ttl{ 1.0f };
    float   radius{ 4.0f };
    int     damage{ 1 };
    bool    active{ false };
    ProjectileKind kind{ ProjectileKind::KnightBasic };
};