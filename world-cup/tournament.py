# Simulate a sports tournament

import csv
import sys
import random
import math
import time

# Number of simluations to run
N = 1000


def main():

    # Ensure correct usage
    if len(sys.argv) != 2:
        sys.exit("Usage: python tournament.py FILENAME")

    teams = []
    # TODO: Read teams into memory from file
    f = open(sys.argv[1], "r")
    reader = csv.DictReader(f)
    for line in reader:
        new_dict = {
            'team': line['team'],
            'rating': int(line['rating'])
        }
        teams.append(new_dict)

    counts = {}
    # TODO: Simulate N tournaments and keep track of win counts
    run_times = range(N)
    for i in run_times:
        winner = simulate_tournament(teams)
        counts.setdefault(winner, 0)
        counts[winner] += 1

    # Print each team's chances of winning, according to simulation
    for team in sorted(counts, key=lambda team: counts[team], reverse=True):
        print(f"{team}: {counts[team] * 100 / N:.1f}% chance of winning")


def simulate_game(team1, team2):
    """Simulate a game. Return True if team1 wins, False otherwise."""
    rating1 = team1["rating"]
    rating2 = team2["rating"]
    probability = 1 / (1 + 10 ** ((rating2 - rating1) / 600))
    return random.random() < probability


def simulate_round(teams):
    """Simulate a round. Return a list of winning teams."""
    winners = []

    # Simulate games for all pairs of teams
    for i in range(0, len(teams), 2):
        if simulate_game(teams[i], teams[i + 1]):
            winners.append(teams[i])
        else:
            winners.append(teams[i + 1])

    return winners


def simulate_tournament(teams):
    """Simulate a tournament. Return name of winning team."""
    """ Next, implement the simulate_tournament function. This function should accept as input a list of teams and should repeatedly simulate rounds until you’re left with one team. The function should the return the name of that team.

    You can call the simulate_round function, which simulates a single round, accepting a list of teams as input and returning a list of all of the winners.
    Recall that if x is a list, you can use len(x) to determine the length of the list.
    You should not assume the number of teams in the tournament, but you may assume it will be a power of 2. """

    n_rounds = math.log2(len(teams))
    x = range(0, 4)
    for i in x:
        teams = simulate_round(teams)
    return teams[0]['team']


if __name__ == "__main__":
    main()
