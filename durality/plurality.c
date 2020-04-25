#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);


int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    int ss = candidate_count;
    int result = 0;
    bool n = false;
    for (int i = 0; i < ss; i++)
    {
        result = strcmp(candidates[i].name, name);
        if (result == 0)
        {
            candidates[i].votes = candidates[i].votes + 1;
            n = true;
        }

    }
    return n;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    int ss = candidate_count;
    int n = 0;
    int x = 0;
    char *win[ss];
    int score;
    for (int i = 0; i < ss; i++)
    {
    	x = candidates[i].votes;
        if (x > score)
	    {
            win[i] = candidates[i].name;
	        score = x;
        }  
    } 
    for (int i = 0; i < ss; i++)
    {
        x = candidates[i].votes;
        if (x == score)
        {
            win[n] = candidates[i].name; 
            n = n + 1;
        }
    }
    for (int i = 0; i < n; i++)
    {
        printf("%s\n", win[i]); 
            
    }
        
}   


