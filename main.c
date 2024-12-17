#include <stdio.h>
#include <stdlib.h>

// Function prototypes
int romanToDecimal(char* roman); // Converts Roman numerals to decimal values
void decimalToWords(int num, char* result); // Converts decimal numbers to words
void processInputFile(char* inputFileName, char* outputFileName); // Handles file reading, processing, and writing
void readInputFile(char* inputFileName, char* lines[]); // Reads lines from the input file
void writeOutputFile(char* outputFileName, char* results[]); // Writes results to the output file
void myStrcat(char* dest, const char* src); // Concatenates two strings
void myStrcpy(char* dest, const char* src); // Copies one string to another
char* myStrtok(char* str, const char* delim); // Custom implementation of string tokenization
char* myStrdup(const char* str); // Creates a duplicate of a string
int isValidRoman(char* str); // Checks if a string is a valid Roman numeral

// Arrays for number words (used in converting numbers to words)
const char* ones[] = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"}; // Ones place
const char* teens[] = {"Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"}; // Teens
const char* tens[] = {"", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"}; // Tens place
const char* thousands[] = {"", "Thousand", "Million", "Billion"}; // For large numbers

int main() {
    // Process the input file and generate the output file
    processInputFile("Input.txt", "Output.txt");
    return 0;
}

// Function to convert a Roman numeral to its decimal equivalent
int romanToDecimal(char* roman) {
    int total = 0; // Total decimal value
    int i = 0; // Index for traversing the Roman numeral string

    // Loop through each character of the Roman numeral
    while (roman[i]) {
        if (roman[i] == 'M') total += 1000; // M = 1000
        else if (roman[i] == 'D') total += 500; // D = 500
        else if (roman[i] == 'C' && (roman[i+1] == 'D' || roman[i+1] == 'M')) total -= 100; // Subtraction rule for CD or CM
        else if (roman[i] == 'C') total += 100; // C = 100
        else if (roman[i] == 'L') total += 50; // L = 50
        else if (roman[i] == 'X' && (roman[i+1] == 'L' || roman[i+1] == 'C')) total -= 10; // Subtraction rule for XL or XC
        else if (roman[i] == 'X') total += 10; // X = 10
        else if (roman[i] == 'V') total += 5; // V = 5
        else if (roman[i] == 'I' && (roman[i+1] == 'V' || roman[i+1] == 'X')) total -= 1; // Subtraction rule for IV or IX
        else if (roman[i] == 'I') total += 1; // I = 1
        i++; // Move to the next character
    }
    return total; // Return the total decimal value
}

// Function to concatenate two strings
void myStrcat(char* dest, const char* src) {
    while (*dest) dest++; // Move to the end of the destination string
    while (*src) *dest++ = *src++; // Append each character from source to destination
    *dest = '\0'; // Null-terminate the resulting string
}

// Function to copy a string
void myStrcpy(char* dest, const char* src) {
    while (*src) *dest++ = *src++; // Copy each character from source to destination
    *dest = '\0'; // Null-terminate the resulting string
}

// Function to tokenize a string
char* myStrtok(char* str, const char* delim) {
    static char* last; // Pointer to keep track of the remaining string
    if (str) last = str; // If a new string is provided, reset the static pointer
    if (!last) return NULL; // Return NULL if there is nothing left to tokenize

    char* start = last; // Mark the start of the token
    while (*last && !strchr(delim, *last)) last++; // Move until a delimiter is found
    if (*last) *last++ = '\0'; // Replace the delimiter with NULL and move forward
    else last = NULL; // Set to NULL if end of string is reached

    return start; // Return the current token
}

// Function to duplicate a string
char* myStrdup(const char* str) {
    int len = 0; // Variable to store string length
    while (str[len]) len++; // Calculate string length

    char* copy = (char*)malloc(len + 1); // Allocate memory for the new string
    for (int i = 0; i <= len; i++) copy[i] = str[i]; // Copy each character including the null terminator
    return copy; // Return the new string
}

// Function to convert a number less than 1000 to words
void numberToWords(int num, char* result) {
    if (num >= 100) { // Handle hundreds place
        myStrcat(result, ones[num / 100]); // Append the word for hundreds
        myStrcat(result, " Hundred "); // Append "Hundred"
        num %= 100; // Reduce to remainder
    }
    if (num >= 20) { // Handle tens place for numbers 20 and above
        myStrcat(result, tens[num / 10]); // Append the word for tens
        myStrcat(result, " ");
        num %= 10; // Reduce to remainder
    } else if (num >= 10) { // Handle teens (10 to 19)
        myStrcat(result, teens[num - 10]); // Append the word for teens
        myStrcat(result, " ");
        return; // Teens do not require further processing
    }
    myStrcat(result, ones[num]); // Append the word for ones place
    myStrcat(result, " ");
}

// Function to convert a decimal number to words
void decimalToWords(int num, char* result) {
    result[0] = '\0'; // Initialize result as an empty string
    if (num == 0) { // Special case for zero
        myStrcpy(result, "Zero");
        return;
    }

    char buffer[1000] = ""; // Buffer to store intermediate results
    if (num < 0) { // Handle negative numbers
        myStrcat(result, "Negative ");
        num = -num; // Make the number positive for further processing
    }

    int i = 0; // Index for "thousands" array
    while (num > 0) { // Process each group of 3 digits
        if (num % 1000 != 0) { // If the current group is not zero
            char temp[100] = "";
            numberToWords(num % 1000, temp); // Convert the group to words
            myStrcat(temp, thousands[i]); // Append the appropriate scale word (thousand, million, etc.)
            myStrcat(temp, " ");
            myStrcat(temp, buffer); // Prepend to buffer
            myStrcpy(buffer, temp); // Update buffer
        }
        num /= 1000; // Move to the next group of digits
        i++; // Increment scale index
    }
    myStrcat(result, buffer); // Copy the final result to "result"
}


// Function to check if a string is a valid Roman numeral
int isValidRoman(char* str) {
    while (*str) {
        if (*str != 'I' && *str != 'V' && *str != 'X' && *str != 'L' && *str != 'C' && *str != 'D' && *str != 'M') {
            return 0; // Return 0 if invalid character is found
        }
        str++;
    }
    return 1; // Return 1 if all characters are valid
}

// Function to process the input file and produce the output file with preventive mechanisms
void processInputFile(char* inputFileName, char* outputFileName) {
    char* lines[100] = {0}; // Initialize the array with NULL
    char* results[100] = {0}; // Initialize the array with NULL
    readInputFile(inputFileName, lines); // Read input file
    for (int i = 0; lines[i] != NULL; i++) { // Loop through each line
        printf("Processing line: %s\n", lines[i]); // Debugging statement

        // Make a copy of the line to avoid modifying the original
        char* lineCopy = myStrdup(lines[i]);

        char* roman1 = myStrtok(lineCopy, " "); // Get the first Roman numeral
        char* op = myStrtok(NULL, " ");         // Get the operator
        char* roman2 = myStrtok(NULL, " ");     // Get the second Roman numeral

        // Ensure all tokens are valid before proceeding
        if (!roman1 || !op || !roman2) {
            printf("Invalid input format. Skipping line.\n");
            results[i] = myStrdup("Invalid input");
            free(lineCopy);
            continue;
        }

        if (!isValidRoman(roman1) || !isValidRoman(roman2)) {
            printf("Invalid Roman numeral found. Skipping line.\n");
            results[i] = myStrdup("Invalid input");
            free(lineCopy);
            continue;
        }

        int num1 = romanToDecimal(roman1); // Convert the first Roman numeral to decimal
        int num2 = romanToDecimal(roman2); // Convert the second Roman numeral to decimal
        printf("Converted %s to %d\n", roman1, num1); // Debugging statement
        printf("Converted %s to %d\n", roman2, num2); // Debugging statement
        int result;

        // Perform operations
        if (strcmp(op, "+") == 0) result = num1 + num2;
        else if (strcmp(op, "-") == 0) result = num1 - num2;
        else if (strcmp(op, "*") == 0) result = num1 * num2;
        else if (strcmp(op, "/") == 0) result = (num2 != 0) ? num1 / num2 : 0;
        else {
            printf("Invalid operator found. Skipping line.\n");
            results[i] = myStrdup("Invalid operator");
            free(lineCopy);
            continue;
        }

        printf("Result of %d %s %d = %d\n", num1, op, num2, result); // Debugging statement
        char resultWords[1000] = "";
        decimalToWords(result, resultWords); // Convert result to words
        results[i] = myStrdup(resultWords); // Copy the result to results array
        printf("Converted result to words: %s\n", resultWords);

        free(lineCopy); // Free the copied line
    }
    writeOutputFile(outputFileName, results); // Write output file

    // Free allocated memory
    for (int i = 0; lines[i] != NULL; i++) free(lines[i]);
    for (int i = 0; results[i] != NULL; i++) free(results[i]);
}



// Function to write output file
void writeOutputFile(char* outputFileName, char* results[]) {
    FILE* file = fopen(outputFileName, "w"); // Open the output file
    if (file == NULL) { // Check if file was opened successfully
        printf("Error: Could not open file %s\n", outputFileName); // Print error message
        return;
    }
    for (int i = 0; results[i] != NULL; i++) { // Loop through each result
        fprintf(file, "%s\n", results[i]); // Write result to file
    }
    fclose(file); // Close the file
    printf("Output written to %s\n", outputFileName); // Debugging statement
}

// Function to read input file
void readInputFile(char* inputFileName, char* lines[]) {
    FILE* file = fopen(inputFileName, "r"); // Open the input file
    if (file == NULL) { // Check if file was opened successfully
        printf("Error: Could not open file %s\n", inputFileName); // Print error message
        return;
    }
    char line[100]; // Buffer to hold each line
    int i = 0; // Initialize index
    while (fgets(line, sizeof(line), file)) { // Loop through each line in the file
        // Remove trailing newline character if present
        char* newline = strchr(line, '\n');
        if (newline) *newline = '\0';

        lines[i] = myStrdup(line); // Duplicate the line for safe keeping
        i++; // Increment index
    }
    lines[i] = NULL; // Ensure the array is null-terminated
    fclose(file); // Close the file
}
