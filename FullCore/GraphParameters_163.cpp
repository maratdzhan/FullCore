#include "163CommonHeader.h"
#include "GraphParameters_163.h"

#include <Windows.h>
#include <tchar.h>
#include <xstring>

#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

typedef std::basic_string<TCHAR, std::char_traits<TCHAR>,
	std::allocator<TCHAR> > String;

//// TEMPORARY
std::string strGraphParametersFile = "D:/Vasiliev/PERMAK-A/Local_new/bin/data/Cr/GraphParameters.ini";
std::string strMcuKqFile = "D:/Vasiliev/PERMAK-A/Local_new/bin/res/crc/mcu_res.txt";
std::string strPermakKqFile = "D:/Vasiliev/PERMAK-A/Local_new/bin/res/crc/permak_res.txt";
std::vector<double> mcu_kq;
std::vector<double> permak_kq;

void McukqRes_TEMPORARY()
{
	std::ifstream inputfile(strMcuKqFile);
	std::string str;

	while (!inputfile.eof())
	{
		getline(inputfile, str);
		if (!str.empty())
		{
			mcu_kq.push_back(stod(str));
		}
	}
	inputfile.close();
	inputfile.open(strPermakKqFile);
	while (!inputfile.eof())
	{
		getline(inputfile, str);
		if (!str.empty())
		{
			permak_kq.push_back(stod(str));
		}
	}
	inputfile.close();
}

std::vector<double> GetMcuKq()
{
	return mcu_kq;
}

////////////////////////////////////////////////
std::map<std::string, std::string> parameters;
std::map<std::string, int> stringsPosition;
std::vector<int> text_coords;
double scale=0;
double max_kq = -100000, min_kq = 10000000;

void SortResults()
{
	double dev = 0;
	for (unsigned short i = 0; i < permak_kq.size(); i++)
	{
		dev = (permak_kq[i] - mcu_kq[i]) / mcu_kq[i];
		if ((dev) > max_kq)
			max_kq = dev;
		if ((dev) < min_kq)
			min_kq = dev;
	}
}

void Coord_TextOutCore()
{
	// Полный размер области с текстом
	int count = stoi(parameters["STRINGS_COUNT"]);
	int step = (int)((1 + (1 / scale))*(stoi(parameters["TEXT_SIZE"])));
	int full_size = (count - 1)*(step);
	// Верхняя позиция
	int st_pos = -1*(step/3+full_size - (full_size / 2));
	text_coords.push_back(st_pos);
	// Начиная со второго элемента - прибавляем
	for (short i = 1; i < count; i++)
	{
		text_coords.push_back((text_coords[i - 1] + step));
	}
}



void GraphParameters_163Load()
{
	std::ifstream inputFile(strGraphParametersFile);
	std::string str;
	short tempSumma = 0;
	short stringsCount=0;

	if (!inputFile.is_open())
		std::cout<<"CANT OPEN PARAMETERS FILE FOR FULL CORE/nGraphParameters_163Load\n"<<std::endl;
	else
	{
		while (!inputFile.eof())
		{
			getline(inputFile, str);
			if (!str.empty())
			{
				parameters[GetParamS(str, 1)] = GetParamS(str, 2);
				if ((str.find("FA_NUMBER")!=-1) || (str.find("KQ_MCU_BOOL") != -1) || (str.find("KQ_PERMAK_BOOL") != -1) || (str.find("KQ_DEV") != -1))
				{
					tempSumma = GetParam(str, 3);
					stringsPosition[GetParamS(str, 1)] = tempSumma;
					if (GetParam(str, 2) > 0)
						stringsCount++;
				}
				if (str.find("COLOR_VAR") != -1)
				{
					if (stoi(parameters["COLOR_VAR"]) == 1)
					parameters["COLOR_COUNT"] = GetParamS(str, 3);
				}
				if (str.find("CRITICAL_ENERGY_RELEASE") != -1)
				{
					if (stoi(parameters["COLOR_VAR"]) == 1)
						parameters["CRITICAL_ER_VALUE"] = GetParamS(str, 3);
					else
						printf("SET COLORS COUNT FIRST!!!!!!!!!!!!");
				}
			}
		}
	}
	parameters["STRINGS_COUNT"] = std::to_string(stringsCount);
	
	if (stoi(parameters["SHOW_GAPS_SIZE"]) != 0) parameters["GRID_PITCH"] = std::to_string(1.25*(stod(parameters["GRID_PITCH"])));

	scale = stod(GraphParameters_163Return("SCALE_SIZE"));
	Coord_TextOutCore();

	//// TEMPORARY



	SortResults();
	inputFile.close();
}


