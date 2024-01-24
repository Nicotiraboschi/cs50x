// Check that a password has at least one lowercase letter, uppercase letter, number and symbol
// Practice iterating through a string
// Practice using the ctype library

#include <cs50.h>
#include <stdio.h>

bool valid(string password);

int main(void)
{
    string password = get_string("Enter your password: ");
    if (valid(password))
    {
        printf("Your password is valid!\n");
    }
    else
    {
        printf("Your password needs at least one uppercase letter, lowercase letter, number and symbol\n");
    }
}

// TODO: Complete the Boolean function below
bool valid(string password)
{

    /* creo variabili con value false per numbers && uppers: 65-90 && lowers: 97-122 && signs:
    (33-47 && 58-64 && 94-96 && 123-128) */

    bool numbers = false;
    bool uppers = false;
    bool lowers = false;
    bool signs = false;

    // passo un for loop nella parola, se password[i] è compresa tra: 48-57 numbers=true, ecc.

    for (int i = 0; password[i] != '\0'; i++)
    {
        int letter = password[i];
        if (letter >= 48 && letter <= 57) { numbers = true;}
        else if (letter >= 65 && letter <= 90) { uppers = true;}
        else if (letter >= 97 && letter <= 122) { lowers = true;}
        else if ((letter >= 33 && letter <= 47) || (letter >= 58 && letter <= 64) || (letter >= 94 && letter <= 96) || (letter >= 123 && letter <= 128))
        { signs = true;}
    }

     //if (numbers && ecc.) return true, else false.

    if (numbers && uppers && lowers && signs) {
        return true;
    }

    else {return false;};
}
