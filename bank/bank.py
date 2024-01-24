print("Greeting: ", end = "")
word = input().strip() + " "
greeting = word[:6]
if greeting[0] == "h" and "hello " != greeting:
    print("$20")
elif "hello " == greeting:
    print("$0")
else:
    print("$100")