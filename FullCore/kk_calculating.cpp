#include "163CommonHeader.h"
#include "kk_calculating.h"

#include <vector>
#include <string>
#include <fstream>
#include <direct.h>

class KK_values
{

public:

	std::vector<short> innerNumeration;
	static const int NUM_FA_ = 163;
	static const int NUM_FR_ = 331;
	double kk_values_p[NUM_FA_+1][NUM_FR_+1];
	double kk_values_m[NUM_FA_ + 1][NUM_FR_ + 1];
	double kk_dev[NUM_FA_ + 1][NUM_FR_ + 1];
	double kk_perturbance_value[NUM_FA_ + 1][NUM_FR_ + 1];
	double additional_deviations[NUM_FA_ + 1][NUM_FR_ + 1];
	double kk_additional_values[NUM_FA_ + 1][NUM_FR_ + 1];
	double tvs_max_dev[NUM_FA_ + 1];
	double tvs_min_dev[NUM_FA_ + 1];
	double max_val=-1000, min_val = 1000;
	double max_val_m[NUM_FA_ + 1], max_val_p[NUM_FA_ + 1];
	double min_val_m[NUM_FA_ + 1], min_val_p[NUM_FA_ + 1];
	double global_max_p=-1000, global_max_m=-1000;
	double global_min_p=1000, global_min_m=1000;
	double core_max_dev, core_min_dev;

	std::vector<double> permak_kq;
	std::vector<double> mcu_kq;

	/////////////////////////////////////////
	double mcu_kr_val[NUM_FA_ + 1][NUM_FR_ + 1];
	double permak_kr_val[NUM_FA_ + 1][NUM_FR_ + 1];
	double perturbance_kr_val[NUM_FA_ + 1][NUM_FR_ + 1];
	double deviations_kr_val[NUM_FA_ + 1][NUM_FR_ + 1];


	void ReadingPermakFile(std::string path);
	void LoadingScheme();
	void LoadingMCU(std::string pathway);
	void DeviationsCalculatings();
	void AdditionalDeviations();
	void GetPerturbatedValues();
	void BarChart();

	void GetKq()
	{
		permak_kq = GetPermakKq();
		mcu_kq = GetMcuKq();
	}

	void GetKR()
	{
		for (int tvs = 1; tvs < NUM_FA_ + 1; tvs++)
		{
			for (int tvel = 1; tvel < NUM_FR_ + 1; tvel++) {
				mcu_kr_val[tvs][tvel] = mcu_kq[tvs - 1] * kk_values_m[tvs][tvel];
				permak_kr_val[tvs][tvel] = permak_kq[tvs - 1] * kk_values_p[tvs][tvel];
			}
		}
	}

	void WriteKr()
	{
		std::string pm = "D:/Vasiliev/PERMAK-A/Local_new/bin/res/crc/";
		std::ofstream p_kr(pm + "permak_kr.txt");
		std::ofstream m_kr(pm + "mcu_kr.txt");
		for (int tvs = 1; tvs < NUM_FA_ + 1; tvs++)
		{
			for (int tvel = 1; tvel < NUM_FR_ + 1; tvel++) {
				p_kr << mcu_kr_val[tvs][tvel] <<std::endl;
				m_kr << permak_kr_val[tvs][tvel] << std::endl;
				if ((tvel >= 218) && (tvel <= 331))
					WriteKrMy_FirstSecondRow(mcu_kr_val[tvs][tvel], permak_kr_val[tvs][tvel], tvs, tvel);
			}
		}
	}

	void WriteKrMy_FirstSecondRow(double _mcu, double _permak, int tvs, int tvel) {
		std::string pm = "D:/Vasiliev/PERMAK-A/Local_new/bin/res/crc/";
		std::ofstream p_kr(pm + "permak_first_second_row_kr.txt",std::ios::app);
		std::ofstream m_kr(pm + "mcu_first_second_row_kr.txt", std::ios::app);
		p_kr << _permak << " ; " << tvs << " ; " << tvel << " ; " << std::endl;
		m_kr << _mcu << " ; " << tvs << " ; " << tvel << " ; " << std::endl;

	}


};

KK_values TVS;

