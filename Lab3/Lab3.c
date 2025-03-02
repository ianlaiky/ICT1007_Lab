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
    int fileHandleCounter = 0;

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
    int blocksizebool = 0;
    do {
        printf("Please key in the block size: \n");
        scanf("%d", &blockSize);
        if (blockSize == 1 || blockSize == 2 || blockSize == 4 || blockSize == 8 || blockSize == 16 ||
            blockSize == 32 ||
            blockSize == 64 || blockSize == 128 || blockSize == 256) {
            blocksizebool = 1;
        }

    } while (blocksizebool == 0);

    // how many times to loop to cauclate the locations of the blocks size
    timesToLoop = (256 / blockSize);

    // generating block locations index
    for (int i = 0; i < timesToLoop; ++i) {
        blockLocations[i] = tempBlockLocationCounter;
        tempBlockLocationCounter += blockSize;
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

        // if string contains add
        if (strstr(userInput, "add")) {
            int timesToLoopDueToSizeReq = 0;
            int tempCount = 0;
            // getting index of the 2 spaces
            for (int i = 0; i < userInputLen; ++i) {

                if ((char) userInput[i] == 32) {
                    indexOfSpace[tempCount] = i;
                    tempCount++;
                }
            }
            // getting the 2 int
            // first int
            int firstemp = 0;
            int secondtemp = 0;
            firstInt = malloc((indexOfSpace[1] - indexOfSpace[0]));
            secondInt = malloc((userInputLen - indexOfSpace[1]));

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
            while (secondIntClone > 0) {
                secondIntClone = secondIntClone - blockSize;
                timesToLoopDueToSizeReq++;
            }

            int seekloop = timesToLoopDueToSizeReq * blockSize;
            int currentSeekLocation = 0;
            int outofspace = 0;

            // finding for empty locations
            int seektimes = 256 / blockSize;
            for (int j = 0; j < seektimes; ++j) {
                int boolFirstAvalLocation = 0;

                // MAIN LOGIC
                // for every block size, e.g. blocsize 2, it will check from 0+k(where k is size of file)
                // iteratively
                // if there is data in any of that space, it will move file pointer to 2 then 4 and so on
                for (int k = 0; k < sizeOfFile; ++k) {
                    if (storage[currentSeekLocation + k] != 0) {
                        boolFirstAvalLocation = 1;
                    }
                }
                // if input data is larger than the avail space itself
                if ((int) (currentSeekLocation + sizeOfFile) > 256) {
                    boolFirstAvalLocation = 1;
                }
                currentSeekLocation += blockSize;

                // continuing from above example, if the space if any 16 blocks is empty
                // tempseekloc will be the starting location of the empty blocks are
                if (boolFirstAvalLocation == 0) {
                    // must keep setting out of space to 0. wont affect as there is a break
                    outofspace = 0;

                    int tempseekloc = currentSeekLocation - blockSize;
                    // saving blocklocation for deletion ltr
                    fileStartingBlockLocation[fileHandleCounter] = tempseekloc;
                    // saving handlearr for deletion ltr
                    fileHandleArr[fileHandleCounter] = fileHandle;
                    // saving filesize for deletion ltr
                    fileSize[fileHandleCounter] = sizeOfFile;
                    // populating the storage arr with data
                    for (int adding = 0; adding < sizeOfFile; ++adding) {
                        storage[tempseekloc] = fileHandle + adding + 1;
                        tempseekloc++;
                    }

                    // for deletion purpose, to find out how many file are in the system
                    fileHandleCounter++;

                    // break; when data is inputted, get out of loop
                    break;
                }
                outofspace = 1;
            }

            // end program when out of space
            if (outofspace == 1) {
                printf("Disk is FULL. End of program\n");
                break;
            }

            //             DISPLAY ARRAY
            printf("_____DISPLAY 256 Array_____\n");
            int displaycounter = 0;
            for (int m = 0; m < 256; ++m) {
                if (displaycounter == blockSize) {
                    printf("\n");
                    displaycounter = 0;
                }
                printf("%d ", storage[m]);

                displaycounter++;
            }
            printf("\n");
            printf("_____DISPLAY End Of 256 Array_____\n");
        } else if (strstr(userInput, "delete")) {

            // temp var
            int concatCounter = 0;
            int userDeleteInputSpaceIndex = 0;
            char *fileHandleDelete;
            int intfileHandleDelete;
            // finding location of space
            for (int i = 0; i < userInputLen; ++i) {
                if ((char) userInput[i] == 32) {
                    userDeleteInputSpaceIndex = i;
                    break;
                }
            }
            // reserving space
            fileHandleDelete = malloc(userInputLen - userDeleteInputSpaceIndex);

            // concat the char
            for (int k = (userDeleteInputSpaceIndex + 1); k < (userInputLen); ++k) {
                fileHandleDelete[concatCounter] = userInput[k];
                concatCounter++;

            }
            // convert *char to int
            fileHandleDelete[userInputLen - userDeleteInputSpaceIndex] = '\0';
            intfileHandleDelete = atoi(fileHandleDelete);
            // temp variables
            int fileNotFound = 0;
            int deletesucess = 0;
            // loop thru all the file handles
            for (int j = 0; j < fileHandleCounter; ++j) {
                // if file handle matches what to be deleted
                if (fileHandleArr[j] == intfileHandleDelete) {
                    fileNotFound = 0;
                    fileHandleArr[j] = 0;
                    deletesucess = 1;

                    for (int i = 0; i < fileSize[j]; ++i) {
                        storage[fileStartingBlockLocation[j] + i] = 0;

                    }
                    break;
                }
                fileNotFound = 1;
            }
            if (fileHandleCounter == 0) {
                fileNotFound = 1;
            }
            //             DISPLAY ARRAY
            printf("_____DISPLAY 256 Array_____\n");
            int displaycounter = 0;
            for (int m = 0; m < 256; ++m) {
                if (displaycounter == blockSize) {
                    printf("\n");
                    displaycounter = 0;
                }
                printf("%d ", storage[m]);

                displaycounter++;
            }
            printf("\n");
            printf("_____DISPLAY End Of 256 Array_____\n");
            if (fileNotFound == 1) {
                printf("File not found\n");
            }
            if (deletesucess == 1) {
                printf("Deleted Sucessfully\n");
            }
        }
    }
    if (strstr(userInput, "-1")) {
        printf("Terminated by User\n");
    }
}
