#include <stdio.h>
#include <string.h>

void removeSpaces(char *str)
{
    int readIdx, writeIdx;
    char t;

    readIdx = writeIdx = 0;
    while ((t = str[readIdx++]) != '\0') {
        if (t != ' ') {
            str[writeIdx++] = t;
        }
    }
    str[writeIdx] = '\0';
}

int main()
{
    char *str = strdup("  HelloWorld  ");
    printf("%s\n", str);
    removeSpaces(str);
    printf("%s\n", str);

    free(str);
}

