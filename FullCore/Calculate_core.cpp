#include "163CommonHeader.h"
#include "Assemblies_Object.h"


#include <Windows.h>
#include <tchar.h>
#include <string>
#include <iostream>
#include<direct.h>


bool modifier = false;

void Initializing()
{
		FA_Define();
		DefMapType();
		DefKasType();
		DefineMainAssemblyParameters();
		Gap_Define_S(modifier, false);
		x5Define();

}




void fCore_Main(bool withMCs)
{
	std::string s="-1";
	int kl = 0;
	bool initialized = false;
	modifier = withMCs;
	HANDLE consoleHandle_CC = GetStdHandle(STD_OUTPUT_HANDLE);
	//ListDefine();
	// Загрузка профиля
	// Определение основных параметров ТВС
	ListDefine();

	int err = -1;
	printf("<NEW> - create new configuration\n");
	printf("<LOAD> - to load common profile\n");
	printf("<LOAD> parameters: \n");
	printf("\t' -COORD' - load coords only\n");
	printf("\t\t- '_ttn' - load datas from mr. Tutnov\n");
	printf("\t\t- '_abs' - load absolute values of FA's center\n");
	printf("<SAVE> - save current configuration\n");
	

	// Начинаем работу с зазорами
	while ((s.find("END") == -1))
	{
		s.clear();
		printf("Enter command of FA number\n");
		
		std::cin >> s;

		
		
		// Получили строку, вытаскиваем из нее номер и пишем в KL
		kl = std::stoi(ReturnNumbers(s));
		// Смотрим, получили ли мы номер или нет + проверка: была ли инициализация
		if (((kl > 0) && (kl <= 163)) && (initialized))
			FA_Movement(kl);
		else
		{
			if (!initialized)
			{
				SetConsoleTextAttribute(consoleHandle_CC, 12);
				printf("Objects are not initialized!\n");
				SetConsoleTextAttribute(consoleHandle_CC, 7);
			}
			s = ToUpperFunct(s);
		}

		if (s.find("END") != -1) {
			if (initialized) break;
			else s.clear();
		}

		if (s.find("LOAD") != -1)
		{
			// если встретили KEYWORD <LOAD>
			if (s.find("-COORD") != -1)
			{
				if (s.find("_ABS") != -1)
				{
					// если есть параметр <-COORD> - инициализируем и загружаем координаты
					Initializing();
					SetConsoleTextAttribute(consoleHandle_CC, 10);
					printf("Start loading coordinates (absolute values)\n");
					SetConsoleTextAttribute(consoleHandle_CC, 7);
					err = ProfileHandler(0, 0, absolute);
					Gap_Define_S(modifier, false);
					printf("Errors: %i\n", err);
					break;
				}
				if (s.find("_TTN") != -1) {
					Initializing();
					SetConsoleTextAttribute(consoleHandle_CC, 10);
					printf("Start loading coordinates (TUTNOV data types)\n");
					SetConsoleTextAttribute(consoleHandle_CC, 7);
					err = ProfileHandler(0, 0, tutnov);
					Gap_Define_S(modifier, false);
					printf("Errors: %i\n", err);
				}
				else {
					// Если просто <LOAD> - загружаем весь профиль
					SetConsoleTextAttribute(consoleHandle_CC, 10);
					printf("Start loading profile\n");
					SetConsoleTextAttribute(consoleHandle_CC, 7);
					err = ProfileHandler(0, 0, absolute);
					//		Gap_Define_S(modifier, false);
					printf("Errors: %i\n", err);
				}
				// Считаем, что профиль инициализирован
			}
		}

		if ((s.find("SAVE") != -1) && (initialized))
		{
			printf("Saving initialized...\t");
			err = ProfileHandler(1, 0, absolute);
			printf("Completed. Errors: %i\n", err);
		}
		if (s.find("NEW") != -1)
		{
			printf("Program initializing\n");
			Initializing();
			err = 0;
			printf("Completed\n ");
		}
		if ((err == 0)&&(!initialized)) {
			SetConsoleTextAttribute(consoleHandle_CC, 14);
			printf("Profile has been initialized!\n");
			SetConsoleTextAttribute(consoleHandle_CC, 7);
			initialized = true;
		}
	}

	Gap_Define_S(modifier, true);



	// Конец работы
	PermparMaking();

	TCHAR current_path[80] = "D:/Vasiliev/PERMAK-A/XIPI_18L_W/";

	_chdir(current_path);

	//GetCurrentDirectory(100, current_path);
	
	
	system("PERMAK2007I.exe");

	Extracting_KQ();
	kk_calculating();

}