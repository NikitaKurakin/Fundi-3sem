#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#define WOLF 4
#define GOAT 2
#define CABBAGE 1

const char *commands[3] = {"take", "put;", "move;"};
const char *objects[3] = {"cabbage", "goat", "wolf"};


int	take(FILE *fIn, size_t *left_coast, size_t *right_coast, size_t *boat)
{
	char ch;
	char word[7];
	char *ptr = word;
	int i;

	while ((ch = getc(fIn)) != '\n')
		*ptr++ = ch;
	*ptr = 0;
	for (i = 0; i < 3; i++)
	{
		if (!(strcmp(word, objects[i])))
		{
			if (!(*boat & 7))
			{
				*boat |= 1 << i;
				if ((*boat & 8))
				{
					if ((*right_coast & (1 << i)))
						*right_coast ^= 1 << i;
					else
					{
						printf("Ошибка take! На берегу нет %s\n", word);
						return (0);
					}
				}
				else
				{
					if ((*left_coast & (1 << i)))
						*left_coast ^= 1 << i;
					else
					{
						printf("Ошибка take! На берегу нет %s\n", word);
						return (0);
					}
				}
				break ;
			}
			else
			{
				printf("Ошибка take! Лодка полна\n");
				return (0);
			}
		}
	}
	return ((i < 3));
}

int	put(size_t *left_coast, size_t *right_coast, size_t *boat)
{
	if (!boat)
	{
		printf("Ошибка put! Лодка пуста\n");
		return (0);
	}
	else
	{
		if ((*boat & 8))
		{
			*right_coast |= *boat & 7;
			*boat &= 8;
		}
		else
		{
			*left_coast |= *boat & 7;
			*boat &= 8;
		}
	}
}

int	move(size_t *left_coast, size_t *right_coast, size_t *boat)
{
	if ((*boat & 8))
	{
		switch (*right_coast)
		{
			case 3:
				printf("Игра окончена. Коза съела капусту\n");
				return 0;
			case 6:
				printf("Игра окончена. Волк съел козу\n");
				return 0;
			case 7:
				printf("Игра окончена. Коза съела капусту. Волк съел козу\n");
				return 0;
			default:
				*boat ^= 8;
				break;
		}
	}
	else
	{
		switch (*left_coast)
		{
			case 3:
				printf("Игра окончена. Коза съела капусту\n");
				return 0;
			case 6:
				printf("Игра окончена. Волк съел козу\n");
				return 0;
			case 7:
				printf("Игра окончена. Коза съела капусту. Волк съел козу\n");
				return 0;
			default:
				*boat ^= 8;
				break;
		}

	}
}

int	choose(FILE *fIn, char *str, size_t *left_coast, size_t*right_coast, size_t *boat)
{

	int i;

	for (i = 0; i < 3; i++)
	{
		if (!(strcmp(commands[i], str)))
		{
			switch (i)
			{
				case 0:
					if (!take(fIn, left_coast, right_coast, boat))
						return 0;
					break;
				case 1:
					if (!put(left_coast, right_coast, boat))
						return 0;
					break;
				case 2:
					if (!move(left_coast, right_coast, boat))
						return 0;
					break;
			}
			break;
		};
	}
	return ((i < 3));
}

int	main(int argc, char **argv)
{
	FILE *fIn;
	size_t left_coast = 7, right_coast = 0, boat = 0;
	char ch;
	char word[7];
	char *ptr = word;

	if (argc != 2)
    {
        printf("Ошибка! Неверное количество аргументов\n");
        exit(-1);
    }

    if ((fIn = fopen(argv[1], "r")) == NULL)
    {
        printf("Ошибка! Не удаётся открыть файл ввода\n");
        exit(-2);
    }

	while ((ch = getc(fIn)) != EOF)
	{
		if (isalpha(ch) || ch == ';')
			*ptr++ = ch;
		if (isspace(ch))
		{
			*ptr = 0;
			if (!(choose(fIn, word, &left_coast, &right_coast, &boat)))
				return 0;
			ptr = word;
		}
	}
	if (right_coast == 7)
		printf("Задача решена верно\n");
	else
		printf("Задача решена неверно\n");
	return 0;
	fclose(fIn);
}
