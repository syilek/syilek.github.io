#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int fun(char *src);
int cop(char *t, char *s);

int main(int argc, char *argv[]){
	fun(argv[1]);
	//shell();
	return 0;
}

int fun(char *s){
	char t[64];
	int x;
	x = 0;
	printf("address of t is %p \n", t);
	cop(t,s);
	return 0;
}

int cop(char *t, char *s){
	strcpy(t,s);
	return 0;
}

