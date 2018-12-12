#include "163CommonHeader.h"

#include <Windows.h>
#include <tchar.h>

int sx, sy;



LRESULT CALLBACK WndGraph_163(HWND hGraph_163, UINT message, WPARAM wParam, LPARAM lParam)
{
	////////////// LOCAL VARIABLES
	double cell_size_local = stod(GraphParameters_163Return("CELL_SIZE"));
	double scale = stod(GraphParameters_163Return("SCALE_SIZE"));;
	LONG x_1 = 0*cell_size_local*scale, x_2 = 6.4*cell_size_local*scale, y_1 = 8.0*cell_size_local*scale, y_2 = 4.0*cell_size_local*scale;
	LONG x_3 = 0 * cell_size_local*scale, x_4 = 6.6*cell_size_local*scale, y_3 = 8.2*cell_size_local*scale, y_4 = 4.2 * cell_size_local*scale;
	short NUMBER_OF_FA = stoi(GraphParameters_163Return("FA_COUNT"));
	double x, y = 0;
	double x_tvel = 0, y_tvel = 0;
	double grid_pitch = stod(GraphParameters_163Return("GRID_PITCH"));
	short numberOfStrings = stoi(GraphParameters_163Return("STRINGS_COUNT"));
	bool colored = stoi(GraphParameters_163Return("COLOR_VAR"));
	bool DISPLAY_GAPS = stoi(GraphParameters_163Return("SHOW_GAPS_SIZE"));
	bool PAINT_FR = stoi(GraphParameters_163Return("PAINT_FR"));
	short NUMBER_OF_TVEL = stoi(GraphParameters_163Return("FR_COUNT"));
	double TVEL_SIZE = stod(GraphParameters_163Return("FR_RADIUS"));

	if (PAINT_FR) { colored = false;  numberOfStrings = -1; }
	
	///// CONST VARIABLES
	POINT pt[6] = { { x_1,y_1 },{ x_2,y_2 },{ x_2,-y_2 },{ x_1,-y_1 },{ -x_2,-y_2 },{ -x_2,y_2 } };
	const POINT pt_BOUND[6] = { { x_3,y_3 },{ x_4,y_4 },{ x_4,-y_4},{ x_3,-y_3 },{ -x_4,-y_4 },{ -x_4,y_4 } };

	PAINTSTRUCT ps;
	HDC hdc;
	static HBRUSH hBrush;
	static HBRUSH hColoredbrush;
	static HBRUSH hBoundBrush_FA = CreateSolidBrush(RGB(100, 100, 100));
	static HBRUSH hBoundBrush_FR = CreateSolidBrush(RGB(220, 220, 220));
	

	switch (message)
	{
	case WM_CREATE:
		/*sx = LOWORD(lParam);
		sy = HIWORD(lParam);*/
		break;
	case WM_SIZE:
		sx = LOWORD(lParam);
		sy = HIWORD(lParam);
		break;
	case WM_PAINT:
	{
		hdc = BeginPaint(hGraph_163, &ps);
		SetMapMode(hdc, MM_ANISOTROPIC);				// режим отображения - разномасштабируемый
		SetViewportOrgEx(hdc, sx / 2, sy / 2, NULL);	// центр текущего построения
		SetWindowExtEx(hdc, sx, sy, NULL);				// размер окна
		SetViewportExtEx(hdc, sx, sy, NULL);			// размер вывода - непонятная штука

		for (short i = 0; i < NUMBER_OF_FA; i++)
		{
			//// Рисовка ТВС
		
				x = (ReturnCoordinatesTvs(i + 1, true))*grid_pitch*scale;
				y = (ReturnCoordinatesTvs(i + 1, false))*grid_pitch*scale;
				SetViewportOrgEx(hdc, int(sx / 2 + x), (int)(sy / 2 - y), NULL);


				// BOUND
				BeginPath(hdc);
				Polyline(hdc, pt_BOUND, 6);
				CloseFigure(hdc);
				EndPath(hdc);
				if (!PAINT_FR)
					SelectObject(hdc, hBoundBrush_FA);
				else
					SelectObject(hdc, hBoundBrush_FR);
				SetPolyFillMode(hdc, WINDING);
				FillPath(hdc);

				if (!PAINT_FR) {

					BeginPath(hdc);
					Polyline(hdc, pt, 6);
					CloseFigure(hdc);
					EndPath(hdc);

					SetPolyFillMode(hdc, WINDING);


					if (colored)
					{
						hBrush = CreateSolidBrush(ColorReference(i, -1, 6,5));
						SelectObject(hdc, hBrush);
					}
					else
					{
						hBrush = CreateSolidBrush(RGB(255, 255, 255));
						SelectObject(hdc, hBrush);
					}
						FillPath(hdc);
						DeleteObject(hBrush);
					
				}
				// */
				if (PAINT_FR) {
					for (short tvel = 1; tvel < NUMBER_OF_TVEL + 1; tvel++)
					{
						x_tvel = 1.02*ReturnCoordinatesTvel(tvel, true);
						y_tvel =1.1*ReturnCoordinatesTvel(tvel, false);
						hColoredbrush = CreateSolidBrush(ColorReference(i+1, tvel, _e_kk_mcu_current,_e_mcu_core_kk));
						SelectObject(hdc, hColoredbrush);
						Ellipse(hdc,
							(0.45*(-x_tvel - TVEL_SIZE)),
							(0.45*(-y_tvel - TVEL_SIZE)),
							(0.45*(-x_tvel + TVEL_SIZE)),
							(0.45*(-y_tvel + TVEL_SIZE)));
						FillPath(hdc);
						DeleteObject(hColoredbrush);
					}
				}

				if (DISPLAY_GAPS)
					DisplayGapsSize(hdc, x_2, y_2, i + 1);

			//// Вывод текста, если текст есть
			if (numberOfStrings > 0)
			{
				TVS_TextOutCore(hdc, i + 1);
			}
		}
		EndPaint(hGraph_163, &ps);
		StoreClientWindowBitmap(hGraph_163, "D:/Vasiliev_Others/CppTestDir/bin/data/1.bmp");
	}
	default:
	{
		return DefWindowProc(hGraph_163, message, wParam, lParam);
	}
	}
	return 0;
}

