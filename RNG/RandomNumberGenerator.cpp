#include "RandomNumberGenerator.h"
#include <iostream>
#include <fstream>

void TRNG::initMapping()
{
	SCREEN_WIDTH = GetSystemMetrics(SM_CXSCREEN);
	SCREEN_HEIGHT = GetSystemMetrics(SM_CYSCREEN);

	MAP_RATIO_WIDTH = (float)SCREEN_WIDTH / (float)IMAGE_WIDTH;
	MAP_RATIO_HEIGHT = (float)SCREEN_HEIGHT / (float)IMAGE_HEIGHT;
}

void TRNG::mapPosition(POINT* position)
{
	x_map = position->x / MAP_RATIO_WIDTH;
	y_map = position->y / MAP_RATIO_HEIGHT;
	//modyfing point
	int x_map_round = x_map = round(x_map);
	int y_map_round = y_map = round(y_map);

	if (x_map_round >= 256) x_map_round %= 255;
	if (y_map_round >= 128) y_map_round %= 127;

	position->x = x_map_round;
	position->y = y_map_round;
}

void TRNG::mapping()
{
	POINT mousePositionCurrent;
	POINT mousePositionPrevious;

	int numberCounter = 0;
	int positionsCounter = 0;

	GetCursorPos(&mousePositionPrevious);

	while (numberCounter < MAX_NUMBERS)
	{
		
		while (positionsCounter < MAX_POSITIONS)
		{
			GetCursorPos(&mousePositionCurrent);
			
			while (abs(mousePositionCurrent.x - mousePositionPrevious.x) <= 1 ||
				abs(mousePositionCurrent.y - mousePositionPrevious.y) <= 1) 
			{
				GetCursorPos(&mousePositionCurrent);
			}

			mousePositionPrevious.x = mousePositionCurrent.x;
			mousePositionPrevious.y = mousePositionCurrent.y;

			mapPosition(&mousePositionCurrent);
			saveToFile(&mousePositionCurrent);

			positionsCounter++;
			
		}
		system("CLS");
		positionsCounter = 0;
		std::cout << "liczby:  " << numberCounter + 1 << "/ " <<MAX_NUMBERS<< std::endl;
		numberCounter++;
	}
}


void TRNG::openFileToRead(const char* filename)
{

	read_file_handler = new std::ifstream(filename, std::ios::binary);
	point_buffer = new char[sizeof(POINT)];
	number_buffer = new char[sizeof(int)];
}

void TRNG::openFileToSave(const char* filename)
{
	save_file_handler = new std::ofstream(filename, std::ios::binary | std::ios::out | std::ios::app);
}


void TRNG::closeFileToRead()
{
	delete [] point_buffer;
	delete [] number_buffer;
	delete read_file_handler;
}

void TRNG::closeFileToSave()
{
	delete save_file_handler;
}

void TRNG::saveToFile(const POINT* position)
{
	save_file_handler->write((char*)position, sizeof(POINT));
}

void TRNG::saveToFile(const int random_number)
{
	save_file_handler->write((char*)&random_number, sizeof(int));
}


bool TRNG::readFromFile(POINT* position)
{
	read_file_handler->read(point_buffer, sizeof(POINT));
	POINT* temp = (POINT*)point_buffer;

	if (read_file_handler->eof()) return false;
	
	position->x = temp->x;
	position->y = temp->y;

	return true;
}

bool TRNG::readFromFile(int& random_number)
{
	read_file_handler->read(number_buffer, sizeof(int));
	int* temp = (int*)number_buffer;

	if (read_file_handler->eof()) return false;

	random_number = *temp;

	return true;
}

void TRNG::initGeneratingNumbers()
{
	number_bits.reset();
}

void TRNG::generateNumbers()
{
	int random_number = 0;
	POINT position;

	positions_per_number.reserve(MAX_POSITIONS);

	while (readFromFile(&position))
	{
		positions_per_number.emplace_back(position);

		if (positions_per_number.size() == MAX_POSITIONS)
		{
			countNumberBits(random_number);
			saveToFile(random_number);

			positions_per_number.clear();
		}
		//std::cout << "rn: " << (int)random_number << std::endl;
	}
}

