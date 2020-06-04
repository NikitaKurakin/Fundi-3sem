#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

typedef struct Data
{
	char surname[20];
	char secondname[20];
	char name[20];
	int tm_mday;
	int tm_mon;
	int tm_year;
	char sex;
	int avaragein;
}Data;

typedef struct Node
{
	Data data;
	struct Node *next;
}Node, *PNode;

PNode addperson(PNode people, Data in);
void search(PNode people, Data in);
PNode delete(PNode people, Data in, int *key);

int main()
{
	FILE *fIn = fopen("in4.txt", "r"), *fOut;;
	PNode people = NULL, list;
	Data in;
	int key, flag = 0;

	if (!fIn)
	{
		printf("Can't open input file!!!\n");
		exit(1);
	}

	while (!feof(fIn))
	{
		fscanf(fIn, "%s %s %s %d.%d.%d %c %d", in.surname, in.name, in.secondname, &(in.tm_mday), &(in.tm_mon), &(in.tm_year), &(in.sex), &(in.avaragein));
		people = addperson(people, in);
	}

	while (1)
	{
		if (flag)
			break;

		printf("\nPlease, choose what to do: \n(1)-to search \n(2)-to add person \n(3)-to delete person \n(4)-to uploade list to a file \n(5)-to print list \n(6)-to exit\n");
		scanf("%d", &key);

		switch (key)
		{
			case 1:
				printf("Enter surname, name and secondname to search person\n");
				scanf("%s%s%s", in.surname, in.name, in.secondname);
				search(people, in);
				break;

			case 2:
				printf("Enter surname, name, secondname, date of birth, sex(m/f) and average income\n");
				scanf("%s %s %s %d.%d.%d %c %d", in.surname, in.name, in.secondname, &(in.tm_mday), &(in.tm_mon), &(in.tm_year), &(in.sex), &(in.avaragein));
				people = addperson(people, in);
				printf("Successfully add %s\n", in.name);
				break;

			case 3:
				printf("Enter surname, name and secondname to delete person from list\n");
				scanf("%s%s%s", in.surname, in.name, in.secondname);
				people = delete(people, in, &key);

				if (key == 0)
					printf("Successfully delete %s\n", in.name);
				else
					printf("Can't find %s %s %s in list\n", in.surname, in.name, in.secondname);
				break;

			case 4:
				if (!(fOut = fopen("out4.txt", "w")))
				{
					printf("Can't create output file!!!\n");
					break;
				}
				list = people;

				while(list != NULL)
				{
					fprintf(fOut, "%s %s %s %d.%d.%d %c %d\n", list->data.surname, list->data.name, list->data.secondname, list->data.tm_mday, list->data.tm_mon, list->data.tm_year, list->data.sex, list->data.avaragein);
					list = list->next;
				}

				fclose(fOut);
				printf("Successfully uploade to file 'out4.txt'\n");
				break;

			case 5:
				list = people;
				while (list != NULL)
				{
					printf("%s %s %s %d.%d.%d %c %d\n", list->data.surname, list->data.name, list->data.secondname, list->data.tm_mday, list->data.tm_mon, list->data.tm_year, list->data.sex, list->data.avaragein);
					list = list->next;
				}
				break;

			case 6:
				flag = 1;
				break;

			default:
				printf("Wrong move\n");
				break;
		}
	}

	while (people != NULL)
	{
		list = people;
		people = people->next;
		free(list);
	}

	fclose(fIn);
	return 0;
}

PNode addperson(PNode people, Data in)
{
	PNode list = people, new;

	if (people != NULL)
	{
		if ((list->data.tm_year == in.tm_year
		 && list->data.tm_mon == in.tm_mon && list->data.tm_mday
		  <= in.tm_mday) || (list->data.tm_year == in.tm_year &&
		   list->data.tm_mon <= in.tm_mon) || (list->data.tm_year <= in.tm_year))
		{
			new = (PNode)malloc(sizeof(Data));
			strcpy(new->data.surname, in.surname);
			strcpy(new->data.secondname, in.secondname);
			strcpy(new->data.name, in.name);
			new->data.tm_mday = in.tm_mday;
			new->data.tm_mon = in.tm_mon;
			new->data.tm_year = in.tm_year;
			new->data.sex = in.sex;
			new->data.avaragein = in.avaragein;
			new->next = list;
			return new;
		}

		while (list->next != NULL && ((list->next->data.tm_year == in.tm_year
		 && list->next->data.tm_mon == in.tm_mon && list->next->data.tm_mday
		  >= in.tm_mday) || (list->next->data.tm_year == in.tm_year &&
		   list->next->data.tm_mon >= in.tm_mon) || (list->next->data.tm_year >= in.tm_year)))
		{
			list = list->next;
		}

		new = (PNode)malloc(sizeof(Data));
		new->next = NULL;
		strcpy(new->data.surname, in.surname);
		strcpy(new->data.secondname, in.secondname);
		strcpy(new->data.name, in.name);
		new->data.tm_mday = in.tm_mday;
		new->data.tm_mon = in.tm_mon;
		new->data.tm_year = in.tm_year;
		new->data.sex = in.sex;
		new->data.avaragein = in.avaragein;

		if (list->next == NULL)
			list->next = new;
		else
		{
			new->next = list->next;
			list->next = new;
		}
	}
	else
	{
		people = (PNode)malloc(sizeof(Data));
		people->next = NULL;
		strcpy(people->data.surname, in.surname);
		strcpy(people->data.secondname, in.secondname);
		strcpy(people->data.name, in.name);
		people->data.tm_mday = in.tm_mday;
		people->data.tm_mon = in.tm_mon;
		people->data.tm_year = in.tm_year;
		people->data.sex = in.sex;
		people->data.avaragein = in.avaragein;
	}

	return people;
}

void search(PNode people, Data in)
{
	PNode list = people;
	int flag = 0;

	if (list == NULL)
	{
		printf("Can't search, list is empty");
		return;
	}

	while (1)
	{
		if (!strcmp(list->data.name, in.name) && !strcmp(list->data.surname, in.surname)
		 && !strcmp(list->data.secondname, in.secondname))
		{
			printf("%s %s %s %d.%d.%d %c %d", list->data.surname, list->data.name,
			 list->data.secondname, list->data.tm_mday, list->data.tm_mon,
			  list->data.tm_year, list->data.sex, list->data.avaragein);
			flag = 1;
		}

		if (list->next != NULL)
			list = list->next;
		else
			break;
	}

	if (flag == 0)
		printf("Can't find %s %s %s in list\n", in.surname, in.name, in.secondname);
}

PNode delete(PNode people, Data in, int *key)
{
	PNode list = people, del;
	*key = 1;

	if (list == NULL)
		return people;

	while (list->next != NULL)
	{
		if (!strcmp(list->data.name, in.name) && !strcmp(list->data.surname, in.surname)
		 && !strcmp(list->data.secondname, in.secondname))
		{
			*key = 0;
			del = list->next;
			free(list);
			return del;
		}

		if (!strcmp(list->next->data.name, in.name) && !strcmp(list->next->data.surname, in.surname)
		 && !strcmp(list->next->data.secondname, in.secondname))
		{
			del = list->next;
			list->next = list->next->next;
			free(del);
			*key = 0;
			return people;
		}

		list = list->next;
	}
}
