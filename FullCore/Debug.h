#pragma once


void Core::GetDebugPVM()
{
	std::ofstream odstream(m_Compilation.GetFileByName("COORDS.PVM") + "_dbg");

	odstream << std::endl << std::endl;
	int num = 1;
	for (int i = 0; i < first_coodinate.size(); i++)
	{
		odstream << (num++) << "," << first_coodinate[i] << "," << second_coordinate[i] << std::endl;
		if (num % 164 == 0) 
			num = 1;
	}

	odstream.close();

}
//	