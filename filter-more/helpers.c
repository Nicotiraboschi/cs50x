#include "helpers.h"
#include <math.h>
#include <stdlib.h>


// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int avg_colors = round((float)(image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3);
            image[i][j].rgbtBlue = avg_colors;
            image[i][j].rgbtGreen = avg_colors;
            image[i][j].rgbtRed = avg_colors;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE(*cornice)[width + 2] = calloc(height + 2, (width + 2) * sizeof(RGBTRIPLE));
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            cornice[i + 1][j + 1] = image[i][j];
        }
    }


    // MIDDLE
    for (int i = 1; i < height + 1; i++)
    {
        for (int j = 1; j < width + 1; j++)
        {
            int sumB = 0;
            int sumG = 0;
            int sumR = 0;
            for (int n = i - 1; n < i + 2; n++)
            {
                for (int m = j - 1; m < j + 2; m++)
                {
                    sumB += cornice[n][m].rgbtBlue;
                    sumG += cornice[n][m].rgbtGreen;
                    sumR += cornice[n][m].rgbtRed;
                }
            }
            if (i > 1 && j > 1 && i < height && j < width)
            {
                image[i - 1][j - 1].rgbtBlue = round((float)sumB / 9);
                image[i - 1][j - 1].rgbtGreen = round((float)sumG / 9);
                image[i - 1][j - 1].rgbtRed = round((float)sumR / 9);
            }
            else if ((i == 1 && j == 1) || (i == 1 && j == width) || (i == height && j == 1) || (i == height && j == width))
            {
                image[i - 1][j - 1].rgbtBlue = round((float)sumB / 4);
                image[i - 1][j - 1].rgbtGreen = round((float)sumG / 4);
                image[i - 1][j - 1].rgbtRed = round((float)sumR / 4);
            }
            else
            {
                image[i - 1][j - 1].rgbtBlue = round((float)sumB / 6);
                image[i - 1][j - 1].rgbtGreen = round((float)sumG / 6);
                image[i - 1][j - 1].rgbtRed = round((float)sumR / 6);
            }
        }
    }
    free(cornice);
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE(*cornice)[width + 2] = calloc(height + 2, (width + 2) * sizeof(RGBTRIPLE));
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            cornice[i + 1][j + 1] = image[i][j];
        }
    }

    // MIDDLE

    for (int i = 1; i < height + 1; i++)
    {
        for (int j = 1; j < width + 1; j++)
        {
            int sumYB = 0;
            int sumYG = 0;
            int sumYR = 0;
            int sumXB = 0;
            int sumXG = 0;
            int sumXR = 0;
            int sqrtB;
            int sqrtG;
            int sqrtR;
            for (int m = j - 1; m < j + 2; m++)
            {
                for (int n = i - 1; n < i + 2; n++)
                {
                    int x;
                    int y;
                    x = ((m == j) ? 2 : 1);
                    y = ((n < i) ? -1 : (n == i ? 0 : 1));
                    sumYB += cornice[n][m].rgbtBlue * x * y;
                    sumYG += cornice[n][m].rgbtGreen * x * y;
                    sumYR += cornice[n][m].rgbtRed * x * y;

                    x = ((n == i ? 2 : 1));
                    y = ((m < j) ? -1 : ((m == j) ? 0 : 1));
                    sumXB += cornice[n][m].rgbtBlue * x * y;
                    sumXG += cornice[n][m].rgbtGreen * x * y;
                    sumXR += cornice[n][m].rgbtRed * x * y;
                }
            }
            sqrtB = (int)round(sqrt((float)sumYB * sumYB + sumXB * sumXB));
            sqrtG = (int)round(sqrt((float)sumYG * sumYG + sumXG * sumXG));
            sqrtR = (int)round(sqrt((float)sumYR * sumYR + sumXR * sumXR));

            image[i - 1][j - 1].rgbtBlue = (sqrtB > 255 ? 255 : sqrtB);
            image[i - 1][j - 1].rgbtGreen = (sqrtG > 255 ? 255 : sqrtG);
            image[i - 1][j - 1].rgbtRed = (sqrtR > 255 ? 255 : sqrtR);
        }
    }
    free(cornice);
    return;
}