void GraphParameters_163Clear()
{
	parameters.clear();
	text_coords.clear();
	stringsPosition.clear();
	permak_kq.clear();
	mcu_kq.clear();
	max_kq = -10000000;
	min_kq = 10000000;
}

std::string GraphParameters_163Return(std::string parameter_name)
{
	if (!parameters[parameter_name].empty())
		return parameters[parameter_name];
	else
		std::cout << "PARAMETER ''"<< parameter_name<<"'' NOT LOADED << ReturnParameters \n";
	return "-72";
}


void TVS_TextOutCore(HDC hdc, short tvs)
{
	// первое обращение -> tvs==1;
	std::string str[4];
	HFONT newFont, oldFont;
	short textSize =  (1 + (1 / scale))*stoi(GraphParameters_163Return("TEXT_SIZE"));
	std::string str1;

	// Задаем порядок выдачи и выдаваемую информацию
	{
		if (stoi(parameters["FA_NUMBER"]) == 1)
			str[stringsPosition["FA_NUMBER"]-1] = std::to_string(tvs);
		if (stoi(parameters["KQ_MCU_BOOL"]) == 1)
			str[stringsPosition["KQ_MCU_BOOL"]-1] = std::to_string(mcu_kq[tvs-1]);
		if (stoi(parameters["KQ_PERMAK_BOOL"]) == 1)
			str[stringsPosition["KQ_PERMAK_BOOL"]-1] = std::to_string(permak_kq[tvs-1]);
		if (stoi(parameters["KQ_DEV"]) == 1)
			str[stringsPosition["KQ_DEV"] - 1] = std::to_string(100.*(permak_kq[tvs - 1] - mcu_kq[tvs - 1]) / mcu_kq[tvs - 1]);
	}

	// Обработка координат
	// Вынесена в GraphParameters_163Load() как функция Coord_TextOutCore();


	// Вывод текста
	int test = (stoi(parameters["STRINGS_COUNT"]));
	for (short i = 0; i <test; i++)
	{
		// Черный цвет текста
		SetTextColor(hdc, 0);
		// Прозрачный фон
		SetBkMode(hdc, TRANSPARENT);
		// Создаем новый шрифт:
		newFont = CreateFont(textSize, 0, 0, 0, 700, 0, 0, 0, 
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
			PROOF_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Arial"));

		// Переприсваивание
		oldFont = (HFONT)SelectObject(hdc, newFont);

		unsigned short textLenght = stoi(parameters["TEXT_LENGHT"]);
		if (str[i].size() > textLenght)
		{
			for (short j = 0; j < textLenght; j++)
				str1 += str[i][j];
		}
		else
			str1 = str[i];
		
		TextOut(hdc, (int)(-1* (1 + ((textSize*2-scale)/(scale * scale)))*((str1.size()))), 
			(int)((1 + (1 / scale))*text_coords[i]), str1.data(), str1.size());
		SelectObject(hdc, oldFont);
		DeleteObject(newFont);
		str1.clear();
	}


}

void TVS_Solo_TextOutCore(HDC hdc, short tvs, short tvel)
{
	// первое обращение -> tvs==1;
	std::string str[4];
	HFONT newFont, oldFont;
	short textSize = stoi(GraphParameters_163Return("TEXT_SIZE")) / 2;
	std::string str1;
	str[0] = std::to_string(tvel);
	str[1] = std::to_string(KK_Return_Values(_e_kk_mcu_current, tvs, tvel));
	str[3] = std::to_string(KK_Return_Values(_e_kk_additional_value, tvs, tvel));
	double vv = KK_Return_Values(_e_kk_additional_value, tvs, tvel);
	str[2] = std::to_string(KK_Return_Values(_e_kk_dev_current, tvs, tvel));

	// Вывод текста
	int tvelStringsNumber = (stoi(parameters["STRINGS_COUNT"]));
	for (short i = 0; i < tvelStringsNumber; i++)
	{
		// Черный цвет текста
		SetTextColor(hdc, 0);
		// Прозрачный фон
		SetBkMode(hdc, TRANSPARENT);
		// Создаем новый шрифт:
		newFont = CreateFont(1.5*textSize, 0, 0, 0, 700, 0, 0, 0,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			PROOF_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Arial"));

		// Переприсваивание
		oldFont = (HFONT)SelectObject(hdc, newFont);

		unsigned short textLenght = stoi(parameters["TEXT_LENGHT"]);
		if (str[i].size() > textLenght)
		{
			for (short j = 0; j < textLenght; j++)
				str1 += str[i][j];
		}
		else
			str1 = str[i];

		TextOut(hdc, (int)(-1 * (1 + ((textSize * 2 - scale) / (scale * scale)))*((str1.size()))),
			(int)((1 + (1 / scale))*text_coords[i] / 3), str1.data(), str1.size());
		SelectObject(hdc, oldFont);
		DeleteObject(newFont);
		str1.clear();
	}


}


COLORREF ColorReference(short tvs, short tvel, int mainValue, int mm)
{
	COLORREF cellColor;
	short colorCount = stoi(GraphParameters_163Return("COLOR_COUNT"));
	bool elem_tvs = stoi(GraphParameters_163Return("PAINT_FR"));
	double min_value = 1000;
	double max_value = -1000;
	double current_value = -1000;
	bool critical_er = stoi(GraphParameters_163Return("CRITICAL_ENERGY_RELEASE"));
	short critical_val = -10;

	if (critical_er == true)  critical_val = stoi(GraphParameters_163Return("CRITICAL_ER_VALUE"));


	switch (mainValue) {
	case _e_kk_permak_current:
		current_value= KK_Return_Values(_e_kk_permak_current, tvs, tvel);
		break;
	case _e_kk_mcu_current:
		current_value = KK_Return_Values(_e_kk_mcu_current, tvs, tvel);
		break;
	case _e_kk_dev_current:
		current_value = KK_Return_Values(_e_kk_dev_current, tvs, tvel);
		break;
	case _e_tvs:
		current_value = (permak_kq[tvs] - mcu_kq[tvs]) / mcu_kq[tvs];
		break;
	default:
		current_value = -9160820148;
	}

	switch (mm)
	{
	case _e_permak_tvs_kk:
		min_value = KK_Return_Values(_e_min_in_tvs_permak, tvs, tvel);
		max_value = KK_Return_Values(_e_max_in_tvs_permak, tvs, tvel);
		break;
	case _e_mcu_tvs_kk:
		min_value = KK_Return_Values(_e_min_in_tvs_mcu, tvs, tvel);
		max_value = KK_Return_Values(_e_max_in_tvs_mcu, tvs, tvel);
		break;
	case _e_permak_core_kk:
		min_value = KK_Return_Values(_e_min_in_core_permak, tvs, tvel);
		max_value = KK_Return_Values(_e_max_in_core_permak, tvs, tvel);
		break;
	case _e_mcu_core_kk:
		min_value = KK_Return_Values(_e_min_in_core_mcu, tvs, tvel);
		max_value = KK_Return_Values(_e_max_in_core_mcu, tvs, tvel);
		break;
	case _e_dev_core:
		min_value = KK_Return_Values(_e_min_dev_core, tvs, tvel);
		max_value = KK_Return_Values(_e_max_dev_core, tvs, tvel);
		break;

	case _e_dev_tvs:
		min_value = KK_Return_Values(_e_min_dev_tvs, tvs, tvel);
		max_value = KK_Return_Values(_e_max_dev_tvs, tvs, tvel);
		break;


	default:
		min_value = -10000000;
		max_value = 10000000;
	}


	// Если сравниваем по kq
	if (mainValue == _e_tvs) {
		min_value = min_kq;
		max_value = max_kq;
	}

		
		
	//////////////////////////////////


		double range = max_value - min_value;
		double step_size = range / (colorCount - 1);

		short group_number = -1;

	unsigned short stColorVal = 120;
	unsigned short endColorVal = 255;

	////// ПРИНИМАЕТСЯ НОРМИРОВАНИЕ НА 0
	short zero_group = 0;

	for (short i = 2; i < colorCount+1; i++)
	{
		double kkz = min_value + i * step_size;
		if (current_value <= (min_value+i*step_size))
		{
			group_number = i - 1;
			break;
		}
	}

	if ((min_value*max_value < 0))
	{
		for (short i = 2; i < colorCount + 1; i++)
		{
			if (0 < (min_value + (i*step_size)))
			{
				zero_group = i - 1;
				break;
			}
		}
	}
	else
	{
		for (short i = 2; i < colorCount + 1; i++)
		{
			if (1 < (min_value + (i*step_size)))
			{
				zero_group = i - 1;
				break;
			}
		}
	}

	unsigned short gr = 0;
	unsigned short bl = 0;
	unsigned short rd = 0;

	if (zero_group < 2) printf("\n\nError in zero group!!!!!!!\nThat may be caused in case of difference in tvels position\n");
	// Верхние границы груп
	// Подразумевается момент пересменки цветов (меняли синий, теперь меняем зеленый)

	// Новый алгоритм

	// как работает:
	// от минимального значения до группы, в которой находится нормируемое значение
	// { 
	// задаем  rd_step_1 = (250/(zero_group-1)) - шаг по красному цвету от нулевой группы до нормируемой
	// где в числителе - конечная величина, которую необходимо получить,
	// в знаменателе величина интервала, на котором варьируем
	// ......
	// потом прибавляем к начальному значению цвета произведение (шага цвета * номер группы)
	// // то есть чем ближе к zero_group, тем "краснее"
	// аналогично для green. 
	// }
	// если номер группы, в которой находится величина, больше, 
	// чем группа, в которой величина, на которую нормируем, то:
	// {
	// задаем аналогично начальные цвета и шаги для синего и зеленого
	// бьем все группы, номер которых больше номера группы с нормировочной величиной, 
	// на два интервала (*): сначала уменьшаем синий, потом уменьшаем зеленый
	// {
	// уменьшаем синий до  группы (*)  до величины 120 - это создает желтые оттенки
	// }
	// {
	// уменьшаем зеленый от группы (*) до конца группы, это создает оражево-красные оттенки
	// }
	// 
	// 
	// 
	int inner_pt = 0;
	short rd_step_1 = 0, gr_step_1 = 0, bl_step_1 = 0, gr_step_2 = 0;
	if (critical_er)
	{
		if (current_value > abs(critical_val))
		{
			rd = 255;
			bl = 50;
			gr = 50;
		}
		else if ((current_value <(0-critical_val)))
		{
			rd = 50;
			bl = 255;
			gr = 50;
		}
		else
		{
			rd = 255;
			gr = 255;
			bl = 255;
		}
	}
	else
	{
		if (group_number < zero_group)
		{
			rd = 0;
			gr = 135;
			bl = 255;
			rd_step_1 = (250 / (zero_group - 1));
			gr_step_1 = (120 / (zero_group - 1));
			inner_pt = 1;
			rd += (rd_step_1*group_number);
			gr += (gr_step_1*group_number);
		}
		else
		{
			rd = 255;
			gr = 255;
			bl = 255;
			if (group_number < (colorCount + zero_group) / 2)
			{
				bl -= (255 / (colorCount - zero_group))*(group_number - zero_group);
				//gr = 255-180 / ((colorCount + zero_group)/2.)*(group_number - zero_group);
			}
			else
			{
				bl = 130 - (130 / (colorCount - zero_group-1))*(group_number - zero_group+1);
				gr = 255 - (255 / (colorCount - zero_group-1))*(group_number - zero_group+1);
			}
		}
	}

	cellColor = RGB(rd, gr, bl);
	return cellColor;
}

void DisplayGapsSize(HDC hdc, short x, short y, short tvs_num)
{
	HFONT newFont, currentFont;
	short GEOM = stoi(GraphParameters_163Return("GEOMETRY"));
	short text_size = (int)((stoi(GraphParameters_163Return("TEXT_SIZE"))));
	short an_m = 60, angle = 150;	
	unsigned short textLenght = 2+stoi(parameters["TEXT_LENGHT"]);
	std::string str,str1;
	double coord_modifier[6][2] = { 
	{-1.4, 0.7, },	//+++
	{-1.1,-1.7,},	// +++
	{0.36,-2.35,  },  
	{1.40,-0.7, },	//+++
	{ 1.1,1.70,  }, // +++
	{ -0.36,2.35, }, 
	};
	double x_m, y_m;

	for (short i = 0; i < GEOM; i++)
	{
		// 1 - извлекаем модифицированную величину зазора
		// 2 - извлекаем неизмененную величину зазора
		// если передаем больше 1, а модификатора нет - он использует 1 внутри программы
		// 3 - особый тип извлекаемых данных (в задаче Тутнова это - (fCore[tvs].gap_size - 1.5)
		str = ObjectExtracting(3, tvs_num, i); 
		
		if (str.size() > textLenght)
		{
			for (short j = 0; j < textLenght; j++)
				str1 += str[j];
		}
		else
			str1 = str;


		x_m = (int)(x * coord_modifier[i][0]);
		y_m = (int)(y * coord_modifier[i][1]);

		angle = angle - an_m;
		newFont = CreateFont(text_size,0,10*angle, 0, 0, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Arial"));
		currentFont = (HFONT)SelectObject(hdc, newFont);

		TextOut(hdc, x_m, y_m, str1.data(), str1.size());
		SelectObject(hdc, currentFont);
		DeleteObject(newFont);
		str1.clear();

	}


}

