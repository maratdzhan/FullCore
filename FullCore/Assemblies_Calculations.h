#pragma once


void Core::LoadingAssemblies()
{
	std::cerr << "14. Start work with assemblies ..\n";
	_fuelAssemblies.resize(_fa_count);
	fa_library_file = _c_library + "Coordinates_Definition.dll";
	m_coreCoordinates.AddLibrary(fa_library_file); // Path to library
	m_coreCoordinates.SetSize(_fa_count); // Assemblies quantity
	m_coreCoordinates.GetTvsCoordinates(_tvs_step, std::stoi(arguments.ExtractParameter("DEBUG"))); // Get project assemblies x,y-position
	std::vector<std::pair<double, double>> coordinates = m_coreCoordinates.V_ReturnCoordinatesTvs(); // Transfer coordinates. WHY?
	std::cerr << "OK\n";

	try {
		//// PROJECT
		AssembliesInitialize(coordinates);
		CycleSetNeigsForTvs(); //Set neigs
		//// FACT
		AssembliesShiftings(coordinates);
		CycleSetPlaneGapsForTvs();
		CycleSetCornerGapsForTvs();

	}
	catch (std::exception& lfa)
	{
		std::cerr << __FUNCTION__ << " " << lfa.what() << std::endl;
	}
}

void Core::AssembliesInitialize(std::vector<std::pair<double, double>> & coordinates)
{
	int num_e = 0;
	try {

		m_coreCoordinates.NeigArrayInitializing(); // Just initializing with values, without assigning

		for (size_t num = 0; num < _fa_count; num++) {
			//
			num_e = (int)num;
			_fuelAssemblies[num].SetTimePointsQuantity(permak_max_states_quantity); // !!! NEW
			_fuelAssemblies[num].InitializeParameters(_tvs_size, _tvs_step, num); // Init tvs-size, tvs-step, tvs-number
			_fuelAssemblies[num].SetProjectCoordinates(coordinates[num].first, coordinates[num].second); // set project coord

			for (size_t _tp = 0; _tp < (size_t)permak_max_states_quantity; _tp++)
				_fuelAssemblies[num].SetPermparNumber(DefinePermparNumber(_fuelAssemblies[num].GetTvsNumber(), (int)_tp), _tp);
		}
	}
	catch (std::exception& exc)
	{
		std::cerr << exc.what() << " at " << num_e << std::endl;
		system("pause");
	}
}

void Core::CycleSetNeigsForTvs()
{
	for (auto & tvs : _fuelAssemblies)
	{
		tvs.SetNeigs(m_coreCoordinates.Neig_Array(tvs.GetTvsNumber()));
	}
}

void Core::AssembliesShiftings(std::vector<std::pair<double, double>>& coordinates)
{
	size_t _state = 0;
	for (int i = 0; i < (int)first_coordinate.size(); ++i)
	{
		if (_coordinate_system == 1)
			_fuelAssemblies[i % _fa_count].SetCurrentCoordinates(first_coordinate[i], second_coordinate[i], _state);
		else if (_coordinate_system == 2)
			_fuelAssemblies[i % _fa_count].SetCurrentCoordinates(coordinates[i % _fa_count].first + first_coordinate[i], coordinates[i % _fa_count].second + second_coordinate[i], _state);
		if ((i % _fa_count) + 1 == _fa_count) 
			_state++;
	}
}

void Core::CycleSetPlaneGapsForTvs()
{
	for (size_t _tp = 0; _tp < permak_max_states_quantity; _tp++) {
		for (auto& tvs : _fuelAssemblies)
		{
			SetPlaneGapsForTvs(tvs, _tp);
		}
	}
}