void TRNG::countNumberBits(int& random_number)
{
	POINT pos;

	for (auto pos_it = positions_per_number.begin(); pos_it != positions_per_number.end(); ++pos_it)
	{
		pos = *pos_it;
		
		if (pos.x <= 63) 
		{	//flip neguje dany bit
			if (pos.y <= 63) number_bits.flip(7); //b7
			else number_bits.flip(3);//b3
		}
		else if (pos.x <= 127)
		{
			if (pos.y <= 63) number_bits.flip(6);//b6
			else number_bits.flip(2);//b2
		}
		else if (pos.x <= 191)
		{
			if (pos.y <= 63) number_bits.flip(5);//b5
			else number_bits.flip(1);//b1
		}
		else
		{
			if (pos.y <= 63) number_bits.flip(4);//b4
			else number_bits.flip(0);//b0
		}
	}
	random_number = (int)(number_bits.to_ulong());
}

void TRNG::printNumbers()
{
	int random_number = 0;

	while (readFromFile(random_number))
	{
 		std::cout << "number: " << random_number << std::endl;
	}
}

void TRNG::initPostprocessing()
{
	image = new bool*[IMAGE_HEIGHT];
	imageArnoldsCat = new bool*[IMAGE_HEIGHT];

	for (auto i = 0; i < IMAGE_HEIGHT; i++)
	{
		image[i] = new bool[IMAGE_WIDTH];
		imageArnoldsCat[i] = new bool[IMAGE_WIDTH];
	}
}

void TRNG::resetImage()
{
	for (auto h = 0; h < IMAGE_HEIGHT; h++)
	{
		for (auto w = 0; w < IMAGE_WIDTH; w++)
		{
			image[h][w] = 0;
			imageArnoldsCat[h][w] = 0;
		}
	}
}

void TRNG::ArnoldsCatMap(int iterations = 2)
{
	POINT position;
	POINT new_position;
	int b = 134, c = 84, x_new = 0, y_new = 0;

	while (readFromFile(&position))
	{
		x_new = (1 * position.x + b * position.x) % IMAGE_WIDTH;
		y_new = (c * position.y + (b + c + 1) * position.y) % IMAGE_HEIGHT;

		new_position.x = x_new;
		new_position.y = y_new;
		saveToFile(&new_position);

	}

	closeFileToRead();
	closeFileToSave();

	const char* fnArnold = "pozycjeArnoldsCat.bin";
	const char* fnArnoldPom = "pozycjeArnoldsCatTEMP.bin";

	for (auto i = 0; i < iterations; i++) 
	{
		std::cout << i << std::endl;

		openFileToRead(fnArnold);
		openFileToSave(fnArnoldPom);

		while (readFromFile(&position))
		{
			x_new = (1 * position.x + b * position.x) % IMAGE_WIDTH;
			y_new = (c * position.y + (b * c + 1) * position.y) % IMAGE_HEIGHT;

			new_position.x = x_new;
			new_position.y = y_new;
			saveToFile(&new_position);

		}

		closeFileToRead();
		closeFileToSave();

		std::remove(fnArnold);
		std::swap(fnArnold, fnArnoldPom);

	}

	openFileToRead("pozycje.bin");
	openFileToSave("pozycjeArnoldsCat.bin");

}


void TRNG::disablePostprocessing()
{
	for (auto i = 0; i < IMAGE_WIDTH; i++)
	{
		delete [] image[i];
		delete [] imageArnoldsCat[i];
	}
	delete [] image;
	delete [] imageArnoldsCat;
}

void TRNG::printImage(bool** img)
{
	for (auto h = 0; h < IMAGE_HEIGHT; h++)
	{
		std::cout << std::endl;

		for (auto w = 0; w < IMAGE_WIDTH; w++)
		{
			//std::cout << "11";
			//if (img[h][w]) std::cout << img[h][w];
			//else std::cout << " ";
		}
	}
}

void TRNG::exportToTxtNumbers()
{
	std::ofstream fileTxt;
	fileTxt.open("liczbyLosowe.txt");

	openFileToRead("liczby_losowe.bin");
	
	int number = 0;

	while (readFromFile(number))
	{
		fileTxt << number << "\n";
	}

	fileTxt.close();
	closeFileToRead();
}

void TRNG::exportToTxtArnoldsNumbers()
{
	std::ofstream fileTxt;
	fileTxt.open("liczbyLosoweArnoldsCat.txt");

	openFileToRead("liczby_losowe_ArnoldsCat.bin");

	int number = 0;

	while (readFromFile(number))
	{
		fileTxt << number << "\n";
	}

	fileTxt.close();
	closeFileToRead();
}
