#ifndef VSRTL_ENUM_H
#define VSRTL_ENUM_H

#include "enum.h"

#include <string>

#define Enum(name, ...) BETTER_ENUM(name, int, __VA_ARGS__);

#endif  // VSRTL_ENUM_H