void KK_values::ReadingPermakFile(std::string path)
{
	std::ifstream permFile(path);


	std::string str1, s_Full;
	short tvs = 0;
	short num = 0;
	double tmp;
	int tvel = 0;
	bool loadPermakFile = true;
	// Далее для твэлов
	// Если файл не открылся
	if (permFile.is_open() != 1)
		printf("Error function link here\n");
	// А если открылся
	else
	{
			printf("Start extracting fuel rod.\n");
			// До конца файла считываем построчно
			while (permFile.eof() != 1)
			{
				getline(permFile, str1);
				// Если строка такая, то
				if (str1.find("Kk,") == 0)
				{
					tvs++;
					tvel = 1;
					// Начинаем запись в поток
					// Пока не запишем 331 твэл **
					do
					{
						getline(permFile, str1);
						// Считали строку
						// С 4 по 15 символ в другую строку записали
						short str1Length = str1.length();
						for (int kl = 4; kl < str1Length; kl++)
						{
							s_Full += str1[kl];
						}
						// А теперь новую строку с energy release записываем в tmp как double
						tmp = stod(s_Full);
						// И присваиваем объекту значение
						// Потом очищаем строки, переходим к следующему твэлу
						// Записываем в поток
						kk_values_p[tvs][tvel] = tmp;
						s_Full.clear();
						str1.clear();
						tvel++;
					} while (tvel % (NUM_FR_ + 1) != 0);
					// **Как только записали 331 твэл ->
				}
				// -> Увеличиваем k на единицу (это номер момента времени)
				if (tvel > (NUM_FA_ - 1)*NUM_FR_)
				{
					tvs = 0;
					tvel = 0;
					printf("Time point %i\n", tvs);
				}
			}
	}
	permFile.close();
}

void KK_values::LoadingScheme()
{
	std::ifstream schemeFile("D:/Vasiliev/PERMAK-A/Local_new/bin/data/Nt/permak_inner_numeration.txt");
	std::string str1;

	if (!schemeFile.is_open())
		printf("error opening permak tvs scheme\n");
	else
	{
		while (!schemeFile.eof())
		{
			getline(schemeFile, str1);
			if (!str1.empty())
				innerNumeration.push_back(stoi(str1));
		}
	}
	schemeFile.close();
}

void KK_values::LoadingMCU(std::string pathway)
{
	std::ifstream mcuFile(pathway);
	std::string str1,str1T;
	// все это фуфло можно заменить на loadArray1 и loadArray2 и с ними работать
	// а не так что ты вызываешь файл мцу и проверяешь - "а это мцу?"

	if (mcuFile.is_open())
	{
		std::string str1, s_Full;
		short tvs = 0;
		short num = 0;
		double tmp;
		int tvel = 0;
		while (!mcuFile.eof())
		{
			getline(mcuFile, str1);
			// Если строка такая, то
			if (str1.find("Kk,") == 0)
			{
				tvs++;
				tvel = 1;
				// Начинаем запись в поток
				// Пока не запишем 331 твэл **
				do
				{
					getline(mcuFile, str1);
					// Считали строку
					// С 4 по 15 символ в другую строку записали
					short str1Length = str1.length();
					for (int kl = 4; kl < str1Length; kl++)
					{
						s_Full += str1[kl];
					}
					// А теперь новую строку с energy release записываем в tmp как double
					tmp = stod(s_Full);
					// И присваиваем объекту значение
					// Потом очищаем строки, переходим к следующему твэлу
					// Записываем в поток
					kk_values_m[tvs][tvel] = tmp;
					s_Full.clear();
					str1.clear();
					tvel++;
				} while (tvel % (NUM_FR_ + 1) != 0);
				// **Как только записали 331 твэл ->
			}
			// -> Увеличиваем k на единицу (это номер момента времени)
			if (tvel > (NUM_FA_ - 1)*NUM_FR_)
			{
				tvs = 0;
				tvel = 0;
				printf("Time point %i\n", tvs);
			}
		}
	}
	else printf("error opening mcu kk file");

	mcuFile.close();
}

