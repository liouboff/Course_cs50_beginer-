// The program name is caesar demonsrating method of encryption
// Using the comand line arguement (needs stdio.h lib)
// For correct start type "./caesar n" where n-value of key "
#include<stdio.h>
#include<stdlib.h>
#include<cs50.h>
#include<string.h>
#include<ctype.h>
//main function conteining key for encryption in the second argument
int main(int argc, string argv[])
{
//key validation for lack of key
//and forn too many arguments
    if (argc == 2)
    {
        int s = strlen(argv[1]); //second argument
        int i = 0;
        int d = 1;
        int dcheck = 0; //controll number
//validation for digits        
        do
        {
            char c = argv[1][i];  //digits of the second argument
            d = isdigit(c);      //function chacking for digits returning boolean expression
//loop chacking for digits returning boolean expression            
            if (d == 0)         
            {
                dcheck = dcheck + 1; //controll number
            }
            i = i + 1;
        }
        while (i <= s);
//validation for digits 
        if (dcheck == 1)
        {
            int key = atoi(argv[1]);            //reciving the key  
            string plain = get_string("plaintext:  ");  // getting string for encription
            int ss = strlen(plain);                 //function chacking lengh of recived string
            char cipher[ss];                     //determing leng of future cipher string
//srting for returning calculation if formula to alphabetic symbols uppercase letters  
            char ciupp[26] = {'N', 'O', 'P', 'Q', 'R', 'S', 'T', 
                              'U', 'V', 'W', 'X', 'Y', 'Z', 'A', 'B', 'C', 'D', 
                              'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M'
                             };
//srting for returning calculation if formula to alphabetic symbols lowcase letters                              
            char cilow[26] = {'h', 'i', 'j', 'k', 'l', 'm', 'n', 
                              'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 
                              'y', 'z', 'a', 'b', 'c', 'd', 'e', 'f', 'g'
                             };
            for (int ii = 0; ii <= ss; ii++)
            {
//applying formula for calculating encription                   
                int ci = key + plain[ii];
                ci = ci % 26;   
//separarion other simbols, copying with no changees                
                if (isalpha(plain[ii]) == 0)
                {
                    cipher[ii] = plain[ii];
                }
//uppercase letters                 
                else if (islower(plain[ii]) == 0)
                {
                    cipher[ii] = ciupp[ci]; 
                }
//lowcase letters                  
                else
                {
                    cipher[ii] = cilow[ci];
                }
            }
//returning calculated result of encryption            
            printf("ciphertext: %s\n", cipher); 
        } 
//validation for digits          
        else
        {
            printf("Usage: ./caesar key\n");
            return 1;       //in the case of error the program returning 1
        }
    }
//validation for too many arguments 
    else
    {
        printf("Usage: ./caesar key\n");
        return 1;     //in the case of error the program returning 1
    }
//in the case of the program returning correct return 0    
    return 0;
}