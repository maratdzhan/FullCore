#include "163CommonHeader.h"


template < typename T >
void GetParam(T & _value, const std::string & s, const size_t number)
{
	std::string resultString;
	size_t currentNumber = 1;

	bool breaking = false;
	try {
		for (unsigned int pos = 0; pos < s.size(); pos++)
		{
			if ((s[pos] != ',') && (currentNumber == number)&&(s[pos]!=32))
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

		//		assert(!resultString.empty());
		if (typeid(T) == typeid(int))
		{
			_value = stoi(resultString);
		}
		else if (typeid(T) == typeid(double))
		{
			_value = stod(resultString);
		}
	//	else
	//	{
			//		_value = resultString;
	//	}
	}
	catch (std::exception & exc)
	{
		std::cerr << exc.what() << std::endl;
	}
}

std::string GetStringParam(const std::string &inputString, size_t number)
{
	std::string resultString;
	size_t currentNumber = 1;

	bool breaking = false;
	try {
		for (unsigned int pos = 0; pos < inputString.size(); pos++)
		{
			if ((inputString[pos] != ',') && (currentNumber == number))
			{
				resultString += inputString[pos];
			}
			else
			{
				if (!breaking)
				{
					if (currentNumber > number)
						breaking = true;
					if (inputString[pos] == ',')
						currentNumber++;
				}
				else
					break;
			}
		}

		return resultString;

	}
	catch (std::exception & exc)
	{
		std::cerr << exc.what() << std::endl;
	}
}

std::string ReturnNumbers(std::string str)
{
	std::string result;
	if (!str.empty())
	{
		for (short i = 0; i < str.size(); i++)
		{
			if (((char(str[i]) > 47) && (char(str[i]) < 58)) || (char(str[i] == 44)) || (char(str[i] == 46)) || (char(str[i]) == 45))
			{
				if ((char(str[i] == 44)))
					result += ".";
				else
					result += str[i];
			}
		}
	}
	try
	{
		(std::stod(result));
	}
	catch (const std::invalid_argument& EX)
	{
		result = "0";
	}
	return result;
}

void ToUpperFunct(std::string & str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::toupper);
}

void PathPreparing(std::string & assumedPath)
{
	for (size_t i = 0; i < assumedPath.length(); ++i)
	{
		if (char(assumedPath[i]) == '\\')
			assumedPath[i] = '/';
	}
}