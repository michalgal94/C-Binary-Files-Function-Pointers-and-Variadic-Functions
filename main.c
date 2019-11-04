#include <stdio.h>
#include <stdlib.h>

#include "General.h"
#include "Kindergarten.h"
#include "Child.h"
#include "City.h"

FileType fileType;

int main(int argc, char* argv[])
{
	City utz = { NULL,0 };
	int uReq, typeIndicator, numberofparameters =0;

	if(argc != 2)
	{
		printf("You must enter 0 for text file and 1 for binary file");
		return -1;
	}

	numberofparameters = sscanf(argv[1] , "%d" , &typeIndicator);
	fileType = typeIndicator;

	if(numberofparameters != 1)
		return -1;


	//first time read
	readCity(&utz);
	do
	{
		uReq = menu();
		switch (uReq)
		{
		case  READ_CITY:
			readCity(&utz); 
			break;

		case  SHOW_CITY:
			showCityGardens(&utz);
			break;

		case  SHOW_GARDEN:
			showSpecificGardenInCity(&utz);
			break;

		case  WRITE_CITY:
			saveCity(&utz);
			break;

		case  ADD_GARDEN:
			cityAddGarden(&utz);
			break;

		case  ADD_CHILD:
			addChildToSpecificGardenInCity(&utz);
			break;

		case  CHILD_BIRTHDAY:
			birthdayToChild(&utz);
			break;

		case COUNT_GRADUATE:
			printf("There are %d children going to school next year\n",countChova(&utz));
			break;

		case SORT_MENU:
			sortingMenu(&utz);
            break;

		case LINKED_LIST:
			kindergartensLinkedList(&utz);
			break;
		}
	}while (uReq!=EXIT);

	releaseCity(&utz);//free all allocations

	return EXIT_SUCCESS;
}

