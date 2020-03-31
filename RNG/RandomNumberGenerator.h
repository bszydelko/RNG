#pragma once
#include <Windows.h>
#include <fstream>
#include <string>
#include <math.h>
#include <Bits.h>
#include <bitset>
#include <vector>



class TRNG
{
private:
	
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

	std::ofstream* output_positions;
	std::ifstream* input_positions;

	char* bufferPosition;
	//MOUSE PATTERNS-VECTOR OF BITSETS

	std::bitset<8> number_bits;



public:
	TRNG() :
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

	void initSavingPositions(std::string& filename);
	void savePosistion(const POINT* position);
	void disableSavingPositions();

	void initReadingPositions(std::string& filename);
	bool readPosition(POINT* position);
	void disableReadingPositions();

	//TODOs
	void initGeneratingNumbers();
	void generateNumber(uint8_t* number, std::vector<POINT>& vec_positions);
	void disableGeneratingNumbers();

	bool initSavingNumbers(std::string& filename);
	void saveNumber(unsigned int number);
	void disableSavingNumbers();



};