#include "163CommonHeader.h"
#pragma once

void Core::FileReading()
{
	ReadingParameters();
	ReadingUnitInfo();
	ReadingMapn(_fa_count);
	ReadingGaps();
	ReadingMapkas();
	ReadingList();
	ReadingPermpar();
	ReadingConstants();
	ReadingBasement();
	ReadingNewdataParameters();
	ReadingTimeParameters();
	ParsingTimeParameters();
//	if (std::stoi(arguments.ExtractParameter("SSM")) == 1)
//		GSLastState(0, false);
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
		case (accounted_points):
			GetParam(accounted_points_number, item, 2);
			break;
		case (msq):
			GetParam(permak_max_states_quantity, item, 2);
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
		default:
			break;
		}
	}
	std::cerr << "3. Files loaded : " << __FUNCTION__ << "\n";
}

void Core::ReadingUnitInfo()
{
	std::ifstream perm(p_workdirectory+"PERM");
	int cntr = 0;
	while (1)
	{
		getline(perm, outFile);
		rtrim(outFile);
		/* new algorithm */
		if (cntr == 0) {
			project_path_properties.push_back(outFile);
		}
		else if (cntr == 1)
		{
			project_path_properties.push_back(outFile);
		//	p_project_name = outFile;
		}
		else if (cntr == 2) {
			if (outFile.size() < 2)
				outFile = "0" + outFile;
			project_path_properties.push_back("B" + outFile);
		//	unit_number = stoi(outFile);
		}
		else if (cntr == 3)
		{
			if (outFile.size() < 2)
				outFile = "0" + outFile;
			project_path_properties.push_back("K" + outFile);
		//	fuel_cycle_number = stoi(outFile);
		}
		/* end of new algorithm */
		if (cntr == 4) break;
		cntr++;
	}
	//	outFile = unitPath + outFile;
	outFile = project_path_properties[0] + project_path_properties[1] + char(92) 
		+ project_path_properties[2] + char(92) + project_path_properties[3] + char(92) + outFile;
	std::cerr << "Grab OUT-File path: " << outFile << std::endl;
	perm.close();
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
	std::cerr << "4. Mapn Readed\n";
}

void Core::ReadingGaps()
{
	try {
		VS gapsString =
			file.GetLine(m_Compilation.GetFileByName("COORDS.PVM"));
		ToUpperFunct(gapsString[0]);

		if (gapsString.size() <= _fa_count) {
			if (gapsString.size() == 1)
			{
				file.ScanDirectoryForFiles(gapsFilesList, gapsString[0]);
			}
			else {
				throw std::out_of_range(">>>Wrong gaps data<<<\n");
			}
		}
		else
		{
			ExtractCoordinates(gapsString);	
		}

		
		if (CT_BLANK)
			GetDebugPVM();
	}
	catch (std::exception & gapsExc)
	{
		system("cls");
		std::cerr << gapsExc.what() << " " << __FUNCTION__ << std::endl;
		system("pause");
	}

	SetStatMode();
	std::cerr << "5. Gaps readed\n";
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
	std::cerr << "6. Mapskas readed\n";
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

	std::cerr << "7. List readed\n";
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
	std::cerr << "8. Permpar readed\n";
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
				CHT.Put(constants[i], static_cast<int>(i + 1));
		// Тут аккуратно: мы считали файл, в котором есть замены типа "KASSET"
	}
	catch (std::exception & constExc)
	{
		std::cerr << constExc.what() << " " << __FUNCTION__ << std::endl;
	}
	std::cerr << "9. Constants created\n";
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
	std::cerr << "10. New data parameters readed\n";
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
		std::cerr << "Wrong input parameters: " << __FUNCTION__ << std::endl;
}

void Core::ReadingTimeParameters()
{
	std::cerr << "Time points: ... ";

	try {
		VS timeStrings = file.GetLine(m_Compilation.GetFileByName("TIME_POINTS.CON"));
		for (const auto& _str : timeStrings)
		{
			std::string _word = GetStringParam(_str, 1);
			ToUpperFunct(_word);
			if (_word.find("TIME_POINTS") != -1)
			{
				GetPoints(_str, m_time_points, 1);
			}
			else if (_word.find("RELOADS") != -1)
			{
				GetPoints(_str, reloads, 1);
			}
			else {
				if (!_word.empty())
					throw("invalid argument " + _word);
			}
		}
		std::cerr << "Ok;\n";
	}
	catch (std::exception& time_failed)
	{
		std::cerr << "Error;\n";
		std::cerr << __FUNCTION__ << " " << time_failed.what() << std::endl;
	}
	std::cerr << "11. Time parameters readed\n";
}

