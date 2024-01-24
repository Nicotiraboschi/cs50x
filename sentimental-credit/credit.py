# TODO
import cs50


def main():
    credit_card = cs50.get_string("Credit card? ")
    if len(credit_card) == 15 and (credit_card[0] == "3" and (credit_card[1] == "4" or credit_card[1] == "7")):
        type = "AMEX"
    elif (len(credit_card) == 13 or len(credit_card) == 16) and credit_card[0] == "4":
        type = "VISA"
    elif len(credit_card) == 16 and (int(credit_card[1]) >= 1 and int(credit_card[1]) <= 5):
        type = "MASTERCARD"
    else:
        print("INVALID")
        return

    sum = 0
    digits_second_to_last = range(len(credit_card)-2, -1, -2)
    for i in digits_second_to_last:
        tmp_sum = str(int(credit_card[i]) * 2)
        for digit in tmp_sum:
            sum += int(digit)
    other_digits = range(len(credit_card)-1, -1, -2)
    for i in other_digits:
        sum += int(credit_card[i])
    sum = str(sum)
    if sum[len(sum)-1] == "0":
        print(type)
    else:
        print('INVALID')


main()
