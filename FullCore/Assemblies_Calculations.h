#pragma once


void Core::CycleSetPlaneGapsForTvs()
{
	for (auto & tvs : _fuelAssemblies)
	{
		SetGapsForTvs(tvs);
	}
}

void Core::CycleSetCornerGapsForTvs()
{
	for (auto & tvs : _fuelAssemblies)
	{
		SetCornerGapsForTvs(tvs);
	}
}

void Core::CycleSetNeigsForTvs()
{
	for (auto & tvs : _fuelAssemblies)
	{
		tvs.SetNeigs(m_coreCoordinates.Neig_Array(tvs.GetTvsNumber()));
	}
}

void Core::LoadingAssemblies()
{
	_fuelAssemblies.resize(_fa_count);
	double x = 0;
	double y = 0;
	fa_library_file = _c_library + "Coordinates_Definition.dll";
	m_coreCoordinates.AddLibrary(fa_library_file);
	m_coreCoordinates.SetSize(_fa_count);
	m_coreCoordinates.GetTvsCoordinates(_tvs_step);
	m_coreCoordinates.NeigArrayInitializing();
	std::vector<std::pair<double, double>> coordinates = m_coreCoordinates.V_ReturnCoordinatesTvs();
	// Ключ для данных тутнова;

	for (size_t i = 0; i < _fa_count; ++i)
	{
		x = coordinates[i].first;
		y = coordinates[i].second;
		_fuelAssemblies[i].Initialize(_tvs_size, _tvs_step, i);
		_fuelAssemblies[i].SetProjectCoordinates(x, y);
		// here we must send coordinates and type of shift
		_fuelAssemblies[i].SetCurrentCoordinates(first_coodinate[i], second_coordinate[i]);
	}

	std::vector<int> neigs(geometry);

	CycleSetNeigsForTvs();
	CycleSetPlaneGapsForTvs();
	CycleSetCornerGapsForTvs();
}

std::pair<size_t, double> Core::Rounding(double _gs) const
{
	int i = 0;
	int j = _gapSize.size();
	int k = 0;
	int itt = 0;
	_gs *= 100;
	static_cast<int>(_gs);
	_gs = (_gs - 0.02) / 100.;

	while (j - i >= 2)
	{
		k = (j + i) / 2;
		if (_gs >= _gapSize[k])
		{
			i = k;
		}
		else
		{
			j = k;
		}
		itt++;
		if (itt > 1000)
			throw (std::out_of_range("Can't find mathc in ROUNDING()\n"));

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

void Core::SetCornerGapsForTvs(Assembly &tvs)
{
	double _result = 0;
	bool isRef = false;
	//		std::cerr << tvs.GetTvsNumber() + 1 << ". ";;
	for (size_t side = 0; side < static_cast<size_t>(geometry); side++)
	{
		_result = -999;
		/*
		First: Direct edge gap
		Second: Gap at previous edge
		Third: Gap at
		*/
		int _neig = -999;
		double _first = tvs.GetGapSize(side);
		double _second = tvs.GetGapSize((side - 1 + geometry) % geometry);
		double _third = 0;
		_neig = tvs.GetNeig((side - 1 + geometry) % geometry) - 1;
		if (_neig >= 0)
		{
			_third = _fuelAssemblies[_neig].GetGapSize((side + 1) % geometry);

		}
		else
		{
			_neig = tvs.GetNeig(side) - 1;
			if (_neig >= 0)
			{
				_third = _fuelAssemblies[_neig].GetGapSize((side + 1 + geometry / 2) % geometry);
			}
			else
			{
				_third = reflectorDistance - nominalGapSize;
			}
			isRef = true;
		}


		_result = (_first + _second + _third) / 3.;
		std::pair < size_t, double > t = Rounding(_result);
		tvs.SetCornerConstants(side, _gapSizeCornerConstant[t.first]);
		tvs.SetCornerGapSize(side, t.second);
		//			std::cerr << _gapSizeCornerConstant[t.first] << " ";
		if (isRef)
			nal3.insert(_gapSizeCornerConstant[t.first]);
	}
	//		std::cerr << std::endl;
}

void Core::SetGapsForTvs(Assembly &tvs)
{
	double _x = 0;
	double _y = 0;
	int _neig = -1;
	bool isRef = false;
	double _gapSize = -999;

	// /**/		std::cerr << tvs.GetTvsNumber() << ". ";

	for (auto side = 0; side < geometry; side++)
	{
		try {
			_gapSize = -999;
			// Х: Текущее Х центра кассета - (определяем соседа (GetNeig()) и для него получаем текущий Х)
			_neig = tvs.GetNeig(side);
			if (_neig > 0) {
				_neig -= 1;
				_x = tvs.GetCurrentX() - _fuelAssemblies[_neig].GetCurrentX();
				_y = tvs.GetCurrentY() - _fuelAssemblies[_neig].GetCurrentY();
				_gapSize = sqrt(_x*_x + _y * _y) - _tvs_step;
			}
			else
			{
				double angle = M_PI * ((180 - side * 60) % 360) / 180.;
				//					double debug_1 = tvs.GetShiftX()*cos(angle);
				//					debug_1 = tvs.GetShiftY()*sin(angle);
				_gapSize = reflectorDistance - tvs.GetShiftX()*cos(angle) - tvs.GetShiftY()*sin(angle) - nominalGapSize;
				isRef = true;
			}

			//		if (isModifierAccounted)
			//			SetCorrection(cb, gam, ro5, gs);
//				std::cerr << _gapSize << std::endl;
			std::pair < size_t, double > t = Rounding(_gapSize);
			tvs.SetGapSize(side, t.second);
			tvs.SetPlaneConstants(side, _gapSizePlaneConstant[t.first]);
			if (isRef)
				nal2.insert(_gapSizePlaneConstant[t.first]);

			// /**/				std::cerr << _gapSizePlaneConstant[t.first] << " ";

		}
		catch (std::exception & e_c)
		{
			std::cerr << e_c.what() << " at " << __FUNCTION__ << std::endl;
		}
	}
	// /**/		std::cerr << std::endl;
}
