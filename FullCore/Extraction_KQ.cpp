//#include "163CommonHeader.h"
//
//
//bool R_String(short string_number, bool core_with_reflector);
//void R_String_Processor(std::string input_string);
//void Writting_kq_permak();
//
//
//std::vector<double> kq_permak;
//
//std::vector<double> GetPermakKq() {
//	return kq_permak;
//}
//
//
//
//void Extracting_KQ()
//{
//	bool reflector = true;
//
//	std::ifstream FA("D:/Vasiliev/PERMAK-A/XIPI_18L_W/ROSTOV/B02/K01/permVAS.out");
//	
//
//	short counter = 0;
//	std::string s;
//
//	// Открываем файл с кассетами
//	if (!FA.is_open())
//		std::cout << "ERROR OPENING RESULT FILE\n";
//	else
//	{
//		while (!FA.eof())
//		{
//			// Читаем по строкам пока не найдем эту фразу
//			getline(FA, s);
//			if (s.find("6-DEVIATION FROM") !=-1)
//			{
//				counter = 0;
//				while (counter < 120)
//				{
//					getline(FA, s);
//					counter++;
//					if (R_String(counter, true))
//						R_String_Processor(s);
//				}
//				if (counter > 110)
//					break;
//			}
//		}
//	}
//
//	Writting_kq_permak();
//	FA.close();
//	
////	kq_permak.clear();
//}
//
//bool R_String(short string_num, bool reflector)
//{
//	const int ROWS = 15;
//	std::vector<short> k;
//	switch (reflector)
//	{
//	case true:
//	{
//		for (int i = 0; i < ROWS; i++)
//		{
//			k.push_back(8 + 6 * (i));
//		}
//		break;
//	}
//	case false:
//	{
//
//		break;
//	}
//	// на данный момент не рассматривается
//	}
//
//	for (unsigned short i = 0; i < k.size(); i++)
//	{
//		if (string_num == k[i])
//		{
//			k.clear();
//			return true;
//		}
//	}
//	k.clear();
//	return false;
//}
//
//// вынимает из строки пробелы
//void R_String_Processor(std::string input)
//{
//	std::string str;
//
//	for (unsigned short i = 0; i < input.size(); i++)
//	{
//		// Если не пробел
//		if (char(input[i])!=32)
//		{
//			str += input[i];
//		}
//		else
//		{
//			if (!str.empty())
//			{
//				double value = std::stod(ReturnNumbers(str));
//				if (value != 0)
//					kq_permak.push_back(value);
//				str.clear();
//			}
//		}
//	}
//}
//
//void Writting_kq_permak()
//{
//	std::vector<double> result(kq_permak.size());
//	std::ofstream FA_R("D:/Vasiliev/PERMAK-A/Local_new/bin/res/crc/permak_res.txt");
//
//	int numBlock[163] = {
//		158,159,160,161,162,163,149,150,151,152,153,154,155,156,157,139,
//		140,141,142,143,144,145,146,147,148,128,129,130,131,132,133,134,
//		135,136,137,138,116,117,118,119,120,121,122,123,124,125,126,127,
//		103,104,105,106,107,108,109,110,111,112,113,114,115,89,90,91,
//		92,93,94,95,96,97,98,99,100,101,102,76,77,78,79,80,
//		81,82,83,84,85,86,87,88,62,63,64,65,66,67,68,69,
//		70,71,72,73,74,75,49,50,51,52,53,54,55,56,57,58,
//		59,60,61,37,38,39,40,41,42,43,44,45,46,47,48,26,
//		27,28,29,30,31,32,33,34,35,36,16,17,18,19,20,21,
//		22,23,24,25,7,8,9,10,11,12,13,14,15,1,2,3,
//		4,5,6,
//	};
//
//	for (unsigned short i=0; i<kq_permak.size(); i++)
//	{
//		result[numBlock[i]-1]=kq_permak[i];
//	}
//	
//	kq_permak = result;
//
//	for (auto i : result)
//	{
//		FA_R << i << std::endl;
//	}
//
//	FA_R.close();
//}