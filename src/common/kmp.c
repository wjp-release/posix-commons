#include "kmp.h"

typedef struct {
  size_t *P;
  size_t len;
  const char * str;
} kmp_prefix;


void F(const char *str, size_t len, kmp_prefix* prefix) {
    size_t *P = prefix->P;
    size_t i = 1;
    size_t j = P[0] = 0; // longest prefix length
    P[1] = 0;
    for (; i < len ; i++) {
        while (j > 0 && str[i] != str[j]) {
            j = P[j-1];
        }
        if (str[i] == str[j]) {
            j++;
        }
        P[i] = j;
    }
}


int kmp_prefix_search(const char *text, size_t text_len, kmp_prefix *prefix)
{
    const char *needle = prefix->str;
    const size_t needle_len = prefix->len;
    uint32_t *P = prefix->P;
    uint32_t j = 0;
    for (uint32_t i = 0; i < text_len ; i++) {
        while (j > 0 && text[i] != needle[j]) {
            j = P[j+1];
        }
        if (needle[j] == text[i]) {
            j++;
        }
        if (j == needle_len) {
            return i - needle_len + 1;
        }
    }
    return -1;
}

int posixc_kmp_contains(const char *text, size_t text_len, const char *needle, size_t needle_len)
{
    uint32_t P[needle_len];
    kmp_prefix prefix;
    prefix.P = P;
    prefix.str = needle;
    prefix.len = needle_len;
    F(needle, needle_len, &prefix);
    return kmp_prefix_search(text, text_len, &prefix);
}