// Сохранение BMP в файл
BOOL StoreBitmapFile(LPCTSTR lpszFileName, HBITMAP HBM)
{

	BITMAP BM;
	BITMAPFILEHEADER BFH;
	LPBITMAPINFO BIP;
	HDC DC;
	LPBYTE Buf;
	DWORD ColorSize, DataSize = 0;
	WORD BitCount;
	HANDLE FP;
	DWORD dwTemp;


	GetObject(HBM, sizeof(BITMAP), (LPSTR)&BM);


	BitCount = (WORD)BM.bmPlanes * BM.bmBitsPixel;
	switch (BitCount)
	{
	case 1:
	case 4:
	case 8:
	case 32:
		ColorSize = sizeof(RGBQUAD) * (1 << BitCount);
		break;
	case 16:
	case 24:
		ColorSize = 0;
		break;
	default:
		ColorSize = sizeof(RGBQUAD) * (1 << BitCount);
		break;
	}


	DataSize = ((BM.bmWidth*BitCount + 31) & ~31) / 8 * BM.bmHeight;

	BIP = (LPBITMAPINFO)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(BITMAPINFOHEADER) + ColorSize);

	BIP->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	BIP->bmiHeader.biWidth = BM.bmWidth;
	BIP->bmiHeader.biHeight = BM.bmHeight;
	BIP->bmiHeader.biPlanes = 1;
	BIP->bmiHeader.biBitCount = BitCount;
	BIP->bmiHeader.biCompression = 0;
	BIP->bmiHeader.biSizeImage = DataSize;
	BIP->bmiHeader.biXPelsPerMeter = 0;
	BIP->bmiHeader.biYPelsPerMeter = 0;
	if (BitCount < 16)
		BIP->bmiHeader.biClrUsed = (1 << BitCount);
	BIP->bmiHeader.biClrImportant = 0;



	BFH.bfType = 0x4d42;
	BFH.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + BIP->bmiHeader.biClrUsed * sizeof(RGBQUAD);
	BFH.bfReserved1 = 0;
	BFH.bfReserved2 = 0;
	BFH.bfSize = BFH.bfOffBits + DataSize;


	Buf = (LPBYTE)GlobalAlloc(GMEM_FIXED, DataSize);


	DC = GetDC(0);
	GetDIBits(DC, HBM, 0, (WORD)BM.bmHeight, Buf, BIP, DIB_RGB_COLORS);
	ReleaseDC(0, DC);

	FP = CreateFile(lpszFileName, GENERIC_READ | GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(FP, &BFH, sizeof(BITMAPFILEHEADER), &dwTemp, NULL);
	WriteFile(FP, BIP, sizeof(BITMAPINFOHEADER) + BIP->bmiHeader.biClrUsed * sizeof(RGBQUAD), &dwTemp, NULL);
	WriteFile(FP, Buf, DataSize, &dwTemp, NULL);

	CloseHandle(FP);
	GlobalFree((HGLOBAL)Buf);
	HeapFree(GetProcessHeap(), 0, (LPVOID)BIP);

	return(true);
}

