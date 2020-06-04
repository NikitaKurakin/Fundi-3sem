#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#define STACK_OVERFLOW  -61600
#define STACK_UNDERFLOW -5051
#define BUF 256

typedef int T;
typedef char C;
typedef struct Stack {
    T data[BUF];
    size_t size;
} Stack;

size_t flag = 1;

void push(Stack *stack, const T value) {
    if (stack->size >= BUF) {
		printf("Stack is full\n");
        flag = 0;
        return -1;
    }
    stack->data[stack->size] = value;
    stack->size++;
}


T pop(Stack *stack) {
    if (stack->size == 0) {
        flag = 0;
        return -1;
    }
    stack->size--;
    return stack->data[stack->size];
}

T peek(const Stack *stack) {
    if (stack->size <= 0) {
        flag = 0;
        return -1;
    }
    return stack->data[stack->size - 1];
}

typedef struct Stack_c {
    C data[BUF];
    size_t size;
} Stack_c;

void push_c(Stack_c	 *stack, const C value) {
    if (stack->size >= BUF) {
        printf("Stack_c is full\n");
        flag = 0;
        return -1;
    }
    stack->data[stack->size] = value;
    stack->size++;
}

C pop_c(Stack_c *stack) {
    if (stack->size == 0) {
        flag = 0;
        return -1;
    }
    stack->size--;
    return stack->data[stack->size];
}

C peek_c(const Stack_c *stack) {
    if (stack->size <= 0) {
        flag = 0;
        return -1;
    }
    return stack->data[stack->size - 1];
}

size_t issimbol(char ch)
{
	return (ch == '+' || ch == '-' || ch == '*' || ch == '/' ||
			ch == '%' || ch == '^' || ch == '(' || ch == ')');
}

int priority(char ch)
{
	switch(ch)
	{
		case '^':
			return 4;
		case '*':
		case '/':
			return 3;
		case '%':
			return 2;
		case '+':
		case '-':
			return 1;

	}
}
int calc(int a, int b, char ch)
{
	switch(ch)
	{
		case '^':
			return pow(a, b);
		case '*':
			return a * b;
		case '/':
			return a / b;
		case '%':
			return a % b;
		case '+':
			return a + b;
		case '-':
			return a - b;
	}
}
int calculation(char* current, Stack* st)
{
	int tmp = 0;
	while(*current)
	{
		if (isdigit(*current))
			tmp = tmp * 10 + *current - '0';
		else if (issimbol(*current))
		{
			int a = calc(pop(st), pop(st), *current);
			push(st, a);
		}
		else if (*current = ' ')
		{
			push(st, tmp);
			tmp = 0;
		}
		current++;
	}
	return pop(st);
}



void writepolish(char* current, char* target, Stack_c* st_c)
{
	while(*current)
	{
		if (*current == '(')
		{
			push_c(st_c, *current);
			current++;
		}
		else if (*current == ')')
		{
			while (st_c->size > 0 && peek_c(st_c) != '(')
				*target++ = pop_c(st_c);
			pop_c(st_c);
			current++;
		}
		else if (isdigit(*current))
		{
			while(!issimbol(*current) && *current)
			{
				*target++ = *current++;
			}
			*target++ = ' ';
		}
		else if (issimbol(*current))
		{
			if (st_c->size == 0)
				push_c(st_c, *current);
			else if(priority(*current) > priority(peek_c(st_c))) push_c(st_c, *current);
				else
				{
					while(st_c->size > 0 && priority(*current) <= priority(peek_c(st_c)) && peek_c(st_c) != '(')
						{
							*target++ = pop_c(st_c);
						}
					push_c(st_c, *current);
				}
			current++;
        }

	}
	while (st_c->size > 0) *target++ = pop_c(st_c);
	*target = 0;
}


int main(int argc, char const *argv[])
{
	FILE *fIn, *fOut;


    if ((fIn = fopen(argv[1], "r")) == NULL)
    {
        printf("Ошибка! Не удаётся открыть файл ввода\n");
        exit(-2);
    }

	size_t count = 0;
    char ch;
    char buf[BUF], polish[BUF * 2], *bufptr = buf, *polishptr = polish;
	Stack st; Stack_c st_c;
	st.size = 0;
	st_c.size = 0;
	printf("%s\n", buf);
	fOut = fopen("Mistakes.txt", "w");
	while (!feof(fIn))
	{
		fscanf(fIn, "%s ", buf);
		printf("%s", buf);
		writepolish(bufptr, polishptr, &st_c);
		count++;
		if (!flag)
		{
			fprintf(fOut, "%ld %s\n", count, buf);
			printf("\n");
			flag = 1;
			continue;
		}
		printf("=%d\n", calculation(polishptr, &st));
	}
	return 0;
}

