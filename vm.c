// Glenn Hartwell
// Martin de Salterain
// COP3402, Spring 2021
// HW1 Virtual Machine

#include <stdio.h>
#include <stdlib.h>

#define MAX_STACK_HEIGHT 50
#define MAX_CODE_LENGTH 100
#define INSTRUCTION_LENGTH 3 // This is the length of each instruction

typedef struct instruction
{
	int opcode;
	char op[4]; // For storing instruction abbreviation
	int l;
	int m;
} instruction;

int main(int argc, char *argv[])
{
	int *stack = calloc(MAX_STACK_HEIGHT, sizeof(int));
	instruction *text = calloc(MAX_CODE_LENGTH, sizeof(instruction));

	FILE *fp;
	instruction *ir = NULL;

	int sp = -1;
	int bp = 0;
	int pc = 0;
	int halt = 0;
	int i, j;
	int scan_buffer;

	// Check proper argument count
	if (argc != 2)
	{
		printf("The format for compiling is: ./a.out your_file_name.txt\n");
		return 0;
	}

	// Load instructions into array from file. (Check for EOF).
	fp = fopen(argv[1], "r");
	for(i = 0, fscanf(fp, "%d", &scan_buffer); i < MAX_CODE_LENGTH, scan_buffer!= EOF; i++)
	{
				text[i].opcode = scan_buffer;
				fscanf(fp, "%d", &(text[i].l));
				fscanf(fp, "%d", &(text[i].m));
	}

	fclose(fp);

	ir = calloc(1, sizeof(instruction));

	while (!halt)
	{
		// Fetch
		ir->opcode = text[pc].opcode;
		ir->l = text[pc].l;
		ir->m = text[pc].m;
		pc += 1;

		// Remember to populate ir->op[] array with string abbreviating operation.

		// Execute
		switch(ir->opcode)
		{
			case 1: //LIT: Pushes a literal value "m" onto the stack.
				sp++;
				stack[sp] = ir->m;
				strcpy("LIT", ir->op);
				break;

			case 2: //OPR
				if ()
				{

				}
				break;

			// LOD: Pushes an element onto the stack at "l" lexicographical levels down
			// plus some offset "m."
			case 3:
				sp++;
				stack[sp] = stack[base(stack, ir->l, bp) + ir.m];
				strcpy("LOD", ir->op);
				break;

			case 4: //STO
				break;

			case 5: //CAL
				break;

			case 6: //INC
				break;

			case 7: //JMP
				break;

			case 8: //JPC
				break;

			case 9: //SYS
				break;
		}

	}



	return 0;
}

// Returns the index of the stack indicating the base of the activation record found at some
// given lexicographical level.
int base(int stack[], int level, int bp)
{
	int base = bp;
	int counter = level;
	while (counter > 0)
	{
		base = stack[base];
		counter--;
	}
	return base;
}
