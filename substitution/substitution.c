// The program name is substitution  demonsrating method of encryption
// Using the comand line arguement (needs stdio.h lib)
// For correct start type "./substitution n" where n-value of key "
#include<stdio.h>
#include<stdlib.h>
#include<cs50.h>
#include<string.h>
#include<ctype.h>
//main function containing the key for encryption in the second argument
//the argument can't contain digits or other type of symbols
//also can not be more or less then one key
int main(int argc, string argv[])
{
//first key validation for lack of key
//or too many arguments
    if (argc == 2)
    {
        int s = strlen(argv[1]); //lengh of the second argument
        if (s == 26) //second key validation for amount of charakters
        {
            int i = 0;
            int d = 1;
            int dcheck = 0; //controll number
//Key must not contain repeated characters.
//Key must only contain alphabetic characters.
//validation for key characters
            do
            {
                char c = argv[1][i];  //symbols of the second argument
                d = isalpha(c); //function chacking for alphabetic symbols returns boolean expression
//loop chacking for alphabetic symbols returning boolean expression
                if (d == 0)
                {
                    dcheck = dcheck + 1; //controll number
                }
                i = i + 1;
            }
            while (i <= s);
//third validation for symbols
            if (dcheck == 1)
            {
//srting for returning calculation if formula to alphabetic symbols uppercase letters
                char ciruppkey[26] = {'A', 'B', 'C', 'D', 'E', 'F', 'G',
                                      'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q',
                                      'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'
                                     };
//srting for returning calculation if formula to alphabetic symbols lowercase letters
                char cirlowkey[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g',
                                      'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q',
                                      'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'
                                     };
                int iv = 0;
                char loargv[26]; //string only in upper case symbols
                while (iv <= 26) 
                {  
                    loargv[iv] = tolower(argv[1][iv]);
                    iv++;
                } 
                int il = 0;
                char upargv[26]; //string only in upper case symbols
                while (il <= 26) 
                {  
                    upargv[il] = toupper(argv[1][il]);
                    il++;
                } 
                int con = 1;
                for (int ik = 0; ik <= 26; ik++)
                {   
                    int x = 0;
                    char z = ciruppkey[ik];
                    for (int ic = 0; ic <= 26; ic ++)
                    {
                        char w = upargv[ic];
                        if (z == w)
                        {
                            x =  x + 1;
                        }
                    }
                    if (x != 1) 
                    {
                        con = 0;
                    }
                }
                if (con == 1)
                {
                    string plain = get_string("plaintext:  ");  // getting string for encription
                    int ss = strlen(plain);                 //function chacking lengh of recived string
                    char cipcip[ss];                     //determing leng of future cipher string
                    char du = 0;
                    char nu = 0;
                    char fu = 0;
                    for (int im = 0; im <= ss; im++)
                    {
                        char u = plain[im];
                        for (int id = 0; id <= 26; id++)
                        {
                            char j = ciruppkey[id];
                            char o = cirlowkey[id];
                            char q = upargv[id];
                            char g = loargv[id];
                            if (u == j)
                            {
                                du = q;  
                            }
                            else if (u == o) 
                            {
                                nu = g;
                            }
                            else 
                            {
                                fu = u;
                            }
                        }
                        cipcip[im] = du;
// separarion other simbols, copying with no changees
                        if (isalpha(plain[im]) == 0)
                        {
                            cipcip[im] = fu;
                        }
//uppercase letters
                        else if (islower(plain[im]) == 0) 
                        {
                            cipcip[im] = du;
                        }
//lowcase letters
                        else
                        {
                            cipcip[im] = nu;
                        }
                    }
                    printf("ciphertext: %s\n", cipcip);
                }
                else
                {
                    printf("Key must not contain repeated characters.\n");
                    return 1;
                }
            }
//validation for alphabetic charactres
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
//validation for too many arguments
    else
    {
        printf("Usage: ./substitution key\n");
        return 1;     //in the case of error the program returning 1
    }
//in the case of the program returning correct return 0
    return 0;
}