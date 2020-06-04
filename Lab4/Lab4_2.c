#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define BUFF_MAX 100

#define isoper(c) (c == '+' || c == '-' || c == '*' || c == '/' || c == '%')

typedef struct Memory_cell
{
	char *name;
	int data;
}Memory_cell, *Mem_cell;

Mem_cell add_var(Mem_cell terms, int *count, char *name);
int search(Mem_cell terms, int count, char *name);
int oper(int a, char o, int b, int *res);
int comp (const Memory_cell *fst, const Memory_cell *scd);
int compare(char *fst, char *scd);

int main(int argc, char *argv[]) {

	FILE *In;
	char buff[BUFF_MAX], *bufptr, names[3][BUFF_MAX], *namesptr;
	Mem_cell terms = NULL;
	int count = 0, num1, num2, flag, operation, i, res = 0;

	if(argc < 1)
	{
		printf("Problem with file!\n");
		exit(-1);
	}

	if((In = fopen(argv[1], "r")) == 0)
	{
		printf("There is a problem with input file!\n");
		exit(-2);
	}

	while (!feof(In))
	{
		fgets(buff, BUFF_MAX, In);
		bufptr = buff;
		flag = 0;
		num1 = 0, num2 = 0;

		namesptr = names[0];

		while (*bufptr != ' ' && *bufptr != '=' && *bufptr != ';')
			*namesptr++ = *bufptr++;

		*namesptr = '\0';
		namesptr = names[1];

		switch(*bufptr)
		{
			case ';':
				for (i = 0; i < count; ++i)
					printf("%s=%d\n", (terms + i)->name, (terms + i)->data);
				continue;

			case ' ':
				bufptr++;
				while (*bufptr != ';')
					*namesptr++ = *bufptr++;
				*namesptr = '\0';
				res = search(terms, count, names[1]);
				if (res != -1)
					printf("%s=%d\n", (terms + res)->name, (terms + res)->data);
				else
					printf("There is no such variable as %s\n", names[1]);
				continue;

			case '=':
				bufptr++;
				while (*bufptr != ';')
				{
					if (isoper(*bufptr))
					{
						flag = 1;
						*namesptr = '\0';
						namesptr = names[2];
						operation = *bufptr;
						bufptr++;
					}
					*namesptr++ = *bufptr++;
				}
				*namesptr = '\0';
				break;
		}

		namesptr = names[1];
		if (isdigit(*namesptr))
			while (*namesptr)
				num1 = num1 * 10 + *namesptr++ - '0';
		else if (search(terms, count, names[1]) == -1)
		{
			printf("There is no such variable as %s\n", names[1]);
			continue;
		}
		else
			num1 = (terms + (search(terms, count, names[1])))->data;

		if (flag > 0)
		{
			namesptr = names[2];
			if (isdigit(*namesptr))
				while (*namesptr)
					num2 = num2 * 10 + *namesptr++ - '0';
			else if (search(terms, count, names[2]) == -1)
			{
				printf("There is no such variable as %s\n", names[2]);
				continue;
			}
			else
			{
				num2 = (terms + (search(terms, count, names[2])))->data;
			}
			if (search(terms, count, names[0]) == -1)
				terms = add_var(terms, &count, names[0]);
			if (!oper(num1, operation, num2, &res))
				(terms + (search(terms, count, names[0])))->data = res;
			else
				printf("Can't divide by 0\n");
			continue;
		}
		if (search(terms, count, names[0]) == -1)
				terms = add_var(terms, &count, names[0]);
		(terms + (search(terms, count, names[0])))->data = num1;

	}

	return 0;
}

Mem_cell add_var(Mem_cell terms, int *count, char *name)
{
	size_t k;

	terms = (Mem_cell)realloc(terms, sizeof(Memory_cell) * (*count + 1));
	(terms + *count)->name = (char *)malloc(sizeof(char) * (strlen(name) + 1));
	strcpy((terms + *count)->name, name);
	(*count)++;
	k = *count;
	qsort(terms, k, sizeof (Memory_cell), (int(*) (const void *, const void *)) comp);

	return terms;
}

int comp(const Memory_cell *fst, const Memory_cell *scd)
{
	char *first = fst->name, *second = scd->name;

	while (*first && *second)
	{
		if (*first > *second)
			return 1;
		else if (*first < *second)
			return -1;
		else
			first++, second++;
	}

	if (*first == '\0' && *second == '\0')
		return 0;
	else if (*first = '\0')
		return -1;
	else
		return 1;
}

int search(Mem_cell terms, int count, char *name)
{
	int mid = 0, left = 0, right = count;

	if (count == 0)
		return -1;

	while (1)
	{
		mid = (left + right) / 2;
		if (mid == count)
			return -1;

		if (compare(name, (terms + mid)->name) == -1)
			right = mid - 1;
		else if (compare(name, (terms + mid)->name) == 1)
			left = mid + 1;
		else
			return mid;

		if (left > right)
			return -1;
	}
}

int compare(char *fst, char *scd)
{
	char *first = fst, *second = scd;

	while (*first && *second)
	{
		if (*first > *second)
			return 1;
		else if (*first < *second)
			return -1;
		else
			first++, second++;
	}

	if (*first == '\0' && *second == '\0')
		return 0;
	else if (*first = '\0')
		return -1;
	else
		return 1;
}

int oper(int a, char o, int b, int *res)
{
	switch(o)
	{
	case '+':
		*res = a + b;
	case '-':
		*res = a - b;
	case '*':
		*res = a * b;
	case '/':
		if (b == 0)
			return 1;
		*res = a / b;
	case '%':
		if (b == 0)
			return 1;
		*res = a % b;
	}

	return 0;
}
