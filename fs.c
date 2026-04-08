#include <stdio.h>


main(){
	int x;
	char s[128];
	x = 38;

	printf("What is your name? ");
	fgets(s,128,stdin);

	printf("Welcome ");
	printf(s);
	printf("address of x is %p and its value is %d \n", &x,x);
}

