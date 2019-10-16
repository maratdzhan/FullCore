#pragma once


void Core::LoadingAssemblies()
{
	_fuelAssemblies.resize(_fa_count);

	double x = 0;
	double y = 0;
	/// Library handling
	fa_library_file = _c_library + "Coordinates_Definition.dll";
	m_coreCoordinates.AddLibrary(fa_library_file); // Path to library
	m_coreCoordinates.SetSize(_fa_count); // Assemblies quantity
	m_coreCoordinates.GetTvsCoordinates(_tvs_step); // Get project assemblies x,y-position
	

	m_coreCoordinates.NeigArrayInitializing(); // Just initializing with values
	std::vector<std::pair<double, double>> coordinates = m_coreCoordinates.V_ReturnCoordinatesTvs(); // Transfer coordinates. WHY?

	try {
		//// PROJECT
		for (size_t num = 0; num < _fa_count; num++) {
			//
			_fuelAssemblies[num].SetTimePointsQuantity(permak_max_states_quantity); // !!! NEW
			x = coordinates[num].first; // init project coord-x
			y = coordinates[num].second; // init project coord-y
			_fuelAssemblies[num].Initialize(_tvs_size, _tvs_step, num); // Init tvs-size, tvs-step, tvs-number
			_fuelAssemblies[num].SetProjectCoordinates(x, y); // set project coord

			for (size_t _tp = 0; _tp < permak_max_states_quantity; _tp++)
				_fuelAssemblies[num].SetPermparNumber(DefinePermparNumber(_fuelAssemblies[num].GetTvsNumber(), _tp), _tp);
	
		}

		//// FACT
		size_t _state = 0;
		for (size_t i = 0; i < first_coodinate.size(); ++i)
		{
			if (_coordinate_system == 1)
				_fuelAssemblies[i % _fa_count].SetCurrentCoordinates(first_coodinate[i], second_coordinate[i], _state);
			else if (_coordinate_system == 2)
				_fuelAssemblies[i % _fa_count].SetCurrentCoordinates(coordinates[i % _fa_count].first - first_coodinate[i], coordinates[i % _fa_count].second - second_coordinate[i], _state);
			if ((i % _fa_count) + 1 == _fa_count) _state++;
		}


		//std::vector<int> neigs(geometry);
		CycleSetNeigsForTvs(); //Set neigs
		CycleSetPlaneGapsForTvs(); // Set plane constants for all states (time_points)
		CycleSetCornerGapsForTvs(); // Set corner constants for all states (time_point)

	}
	catch (std::exception& lfa)
	{
		std::cerr << __FUNCTION__ << " " << lfa.what() << std::endl;
	}
}

void Core::CycleSetPlaneGapsForTvs()
{
	for (size_t _tp = 0; _tp < permak_max_states_quantity; _tp++) {
		for (auto& tvs : _fuelAssemblies)
		{
			SetGapsForTvs(tvs,_tp);
		}
	}
}

void Core::CycleSetCornerGapsForTvs()
{
	//for (size_t _tp = 0; _tp < accounted_points_number; _tp++) {
	for (size_t _tp = 0; _tp < permak_max_states_quantity; _tp++) {
		for (auto& tvs : _fuelAssemblies)
		{
			SetCornerGapsForTvs(tvs,_tp);
		}
	}
}

void Core::CycleSetNeigsForTvs()
{
	for (auto & tvs : _fuelAssemblies)
	{
		tvs.SetNeigs(m_coreCoordinates.Neig_Array(tvs.GetTvsNumber()));
	}
}

std::pair<size_t, double> Core::Rounding(double _gs) const
{
	size_t i = 0;
	size_t j = _gapSize.size();
	size_t k = 0;
	int itt = 0;
	int gsz = _gs * 100-2;
	_gs = (gsz/100.);

	while (j - i >= 2)
	{
		k = (j + i) / 2;

		if (_gs >= _gapSize[k])
			i = k;
		else
			j = k;

		itt++;
		if (itt > 1000)
			throw (std::out_of_range("Can't find match in ROUNDING()\n"));

	}
	return { j ,_gapSize[j] };

}