/* //(EDGES FUNCTION)
MIDDLE EDGES:sumXB = (cornice[i - 1][j - 1].rgbtBlue *-1 + cornice[i - 1][j+1].rgbtBlue *1 + cornice[i][j - 1].rgbtBlue *-2 + cornice[i][j+1].rgbtBlue *2 + cornice[i+1][j - 1].rgbtBlue *-1 + cornice[i+1][j+1].rgbtBlue *1);
            sumXG = (cornice[i - 1][j - 1].rgbtGreen *-1 + cornice[i - 1][j+1].rgbtGreen *1 + cornice[i][j - 1].rgbtGreen *-2 + cornice[i][j+1].rgbtGreen *2 + cornice[i+1][j - 1].rgbtGreen *-1 + cornice[i+1][j+1].rgbtGreen *1);
            sumXR = (cornice[i - 1][j - 1].rgbtRed *-1 + cornice[i - 1][j+1].rgbtRed *1 + cornice[i][j - 1].rgbtRed *-2 + cornice[i][j+1].rgbtRed *2 + cornice[i+1][j - 1].rgbtRed *-1 + cornice[i+1][j+1].rgbtRed *1);

            sumYB = (cornice[i - 1][j - 1].rgbtBlue *-1 + cornice[i - 1][j].rgbtBlue *-2 + cornice[i - 1][j+1].rgbtBlue *-1 + cornice[i+1][j - 1].rgbtBlue *1 + cornice[i+1][j].rgbtBlue *2 + cornice[i+1][j+1].rgbtBlue *1);
            sumYG = (cornice[i - 1][j - 1].rgbtGreen *-1 + cornice[i - 1][j].rgbtGreen *-2 + cornice[i - 1][j+1].rgbtGreen *-1 + cornice[i+1][j - 1].rgbtGreen *1 + cornice[i+1][j].rgbtGreen *2 + cornice[i+1][j+1].rgbtGreen *1);
            sumYR = (cornice[i - 1][j - 1].rgbtRed *-1 + cornice[i - 1][j].rgbtRed *-2 + cornice[i - 1][j+1].rgbtRed *-1 + cornice[i+1][j - 1].rgbtRed *1 + cornice[i+1][j].rgbtRed *2 + cornice[i+1][j+1].rgbtRed *1);
 */
