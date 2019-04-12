#pragma once

#define _USE_MATH_DEFINES

#include <math.h>
#include <Windows.h>
#include <vector>
//#include <string>
//#include <iostream>
#include <iomanip>
#include <fstream>
//#include <algorithm>
#include <cassert>
#include <filesystem>
#include <map>
#include <set>



using VS=std::vector<std::string>;

#include "GetParam.h"

#include "Coordinates_Definition.h"

#include "Assemblies_Object.h"


enum parametersName
{
	unknown,
	tvs_size,
	tvel_count,
	tveg_count,
	fa_count,
	tvs_step,
	library,
	coord_library,
	reflector_distance,
	nominal_gap_state,
};

struct FileHandler
{
public:
	std::vector<std::string> GetLine(const std::string & _path)
	{
		std::string currentString;
		std::vector<std::string> vc;
		try {
			std::ifstream iFile(_path);
			if (iFile.is_open())
			{
				while (!iFile.eof())
				{
					getline(iFile, currentString);
					if (!currentString.empty())
					{
						vc.push_back(currentString);
					}
				}
			}
			else {
				throw (std::invalid_argument("Cant find <" + _path + "> \n"));
			}
			iFile.close();
		}
		catch (std::exception & reading_exc)
		{
			std::cerr << reading_exc.what() << std::endl;
		}

		m_handledFiles.push_back(_path);
		return vc;
	}

	parametersName GetType(const std::string & _parameterName)
	{
		try {
			std::string temporary = _parameterName;
			ToUpperFunct(temporary);
			if (temporary == "TVS_SIZE")
				return tvs_size;
			if (temporary == "TVS_STEP")
				return tvs_step;
			if (temporary == "TVEL_COUNT")
				return tvel_count;
			if (temporary == "TVEG_COUNT")
				return tveg_count;
			if (temporary == "FA_COUNT")
				return fa_count;
			if (temporary == "LIBRARY")
				return library;
			if (temporary == "COORD_LIBRARY")
				return coord_library;
			if (temporary == "REFLECTOR_DISTANCE")
				return reflector_distance;
			if (temporary == "NOMINAL_GAP_SIZE")
				return nominal_gap_state;

			throw std::invalid_argument("unknown argument " + _parameterName);
		}
		catch (std::invalid_argument & ia_p)
		{
			std::cerr << ia_p.what() << " at " << __FUNCTION__ << std::endl;
		}
	}

private:
	VS m_handledFiles;
};

struct Calculation
{
	Calculation()
	{
		m_name = "default constructor\n";
		m_isInitialized = false;
	}

	Calculation(const VS &_filesList)
	{
		m_isInitialized = false;
		m_filesList = _filesList;
		
	}

	void SetFilesNames(const VS & _filesList)
	{
		m_filesName = _filesList;
	} 

	void SetInitializing()
	{
		m_isInitialized = true;
		MapDistribution();
	}

	void MapDistribution()
	{ 
		for (auto & item : m_filesList)
		{
			ToUpperFunct(item);
		}
		std::sort(m_filesList.begin(), m_filesList.end());
		std::sort(m_filesName.begin(), m_filesName.end());
		for (size_t i = 0; i < m_filesName.size(); ++i)
		{

			m_filesListMap[m_filesName[i]] = m_filesList[i];
		}

	}

	std::map<std::string, std::string> GetFilesSet() const
	{
		return m_filesListMap;
	}

	std::string GetFileByName(const std::string & name) const
	{
		try {
			return m_filesListMap.at(name);
		}
		catch (std::exception & wrong_file_id) {
			std::cerr << "Wrong file ID at "
				<< __FUNCTION__ << " name <" << name << ">\n"
				<< wrong_file_id.what() << std::endl;
		}
		return "ERROR";
	}

	VS List() const
	{
		return m_filesList;
	}

	void SetTestName(const std::string & _name)
	{
		m_name = _name;
	}

	std::string GetTestName() const
	{
		return m_name;
	}

	VS GetFilesNames() const
	{
		return m_filesName;
	}

	bool IsCalculationInitialized() const
	{
		return m_isInitialized;
	}

private:
	bool m_isInitialized;
	VS m_filesList;
	VS m_filesName;
	std::string m_name;
	std::map<std::string, std::string> m_filesListMap;
};


class CommonParametersHandler
{
public:
	CommonParametersHandler() {

		mainParameters = "gap.par";
		InnerStructInitialize();
		FileListInitialize(); // Initializing list for files checking
		GetParametersList();
		GetTestsName();
		FilesListCheck();

	}

	
	void FilesListCheck() 
	{
		
		for (size_t j=0; j<m_relativeFileList.size(); ++j)
		{
			size_t counter = 0;
			VS item = m_relativeFileList[j].List();
			for (auto file : item)
			{
				ToUpperFunct(file);
				for (size_t i = 0; i < fileList.size(); ++i)
				{
					if (file.find(fileList[i]) != -1)
					{
						counter++;
						break;
					}
				}
			}
			if (counter != fileList.size()-1)
			{
				std::cerr << "calculation cant be prepared: "
					<<m_relativeFileList[j].GetTestName()<<" not inizialized\n";
			}
			else
				m_relativeFileList[j].SetInitializing();
		}

	}