VS Core::CyclingConstantFinding(const VS & _id)
{
	VS result;
	for (const auto & item : _id)
	{
		result.push_back(std::to_string(FindTheConstant(item)));
	}

	return result;
}

int Core::FindTheConstant(const std::string & _id) const
{
	int _index = CHT.Get(_id);
	if (_index > 0)
		return _index;

	std::cerr << "Exception -> " << __FUNCTION__ << std::endl;
	throw (std::out_of_range("Constant not found " + _id + " "));
	return 0;
}

void Core::SetCornerGapsForTvs(Assembly &tvs, const size_t /*_time_point*/ _states)
{
	double _result = 0;
	bool isRef = false;
	for (size_t side = 0; side < static_cast<size_t>(geometry); side++)
	{
		_result = -999;
		/*
		First: Direct edge gap
		Second: Gap at previous edge
		Third: Gap at
		*/
		int _neig = -999;
		//double _first = tvs.GetGapSize(side,_time_point);
		//double _second = tvs.GetGapSize((side - 1 + geometry) % geometry, _time_point);
		double _first = tvs.GetGapSize(side, _states);
		double _second = tvs.GetGapSize((side - 1 + geometry) % geometry, _states);
		double _third = 0;
		_neig = tvs.GetNeig((side - 1 + geometry) % geometry) - 1;
		if (_neig >= 0)
		{
			//_third = _fuelAssemblies[_neig].GetGapSize((side + 1) % geometry, _time_point);
			_third = _fuelAssemblies[_neig].GetGapSize((side + 1) % geometry, _states);
		}
		else
		{
			_neig = tvs.GetNeig(side) - 1;
			if (_neig >= 0)
			{
				//_third = _fuelAssemblies[_neig].GetGapSize((side + 1 + geometry / 2) % geometry, _time_point);
				_third = _fuelAssemblies[_neig].GetGapSize((side + 1 + geometry / 2) % geometry, _states);
			}
			else
			{
				_third = reflectorDistance - nominalGapSize;
			}
			isRef = true;
		}


		_result = (_first + _second + _third) / 3.;
		std::pair < size_t, double > t = Rounding(_result);
		//tvs.SetCornerConstants(side, _gapSizeCornerConstant[t.first], _time_point);
		//tvs.SetCornerGapSize(side, t.second, _time_point);
		tvs.SetCornerConstants(side, _gapSizeCornerConstant[t.first], _states);
		tvs.SetCornerGapSize(side, t.second, _states);
		if (isRef)
			nal3.insert(_gapSizeCornerConstant[t.first]);
	}
}

void Core::SetGapsForTvs(Assembly &tvs, const size_t /*_time_point*/ _state)
{
	double _x = 0;
	double _y = 0;
	int _neig = -1;
	bool isRef = false;
	double _gapSize = -999;

	for (auto side = 0; side < geometry; side++)
	{
		try {
			_gapSize = -999;
			// Õ: Current Õ of assembly center - (define neighbor (GetNeig()) and get his current x of center)
			_neig = tvs.GetNeig(side);
			if (_neig > 0) {
				_neig -= 1;
				_x = tvs.GetCurrentX(_state) - _fuelAssemblies[_neig].GetCurrentX(_state);
				_y = tvs.GetCurrentY(_state) - _fuelAssemblies[_neig].GetCurrentY(_state);
				_gapSize = sqrt(_x*_x + _y * _y) - _tvs_step;
			}
			else
			{
				double angle = M_PI * ((180 - side * 60) % 360) / 180.;
				_gapSize = reflectorDistance - tvs.GetShiftX(_state) * cos(angle) 
					- tvs.GetShiftY(_state) * sin(angle) - nominalGapSize;
				isRef = true;
			}

	//		if (isModifierAccounted)
	//			SetCorrection(cb, gam, ro5, gs);
			std::pair < size_t, double > t = Rounding(_gapSize);
			tvs.SetGapSize(side, t.second, _state);
			tvs.SetPlaneConstants(side, _gapSizePlaneConstant[t.first], _state);
			if (isRef)
				nal2.insert(_gapSizePlaneConstant[t.first]);

		}
		catch (std::exception & e_c)
		{
			std::cerr << e_c.what() << " at " << __FUNCTION__ << std::endl;
		}
	}

}
