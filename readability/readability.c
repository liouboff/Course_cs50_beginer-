//The Coleman-Liau index of a text is designed to output what (U.S.) 
//grade level is needed to understand the text. 
#include<stdlib.h>
#include<cs50.h>
#include<ctype.h>
#include<string.h>
#include<math.h>
#include<stdio.h>

int main(void) 
{
//Reciving the string, text for evaluating.
    string b  = get_string("Text: ");
//assigning variables for chacking amounts of symbols
    int sym = strlen(b);
    int i = 0;
    int spa = 0;
    int upp = 0;
    int low = 0;
    int other = 0;
    int mark = 0;
//finding
    while (i <= sym)
    {   
        if (isblank(b[i]))
        {
            spa = spa + 1;
        }
        else if (isupper(b[i]))
        {
            upp = upp + 1;
        }
        else if (islower(b[i]))
        {
            low = low + 1;
        }
        else if ((b[i] == '!') || (b[i] == '?') || (b[i] == '.'))
        {
            mark = mark + 1;
        }
        
        i++;
    }
//assigning variables fo calculation in floating numbers    
    float fupp = upp;
    float flow = low;
    float fspa = spa + 1;
    float fmark = mark;
    float lett = fupp + flow;
    float lind = lett / fspa;
    lind = lind * 100;
    float sind = fspa / fmark;
    sind = 100 / sind; 
//Coleman-Liau index is computed using the formula:
//index = 0.0588 * L - 0.296 * S - 15.8    
    float nlind = lind * 0.0588;
    float nsind = sind * 0.296;
    float ind = nlind - nsind;         
    float find = ind - 15.8;
// Grade 
    int nnn = round(find); 
    if (find < 1)
    {
        printf("Before Grade 1\n");   
    }
    else if (find > 16)
    {
        printf("Grade 16+\n"); 
    }       
    else
    {
        printf("Grade %d\n", nnn);
    }
}
