#pragma once
#include <typeinfo>
#include <string>
#include <algorithm>
#include <iostream>
#include <vector>
#include <fstream>

#include <cctype>
#include <functional> 

std::string GetStringParam(const std::string &inputString, size_t number);
std::string ReturnNumbers(std::string str);
void ToUpperFunct(std::string & str);
void PathPreparing(std::string & assumedPath, int _type_of_replacer);
void SymbolsDeleting(std::string & inputString);
double SetCorrection(double cb, double gam, double ro5, double gs);
void FromStringVectorToString(std::string & _outputString, std::vector<std::string> inputVector, char _separator, int start_pos, int end_pos);
void DebugWriteToFile(std::vector<std::vector<double>>input);



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

		if (!resultString.empty())
			_value = static_cast<T>(stod(resultString));
		else
			_value = -1;
		
	}
	catch (std::exception & exc)
	{
		std::string error_text = exc.what();
		error_text+= +" at <" + s + ">";
		error_text += "at pos " + number;
		std::cerr << error_text << std::endl;
	}
}

template<typename K>
void FromNumericalVectorToString(std::string & _ai, std::vector<K> mapk, char _separator, int start_pos, int end_pos)
{
	if (end_pos == -1)
		end_pos = static_cast<int>(mapk.size());

	for (int i = start_pos; i < end_pos; i++) {
		_ai += (std::to_string(mapk[i]));
		_ai += _separator;
	}

}



static inline std::string& rtrim(std::string& s) {

	int k = 0;
	int i = 0;
	while (i < (int)s.size()) {
		if (s[i] == 32)
			s[i] = s[++k];
		else
		{
			i++;
			k++;
		}
		if (k == (int)s.size())
			break;
	}
	s.resize((k < i ? k : i));

	return s;
}

