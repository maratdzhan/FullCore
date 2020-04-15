#include "163CommonHeader.h"

#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <xstring>
#include <iostream>

typedef std::basic_string<TCHAR, std::char_traits<TCHAR>,
	std::allocator<TCHAR> > String;

int sx_S, sy_S;


LRESULT CALLBACK WndGraph_Solo(HWND hGraph_Solo, UINT message, WPARAM wParam_s, LPARAM lParam_s)
{
	PAINTSTRUCT ps;
	HDC hdc;
	static HBRUSH hBrush, hBrush_this, hBrush_this_edge_1, cBr;
	//	HRGN hRgn;
	cBr = CreateSolidBrush(RGB(200, 200, 200));
	hBrush = CreateSolidBrush(RGB(200, 200, 200));
	double z1 = 0;
	double z2 = 0;
	int x = 0;
	int y = 0;
	bool coloredFR = stoi(GraphParameters_163Return("COLOR_VAR"));
	int TvelMainValue = stoi((GraphParameters_163Return("TVEL_MAIN_VALUE")));
	//message = WM_PAINT;

	int map_size_t = 1;
	double map_size_s = 3.1;
	double map_size_tvs = 2.7;
	double scale = 1.1;
	double cart_pos_y = 25;
	int mn = 2;
	double scale_pos = 1.;
	LONG x2t = 12, x1t = 8, y2t = 0, y1t = 10;
	POINT pt_pos[6] = {
	{ -x1t * map_size_tvs, y1t * map_size_tvs },
	{ x1t * map_size_tvs, y1t * map_size_tvs },
	{ x2t * map_size_tvs, y2t * map_size_tvs },
	{ x1t * map_size_tvs, -y1t * map_size_tvs },
	{ -x1t * map_size_tvs, -y1t * map_size_tvs },
	{ -x2t * map_size_tvs, y2t * map_size_tvs }, 
	};
	HFONT newFont, oldFont;


	switch (message)
	{
	case WM_CREATE:
		/*sx = LOWORD(lParam);
		sy = HIWORD(lParam);*/
		break;
	case WM_SIZE:
		sx_S = LOWORD(lParam_s);
		sy_S = HIWORD(lParam_s);
		break;
	case WM_PAINT:
	{

		hdc = BeginPaint(hGraph_Solo, &ps);
		SetMapMode(hdc, MM_ANISOTROPIC);				// режим отображения - разномасштабируемый
		
		SetWindowExtEx(hdc, sx_S, sy_S, NULL);				// размер окна
		SetViewportExtEx(hdc, sx_S, sy_S, NULL);			// размер вывода - непонятная штука
		
		for (int tvs_kart = 1; tvs_kart < 164; tvs_kart++)
		{
			SetViewportOrgEx(hdc, sx_S / 2, sy_S / 2 + cart_pos_y, NULL);	// центр текущего построения
			// НОМЕР КАССЕТЫ
			newFont = CreateFont(15, 0, 0, 0, 0, 0, 0, 0, 
				DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
				DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");
			oldFont = (HFONT)SelectObject(hdc, newFont);
			std::string sss0 = "Fuel Assambly #" + std::to_string(tvs_kart);
			TextOut(hdc, 0-2.5*sss0.length(), 0+sy_S/2-80, sss0.data(), sss0.size());
			SelectObject(hdc, oldFont);
			DeleteObject(newFont);
				

			for (int num = 1; num < 332; num++)
			{
				try {
					z1 = 1.5*ReturnCoordinatesTvel(num, false);
					z2 = 1.6*ReturnCoordinatesTvel(num, true);
					x = ((sx_S / 2) - z2 * map_size_s);
					y = ((sy_S / 2) - z1 * map_size_s - cart_pos_y);
					SetViewportOrgEx(hdc, x, y, NULL); // установка центра из которого начинается построение

					BeginPath(hdc);
					Polyline(hdc, pt_pos, 6);
					CloseFigure(hdc);
					EndPath(hdc);

					if (coloredFR)
						cBr = CreateSolidBrush(ColorReference(tvs_kart, num, _e_kk_dev_current, _e_dev_tvs));
					else
						cBr = CreateSolidBrush(RGB(0, 0, 0));

					SelectObject(hdc, cBr);
					SetPolyFillMode(hdc, WINDING);
					FillPath(hdc);
					DeleteObject(cBr);

					TVS_Solo_TextOutCore(hdc, tvs_kart, num);
					int debug_item_0 = 0;
				}
				catch (std::exception & EX_2)
				{
					std::cout << EX_2.what() << std::endl;
				}
			}
				
			std::string filename_tvs = "D:/Vasiliev_Others/CppTestDir/bin/data/tvs_kart/" + std::to_string(tvs_kart) + ".bmp";
			StoreClientWindowBitmap(hGraph_Solo, filename_tvs.c_str());
			
			SetViewportOrgEx(hdc, 0, 0, NULL);	// центр текущего построения
			RECT rc;
			GetClientRect(hGraph_Solo, &rc);
			FillRect(hdc, &rc, (HBRUSH)(COLOR_WINDOW + 1));

			int debug_item_1 = 0;
		}
		EndPaint(hGraph_Solo, &ps);
	}
	default:
	{
		return DefWindowProc(hGraph_Solo, message, wParam_s, lParam_s);
	}
	}
	return 0;
}

