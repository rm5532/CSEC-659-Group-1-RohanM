#include <stdio.h>
// 1. compile via cl. 
// 2. run the binary. notice the output strings.
// 3. run strings.exe on the binary. 
// 4. try to find evil.exe and evilW.exe.
int main(void) {
    char s1[] = {'e','v','i','l','.','e','x','e',0};
    char s2[] = "evil2.exe";
    printf("%s\n",s1);
    printf("%s\n",s2);

    wchar_t s3[] = { L'e', L'v', L'i', L'l', L'W', L'.', L'e', L'x', L'e', 0};
    wchar_t s4[] = L"evil4.exe";
    printf("%S\n",s3);
    printf("%S\n",s4);
    return 0;
}
