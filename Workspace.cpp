#include "stdafx.h"
#include <vector>
#include "atltypes.h"
#include "Workspace.h"

Workspace g_Workspace;
Workspace m1;

Workspace::Workspace()
{
	set_change = 0;
	op.open_file = 1;
	c_clean.x = -1;
	c_clean.y = -1;
	shapes = 0;


};
Workspace::~Workspace()
{
};
void Workspace::fToString(int fl, LPWSTR& s){
	std::ostringstream ost;
	ost << std::fixed << std::setprecision(12) << (fl);
	std::cout << ost.str() << std::endl;
	std::string s1;
	s1 = ost.str();
	WCHAR s_1[1024] = { L' ' };
	for (int i = 0; i <= s1.length(); i++){
		s_1[i] = (WCHAR)s1[i];
	}
	s = s_1;
}




