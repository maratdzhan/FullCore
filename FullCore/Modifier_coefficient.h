#pragma once


double Core::SetCorrection(double cb, double gam, double ro5, double gs)
{
	double modifier = 0;

	double _cfBor = 6.54;	//const
	double _cfGam = 0.725;	//const
	double _cfRo5 = 3.4;	//const

							// Поправка до 0
	double cfGs_1 = 0.0122;
	double cfGs_2 = -0.0475;
	double cfGs_3 = 0.1463;

	double cfBor_1 = 0.0037;
	double cfBor_2 = -0.0258;
	double cfBor_3 = 0.0509;

	double cfGam_1 = 0.0037;
	double cfGam_2 = -0.1750;
	double cfGam_3 = 2.3613;

	double cfRo5_1 = -0.0059;
	double cfRo5_2 = 0.0850;
	double cfRo5_3 = -0.3151;

	double cfB_1 = 0.0059;
	double cfB_2 = -0.0025;
	double cfB_3 = 0.9756;



	double m1 = -1000, m2 = -1000, m3 = -1000, m4 = -1000, m5 = -1000;

	//
	m1 = (cfGs_1 * gs * gs + cfGs_2 * gs + cfGs_3) * gs;						// ++
	m2 = (cfBor_1 * gs * gs + cfBor_2 * gs + cfBor_3) * (cb - _cfBor);			// ++
	m3 = (cfGam_1 * gs * gs + cfGam_2 * gs + cfGam_3) * (gam - _cfGam);			// ++
	m4 = (cfRo5_1 * gs * gs + cfRo5_2 * gs + cfRo5_3) * (ro5 - _cfRo5);			// ++
	m5 = (cfB_1 * gs * gs + cfB_2 * gs + cfB_3);								// ++
	//
	modifier = m1 + m2 + m3 + m4 + m5;

	modifier = modifier + gs;

	return modifier;
}


double Core::ExtractRo5(int _const_number)
{
	_const_number -= 1; // permak has another start numeration position
	std::string sres;
	double res = 0;

	try {
		if (constants.size() > (size_t)_const_number)
		{
			for (int i = 0; i < 4; i++)
				if (isdigit(constants[_const_number][i]))
					sres += constants[_const_number][i];
			res = stod(sres) / 100.;
			return res;
		}
		else
		{
			std::string err_msg = std::to_string(_const_number) + "const number not exist";
			std::cerr << err_msg;
			return 0;
		}
	}
	catch (std::exception& er5)
	{
		std::cerr << er5.what() << " " << constants[_const_number] << " from i=0 to 3 -> error\n";
	}
	return -1;
}

