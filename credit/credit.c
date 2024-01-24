#import <stdio.h>
#import <cs50.h>
#import <string.h>

int main(void)
{
    string credit_card = get_string("Credit card? ");
    string type = "";
    if (strlen(credit_card) == 15 && (credit_card[0] == '3' && (credit_card[1] == '4' || credit_card[1] == '7')))
    {
        type = "AMEX";
    }
    else if ((strlen(credit_card) == 13 || strlen(credit_card) == 16) && credit_card[0] == '4')
    {
        type = "VISA";
    }
    else if (strlen(credit_card) == 16 && (credit_card[1] - '0' >= 1 && credit_card[1] - '0' <= 5))
    {
        type = "MASTERCARD";
    }
    else
    {
        printf("INVALID\n");
        return 0;
    }

    int sum = 0;
    for (int i = strlen(credit_card) - 2; i >= 0; i -= 2)
    {
        int tmp_sum = (credit_card[i] - '0') * 2;
        sum += tmp_sum / 10;     // Add the tens place digit
        sum += tmp_sum % 10;     // Add the ones place digit
    }
    for (int i = strlen(credit_card) - 1; i >= 0; i -= 2)
    {
        sum += credit_card[i] - '0';
    }
    char arr[2];
    for (int j = 0; j < 2; j++)
    {
        arr[j] = 0;
    }
    sprintf(arr, "%d", sum);

    if (arr[strlen(arr) - 1] == '0')
    {
        printf("%s\n", type);
    }
    else
    {
        printf("INVALID\n");
        return 0;
    }
    return 0;
}