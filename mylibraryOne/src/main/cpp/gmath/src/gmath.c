#include <android/log.h>
#include "gmath.h"

#define LOGE(...) \
  ((void)__android_log_print(ANDROID_LOG_ERROR, "gmath::", __VA_ARGS__))

#if defined(__GNUC__) && __GNUC__ >= 4
#define GMATH_EXPORT __attribute__((visibility("default")))
#elif defined(__SUNPRO_C) && (__SUNPRO_C >= 0x590)
#define GMATH_EXPORT __attribute__((visibility("default")))
#else
#define GMATH_EXPORT
#endif

GMATH_EXPORT unsigned gpower(unsigned n) {
  if (n == 0)
    return 1;
  if (n > 31) {
    LOGE("error from power(%d): integer overflow", n);
    return 0;
  }

  unsigned val = gpower(n>>1) * gpower(n>>1);
  if(n & 1) {
    val *= 2;
  }

  return val;
}