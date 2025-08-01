# Prime numbers through Eratosthenes sieve algortihm

Author: Malloc83

**This file and associated code is licensed under the MIT License, see LICENSE.txt**

This program uses the **Eratosthenes sieve algoritm** to determine the prime numbers of a sequentially ordered list of unsigned integers starting at the value two (2) to an upper value which is provided by user input. The array with prime numbers is either printed on the screen (stdout) or redirected to a comma separated value format file (*.csv) determined by user during execution of the program (CLI) or argument (argv) during the initiaton of the program itself.

## Why is this project useful?
It is a simple example to showcase functionality of C language where the actual function itself, i.e. creating
a list of prime number, is a very short user created algorith function, however where user input and error checking creates significant additional code. This project was started with the predefinded required fuctionality as listed in next paragraph. It can be used as a hands-on example for students to learn the basics of the C language and particulary to understand the functionality of **argv**.  

### Required functionality of the program

1. Through user input, either during execuion or argument (-n), provide the upper limit (UL) value of the sequential list as *unsigned integer*.
2. Determine the prime numbers within the sequence from 2 up to and including the UL through the Eratosthenes algorithm through a separate function called 'sieve_of_eratosthenes' which takes UL as unsigned integer as input and creates an array with the prime numbers.
3. Through user input, either during execution or argument (-f), check if the array needs to be printed on screen (stdout) or stored in .csv file with a name chosen by the user. If no filename is provided then prime numbers are printed on stdout (monitor). Numbers need to be stored as comma separated values. If a filename other then *.csv is provided then warning message is printed.
4. If argument -h or --help is given then a help text with explanation needs to be printed on the screen. The help text needs to be stored in separate function which is called within the main() function. No other actions needs to be done when -h or --help is given as argument.
5. If invalid arguments are given, a warning message will be printed that these are ignored. 

There needs to be sufficient (input) error checking to prevent that the program terminates unexpectedly.
Integer numbers and file names need to be valid. 

## How to get started with this project.
Usage: ./eratos3 -f [output_filename] -n [integer value]

  Options:
  
    -f [output_filename] : Specify the output file name for the sieve. When omitted standard output (terminal).
    
    -n [integer value]   : Specify the limit for prime number generation (must be between 2 and UL).
    
    -h, --help           : Display this help message
    
Example: ./eratos3 -f output.csv -n 100

This will generate a sieve of Eratosthenes up to 100 and save it to output.csv

## Eratosthenes algorithm - steps

1. Make a sorted list of all numbers from 2 to the upper limit.
2. Select the smallest number from the list.
3. Remove all the multiples of the chosen number (but not the selected number itslef).
4. Continue with the next number in the sequence and repeat step 3.
5. The numers which remain are prime numbers up to and including the UL.

*The procedure can be optimized through:*
1. Is makes no sense to select a number which is already crossed out, because all multiples of this number are already crossed out.
2. You can start with crossing out at the square (n^2) of the selected number. All smaller multiples are already crossed-out.
3. If the chosen number if larger than the square root of the maximum then the procedure is completed.

## Who maintains this project 
This project is maintained by Malloc83 and last revised on 1st August 2025.

### known flaws in the current version
1. There is memory leakage on the dynamic memory allocation of the filename. If free(file) will create a segmentation fault when the filename is provided via the argv. When the filename is provided during execution of the program (called via malloc) this is no issue. Needs some additional code to see if filename variable is created dynamically. For now the free(filename) command is ommited to prevent segmentation fault.
2. On older computer with insuffient memory (guess that this is the case) when a very high value for the upper limit (unsigned integer) is given then a failed memory allocation fault is created and program halted. Lower end error checking (< 2 is working properly).
