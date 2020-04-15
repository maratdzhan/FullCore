#pragma once
#include <string>
#include <Windows.h>

void GraphParameters_163Load();
void GraphParameters_163Clear();
std::string GraphParameters_163Return(std::string parameter_name);
void TVS_TextOutCore(HDC device_hinstance, short fa_num);
void TVS_Solo_TextOutCore(HDC hdc, short tvs, short tvel);
COLORREF ColorReference(short tvs, short tvel, int mainValue, int mm);
VOID StoreClientWindowBitmap(HWND wnd, LPCTSTR pszFileName);
void DisplayGapsSize(HDC device_hinstance, short x, short y, short tvs);



enum valuesType
{
	_e_permak_tvs_kk,
	_e_mcu_tvs_kk,
	
	_e_permak_core_kk,
	_e_mcu_core_kk,
	
	_e_dev_core,
	_e_dev_tvs,

	_e_tvs
};