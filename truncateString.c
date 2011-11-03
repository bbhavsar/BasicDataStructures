#include <stdio.h>
#include <string.h>
#include <assert.h>

void removeSpaces(char *str)
{
    char t;
    int readIdx = 0, writeIdx = 0;

    while ((t = str[readIdx++]) != '\0') {
        if (t != ' ') {
            str[writeIdx++] = t;
        }
    }
    str[writeIdx] = '\0';
}

int main()
{
    char *str = strdup("  Hello   World  Lol ");
    assert(str != NULL);
    printf("%s\n", str);
    removeSpaces(str);
    printf("%s\n", str);

    free(str);
    return 0;
}

