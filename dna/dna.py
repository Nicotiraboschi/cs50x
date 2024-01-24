import csv
import sys


def main():

    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")
        return 1

    # TODO: Read database file into a variable
    csvfile = open(sys.argv[1], "r")
    database = csv.reader(csvfile)

    # TODO: Read DNA sequence file into a variable
    csvfile = open(sys.argv[2], "r")
    sequence = next(csv.reader(csvfile))[0]

    # TODO: Find longest match of each STR in DNA sequence
    comparison_array = [0] * 8
    x = range(0, 8)
    for i in x:
        comparison_array[i] = 0
    first_line_database = (next(database))
    x = range(1, len(first_line_database))
    for i in x:
        subsequence = first_line_database[i]
        comparison_array[i-1] = longest_match(sequence, subsequence)
    # print(comparison_array)
    # TODO: Check database for matching profiles
    name = ""
    for row in database:
        i = 0
        while comparison_array[i] == int(row[i + 1]):
            if i < len(row) - 2:
                i += 1
            else:
                name = row[0]
                break
    if name != "":
        print(name)
    else:
        print("No match")
    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
