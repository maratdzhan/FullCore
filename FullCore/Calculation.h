#pragma once


struct Calculation
{
	Calculation();
	Calculation(const std::set<std::string>& _filesList);

	void SetFilesNames(const std::set<std::string>& _filesList);
	void SetInitializing();
	MSS GetFilesSet() const;
	std::string GetFileByName(const std::string& name) const;
	SS List() const;
	void SetTestName(const std::string& _name);
	std::string GetTestName() const;
	SS GetFilesNames() const;
	bool IsCalculationInitialized() const;
	void SetMap(const std::string& _key, const std::string& _value);
	void CopyPathsMap(const MSS& _map);
	MSS PathsMap() const;

private:
	bool m_isInitialized;
	SS m_filesList;
	SS m_filesName;
	std::string m_name;
	MSS m_filesListMap;
};


Calculation::Calculation()
{
	m_name = "default constructor\n";
	m_isInitialized = false;
}

Calculation::Calculation(const std::set<std::string>& _filesList)
{
	m_isInitialized = false;
	m_filesList = _filesList;

}

void Calculation::SetFilesNames(const std::set<std::string>& _filesList)
{
	m_filesName = _filesList;
}

void Calculation::SetInitializing()
{
	m_isInitialized = true;
}

MSS Calculation::GetFilesSet() const
{
	return m_filesListMap;
}

std::string Calculation::GetFileByName(const std::string& name) const
{
	try {
		return m_filesListMap.at(name);
	}
	catch (std::exception& wrong_file_id) {
		std::cerr << "Wrong file ID at "<< __FUNCTION__ << 
					" name <" << name << ">\n"
					<< wrong_file_id.what() << std::endl;
	}
	return "ERROR";
}

SS Calculation::List() const
{
	return m_filesList;
}

void Calculation::SetTestName(const std::string& _name)
{
	m_name = _name;
}

std::string Calculation::GetTestName() const
{
	return m_name;
}

SS Calculation::GetFilesNames() const
{
	return m_filesName;
}

bool Calculation::IsCalculationInitialized() const
{
	return m_isInitialized;
}

void Calculation::SetMap(const std::string& _key, const std::string& _value)
{
	m_filesListMap[_key] = _value;
}

void Calculation::CopyPathsMap(const MSS& _map)
{
	if (!_map.empty())
		m_filesListMap = _map;
}

MSS Calculation::PathsMap() const
{
	return m_filesListMap;
}

