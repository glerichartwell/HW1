// Glenn Hartwell
// Martin de Salterain
// COP3402 Spring 21
// HW1 Virtual Machine
#include <stdio.h>
#include <stdlib.h>

#define MAX_STACK_HEIGHT 50
#define MAX_CODE_LENGTH 100
#define INSTRUCTION_LENGTH 3 // This is the length of each instruction

typedef struct instruction
{
	int opcode;
	char op[4]; // NOT SURE WHY THIS IS NEEDED IF YOU HAVE ANY IDEAS LET ME KNOW
	int l;
	int m;
} instruction;

int main(int argc, char *argv[])
{
	int *stack = calloc(MAX_STACK_HEIGHT, sizeof(int));
	int **text = calloc(MAX_CODE_LENGTH, sizeof(int*));

	// THIS IS TO CHECK GIT HUB
	FILE *fp;
	instruction IR = NULL;
	
	int SP = -1;
	int BP = 0;
	int PC = 0;
	int halt = 0;
	int i, j;
	
	// Check proper argument count
	if (argc != 2)
	{
		printf("The format for compiling is: ./a.out your_file_name.txt\n");
		return 0;
	}

	// text is an array of arrays that hold the integers for each instruction
	// IF YOU THINK OF A BETTER WAY TO STORE THE INSTRUCTIONS DO IT!
	// Initialize text arrays
	for (i = 0; i < MAX_CODE_LENGTH; i++)
	{
		text[i] = calloc(INSTRUCTION_LENGTH, sizeof(int));
	}

	// Load instructions into array from file
	fp = fopen(argv[1], "r");
	for(i = 0; i < MAX_CODE_LENGTH; i++)
	{
		for (j = 0; j < INSTRUCTION_LENGTH; j++)
		{
			fscanf(fp, "%d", &text[i][j]); 
		}
	}
	// Print to make sure it loaded the arrays properly
	// Can be deleted later
	/*
	for(i = 0; i < MAX_CODE_LENGTH; i++)
	{
		for (j = 0; j < INSTRUCTION_LENGTH; j++)
		{
			printf("%d ", text[i][j]);
		}
		printf("\n");
	}
	*/
	fclose(fp);


	while (!halt)
	{
		// Fetch
		IR.opcode = text[PC][0]; // opcode is first num in instruction
		IR.l = text[PC][1]; 	 // l is second num in instruction
		IR.m = text[PC][2];		 // m is third num in instruction
		PC += 1;

		// Execute
		switch (IR.opcode)
		{
			case 1: 
				// sp = sp + 1
				// stack[sp] = M;
				// break;
			case 2: 
				switch (IR.m) // CASES ARE JUST PSEUDO CODE FROM HW INSTRUCTIONS 
							  // NEEDS TO BE UPDATED
				{
					case 0: // RTN
						// stack[bp - 1] = stack[sp];
						// sp = bp - 1;
						// bp = stack[sp + 2];
						// pc = stack[sp + 3];

					case 1: // NEG
						// stack[sp] = -1 * stack[sp];
					
					case 2: // ADD
						// sp = sp – 1;
						// stack[sp] = stack[sp] + stack[sp + 1];

					case 3: // SUB
						// sp = sp – 1;
						// stack[sp] = stack[sp] - stack[sp + 1];

					case 4: // MUL
						// sp = sp – 1;
						// stack[sp] = stack[sp] * stack[sp + 1];

					case 5: // DIV
					
					case 6: // ODD
					
					case 7: // MOD
					
					case 8: // EQL
					
					case 9:	// NEQ
					
					case 10: // LSS
					
					case 11: // LEQ
					
					case 12: // GTR

					case 13: // GEQ

				}

			case 3:

			case 4:

			case 5:

			case 6:

			case 7:

			case 8:

			case 9:

		}

	}
	


	return 0;
}

int base(int stack[], int level, int BP)
{
	int base = BP;
	int counter = level;
	while (counter > 0)
	{
		base = stack[base];
		counter--;
	}
	return base;
}
