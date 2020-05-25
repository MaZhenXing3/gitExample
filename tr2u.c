#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char ** argv){

	if (argc != 3){
		fprintf(stderr, "Error: two arguments allowed \n");
		exit(1);
	}
	if (strlen(argv[1]) != strlen(argv[2])){
		fprintf(stderr, "Error: two arguments must be same length \n");
		exit(1);
	}

	for (int a = 1; a < argc; a++){
		for (int i=0; i < strlen(argv[1]); i++){
			if (argv[a][i] == '[' || argv[a][i] == '-' || argv[a][i] == '\\' || argv[a][i] == ']'){
				fprintf(stderr, "Error: operands with no meanings \n");
				exit(1);
			}
		}
	}


	for (int a = 1; a < argc; a++){
		for (int i = 0; i < strlen(argv[1]); i++){
			int diff = 1;
			while(diff <= (strlen(argv[1]) - (i+1)) ){
				if (argv[a][i+diff] == argv[a][i]){
					fprintf(stderr, "Error: duplicate chars \n");
					exit(1);
				}
				diff++;
			}
		}
	}

	char *c = malloc(sizeof(char));
  	memset(c, '\0', sizeof(char));
  	
	if (*c == EOF){
		fprintf(stdout, "nothing to read \n");
	}

	int checker = 0;
	while(read(STDIN_FILENO, c, sizeof(char)) > 0){
		for (int i = 0; i < strlen(argv[1]); i++){
			if (*c == argv[1][i]) {
				write(STDOUT_FILENO, &(argv[2][i]), strlen(c)); //(argv[2][i]);
				checker = 1;
				break;
			}
		}
		if (checker == 0){
			write(STDOUT_FILENO, c, strlen(c));
		}
		checker = 0;
		//read(STDIN_FILENO, c, sizeof(char));
		
	}
return 0;
}
