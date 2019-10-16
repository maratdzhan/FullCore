#pragma once

void Core::GrabResults()
{
	if (CT_BLANK) {
		std::cerr << "Grab result file -> skipped (CT_BLANK==1)\n"; 
		return;
	}
	// Extraction does not work in case "with out reflectors"
	std::vector<std::string> kq_str;
	std::vector<std::string> kq_p;

	GrabOutFile(kq_str);
//	KQ_P_String(kq_str, kq_p);
	GrabKqValues(kq_str,kq_p);
	SaveKqValues(kq_p);

}


void Core::GrabOutFile(std::vector<std::string>& kq_str)
{
	std::string outFile;
	std::ifstream perm(p_workdirectory + "PERM");
	if (!perm.is_open()) { std::cerr << __FUNCTION__ << " perm file is not opened. Check folder: " << p_workdirectory << "\n"; return; }

	//int cntr = 0;
	//while (1)
	//{
	//	getline(perm, outFile);
	//	if (cntr == 4) break;
	//	cntr++;
	//}
	//outFile = unitPath + outFile;
	std::cerr << "Grab OUT-File path: " << outFile << std::endl;
	perm.close();

	

	std::ifstream out_file(outFile);
	if (!out_file.is_open()) { std::cerr << __FUNCTION__ << " OUT-File file is not opened. Check folder: " << outFile << "\n"; return; }
	std::string trigger = "DEVIATION FROM  B I P R";
	std::string str;
	bool isData = false;
	int cn = 0;
	while (!out_file.eof())
	{
		getline(out_file, str);
		if (str.find(trigger) != -1) { isData = true; continue; }
		if (isData) {
			if (cn < 102) // 102 -size of array
			{
				kq_str.push_back(str);
				cn++;
			}
			else
			{
				isData = false;
				cn = 0;
			}
		}
	}

	std::cerr << "Grab " << kq_str.size() << " points\n";

}



void Core::KQ_P_String(std::string&str, std::vector<std::string>& output)
{
	std::string word;
	for (const auto& ch : str)
	{
		if (ch != 32)
			word += ch;
		else if (!word.empty())
		{
			output.push_back(word);
			word.clear();
		}
	}

}

void Core::GrabKqValues(std::vector<std::string>& kq_str, std::vector<std::string>& kq_p)
{
	int cntr = 0;
	for (auto& item : kq_str)
	{
		cntr++;
		if (cntr == 2)
			KQ_P_String(item, kq_p);
		if (cntr == 6)
			cntr = 0;
	}
}

void Core::SaveKqValues(std::vector<std::string>& input)
{
	std::vector<double> vectorToSave(input.size() / 211 * 163);
	std::string s = (p_project_name + "_" + std::to_string(unit_number) + "_" + m_Compilation.GetTestName());
	std::ofstream ofs(s);
	
	for (int i = 0; i < (int)input.size(); i++)
	{
		int num = GetOutSchemeElement(i);
			if (num > 0) {

				vectorToSave[num+ ((i / 211) * 163)-1] = stod(input[i]);

			}
			else
				continue;
			
	}
	for (auto& item : vectorToSave)
	{
		ofs << item << std::endl;
	}
	ofs.close();

}

int Core::GetOutSchemeElement(int& inputNumber)
{
	return numBlock[inputNumber % (numBlock.size())];
}

void Core::OutSchemeElementInitializing()
{
	numBlock = {
					-1,-1,-1,-1,-1,-1,-1,
			-1,-1,158,159,160,161,162,163,-1,-1,
		-1,149,150,151,152,153,154,155,156,157,-1,
	  -1,139,140,141,142,143,144,145,146,147,148,-1,
	 -1,128,129,130,131,132,133,134,135,136,137,138,-1,
	-1,116,117,118,119,120,121,122,123,124,125,126,127,-1,
  -1,103,104,105,106,107,108,109,110,111,112,113,114,115,-1,
 -1, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99,100,101,102,-1,
		-1,76,77,78,79,80,81,82,83,84,85,86,87,88,-1,
	-1,62,63,64,65,66,67,68,69,70,71,72,73,74,75,-1,
	-1,49,50,51,52,53,54,55,56,57,58,59,60,61,-1,
	-1,37,38,39,40,41,42,43,44,45,46,47,48,-1,
	-1,26,27,28,29,30,31,32,33,34,35,36,-1,
	-1,16,17,18,19,20,21,22,23,24,25,-1,
	-1,7,8,9,10,11,12,13,14,15,-1,
	-1,-1,1,2,3,4,5,6,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,
	};
}