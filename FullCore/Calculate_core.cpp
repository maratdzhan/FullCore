//
//
//bool modifier = false;
//
//void Initializing()
//{
//		FA_Define();
//		DefMapType();
//		DefKasType();
//		DefineMainAssemblyParameters();
//		Gap_Define_S(modifier, false);
//		x5Define();
//
//}
//
//
//void fCore_Main(bool withMCs)
//{
//	std::string s="-1";
//	int kl = 0;
//	bool initialized = false;
//	modifier = withMCs;
//	//ListDefine();
//	// �������� �������
//	// ����������� �������� ���������� ���
//	ListDefine();
//
//	int err = -1;
//	printf("<NEW> - create new configuration\n");
//	
//	// �������� ������ � ��������
//	while ((s.find("END") == -1))
//	{
//		s.clear();
//		printf("Enter command of FA number\n");
//		
//		std::cin >> s;
//
//		
//		
//		// �������� ������, ����������� �� ��� ����� � ����� � KL
//		kl = std::stoi(ReturnNumbers(s));
//		// �������, �������� �� �� ����� ��� ��� + ��������: ���� �� �������������
//		if (((kl > 0) && (kl <= 163)) && (initialized))
//			FA_Movement(kl);
//		else
//		{
//			s = ToUpperFunct(s);
//		}
//
//
//	}
//
//	Gap_Define_S(modifier, true);
//
//
//
//	// ����� ������
//	PermparMaking();
//
//	TCHAR current_path[80] = "D:/Vasiliev/PERMAK-A/XIPI_18L_W/";
//
//	_chdir(current_path);
//
//	//GetCurrentDirectory(100, current_path);
//	
//	
//	system("PERMAK2007I.exe");
//
//	Extracting_KQ();
//	kk_calculating();
//
//}