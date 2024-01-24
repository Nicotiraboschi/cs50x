// Practice working with structs
// Practice applying sorting algorithms

#include <cs50.h>
#include <stdio.h>

#define NUM_CITIES 10

typedef struct
{
    string city;
    int temp;
}
avg_temp;

avg_temp temps[NUM_CITIES];

void sort_cities(void);

int main(void)
{
    temps[0].city = "Austin";
    temps[0].temp = 97;

    temps[1].city = "Boston";
    temps[1].temp = 82;

    temps[2].city = "Chicago";
    temps[2].temp = 85;

    temps[3].city = "Denver";
    temps[3].temp = 90;

    temps[4].city = "Las Vegas";
    temps[4].temp = 105;

    temps[5].city = "Los Angeles";
    temps[5].temp = 82;

    temps[6].city = "Miami";
    temps[6].temp = 97;

    temps[7].city = "New York";
    temps[7].temp = 85;

    temps[8].city = "San Francisco";
    temps[8].temp = 66;

    temps[9].city = "Phoenix";
    temps[9].temp = 107;


    sort_cities();

    printf("\nAverage July Temperatures by City\n\n");

    for (int i = 0; i < NUM_CITIES; i++)
    {
        printf("%s: %i\n", temps[i].city, temps[i].temp);
    }
}


// TODO: Sort cities by temperature in descending order
// INSERTION SORT

/* int i = 1;
avg_temp checked;

void sort_cities(void)
    {
        if (i < NUM_CITIES)
        {
            checked = temps[i];
            for (int n = 1; n <= i; n++)
            {
                if (checked.temp > temps[i-n].temp)
                {
                    temps[i-n+1] = temps [i-n];
                    temps [i-n] = checked;
                }

            }
            i++;
            sort_cities();
        }
    } */




// TODO: Sort cities by temperature in descending order:
//SELECTION SORT

/* avg_temp big;
int i = 0;
void sort_cities(void)
{
    if (i < NUM_CITIES)
    {
        int h = i;
        big = temps[i];
        for (int n = i; n < NUM_CITIES; n++)
        {
            if (temps[n+1].temp > big.temp)
            {
                h = n + 1;
                big = temps[h];
            }
        }
        temps[h] = temps[i];
        temps[i] = big;
        i++;
        sort_cities();
    }
} */

avg_temp small;
int num = NUM_CITIES;

//BUBBLE SORT
void sort_cities(void)
{
    if (num > 0)
    {
        for (int n = 0; n + 1 < num; n++)
        {
            if (temps[n].temp < temps[n+1].temp)
            {
                small = temps[n];
                temps[n] = temps[n+1];
                temps[n+1] = small;
            }
        }
        num--;
        sort_cities();
    }
}