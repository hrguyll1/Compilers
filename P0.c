#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 72
int main(){
	FILE *file;
	FILE *outfile = fopen("outfile.txt", "w");
	int line_counter = 1;
	if(outfile == NULL){
		fprintf(stderr, "Can't open outfile\n");
		return -1;
	}
	file = fopen("sourcefile.txt", "r");
	if(file == NULL){
		fprintf(stderr, "Can't open source file\n");
		return -1;
	}
	char buffer[SIZE];

	while(fgets(buffer, sizeof(buffer), file)){
		fprintf(outfile,"%d. %s", line_counter, buffer);
		line_counter++;
		memset(buffer, '\0', SIZE);
	}
	fclose(file);
	fclose(outfile);
}