void Core::SetPlaneGapsForTvs(Assembly& tvs, const size_t /*_time_point*/ _state)
{
	double _x = 0;
	double _y = 0;
	int _neig = -1;
	bool isRef = false;
	double _gapSize = -999;

	for (auto side = 0; side < geometry; side++)
	{
		_gapSize = -999;
		// Х: Current Х of assembly center - (define neighbor (GetNeig()) and get his current x of center)
		_neig = tvs.GetNeig(side);
		if (_neig > 0) {
			_neig -= 1;
			_x = tvs.GetCurrentX(_state) - _fuelAssemblies[_neig].GetCurrentX(_state);
			_y = tvs.GetCurrentY(_state) - _fuelAssemblies[_neig].GetCurrentY(_state);
			_gapSize = sqrt(_x * _x + _y * _y) - _tvs_step;
		}
		else
		{
			double angle = M_PI * ((180 - side * 60) % 360) / 180.;
			// THE FORMULA IS TRUE. FOR ALL EDGES AND ALL VARIANTS OF RADIUS-VECTORS. 10.02.2020 // Vasilyev M. N.
			_gapSize = reflectorDistance - tvs.GetShiftX(_state) * cos(angle)
				- tvs.GetShiftY(_state) * sin(angle) - nominalGapSize;
			isRef = true;
		}

		if (_gapSize < -10)
		{
			std::cerr << __FUNCTION__ << " ";
			std::cerr << "WRONG GAP SIZE! -> " << _gapSize << " at " << tvs.GetTvsNumber() << std::endl;
			system("pause");
			return;
		}
		std::pair < size_t, double > t = Rounding(_gapSize);
		tvs.SetPlaneGapSize(side, t.second, _state);
		tvs.SetPlaneConstants(side, _gapSizePlaneConstant[t.first], _state);

	}

}

void Core::CycleSetCornerGapsForTvs()
{
	for (size_t _tp = 0; _tp < permak_max_states_quantity; _tp++) {
		for (auto& tvs : _fuelAssemblies)
		{
			SetCornerGapsForTvs(tvs, _tp);
		}
	}
}

void Core::SetCornerGapsForTvs(Assembly &tvs, const size_t _states)
{
	double _result = 0;
	for (size_t side = 0; side < (size_t)geometry; side++)
	{
		_result = -999;
		/*
		First: Direct edge gap
		Second: Gap at previous edge
		Third: Gap at
		*/
		int _neig = -999;
		double _first = tvs.GetPlaneGapSize(side, _states);
		double _second = tvs.GetPlaneGapSize((side - 1 + geometry) % geometry, _states);
		double _third = 0;
		_neig = tvs.GetNeig((side - 1 + geometry) % geometry) - 1;
		if (_neig >= 0)
		{
			_third = _fuelAssemblies[_neig].GetPlaneGapSize((side + 1) % geometry, _states);
		}
		else
		{
			_neig = tvs.GetNeig(side) - 1;
			if (_neig >= 0)
			{
				_third = _fuelAssemblies[_neig].GetPlaneGapSize((side + 1 + geometry / 2) % geometry, _states);
			}
			else
			{
				_third = reflectorDistance - nominalGapSize;
			}
		}


		_result = (_first + _second + _third) / 3.;
		std::pair < size_t, double > t = Rounding(_result);
		tvs.SetCornerConstants(side, _gapSizeCornerConstant[t.first], _states);
		
	}
}

std::pair<size_t, double> Core::Rounding(double _gs) const
{
	size_t i = 0;
	size_t j = _gapSize.size();
	size_t k = 0;
	int itt = 0;
	int gsz = (int)(_gs * 100 - 2);
	_gs = (gsz / 100.);
	if (_gs >= _gapSize[_gapSize.size() - 1])
		return { _gapSize.size() - 2, _gapSize[_gapSize.size() - 2] };
	while (j - i >= 2)
	{
		k = (j + i) / 2;

		if (_gs >= _gapSize[k])
			i = k;
		else
			j = k;

		itt++;
		if (itt > 1000)
		{
			throw (std::out_of_range("Can't find match in ROUNDING()\n"));
			break;
		}
	}

	return { j ,_gapSize[j] };

}

VS Core::CyclingConstantFinding(const VS& _id)
{
	VS result;
	for (const auto& item : _id)
	{
		result.push_back(std::to_string(FindTheConstant(item)));
	}

	return result;
}

int Core::FindTheConstant(const std::string& _id) const
{
	int _index = CHT.Get(_id);
	if (_index > 0)
		return _index;

	std::cerr << "Exception -> " << __FUNCTION__ << std::endl;
	if (_id.empty())
		throw (std::out_of_range("Empty constant "));
	else
		throw (std::out_of_range("Constant not found " + _id + " "));
	return 0;
}

