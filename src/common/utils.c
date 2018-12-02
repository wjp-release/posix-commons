#include "utils.h"
#include <string.h>
#include <ctype.h>

static char regex_chars[] = {'$','(',')','*','+','.','?','[','\\','^','{','|','\0'};

int posixc_is_regex(const char *query) {
    return (strpbrk(query, regex_chars) != NULL);
}

int binary_search(const char *needle, char **haystack, int start, int end) {
    if (start == end) return -1;
    int mid = start + ((end - start) / 2);
    int rc = strcmp(needle, haystack[mid]);
    if (rc < 0) {
        return binary_search(needle, haystack, start, mid);
    } else if (rc > 0) {
        return binary_search(needle, haystack, mid + 1, end);
    }
    return mid;
}

bool is_lowercase(const char *s) {
    int i;
    for (i = 0; s[i] != '\0'; i++) {
        if (!isascii(s[i]) || isupper(s[i])) {
            return false;
        }
    }
    return true;
}

