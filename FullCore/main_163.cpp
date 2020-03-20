
// CT_BLANK = 1 - not rewrite permpar, newdata, out-file
#define CT_BLANK 0
#define EXTRACT_RESULTS 1

#include "163CommonHeader.h"

int main()
{
	CommonParametersHandler handle;


	for (const auto& content : handle.GetFilesList())
	{
		if (!content.IsCalculationInitialized())
			continue;

		Core CurrentLoad(content);
		
		
		CurrentLoad.FileReading();
		if (CurrentLoad.GetStatMode())
			CurrentLoad.StatMode();
		else
			CurrentLoad.SingleMode();

	}

	system("pause");
	return 0;
}

