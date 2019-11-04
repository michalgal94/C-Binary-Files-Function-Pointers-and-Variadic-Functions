#ifndef __PROTOTYPE__
#define __PROTOTYPE__

#include "City.h"

typedef enum
{
	EXIT,
	READ_CITY,
	SHOW_CITY,
	SHOW_GARDEN,
	WRITE_CITY,
	ADD_GARDEN,
	ADD_CHILD,
	CHILD_BIRTHDAY,
	COUNT_GRADUATE,
	SORT_MENU,
	LINKED_LIST,
	NofOptions
}MenuOptions;

typedef enum
{
	SORT_KINDERGARTENS_BY_NAME,
	SORT_KINDERGARTENS_BY_TYPE,
	SORT_CHILDREN_BY_ID,
	NumberofSorts
}SortingType;


typedef enum
{
	TEXT,
	BINARY
}FileType;

typedef enum {False, True} BOOL;

int		menu();
char*	getStrExactLength(char* inpStr);
int		checkAllocation(const void* p);
void insertionSort(void* arr, int count, int typeSize, int (*comperator)(const void*,const void*));
int compareGardenByName(const void* str1, const void* srt2);
int compareChildByiD(const void* str1, const void* str2);
int compareGardenByTypeAndCountChild(const void* str1, const void* str2);
void sortingMenu(City* pCity);
void variadicFunction(const char* str, ...);
BOOL listInit(List* pList);
Node* listInsert(Node* pNode, Data value);
int listPrint(List* pList,  void (*print)(const void*));
Node* createLinkedListForKindergartenType(const City *pCity, GardenType type);
void displayKindergartensForList(const Node* list);
void kindergartensLinkedList(const City* pCity);
void releaseKindergartensLinkedList(Node* node);

extern FileType fileType;

#endif
