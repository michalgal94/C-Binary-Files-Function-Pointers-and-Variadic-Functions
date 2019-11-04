#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Kindergarten.h"
#include "Child.h"
#include "General.h"

const char* GardenTypeName[NofTypes] =
{ "Chova", "Trom Chova", "Trom Trom Chova" };


FileType fileType;

//**************************************************
// Read data off all Kindergartens from file
//**************************************************
Garden**	readAllGardensFromFile(char* fileName,int* pGardenCount)
{
	int count, i;
	FILE *fp;
	Garden** gardens;

	*pGardenCount = 0;

	if (fileType == TEXT)
	{
		fp = fopen(fileName, "rt");

		if(fp == NULL)
		{
			printf("Cannot Open File '%s'", fileName);
			return NULL;
		}
		fscanf(fp, "%d", &count);
	}
	else
	{
		fp = fopen(fileName , "rb");

		if(fp == NULL)
		{
			printf("Cannot Open File '%s'", fileName);
			return NULL;
		}
		fread(&count,sizeof(int),1,fp);
	}

	//Would like to do the allocation even if count ==0
	//so will not return NULL as error
	gardens = (Garden**)malloc(count * sizeof(Garden*));

	if (!checkAllocation(gardens))
		return NULL;

	for (i = 0; i < count; i++)
	{
		gardens[i] = (Garden*)malloc(sizeof(Garden));
		if (!checkAllocation(gardens[i]))
			return NULL;
		readGarden(fp, gardens[i]);
	}

	fclose(fp);

	*pGardenCount = count;
	return gardens;
}

/**************************************************/
/*             Read a Kindergarten from a file           */
/**************************************************/
void	readGarden(FILE* fp, Garden* pGarden)
{
	int i,type;
	int nameLength;
	char sTemp[100];
	unsigned char binarydata;

	if(fileType == TEXT)
	{
		//Kindergarten Name
		//Get it to a temp string and then create the pointer to save
		//in the struct in exact length.
		fscanf(fp, "%s", sTemp);

		//Kindergarten type
		fscanf(fp, "%d", &type);

		//Child count
		fscanf(fp, "%d", &pGarden->childCount);
	}
	else
	{
		fread(&nameLength, sizeof(int),1 , fp);
		fread(sTemp, sizeof(char), nameLength,fp);
		fread(&binarydata,sizeof(char) , 1 ,fp);

		type = binarydata & 0x03;
		pGarden->childCount = (binarydata & 0xFC) >> 2;
	}

	pGarden->name = getStrExactLength(sTemp);
	pGarden->type = (GardenType)type;

	if(pGarden->childCount==0)
	{
		pGarden->childPtrArr = NULL;
		return;
	}

	pGarden->childPtrArr = (Child**)malloc(pGarden->childCount * sizeof(Child*));
	if (!checkAllocation(pGarden->childPtrArr))
		return;

	//Read each child
	for (i = 0; i < pGarden->childCount; i++)
	{
		pGarden->childPtrArr[i] = (Child*)malloc(sizeof(Child));
		if (!checkAllocation(pGarden->childPtrArr[i]))
			return;
		readChild(fp, pGarden->childPtrArr[i]);
	}
}

/**************************************************/
/*            show all Kindergartens		      */
/**************************************************/
void	showAllGardens(Garden** pGardenList, int count)
{
	int i;
	printf("There are %d kindergarten in the city\n",count);

	if(count >= 3)
		variadicFunction(pGardenList[0]->name, pGardenList[0]->childCount,
				pGardenList[1]->name, pGardenList[1]->childCount,
				pGardenList[2]->name, pGardenList[2]->childCount,
				NULL);
	for (i = 0; i<count; i++)
	{
		printf("\nKindergarten %d:\n", i + 1);
		showGarden(pGardenList[i]);
		printf("\n");
	}
	printf("\n");
}

