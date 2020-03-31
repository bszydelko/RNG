#include <iostream>
#include "RandomNumberGenerator.h"
#include <Windows.h>

int main()
{
	POINT mousePosition;
	TRNG Trng;


	int numberCounter = 0;
	const int MAX_NUMBERS = 100; 

	int positionsCounter = 0;
	const int MAX_POSITIONS = 512;

	std::string positionsFilename = "pozycje.bin";
	
	Trng.initMapping();
	Trng.initSavingPositions(positionsFilename);
	
	while (numberCounter < MAX_NUMBERS)
	{
		while (positionsCounter < MAX_POSITIONS)
		{
			GetCursorPos(&mousePosition);
			Sleep(1);
			Trng.mapPosition(&mousePosition);
			Trng.savePosistion(&mousePosition);

			positionsCounter++;
		}
		
		positionsCounter = 0;
		//std::cout << "number counter: " << numberCounter + 1 << std::endl;
		numberCounter++;
	}

	Trng.disableSavingPositions();

	numberCounter = 0;
	positionsCounter = 0;

	uint8_t number;
	POINT position;
	std::vector<POINT> vec_positions(MAX_POSITIONS);

	Trng.initReadingPositions(positionsFilename);
	

	while (numberCounter < MAX_NUMBERS)
	{
		Trng.initGeneratingNumbers();
		vec_positions.clear();

		while (positionsCounter < MAX_POSITIONS)
		{
			Trng.readPosition(&position);
			vec_positions.emplace_back(position);

			positionsCounter++;
		}

		Trng.generateNumber(&number, vec_positions);

		std::cout <<"rn: "<< (int)number << std::endl;
		positionsCounter = 0;
		numberCounter++;
	}

	//TESTING
	//POINT pos;
	//int c = 0;

	//Trng.initReadingPositions(positionsFilename);
	//
	//while (Trng.readPosition(&pos))
	//{
	//	std::cout << pos.x << ", " << pos.y << std::endl;
	//	//std::cout << ++c << std::endl;
	//}
	

	return 0;
}

