#include <stdio.h>
#include <netinet/in.h>

int main() {
    short i = 0x1234;
    const char* p = (const char*)(&i);
    const char  n = *p;
    printf("%x\n",n);

    short ii = htons(i);
    const char* pp = (const char*)(&ii);
    const char  nn = *pp;
    printf("%x\n",nn);

    return 0;
}