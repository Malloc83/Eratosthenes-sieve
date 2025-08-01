/*
This code is part of the Eratosthenes project, which implements the Sieve of Eratosthenes algorithm
to find all prime numbers up to a specified limit. This code is designed to read command line arguments 
for the limit and output file name,and it will generate a sieve of Eratosthenes based on the provided limit.
If no output file name is provided, it will print the prime numbers to the standard output (terminal).
It also includes a help message that can be displayed by using the -h or --help flag.

This code is written in C and adheres to the C17 standard.
It includes error handling for memory allocation and input validation.
The sieve is implemented using an array, and the prime numbers are printed or written to a CSV file.

Compilation instructions:
To compile this code, use the following command:
gcc -std=c17 -o eratos3 eratos3.c -lm
This command compiles the code with the C17 standard and links the math library for the sqrt function.

This project is maintained by Malloc83.
This code was written between 26.07.2025 and 31.07.2025.

Author: Malloc83

This code is licensed under the MIT License.
You are free to use, modify, and distribute this code as long as you include the original license and copyright notice.
*/

#include <stdio.h>  // For printf and scanf
#include <stdlib.h> // For malloc, free and atoi
#include <limits.h> // For UINT_MAX
#include <math.h> // For sqrt function
#include <stddef.h> // For size_t type
#include <errno.h> // For error handling
#include <string.h> // For string manipulation functions
#include <ctype.h> // For tolower function

//  Define constants for the maximum limit and prime status
#define MAX_LIMIT UINT_MAX  // Define the maximum limit for prime number generation (unsigned int)
#define IS_PRIME 1
#define NOT_PRIME 0 
#define ERROR -1
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1 
#define EXIT_HELP 2

// Global variables
int *sieve; // Array to hold the sieve of Eratosthenes
char* file_out = NULL; // Output file name
unsigned limit = 0; // Limit for prime number generation

// Function prototypes
int read_cmnd_arg(int argc, char* argv[]);  // Function to read command line arguments
void print_help(); // Function to print help message
void initialize_sieve(unsigned limit); // Function to initialize the sieve
void sieve_of_eratosthenes(unsigned limit); // Function to perform the Sieve of Eratosthenes algorithm
void print_primes(unsigned limit); // Function to print the prime numbers found in the sieve
void write_sieve_to_csv(const char *filename, unsigned limit); // Function to write the sieve to a CSV file
void free_sieve(); // Function to free the allocated memory for the sieve

//main function
int main(int argc, char* argv[]){

    // Read command line arguments though the read_cmnd_arg function
    if (read_cmnd_arg(argc, argv) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }
    // Check if the limit is set, if not, ask the user for input
    if (limit == 0) {
        printf("Please enter an upper limit for prime number generation (between 2 and %u): ", MAX_LIMIT);
        char input[20]; // Buffer for user input
        if (fgets(input, sizeof(input), stdin) != NULL) {
            limit = atoi(input); // Convert input to unsigned int
            if (limit < 2 || limit > MAX_LIMIT) {
                fprintf(stderr, "Limit must be between 2 and %u\n", MAX_LIMIT);
                puts("Program aborted due to invalid limit.");
                return EXIT_FAILURE;
            }
        } else {
            fprintf(stderr, "Invalid input. Program aborted.\n");
            return EXIT_FAILURE;
        }
    }

    //Check if filename is provided via command line arguments
    if (file_out == NULL) {
        printf("Enter filename for output file (*.csv) or <enter> for screenprint: ");
        char f_input[50]; // Buffer for user input filename
        if (fgets(f_input, sizeof(f_input), stdin) != NULL) {
            // Remove newline character from input
            size_t len = strlen(f_input);
            if (len > 0 && f_input[len - 1] == '\n') {
                f_input[len - 1] = '\0'; // Remove newline character    
            }
        }
        // Check if the user provided a filename
        if (strlen(f_input) > 0) {
            file_out = malloc(strlen(f_input) + 1); // Allocate memory for the filename
            if (file_out == NULL) {
                fprintf(stderr, "Memory allocation failed for file name\n");
                return EXIT_FAILURE;
            }
            strcpy(file_out, f_input); // Copy the user input to file_out
        } else {
            file_out = NULL; // No output file specified, will print to stdout
        }
    }
    // Check if the provided file name ends with .csv
    if (file_out != NULL && strlen(file_out) > 4 && strcmp(file_out + strlen(file_out) - 4, ".csv") != 0) {
        fprintf(stderr, "\033[1;31mWarning:\033[0m Output file name should end with .csv. Using %s instead.\n", file_out);
    }

    // Initialize the sieve with the specified limit   
    initialize_sieve(limit); // Initialize the sieve
    sieve_of_eratosthenes(limit); // Perform the sieve of Eratosthenes

    // If a file name is provided, write the sieve to a CSV file
    if (file_out != NULL) {
        write_sieve_to_csv(file_out, limit); // Write the sieve to a CSV file
        printf("Sieve written to %s\n", file_out); // Notify user of the file
    } else {
        // If no file name is provided, print the prime numbers to stdout
        printf("Prime numbers up to %u:\n", limit);
        print_primes(limit); // Print the prime numbers to stdout
    }

    // ** FREEING MEMORY **
    // Free the sieve memory
    free_sieve(); // Free the allocated memory

    // Return success status
    printf("Program completed successfully.\n");
    return EXIT_SUCCESS;
} // End of main function

