#include "RandomNumberGenerator.h"

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
	position->x = round(x_map);
	position->y = round(y_map);
}

void TRNG::initSavingPositions(std::string& filename)
{
	output_positions = new std::ofstream(filename, std::ios::binary | std::ios::out | std::ios::app);
}

void TRNG::savePosistion(const POINT* position)
{
	output_positions->write((char*)position, sizeof(POINT));
}

void TRNG::disableSavingPositions()
{
	delete output_positions;
}

void TRNG::initReadingPositions(std::string& filename)
{
	input_positions = new std::ifstream(filename, std::ios::binary);

	bufferPosition = new char[sizeof(POINT)];
}

bool TRNG::readPosition(POINT* position)
{
	input_positions->read(bufferPosition, sizeof(POINT));
	POINT* tempPosition = (POINT*)bufferPosition;

	if (input_positions->eof()) return false;

	position->x = tempPosition->x;
	position->y = tempPosition->y;

	return true;
}

void TRNG::disableReadingPositions()
{
	delete input_positions;
}

void TRNG::initGeneratingNumbers()
{
	number_bits.reset();
}

void TRNG::generateNumber(uint8_t* number, std::vector<POINT>& vec_positions)
{
	POINT pos;

	for (auto pos_it = vec_positions.begin(); pos_it != vec_positions.end(); ++pos_it)
	{
		pos = *pos_it;
		
		if (pos.x <= 63) 
		{
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

	*number = (uint8_t)(number_bits.to_ulong());
}
