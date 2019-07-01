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
using SS=std::set<std::string>;

#include "GetParam.h"

#include "Coordinates_Definition.h"

#include "Assemblies_Class.h"

#include "HashTables.h"

#include "File handler files.h"



class Core
{
private:
	int noErrors;
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
	uint8_t accounted_points_number;

	std::string p_workdirectory;
	std::string p_project_name;
	uint8_t unit_number;
	uint8_t fuel_cycle_number;
	// Newdata:
private:
	std::vector<int16_t> m_time_points,m_xe_flag, m_sm_flag, m_dopler_flag;;
	int m_states_number;
	int m_nbippar;
	int m_symmetry;
	int m_ireg;
	std::vector<std::vector<double>> m_temp, m_gam, m_backl;
	std::vector<double> m_itemp, m_igam, m_wud, m_bor;
	VS newdata_parameters;
	std::string newdataPath;
	// Permpar:
private:
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
	int number_shifting; // сдвиг номера в связи с недоступностью номеров 1000-1005, 3000-3005, 5000-5005
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
	int geometry;
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
		m_states_number=0;
		noErrors = 0;
		m_nbippar = m_symmetry = 0, number_shifting = 0;
		m_ireg = 0;
		maxGapVal = minGapVal = 0;
		stepGapValue = reflectorDistance = 0;
		geometry = 6;
		accounted_points_number = 0;
		isInitialized = _currentObject.IsCalculationInitialized();
		isModifierAccounted = true;

		if (_currentObject.IsCalculationInitialized()) {
		try {

				//  Begin work cycle
				std::cout << "===============\nHandling begin:"
					<< _currentObject.GetTestName() << std::endl;
				m_Compilation = { _currentObject.List() };
				m_Compilation.SetFilesNames(_currentObject.GetFilesNames());
				m_Compilation.SetTestName(_currentObject.GetTestName());
				m_Compilation.SetInitializing();
				m_Compilation.CopyPathsMap(_currentObject.PathsMap());
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
	void ReadingNewdataParameters();
	template<typename K>
	void _WriteToStream(std::ostream& ofs, const K& inputCollection, unsigned int _init_pos, unsigned int _end_pos);
	template<typename K>
	void _Comp_WriteToStream(std::ostream& ofs, const K& inputCollection, unsigned int _init_pos, unsigned int _end_pos);
	template<typename K>
	bool _GetParametersFromFile(K& inputCollection, const std::string & file);
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
	int DefinePermparNumber(int number, size_t time_point);
//// Assemblies handler
	void CycleSetPlaneGapsForTvs();
	void CycleSetCornerGapsForTvs();
	void CycleSetNeigsForTvs();
	void LoadingAssemblies();
	std::pair<size_t, double> Rounding(double _gs) const;
	VS CyclingConstantFinding(const VS & _id);
	int FindTheConstant(const std::string & _id) const;
	void SetCornerGapsForTvs(Assembly& tvs, const size_t _time_point);
	void SetGapsForTvs(Assembly& tvs, const size_t _time_point);
//// Making newdata file
	// Half part may be repalaced with a cycle, that call all parameters in order
	void NewdataMaking();
	void NewdataPS();
	void CoolantParsing(const std::vector<std::string>& inputVector, std::vector<std::vector<double>>& outputVector);
	template<typename K>
	void GetPoints(const std::string& inputString, std::vector<K> & inputArray, int initPos);
	void GetBurnupHistory();
	void GetStatesParameters();
	void GetDensity();
	void GetTemperatures();
	void GetBacklings();
	void GetSpecificPower();
	void GetBoricAcid();
	void NewdataForming();
	void NewdataPathForming();
	void GenerateCommonParameters(std::ostream& ofs);
	bool CheckInputDataStates();
	void PermutationForming(std::ostream& ofs);

};


#include "Loading test parameters.h"
#include "Permapar.h"
#include "Assemblies_Calculations.h"
#include "Newdata.h"

