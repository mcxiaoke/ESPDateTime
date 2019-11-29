//#include <AUnit.h>
#include <AUnitVerbose.h>

#define ae(arg1, arg2) assertEqual(arg1, arg2)
#define ane(arg1, arg2) assertNotEqual(arg1, arg2)
#define al(arg1, arg2) assertLess(arg1, arg2)
#define am(arg1, arg2) assertMore(arg1, arg2)
#define ale(arg1, arg2) assertLessOrEqual(arg1, arg2)
#define ame(arg1, arg2) assertMoreOrEqual(arg1, arg2)
#define at(arg) assertTrue(arg)
#define af(arg) assertFalse(arg)

inline bool contains(const char* str, const String& text) {
  return strstr(str, text.c_str()) != nullptr;
}