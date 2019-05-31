#pragma once

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
	accounted_points,
};

enum newdataParameters
{
	nd_unknown,
	states_number,
	xe_flag,
	sm_flag,
	dopler_flag,
	nbippar,
	symmetry,
	ireg,
	itemp,
	igam,
	time_points,
};

struct FileHandler
{
public:
//** - not using so commented

	//// Using getline method to read file
	std::vector<std::string> GetLine(const std::string& _path)
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
				throw (std::invalid_argument("Cant find file <" + _path + "> \n"));
			}
			iFile.close();
		}
		catch (std::exception & reading_exc)
		{
			std::cerr << reading_exc.what() << std::endl;
		}

//**	m_handledFiles.push_back(_path);
		return vc;
	}

	//// Define parameter type by name
	//// WOULD BE REPLACED BY HASHTABLE
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
			if (temporary == "ACCOUNTED_POINTS")
				return accounted_points;


			throw std::invalid_argument("unknown argument " + _parameterName);
			return unknown;
		}
		catch (std::invalid_argument & ia_p)
		{
			std::cerr << ia_p.what() << " at " << __FUNCTION__ << std::endl;
		}
		return unknown;
	}

	//// Using CreatePath method to create folders
	int CreatePath(const std::string _path)
	{
		std::string t;
		if (CreateDirectory(_path.c_str(), NULL))
		{
			t = "- Directory " + (_path)+" created.\n";
			return 1;
		}
		else {
		//	ToConsole(GetLastError());
			if (GetLastError() == ERROR_ALREADY_EXISTS) {
				t = "Directory " + _path+ " already exists.\n";
				return 0;
			}
			else if (GetLastError() == ERROR_PATH_NOT_FOUND) {
				t = "WARNING! Directory " + _path + " is not created. \n";
				return -1;
			}
		}
		return -1;
	}

	//// WOULD BE REPLACED BY HASHTABLE
	newdataParameters GetNewdataType(const std::string& _parameterName)
	{
		try {
			std::string temporary = _parameterName;
			ToUpperFunct(temporary);
//			if (temporary == "TIME_POINTS")
//				return time_points;
			if (temporary == "STATES_NUMBER")
				return states_number;
			if (temporary == "XE_FLAG")
				return xe_flag;
			if (temporary == "SM_FLAG")
				return sm_flag;
			if (temporary == "DOPLER_FLAG")
				return dopler_flag;
			if (temporary == "NBIPPAR")
				return nbippar;
			if (temporary == "SYMMETRY")
				return symmetry;
			if (temporary == "IREG")
				return ireg;
			if (temporary == "INPUT_TEMPERATURE")
				return itemp;
			if (temporary == "INPUT_DENSITY")
				return igam;
//			if (temporary == "SPECIFIC_POWER")
//				return wud;

			throw std::invalid_argument("unknown argument " + _parameterName);
			return nd_unknown;
		}
		catch (std::invalid_argument & ia_p)
		{
			std::cerr << ia_p.what() << " at " << __FUNCTION__ << std::endl;
		}
		return nd_unknown;

	}

private:
//**	VS m_handledFiles;
};

struct Calculation
{
	Calculation()
	{
		m_name = "default constructor\n";
		m_isInitialized = false;
	}

	Calculation(const std::set<std::string>& _filesList)
	{
		m_isInitialized = false;
		m_filesList = _filesList;

	}

	void SetFilesNames(const std::set<std::string>& _filesList)
	{
//		SetInitializing();
//		for (const auto& item : _filesList)

		m_filesName = _filesList;
	}

	//// Set variable m_isInitialized t-> true; call MapDistribution();
	void SetInitializing()
	{
		m_isInitialized = true;
	}

	//// Make the relation between file path (value) and his name (key)

	//// Returns key-value list of files
	std::map<std::string, std::string> GetFilesSet() const
	{
		return m_filesListMap;
	}

	//// Return file path by the name
	std::string GetFileByName(const std::string& name) const
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

	//// Returns the list with file path
	SS List() const
	{
		return m_filesList;
	}

	//// Set the test name
	void SetTestName(const std::string& _name)
	{
		m_name = _name;
	}

	//// Return test name
	std::string GetTestName() const
	{
		return m_name;
	}

	//// Returns list with file name
	SS GetFilesNames() const
	{
		return m_filesName;
	}

