#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 72

int whitespace(char* buffer){
	int b = 0;
	int f = 7;
	f = f - b - 1;
	b = f;
	//int b = 0;
	//int f = 0;
	while(f < 72 && (buffer[f] == '\n' || buffer[f] == '\t' || buffer[f] == ' ')){
		f++;
	}
		printf("f = %d", f + 1);
	if(f == b){
		printf("Did not progress\n");
	}
	printf("\n%d", f);
	printf("\nchar = %c", buffer[f]);
	printf("\nBuffer = %s\n", buffer);
	return 1;
}
int main(){
	char buffer[] = "       hello there";
	whitespace(buffer);

	return 0;
}
