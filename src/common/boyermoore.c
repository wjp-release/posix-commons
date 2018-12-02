#include "boyermoore.h"
#include "math.h"
#include <string.h>
#include <ctype.h>

struct posixc_boyermoore{
    size_t      alpha_skip_lookup[256];
    size_t*     find_skip_lookup;
    bool        case_sensitive;
    const char* query;
    size_t      query_len;
};

bool is_prefix(const char *s, const size_t s_len, const size_t pos, const bool case_sensitive) {
    size_t i;
    for (i = 0; pos + i < s_len; i++) {
        if (case_sensitive) {
            if (s[i] != s[i + pos]) {
                return false;
            }
        } else {
            if (tolower(s[i]) != tolower(s[i + pos])) {
                return false;
            }
        }
    }
    return true;
}

size_t suffix_len(const char *s, const size_t s_len, const size_t pos, const bool case_sensitive) {
    size_t i;
    for (i = 0; i < pos; i++) {
        if (case_sensitive) {
            if (s[pos - i] != s[s_len - i - 1]) {
                break;
            }
        } else {
            if (tolower(s[pos - i]) != tolower(s[s_len - i - 1])) {
                break;
            }
        }
    }
    return i;
}

void generate_find_skip(const char *find, const size_t f_len, size_t **skip_lookup, const bool case_sensitive) {
    size_t i;
    size_t s_len;
    size_t *sl = (size_t*)malloc(f_len * sizeof(size_t));
    *skip_lookup = sl;
    size_t last_prefix = f_len;
    for (i = last_prefix; i > 0; i--) {
        if (is_prefix(find, f_len, i, case_sensitive)) {
            last_prefix = i;
        }
        sl[i - 1] = last_prefix + (f_len - i);
    }
    for (i = 0; i < f_len; i++) {
        s_len = suffix_len(find, f_len, i, case_sensitive);
        if (find[i - s_len] != find[f_len - 1 - s_len]) {
            sl[f_len - 1 - s_len] = f_len - 1 - i + s_len;
        }
    }
}

void generate_alpha_skip(const char *find, size_t f_len, size_t skip_lookup[], const bool case_sensitive) {
    size_t i;
    for (i = 0; i < 256; i++) {
        skip_lookup[i] = f_len;
    }
    f_len--;
    for (i = 0; i < f_len; i++) {
        if (case_sensitive) {
            skip_lookup[(unsigned char)find[i]] = f_len - i;
        } else {
            skip_lookup[(unsigned char)tolower(find[i])] = f_len - i;
            skip_lookup[(unsigned char)toupper(find[i])] = f_len - i;
        }
    }
}

void posixc_boyermoore_init(posixc_boyermoore* boyermoore, char* query, size_t query_len, bool case_sensitive){
    boyermoore->query=query;
    boyermoore->query_len=query_len;
    boyermoore->case_sensitive=case_sensitive;
    generate_alpha_skip(query, query_len, boyermoore->alpha_skip_lookup, case_sensitive);
    generate_find_skip(query, query_len, &boyermoore->find_skip_lookup, case_sensitive);
}

const char* posixc_boyermoore_search_in(posixc_boyermoore* boyermoore, const char* text, size_t text_len){
    size_t pos = boyermoore->query_len - 1;
    while (pos < s_len) {
        ssize_t i = boyermoore->query_len - 1;
        for (;i >= 0 && (boyermoore->case_sensitive?text[pos]:tolower(text[pos]))==boyermoore->query[i]; pos--, i--);
        if (i < 0) return text + pos + 1;
        pos += posixc_max(boyermoore->alpha_skip_lookup[(unsigned char)text[pos]], boyermoore->find_skip_lookup[i]);
    }
    return NULL;
}

const char* posixc_boyermoore_contains(const char* text, size_t text_len, const char* query, size_t query_len){
    posixc_boyermoore tmp;
    posixc_boyermoore_init(&tmp, query, query_len, true);
    return posixc_boyermoore_search_in(&tmp, text, text_len);
}

const char* posixc_boyermoore_contains_case_ignored(const char* text, size_t text_len, const char* query, size_t query_len){
    posixc_boyermoore tmp;
    posixc_boyermoore_init(&tmp, query, query_len, false);
    return posixc_boyermoore_search_in(&tmp, text, text_len);
}