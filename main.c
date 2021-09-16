#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MESSAGE_LENGTH 100
#define ASCII_LENGTH 127
#define ENCODING_BYTE_SIZE 8
#define BYTE_ZERO_CHAR 48
#define BYTE_ONE_CHAR 49

/*
 * Method that contains the algorithm for the encryption/decryption of message
 *
 * @param message: a string that contains a message that should be encrypted or decrypted
 * @param k      : an integer value that tells how far to encrypt/decrypt when converting letters

 * @returns -> A encrypted/decrypted String
*/
char* caesarCipher(const char* message, int k) {
    // Boolean if NULL Char found {0, 1}
    int nullCharFound = 0;
    // Index to a char in the message
    int i = 0;
    // Contains encrypted message
    char encryptedMessage[MESSAGE_LENGTH];
    // Variable that will be returned after the method is finished. MALLOC onto the stack so memory addr is not lost when going back to the main method
    char* encodedMessage = malloc(sizeof(char) * MESSAGE_LENGTH);

    while (nullCharFound != 1 && i < MESSAGE_LENGTH) {
        // if Char is a null character or a new line break
        if ( (char) message[i] == 0 || (char) message[i] == '\n')
            nullCharFound = 1;
        else {
            // if cipher makes char go outside the bounds of ASCII, wrap around to the beginning
            if ( message[i] + k > ASCII_LENGTH )
                encryptedMessage[i] = (char) ( (message[i] + k) - (ASCII_LENGTH + 1) );
            else
                encryptedMessage[i] = (char) (message[i] + k);
        }
        i++;
    }
    // copy string to a new pointer & return
    strcpy(encodedMessage, encryptedMessage);
    return encodedMessage;
}

/*
 * Method made for encrypting a message. Passing the + k value to caesarCipher()
 *
 * @param message: a string that contains a message that should be encrypted or decrypted
 * @param k      : an integer value that tells how far to encrypt/decrypt when converting letters
 *
 * @returns -> A encrypted/decrypted String
 */
char* encryptMessage(const char* message, int k) { return caesarCipher(message, k); }

/*
 * Method made for encrypting a message. Passing the - k value to caesarCipher()
 *
 * @param message: a string that contains a message that should be encrypted or decrypted
 * @param k      : an integer value that tells how far to encrypt/decrypt when converting letters
 *
 * @returns -> A encrypted/decrypted String
 */
char* decryptMessage(const char* message, int k) { return caesarCipher(message, -k); }

/*
 * Method made for decoding a message
 *
 * @param message: a string that contains a message that should be decoded
 *
 * @returns -> A decoded String
 */
char* decodeMessage(const char* message) {

    unsigned int messageLength = strlen(message);

    // Checks if the binary representation is valid
    for (int i = 0; i < messageLength - 1; i++) {
        if ( *(message + i) >= '0' && *(message + i) <='9')
            continue;
        else
            return "Pleas enter a valid binary sequence.";
    }

    // Checks if the binary representation has an encoding byte size of 8
    if ( messageLength % 8 != 0 )
        return "Please enter a binary sequence with each byte having 8 bits";

    unsigned int decodedMessageLength = messageLength / ENCODING_BYTE_SIZE;
    char* decodedMessage = malloc(sizeof(char) * decodedMessageLength);

    int decodedMessageIndex = 0;
    int index = 0;
    // Loops through the predicted size of the decoded message
    for (int i = 0; i < decodedMessageLength; i++) {
        index = i * ENCODING_BYTE_SIZE;
        int binarySum = 0;
        // Loops through the 8 bits for the encoding
        for (int j = 0; j < ENCODING_BYTE_SIZE; j++) {
            int binaryIndex = ENCODING_BYTE_SIZE - j - 1;
            int bit = (int) *(message + index + binaryIndex) == 48 ? 0 : 1 ;
            binarySum = binarySum + (int) ( bit * pow(2, j) );
        }
        *(decodedMessage + decodedMessageIndex) = (char) binarySum;
        binarySum = 0;
        decodedMessageIndex++;
    }
    return decodedMessage;
}

/*
 * Method made for encoding a message
 *
 * @param message: a string that contains a message that should be encoded
 *
 * @returns -> An encoded String
 */
