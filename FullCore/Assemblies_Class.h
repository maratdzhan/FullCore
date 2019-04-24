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
		_current_coordinate_x = -10000;
		_current_coordinate_y = -10000;
		step = 0;
		fa_size = 0;
		geometry = 6;
		gap_size.resize(geometry);
		corner_gap_size.resize(geometry);
		neigs.resize(geometry);
		c_consts.resize(geometry);
		p_consts.resize(geometry);
		shift_x = 0;
		shift_y = 0;


	}

	void Initialize(double tvs_size, 
		double tvs_step, size_t tvs_number)
	{

		step = tvs_step;
		fa_size = tvs_size;
		_tvs_number = tvs_number;
		double x = 0, y = 0;


	}

	void SetPlaneConstants(size_t _side, const std::string & _p)
	{
		p_consts[_side] = _p;
	}

	std::vector<std::string> GetPlaneConstants() const
	{
		return p_consts;
	}

	std::vector<std::string> GetCornerConstants() const
	{
		return c_consts;
	}

	void SetShift()
	{
		shift_x = _current_coordinate_x - _project_coordinate_x;
		shift_y = _current_coordinate_y - _project_coordinate_y;
	}

	double GetShiftX()
	{
		return shift_x;
	}

	double GetShiftY()
	{
		return shift_y;
	}

	double GetCurrentX() const
	{
		return _current_coordinate_x;
	}

	double GetCurrentY() const
	{
		return _current_coordinate_y;
	}

	double GetGapSize(size_t _side) const
	{
		return gap_size[_side];
	}

	void SetCornerGapSize(size_t _side, double _gap_size)
	{
		corner_gap_size[_side] = _gap_size;
	}

	void SetCornerConstants(size_t _side, const std::string _c)
	{
		c_consts[_side] = _c;
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

	void SetCurrentCoordinates(double _x, double _y)
	{
		_current_coordinate_x = _x;
		_current_coordinate_y = _y;

		SetShift();
	}

	void SetGapSize(size_t _side, double _gapSize)
	{
		gap_size[_side] = _gapSize;
	}

	void SetNeigs(const std::vector<int> & _neigs)
	{
		neigs = _neigs;
	}

	int GetNeig(int _side)
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
	double _current_coordinate_x;
	double _current_coordinate_y;
	size_t _tvs_number, geometry;
	std::vector<double> gap_size;
	std::vector<double> corner_gap_size;
	std::vector<int> neigs;
	double shift_x;
	double shift_y;
	std::vector<std::string> c_consts, p_consts;
	int mapk;
};