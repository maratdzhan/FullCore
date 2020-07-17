#pragma once


struct CommandLineArgsBuilder
{

	void Parser(const char**);
	void CheckValid(const std::string&);
	std::string ExtractParameter(const std::string&)const;
	void SetArgument(const std::string&, const std::string&);
	bool CheckArgs();
	int errors = 0;

protected:
	MSS parameters;
};


void CommandLineArgsBuilder::Parser(const char** argv)
{
	int i = 0;

	try {
		std::string name(argv[i++]);
		parameters["NAME"] = name;

		while (argv[i])
		{
			std::string parameter(argv[i]);
			std::string arg;
			std::string val;
			CheckValid(parameter);
			ToUpperFunct(parameter);
			int es = parameter.find("=");
			if (es == -1)
			{
				arg = parameter.substr(0, parameter.size());
				parameters[arg] = "0";
			}
			else
			{
				arg = parameter.substr(0, es);
				val = parameter.substr(es + 1, parameter.size());
				parameters[arg] = val;
			}
			i++;
		}
		if (!CheckArgs())
			throw(std::invalid_argument("errors in arguments"));
	}
	catch (std::invalid_argument & arg)
	{
		std::cerr << "Error in command line arguments:\n"
			<< arg.what() << std::endl;
		system("pause");
		errors++;
	}
}

void CommandLineArgsBuilder::SetArgument(const std::string& parameter_name, const std::string& parameter_value)
{

}

void CommandLineArgsBuilder::CheckValid(const std::string &parameter)
{
	if (std::count(parameter.begin(), parameter.end(),'=')  > 1)
	{
		errors++;
		throw(std::invalid_argument(parameter));
	}
}

std::string CommandLineArgsBuilder::ExtractParameter(const std::string& parameter_name)const
{
	if (parameters.count(parameter_name))
		return parameters.at(parameter_name);
	else
	{
		std::cerr << ">>>>>\n>>>>>\n>>>>> parameter " << parameter_name
			<< " is absence\n>>>>>\n>>>>>\n";
		return "0";
	}
}

bool CommandLineArgsBuilder::CheckArgs()
{

	if (parameters.count("INPUT_FILE") == 0)
		parameters["INPUT_FILE"] = "gap.par";
	if (parameters.count("MODIFIERS") == 0)
		parameters["MODIFIERS"] = "1";

	// если получаем SSM - обязан быть и SSM_STATE
	if (parameters.count("SSM"))
		if (!parameters.count("SSM_STATE"))
		{
			errors++;
			std::cerr << "1. BAD ARGUMENTS : SSM AND NO SSM_STATE\n";
			return false;
		}


	// если есть C_MODE=1, должен быть и C_MODE_FILE и SSMn и SSMn_STATE // n - next
	if (parameters.count("C_MODE") == 1)
	{
		if (!(parameters.count("SSMN") *
			parameters.count("SSMN_STATE"))) {
			errors++;
			std::cerr << "2. BAD ARGUMENTS : C_MODE AND MISSING SSMN OR/AND SSMN_STATE\n";
			return false;
		}

	}

	return true;
}