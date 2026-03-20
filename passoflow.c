#include <stdio.h>

void getString(char s[]);
void win(void);
void lose(void);
int checkPass(void);
int stringLen(char s[]);
int stringCompare(char s[], char t[]);

int main(){


	if (checkPass()){
		win();
	} else {
		lose();
	}
	return 0;
}

/*
stringLen loops through characters
of input string until encountering null character
*/
int stringLen(char s[]){
	int i;
	i = 0;
	while (s[i] != '\0'){
		i++;
	}
	return i;
}


/*
stringCompare takes two strings as input
and returns 1 if the strings are equal (i.e., same length
and same characters), 0 otherwise.
*/
int stringCompare(char s[], char t[]){
	int i;
	i = 0;
	if (stringLen(s) != stringLen(t)){
		return 0;
	}

	while (s[i] != '\0'){
		if (s[i] != t[i]){
			return 0;
		}
		i++;
	}
	//if reach here, equal
	return 1;
}

void getString(char s[]){
	int c, i;
	i = 0;
	c = getchar();
	while (c != '\n' && c != EOF){
		s[i] = c;
		i++;
		c = getchar();
	}
	s[i] = '\0';
}

int checkPass(){
	char s[16];
	printf("Password: ");
	getString(s);
	if (stringCompare(s,"mypasswd")){
		return 1;
	}

	return 0;

}

void lose(){
	printf("You lose!\n");
}

void win(){
	printf("You win!\n");
}
