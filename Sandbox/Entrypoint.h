#include "Macros.h"

#define ACTIVE_LESSON_MAJOR 5
#define ACTIVE_LESSON_MINOR 4
#define TEST				0

#define CHECK_ACTIVE(major, minor, test)\
ACTIVE_LESSON_MAJOR == major && ACTIVE_LESSON_MINOR == minor && TEST == test