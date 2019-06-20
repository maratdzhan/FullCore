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
			CurrentLoad.NewdataMaking();
			CurrentLoad.LoadingAssemblies();
			CurrentLoad.PermparMaking();
		}
	}

	return 0;
}
 
