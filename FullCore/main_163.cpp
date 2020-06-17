
// CT_BLANK = 1 - not rewrite permpar, newdata, out-file
#define CT_BLANK 0
#define EXTRACT_RESULTS 1
#define SINGLE_STATE_MODE 1

#include "163CommonHeader.h"

void PrintArgs(char **arr)
{
	int i = 0;
	if (arr)
		while (arr[i])
		{
			printf("%s\n", arr[i]);
			i++;
		}
	printf("\n");
}

int main(int argc, char **argv)
{
	CommonParametersHandler handle;

	int SSM = 0;
	int SSM_STATE = 0;
	PrintArgs(argv);
	try {
		if (argc > 1) {
			SSM = atoi(argv[1]);
			std::cout << "SINGLE_STATE_MODE = " << SSM << std::endl;
			if (SSM == 1)
			{
				SSM_STATE = atoi(argv[2]);
				std::cout << "STATE = " << SSM_STATE << std::endl;
			}
		}
	}
	catch (std::exception & e)
	{
		std::cerr << "invalid parameter at ARGV => " << e.what() << "\n";
		system("pause");
		return 1;
	}

	for (const auto& content : handle.GetFilesList())
	{
		if (!content.IsCalculationInitialized())
			continue;

		Core CurrentLoad(content, SSM);
		

		if (!SSM)
			CurrentLoad.GSLastState(SSM_STATE, true);
		CurrentLoad.FileReading();
		if (CurrentLoad.GetStatMode())
			CurrentLoad.StatMode();
		else
			CurrentLoad.SingleMode();

	}

	return 0;
}

