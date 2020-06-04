#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>
#define BUFF_SIZE 30
#define NUMBER_OPERATION 11
#define MAX(a, b) a > b ? a : b
#define ABS(a) a < 0 ? -a : a

typedef struct
{
	int size;
	int *array;
}	vector;

vector vector_array[26];

void load(char *str);
void save(char *str);
void concat(char *str);
void randomize(char *str);
void my_free(char *str);
void my_remove(char *str);
void copy(char *str);
void sort(char *str);
void permute(char *str);
void stat(char *str);
void print(char *str);
int comp_minus(const int *a, const int *b);
int comp_plus(const int *a, const int *b);
void strtolower(char *str);
void call_function(char *str);
void print_error(int a, char *str, char c);

int main(int argc, char **argv)
{
	FILE	*f1, *f_array;
	char	str[BUFF_SIZE];

	if (!(f1 = fopen(argv[1], "r")))
		print_error(0, argv[1], 0);
	while (fgets(str, BUFF_SIZE, f1))
	{
		call_function(str);
		printf("\n");
	}
}


void load(char *str)
{
	FILE *f;
	int temp = 0, flag = 0;
	int el;
	char c;
	int sign = 1;

	el = *(str + 5) - 'a';
	str[strlen(str) - 2] = 0;
	printf("%s\n", str);
	if (!(f = fopen(str + 8, "r")))
		print_error(0, str + 8, 0);
	if (!vector_array[el].size)
	{
		vector_array[el].size = 1;
		vector_array[el].array = (int*)malloc(sizeof(int));
	}
	while ((c = fgetc(f)))
	{
		if (c == '-')
		{
			sign = -1;
			continue ;
		}
		if (isdigit(c) && (flag = 1))
			temp = temp * 10 + sign * (c - '0');
		else
		{
			if (flag == 1)
				vector_array[el].array[vector_array[el].size - 1] = temp;
				if (c == EOF)
					break ;
			if (flag == 1)
			{
				vector_array[el].size++;
				vector_array[el].array = realloc(vector_array[el].array, sizeof(int) * vector_array[el].size);
				temp = 0;
				sign = 1;
			}
				flag = 0;
		}
	}
	fclose(f);
}

void save(char *str)
{
	FILE *f;
	int el;
	char c;

	el = *(str + 5) - 'a';
	str[strlen(str) - 2] = 0;
	if (!(f = fopen(str + 8, "w")))
		print_error(0, str + 8, 0);
	if (!vector_array[el].size)
		print_error(1, 0, str[5]);
	for (int i = 0; i < vector_array[el].size; i++)
		if (i != vector_array[el].size - 1)
			fprintf(f, "%d ", vector_array[el].array[i]);
		else
			fprintf(f, "%d", vector_array[el].array[i]);
	fclose(f);
}

void concat(char *str)
{
	int el1, el2;

	el1 = str[7] - 'a';
	el2 = str[10] - 'a';
	if (!vector_array[el1].size)
		print_error(1, 0, el1 + 'a');
	if (!vector_array[el2].size)
		print_error(1, 0, el2 + 'a');
	vector_array[el1].size += vector_array[el2].size;
	vector_array[el1].array = (int*)realloc(vector_array[el1].array, sizeof(int) * vector_array[el1].size);
	for (int j = 0, i = vector_array[el1].size - vector_array[el2].size; i < vector_array[el1].size; i++, j++)
		vector_array[el1].array[i] = vector_array[el2].array[j];
}

void randomize(char *str)
{
	int el;
	int count = 0;
	int sign = 1;
	int lb = 0, lr = 0;

	srand(time(NULL));
	el = str[5] - 'a';
	str += 7;
	count = atoi(str);
	str = strchr(str, ',');
	lb = atoi(++str);
	str = strchr(str, ',');
	lr = atoi(++str);
	vector_array[el].size = count;
	vector_array[el].array = (int*)malloc(sizeof(int) * count);
	for (int i = 0; i < count; i++)
		vector_array[el].array[i] = rand() % ((ABS(lb)) + (ABS(lr)) + 1) - (ABS(lb));
}

void my_free(char *str)
{
	int el;

	el = str[5] - 'a';
	vector_array[el].size = 0;
	free(vector_array[el].array);
}

void my_remove(char *str)
{
	int el;
	int count = 0, index = 0;

	el = str[7] - 'a';
	str += 10;
	index = atoi(str);
	str = strchr(str, ',');
	count = atoi(++str);
	if (index + count >= vector_array[el].size)
		print_error(2, 0, el + 'A');
	for (int i = index; i < count + index && i + count < vector_array[el].size; i++)
		vector_array[el].array[i] = vector_array[el].array[i + count];
	vector_array[el].size -= count;
	vector_array[el].array = (int*)realloc(vector_array[el].array, sizeof(int) * vector_array[el].size);
}

