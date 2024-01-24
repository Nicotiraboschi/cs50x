#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 10

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1; //it is the number of candidates
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
        // it is putting each name into the array
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;

    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);
            //nome del votato in i position

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}
// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
// TODO
{

    for (int n = 0; n < candidate_count; n++)
    {
        if (!strcmp(name, candidates[n]))
        {
            ranks[rank] = n;
            return true;
        }
        else
        {
            continue;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    // preferences[i][j]
    for (int j = 0; j < candidate_count; j++)
    {
        for (int i = j + 1; i < candidate_count; i++)
        {
            preferences[ranks[j]][ranks[i]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    int n = 0;
    for (int j = 0; j < candidate_count; j++)
    {
        for (int i = j + 1; i - 1 < candidate_count; i++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[n].winner = i;
                pairs[n].loser = j;
                n++;
            }
            else if (preferences[j][i] > preferences[i][j])
            {
                pairs[n].winner = j;
                pairs[n].loser = i;
                n++;
            }
            else
            {
                continue;
            }
        }
    }
    pair_count = n;
    return;
}

int lost;
int m;
// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO
    int strength[pair_count];
    for (int n = 0; n < pair_count; n++)
    {
        strength[n] = preferences[pairs[n].winner][pairs[n].loser] - preferences[pairs[n].loser][pairs[n].winner];
    }
    for (int h = 0; h < pair_count; h++)
    {
        int big = strength[h];
        pair big_pair = pairs[h];
        int index = h;
        for (int n = h; n + 1 < pair_count; n++)
        {
            if (big < strength[n + 1])
            {
                big = strength[n + 1];
                index = n + 1;
            }
        }
        strength[h] = strength[index];
        strength[index] = big;
        pairs[h] = pairs[index];
        pairs[index] = big_pair;
    }
    lost = pairs[0].loser;
    m = 0;
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles

int new_lost = MAX;
int n = -1;
int arr[MAX];

void lock_pairs(void)
{
    // TODO
    if (m < pair_count)
    {
        locked[pairs[m].winner][pairs[m].loser] = true;
        for (int i = 0; i < candidate_count; i++)
        {
            if (locked[lost][i] == true)
                if (i == pairs[m].winner)
                {
                    locked[pairs[m].winner][pairs[m].loser] = false;
                    m++;
                    lost = pairs[m].loser;
                    n = -1;
                    lock_pairs();
                    return;
                }
                else
                {
                    n++;
                    arr[n] = i;
                    int old = lost;
                    lost = i;
                    lock_pairs();
                    lost = old;
                }
            else
            {
                if (n != -1 && arr[n] < candidate_count - 1)
                {
                    arr[n]++;
                    continue;
                }
                else
                {
                    continue;
                }
            }
        }
        if (n != -1)
        {
            n--;
            return;
        }
        else
        {
            m++;
            lost = pairs[m].loser;
            lock_pairs();
        }
    }
    return;
}

int col = 0;
// Print the winner of the election
void print_winner(void)
{
    // TODO
    if (col < candidate_count)
    {
        for (int i = 0; i < candidate_count; i++)
        {
            if (locked[i][col] == true)
            {
                col++;
                print_winner();
                return;
            }
            else
            {
                continue;
            }
        }
        printf("%s\n", candidates[col]);
        return;
    }
}