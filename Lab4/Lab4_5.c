//после запуска программа выведет дерево и попросит ввести слово, введите любое слово и программа выведет оставшуюся информацию
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Tree
{
	char *word;
	unsigned int count;
	struct Tree *left;
	struct Tree *right;
}Tree, *PTree;

PTree addnode(char *word, PTree tree);
void treeprint(PTree tree, int n);
void minmax(PTree t, char *min, char *max);
void search(PTree t, char *word, int *count);
void depth(PTree t, int *depth, int *current);
void tofile(PTree t, FILE *fOut);
PTree fromfile(PTree tree, FILE *fOut);

int main(int argc, char *argv[])
{
	FILE *fIn, *fOut;
	PTree tree = NULL, outtree = NULL;
	char word[20], min[20], max[20], *minp = min, *maxp = max;
	int count = 0, ddepth = 1;
	*minp = 0, *maxp = 0;

	if (!(fIn = fopen(argv[1], "r")))
	{
		printf("Can't open input file!!!");
		exit(1);
	}

	while(!feof(fIn))
	{
		fscanf(fIn, "%s", word);
		tree = addnode(word, tree);
	}

	treeprint(tree, 0);
	minmax(tree, minp, maxp);
	printf("min = %s, max = %s\n\n", min, max);
	printf("Enter word to search: ");
	scanf("%s", word);
	search(tree, word, &count);
	printf("There are(is) %d \"%s\" in file\n\n", count, word);
	count = 1;
	depth(tree, &ddepth, &count);
	printf("The tree has depth of %d\n\n", ddepth);

	if (!(fOut = fopen("out5.txt", "w")))
		printf("Can't create output file\n");
	else
	{
		tofile(tree, fOut);
		printf("Successfully uploaded to output file\n");
	}

	fclose(fOut);

	if (!(fOut = fopen("out5.txt", "r")))
		printf("Can't open input file\n");
	else
	{
		outtree = fromfile(outtree, fOut);
		printf("Successfully uploaded from output file\n\n");
		treeprint(outtree, 0);
		fclose(fOut);
	}

	fclose(fIn);
	return 0;
}

PTree addnode(char *word, PTree tree)
{
	if (tree == NULL)
	{
	    tree = (PTree)malloc(sizeof(Tree));
	    tree->word = (char *)malloc(sizeof(char) * strlen(word));
	    strcpy(tree->word, word);
	    tree->count = 1;
	    tree->left = NULL;
	    tree->right = NULL;
  	}
  	else if (strcmp(word, tree->word) == 0)
  	{
  		tree->count++;
  		return tree;
  	}
  	else if (strcmp(word, tree->word) < 0)
    	tree->left = addnode(word, tree->left);
  	else
    	tree->right = addnode(word, tree->right);

    return tree;
}

void treeprint(PTree tree, int n)
{
	if (tree->left)
		treeprint(tree->left, n + 1);

	for (int i = 0; i < n * 4; i++)
		printf(" ");

	printf("%s\n", tree->word);

	if (tree->right)
		treeprint(tree->right, n + 1);
}

void minmax(PTree t, char *min, char *max)
{
	if (t == NULL)
		return;

	if (*min == 0 || *max == 0)
	{
		strcpy(min, t->word);
		strcpy(max, t->word);
	}

	if (strlen(min) > strlen(t->word))
		strcpy(min, t->word);

	if (strlen(max) < strlen(t->word))
		strcpy(max, t->word);

	minmax(t->left, min, max);
	minmax(t->right, min, max);
}

void search(PTree t, char *word, int *count)
{
	if (t)
	{
		if (strcmp(t->word, word) == 0)
		{
			*count = t->count;
			return;
		}

		search(t->left, word, count);
		search(t->right, word, count);
	}
}

void depth(PTree t, int *ddepth, int *current)
{
	if (!t)
		return;

	if (t->left)
	{
		(*current)++;
		depth(t->left, ddepth, current);
	}
	else if (*ddepth < *current)
			*ddepth = *current;

	if (t->right)
	{
		(*current)++;
		depth(t->right, ddepth, current);
	}
	else if (*ddepth < *current)
			*ddepth = *current;

	(*current)--;
}

void tofile(PTree t, FILE *fOut)
{
	if (!t)
		return;

	fprintf(fOut, "%s ", t->word);

	if (t->left)
		fprintf(fOut, "1 ");
	else
		fprintf(fOut, "0 ");

	if (t->right)
		fprintf(fOut, "1 ");
	else
		fprintf(fOut, "0 ");

	tofile(t->left, fOut);
	tofile(t->right, fOut);
}

PTree fromfile(PTree tree, FILE *fOut)
{
	char word[20];
	int l, r;
	fscanf(fOut, "%s %d %d", word, &l, &r);

	if (tree == NULL)
	{
	    tree = (PTree)malloc(sizeof(Tree));
	    tree->word = (char *)malloc(sizeof(char) * strlen(word));
	    strcpy(tree->word, word);
	    tree->left = NULL;
	    tree->right = NULL;
  	}

  	if (l)
    	tree->left = fromfile(tree->left, fOut);

  	if (r)
    	tree->right = fromfile(tree->right, fOut);

    return tree;
}