/*
CORNERS LEFT
   for (int i = 1; i < height-1; i++)
   {
       sumXB = (image[i - 1][0].rgbtBlue *-1 + image[i][0].rgbtBlue *-2 + image[i+1][0].rgbtBlue * -1);
       sumXG = (image[i - 1][0].rgbtGreen *-1 + image[i][0].rgbtGreen *-2 + image[i+1][0].rgbtGreen * -1);
       sumXR = (image[i - 1][0].rgbtRed *-1 + image[i][0].rgbtRed *-2 + image[i+1][0].rgbtRed * -1);

       sumYB = (image[i - 1][0].rgbtBlue *-1 + image[i - 1][1].rgbtBlue *-2 + image[i+1][0].rgbtBlue + image[i+1][1].rgbtBlue *2);
       sumYG = (image[i - 1][0].rgbtGreen *-1 + image[i - 1][1].rgbtGreen *-2 + image[i+1][0].rgbtGreen + image[i+1][1].rgbtGreen *2);
       sumYR = (image[i - 1][0].rgbtRed *-1 + image[i - 1][1].rgbtRed *-2 + image[i+1][0].rgbtRed + image[i+1][1].rgbtRed *2);

       edges[i][0].rgbtBlue = (int)round(sqrt((float)(sumYB*sumYB + sumXB*sumXB)));
       edges[i][0].rgbtGreen = (int)round(sqrt((float)(sumYG*sumYG + sumXG*sumXG)));
       edges[i][0].rgbtRed = (int)round(sqrt((float)(sumYR*sumYR + sumXR*sumXR)));
   }

  // CORNERS RIGHT
  for (int i = 1; i < height-1; i++)
  {
       sumXB = (image[i - 1][width-2].rgbtBlue *-1 + image[i][width-2].rgbtBlue *-2 + image[i+1][width-2].rgbtBlue *-1);
       sumXG = (image[i - 1][width-2].rgbtGreen *-1 + image[i][width-2].rgbtGreen *-2 + image[i+1][width-2].rgbtGreen *-1);
       sumXR = (image[i - 1][width-2].rgbtRed *-1 + image[i][width-2].rgbtRed *-2 + image[i+1][width-2].rgbtRed *-1);

       sumYB = (image[i - 1][width-2].rgbtBlue *-2 + image[i+1][width-2].rgbtBlue *2 + image[i - 1][width-1].rgbtBlue *-1 + image[i+1][width-1].rgbtBlue);
       sumYG = (image[i - 1][width-2].rgbtGreen *-2 + image[i+1][width-2].rgbtGreen *2 + image[i - 1][width-1].rgbtGreen *-1 + image[i+1][width-1].rgbtGreen);
       sumYR = (image[i - 1][width-2].rgbtRed *-2 + image[i+1][width-2].rgbtRed *2 + image[i - 1][width-1].rgbtRed *-1 + image[i+1][width-1].rgbtRed);

       edges[i][width-1].rgbtBlue = (int)round(sqrt((float)(sumYB*sumYB + sumXB*sumXB)));
       edges[i][width-1].rgbtGreen = (int)round(sqrt((float)(sumYG*sumYG + sumXG*sumXG)));
       edges[i][width-1].rgbtRed = (int)round(sqrt((float)(sumYR*sumYR + sumXR*sumXR)));
   }

   //CORNERS UP
       for (int j = 1; j < width - 1; j++)
       {
           sumYB = 0;
           sumYG = 0;
           sumYR = 0;
           sumXB = 0;
           sumXG = 0;
           sumXR = 0;
                   for (int m = j - 1; m < j + 2; m++)
                   {
                       int x;
                       x = ((m == j) ? - 2 : -1);
                       sumYB += image[0][m].rgbtBlue *x;
                       sumYG += image[0][m].rgbtGreen *x;
                       sumYR += image[0][m].rgbtRed *x;

                       for (int n = 0; n < 2; n++)
                       {
                           int y;
                           y = ((m < j) ? -1 : 1);
                           x = ((m != j) ? ((n == 0) ? 1 : 2) : 0);
                           sumXB += image[n][m].rgbtBlue *x *y;
                           sumXG += image[n][m].rgbtGreen *x *y;
                           sumXR += image[n][m].rgbtRed *x *y;
                       }
                   }
       edges[0][j].rgbtBlue = (int)round(sqrt((float)(sumYB*sumYB + sumXB*sumXB)));
       edges[0][j].rgbtGreen = (int)round(sqrt((float)(sumYG*sumYG + sumXG*sumXG)));
       edges[0][j].rgbtRed = (int)round(sqrt((float)(sumYR*sumYR + sumXR*sumXR)));
       }
   //CORNERS DOWN
       for (int j = 1; j != width -1 ; j++)
       {
           sumYB = 0;
           sumYG = 0;
           sumYR = 0;
           sumXB = 0;
           sumXG = 0;
           sumXR = 0;
                   for (int m = j - 1; m < j + 2; m++)
                   {
                       int x;
                       x = ((m == j) ? 2 : 1);
                       sumYB += image[height-1][m].rgbtBlue *x;
                       sumYG += image[height-1][m].rgbtGreen *x;
                       sumYR += image[height-1][m].rgbtRed *x;

                       for (int n = height-2; n < height; n++)
                       {
                           int y;
                           y = ((m < j) ? -1 : 1);
                           x = ((m != j) ? ((n == height-1) ? 1 : 2) : 0);
                           sumXB += image[n][m].rgbtBlue *x *y;
                           sumXG += image[n][m].rgbtGreen *x *y;
                           sumXR += image[n][m].rgbtRed *x *y;
                       }
                   }
               edges[height-1][j].rgbtBlue = (int)round(sqrt((float)(sumYB*sumYB + sumXB*sumXB)));
               edges[height-1][j].rgbtGreen = (int)round(sqrt((float)(sumYG*sumYG + sumXG*sumXG)));
               edges[height-1][j].rgbtRed = (int)round(sqrt((float)(sumYR*sumYR + sumXR*sumXR)));
           }
*/

/*     for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                image[i][j] = edges[i][j];
            }
        }
    return;
}
 */

