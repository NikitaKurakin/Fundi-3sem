#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define BUF_SIZE 256


int main(int argc, char const *argv[])
{
	FILE *fIn, *fOut;
	char str[BUF_SIZE], *strptr, *bufptr;

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

	if ((fOut = fopen("fOut.txt", "w")) == NULL)
	{
		printf("Ошибка! Не удаётся открыть файл вывода\n");
		exit(-3);
	}

	char c;
    char *buf[3];
    for (int i = 0; i < 3; ++i) {
        buf[i] = malloc(sizeof(char) * 2);
    }

    int count = 0;
    int i = 0;
    while ((c = fgetc(fIn)) != EOF) {
        if (c != '\n') {
            if (c != ' ') {
                buf[count] = realloc(buf[count], sizeof(buf[count]) + 1);
                buf[count][i] = c;
                i++;
            } else {
                i = 0;
                count++;
            }
        } else {
            printf("%s %s %s \n", buf[2], buf[0], buf[1]);
            fprintf(fOut, "%s %s %s \n", buf[2], buf[0], buf[1]);
            count = 0;
            for (int i = 0; i < 3; ++i) {
                buf[i] = malloc(sizeof(char) * 5);
            }
            i = 0;
        }
    }
    if (count)
    {
        printf("%s %s %s\n", buf[2], buf[0], buf[1]);
        fprintf(fOut, "%s %s %s \n", buf[2], buf[0], buf[1]);
    }
    for (int i = 0; i < 3; ++i) {
        free(buf[i]);
    }

	fclose(fIn);
	fclose(fOut);
	return 0;
}
