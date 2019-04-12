#include "GetParam.h"

//template < typename T >
//T GetParam(const T _value, const std::string & s, const size_t number)
//{
//	std::string resultString;
//	size_t currentNumber = 1;
//	T result = 0;
//
//	bool breaking = false;
//	try {
//		for (unsigned int pos = 0; pos < s.size(); pos++)
//		{
//			if ((s[pos] != ',') && (currentNumber == number)&&(s[pos]!=32))
//			{
//				resultString += s[pos];
//			}
//			else
//			{
//				if (!breaking)
//				{
//					if (currentNumber > number)
//						breaking = true;
//					if (s[pos] == ',')
//						currentNumber++;
//				}
//				else
//					break;
//			}
//		}
//
//		if (typeid(T) == typeid(int))
//		{
//			result = stoi(resultString);
//		}
//		else if (typeid(T) == typeid(double))
//		{
//			result = stod(resultString);
//		}
//		return result;
//	}
//	catch (std::exception & exc)
//	{
//		std::cerr << exc.what() << std::endl;
//	}
//}

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
				if (inputString[pos] != ' ')
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
		for (size_t i = 0; i < str.size(); i++)
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

void SymbolsDeleting(std::string & inputString)
{
	std::string _newString = inputString;
	if (_newString.empty())
		throw(std::invalid_argument("empty string"));
	inputString.clear();
	for (size_t i = 0; i < _newString.size(); ++i)
	{
		if (	(char(_newString[i]) != 32)
			&& (char(_newString[i]) > 47) 
			&& (char(_newString[i]) < 58)
			&& (char(_newString[i])  >64)  
			&& (char(_newString[i]) < 91) 
			&& (char(_newString[i]) > 96)
			&& (char(_newString[i]) < 123))
			inputString += _newString[i];
	}
}