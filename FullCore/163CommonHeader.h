#pragma once

#include <Windows.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cassert>
#include <map>


using VS=std::vector<std::string>;

#include "Assemblies_Object.h"
#include "GetParam.h"
#include "Coordinates_Definition.h"

#include <filesystem>

















struct Calculation
{
	Calculation()
	{
		m_name = "default constructor\n";
	}

	Calculation(const VS _filesList)
	{
		m_isInitialized = false;
		m_files = _filesList;
	}

	void SetInitializing()
	{
		m_isInitialized = true;
	}

	VS List() const
	{
		return m_files;
	}

	void SetName(std::string _name)
	{
		m_name = _name;
	}

	std::string GetName() const
	{
		return m_name;
	}

	bool IsCalculationInitialized() const
	{
		return m_isInitialized;
	}

private:
	bool m_isInitialized;
	VS m_files;
	std::string m_name;
};










class CommonParametersHandler
{
public:
	CommonParametersHandler() {
		mainParameters = "gap.par";
		InnerStructInitialize();
		GetParametersList();
		GetTestsName();
		FileListInitialize(); // Initializing list for files checking
		FilesListCheck();
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
			std::cerr << reading_exc.what() << std::endl;
		}
		
		return vc;
	}

	void FilesListCheck() 
	{
		size_t counter = 0;
		for (size_t j=0; j<m_relativeFileList.size(); ++j)
		{
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
					<<m_relativeFileList[j].GetName()<<"\n";
			}
			else
				m_relativeFileList[j].SetInitializing();
			counter = 0;
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
		parameters = FileReader(mainParameters);
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
		//SetCurrentDirectory(parametersList.at("TESTS_DIRECTORY").c_str());
		testName = FileReader(parametersList.at("TESTS_LIST"));
		
		for (auto & item : testName) {
			ToUpperFunct(item);
			GetFilesList(parametersList.at("WORK_DIRECTORY")+char(92)+innerStruct_Cr+char(92) + item+char(92));
			try {
				m_relativeFileList[m_relativeFileList.size() - 1].SetName(item);
			}
			catch (std::exception & relativeFileListErr)
			{
				std::cerr << relativeFileListErr.what() << "\nUnexpected end of array at\n"
					<< item << "\n";
			}
		}
	}


	void GetFilesList(const std::string & folder)
	{
		try {
			VS filesForTest;
			VS m_absoluteFileList;
			//std::string name = 
			for (const auto & entry : std::filesystem::recursive_directory_iterator(folder)) {
				std::string entire = entry.path().string();

					m_absoluteFileList.push_back(entry.path().string());

				// Must run reversely (back order)
				for (unsigned int i = 0; i < folder.size(); i++) {
					entire.erase(entire.begin());
				}
				
			//	filesForTest.push_back(entire);
			}
			m_relativeFileList.push_back(m_absoluteFileList);
		}
		catch (std::exception & exc_file_system)
		{
			std::cerr << folder << ">>>>>>\n error when get file list at:\n"<<
				folder<<"\n>>>>>>\n";
		}
	}

	void InnerStructInitialize()
	{
		innerStruct_bin = "bin";
		innerStruct_data = innerStruct_bin + char(92) + "data";
		innerStruct_Cr = innerStruct_data + char(92) + "Cr";
		innerStruct_res = innerStruct_bin + char(92) + "res";
	}

private:

	
	std::string innerStruct_bin;
	std::string innerStruct_data;
	std::string innerStruct_Cr;
	std::string innerStruct_res;

	std::string mainParameters = "gap.par";
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
		if (_currentObject.IsCalculationInitialized())
			m_Compilation = { _currentObject.List() };
		else
			throw std::out_of_range(_currentObject.GetName()
				+ ": the task cannot be completed because it is not initialized."+
				"\nThe following calculation will be launched.\n");
	}

private:

		Calculation m_Compilation;

};