void KK_values::GetPerturbatedValues()
{
	std::ifstream iFile;
	iFile.open("D:/Vasiliev/PERMAK-A/Local_new/bin/res/crc/Result_Nominal_MCU_Kk.out");
	if (iFile.is_open()) {
		std::string str1, s_Full;
		short tvs = 0;
		short num = 0;
		double tmp;
		int tvel = 0;
		while (!iFile.eof())
		{
			getline(iFile, str1);
			// Если строка такая, то
			if (str1.find("Kk,") == 0)
			{
				tvs++;
				tvel = 1;
				// Начинаем запись в поток
				// Пока не запишем 331 твэл **
				do
				{
					getline(iFile, str1);
					// Считали строку
					// С 4 по 15 символ в другую строку записали
					short str1Length = str1.length();
					for (int kl = 4; kl < str1Length; kl++)
					{
						s_Full += str1[kl];
					}
					// А теперь новую строку с energy release записываем в tmp как double
					tmp = stod(s_Full);
					// И присваиваем объекту значение
					// Потом очищаем строки, переходим к следующему твэлу
					// Записываем в поток
					kk_additional_values[tvs][tvel] = tmp;
					s_Full.clear();
					str1.clear();
					tvel++;
				} while (tvel % (NUM_FR_ + 1) != 0);
				// **Как только записали 331 твэл ->
			}
			// -> Увеличиваем k на единицу (это номер момента времени)
			if (tvel > (NUM_FA_ - 1)*NUM_FR_)
			{
				tvs = 0;
				tvel = 0;
				printf("Time point %i\n", tvs);
			}
		}
	}
	else 
		throw std::invalid_argument("Check - <D:/Vasiliev/PERMAK-A/Local_new/bin/res/crc/Result_<state>_MCU_Kk.out>");

	iFile.close();


	iFile.open("D:/Vasiliev/PERMAK-A/Local_new/bin/res/crc/Result_Perturbation_MCU_Kk.out");
	if (iFile.is_open()) {
		std::string str1, s_Full;
		short tvs = 0;
		short num = 0;
		double tmp;
		int tvel = 0;
		while (!iFile.eof())
		{
			getline(iFile, str1);
			// Если строка такая, то
			if (str1.find("Kk,") == 0)
			{
				tvs++;
				tvel = 1;
				// Начинаем запись в поток
				// Пока не запишем 331 твэл **
				do
				{
					getline(iFile, str1);
					// Считали строку
					// С 4 по 15 символ в другую строку записали
					short str1Length = str1.length();
					for (int kl = 4; kl < str1Length; kl++)
					{
						s_Full += str1[kl];
					}
					// А теперь новую строку с energy release записываем в tmp как double
					tmp = stod(s_Full);
					// И присваиваем объекту значение
					// Потом очищаем строки, переходим к следующему твэлу
					// Записываем в поток
					kk_perturbance_value[tvs][tvel] = tmp;
					s_Full.clear();
					str1.clear();
					tvel++;
				} while (tvel % (NUM_FR_ + 1) != 0);
				// **Как только записали 331 твэл ->
			}
			// -> Увеличиваем k на единицу (это номер момента времени)
			if (tvel > (NUM_FA_ - 1)*NUM_FR_)
			{
				tvs = 0;
				tvel = 0;
				printf("Time point %i\n", tvs);
			}
		}
	}
	else
		throw std::invalid_argument("Check - <D:/Vasiliev/PERMAK-A/Local_new/bin/res/crc/Result_<state>_MCU_Kk.out>");

	iFile.close();
}

void KK_values::DeviationsCalculatings()
{
	double m_val;
	
	for (short tvs = 1; tvs < NUM_FA_ + 1; tvs++)
	{
		short count = 1;
		short tveg_count = 0;
		for (short tvel = 1; tvel < NUM_FR_+1; tvel++)
		{
			m_val = kk_values_m[tvs][tvel];
			kk_dev[tvs][tvel] = 
				(m_val != 0 ? (100 * (kk_values_p[tvs][tvel] - m_val) / m_val) : 0);

			if (kk_dev[tvs][tvel] > max_val) max_val = kk_dev[tvs][tvel];
			if (kk_dev[tvs][tvel] < min_val) min_val = kk_dev[tvs][tvel];

		}
	}
}

void KK_values::AdditionalDeviations()
{
	for (short tvs = 1; tvs < NUM_FA_ + 1; tvs++)
	{
		for (short tvel = 1; tvel < NUM_FR_ + 1; tvel++)
		{
			additional_deviations[tvs][tvel] = (
				(kk_additional_values[tvs][tvel] != 0) ?
				((100 * (kk_perturbance_value[tvs][tvel] - kk_additional_values[tvs][tvel]) / kk_additional_values[tvs][tvel])) : 0);
		}
	}

}

void KK_values::BarChart()
{
	std::ofstream OFile("D:/Vasiliev/PERMAK-A/Local_new/bin/res/crc/BC.txt", std::ios::app);
	for (int tvs = 1; tvs < NUM_FA_ + 1; tvs++)
	{
		for (int tvel = 1; tvel < NUM_FR_ + 1; tvel++)
		{
			double result = additional_deviations[tvs][tvel] - kk_dev[tvs][tvel];
			OFile << result << std::endl;
		}
	}

	OFile.close();
}



