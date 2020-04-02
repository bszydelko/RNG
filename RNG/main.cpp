#include <iostream>
#include "RandomNumberGenerator.h"
#include <Windows.h>

int main()
{
	const int MAX_NUMBERS = 1; //ile liczb wygenerowac
	const int MAX_POSITIONS = 1000; //pozycje na 1 liczbe

	TRNG Trng(MAX_NUMBERS, MAX_POSITIONS);
	//otwarcie pliku do zapisywania zmapowanych pozycji
	Trng.openFileSave("pozycje.bin");
	//pobieranie pozycji kursora i mapowanie, moze zmineic nazwe mapping na inna?
	Trng.initMapping();
	Trng.mapping();
	//zapisanie i zakmniecie pliku
	Trng.closeFileSave();

	//otwarcie plikow do odczytu pozycji i zapisu liczb
	Trng.openFileRead("pozycje.bin");
	Trng.openFileSave("liczby_losowe.bin");
	//generowanie liczb
	Trng.initGeneratingNumbers();
	Trng.generateNumbers();
	//zamkniecie wszystkich plikow
	Trng.closeFileSave();
	Trng.closeFileRead();
	//testowe drukowanie wygenerowanych liczb
	Trng.openFileRead("liczby_losowe.bin");
	Trng.printNumbers();
	Trng.closeFileRead();

	
	Trng.openFileRead("pozycje.bin");
	Trng.openFileSave("pozycjeMASK.bin");

	Trng.initPostprocessing();
	Trng.postprocessing();

	
	Trng.closeFileSave();
	Trng.closeFileRead();

	//w sumie potem odczytywanie pozycji po MASKowaniu
	//i nowe wygenerowanie liczb

	return 0;
}

