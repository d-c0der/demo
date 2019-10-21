#include <cs50.h>
#include <stdio.h>
#include <crypt.h>
#include <string.h>

void rotate(int ltr_num, char kword[]);

int main(int argc, string argv[])
{
    // Check to see that only 1 argument was input
    if (argc != 2)
    {
        printf("Usage: ./task1 hash\n");
        return 1;
    }

    // Initialize password string
    char c1 = '@';
    char c2 = '\0';
    char c3 = '\0';
    char c4 = '\0';
    char c5 = '\0';
    char c6 = '\0';
    char pwdout[] = {c1, c2, c3, c4, c5, c6, '\0'};

    // Salt calculation
    char salt[] = {argv[1][0], argv[1][1], '\0'};

    // Probe for password
    for (int i = 0; i < 500000000; i++)
    {
        // Enrypt current password sample
        string hashout = crypt(pwdout, salt);

        if (strcmp(argv[1], hashout) == 0)
        {
            // Match found
            printf("%s\n", pwdout);
            return 0;
        }
        else
        {
            rotate(0, pwdout);
        }
    }

    printf("match not found - loop limit reached\n");
    return 1;
}

// Rotates first password letter by one and after full cycle (when it was'z')
// resets to 'A' and changes next letter(s) by 1
void rotate(int ltr_num, char kword[])
{
    kword[ltr_num] = kword[ltr_num] + 1;

    if (kword[ltr_num] == 91)
    {
        kword[ltr_num] = 97;
    }
    if (kword[ltr_num] == 123)
    {
        kword[ltr_num] = 65;
        rotate(ltr_num + 1, kword);

        // When new letter is initialized, its ascii value of 0 ('\0') changes to 1 (Start Of Heading)
        // and must be scrolled to 'A'
        if (kword[ltr_num + 1] == 1)
        {
            kword[ltr_num + 1] = 65;
        }
    }
}
