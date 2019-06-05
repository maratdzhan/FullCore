#pragma once
#include <direct.h>

//	double ReturnCoordinatesTvel(short tvel, bool x)
//	{
//		//static HINSTANCE hLibrary_163;
//		HMODULE hLibrary_331;
//
//		// ��� �������� �������� ������� ���������� � ���������� �������� ������, ��� �� ���.
//
//
//		_chdir(current_library_path);
//
//		hLibrary_331 = LoadLibrary(("FR_Coords"));
//		double(*pFunctionFR) (bool, int);
//
//		if (hLibrary_331)
//		{
//			(FARPROC &)pFunctionFR = GetProcAddress(hLibrary_331, "RetFrCoord");
//			if (pFunctionFR == NULL)
//				std::cerr << "NOT FIND cdefine at Coordinates_definition at ''ReturnCoordinates function\n";
//			else
//			{
//				if (x)
//					return pFunctionFR(true, tvel);
//				else
//					return pFunctionFR(false, tvel);
//			}
//		}
//		else
//		{
//			std::cerr << "ERROR OPENING LIBRARY => COORDINATES_DEFINITION.dll\n";
//		}
//	}
//
//

class Coordinates
{
public:
	Coordinates()
	{
		core_fa_count = 0;
		m_errors = 0;
	}

	void AddLibrary(const std::string & _path_value)
	{
		m_fa_library_path = _path_value;
	}

	void SetSize(const size_t size_of_core)
	{
		core_fa_count = size_of_core;
	}

	void GetTvsCoordinates(double fa_step)
	{
		//static HINSTANCE hLibrary_163;
		HMODULE hLibrary_163;

		// ��� �������� �������� ������� ���������� � ���������� �������� ������, ��� �� ���.
		//_path

		const char *path = m_fa_library_path.c_str();

		int tr = _chdir(path);

		hLibrary_163 = LoadLibrary(("Coordinates_Definition"));
		double(*pFunction) (double, int, bool);
		try {
			if (hLibrary_163)
			{
				(FARPROC &)pFunction = GetProcAddress(hLibrary_163, "cdefine");
				if (pFunction == NULL)
					std::cerr << "NOT FIND cdefine at Coordinates_definition at " << __FUNCTION__ << std::endl;
				else
				{
					double x = 0, y = 0;
					for (int i = 1; i <= static_cast<int>(core_fa_count); ++i)
					{
						x = pFunction(fa_step, i, true);
						y = pFunction(fa_step, i, false);
						m_coordinates.push_back({ x,y });
					}
				}
			}
			else
			{
				throw std::exception("ERROR OPENING LIBRARY = > COORDINATES_DEFINITION.dll\n");
			}
		}
		catch (std::exception & lb_ex)
		{
			SetErrors(1);
			std::cerr << lb_ex.what() << __FUNCTION__ << std::endl;
		}
	}

	std::vector<std::pair<double, double>> V_ReturnCoordinatesTvs() const
	{
		return m_coordinates;
	}

