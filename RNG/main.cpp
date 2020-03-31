#include <iostream>
#include "RandomNumberGenerator.h"
#include <Windows.h>

int main()
{
	POINT mousePosition;
	TRNG Trng;


	int numberCounter = 0;
	const int MAX_NUMBERS = 10; 

	int positionsCounter = 0;
	const int MAX_POSITIONS = 50;

	std::string positionsFilename = "pozycje.bin";
	
	Trng.initMapping();
	Trng.initSavingPositions(positionsFilename);
	
	while (numberCounter < MAX_NUMBERS)
	{
		while (positionsCounter < MAX_POSITIONS)
		{
			GetCursorPos(&mousePosition);
			Trng.mapPosition(&mousePosition);
			Trng.savePosistion(&mousePosition);

			positionsCounter++;
		}
		
		positionsCounter = 0;
		std::cout << "number counter: " << numberCounter + 1 << std::endl;
		numberCounter++;
	}

	Trng.disableSavingPositions();

	//TESTING
	POINT pos;
	int c = 0;

	Trng.initReadingPositions(positionsFilename);
	
	while (Trng.readPosition(&pos))
	{
		//std::cout << pos.x << ", " << pos.y << std::endl;
		std::cout << ++c << std::endl;
	}
	

	return 0;
}

