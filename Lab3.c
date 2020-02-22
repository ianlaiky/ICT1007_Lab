#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_SZ 256

int main() {

    // init data
    int storage[256];
    int blockLocations[256];
    int blockSize;
    int timesToLoop;

    // init temp variables
    int fileHandleArr[255], fileSize[255], fileStartingBlockLocation[255];
    int fileHandleCounter=0;


    int tempBlockLocationCounter = 0;
    char *userInput = malloc(MAX_NAME_SZ);
    unsigned int userInputLen;
    if (userInput == NULL) {
        printf("No memory\n");
        return 1;
    }
    // set storage data to 0
    for (int k = 0; k < 256; ++k) {
        storage[k] = 0;
    }

    printf("Please key in the block size: \n");
    scanf("%d", &blockSize);



    // how many times to loop to cauclate the locations of the blocks size
    timesToLoop = (256 / blockSize);
    printf("%d\n\n", timesToLoop);
    // generating block locations index
    for (int i = 0; i < timesToLoop; ++i) {
        blockLocations[i] = tempBlockLocationCounter;
        tempBlockLocationCounter += blockSize;
    }

    for (int j = 0; j < timesToLoop; ++j) {
        printf("%d\n", blockLocations[j]);
    }

    printf("Number of block = %d\n", timesToLoop);
    fgets(userInput, MAX_NAME_SZ, stdin);
    while (!strstr(userInput, "-1")) {

        int fileHandle, sizeOfFile;
        char *firstInt;
        char *secondInt;
        int indexOfSpace[10];
        printf("File to be added or delete: \n");
        // first fgets is to clear the user buffer

        // second fgets gets the user data
        fgets(userInput, MAX_NAME_SZ, stdin);

        // remove trailing whitespace
        if ((strlen(userInput) > 0) && (userInput[strlen(userInput) - 1] == '\n'))
            userInput[strlen(userInput) - 1] = '\0';


        userInputLen = strlen(userInput);
        printf("%d\n", userInputLen);

        // if string contains add
        if (strstr(userInput, "add")) {
            int timesToLoopDueToSizeReq=0;
            int tempCount = 0;
            // getting index of the 2 spaces
            for (int i = 0; i < userInputLen; ++i) {
//            printf("%d\n",userInput[i]);
                if ((char) userInput[i] == 32) {
                    indexOfSpace[tempCount] = i;
                    tempCount++;
                }
            }
            // getting the 2 int
            // first int
//        firstInt = malloc(20);
            int firstemp = 0;
            int secondtemp = 0;
            firstInt = malloc((indexOfSpace[1] - indexOfSpace[0]) - 1);
            secondInt = malloc((userInputLen - indexOfSpace[1]) - 1);


            // concat first int char
            for (int k = (indexOfSpace[0] + 1); k < (indexOfSpace[1]); ++k) {
                firstInt[firstemp] = userInput[k];
                firstemp++;

            }

            // concat second int char
            for (int l = (indexOfSpace[1] + 1); l < userInputLen; ++l) {
                secondInt[secondtemp] = userInput[l];
                secondtemp++;

            }

            // setting null termiator at the end
            firstInt[(indexOfSpace[1] - indexOfSpace[0]) - 1] = '\0';
            secondInt[(userInputLen - indexOfSpace[1])] = '\0';

            // convert to int
            fileHandle = atoi(firstInt);
            sizeOfFile = atoi(secondInt);

            int secondIntClone = sizeOfFile;
            while(secondIntClone>0){
                secondIntClone = secondIntClone-blockSize;
                timesToLoopDueToSizeReq++;
            }
            // delete ltr
            printf("%d\n", fileHandle);
            printf("%d\n", sizeOfFile);
            printf("%d\n", timesToLoopDueToSizeReq);

            // finding for empty locations
            int looped = 0;
            for (int j = 0; j < timesToLoop; ++j) {
                int boolFirstAvalLocation = 0;

                for (int k = 0; k < blockSize; ++k) {
//                    printf("%d\n", storage[blockLocations[j] + k]);
                    if (storage[blockLocations[j] + k] != 0) {
                        boolFirstAvalLocation = 1;
                    }
                }

                if (boolFirstAvalLocation == 0 && looped==timesToLoopDueToSizeReq) {
                    fileStartingBlockLocation[fileHandleCounter] = blockLocations[j];
                    fileHandleArr[fileHandleCounter] = fileHandle;
                    fileSize[fileHandleCounter] = sizeOfFile;
                    for (int adding = 0; adding < sizeOfFile; ++adding) {
                        storage[blockLocations[j]]=fileHandle+adding;
                    }
                    fileHandleCounter++;
                }

                looped++;
            }
            printf("______________\n");
            for (int m = 0; m <256; ++m) {

                printf("%d\n", storage[m]);

            }
        }


    }


}
