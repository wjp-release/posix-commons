#include "utils.h"
#include <string.h>
#include <ctype.h>
#include <math.h>

static char regex_chars[] = {'$','(',')','*','+','.','?','[','\\','^','{','|','\0'};

int posixc_is_regex(const char *query) {
    return (strpbrk(query, regex_chars) != NULL);
}

int posixc_binary_search(const char *needle, char **haystack, int start, int end) {
    if (start == end) return -1;
    int mid = start + ((end - start) / 2);
    int rc = strcmp(needle, haystack[mid]);
    if (rc < 0) {
        return posixc_binary_search(needle, haystack, start, mid);
    } else if (rc > 0) {
        return posixc_binary_search(needle, haystack, mid + 1, end);
    }
    return mid;
}

bool posixc_is_lowercase(const char *s) {
    int i;
    for (i = 0; s[i] != '\0'; i++) {
        if (!isascii(s[i]) || isupper(s[i])) {
            return false;
        }
    }
    return true;
}

void posixc_reverse(char *str, int len){ 
    int i=0, j=len-1, temp; 
    while (i<j) { 
        temp = str[i]; 
        str[i] = str[j]; 
        str[j] = temp; 
        i++; j--; 
    } 
} 

int posixc_itoa(int x, char* str){ 
    int i = 0; 
    while (x) { 
        str[i++] = (x%10) + '0'; 
        x = x/10; 
    } 
    posixc_reverse(str, i); 
    str[i] = '\0'; 
    return i; 
} 

int posixc_itoa_padding_zeros(int x, char* str, int d){ 
    int i = 0; 
    while (x) { 
        str[i++] = (x%10) + '0'; 
        x = x/10; 
    } 
    while (i < d) str[i++] = '0';  // padding zeros to get required digits
    posixc_reverse(str, i); 
    str[i] = '\0'; 
    return i; 
} 

int posixc_ftoa(float n, char *res, int afterpoint){ 
    int ipart = (int)n;  
    float fpart = n - (float)ipart;  
    int i = posixc_itoa(ipart, res); 
    if (afterpoint != 0) 
    { 
        res[i] = '.'; 
        fpart = fpart * pow(10, afterpoint); 
        posixc_itoa_padding_zeros((int)fpart, res + i + 1, afterpoint); 
    } 
    return i+afterpoint+1;
} 

int posixc_ftoa_within_given_bytes(float n, char *res, int afterpoint, int maxbytes){ 
    int ipart = (int)n;  
    float fpart = n - (float)ipart;  
    int i = posixc_itoa(ipart, res); 
    if(i+afterpoint+1>maxbytes) afterpoint=maxbytes-i-1;
    if (afterpoint != 0) 
    { 
        res[i] = '.'; 
        fpart = fpart * pow(10, afterpoint); 
        posixc_itoa_padding_zeros((int)fpart, res + i + 1, afterpoint); 
    } 
    return i+afterpoint+1;
} 


int posixc_dtoa(double n, char *res, int afterpoint){ 
    int ipart = (int)n;  
    double dpart = n - (double)ipart;  
    int i = posixc_itoa(ipart, res); 
    if (afterpoint != 0) 
    { 
        res[i] = '.'; 
        dpart = dpart * pow(10, afterpoint); 
        posixc_itoa_padding_zeros((int)dpart, res + i + 1, afterpoint); 
    } 
    return i+afterpoint+1;
} 

int posixc_dtoa_within_given_bytes(double n, char *res, int afterpoint, int maxbytes){ 
    int ipart = (int)n;  
    double dpart = n - (double)ipart;  
    int i = posixc_itoa(ipart, res); 
    if(i+afterpoint+1>maxbytes) afterpoint=maxbytes-i-1;
    if (afterpoint != 0) 
    { 
        res[i] = '.'; 
        dpart = dpart * pow(10, afterpoint); 
        posixc_itoa_padding_zeros((int)dpart, res + i + 1, afterpoint); 
    } 
    return i+afterpoint+1;
} 
