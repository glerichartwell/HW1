// Glenn Hartwell
// Martin de Salterain
// COP3402, Spring 2021
// HW1 Virtual Machine

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int main(int argc, char *argv[])
{
	int *stack = calloc(MAX_STACK_HEIGHT, sizeof(int));
	int *ar_delimiter_stack = calloc(MAX_STACK_HEIGHT, sizeof(int));
	instruction *text = calloc(MAX_CODE_LENGTH, sizeof(instruction));

	FILE *fp;
	instruction *ir = NULL;

	int sp = -1;
	int bp = 0;
	int pc = 0;
	int halt = 1;
	int i, line_number, ar_end_index;
	int scan_buffer;

	// Check proper argument count
	if (argc != 2)
	{
		printf("The format for compiling is: ./a.out your_file_name.txt\n");
		return 0;
	}

	// Load instructions into array from file. (Check for EOF).
	if ((fp = fopen(argv[1], "r")) == NULL)
	{
		printf("Error: failed to open input file. Exiting...\n");
		return 0;
	}

	for (i = 0; fscanf(fp, "%d", &scan_buffer) != EOF && i < MAX_CODE_LENGTH; i++)
	{
				text[i].opcode = scan_buffer;
				fscanf(fp, "%d", &(text[i].l));
				fscanf(fp, "%d", &(text[i].m));
	}

	fclose(fp);

	ir = calloc(1, sizeof(instruction));

	/*
	printf("IR: %d %d %d\n", ir->opcode, ir->l, ir->m);
	printf("TEXT: %d %d %d\n", text[pc].opcode, text[pc].l, text[pc].m);
	printf("IR->OP: %s\n", ir->op);
	*/

	// Print headder for stack trace.
	printf("                  PC   BP   SP   stack\n");
	printf("Initial values:   %d   %d   %d   \n", pc, bp, sp);

	while (halt)
	{
		// Fetch
		ir->opcode = text[pc].opcode;
		ir->l = text[pc].l;
		ir->m = text[pc].m;
		line_number = pc;
		pc++;

		// Execute
		switch(ir->opcode)
		{
			//LIT: Pushes a literal value "m" onto the stack.
			case 1:
				sp++;
				stack[sp] = ir->m;
				strcpy(ir->op, "LIT");
				break;

			// OPR: Performs some operation based on the value of m.
			case 2:
				switch (ir->m)
				{
					// RTN: Restore the environment of the caller.
					case 0:
						ar_delimiter_stack[bp - 1] = 0; // Facilitates print format
						stack[bp - 1] = stack[sp]; // Retain value on top of the stack.
						sp = bp - 1;
						bp = stack[sp + 2]; // Base of restored AR = DL of current AR.
						pc = stack[sp + 3]; // Restore program counter to return address.
						strcpy(ir->op, "RTN");
						break;

					case 1: // NEG
						stack[sp] = -1 * stack[sp];
						strcpy(ir->op, "NEG");
						break;

					case 2: // ADD
						sp--;
						stack[sp] = stack[sp] + stack[sp + 1];
						strcpy(ir->op, "ADD");
						break;

					case 3: // SUB
						sp--;
						stack[sp] = stack[sp] - stack[sp + 1];
						strcpy(ir->op, "SUB");
						break;

					case 4: // MUL
						 sp--;
						stack[sp] = stack[sp] * stack[sp + 1];
						strcpy(ir->op, "MUL");
						break;

					case 5: // DIV
						sp--;
						stack[sp] = stack[sp] / stack[sp + 1];
						strcpy(ir->op, "DIV");
						break;

					case 6: // ODD
						stack[sp] = stack[sp] % 2;
						strcpy(ir->op, "ODD");
						break;

					case 7: // MOD
						sp--;
						stack[sp] = stack[sp] % stack[sp + 1];
						strcpy(ir->op, "MOD");
						break;

					case 8: // EQL (==)
						sp--;
						stack[sp] = (stack[sp] == stack[sp + 1]);
						strcpy(ir->op, "EQL");
						break;

					case 9:	// NEQ (!=)
						sp--;
						stack[sp] = (stack[sp] != stack[sp + 1]);
						strcpy(ir->op, "NEQ");
						break;

					case 10: // LSS (<)
						sp--;
						stack[sp] = (stack[sp] < stack[sp + 1]);
						strcpy(ir->op, "LSS");
						break;

					case 11: // LEQ (<=)
						sp--;
						stack[sp] = (stack[sp] <= stack[sp + 1]);
						strcpy(ir->op, "LEQ");
						break;

					case 12: // GTR (>)
						sp--;
						stack[sp] = (stack[sp] > stack[sp + 1]);
						strcpy(ir->op, "GTR");
						break;

					case 13: // GEQ (>=)
						sp--;
						stack[sp] = (stack[sp] >= stack[sp + 1]);
						strcpy(ir->op, "GEQ");
						break;
				}
				break;

			// LOD: Pushes an element at "l" lexicographical levels down plus some offset
			// "m" onto the stack .
			case 3:
				sp++;
				stack[sp] = stack[base(stack, ir->l, bp) + ir->m];
				strcpy(ir->op, "LOD");
				break;

			// STO: Stores the element from the top of the stack at some index "l"
			// lexicographical levels down plus an offset "m".
			case 4:
				stack[base(stack, ir->l, bp) + ir->m] = stack[sp];
				sp--;
				strcpy(ir->op, "STO");
				break;

			// CAL: Calls a fuction whose instruction is indexed in the text by "m."
			case 5:
				ar_end_index = sp;
				stack[sp + 1] = base(stack, ir->l, bp);           // static link (SL)
				stack[sp + 2] = bp;                               // dynamic link (DL)
				stack[sp + 3] = pc;                               // return address (RA)
				stack[sp + 4] = stack[sp];                        // parameter (P)
				bp = sp + 1;                                      // new base pointer (BP)

				//Point program counter to instruction of the callee.
				pc = ir->m;
				strcpy(ir->op, "CAL");
				break;

			// INC: Used to increment the stack pointer by some offset "m."
			case 6:
				sp = sp + ir->m;
				strcpy(ir->op, "INC");
				break;

			// JMP: Sets the next instruction to one at index "m."
			case 7:
				pc = ir->m;
				strcpy(ir->op, "JMP");
				break;

			// JPC: Executes a conditional jump based on the value at the top of the stack.
			case 8:
				if (stack[sp] == 0)
					pc = ir->m;
				sp--;

				strcpy(ir->op, "JPC");
				break;

			// SYS: Initiates a system call based on the value of m.
			case 9:
				strcpy(ir->op, "SYS");
				switch(ir->m)
				{
					case 1:
						printf("Top of Stack Value: ");
						printf("%d\n", stack[sp]);
						sp--;
						break;

					case 2: // Receive input from user.
						sp++;
						printf("Please Enter an Integer: ");
						printf("\n");
						scanf("%d", &stack[sp]);
						break;

					case 3: // Halt program
						halt = 0;
						break;
				}
			break;

			default:
				printf("opcode error: This instruction does not exist.\n");
				break;
		}

		// Print current state.
		printf("%d %s  %d %d    %d    %d    %d", line_number, ir->op, ir->l, ir->m, pc, bp, sp);
		printf("  ");
		for(i = 0; i <= sp; i++)
		{
			printf(" %d", stack[i]);
			if (ar_delimiter_stack[i] == 1)
				printf(" |");
		}
		printf("\n");

		// Facilitates print format
		if (ir->opcode == 5)
			ar_delimiter_stack[ar_end_index] = 1;
	}

	return 0;
}