	void FileListInitialize()
	{
		fileList.push_back("COORDS.PVM");
		fileList.push_back("MAPN.DAT");
		fileList.push_back("MAPKAS.DAT");
		fileList.push_back("LIST.TXT");
		fileList.push_back("PARAMETERS.LOAD");
		fileList.push_back("PERMAPAR_INPUT.DAT");
		fileList.push_back("CONST.DAT");
	}

	std::vector<Calculation> FilesList() const
	{
		// We have list with 7 files.
		return m_relativeFileList;
		
	}


private:
	void GetParametersList()
	{
		std::string _key, _value;
		std::vector<std::string> parameters;
		parameters = fileReader.GetLine(mainParameters);
		for (auto t : parameters)
		{
			_key = GetStringParam(t, 1);
			ToUpperFunct(_key);
			_value = GetStringParam(t, 2);
		//	ToUpperFunct(_value);
		//	PathPreparing(_value);
			parametersList[_key] = _value;
		}
	}

	void GetTestsName()
	{
		std::string catcher;
		testName = fileReader.GetLine(parametersList.at("TESTS_LIST"));
		try {
			for (auto & item : testName) {
				catcher = item;
				ToUpperFunct(item);
				GetFilesList(parametersList.at("WORK_DIRECTORY") + char(92) + innerStruct_Cr + char(92) + item + char(92));
				m_relativeFileList[m_relativeFileList.size() - 1].SetTestName(item);
			}
		}
		catch (std::exception & exc)
		{
			std::cerr << catcher << " " << exc.what()
				<< " " << __FUNCTION__ << std::endl;
		}

	}


	void GetFilesList(const std::string & folder)
	{
		VS m_absoluteFileList;
		//std::string name = 
		for (const auto & entry : std::filesystem::recursive_directory_iterator(folder)) {
			std::string entire = entry.path().string();

			m_absoluteFileList.push_back(entry.path().string());

			//// Must run reversely (back order)
				//for (unsigned int i = 0; i < folder.size(); i++) {
				//	entire.erase(entire.begin());
				//}
			

		}
		m_relativeFileList.push_back(m_absoluteFileList);
		m_relativeFileList[m_relativeFileList.size() - 1].SetFilesNames(fileList);
	}

	void InnerStructInitialize()
	{
		innerStruct_bin = "bin";
		innerStruct_data = innerStruct_bin + char(92) + "data";
		innerStruct_Cr = innerStruct_data + char(92) + "Cr";
		innerStruct_res = innerStruct_bin + char(92) + "res";
	}

private:

	FileHandler fileReader;
	std::string innerStruct_bin;
	std::string innerStruct_data;
	std::string innerStruct_Cr;
	std::string innerStruct_res;

	std::string mainParameters = "gap.par";
	VS temp;
	VS testName;
	VS fileList;
	std::map<std::string, std::string> parametersList;
	std::vector<Calculation> m_relativeFileList;
};



