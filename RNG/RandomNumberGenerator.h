#pragma once
#include <Windows.h>
#include <fstream>
#include <string>
#include <math.h>
#include <bitset>
#include <vector>



class TRNG
{
private:
	unsigned int MAX_NUMBERS;
	unsigned int MAX_POSITIONS;
	
	unsigned int SCREEN_WIDTH;
	unsigned int SCREEN_HEIGHT;

	float MAP_RATIO_WIDTH;
	float MAP_RATIO_HEIGHT;

	unsigned int IMAGE_WIDTH;
	unsigned int IMAGE_HEIGHT;
	unsigned int IMAGE_BLOCK_SIZE;

	float x_map;
	float y_map;
	//FILES

	std::string positions_filename;
	std::string numbers_filename;

	std::ofstream* save_file_handler;
	std::ifstream* read_file_handler;

	char* point_buffer;
	char* number_buffer;
	//MOUSE PATTERNS-VECTOR OF BITSETS

	std::bitset<8> number_bits;
	std::vector<POINT> positions_per_number;

	bool** image;
	bool** imageArnoldsCat;



public:
	TRNG(const unsigned int max_numbers, const unsigned int max_positions) :
		MAX_NUMBERS(max_numbers),
		MAX_POSITIONS(max_positions),
		SCREEN_WIDTH(0),
		SCREEN_HEIGHT(0),
		MAP_RATIO_WIDTH(0.0f),
		MAP_RATIO_HEIGHT(0.0f),
		IMAGE_WIDTH(256),
		IMAGE_HEIGHT(128),
		IMAGE_BLOCK_SIZE(64),
		x_map(0.0f),
		y_map(0.0f)
	{}

	void initMapping();
	void mapPosition(POINT* position);
	void mapping();

	void openFileToRead(const char* filename);
	void openFileToSave(const char* filename);

	void closeFileToRead();
	void closeFileToSave();

	void saveToFile(const POINT* position);
	void saveToFile(const int random_number);

	bool readFromFile(POINT* position);
	bool readFromFile(int& random_number);
	
	void initGeneratingNumbers();
	void generateNumbers();
	void countNumberBits(int& random_number);

	void printNumbers();

	void initPostprocessing();
	void resetImage();
	void ArnoldsCatMap(int iterations);
	void disablePostprocessing();

	void printImage(bool** image); //nie widac za duzo

	void exportToTxtNumbers();
	void exportToTxtArnoldsNumbers();

	

};