#pragma once

#define _USE_MATH_DEFINES

#include "Including.h"
#include "EnumList.h"
#include "GetParam.h"
#include "Coordinates_Definition.h"
#include "Assemblies_Class.h"
#include "HashTables.h"
#include "FileHandler.h"
#include "Calculation.h"
#include "CommonParametersHandler.h"


class Core
{
protected:
	int lastState;
	int noErrors;
	int singleStateMode;
	bool statMode;
	double _tvs_size;
	double _tvs_step;
	double nominalGapSize;
	std::string _pp_library;
	uint16_t _fa_count;
	uint16_t _tvel_count;
	uint16_t _tveg_count;
	uint16_t _node_count;
	uint8_t _coordinate_system = 0;
	std::vector<double> first_coordinate;
	std::vector<double> second_coordinate;
	SS gapsFilesList;
	uint8_t accounted_points_number;
	std::vector<double> gap_sizes;
	std::string p_workdirectory;
	std::string p_project_name;
	uint8_t unit_number;
	uint8_t fuel_cycle_number;
	std::string outFile;
	VS project_path_properties;
	uint8_t permak_max_states_quantity;

	// Newdata:
	std::vector<int16_t> m_time_points, m_xe_flag, m_sm_flag, m_dopler_flag, reloads, parsed_times;
	int m_states_number;
	int m_nbippar;
	int m_symmetry;
	int m_ireg;
	std::vector<std::vector<double>> m_temp, m_gam, m_backl;
	std::vector<double> m_itemp, m_igam, m_wud, m_bor;
	VS newdata_parameters;
	std::string newdataPath;
	std::string unitPath;
	
	// Permpar:
	VS _mapkasArray;
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
	int geometry;
	bool isInitialized;
	bool isModifierAccounted;
	std::vector<int> numBlock;

	// Classes:
	Calculation m_Compilation;
	Coordinates m_coreCoordinates;
	FileHandler file;
	std::vector<Assembly> _fuelAssemblies;
	ConstantsHashTable CHT;
	/////////////////////////////



	
public:
	//// Modes && Initializing
	Core(const Calculation& _currentObject, int);
	void StatMode();
	void SingleMode();
	void Clear();

	//// Reading test input files
	void FileReading();
	void ReadingUnitInfo();
	void ReadingParameters();
	void ReadingMapn(const uint16_t _count);
	void ReadingGaps();
	void ReadingMapkas();
	void ReadingBasement();
	void ReadingList();
	void ReadingPermpar();
	void ReadingConstants();
	void ReadingNewdataParameters();
	void GSLastState(int, bool);
	void ReadingTimeParameters();
	void ParsingTimeParameters();
	void SetStatMode();
	bool GetStatMode() const;
	template<typename K>
	void _WriteToStream(std::ostream& ofs, const K& inputCollection, unsigned int _init_pos, unsigned int _end_pos);
	template<typename K>
	void _Comp_WriteToStream(std::ostream& ofs, const K& inputCollection, unsigned int _init_pos, unsigned int _end_pos);
	template<typename K>
	bool _GetParametersFromFile(K& inputCollection, const std::string& file);
	void GetDebugPVM();
	void ExtractCoordinates(VS& gapsArray);
	double ExtractRo5(int _const_number);
	int GetStateFromTime(int _time) const;

	//// Readed files handling
	void ListHandle(const std::string& inputString);
	void GapsListParameters(uint8_t _parameter, std::string& _value);
	void CraftGapsList();
	//// Making permpar file
	void CreatePermparFile();
	void PermparMaking();
	void PermparSP(const std::string& _s);
	int TouchCase(const std::string& _str) const;
	void WriteToPermpar();
	void NalArraysForming();
	void Nal2Generating();
	void Nal3Generating();
	void AssembliesArrayForming();
	void ConstantsForming();
	void LibraryIncluding();
	int DefinePermparNumber(int number, size_t time_point);
	void BuildMapkas(Assembly& tvs, const size_t _time_point);
	//// Assemblies handler
	void AssembliesInitialize(std::vector<std::pair<double, double>> & coordinates);
	void AssembliesShiftings(std::vector<std::pair<double, double>> & coordinates);
	void CycleSetPlaneGapsForTvs();
	void CycleSetCornerGapsForTvs();
	void CycleSetNeigsForTvs();
	void LoadingAssemblies();
	std::pair<size_t, double> Rounding(double _gs) const;
	VS CyclingConstantFinding(const VS& _id);
	int FindTheConstant(const std::string& _id) const;
	void SetCornerGapsForTvs(Assembly& tvs, const size_t _time_point);
	void SetPlaneGapsForTvs(Assembly& tvs, const size_t _time_point);
	void SetAssemblyGapsFinal();
	double SetCorrection(double cb, double gam, double ro5, double gs);
	void SetAdditionalAttributes(Assembly& tvs);
	void RebuildGaps(Assembly& tvs, const size_t _time_point);
	void SetEnrichment(Assembly& tvs);
	void SetNalArrays(Assembly& tvs, const size_t _state);
	void SaveGaps(int state_num);

