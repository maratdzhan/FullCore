#pragma once

void Core::NewdataMaking()
{
	NewdataPS();
	GetBurnupHistory();
}

void Core::NewdataPS()
{
	for (const auto& item : newdata_parameters)
	{
		newdataParameters _name = file.GetNewdataType(GetStringParam(item, 1));
		switch (_name)
		{
		case (xe_flag):
			GetParam(m_xe_flag, item, 2);
			break;
		case (sm_flag):
			GetParam(m_sm_flag, item, 2);
			break;
		case (dopler_flag):
			GetParam(m_dopler_flag, item, 2);
			break;
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
			break;
		case (itemp):
			GetPoints(item, m_itemp,0);
			break;
		case (igam):
			GetPoints(item, m_igam,0);
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
	while (lastVal!=-1)
	{
		inputVector.push_back(-1);
		GetParam(inputVector[inputVector.size() - 1], inputString, ++pos);
		lastVal = static_cast<int>(inputVector[inputVector.size()-1]);
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
			/*			std::vector<double> tail;		// Greater and equal? Write to outputVector
						if (temporary.size() > _fa_count) // If greater - cute the tail
						{								// Write tail

							for (int i = _fa_count; i < temporary.size(); ++i)
								tail.push_back(temporary[i]);
							temporary.resize(_fa_count);// Change size of array
						}
			*/
			outputVector.push_back(temporary);
			temporary.clear();
			/*			if (tail.size() != 0) {
							for (const auto& item : tail)
								temporary.push_back(item);
						}
			*/
		}
	}
}

void Core::GetBurnupHistory()
{
	GetDensity();
	GetTemperatures();
	GetSpecificPower();
	GetBoricAcid();
}

void Core::GetDensity()
{
	VS densityArray = file.GetLine(m_Compilation.GetFileByName("GAM.CON"));
	CoolantParsing(densityArray, m_gam);
	std::cerr << "Amount Density states : " << m_gam.size() << std::endl;
//	DebugWriteToFile(m_gam);
}

void Core::GetTemperatures()
{
	VS temperatureArray = file.GetLine(m_Compilation.GetFileByName("TEMP.CON"));
	CoolantParsing(temperatureArray, m_temp);
	std::cerr << "Amount Temperature states: " << m_temp.size() << std::endl;
//	DebugWriteToFile(m_temp);
}

void Core::GetSpecificPower()
{
	VS wudArray = file.GetLine(m_Compilation.GetFileByName("WUD.CON"));
	for (const auto& _item : wudArray) {
		GetPoints(_item, m_wud, 0);
	}
	std::cerr << "Amount WUD states : " << m_wud.size() << std::endl;
}

void Core::GetBoricAcid()
{
	VS boracicAcidArray = file.GetLine(m_Compilation.GetFileByName("BOR.CON"));
	for (const auto& _item : boracicAcidArray) {
		GetPoints(_item, m_bor, 0);
	}
	std::cerr << "Amount Boric Acid states : " << m_gam.size() << std::endl;
}