void Core::SetAssemblyGapsFinal()
{	
//	CycleSetPlaneGapsForTvs(); // Set plane constants for all states (time_points)
//	CycleSetCornerGapsForTvs(); // Set corner constants for all states (time_point)
	for (auto& tvs : _fuelAssemblies)
		SetAdditionalAttributes(tvs);
	std::cerr << "Additional attributes has been set\n";
	
	for (auto& tvs : _fuelAssemblies)
		SetEnrichment(tvs);
	std::cerr << "Enrichment has been set\n";

	if (isModifierAccounted)
		std::cerr << "Modifiers will be accounted\n";
	int ls = 0, ltvs = 0, v=-1;
	try {
		for (size_t _state = 0; _state < permak_max_states_quantity; _state++) {
			double bor = m_bor[GetStateFromTime(reloads[_state])];
			ls = _state;
			v = 0;
			for (auto& tvs : _fuelAssemblies)
			{
				ltvs = tvs.GetTvsNumber();
				tvs.SetBoric(bor, (int)_state);
				v=1;
				RebuildGaps(tvs, (int)_state);
				v=2;
				BuildMapkas(tvs, (int)_state);
				v=3;
				SetNalArrays(tvs, (int)_state);
				v=4;
			}
		}
	}
	catch (std::exception quote)
	{
		std::cerr << ">>>>>> ERROR FOUND!!! <<<<<<\n";
		std::cerr << "error at state " << ls << " tvs " << ltvs << " at program " << v << std::endl;
		system("pause");
	}

 }

void Core::SetAdditionalAttributes(Assembly& tvs)
{
	int type = _mapn.at(tvs.GetTvsNumber());
	
	for (const auto& mapk : mapkas)
	{
		// THERE SEARCHING FROM <ASSEMBLIES_ARRAY_FORMING>
		if (mapk[0] == type) {
			// SET MAPK
			tvs.SetMapk(mapk[0]);
			tvs.SetFuelTypeNumber(mapk[3]);
		}
	}
}

void Core::RebuildGaps(Assembly& tvs, const size_t _state)
{
	int _neig = -1;
	bool isRef = false;

	for (auto _side = 0; _side < geometry; _side++)
	{

		if (isModifierAccounted) {
			tvs.SetPlaneGapSize(_side, SetCorrection(tvs.GetBoric((int)_state), 0.7321, tvs.GetRo5(_side), tvs.GetPlaneGapSize(_side, _state)), _state);
			tvs.SetCornerGapSize(_side, SetCorrection(tvs.GetBoric((int)_state), 0.7321, tvs.GetRo5(_side), tvs.GetCornerGapSize(_side, _state)), _state);
			// Проблема в том, что в выбранной ТВС меняется зазор. Когда мы доходим до противоположной ТВС с таким же зазором он может иметь другую 
			// ведичину (другое обогащение кассеты) - раньше брали как среднее трех кассет - можно брать так же и сейчас
		}

		std::pair < size_t, double > t = Rounding(tvs.GetPlaneGapSize(_side, _state));
		tvs.SetPlaneGapSize(_side, t.second, _state);
		tvs.SetPlaneConstants(_side, _gapSizePlaneConstant[t.first], _state);
	}
}

void Core::BuildMapkas(Assembly& tvs, const size_t _state)
{

	size_t _num = tvs.GetTvsNumber();
	int type = _mapn.at(_num);
	size_t _size = 0;
	std::string _assemblyInfo;


	for (const auto& mapk : mapkas)
	{
		if (mapk[0] == type) {
			_assemblyInfo += std::to_string(tvs.GetPermparNumber(_state)) + ",";
			FromNumericalVectorToString(_assemblyInfo, mapk, ',', 2, -1);

			VS p_value = CyclingConstantFinding(tvs.GetPlaneConstants(_state));
			FromStringVectorToString(_assemblyInfo, p_value, ',', 0, -1);


			VS c_value = CyclingConstantFinding(tvs.GetCornerConstants(_state));
			FromStringVectorToString(_assemblyInfo, c_value, ',', 0, -1);


			_size = 100 - (mapk.size() + tvs.GetCornerConstants(_state).size()
				+ tvs.GetPlaneConstants(_state).size());
			_assemblyInfo += std::to_string(_size + 1) + "*0,";
			_mapkasArray.push_back(_assemblyInfo);
			_assemblyInfo.clear();
			break;
		}
	}
}