	//// Making newdata file
	// Half part may be repalaced with a cycle, that call all parameters in order
	void NewdataMaking();
	void NewdataPS();
	void CoolantParsing(const std::vector<std::string>& inputVector, std::vector<std::vector<double>>& outputVector);
	template<typename K>
	void GetPoints(const std::string& inputString, std::vector<K>& inputArray, int initPos);
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
	//// Extracting Results
	void GrabResults();
	void GrabOutFile(std::vector<std::string>& kq_str);
	void GrabKqValues(std::vector<std::string>& kq_str, std::vector<std::string>& output);
	void KQ_P_String(std::string& input, std::vector<std::string>& output);
	void SaveKqValues(std::vector<std::string>& output);
	int GetOutSchemeElement(int& inputNumber);
	void OutSchemeElementInitializing();

	//// PERM
	void CreatePermFile();
};


Core::Core(const Calculation& _currentObject, int ssm)
{
	singleStateMode = ssm;
	lastState = 0;
	accounted_points_number = 0;
	fuel_cycle_number = 0;
	isInitialized = _currentObject.IsCalculationInitialized();
	isModifierAccounted = true;
	statMode = false;
	geometry = 6;
	m_ireg = 0;
	m_nbippar = 0;
	m_states_number = 0;
	m_symmetry = 0;
	maxGapVal = 0;
	minGapVal = 0;
	noErrors = 0;
	nominalGapSize = 0;
	permak_max_states_quantity = 0;
	reflectorDistance = 0.0;
	stepGapValue = 0;
	unit_number = 0;

	_coordinate_system = 0;
	_fa_count = 0;
	_node_count = 0;
	_tvel_count = 0;
	_tveg_count = 0;
	_tvs_size = 0;
	_tvs_step = 0;


	OutSchemeElementInitializing();

	if (_currentObject.IsCalculationInitialized()) {
		try {
			std::cout << "===============\nHandling begin:"
				<< _currentObject.GetTestName() << std::endl;
			m_Compilation = { _currentObject.List() };
			m_Compilation.SetFilesNames(_currentObject.GetFilesNames());
			m_Compilation.SetTestName(_currentObject.GetTestName());
			m_Compilation.SetInitializing();
			m_Compilation.CopyPathsMap(_currentObject.PathsMap());
			std::cerr << "2. Files array has been copied\n";
		}
		catch (std::exception& Core_constructor_exception)
		{
			std::cerr << Core_constructor_exception.what();
		}
	}
}



void Core::StatMode()
{
	int counter = 0;
	bool first = true;
	//// Prepare gaps in cycle and run < single state mode >
	std::string rs = "res";
	CommonParametersHandler h0(true);
	std::ofstream ofs(p_workdirectory + h0.GetInnerStruct(rs) + "gapsFilesList.txt");
	for (const auto& item : gapsFilesList)
		ofs << item << "\n";
	ofs.close();

	//// FULL STATES LIST MODE
	for (const auto& item : gapsFilesList) {
		if (singleStateMode == 1 && counter != lastState)
		{
			counter++;
			continue;
		}

		VS gapsStrings = file.GetLine(item);
		ExtractCoordinates(gapsStrings);

		LoadingAssemblies();
		if (first) {
			NewdataMaking();
			first = false;
		}
		SaveGaps(counter);
		SetAssemblyGapsFinal();
		PermparMaking();

		if (GetStatMode()) {
			CreatePermFile();
			std::cerr << ">>>>>> Waiting for PERMAK... State: " << counter++ << "\n";
			std::cerr << "name: " << item.c_str() << "\n";
			system("#autorun.bat");
		}
		GrabResults();
		Clear();
	}
	if (singleStateMode == 1)
		GSLastState(lastState + 1, true);
}

void Core::SingleMode()
{
	LoadingAssemblies();
	NewdataMaking();
	PermparMaking();
	GrabResults();
}


#include "LoadingTestParameters.h"
#include "Modifier_coefficient.h"
#include "Permapar.h"
#include "Assemblies_Calculations.h"
#include "Newdata.h"
#include "GrabResults.h"
#include "Debug.h"

