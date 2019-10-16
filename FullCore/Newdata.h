#pragma once

void Core::NewdataMaking()
{
	if (!accounted_points_number)
	{
		std::cerr << "Absent accounted points - terminating\n";
		return;
	}

	NewdataPS();
	GetBurnupHistory();
	if (!CheckInputDataStates())
		return;
	/*
	Algorithm description:
	the program read input data with *.con extension. 
	After the reading for each of vector<string>-arrays calling related method (like GetDensity() for density).
	In each method for each string program calling method <CoolantParsing()> that get string and send 
	it to <GetPoint()>.
	In <GetPoint()> method program get vector array from comma-separeted strings, that after added 
	to output parameters array. 
	When first parameters array size reach tvs number, program change state (state mean next vector-array)
	Now call <NewDataForming()>
	*/
	if (noErrors == 0 && !CT_BLANK) {
		NewdataForming();
	}
}



void Core::NewdataPS()
{
	for (const auto& item : newdata_parameters)
	{
		newdataParameters _name = file.GetNewdataType(GetStringParam(item, 1));
		switch (_name)
		{
		case (nbippar):
			GetParam(m_nbippar, item, 2);
			break;
		case (symmetry):
			GetParam(m_symmetry, item, 2);
			break;
		case (ireg):
			GetParam(m_ireg, item, 2);
			break;
		case (states_number):
			GetParam(m_states_number, item, 2);
			if (m_states_number < accounted_points_number)
				noErrors = false;
			break;
		default:
			break;
		}
	}
}


template<typename T>
void Core::GetPoints(const std::string& inputString, std::vector<T> & inputVector,int initPos)
{
	int lastVal = 0;
	int pos = initPos;
	while (lastVal != -1 && lastVal != 255)
	{
		inputVector.push_back(-1);
		GetParam(inputVector[inputVector.size() - 1], inputString, ++pos);
		lastVal = static_cast<int>(inputVector[inputVector.size() - 1]);
	}
	inputVector.erase(inputVector.begin() + inputVector.size() - 1);
}

void Core::CoolantParsing(const std::vector<std::string>& inputVector, std::vector<std::vector<double>>& outputVector)
{
	std::vector<double> temporary;
	for (const auto& str : inputVector)
	{
		GetPoints(str, temporary, 0);			// String written to array
		if (temporary.size() >= _fa_count)	// Check size -> more/equal or lesser, than _fa_count?
		{
			outputVector.push_back(temporary);
			temporary.clear();
		}
	}
}

void Core::GetBurnupHistory()
{
	GetStatesParameters();
	GetDensity();
	GetTemperatures();
	GetSpecificPower();
	GetBoricAcid();
	GetBacklings();
}

template<typename K>
bool Core::_GetParametersFromFile(K & inputCollection, const std::string & _source)
{
	VS _template = file.GetLine(m_Compilation.GetFileByName(_source));

	if (_template.size()) {
		for (const auto& item : _template)
			GetPoints(item, inputCollection, 0);
	}
	else
	{
		std::cerr << "wrong data for ::" << _source << std::endl;
		noErrors ++;
		return false;
	}
	
	return true;

}

void Core::GetStatesParameters()
{
//	std::cerr << "Time points --------->" << (_GetParametersFromFile(m_time_points, "TIME_POINTS.CON") ? "OK" : "FAILED") << std::endl;
	std::cerr << "Xe flags ------------>" << (_GetParametersFromFile(m_xe_flag, "XE_FLAG.CON") ? "OK" : "FAILED") << std::endl;
	std::cerr << "Sm flag ------------->" << (_GetParametersFromFile(m_sm_flag, "SM_FLAG.CON") ? "OK" : "FAILED") << std::endl;
	std::cerr << "Dopler flag --------->" << (_GetParametersFromFile(m_dopler_flag, "DOPL_FLAG.CON") ? "OK" : "FAILED") << std::endl;
	std::cerr << "Input temperatures -->" << (_GetParametersFromFile(m_itemp, "INPUT_TEMP.CON") ? "OK" : "FAILED") << std::endl;
	std::cerr << "Input density ------->" << (_GetParametersFromFile(m_igam, "INPUT_DENSITY.CON") ? "OK" : "FAILED") << std::endl;
}

