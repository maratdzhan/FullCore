#include "163CommonHeader.h"
#pragma once

void Core::FileReading()
{
	ReadingParameters();
	ReadingMapn(_fa_count);
	ReadingGaps();
	ReadingMapkas();
	ReadingList();
	ReadingPermpar();
	ReadingConstants();
	ReadingBasement();
	ReadingNewdataParameters();
	//	if (key)
	//		ReadingBunrup();
}

void Core::ReadingParameters()
{
	std::string _path = m_Compilation.GetFileByName("PARAMETERS.LOAD");
	VS parametersStrings =
		file.GetLine(_path);
	if (parametersStrings.empty()) {
		std::cout << ">>>\n" << __FUNCTION__ << ":" << std::endl;
		throw(std::invalid_argument("Empty parameters string\n>>>"));
	}
	for (const auto & item : parametersStrings)
	{
		parametersName _name = file.GetType(GetStringParam(item, 1));
		switch (_name)
		{
		case (tvs_size):
			GetParam(_tvs_size, item, 2u);
			break;
		case (tvs_step):
			GetParam(_tvs_step, item, 2);
			break;
		case (tvel_count):
			GetParam(_tvel_count, item, 2);
			break;
		case (tveg_count):
			GetParam(_tveg_count, item, 2);
			break;
		case (fa_count):
			GetParam(_fa_count, item, 2);
			break;
		case (library):
			_pp_library = GetStringParam(item, 2);
			break;
		case (coord_library):
			_c_library = GetStringParam(item, 2);
			break;
		case (reflector_distance):
			GetParam(reflectorDistance, item, 2);
			break;
		case (nominal_gap_size):
			GetParam(nominalGapSize, item, 2);
			break;
		case (p_workdir):
			p_workdirectory = GetStringParam(item, 2);
			break;
		case (project):
			p_project_name = GetStringParam(item, 2);
			break;
		case (unit):
			GetParam(unit_number, item, 2);
			break;
		case (fuel_cycle):
			GetParam(fuel_cycle_number, item, 2);
			break;
		case (accounted_points):
			GetParam(accounted_points_number, item, 2);
			break;
		default:

			break;
		}
	}
}

void Core::ReadingMapn(const uint16_t _count)
{
	try {
		VS mapnString =
			file.GetLine(m_Compilation.GetFileByName("MAPN.DAT"));
		if (_count != mapnString.size())
		{
			throw (std::out_of_range("Not enought data for mapn"));
		}
		else {
			for (const auto & map : mapnString)
			{
				_mapn.push_back(stoi(map));
			}
		}
	}
	catch (std::exception & mapnExc)
	{
		std::cerr << mapnExc.what() << " " << __FUNCTION__ << std::endl;

	}
}

void Core::ReadingGaps()
{
	try {
		VS gapsString =
			file.GetLine(m_Compilation.GetFileByName("COORDS.PVM"));
		ToUpperFunct(gapsString[0]);
		if (gapsString.size() <= _fa_count) {
			throw (std::out_of_range("Not enought gaps data\n"));
		}
		else
		{
			// Define coordinate system
			if (gapsString[0] == "POLAR")
			{
				_coordinate_system = 3;
			}
			else if (gapsString[0] == "CARTESIAN")
			{
				ToUpperFunct(gapsString[1]);
				if (gapsString[1] == "ABSOLUTE")
				{
					_coordinate_system = 1;
				}
				else if (gapsString[1] == "SHIFT")
				{
					_coordinate_system = 2;
				}
				else
				{
					throw (std::invalid_argument("Unknown gaps data type"));
				}
				gapsString.erase(gapsString.begin());
			}
			else
			{
				throw (std::invalid_argument("Unknown coordinate system. Check <COORDS.PVM>\n"));
			}
			// Get item values
			gapsString.erase(gapsString.begin());
			double f_p = 0, s_p = 0;
			for (const auto & item : gapsString)
			{
				GetParam(f_p, item, 2);
				GetParam(s_p, item, 3);
				first_coodinate.push_back(f_p);
				second_coordinate.push_back(s_p);
			}
		}
	}
	catch (std::exception & gapsExc)
	{
		std::cerr << gapsExc.what() << " " << __FUNCTION__ << std::endl;
	}
}

void Core::ReadingMapkas()
{
	try {
		VS mapkasString =
			file.GetLine(m_Compilation.GetFileByName("MAPKAS.DAT"));
		if (mapkasString.empty())
			throw(std::invalid_argument(">>>\nEmpty mapkas list\n>>>\n"));
		for (const auto & item : mapkasString)
		{
			std::string _value;
			std::vector<int> _currentVector;
			for (const auto & c : item)
			{
				if (char(c) >= 48 && char(c) <= 57)
				{
					_value += c;
				}
				else
				{
					if (!_value.empty() && char(c) != 32)
					{
						_currentVector.push_back(stoi(_value));
						_value.clear();
					}
				}
			}
			mapkas.push_back(_currentVector);
		}
	}
	catch (std::exception & mapkasExc)
	{
		std::cerr << mapkasExc.what() << " " << __FUNCTION__ << std::endl;
	}
}

