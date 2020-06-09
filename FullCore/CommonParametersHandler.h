#pragma once

class CommonParametersHandler
{
public:
	CommonParametersHandler();
	CommonParametersHandler(bool isBlank);

	void FilesListCheck();
	void FileListInitialize();
	std::vector<Calculation> GetFilesList() const;
	void GetParametersList();
	void GetTestsName();
	void GrabFilesList(const std::string& folder);
	void InnerStructInitialize();
	std::string GetInnerStruct(std::string &folderName);

protected:
	FileHandler fileReader;
	std::string innerStruct_bin;
	std::string innerStruct_data;
	std::string innerStruct_Cr;
	std::string innerStruct_res;

	std::string mainParameters;
	VS temp;
	VS testName;
	std::set<std::string> fileList;
	std::map<std::string, std::string> parametersList;
	std::vector<Calculation> m_relativeFileList;
};

CommonParametersHandler::CommonParametersHandler() {

	mainParameters = "gap_8.par";
	InnerStructInitialize();
	FileListInitialize();
	GetParametersList();
	GetTestsName();
	FilesListCheck();

}

CommonParametersHandler::CommonParametersHandler(bool isBlank)
{
	InnerStructInitialize();
}

void CommonParametersHandler::InnerStructInitialize()
{
	innerStruct_bin = "bin";
	innerStruct_data = innerStruct_bin + char(92) + "data";
	innerStruct_Cr = innerStruct_data + char(92) + "Cr";
	innerStruct_res = innerStruct_bin + char(92) + "res";
	std::cerr << fileReader.CreatePath(innerStruct_data) << " ";
	std::cerr << fileReader.CreatePath(innerStruct_Cr) << " ";
	std::cerr << fileReader.CreatePath(innerStruct_res) << " \n";
}

void CommonParametersHandler::FileListInitialize()
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

void CommonParametersHandler::GetParametersList()
{
	std::string _key, _value;
	std::vector<std::string> parameters;
	parameters = fileReader.GetLine(mainParameters);
	for (auto& t : parameters)
	{
		_key = GetStringParam(t, 1);
		ToUpperFunct(_key);
		_value = GetStringParam(t, 2);
		parametersList[_key] = _value;
	}
}

void CommonParametersHandler::GetTestsName()
{
	testName = fileReader.GetLine(parametersList.at("TESTS_LIST"));
	try {
		for (auto& item : testName) {
			ToUpperFunct(item);
			GrabFilesList(
				parametersList.at("WORK_DIRECTORY")
				+ char(92)
				+ innerStruct_Cr
				+ char(92)
				+ item
				+ char(92));
			m_relativeFileList[m_relativeFileList.size() - 1].SetTestName(item);
		}
	}
	catch (std::exception& exc)
	{
		std::cerr << exc.what()
			<< " " << __FUNCTION__ << std::endl;
	}

}

void CommonParametersHandler::FilesListCheck()
{

	for (size_t j = 0; j < m_relativeFileList.size(); ++j)
	{
		size_t counter = 0;
		SS item = m_relativeFileList[j].List();
		SS fnames = fileList;
		for (auto file : item)
		{
			ToUpperFunct(file);
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

void CommonParametersHandler::GrabFilesList(const std::string& folder)
{
	SS m_absoluteFileList;
	fileReader.ScanDirectoryForFiles(m_absoluteFileList, folder);
	m_relativeFileList.push_back(m_absoluteFileList);
	m_relativeFileList[m_relativeFileList.size() - 1].SetFilesNames(fileList);
}

std::vector<Calculation> CommonParametersHandler::GetFilesList() const
{
	return m_relativeFileList;
}

std::string CommonParametersHandler::GetInnerStruct(std::string & folderName)
{
	ToUpperFunct(folderName);

	if (folderName == "BIN")
	{
		return innerStruct_bin;
	}
	else if (folderName == "CR")
	{
		return innerStruct_Cr;
	}
	else if (folderName == "DATA")
	{
		return innerStruct_data;
	}
	else if (folderName == "RES")
	{
		return innerStruct_res;
	}
	else
	{
		std::cerr << __FUNCTION__ << ": unknown argument -> <" << folderName << "'\n";
		return innerStruct_bin;
	}
}