	//// Return flag of test initialization status
	bool IsCalculationInitialized() const
	{
		return m_isInitialized;
	}

	void SetMap(const std::string & _key, const std::string & _value)
	{
		m_filesListMap[_key] = _value;
	}

	void CopyPathsMap(const std::map<std::string, std::string>& _map)
	{
		if (!_map.empty())
			m_filesListMap = _map;
	}

	std::map<std::string, std::string> PathsMap() const 
	{
		return m_filesListMap;
	}

private:
	bool m_isInitialized;
	SS m_filesList;
	SS m_filesName;
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

	// Check: all necessary files are exist?
	void FilesListCheck()
	{

		for (size_t j = 0; j < m_relativeFileList.size(); ++j)
		{
			size_t counter = 0;
			SS item = m_relativeFileList[j].List();
			SS fnames = fileList;
			for (auto file : item)
			{
				ToUpperFunct(file);
				// //== - mean old algorithm
				// ///	- mean new algorithm

				for (const auto& fitem : fnames)
				{
					if (file.find(fitem) != -1)
					{
						m_relativeFileList[j].SetMap(fitem, file);
						item.erase(file);
						fnames.erase(fitem);
						counter++;
						break;
					}
				}
			}
			if (counter != fileList.size())
			{
				std::cerr << "calculation cant be prepared: "
					<< m_relativeFileList[j].GetTestName() << " not inizialized. Reason:\nWrong input file's count\n";
			}
			else
				m_relativeFileList[j].SetInitializing();
		}
		
	}

	// List with necessary files
	void FileListInitialize()
	{
		fileList.insert("BASEMENT.NAR");
		fileList.insert("BOR.CON");
		fileList.insert("CONST.DAT");
		fileList.insert("COORDS.PVM");
		fileList.insert("GAM.CON");
		fileList.insert("LIST.TXT");
		fileList.insert("MAPKAS.DAT");
		fileList.insert("MAPN.DAT");
		fileList.insert("NEWDATA.DAT");
		fileList.insert("PARAMETERS.LOAD");
		fileList.insert("PERMPAR_INPUT.DAT");
		fileList.insert("TEMP.CON");
		fileList.insert("TIME_POINTS.CON");
		fileList.insert("WUD.CON");
		fileList.insert("BACKL.CON");

		fileList.insert("XE_FLAG.CON");
		fileList.insert("SM_FLAG.CON");
		fileList.insert("DOPL_FLAG.CON");
		fileList.insert("INPUT_DENSITY.CON");
		fileList.insert("INPUT_TEMP.CON");
	}

	// Return files list.. again?
	std::vector<Calculation> FilesList() const
	{
		// We have list with N files.
		return m_relativeFileList;

	}

private:
	//// Get Parameters from gap.par
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

	//// Get names of all tests ...
	void GetTestsName()
	{
		std::string catcher;
		testName = fileReader.GetLine(parametersList.at("TESTS_LIST"));
		try {
			for (auto& item : testName) {
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

	//// Add names to files list
	void GetFilesList(const std::string & folder)
	{
		SS m_absoluteFileList;
		//std::string name = 
		for (const auto& entry : std::filesystem::recursive_directory_iterator(folder)) {
			std::string entire = entry.path().string();

			m_absoluteFileList.insert((entry.path().string()));

			//// Must run reversely (back order)
				//for (unsigned int i = 0; i < folder.size(); i++) {
				//	entire.erase(entire.begin());
				//}


		}
		m_relativeFileList.push_back(m_absoluteFileList);
		m_relativeFileList[m_relativeFileList.size() - 1].SetFilesNames(fileList);
	}

	// Making paths for work
	void InnerStructInitialize()
	{
		innerStruct_bin = "bin";
		innerStruct_data = innerStruct_bin + char(92) + "data";
		innerStruct_Cr = innerStruct_data + char(92) + "Cr";
		innerStruct_res = innerStruct_bin + char(92) + "res";
		std::cerr << fileReader.CreatePath(innerStruct_data) << " ";
		std::cerr << fileReader.CreatePath(innerStruct_Cr) << " ";
		std::cerr << fileReader.CreatePath(innerStruct_res) << " \n";
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
	std::set<std::string> fileList;
	std::map<std::string, std::string> parametersList;
	std::vector<Calculation> m_relativeFileList;
};

