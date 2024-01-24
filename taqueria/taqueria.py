menu = {
    "Baja Taco": 4.00,
    "Burrito": 7.50,
    "Bowl": 8.50,
    "Nachos": 11.00,
    "Quesadilla": 8.50,
    "Super Burrito": 8.50,
    "Super Quesadilla": 9.50,
    "Taco": 3.00,
    "Tortilla Salad": 8.00
}
total = 0
try:
    while True:
        try:
            item = input("Item: ").title()
            total += menu[item]
            print("total: $", "{:.2f}".format(total), sep = "")
        except KeyError:
            continue
except EOFError:
    print("")

        # try prima di true, quando non c'è item deve ignorare