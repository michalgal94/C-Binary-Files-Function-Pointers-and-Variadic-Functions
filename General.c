#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "General.h"
#include "Child.h"
#include "City.h"

const char* stringToSort[NumberofSorts] = { "Sort Kindergartens By Name",
		"Sort Kindergartens By Type", "Sort Child By Id" };

const char* optionStr[NofOptions] =
{
		"Exit","Read City information from file",
		"Show all Kindergartens","Show a specific Kindergarten",
		"Save City information to file","Add a Kindergarten",
		"Add a Child","Birthday to a Child",
		"Count Hova childres",
		"Sorting menu",
		"Linked List"
};

/**************************************************/
int menu()
/**************************************************/
/**************************************************/
{
	int option,i;

	printf("\n==========================");
	printf("\nSelect:\n");
	for(i = 0 ; i < NofOptions; i++)
		printf("\n\t%d. %s.",i, optionStr[i]);
	printf("\n");
	scanf("%d", &option);
	return option;
}

char* getStrExactLength(char* inpStr)
{
	char* theStr = NULL;
	size_t len;

	len = strlen(inpStr) + 1;
	//allocate a place for the string in the right location in the array 
	theStr = (char*)malloc(len*sizeof(char));
	//Copy the string to the right location in the array 
	if (theStr != NULL)
		strcpy(theStr, inpStr);

	return theStr;
}

int checkAllocation(const void* p)
{
	if (!p)
	{
		printf("ERROR! Not enough memory!");
		return 0;
	}
	return 1;
}

void insertionSort(void* arr, int count, int typeSize, int (*comperator)(const void*,const void*))
{
	int i,j;
	void* key = malloc(typeSize);
	if (!checkAllocation(key)) return;
	for (i = 1; i < count; i++)
	{
		key = memcpy(key,arr + i*typeSize,typeSize);
		for (j = i - 1; j >= 0 && (comperator(arr + j*typeSize, key) > 0); j--)
			memmove(arr + (j + 1)*typeSize, arr + j*typeSize, typeSize);
		memmove(arr + (j+1)*typeSize, key,typeSize);
	}
	free(key);
}

int compareGardenByName(const void* str1, const void* str2)
{
	const Garden* kindergarten1 = *(const Garden**) str1;
	const Garden* kindergarten2 = *(const Garden**) str2;
	return strcmp(kindergarten1->name, kindergarten2->name);
}

int compareChildByiD(const void* str1, const void* str2)
{
	const Child* child1 = *(const Child**) str1;
	const Child* child2 = *(const Child**) str2;
	return child1->id - child2->id;
}

int compareGardenByTypeAndCountChild(const void* str1, const void* str2)
{
	int result;
	const Garden* kindergarten1 = *(const Garden**) str1;
	const Garden* kindergarten2 = *(const Garden**) str2;
	result = kindergarten1->type - kindergarten2->type;
	if (result != 0)
		return result;
	else
		return kindergarten1->childCount - kindergarten2->childCount;
}

void sortingMenu(City* pCity)
{
	int i , sortType;
	char gardenName[100];
	do
	{
		for(i=0; i<NumberofSorts ; i++)
			printf("\n\t%d. %s.",i , stringToSort[i]);
		printf("\n");
		scanf("%d" , &sortType);
	} while(sortType >= NumberofSorts || sortType < 0);

	if(sortType == SORT_KINDERGARTENS_BY_NAME)
		insertionSort(pCity->pGardenList, pCity->count, sizeof(Garden*), compareGardenByName);
	else if(sortType == SORT_CHILDREN_BY_ID)
	{
		printf("Give me the kindergarten name: \n");
		scanf("%s", gardenName);
		Garden* garden = findGardenByName(pCity->pGardenList, pCity->count, gardenName);
			if(garden == NULL)
				printf("No such kindergarten.");
			else
				insertionSort(garden->childPtrArr, garden->childCount, sizeof(Child*),compareChildByiD);
	}
	else
		insertionSort(pCity->pGardenList, pCity->count, sizeof(Garden*), compareGardenByTypeAndCountChild);
}

void variadicFunction(const char* str, ...)
{
	int number;
	va_list list;
	va_start(list,str);

	while(str!= NULL)
	{
		number = va_arg(list,int);
		printf("%s %d\n" , str , number);
		str = va_arg(list,char*);
	}
	va_end(list);
}

BOOL listInit(List* pList)
{
	if(pList == NULL)
		return False;

	pList->head.next = NULL;
	return True;
}

Node* listInsert(Node* pNode, Data value)
{
	Node* newNode;
	if(pNode == NULL)
		return NULL;

	if(!checkAllocation((newNode = (Node*)malloc(sizeof(Node)))))
		return NULL;

	if(newNode != NULL)
	{
		newNode->key = value;
		newNode->next = NULL;
		pNode->next = newNode;
		pNode = newNode;
	}

	return pNode;
}

int listPrint(List* pList,  void (*print)(const void*))
{
	Node* node;
	int l =0;
	if(!pList)
		return 0;
	printf("\n");
	for(node =pList->head.next; node; node = node->next, l++)
		print(node->key);
	printf("\n");
	return l;
}

Node* createLinkedListForKindergartenType(const City *pCity, GardenType type)
{
	List list;
	int i;
	Node* node;

	// Initialization
	if(listInit(&list) == False)
		return NULL;

	//Add to list 1 after the other
	node = &list.head; // pointer to the head of the list

	for(i=0; i< pCity->count ; i++)
	{
		if(pCity->pGardenList[i]->type == type)
		{
			node = listInsert(node, pCity->pGardenList[i]);
		}
	}

	Node* headN = &list.head;
	return headN->next;
}

void displayKindergartensForList(const Node* node)
{
	int i;

	for(i = 1 ; node != NULL ; node = node->next, i++)
	{
		printf("\nKindergarten %d:\n",i);
		showGarden((Garden*)node->key);
		printf("\n");
	}
}

void kindergartensLinkedList(const City* pCity)
{
	Node* list;
	GardenType type;
	type = getTypeOption();
	list = createLinkedListForKindergartenType(pCity, type);
	displayKindergartensForList(list);
	releaseKindergartensLinkedList(list);
}

void releaseKindergartensLinkedList(Node* node)
{
	if (!node)
		return;

	for (; node != NULL; node = node->next)
		free(node);
}

