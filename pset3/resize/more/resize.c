// Resizes a BMP file, argv[1] - foat from 0.0 to 100, argv[2] - source file name ( *.bmp) , argv[3] - outfile neme
// Implemented even distribution algorithm with controlling fix

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmp.h"

void write(int *counter, int limit, RGBTRIPLE *inptr, FILE *outptr);

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Wrong number of arguments\n");
        return 1;
    }

    // Check to see if only digits or decimal were typed
    for (int i = 0, n = strlen(argv[1]); i < n; i++)
    {
        if (argv[1][i] < 46 || argv[1][i] > 57 || argv[1][i] == 47)
        {
            printf("Scaling multiplier error, please input digits only");
            return 1;
        }
    }
    // Convert argv[1] to double, creating resize multiplier, and checking for its range
    double scale = atof(argv[1]);
    printf("scaling factor = %f\n", scale);

    if (scale <= 0 || scale > 100)
    {
        printf("Scaling multiplier error, please input positive value from 0.0 to 100\n");
        return 1;
    }


    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        printf("Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        printf("Unsupported file format.\n");
        return 4;
    }

    // Calculate new header parameters

    printf("bfSize = %i\n", bf.bfSize);
    printf("biWidth = %i\n", bi.biWidth);
    printf("biHeight = %i\n", bi.biHeight);
    printf("biSizeImage = %i\n", bi.biSizeImage);

    // Remember original padding
    int inpadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    printf("inpadding = %i\n", inpadding);

    // Remember original RGB table width
    int w1 = bi.biWidth;

    bi.biWidth = (LONG)(((double)bi.biWidth) * scale);
    if (bi.biWidth < 1)
    {
        bi.biWidth = 1;
    }

    // Remember original RGB table height
    int h1 = abs(bi.biHeight);

    // Remember if bi.biHeight is pos or neg
    int biHeightsign = 1;
    if (bi.biHeight < 0)
    {
        biHeightsign = -1;
    }

    bi.biHeight = (LONG)(((double)bi.biHeight) * scale);
    if (abs(bi.biHeight) < 1)
    {
        bi.biHeight = 1 * biHeightsign;
    }

    // Calculate padding for output file scanlines
    int outpadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    bi.biSizeImage = (DWORD)(((bi.biWidth * sizeof(RGBTRIPLE)) + outpadding) * abs(bi.biHeight));

    bf.bfSize = bi.biSizeImage + 54;

    // Display data for debug purpose
    printf("bfSize = %i\n", bf.bfSize);
    printf("biWidth = %i\n", bi.biWidth);
    printf("biHeight = %i\n", bi.biHeight);
    printf("biSizeImage = %i\n", bi.biSizeImage);
    printf("outpadding = %i\n", outpadding);

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // Prepare array to store no of copies of each original scanline
    int linecopy [h1];
    for (int i = 0; i < h1; i++)
    {
        linecopy[i] = 0;
    }

    int h2 = abs(bi.biHeight);
    printf("h2 = %i\n", h2);

    // Add floor(h2/h1) copies for each original line.
    for (int i = 0; i < h1; i++)
    {
        for (int j = 0, n = h2 / h1; j < n; j++)
        {
            linecopy[i]++;
        }
    }

    // Distribute remaining lines among linecopy[]

    // If resized h2 % original h1 == 0, don't do anything
    if ((h2 % h1) == 0)
    {}
    // If kv1 >= 2 use this - every kth original line we have to add extra copy
    else if ((h1 / (h2 % h1)) >= 2)
    {
        int kv1 = (h1 / (h2 % h1));
        printf("kv1 = %i\n", kv1);

        for (int i = 0; i < h1; i++)
        {
            if ((i + 1) % kv1 == 0)
            {
                linecopy[i]++;
            }
        }
    }

    // If kv1 < 2 use this - add extra copy to every original line except kth
    else
    {
        int kv2 = (h1 / (h1 - (h2 % h1)));
        printf("kv2 = %i\n", kv2);

        for (int i = 0; i < h1; i++)
        {
            if ((i + 1) % kv2 != 0)
            {
                linecopy[i]++;
            }
        }
    }

    // Prepare array to store no of copies of each source pixel in scanline
    int pixcopy [w1];
    for (int i = 0; i < w1; i++)
    {
        pixcopy[i] = 0;
    }

    // Add floor(bi.biWidth/w1) copies for each original pixel.
    for (int i = 0; i < w1; i++)
    {
        for (int j = 0, n = bi.biWidth / w1; j < n; j++)
        {
            pixcopy[i]++;
        }
    }

    // Distribute remaining pixels among pixcopy[]

    // If resized bi.biWidth % original bi.biWidth == 0, don't do anything
    if ((bi.biWidth % w1) == 0)
    {}
    // If k1 >= 2 use this - every kth original pixel we have to add extra copy
    else if ((w1 / (bi.biWidth % w1)) >= 2)
    {
        int k1 = (w1 / (bi.biWidth % w1));
        printf("k1 = %i\n", k1);

        for (int i = 0; i < w1; i++)
        {
            if ((i + 1) % k1 == 0)
            {
                pixcopy[i]++;
            }
        }
    }

    // If k1 < 2 use this - add extra copy to every original pixel except kth
    else
    {
        int k2 = (w1 / (w1 - (bi.biWidth % w1)));
        printf("k2 = %i\n", k2);

        for (int i = 0; i < w1; i++)
        {
            if ((i + 1) % k2 != 0)
            {
                pixcopy[i]++;
            }
        }
    }

    int count = 0; // Keeping track of pixels per line not to exceed bi.biWidth.

    int persistentcount = 0; // Persistent pixel counter for displaying pixels printed by algorythm

    int persistentcountfix = 0; // Persistent pixel counter for displaying total printed pixels after fix was applied

    int linecount = 0; // Keeping track of total lines printed.

    int linecountprefix = 0; // Keeping track of lines printed before fix applied.

    // iterate over infile's scanlines
    for (int iv = 0; iv < h1; iv++)
    {
        if (linecopy[iv] == 0)
        {
            fseek(inptr, (w1 * 3) + inpadding, SEEK_CUR);
        }


        for (int jv = 0; jv < linecopy[iv]; jv++)
        {
            if (linecount < h2)
            {
                // temporary storage
                RGBTRIPLE triple;

                // iterate over pixels in scanline
                for (int i = 0; i < w1; i++)
                {
                    // read RGB triple from infile
                    fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                    for (int j = 0; j < pixcopy[i]; j++)
                    {
                        write(&count, bi.biWidth, &triple, outptr);
                    }
                }
                // printf("line pixels counted: %i\n", count);
                persistentcount = count;

                // duplicate last pixel if algorithm failed
                while (count < bi.biWidth)
                {
                    write(&count, bi.biWidth, &triple, outptr);
                }

                // Reset pixel counter at line end
                persistentcountfix = count;
                count = 0;

                // Scroll back to beginnig of the line
                fseek(inptr, w1 * -3, SEEK_CUR);

                // Add output file padding if required
                for (int i = 0; i < outpadding; i++)
                {
                    fputc(0x00, outptr);
                }

                linecount++;

                linecountprefix = linecount;

                // Detect when new sorce line has to be scanned
                if (jv + 1 == linecopy[iv])
                {
                    // Scroll to end of the line
                    fseek(inptr, (w1 * 3) + inpadding, SEEK_CUR);
                }
            }
        }
    }

    // Duplicate last line if algorithm failed
    while (linecount < h2)
    {
        // temporary storage
        RGBTRIPLE triple;

        // Scroll back to beginnig of the line
        fseek(inptr, (w1 * -3) - inpadding, SEEK_CUR);

        // iterate over pixels in scanline
        for (int i = 0; i < w1; i++)
        {
            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            for (int j = 0; j < pixcopy[i]; j++)
            {
                write(&count, bi.biWidth, &triple, outptr);
            }
        }
        // printf("line pixels counted: %i\n", count);
        persistentcount = count;

        // duplicate last pixel if algorithm failed
        while (count < bi.biWidth)
        {
            write(&count, bi.biWidth, &triple, outptr);
        }

        // Reset pixel counter at line end
        persistentcountfix = count;
        count = 0;

        // Scroll over input file padding
        fseek(inptr, inpadding, SEEK_CUR);

        // Add output file padding if required
        for (int i = 0; i < outpadding; i++)
        {
            fputc(0x00, outptr);
        }

        linecount++;
    }



    for (int i = 0; i < w1; i++)
    {
        printf("%i'th pixel copies: %i\n", i + 1, pixcopy[i]);
    }
    for (int i = 0; i < h1; i++)
    {
        printf("%i'th line copies: %i\n", i + 1, linecopy[i]);
    }

    printf("pixels printed by algorithm = %i\n", persistentcount);
    printf("pixel printed after fix = %i\n", persistentcountfix);
    printf("pixels printed by algorithm = %i\n", linecountprefix);
    printf("total printed lines = %i\n", linecount);




    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}

//write(&count, &bi.biWidth, &triple, outptr)

// write RGB triple to outfile
void write(int *counter, int limit, RGBTRIPLE *inptr, FILE *outptr)
{
    if (*counter < limit)
    {
        fwrite(inptr, sizeof(RGBTRIPLE), 1, outptr);
        *counter = *counter + 1;
    }
}