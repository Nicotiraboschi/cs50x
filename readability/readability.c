#include <stdio.h>
#include <string.h>
#include <math.h>

int count_letters(char text[], int length);
int count_words(char text[], int length);
int count_sentences(char text[], int length);


int main(void)
{
    char text[1000];
    printf("Text: ");
    fgets(text, sizeof(text), stdin);
    int length = strlen(text);
    if (text[length - 1] == '\n')
    {
        text[length - 1] = '\0';
    }
    int letters = count_letters(text, length);
    int words = count_words(text, length);
    int sentences = count_sentences(text, length);
    float index = 0.0588 * letters / words * 100 - 0.296 * sentences / words * 100 - 15.8;
    int grade = round(index);
    if (grade > 16)
    {
        printf("Grade 16+\n");
    }
    else if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %d\n", grade);
    }
}

int count_letters(char text[], int length)
{
    int count = 0;
    for (int i = 0; i < length; i++)
    {
        if ((text[i] >= 65 && text[i] <= 90) || (text[i] >= 97 && text[i] <= 122))
        {
            count++;
        }
    }
    return count;
}

int count_words(char text[], int length)
{
    int count = 0;
    for (int i = 0; i < length; i++)
    {
        if (text[i] == 32)
        {
            count++;
        }
    }
    return count + 1;
}

int count_sentences(char text[], int length)
{
    int count = 0;
    for (int i = 0; i < length; i++)
    {
        if ((text[i] == 33 || text[i] == 46 || text[i] == 63) && text[i + 1] == 32)
        {
            count++;
        }
    }
    return count + 1;
}
