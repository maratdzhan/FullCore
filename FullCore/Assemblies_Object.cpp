//
//
//const short GEOM = 6;
//const short NUMBER_OF_FA=163;
// const double H_ASS = 236;	// Nominal
//// const double H_ASS = 234.6;
//double R_CORE = 4;
//double c_x[6] = { 1,0.5,-0.5,-1,-0.5,0.5 };
//double s_x[6] = { 0,-0.8660254,-0.8660254,0,0.8660254,0.8660254, };
//// ERF - NOMINAL нужно определять через функцию
//bool NOMINAL = 0;
//HANDLE consoleHandle_AO = 0;
//void ModifierAccounting(short tvs);
//void Nal_Define(std::string const_val, bool pln, short tvs_num, short edge);
//
//
//// TO CLEAR IN THE END OF CYCLE => !EXIST THIS FUNCTION
//std::vector<double> GapList;
//std::vector<int> pConstNumber;
//std::vector<int> cConstNumber;
//std::vector<int> RefpConstNumber;
//std::vector<int> RefcConstNumber;
//std::vector<std::string> nal2;
//std::vector<std::string> nal3;
//std::vector<short> nal3_a, nal3_b, nal3_c, nal3_s;
//std::vector<short> nal2_a, nal2_b, nal2_s;
//
//struct FuelAssembly
//{
//	int neig[GEOM];
//	double gap_size[GEOM];
//	double gap_size_before_modifying[GEOM];
//	double corner_gap_size[GEOM];
//	bool modifier = false;
//	std::string p_const[GEOM];
//	std::string c_const[GEOM];
//	
//	int p_const_number[GEOM];
//	int c_const_number[GEOM];
//
//	// xb определяется один раз при расчете зазоров в номинальном положении - x-basic.
//	// xn - буффер при сдигах зазоров проверяет, может ли иметь место такой сдвиг, принимает предыдущее значение координат
//	// x - текущая координата
//	// dx - величина смещения относительно номинального состояния.
//	double x, xb, xn,dx;
//	double y, yb, yn,dy;
//	double x5;
//	double r; // Вектор смещения центра кассеты
//	int map_t;
//	int map_k;
//
//	std::string main_parameters;
//	int number_of_main_parameters;
//
//	void Clearing();
//	void Shifting(short tvs_num);
//	void CoordsDefinition(short tvs_num);
//	void Modifier(short tvs_num, bool isModifierUsed);
//};
//
//FuelAssembly fCore[NUMBER_OF_FA + 1];
//
//void FuelAssembly::Clearing()
//{
//	GapList.clear();
//	pConstNumber.clear();
//	cConstNumber.clear();
//	nal2.clear();
//	nal3.clear();
//	nal3_a.clear();
//	nal3_b.clear();
//	nal3_c.clear();
//	nal3_s.clear();
//}
//
//void FuelAssembly::Shifting(short tvs_num)
//{
//	fCore[tvs_num].dx = fCore[tvs_num].xb - fCore[tvs_num].x;
//	fCore[tvs_num].dy = fCore[tvs_num].yb - fCore[tvs_num].y;
//}
//
//void FuelAssembly::CoordsDefinition(short tvs_num)
//{
//	fCore[tvs_num].xb = ReturnCoordinatesTvs(tvs_num, true);
//	fCore[tvs_num].yb = ReturnCoordinatesTvs(tvs_num, false);
//}
//
//void FuelAssembly::Modifier(short tvs_num, bool isModifierUsed)
//{
//	if (isModifierUsed)
//		ModifierAccounting(tvs_num);
//}
//
//
//void x5Define()
//{
//	// ERF - Вся функция требует переработки , смотри комментарии
//	double x5values[19] = { 0,1.30,1.60,1.60,2.20,2.40,3.00,3.30,3.30,3.60,3.60,3.60,3.70,4.00,4.00,4.00,4.4,4.40,4.95, };
//	// сопля
//	/*  ниже блок когда-то зачем-то добавляли... см. комментарий к "правильной форме"*/ 
//	std::map <int, int> value;
//	value[110] = 1.3;
//	value[115] = 2.2;
//	value[120] = 3.0;
//	value[13] = 4.0;
//
//	for (short tvs_num = 1; tvs_num < NUMBER_OF_FA + 1; tvs_num++)
//	{
//		short const_number = GetParam(fCore[tvs_num].main_parameters, 2);
//		fCore[tvs_num].x5 = value[const_number];
//	}
//	
//
//	/* ниже правильный вариант 
//	// он не используется потому что fCore[tvs].x5 нужно определдять апирорно - иначе дикие значения среднего обогащения
//	for (short tvs_num = 1; tvs_num < NUMBER_OF_FA+1; tvs_num++)
//	{
//		short const_number = GetParam(fCore[tvs_num].main_parameters, 2);
//		fCore[tvs_num].x5 = x5values[const_number];
//	}
//	*/
//}
//
//void FA_Define()
//{
//	printf("Start define "); 
//	SetConsoleTextAttribute(consoleHandle_AO, 11);
//	printf("FA position\n");
//	SetConsoleTextAttribute(consoleHandle_AO, 7);
//	for (short i = 1; i <NUMBER_OF_FA+1; i++)
//	{
//		fCore[i].CoordsDefinition(i);
//		fCore[i].xn = fCore[i].xb;
//		fCore[i].yn = fCore[i].yb;
//		///////////////////////////////////
//		fCore[i].x = fCore[i].xb;
//		fCore[i].y = fCore[i].yb;
//		///////////////////////////////////
//		for (short j = 0; j < GEOM; j++)
//			fCore[i].neig[j] = Neig_Array(i, j);
//	}
//}
//
//void DefMapType()
//{
//	std::ifstream mapn("D:/Vasiliev_Others/CppTestDir/bin/data/mapn.dat");
//	std::string mapnLine;
//	int counter = 1;
//	int floatToInt;
//
//	printf("Start reading");
//	SetConsoleTextAttribute(consoleHandle_AO, 11);
//	printf(" mapn.dat\n");
//	SetConsoleTextAttribute(consoleHandle_AO, 7);
//	do
//	{
//		getline(mapn, mapnLine);
//		if (!mapnLine.empty())
//		{
//			floatToInt = stoi(mapnLine);
//			fCore[counter].map_t = floatToInt;
//			counter++;
//		}
//		if (counter > NUMBER_OF_FA+1) { printf("check defmaptype() func. counter overloaded."); }
//	} while ((mapn.eof() != 1));
//
//	mapn.close();
//}
//
//void DefKasType()
//{
//	std::ifstream chKas("D:/Vasiliev_Others/CppTestDir/bin/data/ch_kas.dat");
//	std::string s;
//	std::map<int,int> dataMapnMapk;
//
//	printf("Start reading");
//	SetConsoleTextAttribute(consoleHandle_AO, 11);
//	printf(" ch_kas.dat\n");
//	SetConsoleTextAttribute(consoleHandle_AO, 7);
//
//	if (chKas.is_open() != 1)
//	{
//		printf("ERROR OPENING CH_KAS.DAT\n");
//	}
//	//  Начинаем считывать
//	else
//	{
//		do
//		{
//			getline(chKas, s);
//			if (!s.empty())
//			{
//				dataMapnMapk[(GetParam(s, 1))] = (GetParam(s, 2));
//			}
//		} while (chKas.eof() != 1);
//	}
//
//	chKas.close();
//
//	// Как все сделали - записываем в класс
//	for (int i = 1; i < NUMBER_OF_FA+ 1; i++)
//	{
//		fCore[i].map_k = dataMapnMapk[fCore[i].map_t];
//	}
//	
//}
//
//void DefineMainAssemblyParameters()
//{
//	std::vector<std::string> str;
//	std::vector<std::string> first_element;
//	std::vector<short> inner_size;
//	std::string str_s, str_s_tmp, write_it;
//	int s, s1;
//	std::ifstream mapkasF("D:/Vasiliev_Others/CppTestDir/bin/data/mapkas.dat");
//
//
//	printf("Start reading");
//	SetConsoleTextAttribute(consoleHandle_AO, 11); 
//	printf(" mapkas.dat\n");
//	SetConsoleTextAttribute(consoleHandle_AO, 7);
//	// Обработка файла mapkas.dat
//	{
//		if (mapkasF.is_open())
//		{
//			do
//			{
//				getline(mapkasF, str_s);
//				if (!str_s.empty())
//					str.push_back(str_s);
//			} while (!mapkasF.eof());
//		}
//		else
//		{
//			printf("Error opening mapkas.dat");
//		}
//	}
//	mapkasF.close();
//
//	//Обрезка первого символа
//	for (int i = 0; i < str.size(); i++)
//	{
//		int k = 2;
//		str_s = " ";
//		first_element.push_back(GetParamS(str[i], 1));
//		while (!str_s.empty())
//		{
//			str_s.clear();
//			str_s = GetParamS(str[i], k);
//			if (!str_s.empty())
//				str_s_tmp += str_s + ",";
//			else
//				break;
//			k++;
//		}
//		inner_size.push_back(k-2);
//		str[i] = str_s_tmp;
//		str_s_tmp.clear();
//	}
//
//	
//	// Записываем строки в объект класса
//	// надо определить и запомнить первый элемент!!!!!!!!!!!!!
//	// и по нему искать!!!
//	// и когда if -> записывать
//	{
//		for (unsigned short tvs = 1; tvs < NUMBER_OF_FA + 1; tvs++)
//		{
//			for (unsigned short i = 0; i < str.size(); i++)
//			{
//				s1 = stoi(first_element[i]);
//				s = fCore[tvs].map_k;
//
//				if (s == s1)
//				{
//					fCore[tvs].main_parameters = str[i];
//					s = -1; s1 = -2;
//					fCore[tvs].number_of_main_parameters = 99 - (inner_size[i] + 2 * GEOM);
//					break;
//				}
//			}
//		}
//	}
//
//
//	
//}
//
//void Gap_Define(int fa_num)
//{
//	// ERF - R_CORE тут не надо задавать
//	if (NOMINAL) R_CORE = 0;
//	double x1, x2, y1, y2, obs, shifting;
//	x1 = fCore[fa_num].x;
//	y1 = fCore[fa_num].y;
//	fCore[fa_num].dx = (fCore[fa_num].x - fCore[fa_num].xb);
//	fCore[fa_num].dy = (fCore[fa_num].y - fCore[fa_num].yb);
//	fCore[fa_num].r = sqrt(pow(fCore[fa_num].dy, 2) + pow(fCore[fa_num].dx, 2));
//
//
//		for (int j = 0; j < GEOM; j++)
//		{
//			fCore[fa_num].gap_size[j] = 0;
//			if (fCore[fa_num].neig[j] > 0)
//			{
//
//				// Если не отражатель - ищем расстояние между кассетами
//				x2 = fCore[fCore[fa_num].neig[j]].x;
//				y2 = fCore[fCore[fa_num].neig[j]].y;
//
//				// Считаем расстояние между центрами кассет.
//				obs = (int)((x1 - x2)*(x1 - x2));
//				obs += (int)((y1 - y2)*(y1 - y2));
//				obs = ((sqrt(obs)) - H_ASS);
//				// Если расстояние не номинальное, тогда:
//				if ((int)(100*obs) != 0)
//				{
//
//					
//
//					double dx1 = c_x[j]*((x1 - x2)-(H_ASS*c_x[j]));
//					double dy1 = s_x[j]*((y1 - y2)-(H_ASS*s_x[j]));
//
//					// Если есть сдвиг самой кассеты
//					double result = dx1+dy1;
//					fCore[fa_num].gap_size[j] += result;
//				}
//			}
//			else
//			{
//				// Тут определяем величину зазора между кассетой и отражателем.
//				if (((fCore[fa_num].dx != 0) || (fCore[fa_num].dy != 0)))
//				{
//					fCore[fa_num].gap_size[j] = R_CORE + ((fCore[fa_num].dy*s_x[j]) + (fCore[fa_num].dx*c_x[j]));
//				}
//				else
//					fCore[fa_num].gap_size[j] = R_CORE;
//			}
//		}
//
//}
//
//void CornerGapsDefinition(short fa_num)
//{
//	bool needNal = false;
//	double thirdPln = 0;
//	int u = 0;
//	int alterTvs = 0;
//	int prevPln = 0;
//
//
//		// Для всех углов
//		for (short edge = 0; edge < GEOM; edge++)
//		{
//			fCore[fa_num].corner_gap_size[edge] = 0;
//			// 1)
//			// Определяем prevPln
//			prevPln = edge - 1;
//			if (prevPln < 0)
//				prevPln = GEOM - 1;
//
//
//			// 2)
//			// Определяем противостоящую ТВС
//			// в предположении, что угол - начальная точка грани
//			
//			// Не забывай, что у тебя другой алгоритм определения NAL, может есть корреляция?
//
//			short newEdgeVal = edge - ((GEOM / 2) - 1);
//			if (newEdgeVal < 0)
//				newEdgeVal = GEOM + edge - ((GEOM / 2) - 1);
//
//			short thirdTVS = fCore[fa_num].neig[edge];
//			if (thirdTVS > 0)
//			{
//				thirdPln = fCore[thirdTVS].gap_size[newEdgeVal];
//			}
//			else
//				// Вот ТУТ ТОЖЕ КОСЯЧЕЛЛО ОГРОМНОЕ
//				thirdPln = R_CORE;
//
//			// Общий вид результата
//			double test1 = fCore[fa_num].gap_size[edge];
//			double test2 = fCore[fa_num].gap_size[prevPln];
//			fCore[fa_num].corner_gap_size[edge] = (
//				fCore[fa_num].gap_size[edge]
//				+ fCore[fa_num].gap_size[prevPln]
//				+ thirdPln
//				) / 3.;
//			
//
//		}
//}
//
//void Gap_Define_S(bool modifier, bool complex)
//{
//	// Проход <INITIALIZING>
//	fCore[0].modifier = modifier;
//	if (!complex)
//	{
//		printf("Calculating");
//		SetConsoleTextAttribute(consoleHandle_AO, 11);
//		printf(" gaps size\n");
//		SetConsoleTextAttribute(consoleHandle_AO, 7);
//		for (int i = 1; i < NUMBER_OF_FA + 1; i++)
//			Gap_Define(i);
//		printf("Calculating");
//		SetConsoleTextAttribute(consoleHandle_AO, 11);
//		printf(" corners gap size\n");
//		SetConsoleTextAttribute(consoleHandle_AO, 7);
//		for (int i = 1; i < NUMBER_OF_FA + 1; i++)
//			CornerGapsDefinition(i);
//		printf("Gaps");
//		SetConsoleTextAttribute(consoleHandle_AO, 11);
//		printf(" rounding and gaps mapping\n");
//		SetConsoleTextAttribute(consoleHandle_AO, 7);
//		for (int i = 1; i < NUMBER_OF_FA + 1; i++)
//			GapsRedefine(i, false);
//	}
//	else
//	{
//		// Проход <FINAL_STEP>
//		for (int j = 1; j < NUMBER_OF_FA + 1; j++)
//		{
//			Gap_Define(j);
//			fCore[j].Modifier(j, modifier);
//		}
//		for (short j = 1; j < NUMBER_OF_FA + 1; j++)
//			CornerGapsDefinition(j);	// Считает угловые зазоры
//		for (short j = 1; j < NUMBER_OF_FA + 1; j++)
//			GapsRedefine(j, true);		// Определяет константы по имеющимся зазорам
//		for (short tvs = 1; tvs < NUMBER_OF_FA + 1; tvs++)
//		{
//			for (short edge = 0; edge < GEOM; edge++)
//			{
//				Nal_Define(std::to_string(fCore[tvs].p_const_number[edge]), true, tvs, edge);
//				Nal_Define(std::to_string(fCore[tvs].c_const_number[edge]), false, tvs, edge);
//			}
//		}
//	}
//}
//
//std::string Rounding(double gap_size, bool take_plan)
//{
//	std::string st_1;
//	short number = -1;
//	bool finded = false;
//	if (take_plan)
//		st_1 = "P";
//	else
//		st_1 = "C";
//
//	for (unsigned short i = 0; i < GapList.size(); i++)
//	{
//
//		if ((gap_size - 0.0005) < GapList[i])
//		{
//			number = i;
//			finded = true;
//			break;
//		}
//		
//	}
//
//	if (!finded)
//	{
//		if (gap_size < 0) number=0;
//		else (number = GapList.size() - 1);
//	}
//	st_1 += std::to_string((int)(100 * GapList[number]));
//	return (st_1);
//	
//}
//
//bool Nal_Validation(short a, short b, short c)
//{
//	if (c != -1)
//	{
//		if (((a + b + c) / 3) == a) return true;
//
//		for (short i = 0; i < nal3_s.size(); i++){
//			if (nal3_s[i] == (a + b + c)){
//				if ((a == nal3_b[i]) || (a == nal3_c[i]) || (a == nal3_a[i])){
//					if ((b == nal3_a[i]) || (b == nal3_c[i]) || (b == nal3_b[i])){
//						if ((c == nal3_b[i]) || (c == nal3_a[i]) || (c == nal3_c[i]))
//							return true;
//					}
//				}
//			}
//		}
//
//		if ((a <= 0) || (b <= 0) || (c <= 0)) return true;
//
//		if ((a > 1500) || (b > 1500) | (c > 1500)) return true;
//	}
//	else
//	{
//		if (((a + b)/2) == a) return true;
//		for (short i = 0; i < nal2_s.size(); i++){
//			if(nal2_s[i] == (a + b)){
//				if ((a == nal2_a[i]) || a == nal2_b[i]) {
//					if ((b == nal2_a[i]) || (b == nal2_b[i]))
//						return true;
//				}
//			}
//		}
//	}
//	return false;
//}
//
//void Nal_Define(std::string const_val, bool pln, short tvs_num, short edge)
//{
//
//	// Мы гоним по всем FA и проверяем NAL's на то, чтобы они были заданы корректно.
//	// для всех определяем a (текущий угол текущей кассеты) и два смежныъ угла
//	if (pln)
//	{
//		bool exist = false;
//		short current_val = fCore[tvs_num].p_const_number[edge];
//		short alter_tvs = fCore[tvs_num].neig[edge];
//		short alter_edge = edge - (GEOM / 2);
//
//		if (alter_edge < 0) alter_edge = GEOM + alter_edge;
//		short alter_val = 0;
//
//		if (alter_tvs > 0)
//			alter_val = fCore[alter_tvs].p_const_number[alter_edge];
//		else
//		//	alter_val = 420; ** потому что 420 - это соответствие номинальному зазору - то есть 475
//			alter_val = 475; 
//
//		//short thrd = (current_val + alter_val) / 2; ИСПОЛЬЗОВАли когда не было REFP_VAL Обозначим это как **
//		short thrd = ((current_val - pConstNumber[0]) + (alter_val - pConstNumber[0])) / 2;
//		//if (alter_val < 150)  thrd = current_val; ** ниже поиск нужного элемента
//		thrd = RefpConstNumber[0] + thrd;
//		// ** надо вернуть, чтобы правильно записать
//		alter_val = (alter_val == 475 ? 420 : alter_val);
//		// **
//		std::string str1;
//		str1 = std::to_string(alter_val) + "," + std::to_string(current_val) + "," + std::to_string(thrd) + ",";
//
//		for (unsigned short j = 0; j < nal2.size(); j++)
//		{
//			if (nal2[j].find(str1) != -1)
//			{
//				exist = true; break;
//			}
//		}
//		if (!Nal_Validation(current_val, alter_val, -1))
//		{
//			nal2.push_back(str1);
//			nal2_a.push_back(current_val);
//			nal2_b.push_back(alter_val);
//			nal2_s.push_back(current_val+alter_val);
//		}
//	}
//	else
//	{
//		// FIRST COMPONENT
//		// Смиотрим первый угол
//		short a = fCore[tvs_num].c_const_number[edge];
//
//		// SECOND COMPONENT
//		// Смотрим второй угол. Это угол предыдущей грани
//		short fake_edge = edge - 1;
//		if (fake_edge < 0) fake_edge = GEOM - 1;
//		short b = -1000;
//		short rTvs = fCore[tvs_num].neig[fake_edge];
//		short alter_fake_edge = -1000;
//
//		short b_n = 0, c_n = 0;
//
//		if (rTvs > 0)
//		{
//			alter_fake_edge = (fake_edge - ((GEOM / 2)));
//			if (alter_fake_edge < 0) alter_fake_edge = GEOM - abs(alter_fake_edge);
//			b = fCore[rTvs].c_const_number[alter_fake_edge];
//			// **
//			b_n = b - 676;
//		}
//		else {
//			b = 418;
//			//**
//			b_n = 726 - cConstNumber[0];
//		}
//		// THIRD COMPONENT
//		short c = -1000;
//		short alter_edge = -1000;
//		short eTvs = fCore[tvs_num].neig[edge];
//		if (eTvs > 0)
//		{
//		//	alter_edge = (edge - ((GEOM / 2) + 1));
//			alter_edge = (edge - (GEOM / 2) + 1);
//			if (alter_edge < 0) alter_edge = GEOM - (abs(alter_edge) + 1);
//			c = fCore[eTvs].c_const_number[alter_edge];
//			//**
//			c_n = c - 676;
//		}
//		else {
//			c = 418;
//			//*
//			c_n = 726 - cConstNumber[0];
//		}
//
//		if ((eTvs < 0) && (rTvs < 0))
//		{
//			b = 418;
//			c = 418;
//			//**
//			c_n = 726-676;
//			b_n = c_n;
//		}
//		// ОКЕЙ ОПРЕДЕЛИЛИ
//		bool exists = false;
//		int rs = 0;
//		if ((b < 150) || (c < 150)) rs = a;
//		else //**rs = ((a + b + c) / 3);
//			rs = RefcConstNumber[0] + (a - 676 + b_n + c_n) / 3;
//		std::string str1 = std::to_string(b) + "," + std::to_string(a) + "," + std::to_string(c) + "," + std::to_string(rs) + ",";
//		if (!Nal_Validation(a,b,c))
//		{
//			nal3_a.push_back(a);
//			nal3_b.push_back(b);
//			nal3_c.push_back(c);
//			nal3_s.push_back(a + b + c);
//			nal3.push_back(str1);
//		}
//	}
//
//}
//
//int ConstantMapping(double value, bool plane)
//{
//	short min_val = 0;
//	short max_val = GapList.size() - 1;
//
//	short step = (max_val - min_val) / 2;
//
//	while (step > 0)
//	{
//		if (value > GapList[min_val + step])
//			min_val += step;
//		else
//			max_val -= step;
//
//		step = step / 2;
//	}
//
//	// Определяем, что вернуть
//	// нужно для того, чтобы использовать текущую версию куска 'if(NOMINAL)'
//	int return_value = 0;
//	if (plane)
//		return_value = pConstNumber[min_val + 1];
//	else
//		return_value = cConstNumber[min_val + 1];
//
//	if (!NOMINAL)
//	{
//		return return_value;
//	}
//	else
//	{
//		if (plane)
//			return 109;
//		else
//			return 67;
//	}
//}
//
//void GapsRedefine(short fa_num, bool complex)
//{
//
//	// Далее ф-ия округления
//	// Начинаем округлять все полученные величины зазоров
//	for (int edge = 0; edge < GEOM; edge++)
//	{
//
//		fCore[fa_num].p_const[edge] = Rounding(fCore[fa_num].gap_size[edge], 1);
//		fCore[fa_num].c_const[edge] = Rounding(fCore[fa_num].corner_gap_size[edge], 0);
//
//		fCore[fa_num].p_const_number[edge]= ConstantMapping(fCore[fa_num].gap_size[edge], 1);
//		fCore[fa_num].c_const_number[edge] = ConstantMapping(fCore[fa_num].corner_gap_size[edge], 0);
//
//	}
//
//}
//
//void ListDefine()
//{
//	consoleHandle_AO = GetStdHandle(STD_OUTPUT_HANDLE);
//	std::ifstream ListFile("D:/Vasiliev_Others/CppTestDir/bin/data/List.txt");
//	int num = 0;
//	std::string fromGetLine;
//
//	if (!ListFile.is_open())
//	{
//		printf("Error opening file 'LIST.TXT'");
//	}
//	else
//	{
//		do
//		{
//			getline(ListFile, fromGetLine);
//			if (!fromGetLine.empty())
//			{
//				GapList.push_back(GetParamD(fromGetLine, 1));
//				pConstNumber.push_back(GetParam(fromGetLine, 2));
//				cConstNumber.push_back(GetParam(fromGetLine, 3));
//				RefpConstNumber.push_back(GetParam(fromGetLine, 4));
//				RefcConstNumber.push_back(GetParam(fromGetLine, 5));
//			}
//		} while (!ListFile.eof());
//	}
//
//	ListFile.close();
//}
//
//void GapInfoFA(int fa_num)
//{
//	
//	{
//		printf("\nParameters for FA # %i:\n", fa_num);
//		{
//			printf("Edges: \t Neigbors\n");
//			for (int i = 0; i < GEOM; i++)
//			{
//				printf("%i\n", fCore[fa_num].neig[i]);
//			}
//			printf("\n");
//		}
//		{
//			printf("Edges: \t Gaps value\n");
//			for (int i = 0; i < GEOM; i++)
//			{
//				printf("%i\t%f\n", i, fCore[fa_num].gap_size[i]);
//			}
//			printf("\n");
//		}
//		{
//			printf("FA Coordinates  (%f;%f)   ;", fCore[fa_num].x, fCore[fa_num].y);
//			printf("\n");
//		}
//		{
//			printf("Coordinates changes: X = (%f) ; Y = (%f) ;\n", fCore[fa_num].dx, fCore[fa_num].dy);
//			printf("Radius-vector: R = (%f)\n", fCore[fa_num].r);
//		}
//	}
//}
//
//void FA_Movement(short fa_num)
//{
//	bool access=true;
//
//	double dx=0, dy=0;
//	double temp_prev_x=fCore[fa_num].x, temp_prev_y= fCore[fa_num].y;
//	std::string s;
//
//	printf("FA movement selected\n");
//	printf("\tSelected FA\t%i", fa_num);
//
//	// тут регулятор ввод
//	GapInfoFA(fa_num);
//	
//	printf("\n\tSelect dX\t");
//	std::cin >> s;
//	dx=std::stod(ReturnNumbers(s));
//	s.clear();
//	
//	printf("\n\tSelect dY\t");
//	std::cin >> s;
//	dy= std::stod(ReturnNumbers(s));
//	s.clear();
//
//
//
//	if (dx != 0)
//	{
//		temp_prev_x = fCore[fa_num].x;
//		fCore[fa_num].x = fCore[fa_num].x + dx;
//	}
//
//	if (dy != 0)
//	{
//		temp_prev_y = fCore[fa_num].y;
//		fCore[fa_num].y = fCore[fa_num].y + dy;
//	}
//		
//
//	///////////////////////////////////////////////
//	// Проверка, может ли быть такое отклонение
//	// Проводим процесс определения координат
//	Gap_Define(fa_num);
//	for (short i = 0; i < GEOM; i++)
//	{
//		if (fCore[fa_num].gap_size[i]< -2.0)
//		{
//			printf("\n////////*W-A-R-N-I-N-G*////////\nError in FA Position: %i edge %i, value is %f. [TVS = %i]\n",
//				fa_num, i + 1, fCore[fa_num].gap_size[i], fCore[fa_num].neig[i]);
//			access = false;
//			printf("Coordinates will be returned to previous values;\n\n");
//		}
//
//	}
//	// Если не может - возвращаем с предыдущего шага значения, и опять пересчитываем координаты
//	if ((!access)&&(abs(dx) + abs(dy) != 0))
//	{
//		fCore[fa_num].x = temp_prev_x;
//		fCore[fa_num].y = temp_prev_y;
//		Gap_Define(fa_num);
//	}
//	
//	// Если есть смещения по Х или У и нет наезда
//	if (((abs(dx) + abs(dy) != 0))&&(access))
//	{
//		// Делаем предыдущие точки
//		fCore[fa_num].xn = temp_prev_x;
//		fCore[fa_num].yn = temp_prev_y;
//
//		// Пересчитываем зазоры текущей ТВС 
//		Gap_Define(fa_num);
//		CornerGapsDefinition(fa_num);
//		GapsRedefine(fa_num, false);
//		// Пересчитываем зазоры всех соседей
//		for (short j = 0; j < GEOM; j++)
//		{
//			short neighbor = fCore[fa_num].neig[j];
//			if (neighbor > 0)
//				Gap_Define(neighbor);
//		}
//		for (short j = 0; j < GEOM; j++)
//		{
//			short neighbor = fCore[fa_num].neig[j];
//			if (neighbor > 0)
//				CornerGapsDefinition(neighbor);
//		}
//
//	}
//
//	printf("End of movement\tNew x= %f   y= %f .\n", fCore[fa_num].x, fCore[fa_num].y);
//}
//
//void PermparMaking()
//{
//	std::ifstream permpar("D:/Vasiliev_Others/CppTestDir/bin/data/permpar_input.dat");
//	std::ofstream pp_out("D:/Vasiliev/PERMAK-A/XIPI_18L_W/ROSTOV/B02/permpar");
//	
//	std::string s,s_p,s_c;
//
//	if (!permpar.is_open())
//		printf("Error opening PERMPAR_INP.DAT\n");
//	else
//	{
//		do
//		{
//			getline(permpar, s);
//			std::transform(s.begin(), s.end(), s.begin(), ::toupper);
//			if (s.find("KASSET")!=-1)
//			{
//				for (int tvs = 1; tvs < NUMBER_OF_FA + 1; tvs++)
//				{
//					s_c.clear();
//					s_p.clear();
//					for (short i = 0; i < GEOM; i++)
//					{
//						s_c += (std::to_string(fCore[tvs].c_const_number[i])) + ",";
//					}
//					for (short i = 0; i < GEOM; i++)
//					{
//						s_p += (std::to_string(fCore[tvs].p_const_number[i])) + ",";
//					}
//						
//					pp_out << tvs << "," << fCore[tvs].main_parameters << s_p << s_c<<fCore[tvs].number_of_main_parameters<<"*0,\n";
//				}
//				s.clear();
//			}
//			
//			if (s.find("NAL2_DEFINE") != -1)
//			{
//				for (auto i : nal2)
//					pp_out << i << "\n";
//				s.clear();
//			}
//
//			if (s.find("NAL3_DEFINE")!=-1)
//			{
//				for (auto i : nal3)
//					pp_out << i << "\n";
//				s.clear();
//			}
//			
//			pp_out << s << "\n";
//		} while (permpar.eof() != 1);
//	}
//
//	pp_out.close();
//	permpar.close();
//
//	fCore[0].Clearing();
//}
//
//void ModifierAccounting(short tvs_num)
//{
//	double cb = 6.3;
//	double gam = 0.7241;
//	double ro5 = 4;
//
//	for (short edge = 0; edge < GEOM; edge++)
//	{
//		if (fCore[tvs_num].neig[edge] > 0)
//			ro5 = 0.5*(fCore[tvs_num].x5 + (fCore[fCore[tvs_num].neig[edge]].x5));
//		else
//			ro5 = fCore[tvs_num].x5;
//		double result = GetCorrection(cb, gam, ro5, fCore[tvs_num].gap_size[edge]);
//		fCore[tvs_num].gap_size_before_modifying[edge] = fCore[tvs_num].gap_size[edge];
//		fCore[tvs_num].gap_size[edge] = 1.0*result;
//	}
//}
//
//std::string ObjectExtracting(short param, short tvs, short edge)
//{
//	std::string str;
//	if (param == 2 && fCore[0].modifier == false) param = 1;
//	switch (param)
//	{
//	case 1:
//		str = std::to_string(fCore[tvs].gap_size[edge]);
//		break;
//	case 2:
//		str = std::to_string(fCore[tvs].gap_size_before_modifying[edge]);
//		break;
//	case 3:
//		str = std::to_string(fCore[tvs].gap_size_before_modifying[edge] + 2);
//	}
//
//	return str; 
//} 
//
//void AssemblyConfigurationSaving()
//{
//	char sep = ',';
//	std::string str, n_str, e_str, v_str;
//	for (short k = 1; k < NUMBER_OF_FA + 1; k++)
//	{
//		// k - номер твс, p_name - имя параметра, e_val - номер грани (-1, если неприменимо), p_val, sep = ","
//		// neig =1, gap_size=2,corner_gap_size=3,p_const=4,c_const=5,p_const_number=6,c_const_number=7;
//		// x0,x,xb,xn=8,9,10,11
//		// y0,y,yb,yn=12,13,14,15
//		// map_t=16, map_k=17, main_parameters=18, number_of_main_parameter=19;
//		for (short edge = 0; edge < GEOM; edge++)
//		{
//			e_str = std::to_string(edge);
//
//			// neig
//			v_str = std::to_string(fCore[k].neig[edge]);
//			str = std::to_string(k) + sep + "1" + sep + e_str + sep + v_str + sep;
//			ParametersCoreSave(str); str.clear();
//			// gap_size
//	/*		v_str = std::to_string(fCore[k].gap_size[edge]);
//			str = std::to_string(k) + sep + "2" + sep + e_str + sep + v_str + sep;
//			ParametersCoreSave(str); str.clear();
//			// corner_gap_size
//			v_str = std::to_string(fCore[k].corner_gap_size[edge]);
//			str = std::to_string(k) + sep +"2" + sep + e_str + sep + v_str + sep;
//			ParametersCoreSave(str); str.clear();
//	*/		// p_const
//			v_str = (fCore[k].p_const[edge]);
//			str = std::to_string(k) + sep + "4" + sep + e_str + sep + v_str + sep;
//			ParametersCoreSave(str); str.clear();
//			// c_const
//			v_str = fCore[k].c_const[edge];
//			str = std::to_string(k) + sep + "5" + sep + e_str + sep + v_str + sep;
//			ParametersCoreSave(str); str.clear();
//			// p_const_number
//			v_str = std::to_string(fCore[k].p_const_number[edge]);
//			str = std::to_string(k) + sep + "6" + sep + e_str + sep + v_str + sep;
//			ParametersCoreSave(str); str.clear();
//			// c_const_number
//			v_str = std::to_string(fCore[k].c_const_number[edge]);
//			str = std::to_string(k) + sep + "7" + sep + e_str + sep + v_str + sep; 
//			ParametersCoreSave(str); str.clear();
//		}
//		
//
//		// Without edges
//		e_str = "-1";
//
//		v_str = std::to_string(fCore[k].dx);
//		str = std::to_string(k) + sep + "8" + sep + e_str + sep + v_str + sep;
//		ParametersCoreSave(str); str.clear();
//
//		v_str = std::to_string(fCore[k].x);
//		str = std::to_string(k) + sep + "9" + sep + e_str + sep + v_str + sep;
//		ParametersCoreSave(str); str.clear();
//
//		v_str = std::to_string(fCore[k].xb);
//		str = std::to_string(k) + sep + "10" + sep + e_str + sep + v_str + sep;
//		ParametersCoreSave(str); str.clear();
//
//		v_str = std::to_string(fCore[k].xn);
//		str = std::to_string(k) + sep + "11" + sep + e_str + sep + v_str + sep;
//		ParametersCoreSave(str); str.clear();
//
//		v_str = std::to_string(fCore[k].dy);
//		str = std::to_string(k) + sep + "12" + sep + e_str + sep + v_str + sep;
//		ParametersCoreSave(str); str.clear();
//
//		v_str = std::to_string(fCore[k].y);
//		str = std::to_string(k) + sep + "13" + sep + e_str + sep + v_str + sep;
//		ParametersCoreSave(str); str.clear();
//
//		v_str = std::to_string(fCore[k].yb);
//		str = std::to_string(k) + sep + "14" + sep + e_str + sep + v_str + sep;
//		ParametersCoreSave(str); str.clear();
//
//		v_str = std::to_string(fCore[k].yn);
//		str = std::to_string(k) + sep + "15" + sep + e_str + sep + v_str + sep;
//		ParametersCoreSave(str); str.clear();
//
//		v_str = std::to_string(fCore[k].map_t);
//		str = std::to_string(k) + sep + "16" + sep + e_str + sep + v_str + sep;
//		ParametersCoreSave(str); str.clear();
//
//		v_str = std::to_string(fCore[k].map_k);
//		str = std::to_string(k) + sep + "17" + sep + e_str + sep + v_str + sep;
//		ParametersCoreSave(str); str.clear();
//
//		v_str = (fCore[k].main_parameters);
//		str = std::to_string(k) + sep + "18" + sep + e_str + sep + v_str ;
//		ParametersCoreSave(str); str.clear();
//
//		v_str = std::to_string(fCore[k].number_of_main_parameters);
//		str = std::to_string(k) + sep + "19" + sep + e_str + sep + v_str + sep;
//		ParametersCoreSave(str); str.clear();
//
//		v_str = std::to_string(fCore[k].r);
//		str = std::to_string(k) + sep + "20" + sep + e_str + sep + v_str + sep;
//		ParametersCoreSave(str); str.clear();
//	}
//}
//
//void AssemblyConfigurationLoading()
//{
//	std::string str,v_str;
//	short fa_num = 0;
//	short edge_num = -1;
//	short param_num = -1;
//	short ps=stoi(ParametersCoreThrow(true,-1));
//
//	for (int i = 0; i < ps; i++)
//	{
//		str=ParametersCoreThrow(false, i);
//		if (!str.empty())
//		{
//			fa_num = GetParam(str, 1);
//			param_num = GetParam(str, 2);
//			if (param_num < 8)
//				edge_num = GetParam(str, 3);
//			switch (param_num)
//			{
//			case (1):
//				fCore[fa_num].neig[edge_num] = GetParam(str, 4);
//				break;
//			case(2):
//				fCore[fa_num].gap_size[edge_num] = GetParamD(str, 4);
//				break;
//			case(3):
//				fCore[fa_num].corner_gap_size[edge_num] = GetParamD(str, 4);
//				break;
//			case(4):
//				fCore[fa_num].p_const[edge_num] = GetParamS(str, 4);
//				break;
//			case(5):
//				fCore[fa_num].c_const[edge_num] = GetParamS(str, 4);
//				break;
//			case(6):
//				fCore[fa_num].p_const_number[edge_num] = GetParam(str, 4);
//				break;
//			case(7):
//				fCore[fa_num].c_const_number[edge_num] = GetParam(str, 4);
//				break;
//			case(8):
//				fCore[fa_num].dx = GetParamD(str, 4);
//				break;
//			case(9):
//				fCore[fa_num].x = GetParamD(str, 4);
//				break;
//			case(10):
//				fCore[fa_num].xb = GetParamD(str, 4);
//				break;
//			case(11):
//				fCore[fa_num].xn = GetParamD(str, 4);
//				break;
//			case(12):
//				fCore[fa_num].dy = GetParamD(str, 4);
//				break;
//			case(13):
//				fCore[fa_num].y = GetParamD(str, 4);
//				break;
//			case(14):
//				fCore[fa_num].yb = GetParamD(str, 4);
//				break;
//			case(15):
//				fCore[fa_num].yn = GetParamD(str, 4);
//				break;
//			case(16):
//				fCore[fa_num].map_t = GetParam(str, 4);
//				break;
//			case(17):
//				fCore[fa_num].map_k = GetParam(str, 4);
//				break;
//			case(18):
//			{
//				fCore[fa_num].main_parameters.clear();
//				short i_p = 4;
//				while (!GetParamS(str, i_p).empty())
//				{
//					fCore[fa_num].main_parameters += GetParamS(str, i_p) + ",";
//					i_p++;
//					if (GetParamS(str, i_p).empty()) break;
//				}
//				break;
//			}
//			case(19):
//				fCore[fa_num].number_of_main_parameters = GetParam(str, 4);
//				break;
//			case(20):
//				fCore[fa_num].r = GetParamD(str, 4);
//				break;
//			}
//		}
//	}
//	int k = 0;
//}
//
//void CoordsLoading(LoadingDatasType type)
//{
//	std::string str;
//	short k_size = stoi(ParametersCoreThrow(true, -1));
//	short tvs_num=-1;
//	switch (type)
//	{
//	case (absolute):
//	{		double x, y = -9999;
//
//	for (int z = 1; z < k_size; z++)
//	{
//		str = (ParametersCoreThrow(false, z));
//		tvs_num = -1;
//		x, y = -9999;
//		if (!str.empty())
//		{
//			tvs_num = GetParam(str, 1);
//			fCore[tvs_num].CoordsDefinition(tvs_num);
//			x = GetParamD(str, 2);
//			y = GetParamD(str, 3);
//			fCore[tvs_num].x = x;
//			fCore[tvs_num].y = y;
//		}
//	}
//	break;
//	}
//	case (tutnov):
//	{
//		double dx, dy, dr, psi = -9999;
//		for (short z = 1; z < k_size; z++)
//		{
//			str = (ParametersCoreThrow(false, z));
//			if (!str.empty())
//			{
//				tvs_num = GetParam(str, 1);
//				dr = GetParamD(str, 2);
//				psi = GetParamD(str, 3);
//				psi = psi * M_PI/ 180.;
//				dx = dr * cos(psi);
//				dy = dr * sin(psi);
//				fCore[tvs_num].CoordsDefinition(tvs_num);
//				fCore[tvs_num].x = fCore[tvs_num].xb + dx;
//				fCore[tvs_num].y = fCore[tvs_num].yb + dy;
//			}
//		}
//		break;
//	}
//	}
//
//}
