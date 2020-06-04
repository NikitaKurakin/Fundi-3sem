#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define BUF_SIZE 4096

typedef struct t_priority
{
	struct t_priority *next;
	int priority;
	char *data;
}		Priority;

Priority *new_prior(int priority, char *data)
{
	Priority *new;
	new = (Priority*)malloc(sizeof(Priority));
	new->next = NULL;
	new->priority = priority;
	new->data = (char*)malloc(sizeof(char) * (strlen(data) + 1));
	strcpy(new->data, data);
	return (new);
}

void Enqueue(Priority **prior, int priority, char *data)
{
	Priority *new;
	Priority *shift;

	new = new_prior(priority, data);
	shift = *prior;
	if (!shift)
		*prior = new;
	while (shift->next)
		shift = shift->next;
	shift->next = new;
}

char *Dequeue(Priority **prior)
{
	char *str;
	Priority *temp;

	temp = *prior;
	if (!temp)
		return (NULL);
	*prior = (*prior)->next;
	str = temp->data;
	free(temp);
	return (str);
}

void EnqueuePriority(Priority **prior, int priority, char *data)
{
	Priority *new;
	Priority *shift = *prior;

	new = new_prior(priority, data);
	if (!*prior)
	{
		*prior = new;
		return ;
	}
	if (new->priority < shift->priority)
	{
		new->next = *prior;
		*prior = new;
	}
	while (shift->next && shift->next->priority < new->priority)
		shift = shift->next;
	new->next = shift->next;
	shift->next = new;
}

int	my_strlen(char *str)
{
	int size = 0;
	while (str[size++]);
	return(size);
}

void parser(Priority **prior, char *str)
{
	int priority = 0;

	str = strchr(str, '=');
	while (*++str != ' ')
		priority = priority * 10 + *str -'0';
	str = strchr(str, '=');
	str += 2;
	str[strlen(str) - 2] = 0;
	EnqueuePriority(prior, priority, str);
}

void print_struct(Priority *prior)
{
	while (prior)
	{
		printf("Приоритет: %d Задание: %s\n", prior->priority, prior->data);
		prior = prior->next;
	}
}


void Cleaner(Priority **lst)
{
	Priority *shift = (*lst)->next;
	Priority *temp = *lst;

	
	while (shift)
	{
		free(temp);
		temp = shift;
		shift = shift->next;
	}
	*lst = NULL;
}

int main(int argc, char **argv)
{
	FILE *fIn;
	Priority *prior = NULL;
	char buf[BUF_SIZE];
	char *ptr = buf; 

	for (int i = 1; i < argc; i++)
	{
		if ((fIn = fopen(argv[i], "r")) == NULL)
    	{
        	printf("Ошибка! Не удаётся открыть файл ввода\n");
        	exit(-2);
    	}
		while ((ptr = fgets(buf, BUF_SIZE, fIn)))
		{
			parser(&prior, ptr);
		}
		fclose(fIn);
	}
	print_struct(prior);
	Cleaner(&prior);
}