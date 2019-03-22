//#include "163CommonHeader.h"
//
//class KK_values
//{
//
//public:
//
//	std::vector<short> innerNumeration;
//	double kk_values_p[NUM_FA_+1][NUM_FR_+1];
//	double kk_values_m[NUM_FA_ + 1][NUM_FR_ + 1];
//	double kk_dev[NUM_FA_ + 1][NUM_FR_ + 1];
//	double kk_perturbance_value[NUM_FA_ + 1][NUM_FR_ + 1];
//	double additional_deviations[NUM_FA_ + 1][NUM_FR_ + 1];
//
//	std::vector<double> permak_kq;
//	std::vector<double> mcu_kq;
//
//	/////////////////////////////////////////
//	double mcu_kr_val[NUM_FA_ + 1][NUM_FR_ + 1];
//	double permak_kr_val[NUM_FA_ + 1][NUM_FR_ + 1];
//	double perturbance_kr_val[NUM_FA_ + 1][NUM_FR_ + 1];
//	double deviations_kr_val[NUM_FA_ + 1][NUM_FR_ + 1];
//
//
//	void ReadingPermakFile(std::string path);
//	void LoadingScheme();
//	void LoadingMCU(std::string pathway);
//	void DeviationsCalculatings();
//	void GetPerturbatedValues();
//	void BarChart();
//
//	void GetKR()
//	{
//		for (int tvs = 1; tvs < NUM_FA_ + 1; tvs++)
//		{
//			for (int tvel = 1; tvel < NUM_FR_ + 1; tvel++) {
//				mcu_kr_val[tvs][tvel] = mcu_kq[tvs - 1] * kk_values_m[tvs][tvel];
//				permak_kr_val[tvs][tvel] = permak_kq[tvs - 1] * kk_values_p[tvs][tvel];
//			}
//		}
//	}
//
//	void WriteKr()
//	{
//		std::string pm = "D:/Vasiliev/PERMAK-A/Local_new/bin/res/crc/";
//		std::ofstream p_kr(pm + "permak_kr.txt");
//		std::ofstream m_kr(pm + "mcu_kr.txt");
//		for (int tvs = 1; tvs < NUM_FA_ + 1; tvs++)
//		{
//			for (int tvel = 1; tvel < NUM_FR_ + 1; tvel++) {
//				p_kr << permak_kr_val[tvs][tvel] << std::endl;
//				m_kr << mcu_kr_val[tvs][tvel] << std::endl;
//				if ((tvel >= 218) && (tvel <= 331))
//					WriteKrMy_FirstSecondRow(mcu_kr_val[tvs][tvel], permak_kr_val[tvs][tvel], tvs, tvel);
//			}
//		}
//	}
//
//	void WriteKrMy_FirstSecondRow(double _mcu, double _permak, int tvs, int tvel) {
//		std::string pm = "D:/Vasiliev/PERMAK-A/Local_new/bin/res/crc/";
//		std::ofstream p_kr(pm + "permak_first_second_row_kr.txt",std::ios::app);
//		std::ofstream m_kr(pm + "mcu_first_second_row_kr.txt", std::ios::app);
//		p_kr << _permak << " ; " << tvs << " ; " << tvel << " ; " << std::endl;
//		m_kr << _mcu << " ; " << tvs << " ; " << tvel << " ; " << std::endl;
//
//	}
//
//
//};
//
//KK_values TVS;
//
//void KK_values::ReadingPermakFile(std::string path)
//{
//	std::ifstream permFile(path);
//
//
//	std::string str1, s_Full;
//	short tvs = 0;
//	short num = 0;
//	double tmp;
//	int tvel = 0;
//	bool loadPermakFile = true;
//	// Далее для твэлов
//	// Если файл не открылся
//	if (permFile.is_open() != 1)
//		printf("Error function link here\n");
//	// А если открылся
//	else
//	{
//			printf("Start extracting fuel rod.\n");
//			// До конца файла считываем построчно
//			while (permFile.eof() != 1)
//			{
//				getline(permFile, str1);
//				// Если строка такая, то
//				if (str1.find("Kk,") == 0)
//				{
//					tvs++;
//					tvel = 1;
//					// Начинаем запись в поток
//					// Пока не запишем 331 твэл **
//					do
//					{
//						getline(permFile, str1);
//						// Считали строку
//						// С 4 по 15 символ в другую строку записали
//						short str1Length = str1.length();
//						for (int kl = 4; kl < str1Length; kl++)
//						{
//							s_Full += str1[kl];
//						}
//						// А теперь новую строку с energy release записываем в tmp как double
//						tmp = stod(s_Full);
//						// И присваиваем объекту значение
//						// Потом очищаем строки, переходим к следующему твэлу
//						// Записываем в поток
//						kk_values_p[tvs][tvel] = tmp;
//						s_Full.clear();
//						str1.clear();
//						tvel++;
//					} while (tvel % (NUM_FR_ + 1) != 0);
//					// **Как только записали 331 твэл ->
//				}
//				// -> Увеличиваем k на единицу (это номер момента времени)
//				if (tvel > (NUM_FA_ - 1)*NUM_FR_)
//				{
//					tvs = 0;
//					tvel = 0;
//					printf("Time point %i\n", tvs);
//				}
//			}
//	}
//	permFile.close();
//}
//
//void KK_values::LoadingScheme()
//{
//	std::ifstream schemeFile("D:/Vasiliev/PERMAK-A/Local_new/bin/data/Nt/permak_inner_numeration.txt");
//	std::string str1;
//
//	if (!schemeFile.is_open())
//		printf("error opening permak tvs scheme\n");
//	else
//	{
//		while (!schemeFile.eof())
//		{
//			getline(schemeFile, str1);
//			if (!str1.empty())
//				innerNumeration.push_back(stoi(str1));
//		}
//	}
//	schemeFile.close();
//}
//
//void KK_values::LoadingMCU(std::string pathway)
//{
//	std::ifstream mcuFile(pathway);
//	std::string str1,str1T;
//	// все это фуфло можно заменить на loadArray1 и loadArray2 и с ними работать
//	// а не так что ты вызываешь файл мцу и проверяешь - "а это мцу?"
//
//	if (mcuFile.is_open())
//	{
//		std::string str1, s_Full;
//		short tvs = 0;
//		short num = 0;
//		double tmp;
//		int tvel = 0;
//		while (!mcuFile.eof())
//		{
//			getline(mcuFile, str1);
//			// Если строка такая, то
//			if (str1.find("Kk,") == 0)
//			{
//				tvs++;
//				tvel = 1;
//				// Начинаем запись в поток
//				// Пока не запишем 331 твэл **
//				do
//				{
//					getline(mcuFile, str1);
//					// Считали строку
//					// С 4 по 15 символ в другую строку записали
//					short str1Length = str1.length();
//					for (int kl = 4; kl < str1Length; kl++)
//					{
//						s_Full += str1[kl];
//					}
//					// А теперь новую строку с energy release записываем в tmp как double
//					tmp = stod(s_Full);
//					// И присваиваем объекту значение
//					// Потом очищаем строки, переходим к следующему твэлу
//					// Записываем в поток
//					kk_values_m[tvs][tvel] = tmp;
//					s_Full.clear();
//					str1.clear();
//					tvel++;
//				} while (tvel % (NUM_FR_ + 1) != 0);
//				// **Как только записали 331 твэл ->
//			}
//			// -> Увеличиваем k на единицу (это номер момента времени)
//			if (tvel > (NUM_FA_ - 1)*NUM_FR_)
//			{
//				tvs = 0;
//				tvel = 0;
//				printf("Time point %i\n", tvs);
//			}
//		}
//	}
//	else printf("error opening mcu kk file");
//
//	mcuFile.close();
//}
//
//void KK_values::GetPerturbatedValues()
//{
//	std::ifstream iFile;
//	iFile.open("D:/Vasiliev/PERMAK-A/Local_new/bin/res/crc/Result_Nominal_MCU_Kk.out");
//	if (iFile.is_open()) {
//		std::string str1, s_Full;
//		short tvs = 0;
//		short num = 0;
//		double tmp;
//		int tvel = 0;
//		while (!iFile.eof())
//		{
//			getline(iFile, str1);
//			// Если строка такая, то
//			if (str1.find("Kk,") == 0)
//			{
//				tvs++;
//				tvel = 1;
//				// Начинаем запись в поток
//				// Пока не запишем 331 твэл **
//				do
//				{
//					getline(iFile, str1);
//					// Считали строку
//					// С 4 по 15 символ в другую строку записали
//					short str1Length = str1.length();
//					for (int kl = 4; kl < str1Length; kl++)
//					{
//						s_Full += str1[kl];
//					}
//					// А теперь новую строку с energy release записываем в tmp как double
//					tmp = stod(s_Full);
//					// И присваиваем объекту значение
//					// Потом очищаем строки, переходим к следующему твэлу
//					// Записываем в поток
//					kk_additional_values[tvs][tvel] = tmp;
//					s_Full.clear();
//					str1.clear();
//					tvel++;
//				} while (tvel % (NUM_FR_ + 1) != 0);
//				// **Как только записали 331 твэл ->
//			}
//			// -> Увеличиваем k на единицу (это номер момента времени)
//			if (tvel > (NUM_FA_ - 1)*NUM_FR_)
//			{
//				tvs = 0;
//				tvel = 0;
//				printf("Time point %i\n", tvs);
//			}
//		}
//	}
//	else 
//		throw std::invalid_argument("Check - <D:/Vasiliev/PERMAK-A/Local_new/bin/res/crc/Result_<state>_MCU_Kk.out>");
//
//	iFile.close();
//
//
//	iFile.open("D:/Vasiliev/PERMAK-A/Local_new/bin/res/crc/Result_Perturbation_MCU_Kk.out");
//	if (iFile.is_open()) {
//		std::string str1, s_Full;
//		short tvs = 0;
//		short num = 0;
//		double tmp;
//		int tvel = 0;
//		while (!iFile.eof())
//		{
//			getline(iFile, str1);
//			// Если строка такая, то
//			if (str1.find("Kk,") == 0)
//			{
//				tvs++;
//				tvel = 1;
//				// Начинаем запись в поток
//				// Пока не запишем 331 твэл **
//				do
//				{
//					getline(iFile, str1);
//					// Считали строку
//					// С 4 по 15 символ в другую строку записали
//					short str1Length = str1.length();
//					for (int kl = 4; kl < str1Length; kl++)
//					{
//						s_Full += str1[kl];
//					}
//					// А теперь новую строку с energy release записываем в tmp как double
//					tmp = stod(s_Full);
//					// И присваиваем объекту значение
//					// Потом очищаем строки, переходим к следующему твэлу
//					// Записываем в поток
//					kk_perturbance_value[tvs][tvel] = tmp;
//					s_Full.clear();
//					str1.clear();
//					tvel++;
//				} while (tvel % (NUM_FR_ + 1) != 0);
//				// **Как только записали 331 твэл ->
//			}
//			// -> Увеличиваем k на единицу (это номер момента времени)
//			if (tvel > (NUM_FA_ - 1)*NUM_FR_)
//			{
//				tvs = 0;
//				tvel = 0;
//				printf("Time point %i\n", tvs);
//			}
//		}
//	}
//	else
//		throw std::invalid_argument("Check - <D:/Vasiliev/PERMAK-A/Local_new/bin/res/crc/Result_<state>_MCU_Kk.out>");
//
//	iFile.close();
//}
//
//void KK_values::DeviationsCalculatings()
//{
//	double m_val;
//	
//	for (short tvs = 1; tvs < NUM_FA_ + 1; tvs++)
//	{
//		short count = 1;
//		short tveg_count = 0;
//		for (short tvel = 1; tvel < NUM_FR_+1; tvel++)
//		{
//			m_val = kk_values_m[tvs][tvel];
//			kk_dev[tvs][tvel] = 
//				(m_val != 0 ? (100 * (kk_values_p[tvs][tvel] - m_val) / m_val) : 0);
//
//			if (kk_dev[tvs][tvel] > max_val) max_val = kk_dev[tvs][tvel];
//			if (kk_dev[tvs][tvel] < min_val) min_val = kk_dev[tvs][tvel];
//
//		}
//	}
//}
//
//
//
//void KK_values::BarChart()
//{
//	std::ofstream OFile("D:/Vasiliev/PERMAK-A/Local_new/bin/res/crc/BC.txt", std::ios::app);
//	for (int tvs = 1; tvs < NUM_FA_ + 1; tvs++)
//	{
//		for (int tvel = 1; tvel < NUM_FR_ + 1; tvel++)
//		{
//			double result = additional_deviations[tvs][tvel] - kk_dev[tvs][tvel];
//			OFile << result << std::endl;
//		}
//	}
//
//	OFile.close();
//}
//
//
//
//void kk_calculating()
//{
//
//	_chdir("D:/Vasiliev/PERMAK-A/XIPI_18L_W/");
//
//	system("READSQLITE.exe");
//	try {
//		TVS.LoadingScheme();
//		TVS.LoadingMCU("D:/Vasiliev/PERMAK-A/Local_new/bin/res/crc/Kk_MCU.out");
//		TVS.ReadingPermakFile("D:/Vasiliev/PERMAK-A/XIPI_18L_W/ROSTOV/B02/K01/erfr");
//		TVS.GetPerturbatedValues();
//
//		TVS.DeviationsCalculatings();
//		TVS.AdditionalDeviations();
//		TVS.GetKq();
//		TVS.GetKR();
//		TVS.WriteKr();
//		TVS.BarChart();
//	}
//	catch (std::exception & ex) {
//		MessageBox(NULL, ex.what(), "ERROR", MB_OK);
//		exit(1);
//	}
//}
//
