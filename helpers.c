#include "helpers.h"
#include <math.h>

int roundoff(float avg)//function to calculate roundoff
{
    int num = avg * 100;
    if (num % 100 >= 50)
    {
        avg += 1.0;
    }
    return (int)avg;//return integer value of roundedoff float
}
// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int i, j;
    int num;
    float avg;
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            avg = (image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0;
            num = roundoff(avg);
            image[i][j].rgbtRed = num;//set all to same value
            image[i][j].rgbtGreen = num;
            image[i][j].rgbtBlue = num;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int i, j;
    RGBTRIPLE temp;
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width / 2; j++)//travel upto half width and interchange pixels
        {
            temp = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE duplicate[height][width];//stores result until done
    int i, j, k, l;
    int totalPixel;
    int sum = 0;
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            //////////////////////////RED
            sum = 0;
            totalPixel = 9;
            for (k = -1; k < 2; k++)
            {
                for (l = -1; l < 2; l++)
                {
                    //satisfy matrix boundaries
                    if (i + k >= 0 && j + l >= 0 && i + k < height && j + l < width)
                    {
                        sum += image[i + k][j + l].rgbtRed;
                    }
                    else
                    {
                        totalPixel--;//at edges or corner
                    }
                }
            }
            duplicate[i][j].rgbtRed = roundoff(sum * 1.0 / totalPixel * 1.0);
            //////////////////////////////GREEN
            sum = 0;
            totalPixel = 9;
            for (k = -1; k < 2; k++)
            {
                for (l = -1; l < 2; l++)
                {
                    if (i + k >= 0 && j + l >= 0 && i + k < height && j + l < width)
                    {
                        sum += image[i + k][j + l].rgbtGreen;
                    }
                    else
                    {
                        totalPixel--;
                    }
                }
            }
            duplicate[i][j].rgbtGreen = roundoff(sum * 1.0 / totalPixel * 1.0);
            //////////////////////////////BLUE
            sum = 0;
            totalPixel = 9;
            for (k = -1; k < 2; k++)
            {
                for (l = -1; l < 2; l++)
                {
                    if (i + k >= 0 && j + l >= 0 && i + k < height && j + l < width)
                    {
                        sum += image[i + k][j + l].rgbtBlue;
                    }
                    else
                    {
                        totalPixel--;
                    }
                }
            }
            duplicate[i][j].rgbtBlue = roundoff(sum * 1.0 / totalPixel * 1.0);
        }
    }
    //image = duplicate;
    //assign image its new values
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            image[i][j] = duplicate[i][j];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};//Gx 2D Matrix
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};//Gy 2D Matrix
    int i, j, k, l;
    int sumGx, sumGy;//sum for Gx, Gy
    int G;
    RGBTRIPLE dup[height][width];//for storing result until done
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            /////////////////////RED
            sumGx = 0;
            sumGy = 0;
            for (k = -1; k < 2; k++)
            {
                for (l = -1; l < 2; l++)
                {
                    //Matrix boundry cindition check
                    if (i + k >= 0 && j + l >= 0 && i + k < height && j + l < width)
                    {
                        sumGx += image[i + k][j + l].rgbtRed * Gx[k + 1][l + 1];
                        sumGy += image[i + k][j + l].rgbtRed * Gy[k + 1][l + 1];
                    }
                    //else ignore(beyond edges or corners)
                }
            }
            G = roundoff(sqrt(pow(sumGx, 2) + pow(sumGy, 2)));//sqrt value store roundedoff in G
            if (G < 255)
            {
                dup[i][j].rgbtRed = G;
            }
            else//capp to 255
            {
                dup[i][j].rgbtRed = 255;
            }
            ///////////////////////GREEN
            sumGx = 0;
            sumGy = 0;
            for (k = -1; k < 2; k++)
            {
                for (l = -1; l < 2; l++)
                {
                    if (i + k >= 0 && j + l >= 0 && i + k < height && j + l < width)
                    {
                        sumGx += image[i + k][j + l].rgbtGreen * Gx[k + 1][l + 1];
                        sumGy += image[i + k][j + l].rgbtGreen * Gy[k + 1][l + 1];
                    }
                }
            }
            G = roundoff(sqrt(pow(sumGx, 2) + pow(sumGy, 2)));
            if (G < 255)
            {
                dup[i][j].rgbtGreen = G;
            }
            else
            {
                dup[i][j].rgbtGreen = 255;
            }
            ///////////////////////////BLUE
            sumGx = 0;
            sumGy = 0;
            for (k = -1; k < 2; k++)
            {
                for (l = -1; l < 2; l++)
                {
                    if (i + k >= 0 && j + l >= 0 && i + k < height && j + l < width)
                    {
                        sumGx += image[i + k][j + l].rgbtBlue * Gx[k + 1][l + 1];
                        sumGy += image[i + k][j + l].rgbtBlue * Gy[k + 1][l + 1];
                    }
                }
            }
            G = roundoff(sqrt(pow(sumGx, 2) + pow(sumGy, 2)));
            if (G < 255)
            {
                dup[i][j].rgbtBlue = G;
            }
            else
            {
                dup[i][j].rgbtBlue = 255;
            }
        }
    }
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            image[i][j] = dup[i][j];
        }
    }
    return;
}