/**************************************************/
/*            show a Kindergarten		           */
/**************************************************/
void	showGarden(const Garden* pGarden)
{
	int i;

	// Kindergarten name
	printf("Name:%s", pGarden->name);
	printf("\tType:%s", GardenTypeName[pGarden->type]);
	printf("\t%d Children:", pGarden->childCount);
	//all Children
	for (i = 0; i< pGarden->childCount; i++)
		showChild(pGarden->childPtrArr[i]);
}

/**************************************************/
/*  write Kindergartens to a file					   */

/**************************************************/

void	writeGardensToFile(Garden** pGardenList, int gardenCount, char* fileName)
{
	int i;
	FILE *fp;

	if (fileType == TEXT)
	{
		fp = fopen(fileName, "wt");
		if (fp == NULL)
		{
			printf("Cannot Open File '%s'", fileName);
			return;
		}
		fprintf(fp, "%d\n", gardenCount);
	}
	else
	{
		fp = fopen(fileName , "wb");
		if (fp == NULL)
		{
			printf("Cannot Open File '%s'", fileName);
			return;
		}
		fwrite(&gardenCount,sizeof(int),1,fp);
	}

	for (i = 0; i<gardenCount; i++)
		writeGarden(fp, pGardenList[i]);

	fclose(fp);
}

/**************************************************/
/*Write a Kindergarten to the open file						*/
/**************************************************/
void	writeGarden(FILE* fp,const Garden* pGarden)
{
	int i;
	int nameLenght;
	unsigned char binarydata;

	if(fileType == TEXT)
	{
		//Kindergarten Name
		fprintf(fp, "%s", pGarden->name);
		fprintf(fp, "  %d", pGarden->type);

		//Children
		fprintf(fp, " %d\n", pGarden->childCount);
	}
	else
	{
		nameLenght = strlen(pGarden->name) + 1;	// returns numbers of bytes with additional of '\0'
		fwrite(&nameLenght , sizeof(int), 1, fp);
		fwrite(pGarden->name,sizeof(char),nameLenght, fp);

		binarydata = (pGarden->childCount << 2) | pGarden->type;
		fwrite(&binarydata, sizeof(char), 1 , fp);
	}

	for (i = 0; i <pGarden->childCount; i++)
		writeChild(fp, pGarden->childPtrArr[i]);
}


//*************************************************
// menu option to add Child to a Kindergarten
//*************************************************
void	addChildToGarden(Garden** pGardenList, int gardenCount)
{
	Garden* pGarden;
	if (pGardenList == NULL)
	{
		printf("There are no Kindergartens yet\n");
		return;
	}

	pGarden = getGardenAskForName(pGardenList, gardenCount);
	if (pGarden == NULL)
	{
		printf("no such Kindergarten\n");
		return;
	}

	pGarden->childPtrArr = (Child**)realloc(pGarden->childPtrArr, (pGarden->childCount + 1) * sizeof(Child*));
	if (!checkAllocation(pGarden->childPtrArr))
	{
		pGarden->childCount = 0;
		return;
	}

	pGarden->childPtrArr[pGarden->childCount] = (Child*)malloc(sizeof(Child));
	getChildCheckIdFromUser(pGarden->childPtrArr[pGarden->childCount], pGarden);
	pGarden->childCount++;
}

/**************************************************/
/* Add a Kindergarten  - we need to allocate more space   */
/* in the list.									*/
/**************************************************/
Garden**	addGarden(Garden** pGardenList, int* pGardenCount)
{
	pGardenList = (Garden**)realloc(pGardenList, (*pGardenCount + 1) * sizeof(Garden*));
	if (!checkAllocation(pGardenList))
	{
		*pGardenCount = 0;
		return NULL;
	}

	Garden* temp = (Garden*)malloc(sizeof(Garden));
	getGardenFromUser(temp, pGardenList, *pGardenCount);
	pGardenList[*pGardenCount] = temp;
	(*pGardenCount)++;

	return pGardenList;
}

void	showGardenMenu(Garden** pGardenList, int count)
{
	Garden* pGarden;

	pGarden = getGardenAskForName(pGardenList, count);

	if (pGarden != NULL)
		showGarden(pGarden);
	else
		printf("No such Kindergarten\n");
}

