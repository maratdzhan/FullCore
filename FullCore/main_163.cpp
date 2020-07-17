
// CT_BLANK = 1 - not rewrite permpar, newdata, out-file
#define CT_BLANK 0
#define EXTRACT_RESULTS 1
#define SINGLE_STATE_MODE 1

#include "163CommonHeader.h"

void PrintArgs(const char **arr)
{
	int i = 0;
	printf("Command line arguments:\n");
	if (arr)
		while (arr[i])
		{
			printf("%s\n", arr[i]);
			i++;
		}
	printf("\n");
}

int main(int argc, const char **argv)
{

	PrintArgs(argv);
	CommandLineArgsBuilder cmd_args;
	cmd_args.Parser(argv);
	if (cmd_args.errors) {
		std::cerr << "Error in cmd-line arguments\n";
		return (1);
	}

	CommonParametersHandler handle(cmd_args.ExtractParameter("INPUT_FILE"));
	std::string rs = "res";

	for (const auto& content : handle.GetFilesList())
	{
		if (!content.IsCalculationInitialized())
			continue;

		Core CurrentLoad(content, cmd_args);

	//	if (std::stoi(CurrentLoad.GetArgParameter("SSM")))
	//		CurrentLoad.GSLastState(std::stoi(CurrentLoad.GetArgParameter("SSM_STATE")), true);
		CurrentLoad.FileReading();
		if (CurrentLoad.GetStatMode())
			CurrentLoad.StatMode(handle.GetInnerStruct(rs));
		else
			CurrentLoad.SingleMode();

	}

	return 0;
}