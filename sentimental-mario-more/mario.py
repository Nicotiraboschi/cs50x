# TODO
while True:
    try:
        height = int(input("Height: "))
        if height > 8 or height < 1:
            continue
        break
    except ValueError:
        continue


x = range(1, height+1)
for row in x:
    print(" "*(height-row), "#" * (row), "  ", "#" * (row), sep="")