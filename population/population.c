#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int n  = 0;
    while (n < 9)
    {
        n = get_int("initial population ");
    }

    int f = 1;
    while (f < n)
    {
        f = get_int("final population ");
    }

    int i = 0;

    while (n < f)
    {
        n += ((n / 3) - (n / 4));
        i++;
    }

    printf("Years: %d\n", i);
}
