#include "RandomNumberGenerator.h"
#include <iostream>

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

	if (x_map >= 256) x_map = 255;
	if (y_map >= 128) y_map = 127;

	position->x = round(x_map);
	position->y = round(y_map);
}

void TRNG::mapping()
{
	POINT mousePosition;

	int numberCounter = 0;
	int positionsCounter = 0;

	while (numberCounter < MAX_NUMBERS)
	{
		while (positionsCounter < MAX_POSITIONS)
		{
			GetCursorPos(&mousePosition);
			Sleep(1);
			mapPosition(&mousePosition);
			saveToFile(&mousePosition);

			positionsCounter++;
		}

		positionsCounter = 0;
		//std::cout << "number counter: " << numberCounter + 1 << std::endl;
		numberCounter++;
	}
}


void TRNG::openFileRead(const char* filename)
{
	read_file_handler = new std::ifstream(filename, std::ios::binary);
	point_buffer = new char[sizeof(POINT)];
	number_buffer = new char[sizeof(int)];
}

void TRNG::openFileSave(const char* filename)
{
	save_file_handler = new std::ofstream(filename, std::ios::binary | std::ios::out | std::ios::app);
}


void TRNG::closeFileRead()
{
	delete [] point_buffer;
	delete [] number_buffer;
	delete read_file_handler;
}

void TRNG::closeFileSave()
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
	int numberCounter = 0;
	int positionsCounter = 0;

	int random_number;
	POINT position;

	positions_per_number.reserve(MAX_POSITIONS);

	while (numberCounter < MAX_NUMBERS)
	{
		positions_per_number.clear();

		while (positionsCounter < MAX_POSITIONS)
		{
			readFromFile(&position);
			positions_per_number.emplace_back(position);

			positionsCounter++;
		}

		countNumberBits(random_number);

		saveToFile(random_number);
		//std::cout << "rn: " << (int)random_number << std::endl;
		
		positionsCounter = 0;
		numberCounter++;
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
	imageMASK = new bool*[IMAGE_HEIGHT];

	for (auto i = 0; i < IMAGE_HEIGHT; i++)
	{
		image[i] = new bool[IMAGE_WIDTH];
		imageMASK[i] = new bool[IMAGE_WIDTH];
		//image[i] = malloc(IMAGE_WIDTH * sizeof *image[i]);
		//imageMASK[i] = malloc(IMAGE_WIDTH * sizeof *image[i]);
	}
}

void TRNG::resetImage()
{
	for (auto h = 0; h < IMAGE_HEIGHT; h++)
	{
		for (auto w = 0; w < IMAGE_WIDTH; w++)
		{
			image[h][w] = 0;
			imageMASK[h][w] = 0;
		}
	}
}

void TRNG::postprocessing()
{
	POINT position;

	while (readFromFile(&position))
	{
		resetImage();

		for (auto i = 0; i < MAX_POSITIONS; i++)
		{
			image[position.y][position.x] = 1;
		}

		//MASK(image, imageMASK);

	}
	printImage(image);
}
//TODO
void TRNG::MASK(bool** input_image, bool** output_image)
{
	//tutaj szyfrowanie mask
	//powstaje nowy obraz - nakladane sa punkty na wspolrzedne
	//te wspolrzedne zapisujemy do pliku pozycjeMASK.bin
	//zapis jednego punktu - jedno wywolanie funkcji saveToFile 
}

void TRNG::disablePostprocessing()
{
	for (auto i = 0; i < IMAGE_WIDTH; i++)
	{
		delete [] image[i];
		delete [] imageMASK[i];
	}
	delete [] image;
	delete [] imageMASK;
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
