// Write a function to replace vowels with numbers
// Get practice with strings
// Get practice with command line
// Get practice with switch

#include <cs50.h>
#include <stdio.h>

string replace(string word);

int main(int argc, string argv[1])
{
// takes the string from the first line
// put that in a var
if (!argv[1] || argv[2])
{
    printf("no string written or too many words\n");
    return 1;
}

else {
    string name = replace(argv[1]);
    printf("%s\n", name);
}
}

string replace(string word) {
string name = word;
// find the vowels: for loop in order to find vowels; the string is an array and if string[i] is "a", I want string[i] = 6;
for (int i = 0; name[i] != '\0'; i++)
{
    switch (name[i]) {
  case 'a':
    name[i] = '6';
    break;
  case 'e':
    name[i] = '3';
    break;
  case 'i':
    name[i] = '1';
    break;
  case 'o':
    name[i] = '0';
    break;
}
}
return name;
}
