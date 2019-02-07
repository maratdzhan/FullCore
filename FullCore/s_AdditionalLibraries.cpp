#include "163CommonHeader.h"

#include <direct.h>

extern Parameters currentProfile;

double AssemblyCoordinates(short tvs, bool x)
{

	_chdir(libs_core_path.c_str());
	HMODULE library_fa = LoadLibrary("Coordinates_Definition");
	double(*pFunction) (double, int, bool);

	if (library_fa)
	{
		(FARPROC &)pFunction = GetProcAddress(library_fa, "cdefine");
		if (pFunction == NULL)
			throw std::invalid_argument("NOT FIND cdefine at Coordinates_definition at ReturnCoordinates function\n");
		else
		{
			//if (x)
			//	return pFunction(stod(currentProfile.Accordance(
			//		"FUEL_ASSEMBLIES_GRID_STEP")), tvs, x);
			//else
			return pFunction(stod(currentProfile.Accordance(
				"FUEL_ASSEMBLIES_GRID_STEP")), tvs, x);
		}
	}
	else
	{
		throw std::invalid_argument("ERROR OPENING LIBRARY => COORDINATES_DEFINITION.dll\n");
	}

	return -100000;
}

double FuelRodCoordinates(short tvel, bool x)
{

	// ѕри загрузке мен€етс€ текуща€ директори€ и библиотека грузитс€ оттуда, где ее нет.


	_chdir(libs_core_path.c_str());

	HMODULE library_fr = LoadLibrary("FR_Coords");
	double(*pFunctionFR) (bool, int);

	if (library_fr)
	{
		(FARPROC &)pFunctionFR = GetProcAddress(library_fr, "RetFrCoord");
		if (pFunctionFR == NULL)
			throw std::invalid_argument("NOT FIND cdefine at Coordinates_definition at ReturnCoordinates function\n");
		else
		{
			if (x)
				return pFunctionFR(true, tvel);
			else
				return pFunctionFR(false, tvel);
		}
	}
	else
	{
		throw std::invalid_argument("ERROR OPENING LIBRARY => COORDINATES_DEFINITION.dll\n");
	}

	return -100000;
}

