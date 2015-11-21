#include "stdafx.h"
#include <vector>
#include "atltypes.h"
#include "Workspace.h"

Workspace g_Workspace;

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



