
// CT_BLANK = 1 - not rewrite permpar, newdata, out-file
#define CT_BLANK 0

#include "163CommonHeader.h"

// Replace cout with cerr


int main()
{
   	CommonParametersHandler handle;

		// Get states list
		// Start calculating
	for (const auto & content : handle.FilesList())
	{
		if (content.IsCalculationInitialized()) {
			Core CurrentLoad(content);
			CurrentLoad.LoadingAssemblies();
			CurrentLoad.NewdataMaking();
			CurrentLoad.PermparMaking();
			CurrentLoad.GrabResults();
		}
	}


	system("pause");
	return 0;
}


