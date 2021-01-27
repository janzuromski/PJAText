# PJAText

### A command-line tool for text files analysis.

#### Use it on your text file to:

* Find out how many characters, newlines, digits and numbers it has.

* Get an alphabetically or counter-alphabetically sorted set of all words in it.

* Find palindromes or anagrams of a selected list of words in it.

The output of the program can be printed out directly onto the console or saved into an output file.

#### Flags:

**[ No flag | --help ]**

Prints out description page (this page) into the console.

**[ -f | --file ]**

Place a filepath after this flag to declare the file on which the program will operate. This should be the first declared flag.

**[ -n | --newlines ]**

Use this flag to find out how many newline characters are there in your file.

**[ -d | --digits ]**

Use this flag to find out how many digits are there in your file.

**[ -dd | --numbers ]**

Use this flag to find out how many numbers are there in your file.

**[ -c | --chars ]**

Use this flag to find out how many characters are there in your file.

**[ -a | --anagrams ]**

Place a list of words after this flag to find their anagrams in the files.

**[ -p | --palindromes ]**

Place a list of words after this flag to find matching palindromes in the file. Non-palindromes will be ignored.

**[ -s | --sorted ]**

Use this flag to get a set of all words in the file in alphabetical order.

**[ -rs | --reverse-sorted ]**

Use this flag to get a set of all words in the file in reverse-alphabetical order.

**[ -o | --output ]**

Use this flag to route the output of this program to a file (filepath placed after the flag).

**[ -i | --input ]**

Use this flag with a filepath to declare a file where the instructions are held. If used, this should be the only declared flag.


#### Examples of use:

./PJAText -f filepath -n -c -d

./PJAText -f filepath --newlines --chars --palindromes kajak agata --output output_file

./PJAText -f filepath --newlines -d -c -p kajak agata --output output_file

### This project was created by Jan Żuromski as a part of C++ PJC course at PJAIT.
