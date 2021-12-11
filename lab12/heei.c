#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <ctype.h>

int main()
{
char ch;
scanf("%c", &ch);
printf("%c", ch);
    printf("Hello world!");
    fflush(stdout);
    return 0;
}
