#include <stdio.h>
#include <string.h>
#include <ctype.h>

char *cript(char *string, char *plaintext);

int total = 0;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("./substitution key");
        return 1;
    }
    char string[27];
    sscanf(argv[1], "%26s", string);
    int length = strlen(string);
    if (length != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    for (int i = 0; i < length; i++)
    {
        for (int j = i + 1; j - 1 < length; j++)
        {
            char c = tolower(string[i]);
            if (c == tolower(string[j]) || c < 97 || c > 122)
            {
                return 1;
            }
        }
    }
    printf("plaintext: ");
    char plaintext[1000];
    fgets(plaintext, sizeof(plaintext), stdin);
    printf("ciphertext: ");
    char *ciphertext = cript(string, plaintext);
    printf("%s", ciphertext);
    // printf("\n");
    return 0;
}

char *cript(char *string, char *plaintext)
{
    for (int i = 0; i < strlen(plaintext); i++)
    {
        char flag = 'n';
        if (plaintext[i] >= 97 && plaintext[i] <= 122)
        {
            flag = 'l';
            plaintext[i] = plaintext[i] - 97;
        }
        else if (plaintext[i] >= 65 && plaintext[i] <= 90)
        {
            flag = 'u';
            plaintext[i] = plaintext[i] - 65;
        }
        else
        {
            continue;
        }
        plaintext[i] = (flag == 'l' ? tolower(string[(int)plaintext[i]]) : toupper(string[(int)plaintext[i]]));
    }
    return plaintext;
}

