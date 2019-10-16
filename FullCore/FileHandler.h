#pragma once

struct FileHandler
{
public:

	int CreatePath(const std::string& _path);
	parametersName GetType(const std::string& _parameterName);
	std::vector<std::string> GetLine(const std::string& _path);
	newdataParameters GetNewdataType(const std::string& _parameterName);
	void ScanDirectoryForFiles(SS & inputArray, const std::string& _dir);
};

std::vector<std::string> FileHandler::GetLine(const std::string& _path)
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
					vc.push_back(currentString);
			}
		}
		else {
			throw (std::invalid_argument("Cant find file <" + _path + "> \n"));
		}
		iFile.close();
	}
	catch (std::exception& reading_exc)
	{
		std::cerr << reading_exc.what() << std::endl;
	}

	return vc;
}

parametersName FileHandler::GetType(const std::string& _parameterName)
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
		if (temporary == "MSQ")
			return msq;

		throw std::invalid_argument("unknown argument " + _parameterName);
		return unknown;
	}
	catch (std::invalid_argument& ia_p)
	{
		std::cerr << ia_p.what() << " at " << __FUNCTION__ << std::endl;
	}
	return unknown;
}

int FileHandler::CreatePath(const std::string& _path)
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
			t = "Directory " + _path + " already exists.\n";
			return 0;
		}
		else if (GetLastError() == ERROR_PATH_NOT_FOUND) {
			t = "WARNING! Directory " + _path + " is not created. \n";
			return -1;
		}
	}
	return -1;
}

newdataParameters FileHandler::GetNewdataType(const std::string& _parameterName)
{
	try {
		std::string temporary = _parameterName;
		ToUpperFunct(temporary);
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

		throw std::invalid_argument("unknown argument " + _parameterName);
		return nd_unknown;
	}
	catch (std::invalid_argument& ia_p)
	{
		std::cerr << ia_p.what() << " at " << __FUNCTION__ << std::endl;
	}
	return nd_unknown;

}

void FileHandler::ScanDirectoryForFiles(SS & inputArray, const std::string& _dir)
{
	for (const auto& entry : std::filesystem::recursive_directory_iterator(_dir)) {
		inputArray.insert((entry.path().string()));
	}
}