class Core
{
public:
Core(const Calculation & _currentObject)
{
	try {
		if (_currentObject.IsCalculationInitialized()) {
			isModifierAccounted = true;
			maxGapVal = 0;
			minGapVal = 0;
			stepGapValue = reflectorDistance = 0;
			geometry = 6;
			isInitialized = _currentObject.IsCalculationInitialized();
			_fa_count = _tvel_count = _tveg_count = _node_count = 0;
			_tvs_size = _tvs_step = _coordinate_system = 0;
			nominalGapSize = 0;
			std::cout << "Handling begin:"
				<< _currentObject.GetTestName() << std::endl;
			m_Compilation = { _currentObject.List() };
			m_Compilation.SetFilesNames(_currentObject.GetFilesNames());
			m_Compilation.SetTestName(_currentObject.GetTestName());
			m_Compilation.SetInitializing();
			FileReading();
		}

	}
	catch (std::exception & Core_constructor_exception)
	{
		std::cerr << Core_constructor_exception.what();
	}
}

void LoadingAssembly()
{
	if (isInitialized) {
			_fuelAssemblies.resize(_fa_count);
			double x = 0;
			double y = 0;
			fa_library_file = _c_library + "Coordinates_Definition.dll";
			m_coreCoordinates.AddLibrary(fa_library_file);
			m_coreCoordinates.SetSize(_fa_count);
			m_coreCoordinates.GetTvsCoordinates(_tvs_step);
			m_coreCoordinates.NeigArrayInitializing();
			std::vector<std::pair<double, double>> coordinates = m_coreCoordinates.V_ReturnCoordinatesTvs();
			// Ключ для данных тутнова;

			for (size_t i = 0; i < _fa_count; ++i)
			{
				x = coordinates[i].first;
				y = coordinates[i].second;
				_fuelAssemblies[i].Initialize(_tvs_size, _tvs_step, i);
				_fuelAssemblies[i].SetProjectCoordinates(x, y);
				// here we must send coordinates and type of shift
				_fuelAssemblies[i].SetCurrentCoordinates(first_coodinate[i], second_coordinate[i]);
			}

			std::vector<int> neigs(geometry);

			for (auto & assembly : _fuelAssemblies)
			{
				assembly.SetNeigs(m_coreCoordinates.Neig_Array(assembly.GetNumber()));
				//придумай как реализовать
				SetGapsForTvs(assembly);
				//assembly.SetEffectiveGaps();
			}
			
			for (auto &assembly : _fuelAssemblies)
			{
				SetCornerGapsForTvs(assembly);
			}
	}
}

~Core()
{

}

void SetCornerGapsForTvs(Assembly &tvs)
{
	double _result = 0;
std::cerr << tvs.GetTvsNumber()+1 << ". ";
	for (size_t side = 0; side < geometry; side++)
	{
		_result = -999;
		double _first = tvs.GetGapSize(side);
		double _second = tvs.GetGapSize((side - 1 + 6) % geometry);
		double _third = -999;
		if (tvs.GetNeig(side) != -1)
		{
			_third = _fuelAssemblies[tvs.GetNeig(side)-1].GetGapSize((side + 1 + geometry/2) % geometry);
		}
		else
			_third = reflectorDistance-nominalGapSize;

		_result = (_first + _second + _third) / 3.;
		std::pair < size_t, double > t = Rounding(_result);
		tvs.SetCornerConstants(side, _gapSizeCornerConstant[t.first]);
		tvs.SetCornerGapSize(side, t.second);
		std::cerr << _gapSizeCornerConstant[t.first] << " ";
	}
	std::cerr << std::endl;
}

void SetGapsForTvs(Assembly &tvs)
{
	double _x = 0;
	double _y = 0;
	int _neig = -1;

	double _gapSize = -999;

std::cerr << tvs.GetTvsNumber() << ". ";

	for (auto side = 0; side < geometry; side++)
	{
		try {
			_gapSize = -999;
			// Х: Текущее Х центра кассета - (определяем соседа (GetNeig()) и для него получаем текущий Х)
			_neig = tvs.GetNeig(side);
			if (_neig > 0) {
				_neig -= 1;
				_x = tvs.GetCurrentX() - _fuelAssemblies[_neig].GetCurrentX();
				_y = tvs.GetCurrentY() - _fuelAssemblies[_neig].GetCurrentY();
				_gapSize = sqrt(_x*_x + _y * _y) - _tvs_step;
			}
			else
			{
				double angle = M_PI * ((180 - side * 60) % 360) / 180.;
				double debug_1 = tvs.GetShiftX()*cos(angle);
				debug_1 = tvs.GetShiftY()*sin(angle);
				_gapSize = reflectorDistance - tvs.GetShiftX()*cos(angle) - tvs.GetShiftY()*sin(angle) - nominalGapSize;
			}

			//		if (isModifierAccounted)
			//			SetCorrection(cb, gam, ro5, gs);

			std::pair < size_t, double > t = Rounding(_gapSize);
			tvs.SetGapSize(side, t.second);
			tvs.SetPlaneConstants(side, _gapSizePlaneConstant[t.first]);
			nal2.insert(_gapSizePlaneConstant[t.first]);

std::cerr << _gapSizePlaneConstant[t.first] << " ";

		}
		catch (std::exception & e_c)
		{
			std::cerr << e_c.what() << " at " << __FUNCTION__ << std::endl;
		}
	}           
std::cerr << std::endl;
}

private:
	void FileReading()
	{
		ReadingParameters();
		ReadingMapn(_fa_count);
		ReadingGaps();
		ReadingMapkas();
		ReadingList();
		ReadingPermpar();
		ReadingConstants();

		//	if (key)
		//		ReadingBunrup();
	}

