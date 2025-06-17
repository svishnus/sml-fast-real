#include <float.h>
#include <stdint.h>
#include <string.h>

#include "export.h"
#include "fast_float/fast_float.h"

extern "C" {
double fast_float_parse_chars(char const* chars, int start, int len) {
    double result;
    char const* str = chars + start;
    char const* end = str + len;

    // Check for special cases first
    if (len >= 3) {
        if (strncmp(str, "nan", 3) == 0) {
            return 0.0 / 0.0;   // NaN
        }
        if (strncmp(str, "inf", 3) == 0) {
            if (len >= 8 && strncmp(str, "infinity", 8) == 0) {
                return 1.0 / 0.0;   // Infinity
            }
            return 1.0 / 0.0;   // Infinity
        }
        if (strncmp(str, "+inf", 4) == 0) {
            if (len >= 9 && strncmp(str, "+infinity", 9) == 0) {
                return 1.0 / 0.0;   // Positive Infinity
            }
            return 1.0 / 0.0;   // Positive Infinity
        }
        if (strncmp(str, "-inf", 4) == 0) {
            if (len >= 9 && strncmp(str, "-infinity", 9) == 0) {
                return -1.0 / 0.0;   // Negative Infinity
            }
            return -1.0 / 0.0;   // Negative Infinity
        }
        if (strncmp(str, "~inf", 4) == 0) {
            if (len >= 9 && strncmp(str, "~infinity", 9) == 0) {
                return -1.0 / 0.0;   // Negative Infinity
            }
            return -1.0 / 0.0;   // Negative Infinity
        }
    }

    // For normal numbers, we need to handle ~ as a negative sign
    // Create a temporary buffer with ~ replaced by -
    char* temp = new char[len + 1];
    for (int i = 0; i < len; i++) {
        temp[i] = (str[i] == '~') ? '-' : str[i];
    }
    temp[len] = '\0';

    // Parse the modified string
    fast_float::from_chars_result res = fast_float::from_chars(temp, temp + len, result);
    delete[] temp;

    if (res.ec == std::errc()) {
        return result;   // Success
    }

    // If parsing failed, try parsing without the + prefix
    if (len > 0 && temp[0] == '+') {
        temp = new char[len];
        for (int i = 0; i < len - 1; i++) {
            temp[i] = temp[i + 1];
        }
        temp[len - 1] = '\0';
        res = fast_float::from_chars(temp, temp + len - 1, result);
        delete[] temp;
        if (res.ec == std::errc()) {
            return result;
        }
    }

    return DBL_MAX;   // Failure - using DBL_MAX as a sentinel value
}
}
