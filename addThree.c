#include <stdio.h>

int addThree(int a, int b, int c);
int addTwo(int x, int y);

main(){
	printf("sum of 5,6, and 10 is %d \n", addThree(5,6,10));
}

int addThree(int a, int b, int c){
	int d;
	d = addTwo(b,c);
	return addTwo(a, d);
}

int addTwo(int x, int y){
	int z;
	z = x+y;
	return z;
}

