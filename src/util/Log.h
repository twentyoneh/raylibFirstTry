#pragma once
// Лог-обёртка над raylib TraceLog.
// raylib пишет уровни LOG_TRACE..LOG_FATAL в stderr. Используем макросы
// чтобы можно было искать по тегу [SCENE], [PLAYER], [BULLET] и т.п.
// Уровень логирования задаётся в main через SetTraceLogLevel().

#include "raylib.h"

#define LOG_T(tag, fmt, ...) TraceLog(LOG_TRACE,   "[" tag "] " fmt, ##__VA_ARGS__)
#define LOG_D(tag, fmt, ...) TraceLog(LOG_DEBUG,   "[" tag "] " fmt, ##__VA_ARGS__)
#define LOG_I(tag, fmt, ...) TraceLog(LOG_INFO,    "[" tag "] " fmt, ##__VA_ARGS__)
#define LOG_W(tag, fmt, ...) TraceLog(LOG_WARNING, "[" tag "] " fmt, ##__VA_ARGS__)
#define LOG_E(tag, fmt, ...) TraceLog(LOG_ERROR,   "[" tag "] " fmt, ##__VA_ARGS__)
