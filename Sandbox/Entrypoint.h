#include "Macros.h"

#define FF_ACTIVE_LESSON_MAJOR	5
#define FF_ACTIVE_LESSON_MINOR	2
#define FF_ISTEST				1

#define CHECK_ACTIVE(major, minor, test)\
FF_ACTIVE_LESSON_MAJOR == major && FF_ACTIVE_LESSON_MINOR == minor && FF_ISTEST == test