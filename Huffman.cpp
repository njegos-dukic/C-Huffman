#include "HuffmanModule.h"

int main(int argc, char* argv[])
{
	int totalNumberOfCharacters, numberOfExternalNodes = 0;
	const char* path = ".\\File.txt";

	CreatePriorityQueue(CreateAlphabetPairs(path, &totalNumberOfCharacters), &numberOfExternalNodes);
	node* treeRoot = Huffman(numberOfExternalNodes);

	int* array = (int*)calloc(sizeof(int), numberOfExternalNodes); // Recursion.
	HuffmanNode* codedNodes = (HuffmanNode*)calloc(sizeof(HuffmanNode), numberOfExternalNodes);

	GetHuffmanCodes(treeRoot, array, codedNodes, 0);
	PresentHuffmanCodes(codedNodes, numberOfExternalNodes);

	printf("\nPreorder: ");
	PreorderTreeTraversal(treeRoot, totalNumberOfCharacters);
	printf("\n");

	char codedMessage[5096] = {};
	printf("\nEnter coded message: ");
	fgets(codedMessage, 5096, stdin);

	DecodeHuffman(codedMessage, codedNodes, numberOfExternalNodes);

	// Free dynamically allocated memory.

	return 0;
}