//// New functions not for reading, but for handling

void Core::SetStatMode()
{
	if (gapsFilesList.empty())
		statMode = false;
	else
		statMode = true;
}

bool Core::GetStatMode() const
{
	return statMode;
}

void Core::ExtractCoordinates(VS & gapsArray)
{
	int ipos = 0;
	
	// Define coordinate system
	if (gapsArray[0] == "POLAR")
		_coordinate_system = 3;
	else if (gapsArray[0] == "CARTESIAN")
	{
		ToUpperFunct(gapsArray[1]);
		if (gapsArray[1] == "ABSOLUTE")
			_coordinate_system = 1;
		else if (gapsArray[1] == "SHIFT")
			_coordinate_system = 2;
		else
		{
			throw (std::invalid_argument("Unknown gaps data type"));
		}
		ipos++;
	}
	else
		throw (std::invalid_argument("Unknown coordinate system. Check <COORDS.PVM>\n"));
	double f_p = 0, s_p = 0;

	for (int i = 1 + ipos; i < (int)gapsArray.size(); i++)
	{
		GetParam(f_p, gapsArray[i], 2);
		GetParam(s_p, gapsArray[i], 3);
		first_coordinate.push_back(f_p);
		second_coordinate.push_back(s_p);
	}
	std::cerr << "13. Gaps readed" << __FUNCTION__ << "\n";
}

void Core::Clear()
{


	//// Assemblies
	first_coordinate.clear();
	second_coordinate.clear();
	_fuelAssemblies.clear();
	m_coreCoordinates.Clear();

	//gap_sizes.clear();
	//// Newdata
//	m_time_points.clear();
	m_temp.clear();
	m_gam.clear();
	m_backl.clear();
//	m_bor.clear();
	m_itemp.clear();
	m_igam.clear();
	m_xe_flag.clear();
	m_sm_flag.clear();
	m_dopler_flag.clear();
	m_wud.clear();

	//// Permpar
	toPermpar.clear();
	nal2.clear();
	nal3.clear();
	//nal2array.clear();
	//nal3array.clear();
	_mapkasArray.clear();

	//// Others
 	system("cls");
}

void Core::ParsingTimeParameters()
{
	uint16_t reloaded_element = 0;
	parsed_times.resize(m_time_points.size());

	for (int i = 0; i < (int)m_time_points.size(); i++)
	{
		if (reloaded_element<reloads.size() &&
			m_time_points[i] == reloads[reloaded_element])
		{
			reloaded_element++;
		}
		parsed_times[i] = (reloaded_element-1);
	}
	std::cerr << "13. Time parameters parsed\n";
}

int Core::GetStateFromTime(int _time) const
{
	int lower = 0, upper = (int)m_time_points.size(), mid = 0;
	while (m_time_points[mid]!=_time)
	{
		mid = (lower + upper) / 2;
		if (_time <= m_time_points[mid])
			upper = mid;
		else
			lower = mid;
	}

	return mid;
}

void Core::CreatePermFile()
{
	std::cerr << "PERM file rewritting..\n";
	VS permfile = file.GetLine(p_workdirectory + "PERM");

	if (permfile.size() > 0) {
		permfile[0] = p_workdirectory;
		permfile[1] = p_project_name;
		permfile[2] = std::to_string(unit_number);
		permfile[3] = std::to_string(fuel_cycle_number);

		std::ofstream ofs(p_workdirectory + "PERM");
		for (const auto& str : permfile)
			ofs << str << std::endl;
	}
	else
	{
		std::cerr << "Error rewritting PERM file => " << __FUNCTION__ << "\n";
		system("pause");
		throw(std::invalid_argument("Not found PERM file"));
	}
}
//
//void Core::GSLastState(int inputParam, bool saving)
//{
//	std::string _init_path = m_Compilation.GetFileByName("WUD.CON"); // any file
//	_init_path = _init_path.substr(0, _init_path.find_last_of(92)+1);
//	_init_path = _init_path + "LastState.txt";
//
//	if (saving)
//	{
//		std::ofstream file_obj(_init_path, std::ios_base::out);
//		file_obj << inputParam;
//		file_obj.close();
//	}
//	else
//	{
//		std::string _value;
//		std::ifstream file_obj(_init_path, std::ios_base::in);
//		std::getline(file_obj, _value);
//		try {
//			lastState = stoi(_value);
//		}
//		catch (std::exception & exc)
//		{
//			std::cerr << "ERROR ON PARSING LAST STATE: ?NAN?\n" << exc.what() << std::endl;
//			system("pause");
//		}
//		file_obj.close();
//	}
//}