void Core::SetEnrichment(Assembly& tvs)
{
	// For each side
	// -We look for a neig:
	// -if neig exists
	// --look for neig ro5;
	// --if neig ro5 exists
	// ---use neig ro5 for current tvs
	// -- else calculate ro5
	// -else use full ro5
	//
	int sd = -1;
	try {
		for (int _side = 0; _side < geometry; _side++)
		{
			sd = _side;
			int _first_neig = tvs.GetNeig(_side)-1;
			int neig_side = (_side + (geometry / 2)) % geometry;
			if (_first_neig >= 0)
			{
				if (_fuelAssemblies[_first_neig].GetRo5(neig_side) != 0)
				{
					tvs.SetRo5(_fuelAssemblies[_first_neig].GetRo5((_side + (geometry / 2)) % geometry), _side);
				}
				else
				{
					// plane
					double nro5 = ExtractRo5(_fuelAssemblies[_first_neig].GetFuelTypeNumber());
					double cro5 = ExtractRo5(tvs.GetFuelTypeNumber());
					tvs.SetRo5((nro5 + cro5) / 2., _side);
					// corner
					int _second_neig = tvs.GetNeig((_side + 1) % (geometry)+1)-1;

					if (_second_neig >= 0) {
						double tro5 = ExtractRo5(_fuelAssemblies[_second_neig].GetFuelTypeNumber());
						double val = (nro5 + cro5 + tro5) / 3;
						tvs.SetCornerRo5(val, _side);
						_fuelAssemblies[_first_neig].SetCornerRo5(val, neig_side);
						_fuelAssemblies[_second_neig].SetCornerRo5(val, ((_side + (geometry)) % geometry - 1));
					}
					else
					{
						double val = (nro5 + cro5) / 3;
						tvs.SetCornerRo5(val, _side);
						_fuelAssemblies[_first_neig].SetCornerRo5(val, neig_side);
					}
				}
			}
			else
			{
				tvs.SetRef(true, _side);
				tvs.SetRo5(ExtractRo5(tvs.GetFuelTypeNumber()), _side);
			}
		}
	}
	catch (std::out_of_range& exc_1)
	{
		std::cerr << exc_1.what() << " " << tvs.GetTvsNumber() << " at side " << sd << std::endl;
		system("pause");
	}
}

void Core::SetNalArrays(Assembly& tvs, const size_t _state)
{

	VS _tvp = tvs.GetPlaneConstants(_state);
	VS _tvc = tvs.GetCornerConstants(_state);
	for (size_t t = 0; t < (size_t)geometry; t++) {
		if (tvs.GetRef((int)t))
		{
			nal2.insert(_tvp[t]);
			nal3.insert(_tvc[t]);
			nal3.insert(_tvc[(t + 1) % geometry]);
		}
	}
}


void Core::SaveGaps(int state_num, const std::string &value)
{
	std::vector<std::vector<int>> accountedArray, neig_array;
	VS content;
	accountedArray.resize(_fa_count);
	neig_array.resize(_fa_count);

	for (int _fa = 0; _fa < _fa_count; _fa++) {
		neig_array[_fa] = m_coreCoordinates.Neig_Array(_fa);
	}
	

	for (int _time_point = 0; _time_point < permak_max_states_quantity; _time_point++)
	{
		for (int _fa = 0; _fa < _fa_count; _fa++)
			accountedArray[_fa] = m_coreCoordinates.Neig_Array(_fa);

		for (int _fa = 0; _fa < _fa_count; _fa++)
		{
			std::string _v;
			for (int _side = 0; _side < geometry; _side++)
			{
				///## Wanted to save all gaps, that why comment as ///##
				///##if (accountedArray[_fa][_side] != -2)
				///##{
					_v += std::to_string(state_num);
					_v += ",";
					_v += std::to_string(_time_point);
					_v += ",";
					_v += std::to_string(_fa + 1);
					_v += ",";
					_v += std::to_string(_side + 1);
					_v += ",";
					_v += std::to_string(_fuelAssemblies[_fa].GetPlaneGapSize(_side, _time_point));
				///##	if (accountedArray[_fa][_side]>=0)
				///##		accountedArray[neig_array[_fa][_side] - 1][(_side + (geometry / 2)) % (geometry)] = -2;
				///##	accountedArray[_fa][_side] = -2;
					content.push_back(_v);
				///##}
				_v.clear();
			}
		}
	}

	std::string save_path = "res";
	save_path = p_workdirectory + value + "/gap_size.txt";
	std::ofstream ofs(save_path, std::ios_base::app);
	ofs << "state,time_point,fa,side,gap_size\n";
	for (const auto& line : content)
	{
		ofs << line<<"\n";
	}
	std::cerr << "Gaps saved!\n";
	ofs.close();
}