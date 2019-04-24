#pragma once

#define _USE_MATH_DEFINES

 #define _USING_HASH_TABLE 1;

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

#include "Assemblies_Class.h"

#include "HashTables.h"


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
	nominal_gap_size,
	p_workdir,
	project,
	unit,
	fuel_cycle,
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
				return nominal_gap_size;
			if (temporary == "PWORKDIR")
				return p_workdir;
			if (temporary == "PROJECT")
				return project;
			if (temporary == "UNIT")
				return unit;
			if (temporary == "FUEL_CYCLE")
				return fuel_cycle;

			
			throw std::invalid_argument("unknown argument " + _parameterName);
			return unknown;
		}
		catch (std::invalid_argument & ia_p)
		{
			std::cerr << ia_p.what() << " at " << __FUNCTION__ << std::endl;
		}
		return unknown;
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
		// Generating path system;
		InnerStructInitialize();
		// Initializing list for files checking
		FileListInitialize(); 
		// Get parameters from gap.par
		GetParametersList();
		// Extracting tests from test list
		GetTestsName();
		// Check files in i-test folder
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
					<<m_relativeFileList[j].GetTestName()<<" not inizialized. Reason:\nWrong input file's count\n";
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
		fileList.push_back("BASEMENT.NAR");
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
private:
	double _tvs_size;
	double _tvs_step;
	double nominalGapSize;
	std::string _pp_library;
	uint16_t _fa_count;
	uint16_t _tvel_count;
	uint16_t _tveg_count;
	uint16_t _node_count;
	uint8_t _coordinate_system = 0;
	std::vector<double> first_coodinate;
	std::vector<double> second_coordinate;


	std::string p_workdirectory;
	std::string p_project_name;
	uint8_t unit_number;
	uint8_t fuel_cycle_number;

	// Permpar:
	VS permpar;
	VS toPermpar;
	VS constants;
	std::string _c_library;
	std::string permpar_path;
	std::vector<uint16_t> _mapn;
	std::vector<std::vector<int>> mapkas;
	std::set<std::string> nal2;
	std::set<std::string> nal3;
	VS nal2array;
	VS nal3array;
	VS nal2_r_array;
	VS nal3_r_array;
	/////////////////////////////

	std::string fa_library_file;

	std::vector<double> _gapSize;
	std::vector<std::string> _gapSizePlaneConstant;
	std::vector<std::string> _gapSizeCornerConstant;
	int maxGapVal;
	int minGapVal;
	std::string planePredictor, cornerPredictor;
	double stepGapValue;
	double reflectorDistance;
	int geometry=0;
	bool isInitialized;
	bool isModifierAccounted;
	// Classes:
private:
	Calculation m_Compilation;
	Coordinates m_coreCoordinates;
	FileHandler file;
	std::vector<Assembly> _fuelAssemblies;
	ConstantsHashTable CHT;
	/////////////////////////////


public:
	Core(const Calculation& _currentObject)
	{
		_tvs_size = _tvs_step = nominalGapSize = 0;
		_fa_count = _tvel_count = _tveg_count = _node_count = 0;
		_coordinate_system = 0;
		unit_number = fuel_cycle_number = 0;
		maxGapVal = minGapVal = 0;
		stepGapValue = reflectorDistance = 0;
		geometry = 6;

		isInitialized = _currentObject.IsCalculationInitialized();
		isModifierAccounted = true;

		if (_currentObject.IsCalculationInitialized()) {
		try {
			
				// Set inner variables





				//  Begin work cycle
				std::cout << "Handling begin:"
					<< _currentObject.GetTestName() << std::endl;
				m_Compilation = { _currentObject.List() };
				m_Compilation.SetFilesNames(_currentObject.GetFilesNames());
				m_Compilation.SetTestName(_currentObject.GetTestName());
				m_Compilation.SetInitializing();
				FileReading();
				CreatePermparFile();
			}

		
		catch (std::exception & Core_constructor_exception)
		{
			std::cerr << Core_constructor_exception.what();
		}
		}
	}

//// Reading test input files
public:
	void FileReading();
	void ReadingParameters();
	void ReadingMapn(const uint16_t _count);
	void ReadingGaps();
	void ReadingMapkas();
	void ReadingBasement();
	void ReadingList();
	void ReadingPermpar();
	void ReadingConstants();

//// Readed files handling
	void ListHandle(const std::string & inputString);
	void GapsListParameters(uint8_t _parameter, std::string & _value);
	void CraftGapsList();
//// Making permpar file
	void CreatePermparFile();
	void PermparMaking();
	void PermparSP(const std::string & _s);
	int TouchCase(const std::string & _str) const;
	void WriteToPermpar();
	void NalArraysForming();
	void Nal2Generating();
	void Nal3Generating();
	void AssembliesArrayForming();
	void ConstantsForming();
	void LibraryIncluding();
//// Assemblies handler
	void CycleSetPlaneGapsForTvs();
	void CycleSetCornerGapsForTvs();
	void CycleSetNeigsForTvs();
	void LoadingAssemblies();
	std::pair<size_t, double> Rounding(double _gs) const;
	VS CyclingConstantFinding(const VS & _id);
	int FindTheConstant(const std::string & _id) const;
	void SetCornerGapsForTvs(Assembly &tvs);
	void SetGapsForTvs(Assembly &tvs);
//// Making newdata file
	void NewdataMaking();





};


#include "Loading test parameters.h"
#include "Permapar.h"
#include "Assemblies_Calculations.h"
#include "Newdata.h"