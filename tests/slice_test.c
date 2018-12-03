#include "posixc.h"

int main()
{
    const char* pos= posixc_kmp_contains("123456", 6, "23", 2);
    printf("%s\n",pos);

    printf("PosixC Slice\n");
    posixc_slice s;
    posixc_slice_init_from(&s, "Hello PosixC Slice!");
    posixc_slice needle;
    posixc_slice_init_from(&needle, "six");
    bool result=posixc_slice_contains(&s, &needle);
    printf("result=%s\n", result?"true":"false");

    return 0;
}