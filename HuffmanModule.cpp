#include "HuffmanModule.h"

queueNode* top = NULL;
queueNode* rear = NULL;

char* GetInputChars(const char* path, int* count)
{
	FILE* inputFile;
	fopen_s(&inputFile, path, "r");
	char* input = (char*)calloc(sizeof(char), 5096); // 5 kB

	if (input == NULL)
	{
		printf("Unsuccessful memory allocation.\n\n");
		return 0;
	}

	if (inputFile == NULL)
	{
		printf("Unsuccessful file opening.\n\n");
		return 0;
	}

	char c;
	int i = -1;
	while ((c = fgetc(inputFile)) != EOF)
		if (isalpha(c) || isdigit(c))
			*(input + (++i)) = c;

	fclose(inputFile);

	*count = ++i;
	return input;
}

int GetNumberOfRepeats(char* input, char value)
{
	int returnValue = 0;
	for (int i = 0; i < strlen(input); i++)
		if (input[i] == value)
			returnValue++;

	return returnValue;
}

node* CreateAlphabetPairs(const char* path, int* count)
{
	node* nodes = (node*)calloc(sizeof(node), 62);
	char A = 'A' - 1;
	char a = 'a' - 1;
	char digit = '0' - 1;

	int numberOfCharacters;
	char* input = GetInputChars(path, &numberOfCharacters);

	for (int i = 0; i < 62; i++)
	{
		if (i < 26)
			nodes[i].c = ++A;
		else if (i > 25 && i < 52)
			nodes[i].c = ++a;
		else
			nodes[i].c = ++digit;

		nodes[i].weight = GetNumberOfRepeats(input, nodes[i].c);
	}

	// printf("Characters: %s\Number of read characters: %d.\n\n", input, numberOfCharacters);
	printf("Total number of read characters %d.\n\n", numberOfCharacters);
	int total = 0;
	double probability = 0.0;

	for (int i = 0; i < 62; i++)
		if (nodes[i].weight != 0)
		{
			probability += (double)nodes[i].weight / (double)numberOfCharacters * 100;
			printf("Character: %c\nNumber of times: %d\nProbability: %.3lf%%\n\n", nodes[i].c, nodes[i].weight, (double)nodes[i].weight / (double)numberOfCharacters * 100);
			total += nodes[i].weight;
		}
	printf("Total probability: %.3lf%%.\n\n", probability);

	// if (total == numberOfCharacters)
		// printf("\nNumber of read characters is equal to total number of characters. Probabilities are valid. 100%%.\n\n");

	*count = numberOfCharacters;
	return nodes;
}

queueNode* CreateQueueNode(node* newNode)
{
	queueNode* returnNode = (queueNode*)calloc(1, sizeof(queueNode));
	returnNode->element = newNode;
	returnNode->next = NULL;
	return returnNode;
}

void CreatePriorityQueue(node* nodes, int* numberOfExternalNodes)
{
	for (int i = 0; i < 62; i++)
		if ((nodes + i)->weight)
		{
			AddToPriorityQueue(CreateNode((nodes + i)->weight, (nodes + i)->c));
			(*numberOfExternalNodes)++;
		}
}

node* CreateNode(int weight, char C = '\0')
{
	node* returnNode = (node*)calloc(sizeof(node), 1);
	returnNode->c = C;
	returnNode->weight = weight;
	returnNode->left = returnNode->right = NULL;
	return returnNode;
}

void AddToPriorityQueue(node* newNode)
{
	queueNode* insertNode = CreateQueueNode(newNode);

	if (rear == NULL) // Emtpy queue.
	{
		insertNode->next = NULL;
		top = rear = insertNode;
		// printf("Adding %c[%d] into emtpy queue.\n", insertNode->element->c, insertNode->element->weight);
		return;
	}

	queueNode* current = rear;
	queueNode* previous = NULL;
	
	while (1)
	{
		if (current != NULL && current->element->weight > insertNode->element->weight)
		{
			previous = current;
			current = current->next;
		}

		else
		{
			if (current == NULL) // Add to beginning.
			{
				insertNode->next = NULL;
				top->next = insertNode;
				top = insertNode;
				// printf("Adding %c[%d] at the beginning of the queue.\n", insertNode->element->c, insertNode->element->weight);
				return;
			}

			else if (current == rear) // Add to end.
			{
				insertNode->next = rear;
				rear = insertNode;
				// printf("Adding %c[%d] at the end of the queue.\n", insertNode->element->c, insertNode->element->weight);
				return;
			}
			
			else // Adding after element.
			{
				insertNode->next = current;
				previous->next = insertNode;
				// printf("Adding %c[%d] into the queue behind %c[%d], before %c[%d].\n", insertNode->element->c, insertNode->element->weight, current->element->c, current->element->weight, previous->element->c, previous->element->weight);
				return;
			}
		}
	}
}