void Core::ReadingBasement()
{
	VS _l_basement = file.GetLine(m_Compilation.GetFileByName("BASEMENT.NAR"));
	std::vector<bool> nalArray = { 0, 0, 0, 0 };
	// 1 - NAL2, 2 - NAL3, 3 - SINGLE_NAL2, 4 - SINGLE_NAL3
	int elements_count = 0;

	for (auto & _str : _l_basement)
	{
		ToUpperFunct(_str);
		if (_str == "NAL2_PRIMARY") {
			nalArray = { 1,0,0,0 };
			elements_count += 23;
			continue;
		}
		if (_str == "NAL3_PRIMARY")
		{
			nalArray = { 0,1,0,0 };
			elements_count += 17;
			continue;
		}
		if (_str == "SINGLE_REF_2")
		{
			nalArray = { 0,0,1,0 };
			elements_count += 7;
			continue;
		}
		if (_str == "SINGLE_REF_3")
		{
			nalArray = { 0,0,0,1 };
			elements_count += 13;
			continue;
		}

		if (nalArray[0])
			nal2array.push_back(_str);
		if (nalArray[1])
			nal3array.push_back(_str);
		if (nalArray[2])
		{
			_str = GetStringParam(_str, 1);
			nal2_r_array.push_back(_str);
		}
		if (nalArray[3])
		{
			_str = GetStringParam(_str, 1);
			nal3_r_array.push_back(_str);
		}
	}

	if (elements_count != 60) {
		std::string _err = "Error reading basement.par - > elements count: " + std::to_string(elements_count)
			+ "; \nMust be 23 + 17 + 7 + 13\nOrder : \nnal2_prim\nnal3_prim\nsimgle_ref2\nsingle_ref3\n";
		throw(std::invalid_argument(_err));
	}
}

void Core::ReadingList()
{
	try
	{


		std::ifstream ifs(m_Compilation.GetFileByName("LIST.TXT"));
		if (!ifs.is_open())
			throw(std::invalid_argument("File LIST.txt not found\n"));
		std::string tmp;
		uint8_t _case = 0;
		while (!ifs.eof())
		{
			getline(ifs, tmp);
			if (!tmp.empty())
				ListHandle(tmp);
		}
		CraftGapsList();
	}
	catch (std::exception & listExc)
	{
		std::cerr << listExc.what() << " " << __FUNCTION__ << std::endl;
	}
}

void Core::ReadingPermpar()
{
	try {
		permpar =
			file.GetLine(m_Compilation.GetFileByName("PERMPAR_INPUT.DAT"));
		if (permpar.empty())
			throw(std::invalid_argument(">>>\nEmpty permpar_input list\n>>>\n"));
		// Тут аккуратно: мы считали файл, в котором есть замены типа "KASSET"
	}
	catch (std::exception & permparExc)
	{
		std::cerr << permparExc.what() << " " << __FUNCTION__ << std::endl;

	}
}

void Core::ReadingConstants()
{
	try {
		constants =
			file.GetLine(m_Compilation.GetFileByName("CONST.DAT"));
		if (constants.empty())
			throw(std::invalid_argument(">>>\nEmpty constants list\n>>>\n"));
		else
			for (size_t i = 0; i < constants.size(); ++i)
				CHT.Put(constants[i], i + 1);
		// Тут аккуратно: мы считали файл, в котором есть замены типа "KASSET"
	}
	catch (std::exception & constExc)
	{
		std::cerr << constExc.what() << " " << __FUNCTION__ << std::endl;
	}
	//		CHT.PrintAll();
}

void Core::ReadingNewdataParameters()
{
	try {
		newdata_parameters = file.GetLine(m_Compilation.GetFileByName("NEWDATA.DAT"));
		if (newdata_parameters.empty())
			throw (std::invalid_argument(">>>>>>\nEmpy newdata.dat file\n>>>>>\n"));
	}
	catch (std::exception & ndexc)
	{
		std::cerr << ndexc.what() << __FUNCTION__ << std::endl;
	}

}

void Core::ListHandle(const std::string & inputString)
{
	std::string parameter = (GetStringParam(inputString, 1));
	ToUpperFunct(parameter);
	std::string _value = GetStringParam(inputString, 2);
	uint8_t _param = 0;
	if (parameter.find("MAX") != -1)
		_param = 5;
	if (parameter.find("MIN") != -1)
		_param = 4;
	if (parameter.find("STEP") != -1)
		_param = 3;
	if (parameter.find("PLANE") != -1)
		_param = 2;
	if (parameter.find("CORNER") != -1)
		_param = 1;

	GapsListParameters(_param, _value);
}

void Core::GapsListParameters(uint8_t _parameter, std::string & _value)
{
	switch (_parameter)
	{
	case (5):
		maxGapVal = stoi(_value);
		break;
	case(4):
		minGapVal = stoi(_value);
		break;
	case(3):
		stepGapValue = stod(_value);
		break;
	case(2):
		planePredictor = _value;
		break;
	case(1):
		cornerPredictor = _value;
		break;
	default:
		std::cerr << "unknown parameter at " << __FUNCTION__ << "\n";
	}
}

void Core::CraftGapsList()
{
	int multiplier = 1;
	while (stepGapValue*multiplier < 1)
	{
		multiplier *= 10;
	}

	stepGapValue *= multiplier;

	if (maxGapVal > minGapVal)
	{
		auto pr = static_cast<double>(minGapVal) * multiplier;
		double _currentValue = pr - stepGapValue;
		while (_currentValue / multiplier <= maxGapVal)
		{
			_currentValue += stepGapValue;
			_gapSize.push_back(_currentValue / multiplier);
			_gapSizePlaneConstant.push_back(planePredictor + std::to_string((int)(_currentValue)));
			_gapSizeCornerConstant.push_back(cornerPredictor + std::to_string((int)(_currentValue)));
		}
	}
	else
		std::cerr << "Wrong input parameters :" << __FUNCTION__ << std::endl;
}