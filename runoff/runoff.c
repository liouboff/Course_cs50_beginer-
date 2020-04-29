#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    int ss = candidate_count;
    int st = voter_count;
    int result = 0;
    bool n = false;
    for (int l = 0; l < ss; l++) //runk
    {
        result = strcmp(candidates[l].name, name);
        if (result == 0)
        {
            preferences[voter][rank] = l;
            n = true;
        }
    }

    return n;
}
// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    int ss = candidate_count;
    int st = voter_count;
    for (int i = 0; i < st; i++)
    {   
       
        for (int l = 0; l < ss; l++)
        {
            if (candidates[l].eliminated == true)
            {   
                if (preferences[i][0] == l)
                {
                    int sr = candidate_count - 1;
                     
                    int r = 0;
                    int n = 1;
                    int s = 0;
                    do
                    {
                           
                        for (s = 0; s < sr; s++)
                        {
                            preferences[i][s] = preferences[i][n];
                            r = preferences[i][s];
                            n = n + 1;
                        }
                        sr = sr - 1;
                    }    
                    while ((preferences[i][s] == r) && (s == 0) && (candidates[r].eliminated == true));
                }
            }
        }
    }    
        
    for (int i = 0; i < st; i++)
    {   
        for (int l = 0; l < ss; l++)
        {        
            if (candidates[l].eliminated == false)
            {
                if (preferences[i][0] == l)
                {
                    candidates[l].votes = candidates[l].votes + 1;
                }
            } 
            else if (candidates[l].eliminated == true)
            {
                candidates[l].votes = 0;    
            }    
            
        }    
    }
   

    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    int ss = candidate_count;
    int st = voter_count;
    int sh = st / 2;
    bool n = false;
    for (int l = 0; l < ss; l++)
    {
        if (candidates[l].votes > sh)   //magority
        {
            printf("%s\n", candidates[l].name);
            n = true;
        }
    }

    return n;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    // TODO
    int ss = candidate_count;
    int min = voter_count; 
    for (int i = 0; i < ss; i++)
    {
        if (candidates[i].eliminated == false)
        {
            if (candidates[i].votes < min)
            {
                min = candidates[i].votes;
            }
        }    
    }
    return min;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{

    int ss = candidate_count;
    bool n = true;


    for (int i = 0; i < ss; i++)
    {
        if (candidates[i].eliminated == false)
        {
            if (candidates[i].votes != min) 
            {
                n = false;
            }
        }    
    }


    return n;
}

// Eliminate the candidate (or candidiates) in last place
void eliminate(int min)
{
    int ss = candidate_count;
    for (int i = 0; i < ss; i++)
    {
        if (candidates[i].votes == min)
        {
            candidates[i].eliminated = true;
        }
    }
    
}
