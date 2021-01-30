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
	int halt = 1;
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

	while (halt)
	{
		// Fetch
		ir->opcode = text[pc].opcode;
		ir->l = text[pc].l;
		ir->m = text[pc].m;
		pc += 1;

		// Execute
		switch(ir->opcode)
		{
			//LIT: Pushes a literal value "m" onto the stack.
			case 1:
				sp++;
				stack[sp] = ir->m;
				strcpy("LIT", ir->op);
				break;

			// OPR: Performs some operation based on the value of m.
			case 2:
				switch (ir->m)
				{
					// RTN: Restore the environment of the caller.
					case 0:
						stack[bp - 1] = stack[sp]; // Retain value on top of the stack.
						sp = bp - 1;
						bp = stack[sp + 2]; // Base of restored AR = DL of current AR.
						pc = stack[sp + 3]; // Restore program counter to return address.
						strcpy("RTN", ir->op);
						break;

					case 1: // NEG
						stack[sp] = -1 * stack[sp];
						strcpy("NEG", ir->op);
						break;

					case 2: // ADD
						sp--;
						stack[sp] = stack[sp] + stack[sp + 1];
						strcpy("ADD", ir->op);
						break;

					case 3: // SUB
						sp--;
						stack[sp] = stack[sp] - stack[sp + 1];
						strcpy("SUB", ir->op);
						break;

					case 4: // MUL
						 sp--;
						stack[sp] = stack[sp] * stack[sp + 1];
						strcpy("MUL", ir->op);
						break;

					case 5: // DIV
						sp--;
						stack[sp] = stack[sp] / stack[sp + 1];
						strcpy("DIV", ir->op);
						break;

					case 6: // ODD
						stack[sp] = stack[sp] mod 2;
						strcpy("ODD", ir->op);
						break;

					case 7: // MOD
						sp--;
						stack[sp] = stack[sp] % stack[sp + 1];
						strcpy("MOD", ir->op);
						break;

					case 8: // EQL (==)
						sp--;
						stack[sp] = (stack[sp] == stack[sp + 1]);
						strcpy("EQL", ir->op);
						break;

					case 9:	// NEQ (!=)
						sp--;
						stack[sp] = (stack[sp] != stack[sp + 1]);
						strcpy("NEQ", ir->op);
						break;

					case 10: // LSS (<)
						sp--;
						stack[sp] = (stack[sp] < stack[sp + 1]);
						strcpy("LSS", ir->op);
						break;

					case 11: // LEQ (<=)
						sp--;
						stack[sp] = (stack[sp] <= stack[sp + 1]);
						strcpy("LEQ", ir->op);
						break;

					case 12: // GTR (>)
						sp--;
						stack[sp] = (stack[sp] > stack[sp + 1]);
						strcpy("GTR", ir->op);
						break;

					case 13: // GEQ (>=)
						sp--;
						stack[sp] = (stack[sp] >= stack[sp + 1]);
						strcpy("GEQ", ir->op);
						break;
				}
				break;

			// LOD: Pushes an element at "l" lexicographical levels down plus some offset
			// "m" onto the stack .
			case 3:
				sp++;
				stack[sp] = stack[base(stack, ir->l, bp) + ir.m];
				strcpy("LOD", ir->op);
				break;

			// STO: Stores the element from the top of the stack at some index "l"
			// lexicographical levels down plus an offset "m".
			case 4:
				stack[base(stack, ir->l, bp) + ir->m] = stack[sp];
				sp--;
				strcpy("STO", ir->op);
				break;

			// CAL: Calls a fuction whose instruction is indexed in the text by "m."
			case 5:
				stack[sp + 1] = base(stack, ir->l, bp);           // static link (SL)
				stack[sp + 2] = bp;                               // dynamic link (DL)
				stack[sp + 3] = pc;                               // return address (RA)
				stack[sp + 4] = stack[sp];                        // parameter (P)
				bp = sp + 1;                                      // new base pointer (BP)

				//Point program counter to instruction of the callee.
				pc = ir->m;
				strcpy("CAL", ir->op);
				break;

			// INC: Used to increment the stack pointer by some offset "m."
			case 6:
				sp = sp + ir->m;
				strcpy("INC", ir->op);
				break;

			// JMP: Sets the next instruction to one at index "m."
			case 7:
				pc = ir->m;
				strcpy("JMP", ir->op);
				break;

			// JPC: Executes a conditional jump based on the value at the top of the stack.
			case 8:
				if (stack[sp] == 0)
				{
					pc = ir->m;
					sp--;
				}

				strcpy("JPC", ir->op);
				break;

			// SYS: Initiates a system call based on the value of m.
			case 9:
				strcpy("SYS", ir->op);
				switch(ir->m)
				{
					case 1:
						printf("%d", stack[sp]);
						sp--;
						break;

					case 2: // Receive input from user.
						sp++;
						// NOTE: Initiate waiting loop here until user inputs value/
						scanf("%d", stack[sp]);
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
