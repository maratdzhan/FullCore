#pragma once


void Core::CreatePermparFile()
{
	std::string unit_word = "B";
	unit_word += (unit_number < 10 ? ("0" + std::to_string(unit_number)) : (std::to_string(unit_number)));
	permpar_path = p_workdirectory + p_project_name + char(92) + unit_word + char(92) + "permpar";
	std::ofstream ofs(permpar_path);
	if (!ofs.is_open())
	{
		std::cerr << __FUNCTION__;
		throw (std::exception("--> CAN'T CREATE PERMPAR\n"));
	}
	ofs.close();
}

void Core::PermparMaking()
{
	try {
		for (auto & _i : constants)
		{
			ToUpperFunct(_i);
		}
		for (auto & _s : permpar)
		{
			ToUpperFunct(_s);
			PermparSP(_s);
		}
		WriteToPermpar();
	}
	catch (std::exception & exc_PPm)
	{
		noErrors = 0;
		std::cerr << __FUNCTION__ << " error: " << exc_PPm.what();
	}

}

void Core::PermparSP(const std::string & _s)
{
	// SP = String Parser
	std::string _cs = _s;
	int _case = TouchCase(_cs);

	switch (_case) {
	case (1):
		AssembliesArrayForming();
		break;
	case(2):
		ConstantsForming();
		break;
	case(3):
		LibraryIncluding();
		break;
	case(4):
		NalArraysForming();
		break;
	case(0):
	default:
		toPermpar.push_back(_cs);
		break;
		/*
		std::cerr << "unknown function argument <" << _case
			<< "> at TouchCase -> exception at " << __FUNCTION__ << std::endl;
			*/
	}

}

int Core::TouchCase(const std::string & _str) const
{
	std::string _v = _str;
	ToUpperFunct(_v);

	if (_str.find("ASSEMBLIES") != -1)
		return 1;
	if (_str.find("CONSTANTS") != -1)
		return 2;
	if (_str.find("LIBRARY") != -1)
		return 3;
	if (_str.find("NARRAYS") != -1)
		return 4;

	return 0;
}

void Core::WriteToPermpar()
{
	std::ofstream ofs(permpar_path);

	if (!ofs.is_open())
	{
		std::cerr << __FUNCTION__;
		throw (std::invalid_argument(" --> PERMPAR IS NOT OPENED FOR RECORDING\n"));
	}
	for (const auto & item : toPermpar)
		ofs << item << std::endl;
	// 3. Write items to permpar file

	ofs.close();
}

void Core::NalArraysForming()
{
	// 1. Generate for NAL2 arrays
	Nal2Generating();
	// 2. Generate for NAL3 arrays
	Nal3Generating();
	// 3. Call function: write to "toPermpar" array

}

void Core::Nal2Generating()
{
	std::string _fs;
	std::string _c = ",";
	toPermpar.push_back("NAL2=");
	for (const auto & _const : nal2)
	{
		for (const auto & _ref : nal2_r_array)
		{
			std::string  const_id = std::to_string(FindTheConstant(_const));
			_fs = const_id + _c + _ref + _c + const_id + _c;
			toPermpar.push_back(_fs);
			_fs.clear();
		}
	}
	for (const auto & _item : nal2array)
	{
		toPermpar.push_back(_item);
	}
}

void Core::Nal3Generating()
{
	std::string _fs;
	std::string _c = ",";
	toPermpar.push_back("NAL3=");
	for (const auto & _const : nal3)
	{
		for (const auto & _ref : nal3_r_array)
		{
			std::string  const_id = std::to_string(FindTheConstant(_const));
			_fs = const_id + _c + _ref + _c + _ref + _c + const_id + _c;
			toPermpar.push_back(_fs);
			_fs = const_id + _c + const_id + _c + _ref + _c + const_id + _c;
			toPermpar.push_back(_fs);
			_fs.clear();
		}
	}

	for (const auto & _item : nal3array)
	{
		toPermpar.push_back(_item);
	}
}

void Core::AssembliesArrayForming()
{
	std::string _assemblyInfo;
	for (size_t _tp = 0; _tp < accounted_points_number; _tp++) {
		for (auto assembly : _fuelAssemblies)
		{
			size_t _num = assembly.GetTvsNumber();
			int type = _mapn.at(_num);
			size_t _size = 0;
			for (const auto& mapk : mapkas)
			{
				// Search similar mapk[] type
				if (mapk[0] == type)
				{
		//			assembly.SetPermparNumber(DefineNumber(_num, _tp), _tp);
					_assemblyInfo += std::to_string(assembly.GetPermparNumber(_tp)) + ",";
					FromNumericalVectorToString(_assemblyInfo, mapk, ',', 2, -1);


					VS p_value = CyclingConstantFinding(assembly.GetPlaneConstants(_tp));
					FromStringVectorToString(_assemblyInfo, p_value, ',', 0, -1);
					VS c_value = CyclingConstantFinding(assembly.GetCornerConstants(_tp));
					FromStringVectorToString(_assemblyInfo, c_value, ',', 0, -1);
					// Calculate like (89*0):
					// Size of planesConstants (6) + cornerConstants (6) +
					// + [mapkas.size()-1] + 1 (number of fa)
					_size = 100 - (mapk.size() + assembly.GetCornerConstants(_tp).size()
						+ assembly.GetPlaneConstants(_tp).size());
					_assemblyInfo += std::to_string(_size + 1) + "*0,";
					toPermpar.push_back(_assemblyInfo);
					_assemblyInfo.clear();
					// DEBUG
//					if (_tp > 0 && (toPermpar.at(toPermpar.size() - 1) != toPermpar.at(toPermpar.size() - 164)))
//						std::cerr << toPermpar.at(toPermpar.size() - 1) << " !=\n " << toPermpar.at(toPermpar.size() - 164) << std::endl << std::endl;
					//
					break;
				}
			}
		}
	}
}

void Core::ConstantsForming()
{
	for (const auto & current_library : constants)
		toPermpar.push_back("'" + current_library + "',");
}

void Core::LibraryIncluding()
{
	std::string _c_lib = _pp_library;
	PathPreparing(_c_lib, 1);
	toPermpar.push_back("NAMLIB='" + _c_lib + "',");
}

int Core::DefinePermparNumber(int number, size_t time_point)
{
	int result = 0;
	while (true) {
		result = number % _fa_count + (_fa_count * time_point) + 1 + number_shifting;
		if ((result >= 1000 && result <= 1005) || (result >= 3000 && result <= 3005) || (result >= 5000 && result <= 5005))
			number_shifting++;
		else
			return result;

	}
}