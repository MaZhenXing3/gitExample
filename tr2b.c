#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

	int c = getchar();
	if (c == EOF){
		fprintf(stdout, "nothing to read \n");
	}
	int checker = 0;
	while( c != EOF){

		for (int i = 0; i < strlen(argv[1]); i++){
			if (c == argv[1][i]) {
				putchar(argv[2][i]);
				checker = 1;
				break;
			}
		}
		if (checker == 0){
			putchar(c);
		}
		
		checker = 0;
		c = getchar();
	}

return 0;
}