	std::pair<size_t, double> Rounding(double _gs)
	{
		size_t i = 0;
		size_t j = _gapSize.size();
		size_t k = 0;
		int itt = 0;

		while (true)
		{
			k = (j + i) / 2;
			if (_gs >= _gapSize[k])
			{
				i = k;
			}
			else
			{
				j = k;
			}
			if (itt > 1000)
				throw (std::out_of_range("Can't find mathc in ROUNDING()\n"));
			if ((j - i) == 1)
				break;
		}
		return { j-1,_gapSize[j-1] };

	}

public:
	void ReadingParameters()
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
				_c_library=GetStringParam(item, 2);
				break;
			case (reflector_distance):
				GetParam(reflectorDistance, item, 2);
				break;
			case (nominal_gap_state):
				GetParam(nominalGapSize, item, 2);
				break;
			default:

				break;
			}
		}
	}

	void ReadingMapn(const uint16_t _count)
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

	void ReadingGaps()
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

	void ReadingMapkas()
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

	void ReadingList()
	{
		try
		{
			//VS listString =
			//	file.GetLine(m_Compilation.GetFileByName("LIST.TXT"));
			//if (listString.empty())
			//	throw (std::invalid_argument("Empty list file"));

			//_gapSize.resize(listString.size());
			//_gapSizePlaneConstant.resize(listString.size());
			//_gapSizeCornerConstant.resize(listString.size());
			//for (size_t i=0; i< listString.size();++i)
			//{
			//	float _size = 0;
			//	GetParam(_size, listString[i], 1);
			//	// В этой строке ошибка
			//	std::string nameSize = std::to_string((int)(_size * 100));
			//	_gapSize[i]= _size;
			//	_gapSizePlaneConstant[i]=("P" + nameSize);
			//	_gapSizeCornerConstant[i]=("C" + nameSize);
			//}
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

	void ListHandle(const std::string & inputString)
	{
		std::string parameter =(GetStringParam(inputString, 1));
		ToUpperFunct(parameter);
		std::string _value = GetStringParam(inputString, 2);
		uint8_t _param = 0;
		if (parameter.find("MAX")!=-1)
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


	void GapsListParameters(uint8_t _parameter, std::string & _value)
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

	void CraftGapsList()
	{
		int multiplier = 1;
		while (stepGapValue*multiplier < 1)
		{
			multiplier *= 10;
		}

		stepGapValue *= multiplier;

		if (maxGapVal > minGapVal)
		{
			double _currentValue = minGapVal*multiplier-stepGapValue;
			while (_currentValue/multiplier <= maxGapVal)
			{
				_currentValue += stepGapValue;
				_gapSize.push_back(_currentValue/ multiplier);
				_gapSizePlaneConstant.push_back(planePredictor + std::to_string((int)(_currentValue)));
				_gapSizeCornerConstant.push_back(cornerPredictor + std::to_string((int)(_currentValue)));
			}
		}
		else
			std::cerr << "Wrong input parameters :" << __FUNCTION__ << std::endl;
	}



	void ReadingPermpar()
	{
		try {
			permpar=
				file.GetLine(m_Compilation.GetFileByName("PERMAPAR_INPUT.DAT"));
			if (permpar.empty())
				throw(std::invalid_argument(">>>\nEmpty permpar_input list\n>>>\n"));
			// Тут аккуратно: мы считали файл, в котором есть замены типа "KASSET"
		}
		catch (std::exception & permparExc)
		{
			std::cerr << permparExc.what() << " " << __FUNCTION__ << std::endl;

		}
	}

	void ReadingConstants()
	{
		try {
			constants =
				file.GetLine(m_Compilation.GetFileByName("CONST.DAT"));
			if (constants.empty())
				throw(std::invalid_argument(">>>\nEmpty constants list\n>>>\n"));
			// Тут аккуратно: мы считали файл, в котором есть замены типа "KASSET"
		}
		catch (std::exception & constExc)
		{
			std::cerr << constExc.what() << " " << __FUNCTION__ << std::endl;

		}
	}



private:
		double _tvs_size;
		double _tvs_step;
		double nominalGapSize;
		std::string _pp_library;
		uint16_t _fa_count;
		uint16_t _tvel_count;
		uint16_t _tveg_count;
		uint16_t _node_count;
		std::vector<uint16_t> _mapn;
		uint8_t _coordinate_system;
		std::vector<double> first_coodinate;
		std::vector<double> second_coordinate;
		std::vector<std::vector<int>> mapkas;
		VS permpar;
		VS constants;
		std::vector<double> _gapSize;
		std::vector<std::string> _gapSizePlaneConstant;
		std::vector<std::string> _gapSizeCornerConstant;
		int maxGapVal;
		int minGapVal;
		std::string planePredictor, cornerPredictor;
		double stepGapValue;
		std::string _c_library;
		std::string fa_library_file;
		int geometry;
		double reflectorDistance;
		bool isInitialized;
		bool isModifierAccounted;
// Classes:
private:
	Calculation m_Compilation;
	Coordinates m_coreCoordinates;
	FileHandler file;
	std::vector<Assembly> _fuelAssemblies;
	std::set<std::string> nal2;

};