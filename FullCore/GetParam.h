#pragma once
#include <typeinfo>
#include <string>
#include <algorithm>
#include <iostream>


std::string GetStringParam(const std::string &inputString, size_t number);
std::string ReturnNumbers(std::string str);
void ToUpperFunct(std::string & str);
void PathPreparing(std::string & assumedPath);
void SymbolsDeleting(std::string & inputString);
double SetCorrection(double cb, double gam, double ro5, double gs);

template < typename T >
void GetParam(T & _value, const std::string & s, const size_t number)
{
	std::string resultString;
	size_t currentNumber = 1;
	T result = 0;

	bool breaking = false;
	try {
		for (unsigned int pos = 0; pos < s.size(); pos++)
		{
			if ((s[pos] != ',') && (currentNumber == number) && (s[pos] != 32))
			{
				resultString += s[pos];
			}
			else
			{
				if (!breaking)
				{
					if (currentNumber > number)
						breaking = true;
					if (s[pos] == ',')
						currentNumber++;
				}
				else
					break;
			}
		}

		_value = static_cast<T>(stod(resultString));
		
	}
	catch (std::exception & exc)
	{
		std::cerr << exc.what() << std::endl;
	}
}