// создание BMP из DC
HBITMAP CreateBitmap(HDC DC, int w, int h)
{
	HDC memDC;
	HBITMAP bm, oldBM;


	memDC = CreateCompatibleDC(DC);
	bm = CreateCompatibleBitmap(DC, w, h);
	oldBM = (HBITMAP)SelectObject(memDC, bm);
	BitBlt(memDC, 0, 0, w, h, DC, 0, 0, SRCCOPY);
	SelectObject(memDC, oldBM);
	DeleteDC(memDC);
	DeleteObject(oldBM);
	return bm;
}

// создание BMP клиентской области окна
HBITMAP CreateClientWindwowBitmap(HWND wnd)
{
	RECT r;
	GetClientRect(wnd, &r);
	return(CreateBitmap(GetDC(wnd), r.right, r.bottom));
}

// создание BMP окна
HBITMAP CreateWindwowBitmap(HWND wnd)
{
	RECT r;
	POINT pt;
	GetWindowRect(wnd, &r);
	pt.x = r.right;
	pt.y = r.bottom;
	ScreenToClient(wnd, &pt);
	return(CreateBitmap(GetWindowDC(wnd), pt.x, pt.y));
}

// создание BMP рабочего стола
HBITMAP CreateDesktopBitmap(void)
{
	RECT r;

	SystemParametersInfo(SPI_GETWORKAREA, 0, &r, 0);

	return(CreateBitmap(GetDC(GetDesktopWindow()), r.right, r.bottom));
}

// создание BMP экрана
HBITMAP CreateScreenBitmap(void)
{

	DWORD w, h;
	HDC DC;

	w = GetSystemMetrics(SM_CXSCREEN);
	h = GetSystemMetrics(SM_CYSCREEN);

	DC = CreateDC("DISPLAY", NULL, NULL, NULL);
	return(CreateBitmap(DC, w, h));
	DeleteDC(DC);
}

// сохранение в BMP-файл клиентской области окна
VOID StoreClientWindowBitmap(HWND wnd, LPCTSTR pszFileName)
{

	HBITMAP bm;

	bm = CreateClientWindwowBitmap(wnd);
	StoreBitmapFile(pszFileName, bm);
	DeleteObject(bm);
}

// сохранение в BMP-файл окна
VOID StoreWindowBitmap(HWND wnd, LPCTSTR pszFileName)
{

	HBITMAP bm;

	bm = CreateWindwowBitmap(wnd);
	StoreBitmapFile(pszFileName, bm);
	DeleteObject(bm);
}

// сохранение в BMP-файл рабочего стола
VOID StoreDesktopBitmap(LPCTSTR pszFileName)
{

	HBITMAP bm;

	bm = CreateDesktopBitmap();
	StoreBitmapFile(pszFileName, bm);
	DeleteObject(bm);
}

// сохранение в BMP-файл экрана
VOID StoreScreenBitmap(LPCTSTR pszFileName)
{

	HBITMAP bm;

	bm = CreateScreenBitmap();
	StoreBitmapFile(pszFileName, bm);
	DeleteObject(bm);
}




