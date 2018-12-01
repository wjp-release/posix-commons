#include "algorithms.h"

// match_ptr = boyer_moore_strnstr(match_ptr, opts.query, buf_len - buf_offset, opts.query_len, alpha_skip_lookup, find_skip_lookup, opts.casing == CASE_INSENSITIVE);
const char *boyer_moore_strnstr(const char *s, const char *find, const size_t s_len, const size_t f_len,
                                const size_t alpha_skip_lookup[], const size_t *find_skip_lookup, const int case_insensitive) {
    ssize_t i;
    size_t pos = f_len - 1;
    while (pos < s_len) {
        for (i = f_len - 1; i >= 0 && (case_insensitive ? tolower(s[pos]) : s[pos]) == find[i]; pos--, i--) {
        }
        if (i < 0) {
            return s + pos + 1;
        }
        pos += posixc_max(alpha_skip_lookup[(unsigned char)s[pos]], find_skip_lookup[i]);
    }
    return NULL;
}