	void NeigArrayInitializing()
	{
		neighbor_array.resize(core_fa_count);
		neighbor_array =
		{ { -1,  8,  9,  2, -1, -1 },
		{ 1,  9, 10,  3, -1, -1 },
		{ 2, 10, 11,  4, -1, -1 },
		{ 3, 11, 12,  5, -1, -1 },
		{ 4, 12, 13,  6, -1, -1 },
		{ 5, 13, 14, -1, -1, -1 },
		{ -1, 16, 17,  8, -1, -1 },
		{ 7, 17, 18,  9,  1, -1 },
		{ 8, 18, 19, 10,  2,  1 },
		{ 9, 19, 20, 11,  3,  2 },
		{ 10, 20, 21, 12,  4,  3 },
		{ 11, 21, 22, 13,  5,  4 },
		{ 12, 22, 23, 14,  6,  5 },
		{ 13, 23, 24, 15, -1,  6 },
		{ 14, 24, 25, -1, -1, -1 },
		{ -1, 26, 27, 17,  7, -1 },
		{ 16, 27, 28, 18,  8,  7 },
		{ 17, 28, 29, 19,  9,  8 },
		{ 18, 29, 30, 20, 10,  9 },
		{ 19, 30, 31, 21, 11, 10 },
		{ 20, 31, 32, 22, 12, 11 },
		{ 21, 32, 33, 23, 13, 12 },
		{ 22, 33, 34, 24, 14, 13 },
		{ 23, 34, 35, 25, 15, 14 },
		{ 24, 35, 36, -1, -1, 15 },
		{ -1, 37, 38, 27, 16, -1 },
		{ 26, 38, 39, 28, 17, 16 },
		{ 27, 39, 40, 29, 18, 17 },
		{ 28, 40, 41, 30, 19, 18 },
		{ 29, 41, 42, 31, 20, 19 },
		{ 30, 42, 43, 32, 21, 20 },
		{ 31, 43, 44, 33, 22, 21 },
		{ 32, 44, 45, 34, 23, 22 },
		{ 33, 45, 46, 35, 24, 23 },
		{ 34, 46, 47, 36, 25, 24 },
		{ 35, 47, 48, -1, -1, 25 },
		{ -1, 49, 50, 38, 26, -1 },
		{ 37, 50, 51, 39, 27, 26 },
		{ 38, 51, 52, 40, 28, 27 },
		{ 39, 52, 53, 41, 29, 28 },
		{ 40, 53, 54, 42, 30, 29 },
		{ 41, 54, 55, 43, 31, 30 },
		{ 42, 55, 56, 44, 32, 31 },
		{ 43, 56, 57, 45, 33, 32 },
		{ 44, 57, 58, 46, 34, 33 },
		{ 45, 58, 59, 47, 35, 34 },
		{ 46, 59, 60, 48, 36, 35 },
		{ 47, 60, 61, -1, -1, 36 },
		{ -1, 62, 63, 50, 37, -1 },
		{ 49, 63, 64, 51, 38, 37 },
		{ 50, 64, 65, 52, 39, 38 },
		{ 51, 65, 66, 53, 40, 39 },
		{ 52, 66, 67, 54, 41, 40 },
		{ 53, 67, 68, 55, 42, 41 },
		{ 54, 68, 69, 56, 43, 42 },
		{ 55, 69, 70, 57, 44, 43 },
		{ 56, 70, 71, 58, 45, 44 },
		{ 57, 71, 72, 59, 46, 45 },
		{ 58, 72, 73, 60, 47, 46 },
		{ 59, 73, 74, 61, 48, 47 },
		{ 60, 74, 75, -1, -1, 48 },
		{ -1, -1, 76, 63, 49, -1 },
		{ 62, 76, 77, 64, 50, 49 },
		{ 63, 77, 78, 65, 51, 50 },
		{ 64, 78, 79, 66, 52, 51 },
		{ 65, 79, 80, 67, 53, 52 },
		{ 66, 80, 81, 68, 54, 53 },
		{ 67, 81, 82, 69, 55, 54 },
		{ 68, 82, 83, 70, 56, 55 },
		{ 69, 83, 84, 71, 57, 56 },
		{ 70, 84, 85, 72, 58, 57 },
		{ 71, 85, 86, 73, 59, 58 },
		{ 72, 86, 87, 74, 60, 59 },
		{ 73, 87, 88, 75, 61, 60 },
		{ 74, 88, -1, -1, -1, 61 },
		{ -1, 89, 90, 77, 63, 62 },
		{ 76, 90, 91, 78, 64, 63 },
		{ 77, 91, 92, 79, 65, 64 },
		{ 78, 92, 93, 80, 66, 65 },
		{ 79, 93, 94, 81, 67, 66 },
		{ 80, 94, 95, 82, 68, 67 },
		{ 81, 95, 96, 83, 69, 68 },
		{ 82, 96, 97, 84, 70, 69 },
		{ 83, 97, 98, 85, 71, 70 },
		{ 84, 98, 99, 86, 72, 71 },
		{ 85, 99,100, 87, 73, 72 },
		{ 86,100,101, 88, 74, 73 },
		{ 87,101,102, -1, 75, 74 },
		{ -1, -1,103, 90, 76, -1 },
		{ 89,103,104, 91, 77, 76 },
		{ 90,104,105, 92, 78, 77 },
		{ 91,105,106, 93, 79, 78 },
		{ 92,106,107, 94, 80, 79 },
		{ 93,107,108, 95, 81, 80 },
		{ 94,108,109, 96, 82, 81 },
		{ 95,109,110, 97, 83, 82 },
		{ 96,110,111, 98, 84, 83 },
		{ 97,111,112, 99, 85, 84 },
		{ 98,112,113,100, 86, 85 },
		{ 99,113,114,101, 87, 86 },
		{ 100,114,115,102, 88, 87 },
		{ 101,115, -1, -1, -1, 88 },
		{ -1, -1,116,104, 90, 89 },
		{ 103,116,117,105, 91, 90 },
		{ 104,117,118,106, 92, 91 },
		{ 105,118,119,107, 93, 92 },
		{ 106,119,120,108, 94, 93 },
		{ 107,120,121,109, 95, 94 },
		{ 108,121,122,110, 96, 95 },
		{ 109,122,123,111, 97, 96 },
		{ 110,123,124,112, 98, 97 },
		{ 111,124,125,113, 99, 98 },
		{ 112,125,126,114,100, 99 },
		{ 113,126,127,115,101,100 },
		{ 114,127, -1, -1,102,101 },
		{ -1, -1,128,117,104,103 },
		{ 116,128,129,118,105,104 },
		{ 117,129,130,119,106,105 },
		{ 118,130,131,120,107,106 },
		{ 119,131,132,121,108,107 },
		{ 120,132,133,122,109,108 },
		{ 121,133,134,123,110,109 },
		{ 122,134,135,124,111,110 },
		{ 123,135,136,125,112,111 },
		{ 124,136,137,126,113,112 },
		{ 125,137,138,127,114,113 },
		{ 126,138, -1, -1,115,114 },
		{ -1, -1,139,129,117,116 },
		{ 128,139,140,130,118,117 },
		{ 129,140,141,131,119,118 },
		{ 130,141,142,132,120,119 },
		{ 131,142,143,133,121,120 },
		{ 132,143,144,134,122,121 },
		{ 133,144,145,135,123,122 },
		{ 134,145,146,136,124,123 },
		{ 135,146,147,137,125,124 },
		{ 136,147,148,138,126,125 },
		{ 137,148, -1, -1,127,126 },
		{ -1, -1,149,140,129,128 },
		{ 139,149,150,141,130,129 },
		{ 140,150,151,142,131,130 },
		{ 141,151,152,143,132,131 },
		{ 142,152,153,144,133,132 },
		{ 143,153,154,145,134,133 },
		{ 144,154,155,146,135,134 },
		{ 145,155,156,147,136,135 },
		{ 146,156,157,148,137,136 },
		{ 147,157, -1, -1,138,137 },
		{ -1, -1, -1,150,140,139 },
		{ 149, -1,158,151,141,140 },
		{ 150,158,159,152,142,141 },
		{ 151,159,160,153,143,142 },
		{ 152,160,161,154,144,143 },
		{ 153,161,162,155,145,144 },
		{ 154,162,163,156,146,145 },
		{ 155,163, -1,157,147,146 },
		{ 156, -1, -1, -1,148,147 },
		{ -1, -1, -1,159,151,150 },
		{ 158, -1, -1,160,152,151 },
		{ 159, -1, -1,161,153,152 },
		{ 160, -1, -1,162,154,153 },
		{ 161, -1, -1,163,155,154 },
		{ 162, -1, -1, -1,156,155 }, };

	}

	std::vector<int> Neig_Array(size_t fa_num)
	{
		
		try {
			return neighbor_array[fa_num];
		}
		catch (std::exception & neig_exc)
		{
			std::cerr << neig_exc.what() << __FUNCTION__ << std::endl;
		}

		std::cerr << __FUNCTION__ << "->";
		throw std::out_of_range("Unknown error: TERMINATED\n");
		return { 0 };
	}

	int GetErrors() const
	{
		return m_errors;
	}

private:

	void SetErrors(int _val)
	{
		m_errors = _val;
	}



	std::string m_fa_library_path;
	size_t core_fa_count;
	std::vector<std::pair<double, double>> m_coordinates;
	std::vector<std::vector<int>> neighbor_array;
	int m_errors;
};