//FUNCTION: read command line arguments
int read_cmnd_arg(int argc, char* argv[]) {
    
    // Iterate over command-line arguments to check for the help flag
    // Check if flag is set as --help, wich is the same as -h
    for (int i = 1; i < argc; i++){
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            print_help(); // print the help message
            return EXIT_HELP;
        }
    }
    
    // Iterate over command-line arguments to detect parameters
    for (int i = 1; i < argc; i++){
        // Ensure the argument starts with '-' and has length 2
        if (argv[i][0] == '-' && strlen(argv[i]) == 2) { // argv[i][0] is the first character of ith argument
            char operation = tolower((unsigned char)argv[i][1]); //requires ctype.h for tolower()

            // Ensure there's a next argument for the value
            if (i + 1 < argc) { 
                // Get the value for the parameter
                // Use argv[i + 1] to get the next argument as the value
                char* value = argv[i + 1];

                // Process the parameter
                switch (operation)
                {
                    case 'f':
                        if (argv[i+1][0] == '-') {
                            fprintf(stderr, "Missing value for parameter %s. Parameter ignored.\n", argv[i]);
                            i--; // If the next argument is also a flag, decrement i to avoid skipping it
                            break; // Break out of the switch case if no value is provided
                        }
                        file_out = value;
                        break;

                    case 'n':
                        if (argv[i+1][0] == '-') {
                            fprintf(stderr, "Missing value for parameter %s. Parameter ignored.\n", argv[i]);
                            i--; // If the next argument is also a flag, decrement i to avoid skipping it
                            break; // Break out of the switch case if no value is provided
                        }
                        limit = atoi(value); // Convert string to unsigned int
                        if (limit < 2 || limit > MAX_LIMIT) {
                            fprintf(stderr, "Limit must be between 2 and %u\n", MAX_LIMIT);
                            puts("Program aborted due to invalid limit.");
                            exit(EXIT_FAILURE);
                        }
                        break;

                    default:
                        fprintf(stderr, "Undefined parameter -%c ignored.\n", operation);
                        if (argv[i+1][0] == '-') {
                            i--; // If the next argument is also a flag, decrement i to avoid skipping it
                        }
                        break; // Ignore undefined parameters
                }

                // Skip the next argument since it's used as the value for the current option
                i++;
            }
            else {
                fprintf(stderr, "Missing value for parameter %s. Parameter ignored.\n", argv[i]);
                break; // Break out of the switch case if no value is provided;
            }
        }
        else {
            fprintf(stderr, "Invalid parameter format %s. Parameter ignored.\n", argv[i]);
            // Only check argv[i+1] if i+1 < argc to prevent out-of-bounds access
            if (i + 1 < argc && argv[i+1][0] == '-') {
                i--; // If the next argument is also a flag, decrement i to avoid skipping it
            }
            // Ensure there's a next argument for the value
            if (i + 1 < argc) { 
                i++; // Increment i to skip the invalid parameter 
            } 
            continue; // Continue to the next iteration of the loop
        }
    }
    return EXIT_SUCCESS;
}

// FUNCTION: print help message
void print_help() {
    printf("Usage: ./eratos3 -f [output_filename] -n [integer value]\n");
    printf("Options:\n");
    printf("  -f [output_filename] : Specify the output file name for the sieve. When omitted standard output (terminal).\n");
    printf("  -n [integer value]   : Specify the limit for prime number generation (must be between 2 and %u)\n", MAX_LIMIT);
    printf("  -h, --help           : Display this help message\n");
    printf("Example: ./eratos3 -f output.csv -n 100\n");
    printf("This will generate a sieve of Eratosthenes up to 100 and save it to output.csv\n");
}

// FUNCTION: initialize sieve with the given limit
void initialize_sieve(unsigned limit) {
    sieve = malloc((limit + 1) * sizeof(size_t));
    if (sieve == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i <= limit; i++) {
        sieve[i] = IS_PRIME; // Assume all numbers are prime initially
    }
    sieve[0] = NOT_PRIME; // 0 is not prime
    sieve[1] = NOT_PRIME; // 1 is not prime
}

// FUNCTION: implement the Sieve of Eratosthenes algorithm
void sieve_of_eratosthenes(unsigned limit) {
    for (unsigned i = 2; i <= sqrt(limit); i++) {
        if (sieve[i] == IS_PRIME) {
            for (unsigned j = i * i; j <= limit; j += i) {
                sieve[j] = NOT_PRIME; // Mark multiples of i as not prime
            }
        }
    }
}

// FUNCTION: print the prime numbers found in the sieve
// It prints the prime numbers to stdout
void print_primes(unsigned limit) {
    for (unsigned i = 2; i <= limit; i++) {
        if (sieve[i] == IS_PRIME) {
            printf("%u ", i); // Print the prime number
        }
    }
    printf("\n");
}

/* FUNCTION: write sieve to a CSV file
 * Function to write the sieve array to a comma-separated file.
 * This function opens the specified file for writing, iterates through the sieve array,
 * and writes the prime numbers to the file in a comma-separated format.
 */
void write_sieve_to_csv(const char *filename, unsigned limit) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        fprintf(stderr, "Failed to open file %s for writing\n", filename);
        return;
    }
    int first = 1;
    for (unsigned i = 2; i <= limit; i++) {
        if (sieve[i] == IS_PRIME) {
            if (!first) {
                fprintf(fp, ",");
            }
            fprintf(fp, "%u", i);
            first = 0;
        }
    }
    fprintf(fp, "\n");
    fclose(fp);
}

// FUNCTION: to free the allocated memory for the sieve
void free_sieve() {
    free(sieve); // Free the allocated memory for the sieve
}