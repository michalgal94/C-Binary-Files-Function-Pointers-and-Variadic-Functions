#include <stdio.h>

#include "City.h"
#include "Kindergarten.h"
#include "General.h"

void	readCity(City* pCity)
{
	if (pCity->pGardenList != NULL)
	{
		releaseCity(pCity);
		pCity->count = 0;
	}

	if(fileType == TEXT)
		pCity->pGardenList = readAllGardensFromFile(DATA_FILE, &pCity->count);
	else
		pCity->pGardenList = readAllGardensFromFile(BIN_FILE, &pCity->count);

	if (pCity->pGardenList == NULL)
		printf("Error reading city information\n");
}


void	showCityGardens(City* pCity)
{
	showAllGardens(pCity->pGardenList, pCity->count);
}

void	showSpecificGardenInCity(City* pCity)
{
	showGardenMenu(pCity->pGardenList, pCity->count);
}

void 	saveCity(City* pCity)
{
	if(fileType == TEXT)
		writeGardensToFile(pCity->pGardenList, pCity->count, DATA_FILE);
	else
		writeGardensToFile(pCity->pGardenList, pCity->count, BIN_FILE);
}

void 	cityAddGarden(City* pCity)
{
	pCity->pGardenList = addGarden(pCity->pGardenList, &pCity->count);
	if (pCity->pGardenList == NULL)//Allocation error
		printf("Error adding kindergarten\n");
}

void	addChildToSpecificGardenInCity(City* pCity)
{
	addChildToGarden(pCity->pGardenList, pCity->count);
}

void	birthdayToChild(City* pCity)
{
	handleBirthdayToChild(pCity->pGardenList, pCity->count);
}

int		countChova(City* pCity)
{
	int i;
	int count = 0;
	for (i = 0; i < pCity->count; i++)
	{
		if (pCity->pGardenList[i]->type == Chova)
			count += pCity->pGardenList[i]->childCount;
	}
	return count;
}

void	releaseCity(City* pCity)
{
	release(pCity->pGardenList, pCity->count);
}
