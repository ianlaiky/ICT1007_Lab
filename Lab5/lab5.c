#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 256
int main(){

	int pageFrame;
	
	char *userInput = malloc(MAX_SIZE);
	printf("Number of page frame: \n");

	scanf("%d", &pageFrame);
	
	printf("Enter reference string: \n",userInput);	



  fgets(userInput, MAX_SIZE, stdin);

 fgets(userInput, MAX_SIZE, stdin);

printf("ref str: %s",userInput);

}