void Core::GetDensity()
{
	VS densityArray = file.GetLine(m_Compilation.GetFileByName("GAM.CON"));
	CoolantParsing(densityArray, m_gam);
	//std::cerr << "Amount Density states : " << m_gam.size() << std::endl;
//	DebugWriteToFile(m_gam);
}

void Core::GetTemperatures()
{
	VS temperatureArray = file.GetLine(m_Compilation.GetFileByName("TEMP.CON"));
	CoolantParsing(temperatureArray, m_temp);
	//std::cerr << "Amount Temperature states: " << m_temp.size() << std::endl;
//	DebugWriteToFile(m_temp);
}

void Core::GetBacklings()
{
	VS backlArray = file.GetLine(m_Compilation.GetFileByName("BACKL.CON"));
	CoolantParsing(backlArray, m_backl);
	//std::cerr << "Amount Density states : " << m_backl.size() << std::endl;
	//	DebugWriteToFile(m_gam);
}

void Core::GetSpecificPower()
{
	VS wudArray = file.GetLine(m_Compilation.GetFileByName("WUD.CON"));
	for (const auto& _item : wudArray) {
		GetPoints(_item, m_wud, 0);
	}
	//std::cerr << "Amount WUD states : " << m_wud.size() << std::endl;
}

void Core::GetBoricAcid()
{
	VS boracicAcidArray = file.GetLine(m_Compilation.GetFileByName("BOR.CON"));
	for (const auto& _item : boracicAcidArray) {
		GetPoints(_item, m_bor, 0);
	}
	//std::cerr << "Amount Boric Acid states : " << m_gam.size() << std::endl;
}

void Core::NewdataPathForming()
{
	std::cerr << "Placing newdata file (1 - NEW FOLDER, 0 - OK, 2 - WRONG PATH): ";
	newdataPath = p_workdirectory + "" + p_project_name + "\\";
	int err = file.CreatePath(newdataPath);
	if (err == 0)
		std::cerr << err<<" ";
	newdataPath += "B" + ((unit_number < 9 ? "0" : "") + std::to_string(unit_number)) + "\\";
	err = file.CreatePath(newdataPath);
	if (err == 0)
		std::cerr << err << " ";
	newdataPath += "K" + ((fuel_cycle_number <= 9 ? "0" : "") + std::to_string(fuel_cycle_number)) + "\\";
	err = file.CreatePath(newdataPath);
	unitPath = newdataPath;
	if (err == 0)
		std::cerr << err << "\n";
	newdataPath += "newdata";
}


void Core::GenerateCommonParameters(std::ostream & ofs)
{
	ofs << "&newdata\n";
	ofs << "ireg=" << m_ireg << ",\n";
	ofs << "NBIPPAR=" << m_nbippar << ",\n";
	ofs << "SIM=" << m_symmetry << ",\n";
	ofs << "KSLOTR=1" << ",\n";
}