void kk_calculating()
{

	_chdir("D:/Vasiliev/PERMAK-A/XIPI_18L_W/");

	system("READSQLITE.exe");

	TVS.LoadingScheme();
	TVS.LoadingMCU("D:/Vasiliev/PERMAK-A/Local_new/bin/res/crc/Kk_MCU.out");
	TVS.ReadingPermakFile("D:/Vasiliev/PERMAK-A/XIPI_18L_W/ROSTOV/B02/K01/erfr");
	TVS.GetPerturbatedValues();

	TVS.DeviationsCalculatings();
	TVS.AdditionalDeviations();
	TVS.GetKq();
	TVS.GetKR();
	TVS.WriteKr();
	TVS.BarChart();

	int z1, z2;
	int x, y;
	short sx = 600;
	short sy = 1000;

	HDC hdc_tvel;
	LONG x1 = 12, x2 = 7, y1 = 0, y2 = 10;
	POINT pt[6] = { { -x1,y1 },{ -x2 ,-y2 },{ x2 ,-y2 },{ x1 ,y1 },{ x2,y2 },{ -x2,y2 } };

	short tt = 0; 
	for (short tvs = 1; tvs < TVS.NUM_FA_ + 1; tvs++)
	{
		InnerTvsColorOrder(tvs);
	}


	int kz = 0;
}


double KK_Return_Values(short max, short tvs, short tvel)
{
	// max в имени фунции можно заменить enum'ом и передавать его в функцию
	switch (max)
	{
	// current block
	case _e_kk_mcu_current:
		return TVS.kk_values_m[tvs][tvel];
	case _e_kk_permak_current:
		return TVS.kk_values_p[tvs][tvel];
	case _e_kk_dev_current:
		return TVS.kk_dev[tvs][tvel];

	//in core permak
	case _e_min_in_core_permak:
		return TVS.global_min_p;
	case _e_max_in_core_permak:
		return TVS.global_max_p;

	// in core tvs
	case _e_min_in_core_mcu:
		return TVS.global_min_m;
	case _e_max_in_core_mcu:
		return TVS.global_max_m;

	// in tvs permak
	case _e_min_in_tvs_permak:
		return TVS.min_val_p[tvs];
	case _e_max_in_tvs_permak:
		return TVS.max_val_p[tvs];

	// in tvs mcu
	case _e_min_in_tvs_mcu:
		return TVS.min_val_m[tvs];
	case _e_max_in_tvs_mcu:
		return TVS.max_val_m[tvs];

	// in core devs
	case _e_max_dev_tvs:
		return TVS.tvs_max_dev[tvs];
	case _e_min_dev_tvs:
		return TVS.tvs_min_dev[tvs];
	
	// in tvs
	case _e_max_dev_core:
		return TVS.core_max_dev;
	case _e_min_dev_core:
		return TVS.core_min_dev;
	case _e_kk_perturbance_value:
		return TVS.kk_perturbance_value[tvs][tvel];
	case _e_kk_additional_value:
		return TVS.additional_deviations[tvs][tvel];
	default:
		return -9160820148;
	}
}

void InnerTvsColorOrder(short tvs)
{
	printf("tvs%i ", tvs);
	double localMax = -1000;
	double localMin = 1000;
	double currentValue = 0;
	double currentValue_m = 0;
	double currentValue_p = 0;
	double localMax_m = -1000;
	double localMax_p = -1000;
	double localMin_m = 1000;
	double localMin_p = 1000;

	for (short tvel = 1; tvel < 332; tvel++)
	{
		currentValue =
			TVS.kk_dev[tvs][tvel];
		currentValue_m = TVS.kk_values_m[tvs][tvel];
		currentValue_p = TVS.kk_values_p[tvs][tvel];



		if (currentValue > localMax)
			localMax = currentValue;
		if (currentValue < localMin)
			localMin = currentValue;

		if (currentValue_m > localMax_m)
			localMax_m = currentValue_m;
		if ((currentValue_m < localMin_m) && (currentValue_m < 0))
			localMin_m = currentValue_m;

		if (currentValue_p > localMax_p)
			localMax_p = currentValue_p;
		if ((currentValue_p < localMin_p) && (currentValue_m < 0))
			localMin_p = currentValue_p;

		if ((currentValue_p > TVS.global_max_p) && (currentValue_p < 100))
			TVS.global_max_p = currentValue_p;
		if ((currentValue_p < TVS.global_min_p) && (currentValue_p > 0))
			TVS.global_min_p = currentValue_p;
		if ((currentValue_m > TVS.global_max_m) && (currentValue_m < 100))
			TVS.global_max_m = currentValue_m;
		if ((currentValue_m < TVS.global_min_m) && (currentValue_m > 0))
			TVS.global_min_m = currentValue_m;

		if (currentValue > TVS.core_max_dev) TVS.core_max_dev = currentValue;
		if (currentValue < TVS.core_min_dev) TVS.core_min_dev = currentValue;
	}

	TVS.tvs_max_dev[tvs] = localMax;
	TVS.tvs_min_dev[tvs] = localMin;

	TVS.max_val_p[tvs] = localMax_p;
	TVS.min_val_p[tvs] = localMin_p;

	TVS.max_val_m[tvs] = localMax_m;
	TVS.min_val_m[tvs] = localMin_m;

}