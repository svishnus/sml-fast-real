#include <float.h>
#include <stdint.h>

#include "export.h"
#include "fast_float/fast_float.h"

extern "C" {
double fast_float_parse_chars(char const* chars, int start, int len) {
    double result;
    char const* str = chars + start;
    char const* end = str + len;
    fast_float::from_chars_result res = fast_float::from_chars(str, end, result);

    if (res.ec == std::errc()) {
        return result;   // Success
    }
    return DBL_MAX;   // Failure - using DBL_MAX as a sentinel value
}
}
