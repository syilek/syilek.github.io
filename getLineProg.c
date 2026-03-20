#include <stdio.h>

int getStr(char dest[]);
int fun();

main(){
    fun();
}

int fun(){
    int x;
    x = 5;
    char s[16];
    int y;
    y = 9;
    getStr(s);
    printf("x is %d, y is %d \n", x, y);

}

int getStr(char dest[]){
    int c, count;
    count = 0;

    c = getchar();
    while (c != '\n' && c != EOF){
        dest[count] = c;
        count++;
        c = getchar();
     }
     dest[count] = '\0';
     return count;
}
