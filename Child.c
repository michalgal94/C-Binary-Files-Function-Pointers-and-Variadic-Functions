#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Child.h"
#include "General.h"


/**************************************************/
/*             Read a Child from a file           */
/**************************************************/

void readChild(FILE* fp, Child* pChild)
{
	unsigned short bdata;

	//Child ID
	if(fileType == TEXT)
	{
		fscanf(fp, "%d", &pChild->id);
		fscanf(fp, "%d", &pChild->age);
	}
	else
	{
		fread(&bdata,sizeof(short) , 1 ,fp);
		pChild->id = bdata & 0x1FFF;
		pChild->age = (bdata & 0xE000) >> 13;
	}
}

/**************************************************/
/*            show a Child				           */
/**************************************************/
void showChild(const Child* pChild)
{
	printf("\nID:%d  ", pChild->id);
	printf("Age:%d  ", pChild->age);
}

/**************************************************/
void getChildFromUser(Child* pChild, int id)
/**************************************************/
/**************************************************/
{
	pChild->id = id;

	puts("\nAge:\t");
	scanf("%d", &pChild->age);
}

/**************************************************/
/*Write a Child to the open file				*/
/**************************************************/
void writeChild(FILE* fp,const Child* pChild)
{
	unsigned short bdata;

	if(fileType == TEXT)
		fprintf(fp,"%d %d\n",pChild->id, pChild->age);
	else
	{
		bdata = (pChild->age << 13) | pChild->id;
		fwrite(&bdata, sizeof(short), 1 , fp);
	}
}

//linear search
Child*	findChildById(Child** pChildList, int count, int id)
{
	Child *pChild,**result, key = {id,0};
	pChild = &key;
	qsort(pChildList,count,sizeof(Child*),compareChildByiD);
	result = ((Child**)bsearch(&pChild,pChildList,count,sizeof(Child*),compareChildByiD));
	if(result == NULL)
		return NULL;
	return *result;
}

void birthday(Child* pChild)
{
	pChild->age++;
}

//void	releaseChild(Child* pChild)
//{
//	//nothing to release
//}