void Core::NewdataForming()
{
	NewdataPathForming();
	std::ofstream ofs(newdataPath);
	if (!ofs.is_open())
		CreateFile(newdataPath.c_str(), GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	
	GenerateCommonParameters(ofs);
	ofs << "time=";
	_WriteToStream(ofs, m_time_points, 0, accounted_points_number);
	ofs << "nsos=";
	for (int i = 0; i < accounted_points_number; i++)
		ofs << 1<<",";
	ofs << std::endl;
	ofs << "cb=";
	_WriteToStream(ofs, m_bor, 0, accounted_points_number);
	ofs << "IXE=";
	_WriteToStream(ofs, m_xe_flag, 0, accounted_points_number);
	ofs << "ISM=";
	_WriteToStream(ofs, m_sm_flag, 0, accounted_points_number);
	ofs << "IDOPL=";
	_WriteToStream(ofs, m_dopler_flag, 0, accounted_points_number);
	ofs << "tbx=";
	_WriteToStream(ofs, m_itemp, 0, accounted_points_number);
	ofs << "gbx=";
	_WriteToStream(ofs, m_igam, 0, accounted_points_number);
	ofs << "wud=";
	_WriteToStream(ofs, m_wud, 0, accounted_points_number);
	ofs << "TM=";
	_Comp_WriteToStream(ofs, m_temp, 0, accounted_points_number);
	ofs << "GAM=";
	_Comp_WriteToStream(ofs, m_gam, 0, accounted_points_number);
	ofs << "bz2=";
	_Comp_WriteToStream(ofs, m_backl, 0, accounted_points_number);
	ofs << "mapn=";
	// WRITE RIGHT ALGORITHM
	for (int i = 1; i <= _fa_count; i++)
		ofs << i << ",";
	ofs << "\n";
	PermutationForming(ofs);
	ofs << "&end" << std::endl;
	ofs.close();
}

bool Core::CheckInputDataStates()
{
	if (m_time_points.size() % accounted_points_number == 0 &&
		m_temp.size() % accounted_points_number == 0 &&
		m_gam.size() % accounted_points_number == 0 &&
		m_backl.size() % accounted_points_number == 0 &&
		m_bor.size() % accounted_points_number == 0 &&
		m_itemp.size() % accounted_points_number == 0 &&
		m_igam.size() % accounted_points_number == 0 &&
		m_xe_flag.size() % accounted_points_number == 0 &&
		m_sm_flag.size() % accounted_points_number == 0 &&
		m_dopler_flag.size() % accounted_points_number == 0 &&
		m_wud.size() % accounted_points_number == 0
		)
		return true;

	std::cerr << "Missed some states. Calculation abouted. Check engine.\n";
	return false;

}

void Core::PermutationForming(std::ostream& ofs)
{
	// There insert MAPN_ADD && MAPN_CHNG
	ofs << "MAPN_ADD=\n";
	int reload = 0;
	//for (int i = 0; i < m_states_number; i++)
	/*for (int i = 0; i < accounted_points_number; i++)
	{
		for (const auto & assembly : _fuelAssemblies)
		{
			ofs << assembly.GetPermparNumber(parsed_times[i]);
			ofs << ",";
		}
		ofs << "\n";
	}*/
	for (int i = _fa_count + 1; i <= _fa_count * permak_max_states_quantity; i++)
	{
		ofs << i << ",";
		if (((i+1 - _fa_count) % 21) == 0) ofs << "\n";
		
	}

	ofs << "\nNM_CHNG=\n";
	reload = 0;
	//for (int i = 0; i < m_states_number; i++)
	for (int i = 0; i < accounted_points_number; i++)
	{
		for (const auto& assembly : _fuelAssemblies)
		{
			ofs << assembly.GetPermparNumber(parsed_times[i]);
			ofs << ",";
		}
		ofs << "\n";
	}
}


template<typename K>
void Core::_WriteToStream(std::ostream& ofs, const K& inputCollection, unsigned int _init_pos, unsigned int _end_pos)
{
	for (auto i = _init_pos; i < _end_pos; i++)
	{
		ofs << inputCollection[i] << ",";
	}
	ofs << std::endl;

}

template<typename K>
void Core::_Comp_WriteToStream(std::ostream& ofs, const K& inputCollection, unsigned int _init_pos, unsigned int _end_pos)
{

	for (auto i = _init_pos; i < _end_pos; i++)
	{
		_WriteToStream(ofs, inputCollection[i], 0, _fa_count);
		//	ofs << std::endl;
	}
	ofs << std::endl;

}
