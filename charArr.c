#include <stdio.h>

main(){
	fun();
}

int fun(){
	int x;
	char s[8];
	s[0] = 'A';
	s[1] = 'B';
	s[2] = 'C';
	s[3] = 'D';
	s[4] = 'E';
	s[5] = 'F';
	s[6] = 'G';
	s[7] = 'H';
	x = 0;
	s[9] = 1;
	printf("x is %d \n", x);
}

