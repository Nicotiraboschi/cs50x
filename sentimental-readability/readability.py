# TODO


def main():
    text = input("Text: ")
    length = len(text)
    letters = count_letters(text, length)
    words = count_words(text, length)
    sentences = count_sentences(text, length)
    index = 0.0588 * letters / words * 100 - 0.296 * sentences / words * 100 - 15.8
    grade = round(index)
    if grade > 16:
        print("Grade 16+")
    elif grade < 1:
        print("Before Grade 1")
    else:
        print("Grade", grade)
    return 0


def count_letters(text, length):
    x = range(0, length, 1)
    count = 0
    for i in x:
        if (ord(text[i]) >= 65 and ord(text[i]) <= 90) or (ord(text[i]) >= 97 and ord(text[i]) <= 122):
            count += 1
    return count


def count_words(text, length):
    x = range(0, length, 1)
    count = 0
    for i in x:
        if (ord(text[i]) == 32):
            count += 1
    return count + 1


def count_sentences(text, length):
    x = range(0, length, 1)
    count = 0
    for i in x:
        if (ord(text[i]) == 33 or ord(text[i]) == 46 or ord(text[i]) == 63):
            count += 1
    return count


main()
