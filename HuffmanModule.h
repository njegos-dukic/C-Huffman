#pragma once

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

struct node
{
	char c;
	int weight;
	node* left;
	node* right;
};

struct queueNode
{
	node* element;
	queueNode* next;
};

struct HuffmanNode
{
	node* element;
	char* code;
};

extern queueNode* top;
extern queueNode* rear;

char* GetInputChars(const char* path, int* count);
int GetNumberOfRepeats(char* input, char value);
node* CreateAlphabetPairs(const char* path, int* count);
void CreatePriorityQueue(node* nodes, int* numberOfExternalNodes);
void AddToPriorityQueue(node* newNode);
queueNode* CreateQueueNode(node* newNode);
node* CreateNode(int weight, char C);
queueNode* PopTheNode();
node* Huffman(int numberOfExternalNodes);
void PresentNode(node* current);
void PreorderTreeTraversal(node* root, int numberOfChars);
void PostorderTreeTraversal(node* root, int numberOfChars);
void InorderTreeTraversal(node* root, int numberOfChars);
int IsLeaf(node* treeNode);
// const int MaxDepth(node* node);
void GetHuffmanCodes(node* root, int array[], HuffmanNode* list, int top);
char* ReturnCodeFromArray(int array[], int n);
void PresentHuffmanCodes(HuffmanNode* codedNodes, int count);
void DecodeHuffman(const char* inputString, HuffmanNode* codedNodes, int numberOfElements);
void FindMinimalNumberOfBits(HuffmanNode* codedNodes, int numberOfElements, int totalNumberOfChars);
