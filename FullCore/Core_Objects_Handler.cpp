//#include "163CommonHeader.h"
//
//std::vector<std::string> buffer;
//
//
//
//
//
//void ParametersCoreDistribution();
//
//int ProfileHandler(bool save,bool automatic, LoadingDatasType type)
//{
////	
//	static TCHAR name[256] = _T(" ");
//	HANDLE consoleHandle_COH = GetStdHandle(STD_OUTPUT_HANDLE);
//	static OPENFILENAME file;
//	std::ifstream inputFile;
//	std::ofstream outputFile;
//	std::string st;
//	HINSTANCE hInst=0;
//
//	file.lStructSize = sizeof(OPENFILENAME);
//	file.hInstance = hInst;
//	file.lpstrFilter = "PVM-File\0*.pvm";
//	file.lpstrFile = name;
//	file.nMaxFile = 256;
//	file.lpstrInitialDir = ".\\";
//	file.lpstrDefExt = "txt";
//
//	if (save)
//	{
//		AssemblyConfigurationSaving();
//		if (!automatic)
//		{
//			file.lpstrTitle = _T("Открыть файл для записи");
//			file.Flags = OFN_NOTESTFILECREATE;
//			if (!GetSaveFileName(&file)) return 1;
//			outputFile.open(name);
//			for (auto i:buffer) outputFile << i << '\n';
//			outputFile.close();
//		}
//		else
//		{
//			std::string name_current;
//			if (!GetSaveFileName(&file))
//				name_current = "D:/Vasiliev_Others/CppTestDir/bin/data/profiles/temp_";
//			else
//				name_current = name;
//			outputFile.open(name_current);
//			for (auto i : buffer) outputFile << i << '\n';
//			outputFile.close();
//		}
//		buffer.clear();
//	}
//	else
//	{
//		file.lpstrTitle = "Открыть файл для чтения";
//		file.Flags = OFN_HIDEREADONLY;
//		if (!GetOpenFileName(&file)) {
//		//	SetConsoleTextAttribute(consoleHandle_COH, 12);
//			printf("ERROR LOADING\n");
//		//	SetConsoleTextAttribute(consoleHandle_COH, 7);
//			return 1;
//		}
//
//		inputFile.open(name);
//		while (getline(inputFile, st)) 
//			buffer.push_back(st);
//		inputFile.close();
//		if (ToUpperFunct(buffer[0]).find("COORDS") != -1)
//		{
//			// Вызывается функция <LOADING>
//			// Потом внутри этих функций побрасываются строчки из функции 
//			// ParametersCoreThrow(0,NUM);
//			// Оттуда извлекаются СТРОКОВЫЕ величины buffer[NUM], которые 
//			// туда записываются при чтении файла.
//			// Внутри функций <LOADING> определяется какие параметры извлекаются и как обрабатываются.
//
//			// ERF Если запускаем _ABS - сразу расчет
//			// если запускаем _TTN  - нужно прописывать END
//			if (type==absolute)
//				CoordsLoading(absolute);
//			if (type==tutnov)
//				CoordsLoading(tutnov);
//		}
//		else
//			AssemblyConfigurationLoading();
//		
//	}
//	ParametersCoreDistribution();
//	return 0;
//}
//
//void ParametersCoreDistribution()
//{
//	buffer.clear();
//}
//
//void ParametersCoreSave(std::string inputLine)
//{
//	buffer.push_back(inputLine);
//}
//
//std::string ParametersCoreThrow(bool ss, int num)
//{
//	if (ss)
//		return (std::to_string(buffer.size()));
//	else
//		return (buffer[num]);
//}
//
