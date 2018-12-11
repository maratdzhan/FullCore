#pragma once
#include <vector>
#include <string>
#include <fstream>

void InnerTvsColorOrder(short tvs);

enum returnableValues
{
	_e_kk_mcu_current,
	_e_kk_permak_current,
	_e_kk_dev_current,
	_e_min_in_core_permak,
	_e_max_in_core_permak,
	_e_min_in_core_mcu,
	_e_max_in_core_mcu,
	_e_min_in_tvs_permak,
	_e_max_in_tvs_permak,
	_e_min_in_tvs_mcu,
	_e_max_in_tvs_mcu,
	_e_max_dev_tvs,
	_e_min_dev_tvs,
	_e_max_dev_core,
	_e_min_dev_core,
	_e_min_kq_dev,
	_e_max_kq_dev,
};

