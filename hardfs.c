#include <stdio.h>
#include <string.h>


int main(){
	fun();
}

int fun(){
	char s[64];
	int access = 0;

	printf("Enter your name: ");
	fgets(s, 64, stdin);

	printf("Welcome ");
	printf(s);
	// overwrite newline with another null
	s[strlen(s)-1] = '\0';

	if (strcmp(s, "Dr. Yilek")==0){
		access = 1;
	} else if (strcmp(s, "Prof. Yilek")==0){
		access = 1;
	}
	
	if (access){
		printf("You have access...\n");
	} else {
		printf("Sorry, you do not get access...\n");
	}

	return 0;
}



