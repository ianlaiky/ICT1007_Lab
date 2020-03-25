#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 256

int main() {

    int pageFrame;
    int framearray[MAX_SIZE];
    char *userInput = malloc(MAX_SIZE);
    char *pch;
    int indexposition = 0;
    int pagefaultCounter = 0;
    printf("Number of page frame: \n");

    scanf("%d", &pageFrame);

    printf("Enter reference string:(Include a space between pages E.g. 1 2 1 0) \n");


    // to clear the input from scanf
    fgets(userInput, MAX_SIZE, stdin);
    // getting the user input
    fgets(userInput, MAX_SIZE, stdin);

    // using strtok to delimit by space
    pch = strtok(userInput, " ");
    while (pch != NULL) {
        // to ignore new line, ascii 10
        if (*pch != 10) {
            int inter = atoi(pch);

            // check if data exists in array
            int pagefaultchck = 0;
            for (int i = 0; i < pageFrame; ++i) {
                if (framearray[i] == inter) {
                    pagefaultchck++;
                }
            }

            // if does not exists
            if (pagefaultchck == 0) {
                pagefaultCounter++; // add counter
                framearray[indexposition] = inter; // save data to the array
                indexposition++; // move index by one
                if (indexposition + 1 == pageFrame) { // if index added is more than array size, move index pointer to 0
                    indexposition = 0;
                }
            }

        }
        pch = strtok(NULL, " ");
    }

    printf("Page fault Occurred: %d\n",pagefaultCounter);
    return 0;
}