void copy(char *str)
{
	int el1, el2;
	int index1, index2;

	el1 = str[5] - 'a';

	str = strchr(str, ',');
	index1 = atoi(++str);
	str = strchr(str, ',');
	index2 = atoi(++str);
	el2 = str[strlen(str) - 3] - 'a';
	if (index2 >= vector_array[el1].size || index1 >= vector_array[el1].size)
		print_error(1, 0, el2 + 'A');
	vector_array[el2].size += (index2 - index1 + 1);
	vector_array[el2].array = (int*)realloc(vector_array[el2].array, sizeof(int) * vector_array[el2].size);
	for (int j = index1, i = vector_array[el2].size - (index2 - index1 + 1); i < vector_array[el2].size; j++, i++)
		vector_array[el2].array[i] = vector_array[el1].array[j];
}

void sort(char *str)
{
	int el;
	int temp;

	el = str[5] - 'a';
	if (!vector_array[el].size)
		print_error(1, 0, el + 'A');
	qsort(vector_array[el].array, vector_array[el].size, sizeof(int), (int(*) (const void *, const void *)) comp_plus);
	if (str[6] == '+')
		qsort(vector_array[el].array, vector_array[el].size, sizeof(int), (int(*) (const void *, const void *)) comp_plus);
	else
		qsort(vector_array[el].array, vector_array[el].size, sizeof(int), (int(*) (const void *, const void *)) comp_minus);
}

void permute(char *str)
{
	int el;
	int temp;
	int j;

	srand(time(NULL));
	el = str[8] - 'a';
	for (int i = 0; i < vector_array[el].size; i++)
	{
		j = rand() % vector_array[el].size;
		temp = vector_array[el].array[j];
		vector_array[el].array[j] = vector_array[el].array[i];
		vector_array[el].array[i] = temp;
	}

}

void stat(char *str)
{
	int el;
	int min, max;
	int min_index, max_index;
	int sum = 0;
	int freq, freq_max_count = 1;
	int count = 0;

	el = str[5] - 'a';
	if (!vector_array[el].size)
		print_error(1, 0, el + 'A');
	printf("%c:\n", str[5]);
	printf("Размер: %d\n", vector_array[el].size);
	freq = min = max = vector_array[el].array[0];
	min_index = max_index = 0;
	for (int i = 0; i < vector_array[el].size; i++)
	{
		count = 0;
		for (int j = 0; j < vector_array[el].size; j++)
		{
			if (vector_array[el].array[j] == vector_array[el].array[i])
				count++;
		}
		if (count > freq_max_count)
		{
			freq_max_count = count;
			freq = vector_array[el].array[i];
		}
		sum += vector_array[el].array[i];
		if (min > vector_array[el].array[i])
		{
			min = vector_array[el].array[i];
			min_index = i;
		}
		if (max < vector_array[el].array[i])
		{
			max = vector_array[el].array[i];
			max_index = i;
		}
	}
	sum = sum / vector_array[el].size;
	printf("Максимальный элемент: %d; Индекс: %d\n", max, max_index);
	printf("Минимальный элемент: %d; Индекс: %d\n", min, min_index);
	printf("Среднее: %d\n", sum);
	printf("Наиболее часто встречающийся элемент: %d\n",freq);
	printf("Максимальная разница: %d\n", MAX(ABS(sum - max), ABS(sum - min)));
}

void print(char *str)
{
	if (strstr(str, "all"))
	{
		printf("%c: ", toupper(str[6]));
		for (int i = 0; i < vector_array[str[6] - 'a'].size; i++)
			printf("%d ", vector_array[str[6] - 'a'].array[i]);
		printf("\n");
	}
	else
	{
		int start = 0, end = 0, i = 1;
		char *ptr = str;
		ptr += 9;
		while(isdigit(*ptr))
		{
			start = start * i + (*ptr - '0');
			i *= 10;
			ptr++;
		}
		ptr += 2;
		i = 1;
		while(isdigit(*ptr))
		{
			end = end * i + *ptr - '0';
			i *= 10;
			ptr++;
		}
		if (start > vector_array[str[6] - 'a'].size || end >= vector_array[str[6] - 'a'].size)
			print_error(2, str, 0);
		for (int i = start; i <= end; ++i)
		{
			printf("%d ", vector_array[str[6] - 'a'].array[i]);
		}
		printf("\n");
	}
}


int comp_minus(const int *a, const int *b)
{
	return (*b - *a);
}

int comp_plus(const int *a, const int *b)
{
	return (*a - *b);
}

void strtolower(char *str)
{
	while (*str)
		*str++ = tolower(*str);
}

void call_function(char *str)
{
	void(*function[11])(char*) = {
		load, save, randomize, concat,
		my_free, my_remove, copy, sort,
		permute, stat, print
	};

	char *str_function[NUMBER_OPERATION] = {
		"load", "save", "rand", "concat",
		"free", "remove", "copy", "sort",
		"permute", "stat", "print"
	};
	strtolower(str);
	for (int i = 0; i < NUMBER_OPERATION; i++)
	{
		if (strstr(str, str_function[i]))
		{
			printf("%s\n", str);
			function[i](str);
			break ;
		}
	}

}

void print_error(int a, char *str, char c)
{
	if (a == 0)
		printf("Отсутствует файл: %s\n", str);
	if (a == 1)
		printf("Отсутствует массив: %c\n", c);
	if (a == 2)
		printf("Выход за границу массива: %s\n", str);
	exit(a);
}

