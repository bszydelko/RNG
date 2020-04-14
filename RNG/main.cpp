#include <iostream>
#include "RandomNumberGenerator.h"
#include <Windows.h>

int main()
{
	int MAX_NUMBERS = 10000; //ile liczb wygenerowac
	const int MAX_POSITIONS = 512; //pozycje na 1 liczbe

	TRNG Trng(MAX_NUMBERS, MAX_POSITIONS);


	while (true)
	{
		system("CLS");
		int task = 0;
		//print menu
		std::cout << "1. Uruchom pobieranie pozycji kursora myszy i ich mapowanie"<<std::endl;
		std::cout << "2. Wygeneruj liczby na podstawie pozycji"<<std::endl;
		std::cout << "------------------------------"<<std::endl;
		std::cout << "3. Uruchom postprocessing pozycji"<<std::endl;
		std::cout << "4. Wygeneruj liczby po postprocessingu"<< std::endl;
		std::cout << "------------------------------"<<std::endl;
		std::cout << "5. Wydrukuj liczby PRZED postprocessingiem"<< std::endl;
		std::cout << "6. Wydrukuj liczby PO postprocessingu"<< std::endl;
		std::cout << "------------------------------" << std::endl;
		std::cout << "7. Eksportuj liczby PREZD pp. do TXT" << std::endl;
		std::cout << "8. Eksportuj liczby PO pp. do TXT" << std::endl;

		std::cin >> task;

		switch (task)
		{
		case 1:
		{
			//otwarcie pliku do zapisywania zmapowanych pozycji
			Trng.openFileToSave("pozycje.bin");
			//pobieranie pozycji kursora i mapowanie
			Trng.initMapping();
			Trng.mapping();
			//zapisanie i zakmniecie pliku
			Trng.closeFileToSave();

			std::cout << "Pozycje pobrane!" << std::endl;
			system("PAUSE");
			break;
		}
		case 2:
		{
			//otwarcie plikow do odczytu pozycji i zapisu liczb
			Trng.openFileToRead("pozycje.bin");
			Trng.openFileToSave("liczby_losowe.bin");
			//generowanie liczb
			Trng.initGeneratingNumbers();
			Trng.generateNumbers();
			//zamkniecie wszystkich plikow
			Trng.closeFileToSave();
			Trng.closeFileToRead();
			std::cout << "Liczby wygenerowane!"<<std:: endl;
			system("PAUSE");
			break;
		}
		case 3:
		{
			Trng.openFileToRead("pozycje.bin");
			Trng.openFileToSave("pozycjeArnoldsCat.bin");

			Trng.initPostprocessing();
			//liczba iteracji musi byc parzysta
			//domyslnie jest 2
			//384 iteracje przywracaja oryginalne liczby;
			Trng.ArnoldsCatMap(12); 

			Trng.closeFileToSave();
			Trng.closeFileToRead();
			std::cout << "Postprocessing pozycji wykonany!" << std::endl;
			system("PAUSE");
			break;
		}
		case 4:
		{
			Trng.openFileToRead("pozycjeArnoldsCat.bin");
			Trng.openFileToSave("liczby_losowe_ArnoldsCat.bin");

			Trng.initGeneratingNumbers();
			Trng.generateNumbers();

			Trng.closeFileToSave();
			Trng.closeFileToRead();
			std::cout << "Liczby po postprocessingu wygenerowane!" << std::endl;
			system("PAUSE");
			break;
		}
		case 5:
		{
			Trng.openFileToRead("liczby_losowe.bin");
			Trng.printNumbers();
			Trng.closeFileToRead();
			system("PAUSE");
			break;
		}
		case 6:
		{
			Trng.openFileToRead("liczby_losowe_ArnoldsCat.bin");
			Trng.printNumbers();
			Trng.closeFileToRead();
			system("PAUSE");
			break;
		}
		case 7:
		{
			Trng.exportToTxtNumbers();
			std::cout << "Eksport zakonczony!"<<std::endl;
			system("PAUSE");
			break;
		}
		case 8:
		{
			Trng.exportToTxtArnoldsNumbers();
			std::cout << "Eksport zakonczony!"<<std::endl;
			system("PAUSE");
			break;
		}
		}
	}
	return 0;
}

