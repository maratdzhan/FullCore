#pragma once


class Assembly
{
public:
	Assembly()
	{
		_tvs_number = 0;
		mapk = 0;
		_project_coordinate_x = -10000;
		_project_coordinate_y = -10000;
		step = 0;
		fa_size = 0;
		geometry = 6;
		neigs.resize(geometry);

		size_t init_value = 35;
		_current_coordinate_x.resize(init_value);
		_current_coordinate_y.resize(init_value);
		gap_size.resize(init_value);
		corner_gap_size.resize(init_value);
		c_consts.resize(init_value);
		p_consts.resize(init_value);
		shift_x.resize(init_value);
		shift_y.resize(init_value);

		for (size_t c = 0; c < init_value; c++)
		{
			gap_size[c].resize(geometry);
			corner_gap_size[c].resize(geometry);
			c_consts[c].resize(geometry);
			p_consts[c].resize(geometry);
		}

	}

	void Initialize(double tvs_size, 
		double tvs_step, size_t tvs_number)
	{

		step = tvs_step;
		fa_size = tvs_size;
		_tvs_number = tvs_number;
		double x = 0, y = 0;


	}

	void SetPlaneConstants(size_t _side, const std::string & _p, const size_t _time_point)
	{
		p_consts[_time_point][_side] = _p;
	}

	std::vector<std::string> GetPlaneConstants(const size_t _time_point) const
	{
		return p_consts[_time_point];
	}

	std::vector<std::string> GetCornerConstants(const size_t _time_point) const
	{
		return c_consts[_time_point];
	}

	void SetShift(const size_t _time_point)
	{
		shift_x[_time_point] = _current_coordinate_x[_time_point] - _project_coordinate_x;
		shift_y[_time_point] = _current_coordinate_y[_time_point] - _project_coordinate_y;
	}

	double GetShiftX(const size_t _time_point) const
	{
		return shift_x[_time_point];
	}

	double GetShiftY(const size_t _time_point) const
	{
		return shift_y[_time_point];
	}

	double GetCurrentX(const size_t _time_point) const
	{
		return _current_coordinate_x[_time_point];
	}

	double GetCurrentY(const size_t _time_point) const
	{
		return _current_coordinate_y[_time_point];
	}

	double GetGapSize(const size_t _side, const size_t _time_point) const
	{
		return gap_size[_time_point][_side];
	}

	void SetCornerGapSize(const size_t _side, const double _gap_size, const size_t _time_point)
	{
		corner_gap_size[_time_point][_side] = _gap_size;
	}

	void SetCornerConstants(size_t _side, const std::string _c, const size_t _time_point)
	{
		c_consts[_time_point][_side] = _c;
	}

	size_t GetTvsNumber() const
	{
		return _tvs_number;
	}

	void SetProjectCoordinates(double _x, double _y)
	{
		_project_coordinate_x = _x;
		_project_coordinate_y = _y;
	}

	void SetCurrentCoordinates(double _x, double _y, const size_t _time_point)
	{
		_current_coordinate_x[_time_point] = _x;
		_current_coordinate_y[_time_point] = _y;

		SetShift(_time_point);
	}

	void SetGapSize(const size_t _side, const double _gapSize, const size_t _time_point)
	{
		gap_size[_time_point][_side] = _gapSize;
	}

	void SetNeigs(const std::vector<int> & _neigs)
	{
		neigs = _neigs;
	}

	int GetNeig(size_t _side)
	{
		try {
			return neigs.at(_side);
		}
		catch (std::exception & sideExc)
		{
			std::cerr << sideExc.what() << " -> WARNING!!!\n>>>\nset neigh to -1\n>>>\n " << __FUNCTION__ << std::endl;
			return -1;
		}
	}

	void SetMapk(int mapk_num)
	{
		mapk = mapk_num;
	}



private:
	double step, fa_size;
	double _project_coordinate_x;
	double _project_coordinate_y;
	std::vector<double> _current_coordinate_x;
	std::vector<double> _current_coordinate_y;
	size_t _tvs_number, geometry;
	std::vector < std::vector<double>> gap_size;
	std::vector < std::vector<double>> corner_gap_size;
	std::vector<int> neigs;
	std::vector<double> shift_x;
	std::vector<double> shift_y;
	std::vector < std::vector<std::string>> c_consts, p_consts;
	int mapk;
};