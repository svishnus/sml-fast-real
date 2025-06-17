#include <float.h>
#include <stdint.h>
#include <string.h>

#include "export.h"
#include "fast_float/fast_float.h"

extern "C" {

double fast_float_parse_chars(char const* chars, int start, int len) {
    double result;
    char const* original_str = chars + start;

    if (len == 0) {
        return DBL_MAX;
    }

    if (len >= 3) {
        if (strncmp(original_str, "nan", 3) == 0) {
            return 0.0 / 0.0;
        }
        if (strncmp(original_str, "inf", 3) == 0) {
            return 1.0 / 0.0;
        }
        if (strncmp(original_str, "+inf", 4) == 0) {
            return 1.0 / 0.0;
        }
        if (strncmp(original_str, "-inf", 4) == 0) {
            return -1.0 / 0.0;
        }
        if (strncmp(original_str, "~inf", 4) == 0) {
            return -1.0 / 0.0;
        }
    }

    if (len >= 8) {
        if (strncmp(original_str, "infinity", 8) == 0) {
            return 1.0 / 0.0;
        }
        if (strncmp(original_str, "+infinity", 9) == 0) {
            return 1.0 / 0.0;
        }
        if (strncmp(original_str, "-infinity", 9) == 0) {
            return -1.0 / 0.0;
        }
        if (strncmp(original_str, "~infinity", 9) == 0) {
            return -1.0 / 0.0;
        }
    }

    bool has_tilde = false;
    bool has_leading_plus = (original_str[0] == '+');

    for (int i = 0; i < len; i++) {
        if (original_str[i] == '~') {
            has_tilde = true;
            break;
        }
    }

    if (!has_tilde && !has_leading_plus) {
        fast_float::from_chars_result res = fast_float::from_chars(original_str, original_str + len, result);
        if (res.ec == std::errc()) {
            return result;
        }
        return DBL_MAX;
    } else {
        constexpr int STACK_BUFFER_SIZE = 64;
        char stack_buffer[STACK_BUFFER_SIZE];
        char* temp;
        bool use_heap = len >= STACK_BUFFER_SIZE;

        if (use_heap) {
            temp = new char[len + 1];
        } else {
            temp = stack_buffer;
        }

        int write_pos = 0;
        for (int i = 0; i < len; i++) {
            char c = original_str[i];
            if (c == '~') {
                temp[write_pos++] = '-';
            } else if (c == '+' && i == 0) {
                // Skip leading +
            } else {
                temp[write_pos++] = c;
            }
        }

        fast_float::from_chars_result res = fast_float::from_chars(temp, temp + write_pos, result);

        if (use_heap) {
            delete[] temp;
        }

        if (res.ec == std::errc()) {
            return result;
        }
        return DBL_MAX;
    }
}
}