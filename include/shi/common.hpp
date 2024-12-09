#pragma once

#include <cstdint>

// TODO: idk if these are right
#define shi_likely(...)   __builtin_expect(__VA_ARGS__, 1)
#define shi_unlikely(...) __builtin_expect(__VA_ARGS__, 0)
