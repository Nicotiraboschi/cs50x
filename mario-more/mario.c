#include <cs50.h>
#include <stdio.h>


int main(void)
{
    int n = 0;
    while (n < 1 || n > 8)
    {
        n = get_int("choose a number between 1 and 8\n");
    }
    int m = n;
    for (int l = n; l > 0; l--)
    {
        for (int h = l - 1; h > 0; h--)
        {
            printf(" ");
        }
        int h = m - l + 1;
        for (int j = h; j > 0; j--)
        {
            printf("#");
        }
        printf("  ");
        for (int j = h; j > 0; j--)
        {
            printf("#");
        }
        printf("\n");
    }
}

