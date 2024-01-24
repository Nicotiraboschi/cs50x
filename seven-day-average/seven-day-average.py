import csv
import requests


def main():
    # Read NYTimes Covid Database
    download = requests.get(
        "https://raw.githubusercontent.com/nytimes/covid-19-data/master/us-states.csv"
    )
    decoded_content = download.content.decode("utf-8")
    file = decoded_content.splitlines()
    reader = csv.DictReader(file)

    # Construct 14 day lists of new cases for each states
    new_cases = calculate(reader)

    # Create a list to store selected states
    states = []
    print("Choose one or more states to view average COVID cases.")
    print("Press enter when done.\n")

    while True:
        state = input("State: ").title()
        if state in new_cases:
            states.append(state)
        if len(state) == 0:
            break

    print(f"\nSeven-Day Averages")

    # Print out 7-day averages for this week vs last week
    comparative_averages(new_cases, states)


# TODO: Create a dictionary to store 14 most recent days of new cases by state
def calculate(reader):
    lines = list(reader)
    last_15_days = lines[-840:]
    previous_cases = {}
    for row in last_15_days:
        previous_cases.setdefault(row['state'], []).append(int(row['cases']))
    new_cases = {}
    for key, value in previous_cases.items():
        x = range(1, len(value))
        for i in x:
            new_cases.setdefault(key, []).append(value[i] - value[i-1])
    return new_cases




# TODO: Calculate and print out seven day average for given state
def comparative_averages(new_cases, states):
    for state in states:
        sum_week_1 = 0
        sum_week_2 = 0
        x = range(0, 7)
        for i in x:
            sum_week_1 += new_cases[state][i]
        sum_week_1 /= 7
        # sum_week_1 = round(sum_week_1)
        x = range(7, 14)
        for i in x:
            sum_week_2 += new_cases[state][i]
        sum_week_2 /= 7
        difference = sum_week_2 - sum_week_1
        if difference > 0:
            string = "an increase"
        else:
            string = "a decrease"
        try:
            result = round(difference / sum_week_1, 4) * 100
            # result // 100
            print(state, 'had an average 7 days of', round(sum_week_2), "and", string, "of", "{:.3f}".format(result), end = "")
            print("%")
        except ZeroDivisionError:
            print('no cases last week')

main()