/*
    // CORNERS UP AND DOWN
    for (int j = 1; j < width-1; j++)
    {
        int sumBlue = 0;
        int sumGreen = 0;
        int sumRed = 0;
        for (int n = 0; n < 2; n++)
                {
                    for (int m = j - 1; m < j+2; m++)
                    {
                        sumBlue += image[n][m].rgbtBlue;
                        sumGreen += image[n][m].rgbtGreen;
                        sumRed += image[n][m].rgbtRed;
                    }
                }
                blurred[0][j].rgbtBlue = round((float)(sumBlue)/6);
                blurred[0][j].rgbtGreen = round((float)(sumGreen)/6);
                blurred[0][j].rgbtRed = round((float)(sumRed)/6);
                // DOWN
        sumBlue = 0;
        sumGreen = 0;
        sumRed = 0;
        for (int n = height-2; n < height; n++)
                {
                    for (int m = j - 1; m < j+2; m++)
                    {
                        sumBlue += image[n][m].rgbtBlue;
                        sumGreen += image[n][m].rgbtGreen;
                        sumRed += image[n][m].rgbtRed;
                    }
                }
                blurred[height-1][j].rgbtBlue = round((float)(sumBlue)/6);
                blurred[height-1][j].rgbtGreen = round((float)(sumGreen)/6);
                blurred[height-1][j].rgbtRed = round((float)(sumRed)/6);
    }

            // LEFT
    for (int i = 1; i < height-1; i++)
    {
        int sumBlue = 0;
        int sumGreen = 0;
        int sumRed = 0;
        for (int n = i - 1; n < i+2 ; n++)
                {
                    for (int m = 0; m < 2; m++)
                    {
                        sumBlue += image[n][m].rgbtBlue;
                        sumGreen += image[n][m].rgbtGreen;
                        sumRed += image[n][m].rgbtRed;
                    }
                }
        blurred[i][0].rgbtBlue = round((float)(sumBlue)/6);
        blurred[i][0].rgbtGreen = round((float)(sumGreen)/6);
        blurred[i][0].rgbtRed = round((float)(sumRed)/6);

                // RIGHT
        sumBlue = 0;
        sumGreen = 0;
        sumRed = 0;
        for (int n = i - 1; n < i+2; n++)
                {
                    for (int m = width-2; m < width; m++)
                    {
                        sumBlue += image[n][m].rgbtBlue;
                        sumGreen += image[n][m].rgbtGreen;
                        sumRed += image[n][m].rgbtRed;
                    }
                }
                blurred[i][width-1].rgbtBlue = round((float)(sumBlue)/6);
                blurred[i][width-1].rgbtGreen = round((float)(sumGreen)/6);
                blurred[i][width-1].rgbtRed = round((float)(sumRed)/6);
    }

    // EDGES (BLURRED FUNCTION)

    // TOP LEFT
    blurred[0][0].rgbtBlue = round((float)(image[0][0].rgbtBlue + image[0][1].rgbtBlue + image[1][0].rgbtBlue + image[1][1].rgbtBlue)/4);
    blurred[0][0].rgbtGreen = round((float)(image[0][0].rgbtGreen + image[0][1].rgbtGreen + image[1][0].rgbtGreen + image[1][1].rgbtGreen)/4);
    blurred[0][0].rgbtRed = round((float)(image[0][0].rgbtRed + image[0][1].rgbtRed + image[1][0].rgbtRed + image[1][1].rgbtRed)/4);

    // TOP RIGHT
    blurred[0][width-1].rgbtBlue = round((float)(image[0][width-2].rgbtBlue + image[0][width-1].rgbtBlue + image[1][width-1].rgbtBlue + image[1][width-2].rgbtBlue)/4);
    blurred[0][width-1].rgbtGreen = round((float)(image[0][width-2].rgbtGreen + image[0][width-1].rgbtGreen + image[1][width-1].rgbtGreen + image[1][width-2].rgbtGreen)/4);
    blurred[0][width-1].rgbtRed = round((float)(image[0][width-2].rgbtRed + image[0][width-1].rgbtRed + image[1][width-1].rgbtRed + image[1][width-2].rgbtRed)/4);

    // BOTTOM LEFT
    blurred[height-1][0].rgbtBlue = round((float)(image[height-2][0].rgbtBlue + image[height-2][1].rgbtBlue + image[height-1][0].rgbtBlue + image[height-1][1].rgbtBlue)/4);
    blurred[height-1][0].rgbtGreen = round((float)(image[height-2][0].rgbtGreen + image[height-2][1].rgbtGreen + image[height-1][0].rgbtGreen + image[height-1][1].rgbtGreen)/4);
    blurred[height-1][0].rgbtRed = round((float)(image[height-2][0].rgbtRed + image[height-2][1].rgbtRed + image[height-1][0].rgbtRed + image[height-1][1].rgbtRed)/4);

    // BOTTOM RIGHT
    blurred[height-1][width-1].rgbtBlue = round((float)(image[height-2][width-2].rgbtBlue + image[height-2][width-1].rgbtBlue + image[height-1][width-1].rgbtBlue + image[height-1][width-2].rgbtBlue)/4);
    blurred[height-1][width-1].rgbtGreen = round((float)(image[height-2][width-2].rgbtGreen + image[height-2][width-1].rgbtGreen + image[height-1][width-1].rgbtGreen + image[height-1][width-2].rgbtGreen)/4);
    blurred[height-1][width-1].rgbtRed = round((float)(image[height-2][width-2].rgbtRed + image[height-2][width-1].rgbtRed + image[height-1][width-1].rgbtRed + image[height-1][width-2].rgbtRed)/4);

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = blurred[i][j];
        }
    }

    free(blurred);
    */