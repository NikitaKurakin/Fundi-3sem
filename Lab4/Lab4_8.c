//через командную строку передать in8.txt и out8.txt
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>

#define HASHSIZE 128

typedef struct _Node {
    unsigned long long int def_name;
    char* value;
    struct _Node *next;
    struct _Node *prev;
} Node;


typedef struct _DblLinkedList {
    size_t size;
    Node *head;
    Node *tail;
} DblLinkedList;

DblLinkedList* createDblLinkedList() {
    DblLinkedList *tmp = (DblLinkedList*) malloc(sizeof(DblLinkedList));
    tmp->size = 0;
    tmp->head = tmp->tail = NULL;

    return tmp;
}

void deleteDblLinkedList(DblLinkedList **list) {
    Node *tmp = (*list)->head;
    Node *next = NULL;
    while (tmp) {
        next = tmp->next;
        free(tmp->value);
        free(tmp);
        tmp = next;
    }
    free(*list);
    (*list) = NULL;
}


void pushFront(DblLinkedList *list, unsigned long long def_name, char *value) {
    Node *tmp = (Node*) malloc(sizeof(Node));
    if (tmp == NULL) {
        exit(1);
    }
    tmp->def_name = def_name;
    tmp->value = (char*)malloc(strlen(value) * sizeof(char) + 1);
    strcpy(tmp->value, value);
    tmp->next = list->head;
    tmp->prev = NULL;
    if (list->head) {
        list->head->prev = tmp;
    }
    list->head = tmp;

    if (list->tail == NULL) {
        list->tail = tmp;
    }
    list->size++;
}

void pushBack(DblLinkedList *list, unsigned long long def_name, char *value) {
    Node *tmp = (Node*) malloc(sizeof(Node));
    if (tmp == NULL) {
        exit(3);
    }
    tmp->def_name = def_name;
    tmp->value = (char*)malloc(strlen(value) * sizeof(char) + 1);
    strcpy(tmp->value, value);
    tmp->next = NULL;
    tmp->prev = list->tail;
    if (list->tail) {
        list->tail->next = tmp;
    }
    list->tail = tmp;

    if (list->head == NULL) {
        list->head = tmp;
    }
    list->size++;
}

void insert(DblLinkedList *list, unsigned long long def_name, char *value) {
    Node *elm = list->head;
    Node *ins = NULL;

    while (elm->def_name > def_name) {
        elm = elm->next;
    }
    ins = (Node*) malloc(sizeof(Node));
    ins->def_name = def_name;
    ins->value = (char*)malloc(strlen(value) * sizeof(char) + 1);
    strcpy(ins->value, value);
    ins->prev = elm->prev;
    ins->next = elm;
    ins->prev->next = ins;
    elm->prev = ins;
    list->size++;
}

void printDblLinkedList(DblLinkedList *list) {
    Node *tmp = list->head;
    while (tmp) {
        printf("%lld, %s ", tmp->def_name, tmp->value);
        tmp = tmp->next;
    }
    printf("\n");
}

size_t HASH_FUN(unsigned long long def_name)
{
    return  def_name % 128;
}

unsigned long long conversion (char *ptr)
{
    unsigned long long def_name = 0, pow = 1;
    while (*ptr)
    {
        if isdigit(*ptr)
            def_name += (*ptr-- - '0') * pow;
        else if(toupper(*ptr) == *ptr)
            def_name += (*ptr-- - 'A' + 10) * pow;
        else def_name += (*ptr-- - 'a' + 36) * pow;
        pow *= 64;
    }
    return def_name;
}

char* FindReplace(DblLinkedList *list, unsigned long long def_name)
{
    if (list->head == NULL)
        return  NULL;
    if (def_name > list->head->def_name || def_name < list->tail->def_name)
        return NULL;
    Node *tmp = list->head;
    while(tmp)
    {
        if (tmp->def_name == def_name)
            break;
        tmp = tmp->next;
    }
    if (tmp)
        return tmp->value;
    return NULL;
}

int main(int argc, char const *argv[])
{
    FILE *fIn, *fOut;

    if (argc != 3)
	{
		printf("Ошибка! Неверное количество аргументов\n");
		exit(-1);
	}
	if ((fIn = fopen(argv[1], "r")) == NULL)
	{
		printf("Ошибка! Не удаётся открыть файл ввода\n");
		exit(-2);
	}
	if ((fOut = fopen(argv[2], "w")) == NULL)
	{
		printf("Ошибка! Не удаётся открыть файл вывода\n");
		exit(-3);
	}

    DblLinkedList* HASH_TABLE[HASHSIZE];
    char buf[20] = {0}, *bufptr = buf, ch;
    unsigned long long def_name = 0, pow = 1;
    size_t index;

    for (size_t i = 0; i < HASHSIZE; i++)
        HASH_TABLE[i] = createDblLinkedList();
    while (1)
    {
        bufptr = buf;
        while(!isspace(ch = fgetc(fIn)))
        {
            *bufptr++ = ch;
        }
        *bufptr = 0;
        bufptr = buf;
        if (!strcmp(buf, "#define"))
        {
            while ((ch = fgetc(fIn)) != ' ') {
                *bufptr++ = ch;
            }
            *bufptr-- = 0;
            def_name = conversion(bufptr);

            bufptr = buf;
            while((ch = fgetc(fIn)) != '\n')
            {
                *bufptr++ = ch;
            }
            *bufptr = 0;
            index = HASH_FUN(def_name);
            if (HASH_TABLE[index]->head != NULL) {
                if (HASH_TABLE[index]->head->def_name < def_name)
                    pushFront(HASH_TABLE[index], def_name, buf);
                else if (HASH_TABLE[index]->tail->def_name > def_name)
                    pushBack(HASH_TABLE[index], def_name, buf);
                else insert(HASH_TABLE[index], def_name, buf);
            } else pushBack(HASH_TABLE[index], def_name, buf);
        } else break;
    }
    bufptr+=(strlen(buf));
    printf("%s", bufptr);

    while(!feof(fIn))
    {
        def_name = conversion(--bufptr);
        index = HASH_FUN(def_name);
        char *res;
        if ((res = FindReplace(HASH_TABLE[index], def_name)) != NULL)
            fprintf(fOut, "%s ", FindReplace(HASH_TABLE[index], def_name));
        else fprintf(fOut, "%s ", buf);
        bufptr = buf;
        while (!isalnum(ch = fgetc(fIn)) && ch != EOF)
            fputc(ch, fOut);
        while(!isspace(ch) && ch != EOF)
        {
            *bufptr++ = ch;
            ch = fgetc(fIn);
        }
        if (ch != EOF)
            fputc(ch, fOut);
        *bufptr = 0;

    }
    return 0;
};

