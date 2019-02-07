#include "163CommonHeader.h"

#include <Windows.h>
#include <string>
#include <tchar.h>
#include <xstring>
#include <iostream>



LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
TCHAR WinName[] = _T("MainFrame");
HINSTANCE This;


LRESULT CALLBACK WndGraph_163(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndGraph_Solo(HWND, UINT, WPARAM, LPARAM);
HINSTANCE hInst_ChildProcGraph_163;
HINSTANCE hInst_ChildProcGraph_Solo;
TCHAR WinClassGraphName_163[] = _T("ChildClass_163");
TCHAR WinClassGraphName_Solo[] = _T("ChildClass_Solo");
HWND HGraph_163, HGraph_Solo;
HWND HWnd_163Parent, HWnd_SoloParent;

ATOM RegisterGraph_163Class()
{
	WNDCLASSEX wcgraph_163 = { 0 };
	wcgraph_163.cbSize = sizeof(WNDCLASSEX);
	wcgraph_163.style = CS_HREDRAW | CS_VREDRAW;
	wcgraph_163.lpfnWndProc = WndGraph_163;
	wcgraph_163.hInstance = hInst_ChildProcGraph_163;
	wcgraph_163.hCursor = LoadCursor(NULL, IDC_CROSS);
	wcgraph_163.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcgraph_163.lpszClassName = WinClassGraphName_163;
	return RegisterClassEx(&wcgraph_163);

}

ATOM RegisterGraph_SoloClass()
{
	WNDCLASSEX wcgraph_solo = { 0 };
	wcgraph_solo.cbSize = sizeof(WNDCLASSEX);
	wcgraph_solo.style = CS_HREDRAW | CS_VREDRAW;
	wcgraph_solo.lpfnWndProc = WndGraph_Solo;
	wcgraph_solo.hInstance = hInst_ChildProcGraph_Solo;
	wcgraph_solo.hCursor = LoadCursor(NULL, IDC_CROSS);
	wcgraph_solo.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcgraph_solo.lpszClassName = WinClassGraphName_Solo;
	return RegisterClassEx(&wcgraph_solo);

}




int main()
{
	try {
		fCore_Main(true);


		GraphParameters_163Load();
		int image_size = stoi(GraphParameters_163Return("IMAGE_SIZE"));
		int scale_size = stoi(GraphParameters_163Return("SCALE_SIZE"));
		int sx = (int)(110 * image_size*(scale_size / 2.));
		int sy = (int)(110 * image_size*(scale_size / 2.));
		RegisterGraph_163Class();


		HGraph_163 = CreateWindow(WinClassGraphName_163, _T("Картограмма зоны"),
			WS_SYSMENU | WS_POPUP | WS_VISIBLE | WS_THICKFRAME | WS_CAPTION,
			sx / 6, sy / 10, sx, sy, HWnd_163Parent, 0, hInst_ChildProcGraph_163, NULL);
		UpdateWindow(HGraph_163);
		DestroyWindow(HGraph_163);

		if (stoi(GraphParameters_163Return("DETAILED")))
		{
			// ERF - если нет папки, то данные не сохраняются никуда
			RegisterGraph_SoloClass();
			HGraph_Solo = CreateWindow(WinClassGraphName_Solo, _T("Картограмма кассеты"),
				WS_SYSMENU | WS_POPUP | WS_VISIBLE | WS_THICKFRAME | WS_CAPTION,
				sx / 6, sy / 10, sx / 2, sy / 1.5, HWnd_SoloParent, 0, hInst_ChildProcGraph_Solo, NULL);
			UpdateWindow(HGraph_Solo);
			DestroyWindow(HGraph_Solo);
		}

		void GraphParameters_163Clear();
	}
	catch (std::exception & ex_1)
	{
		std::cout << ex_1.what() << std::endl;
	}

	return 0;
}