Garden*	getGardenAskForName(Garden** pGardenList, int count)
{
	char sTemp[100];
	Garden* pGarden;

	//Kindergarten name
	puts("\nGive me the Kindergarten Name:\t");
	scanf("%s", sTemp);
	getchar(); //remove \n

	//In this exe the find will not be efficient
	pGarden = findGardenByName(pGardenList, count, sTemp);	

	return pGarden;
}

Garden*	findGardenByName(Garden** pGardenList, int count,const char* name)
{
	//linear search
	int i;

	for (i = 0; i < count; i++)
	{
		if (strcmp(pGardenList[i]->name, name) == 0)
			return pGardenList[i];
	}
	return NULL;
}

Child*	getChildAskForId(Garden* pGarden)
{
	int id;

	printf("Enter child id\n");
	scanf("%d", &id);

	//The search will be inefficient - so sort
	return findChildById(pGarden->childPtrArr, pGarden->childCount, id);
}

void	getGardenFromUser(Garden* pGarden, Garden** pGardenList, int count)
{
	int i;
	char sTemp[100];
	int bOK = 0;
	int countC;

	do
	{
		//Kindergarten name
		puts("\nName:\t");
		scanf("%s", sTemp);
		getchar(); //remove second word if exsist
		if (findGardenByName(pGardenList, count, sTemp) != NULL)
			printf("This Kindergarten already in list\n");
		else
			bOK = 1;
	} while (!bOK);

	pGarden->name = getStrExactLength(sTemp);
	pGarden->type = getTypeOption();

	//Children
	puts("\nEnter children Details:\t");
	puts("\nChildren count:");
	scanf("%d", &countC);
	if(countC == 0)
	{
		pGarden->childCount = 0;
		pGarden->childPtrArr = NULL;
		return;
	}

	//create the list of children in the correct size
	pGarden->childPtrArr = (Child**)malloc(countC * sizeof(Child*));
	if (!checkAllocation(pGarden->childPtrArr))
	{
		pGarden->childPtrArr = NULL;
		return;
	}

	pGarden->childCount = 0;
	for (i = 0; i < countC; i++)
	{
		pGarden->childPtrArr[i] = (Child*)malloc(sizeof(Child));
		getChildCheckIdFromUser(pGarden->childPtrArr[i], pGarden);
		pGarden->childCount++;
	}
}

/**************************************************/
/* Init a child from use. Ask for Id, check it is */
/* new and after call the function to ask for all other */
/* information										*/		
/**************************************************/
void	getChildCheckIdFromUser(Child* pChild,const Garden* pGarden)
{
	int id, bOK = 0;
	do
	{
		puts("\nID No.:\t");
		scanf("%d", &id);
		if (findChildById(pGarden->childPtrArr, pGarden->childCount, id) != NULL)
			printf("This child is in the Kindergarten\n");
		else
			bOK = 1;
	} while (!bOK);

	getChildFromUser(pChild, id);
}

void	handleBirthdayToChild(Garden** pGardenList, int count)
{
	Garden* pGarden;
	Child* pChild;

	pGarden = getGardenAskForName(pGardenList, count);
	if (pGarden == NULL)
	{
		printf("No such Kindergarten\n");
		return;
	}

	pChild = getChildAskForId(pGarden);
	if (pChild == NULL)
	{
		printf("No such child\n");
		return;
	}
	birthday(pChild);
}

GardenType	getTypeOption()
{
	int i, type;
	printf("Garden type:\n");
	do
	{
		for (i = 0; i < NofTypes; i++)
			printf("Enter %d for %s\n", i, GardenTypeName[i]);
		scanf("%d", &type);
	} while (type < 0 || type >= NofTypes);
	return (GardenType)type;
}

// release the Children list
//release the name ptr of each Kindergarten
//release the Kindergarten list
void	release(Garden** pGardenList, int count)
{
	int i;
	for (i = 0; i < count; i++)
	{
		free(pGardenList[i]->childPtrArr);
		free(pGardenList[i]->name);
		free(pGardenList[i]);
	}

	free(pGardenList);
}

