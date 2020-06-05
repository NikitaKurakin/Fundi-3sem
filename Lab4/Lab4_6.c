// в программу нужно передать только файл вывода
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Node
{
    char *data;
    struct Node *next;
    struct Node *prev;
} Node;

Node *createNode(char *data)
{
    Node *node = malloc(sizeof(Node));
    char *d = malloc(sizeof(char) * (strlen(data) + 1));
    strcpy(d, data);
    node->data = d;
    node->next = NULL;
    node->prev = NULL;

    return node;
}

void LIST_NodePush(Node *node, Node **tail)
{
    if (*tail == NULL)
    {
        *tail = node;
        return;
    }

    Node *cur = *tail;
    while (cur->next != NULL)
    {
        cur = cur->next;
    }
    cur->next = node;
    node->prev = cur;
    *tail = node;
}

Node *STACK_NodePop(Node **top)
{
    Node *result = *top;

    if (*top != NULL && (*top)->prev != NULL)
    {
        *top = (*top)->prev;
        (*top)->next = NULL;
    }

    return result;
}

Node *QUEUE_NodeDequeue(Node **head) {
    Node *result = *head;

    if (*head == NULL) return NULL;

    if ((*head)->next != NULL)
    {
        *head = (*head)->next;
        (*head)->prev = NULL;
    }
    else
    {
        *head = NULL;
    }

    result->prev = NULL;
    result->next = NULL;
    return result;
}

void nodeFree(Node *r)
{

    free(r->data);
    free(r);
}

int main(int argc, char* argv[])
{
    FILE* file;

    if (argc < 2 || !(file = fopen(argv[1], "w"))) {
        printf("No output file\n");
        exit(-1);
    }

    printf("Type STOP to stop program, type RM to remove N/2 strings \n");
    printf("Enter N - number of strings to save: ");

    char str[250];
    int n, i = 1;
    scanf("%d", &n);
    scanf("%s", str);
    Node* head = NULL;
    Node* tailOrTop = NULL;

    while (strcmp(str, "STOP") != 0)
    {
        if (strcmp(str, "RM") == 0)
        {
            for (int j = 0; j < n/2; ++j)
            {
                Node* remove = STACK_NodePop(&tailOrTop);

                if (remove != NULL)
                {
                    nodeFree(remove);
                } else break;
            }
            i = n/2 - 1;
        }
        else
        {
            Node *node = createNode(str);
            if (head == NULL) head = node;
            LIST_NodePush(node, &tailOrTop);
            if (i == n) {
                Node *dequeue = QUEUE_NodeDequeue(&head);
                while (dequeue != NULL)
                {
                    fprintf(file, "%s \n", dequeue->data);
                    nodeFree(dequeue);
                    dequeue = QUEUE_NodeDequeue(&head);
                    tailOrTop = NULL;
                }
                i = 0;
            }
        }

        scanf("%s", str);
        i++;
    }

    return 0;
}
