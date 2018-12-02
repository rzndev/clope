// clope_console.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <string>
#include <stdlib.h>
#include "Clope.h"


using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	string file_name = "agaricus-lepiota.txt";
	double r = 2.6;
	if (argc > 1)
	{
		wstring name = wstring(argv[1]);
		file_name = string(name.begin(), name.end());
	}
	if (argc > 2)
	{
		wstring param_r = wstring(argv[2]);
		string sr(param_r.begin(), param_r.end());
		r = atof(sr.c_str());
	}
	CClope alg;
	alg.run(file_name, r);
#ifdef _DEBUG
	system("pause");
#endif
	return 0;
}

