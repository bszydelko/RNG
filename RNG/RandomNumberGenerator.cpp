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


