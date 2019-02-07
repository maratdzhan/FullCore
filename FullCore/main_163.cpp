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

int APIENTRY _tWinMain(HINSTANCE This, HINSTANCE Prev, LPTSTR cmd, int mode)
{
	HWND hWnd;
	MSG msg;
	WNDCLASS wc;

	wc.hInstance = This;
	wc.lpszClassName = WinName;
	wc.lpfnWndProc = WndProc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszMenuName = NULL;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	// Заполнение окна белым цветом
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);


	if (!RegisterClass(&wc)) return 0; // Регистрация класса окна


	// Создание окна
	hWnd = CreateWindow(WinName, // Имя класса окна
		_T("Gaps (Core)"), // Заголовок окна
		WS_OVERLAPPEDWINDOW
		//		WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX
		, // Стиль окна
		500, // x
		500, // y 
		// Размеры окна
		800, // Width
		800, // Height
		HWND_DESKTOP, // Дескриптор родительского окна
		NULL, // Нет меню
		This, // Дескриптор приложения
		NULL); // Дополнительной информации нет


	ShowWindow(hWnd, mode); //Показать окно
	UpdateWindow(hWnd);
	// Цикл обработки сообщений
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);// Функция трансляции кодов нажатой клавиши
		DispatchMessage(&msg); // Посылает сообщение функции WndProc()
	}
	return 0;
}


void SetButtonsNumber(std::vector<Buttons> & b)
{
	for (unsigned int i = 0; i < b.size(); i++)
	{
		b[i].SetNumber(i);
	}
}

//Buttons button_run("RUN"), button_configure("CONFIGURE"), button_exit("EXIT");
std::vector<Buttons> button;


void CreatingButtonsContainer(std::vector<Buttons> & b, int begining) {

	if (!begining)
	{
		for (unsigned int item = 0; item < names.size(); item++)
		{
			Buttons bttn(names[item]);
			button.push_back(bttn);
		}
		SetButtonsNumber(button);
	}

}

void DestroyAll() {
	for (auto c : button)
		c.Destroy();
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	HINSTANCE ButtonHinstance;
	HDC hDC;
	PAINTSTRUCT PaintStruct;

	CreatingButtonsContainer(button, button.size());

	switch (message)
	{
	case WM_SIZE:
	{

		for (auto & current_button : button)
		{
			current_button.Destroy();
			current_button.Reconfigure(LOWORD(lParam), HIWORD(lParam), button.size());
			current_button.Create(hWnd, ButtonHinstance);

			hDC = BeginPaint(hWnd, &PaintStruct);
			EndPaint(hWnd, &PaintStruct);
		}
		return 0;

	}
	case WM_CREATE:
	{

		return 0;
	}
	case WM_PAINT:
	{

	}
	case WM_COMMAND:
		// Обработчик сообщений кнопок
		for (auto b : button)
		{
			b.InputSignal(LOWORD(wParam));
		}
		return 0;
	case WM_DESTROY: PostQuitMessage(0);
		break; // Завершение программы
		// Обработка сообщения по умолчанию
	default: return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

int old_main()
{
	
	
	//try {
	//	fCore_Main(true);


	//	GraphParameters_163Load();
	//	int image_size = stoi(GraphParameters_163Return("IMAGE_SIZE"));
	//	int scale_size = stoi(GraphParameters_163Return("SCALE_SIZE"));
	//	int sx = (int)(110 * image_size*(scale_size / 2.));
	//	int sy = (int)(110 * image_size*(scale_size / 2.));
	//	RegisterGraph_163Class();


	//	HGraph_163 = CreateWindow(WinClassGraphName_163, _T("Картограмма зоны"),
	//		WS_SYSMENU | WS_POPUP | WS_VISIBLE | WS_THICKFRAME | WS_CAPTION,
	//		sx / 6, sy / 10, sx, sy, HWnd_163Parent, 0, hInst_ChildProcGraph_163, NULL);
	//	UpdateWindow(HGraph_163);
	//	DestroyWindow(HGraph_163);

	//	if (stoi(GraphParameters_163Return("DETAILED")))
	//	{
	//		// ERF - если нет папки, то данные не сохраняются никуда
	//		RegisterGraph_SoloClass();
	//		HGraph_Solo = CreateWindow(WinClassGraphName_Solo, _T("Картограмма кассеты"),
	//			WS_SYSMENU | WS_POPUP | WS_VISIBLE | WS_THICKFRAME | WS_CAPTION,
	//			sx / 6, sy / 10, sx / 2, sy / 1.5, HWnd_SoloParent, 0, hInst_ChildProcGraph_Solo, NULL);
	//		UpdateWindow(HGraph_Solo);
	//		DestroyWindow(HGraph_Solo);
	//	}

	//	void GraphParameters_163Clear();
	//}
	//catch (std::exception & ex_1)
	//{
	//	std::cout << ex_1.what() << std::endl;
	//}

	return 0;
}