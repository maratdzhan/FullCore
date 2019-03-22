#pragma once

#include <Windows.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cassert>
#include <map>

#include "GetParam.h"
#include "Coordinates_Definition.h"





























class CommonParametersHandler
{
public:
	CommonParametersHandler() {
		mainParameters = "gap.par";
		GetParametersList();
		GetTestsName();
	}

	std::vector<std::string> FileReader(const std::string & _path)
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
			std::cout << reading_exc.what() << std::endl;
		}
		
		return vc;
	}




private:
	void GetParametersList()
	{
		std::string _key, _value;
		std::vector<std::string> parameters;
		parameters = FileReader(mainParameters);
		for (auto t : parameters)
		{
			_key = GetStringParam(t, 1);
			ToUpperFunct(_key);
			_value = GetStringParam(t, 2);
			ToUpperFunct(_value);
	//		PathPreparing(_value);
			parametersList[_key] = _value;
		}
	}

	void GetTestsName()
	{
		//SetCurrentDirectory(parametersList.at("TESTS_DIRECTORY").c_str());
		testName = FileReader(parametersList.at("TESTS_LIST"));
	//	testName = FileReader("C:\\Users\\stu1\\source\\repos\\FullCore\\FullCore\\bin\\data\\Cr\\tests.list");
	}

	void GetFilesList(const std::string & folder)
	{
		//std::string path = folder + char(92);
		//for (const auto & entry : std::filesystem::recursive_directory_iterator(path)) {
		//	std::string entire = entry.path().string();

		//	m_absoluteFileList.push_back(entry.path().string());

		//	for (unsigned int i = 0; i < path.size(); i++) {
		//		entire.erase(entire.begin());
		//	}

		//	m_relativeFileList.push_back(entire);
		//}

	}

private:

	std::string mainParameters = "gap.par";
	std::vector<std::string> testName;
	std::map<std::string, std::string> parametersList;
};