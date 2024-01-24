from sys import argv, exit
from pyfiglet import Figlet
from random import choice
# --help(Figlet)
fonts_list = Figlet().getFonts()
f = Figlet(font = choice(fonts_list))
def print_input(string):
    print(f.renderText(string))


if len(argv) == 1:
    print("Input: ", end="")
    string = input()
    f = Figlet(font = choice(fonts_list))
    print_input(string)
    exit(0)

elif len(argv) == 3 and (argv[1]=="-f" or argv[1]=="-font") and (argv[2] in fonts_list):
    print("Input: ", end="")
    string = input()
    f = Figlet(font = argv[2])
    print_input(string)
    exit(0)
else:
    print("invalid usage")
    exit(1)
