#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("wrong argument count\n");
        return 1;
    }

    // Creating buffer to store data
    unsigned char buffer[513];
    for (int i = 0; i < 513; i++)
    {
        buffer[i] = '\0';
    }

    // Open card memory file
    FILE *image = fopen(argv[1], "r");
    if (!image)
    {
        printf("Error reading file\n");
        return 2;
    }
    int jpgcounter = 0;
    int firstfound = 0;
    char filename[] = "000.jpg";
    FILE *output = NULL;
    while (fread(buffer, 512, 1, image)) // Repeat till read returns 0 - EOF reached
    {

        // If found jpg beginning
        if (buffer[0] == 0xff && buffer[1] == 0xd8
            && buffer[2] == 0xff &&
            buffer[3] > 0xdf && buffer[3] < 0xf0)
        {

            if (firstfound == 0) // If Jpg start was not encountered before
            {
                output = fopen(filename, "w");
                if (!output)
                {
                    printf("Error writing jpg file\n");
                    return 3;
                }
                fwrite(buffer, 512, 1, output);
                firstfound = 1;
            }
            else // If jpg start was encountered before
            {
                fclose(output);
                jpgcounter++;
                sprintf(filename, "%03i.jpg", jpgcounter);
                output = fopen(filename, "w");
                if (!output)
                {
                    printf("Error writing jpg file\n");
                    return 3;
                }
                fwrite(buffer, 512, 1, output);
            }
        }
        // If jpg start not detected
        else
        {
            if (firstfound == 0) // If Jpg start was not encountered before
            {

            }
            else // If jpg start was encountered before
            {
                fwrite(buffer, 512, 1, output);

            }
        }
    }
    fclose(image);
    fclose(output);
    printf("recovery complete\n");
}

