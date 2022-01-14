//program for encryption of text, no changing letter case, and non letter symbols.
//start the program from command line argument ./substitution n (where n 26 letters of alphabet free order and free case)
//prigram will prompt you with input text for encryption
#include<stdio.h>
#include<stdlib.h>
#include<cs50.h>
#include<string.h>
#include<ctype.h>

int main(int argc, string argv[])
{
    if (argc == 2)// 1. check amount of arguments
    {
        int s = strlen(argv[1]); // lengtth of the second argument
        if (s == 26) // 2. second key validation for amount of charakters
        {
            int dcheck = 0; //controll number
            for (int i = 0; i < s; i++)
            {
                int d = isalpha(argv[1][i]);
                if (d == 0)
                {
                    dcheck = dcheck + 1;
                }
            }
            if (dcheck == 0)// 3. chesk for alphabetic simbols
            {
                // alphabetic symbols uppercase letters
                char ciruppkey[26] = {'A', 'B', 'C', 'D', 'E', 'F', 'G',
                                      'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q',
                                      'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'
                                     };
                // alphabetic symbols lowercase letters
                char cirlowkey[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g',
                                      'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q',
                                      'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'
                                     };
                int lcheck[26] = {};
                int fcheck = 0;
                char high[26];
                for (int x = 0; x < 26; x++)
                {
                    high[x] = toupper(argv[1][x]);
                }
                for (int i = 0; i < 26; i++)
                {
                    for (int j = 0; j < 26; j++)
                    {
                        if (ciruppkey[i] == high[j])
                        {
                            lcheck[i] = lcheck[i] + 1;
                        }
                        else
                        {
                            lcheck[i] = lcheck[i];
                        }
                    }
                }
                for (int i = 0; i < 26; i ++)
                {
                    if (lcheck[i] != 1)
                    {
                        fcheck = 1;
                    }
                }
                if (fcheck == 0)  // 4. check for repeating letters
                {
                    string plain = get_string("plaintext:  ");  // getting string for encription
                    int ss = strlen(plain);                 // chacking lengh of recived string
                    char low[26] = {};
                    char cifer[ss];
                    for (int x = 0; x < 26; x++)
                    {
                        low[x] = tolower(argv[1][x]);
                    }
                    for (int i = 0; i <= ss; i++)
                    {
                        if (isalpha(plain[i]) == 0)
                        {
                            cifer[i] = plain[i];
                        }
                        else if (islower(plain[i]) == 0)
                        {
                            for (int x = 0; x < 26; x++)
                            {
                                if (plain[i] == ciruppkey[x])
                                {
                                    cifer[i] = high[x];
                                }
                            }
                        }
                        else
                        {
                            for (int y = 0; y < 26; y++)
                            {
                                if (plain[i] == cirlowkey[y])
                                {
                                    cifer[i] = low[y];
                                }
                            }
                        }
                    }
                    printf("ciphertext: %s\n", cifer);
                }
                else
                {
                    printf("Key must not contain repeated characters.\n");
                    return 1;   //in the case of error the program returning 1
                }
            }
            else
            {
                printf("Key must only contain alphabetic characters.\n");
                return 1;       //in the case of error the program returning 1
            }
        }
        else
        {
            printf("Key must contain 26 characters.\n");
            return 1;       //in the case of error the program returning 1
        }
    }
    else
    {
        printf("Usage: ./substitution key\n");
        return 1;     //in the case of error the program returning 1
    }
}