char* encodeMessage(const char* message) {
    unsigned int messageLength = strlen(message);

    char** encodedCharMessage = malloc(sizeof(char*) * messageLength);

    int** encodedMessage = malloc(sizeof(int*) * messageLength);
    int** tempBinaryArray = malloc(sizeof(int*) * messageLength);

    char* combinedMessage = malloc(sizeof(char) * messageLength * ENCODING_BYTE_SIZE);

    // Loop through each letter in the message that needs to be encoded
    for (int i = 0; i < messageLength; i++) {
        *(encodedCharMessage + i) = malloc(sizeof(char) * ENCODING_BYTE_SIZE);
        *(encodedMessage + i) = malloc(sizeof(int) * ENCODING_BYTE_SIZE);
        *(tempBinaryArray + i) = malloc(sizeof(int) * ENCODING_BYTE_SIZE);
        int charInt = (int) *(message + i);

        // For each letter, convert the letter to the binary representation and store each bit in an array
        // Place an -1 at the end of the array to indicate the ending of the array]
        for (int j = 0; j < ENCODING_BYTE_SIZE; j++) {
            if (charInt == 0) {
                *(*(tempBinaryArray + i) + j) = -1;
                break;
            }
            *(*(tempBinaryArray + i) + j) = charInt % 2;
            charInt = charInt / 2;
        }

        int size = 0;
        int index = 0;
        // Finds the end of the array and stores the index
        while (index < ENCODING_BYTE_SIZE) {
            if (*(*(tempBinaryArray + i) + index) == -1)
                break;
            size++;
            index++;
        }

        // Fills in trailing zeros if not 8 bytes
        int trailingZerosIndex = ENCODING_BYTE_SIZE - size;
        for (int j = 0; j < trailingZerosIndex; j++) {
            *(*(encodedMessage + i) + j) = 0;
        }

        // Flips the array from end to start
        for (int a = index - 1; a >= 0; a--) {
            *(*(encodedMessage + i) + trailingZerosIndex) = (*(*(tempBinaryArray + i) + a));
            trailingZerosIndex++;
        }

        // depending on the char in the array, place an 0 or 1 bit in the a new array]
        for (int t = 0; t < ENCODING_BYTE_SIZE; t++)
            *(*(encodedCharMessage + i) + t) = *(*(encodedMessage + i) + t) == 0 ? (char) BYTE_ZERO_CHAR : (char) BYTE_ONE_CHAR;

        // cat all of the 8 byte arrays into a 8 * message length array
        strcat(combinedMessage, *(encodedCharMessage + i));

    }
    return combinedMessage;
}
/*
 * Method for parsing user input into a command and some message to be encrypted/decrypted
 *
 * @param input: User input String
 *
 * @returns -> Some encrypted/decrypted message based on the command
 */
char* parseUserInput(char* input) {
    // Parser for the command and message
    char delim_A[2] = "(";
    char delim_B[2] = ")";

    // Takes off the end ')'
    strtok(input, delim_B);
    // Message that the user wants to encrypt/decrypt
    char* message = malloc(sizeof(char) * strlen(input));
    // Command that the user wants to operate on the message
    char* command = malloc(sizeof(char) * strlen(input));

    //Parses through user input string with an iterative approach
    char* token = strtok(input, delim_A);
    while (token != NULL) {
        token = strtok(NULL, delim_A);
        if (token != NULL)
            strcpy(message, token);
    }

    //Copies the command within the variable input into the variable command that is allocated on the stack
    strcpy(command, input);
    long inputK;

    if (strcmp(command, "encrypt") == 0 || strcmp(command, "decrypt") == 0) {
        printf("Enter a K value (Max K = 99): ");
        char inputInt[4] = "";
        fgets(inputInt, 3, stdin);
        char *ptr;


        inputK = strtol(inputInt, &ptr, 10);
    }

    if (strcmp(command, "encrypt") == 0)
        return encryptMessage(message, (int) inputK);

    else if (strcmp(command, "decrypt") == 0)
        return decryptMessage(message, (int) inputK);

    else if (strcmp(command, "encode") == 0)
        return encodeMessage(message);

    else if (strcmp(command, "decode") == 0)
        return decodeMessage(message);

    else {
        printf("command doesn't exist, try again!");
        return "\n";
    }
}

int main() {

    char userInput[MESSAGE_LENGTH];
    int shouldExit = 0;
    // Loops until exit
    while (shouldExit == 0) {
        printf("Enter Command: ");
        fgets(userInput, MESSAGE_LENGTH - 1, stdin);
        if (strcmp(userInput, "exit\n") == 0) {
            shouldExit = 1;
            continue;
        }

        char *message = parseUserInput(userInput);
        printf("%s\n", message);
    }
    return 0;


}