queueNode* PopTheNode()
{
	if (rear == NULL)
	{
		printf("\nEmpty list!\n");
		return NULL;
	}
		
	queueNode* current = rear;
	queueNode* returnNode = NULL;
	
	if (top == rear) // Only 1 element in the queue.
	{
		returnNode = top;
		top = rear = NULL;
		// printf("Popped the node: %c[%d]\n", returnNode->element->c, returnNode->element->weight);
		return returnNode;
	}

	while (current->next != top)
		current = current->next;

	returnNode = current->next;
	current->next = NULL;
	top = current;

	// printf("Popped the node: %c[%d]\n", returnNode->element->c, returnNode->element->weight);
	return returnNode;
}

node* Huffman(int numberOfExternalNodes)
{
	node* z = NULL;
	node* x = NULL;
	node* y = NULL;

	for (int i = 0; i < numberOfExternalNodes-1; i++)
	{
		x = PopTheNode()->element;
		y = PopTheNode()->element;
		z = CreateNode(x->weight + y->weight);
		z->left = x;
		z->right = y;
		AddToPriorityQueue(z);
	}

	return z; 
}

void PresentNode(node* current, int numberOfChars)
{
	if (current->c == '\0')
		printf("#[%.2lf]   ", (double) current->weight / (double) numberOfChars * 100);
	else
		printf("%c[%.2lf]   ", current->c, (double) current->weight / (double) numberOfChars * 100);
}

void PreorderTreeTraversal(node* root, int numberOfChars)
{
	if (root == NULL)
		return;
	
	PresentNode(root, numberOfChars);
	PreorderTreeTraversal(root->left, numberOfChars);
	PreorderTreeTraversal(root->right, numberOfChars);

	return;

}

void PostorderTreeTraversal(node* root, int numberOfChars)
{
	if (root == NULL)
		return;

	PostorderTreeTraversal(root->left, numberOfChars);
	PostorderTreeTraversal(root->right, numberOfChars);
	PresentNode(root, numberOfChars);

	return;
}

void InorderTreeTraversal(node* root, int numberOfChars)
{
	if (root == NULL)
		return;

	InorderTreeTraversal(root->left, numberOfChars);
	PresentNode(root, numberOfChars);
	InorderTreeTraversal(root->right, numberOfChars);

	return;
}

int IsLeaf(node* treeNode)
{
	return !(treeNode->left) && !(treeNode->right);
}

// const int MaxDepth(node* node)
// {
// 	if (node == NULL)
// 		return 0;
	
// 	else
// 	{
// 		int leftDepth = MaxDepth(node->left);
// 		int rightDepth = MaxDepth(node->right);

// 		if (leftDepth > rightDepth)
// 			return (leftDepth + 1);

// 		else 
// 			return(rightDepth + 1);
// 	}
// } 

char* ReturnCodeFromArray(int arr[], int numberOfElements)
{
	char* returnChar = (char*)calloc(sizeof(char), numberOfElements + 1);

	for (int i = 0; i < numberOfElements; i++)
	{
		if (arr[i] == 1)
			*(returnChar + i) = '1';
		else
			*(returnChar + i) = '0';
	}

	return returnChar;
}

void GetHuffmanCodes(node* root, int array[], HuffmanNode* list, int top = 0)
{
	static int i = 0;

	if (root->left)
	{
		array[top] = 0;
		GetHuffmanCodes(root->left, array, list, top + 1);
	}

	if (root->right)
	{
		array[top] = 1;
		GetHuffmanCodes(root->right, array, list, top + 1);
	}

	if (IsLeaf(root))
	{
		(list+i)->element = root;
		(list + i)->code = ReturnCodeFromArray(array, top);
		i++;

		//printf("Character '%c' is coded as: %s.\n", root->c, ReturnCodeFromArray(array, top));
	}

	return;
}

void PresentHuffmanCodes(HuffmanNode* codedNodes, int count)
{
	for (int i = 0; i < count; i++)
		printf("Character '%c', code: %s.\n", codedNodes[i].element->c, codedNodes[i].code);
}

void DecodeHuffman(const char* inputString, HuffmanNode* codedNodes, int numberOfElements)
{
	printf("\nDecoded message: ");
	char* temp = (char*)calloc(sizeof(char), 5096); // 5 kB

	int position = 0;
	for (int i = 0; inputString[i]; i++)
		if (inputString[i] == '0' || inputString[i] == '1')
		{
			temp[position] = inputString[i];
			position++;

			for (int j = 0; j < numberOfElements; j++)
				if (!strcmp(temp, codedNodes[j].code))
				{
					printf("%c", codedNodes[j].element->c);
					for (int k = 0; temp[k]; k++)
						temp[k] = '\0';
					position = 0;
				}
		}

	printf("\n");
	return;
}

void FindMinimalNumberOfBits(HuffmanNode* codedNodes, int numberOfElements, int totalNumberOfChars)
{
	double amount = 0;
	for (int i = 0; i < numberOfElements; i++)
	{
		// printf ("\nLength of the code '%s': %d\nElement: %c\nWeight: %d\nProbability: %.3lf", codedNodes[i].code, codedNodes[i].element->c, codedNodes[i].element->weight, )
		amount += (strlen(codedNodes[i].code) * (double)codedNodes[i].element->weight / (double)totalNumberOfChars);
	}
	printf("\n\nMinimal: %.3lf\n\n", amount);
}