#pragma once
#include "internal.h"

int     posixc_is_regex(const char *query);
int     posixc_binary_search(const char *needle, char **haystack, int start, int end);
bool    posixc_is_lowercase(const char *s);
void    posixc_reverse(char *str, int len);
int     posixc_itoa(int x, char* str); //return size of str
int     posixc_itoa_padding_zeros(int x, char* str, int d); //return size of non-zero part of str
int     posixc_ftoa(float n, char *res, int afterpoint); //return size of res
int     posixc_ftoa_within_given_bytes(float n, char *res, int afterpoint, int maxbytes); //return size of res
int     posixc_dtoa(double n, char *res, int afterpoint);  //return size of res
int     posixc_dtoa_within_given_bytes(double n, char *res, int afterpoint, int maxbytes); //return size of res


