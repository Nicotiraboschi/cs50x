// Draws a pyramid using recursion

#include <cs50.h>
#include <stdio.h>

int collatz(int n);

int path = 0;

int main(void)
{
    // Get height of pyramid
    int number = get_int("Number: ");

    // Draw pyramid
    printf("%i\n",collatz(number));
}

int collatz(int n)
{
if (n == 1)
{
return 0;
}

else if (n%2 == 0) {
return 1 + collatz(n/2);
}

else {
return 1 + collatz (n * 3 + 1);
}
}


