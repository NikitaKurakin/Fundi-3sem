#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct TreeNode
{
    char data;
    int size;
    struct TreeNode **childrens;
} TreeNode;

TreeNode *stack[250] = {NULL};
int stackSize = -1;

void put(TreeNode *node)
{
    stack[++stackSize] = node;
}

TreeNode *pop()
{
    if (stackSize == -1)
        return NULL;

    TreeNode *result = stack[stackSize];
    stack[stackSize] = NULL;
    stackSize--;
    return result;
}

TreeNode *top()
{
    return stack[stackSize];
}

int isBracket(TreeNode* node) {
    if (node->data == '(') return 1;
    return 0;
}

TreeNode *createBracket()
{
    TreeNode *node = malloc(sizeof(TreeNode));
    node->data = '(';
    return node;
}

void printTree(TreeNode *root, int deep)
{
    if (root == NULL)
        return;

    for (int i = 0; i < deep; ++i)
    {
        printf("\t");
    }
    printf("%c\n", root->data);
    for (int j = 0; j < root->size; ++j)
    {
        printTree(root->childrens[j], deep+1);
    }
}

void freeTree(TreeNode *root)
{
    if (root == NULL)
        return;

    for (int i = 0; i < root->size; ++i)
    {
        free(root->childrens[i]);
    }
    free(root);
}

int main(int argc, char* argv[])
{
    char c;
    FILE *file;

    if (argc < 2 || !(file = fopen(argv[1], "r")))
    {
        exit(-1);
    }

    while (!(feof(file)))
    {
        c = fgetc(file);
        if (c == '\n' || c == EOF)
        {
            printTree(stack[0], 0);
            TreeNode *f = pop();
            freeTree(f);
            printf("\n\n\n\n");

            if (feof(file))
                break;
        }
        else if (isalpha(c))
        {
            TreeNode *node = malloc(sizeof(TreeNode));
            node->data = c;
            node->size = 0;
            put(node);
        }
        else if (c == '(')
        {
            put(createBracket());
        }
        else if (c == ')')
        {
            int curSize = 0;
            TreeNode **list = malloc(sizeof(TreeNode *) * (curSize + 1));

            if (top()->data != '(')
            {
                list[curSize++] = pop();
            }

            while (top()->data != '(')
            {
                list = realloc(list, sizeof(TreeNode *) * (curSize + 1));
                list[curSize++] = pop();
            }

            if (top()->data == '(')
            {
                TreeNode *f = pop();
                free(f);
            }

            TreeNode *root = pop();
            root->childrens = list;
            root->size = curSize;
            put(root);
        }
    }
    fclose(file);
}
