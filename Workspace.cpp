#include "stdafx.h"
#include <vector>
#include <cstring> 
#include <string>  
#include <iostream>
#include <sstream>
#include <iomanip>

#include "windows.h"
#include "atltypes.h"


#pragma once
class Workspace
{
	struct coord_neigh
	{
		int x;
		int y;
	};
public:
	struct neighbors
{
	coord_neigh el[9];
	int count = 0;
};
private:
	class rectMatrix{
	public:
		std::vector<CPoint> coord;
		bool a[1050][1050];
		/*
		a[0][0], a[0][1],a[0][2]...
		a[1]...
		a[0*w+0],a[0*w+1],a[0*w+2],...
		a[1*w+0],a[1*w+1],
		vector<bool> a;
		minX,minY,maxX,maxY
		w,h
		a[x][y]=a[(y-minY)*w+(x-minX)]
		*/
		int index = -1;
		int size;
		int infinite_field = 1;
		int size_x, size_y;
		int size_of_rect;
		rectMatrix(){
			size_x = 300; size_y = 300;
			size = 0;
			size_of_rect = 15;

		};
		rectMatrix(int size_x, int size_y, int size_of_rect){
			this->size_x = size_x;
			this->size_y = size_y;
			this->size_of_rect = size_of_rect;
		};
		void del(int x, int y, bool q){//when x and y its coord q==true
			if (q == false)		{
				x = x*size_of_rect;
				y = y*size_of_rect;
			}
			else
			{
				x = (x / size_of_rect)*size_of_rect;
				y = (y / size_of_rect)*size_of_rect;
			}
			x = x / size_of_rect;
			y = y / size_of_rect;
			int i = 0;
			if (ret_value(x, y,false) == true)
			{
				for (i = 0; i <= index; i++){
					if (coord[i].x == x && coord[i].y == y)break;
				}
				std::swap(coord[i], coord.back());
				coord.pop_back();
				a[x][y] = 0;
				index--;
			}
		}
		void put(int x, int y, bool q){//when x and y its coord q==true
			if (q == false)		{
				x = x*size_of_rect;
				y = y*size_of_rect;
			}
			else
			{
				x = (x / size_of_rect)*size_of_rect;
				y = (y / size_of_rect)*size_of_rect;
			}
			x = x / size_of_rect;
			y = y / size_of_rect;
			if (x < size_x / size_of_rect && y < size_y / size_of_rect && x >= 0 && y >= 0)
			{
				CPoint c;
				c.x = x;
				c.y = y;
				if (a[x][y] != 1)
				{
					a[x][y] = 1;
					coord.push_back(c);
					index++;
				}

				index = coord.size() - 1;
			}
		};
		bool ret_value(int x, int y, bool q){
			if (q == false)		{
				x = x*size_of_rect;
				y = y*size_of_rect;
			}
			else
			{
				x = (x / size_of_rect)*size_of_rect;
				y = (y / size_of_rect)*size_of_rect;
			}
			x = x / size_of_rect;
			y = y / size_of_rect;
			int i;
			/*for (i = 0; i <= index; i++){
			if (x == coord[i].x && y == coord[i].y)return true;
			}*/
			if (x <= size_x / size_of_rect && y <= size_y / size_of_rect && x >= 0 && y >= 0)
				if (a[x][y] == 1)return true;
			return false;
		}
		void ret_coord(int index, int &x, int &y){

			x = coord[index].x * size_of_rect;
			y = coord[index].y * size_of_rect;
		}
		void clear(){
			std::vector<CPoint> ab;
			this->coord = ab;
			for (int i = 0; i < 1050; i++)
				for (int j = 0; j < 1050; j++)
					a[i][j] = 0;
			index = -1;
		}
		void set_size(int size_x, int size_y, int size_of_rect){
			this->size_x = size_x;
			this->size_y = size_y;
			int change = this->size_of_rect - size_of_rect;
			this->size_of_rect = size_of_rect;
			std::vector<CPoint> coord1;
			CPoint temp_cPoint;
			int ind = 0;
			for (int i = 0; i <= index; i++){
				if (coord[i].x < size_x / size_of_rect && coord[i].y < size_y / size_of_rect){

					coord1.push_back(coord[i]);
					ind++;
				}
				else{
					a[coord[i].x][coord[i].y] = 0;
				}
			}
			index = ind - 1;
			coord = coord1;
		}
		neighbors get_neighbors(int x, int y){
			neighbors nb;
			if (infinite_field == 1)
			{
				if (x < size_x / size_of_rect - 1 && y < size_y / size_of_rect - 1 && x>0 && y>0){//everyone
					nb.el[1].x = x - 1;				nb.el[1].y = y - 1;
					nb.el[2].x = x;					nb.el[2].y = y - 1;
					nb.el[3].x = x + 1;				nb.el[3].y = y - 1;
					nb.el[4].x = x + 1;				nb.el[4].y = y;
					nb.el[5].x = x + 1;				nb.el[5].y = y + 1;
					nb.el[6].x = x;					nb.el[6].y = y + 1;
					nb.el[7].x = x - 1;				nb.el[7].y = y + 1;
					nb.el[8].x = x - 1;				nb.el[8].y = y;
				}
				else if (x == 0 && y == 0){//hight left
					nb.el[1].x = size_x / size_of_rect - 1;	nb.el[1].y = size_y / size_of_rect - 1;
					nb.el[2].x = x;					nb.el[2].y = size_y / size_of_rect - 1;
					nb.el[3].x = x + 1;				nb.el[3].y = size_y / size_of_rect - 1;
					nb.el[4].x = x + 1;				nb.el[4].y = y;
					nb.el[5].x = x + 1;				nb.el[5].y = y + 1;
					nb.el[6].x = x;					nb.el[6].y = y + 1;
					nb.el[7].x = size_x / size_of_rect - 1;	nb.el[7].y = y + 1;
					nb.el[8].x = size_x / size_of_rect - 1;	nb.el[8].y = y;
				}
				else if (x == 0 && y > 0 && y < size_y / size_of_rect - 1){//left
					nb.el[1].x = size_x / size_of_rect - 1;		nb.el[1].y = y - 1;
					nb.el[2].x = x;					nb.el[2].y = y - 1;
					nb.el[3].x = x + 1;				nb.el[3].y = y - 1;
					nb.el[4].x = x + 1;				nb.el[4].y = y;
					nb.el[5].x = x + 1;				nb.el[5].y = y + 1;
					nb.el[6].x = x;					nb.el[6].y = y + 1;
					nb.el[7].x = size_x / size_of_rect - 1;	nb.el[7].y = y + 1;
					nb.el[8].x = size_x / size_of_rect - 1;	nb.el[8].y = y;
				}
				else if (x == 0 && y == size_y / size_of_rect - 1){//low left
					nb.el[1].x = size_x / size_of_rect - 1;	nb.el[1].y = y - 1;
					nb.el[2].x = x;					nb.el[2].y = y - 1;
					nb.el[3].x = x + 1;				nb.el[3].y = y - 1;
					nb.el[4].x = x + 1;				nb.el[4].y = y;
					nb.el[5].x = x + 1;				nb.el[5].y = 0;
					nb.el[6].x = 0;					nb.el[6].y = 0;
					nb.el[7].x = size_x / size_of_rect - 1;	nb.el[7].y = 0;
					nb.el[8].x = size_x / size_of_rect - 1;	nb.el[8].y = size_y / size_of_rect - 1;
				}
				else if (y == size_y / size_of_rect - 1 && x > 0 && x < size_x / size_of_rect - 1){//low
					nb.el[1].x = x - 1;				nb.el[1].y = y - 1;
					nb.el[2].x = x;					nb.el[2].y = y - 1;
					nb.el[3].x = x + 1;				nb.el[3].y = y - 1;
					nb.el[4].x = x + 1;				nb.el[4].y = y;
					nb.el[5].x = x + 1;				nb.el[5].y = 0;
					nb.el[6].x = x;					nb.el[6].y = 0;
					nb.el[7].x = x - 1;				nb.el[7].y = 0;
					nb.el[8].x = x - 1;				nb.el[8].y = y;
				}
				else if (x == size_x / size_of_rect - 1 && y == size_y / size_of_rect - 1){//low right
					nb.el[1].x = x - 1;				nb.el[1].y = y - 1;
					nb.el[2].x = x;					nb.el[2].y = y - 1;
					nb.el[3].x = 0;					nb.el[3].y = y - 1;
					nb.el[4].x = 0;					nb.el[4].y = y;
					nb.el[5].x = 0;					nb.el[5].y = 0;
					nb.el[6].x = x;					nb.el[6].y = 0;
					nb.el[7].x = x - 1;				nb.el[7].y = 0;
					nb.el[8].x = x - 1;				nb.el[8].y = y;
				}
				else if (y > 0 && x == size_x / size_of_rect - 1 && y < size_y / size_of_rect - 1){//right
					nb.el[1].x = x - 1;				nb.el[1].y = y - 1;
					nb.el[2].x = x;					nb.el[2].y = y - 1;
					nb.el[3].x = 0;					nb.el[3].y = y - 1;
					nb.el[4].x = 0;					nb.el[4].y = y;
					nb.el[5].x = 0;					nb.el[5].y = y + 1;
					nb.el[6].x = x;					nb.el[6].y = y + 1;
					nb.el[7].x = x - 1;				nb.el[7].y = y + 1;
					nb.el[8].x = x - 1;				nb.el[8].y = y;
				}
				else if (x == size_x / size_of_rect - 1 && y == 0){//hight right
					nb.el[1].x = size_x / size_of_rect - 2;	nb.el[1].y = size_y / size_of_rect - 1;
					nb.el[2].x = size_x / size_of_rect - 1;	nb.el[2].y = size_y / size_of_rect - 1;
					nb.el[3].x = 0;					nb.el[3].y = size_y / size_of_rect - 1;
					nb.el[4].x = 0;					nb.el[4].y = 0;
					nb.el[5].x = 0;					nb.el[5].y = 1;
					nb.el[6].x = x;					nb.el[6].y = y + 1;
					nb.el[7].x = x - 1;				nb.el[7].y = y + 1;
					nb.el[8].x = x - 1;				nb.el[8].y = y;
				}
				else if (x > 0 && x < size_x / size_of_rect - 1 && y == 0){//hight
					nb.el[1].x = x - 1;				nb.el[1].y = size_y / size_of_rect - 1;
					nb.el[2].x = x;					nb.el[2].y = size_y / size_of_rect - 1;
					nb.el[3].x = x + 1;				nb.el[3].y = size_y / size_of_rect - 1;
					nb.el[4].x = x + 1;				nb.el[4].y = y;
					nb.el[5].x = x + 1;				nb.el[5].y = y + 1;
					nb.el[6].x = x;					nb.el[6].y = y + 1;
					nb.el[7].x = x - 1;				nb.el[7].y = y + 1;
					nb.el[8].x = x - 1;				nb.el[8].y = y;
				}
			}
			else
			{
				if (x <= size_x / size_of_rect - 1 && y <= size_y / size_of_rect - 1 && x>=0 && y>=0){//everyone
					nb.el[1].x = x - 1;				nb.el[1].y = y - 1;
					nb.el[2].x = x;					nb.el[2].y = y - 1;
					nb.el[3].x = x + 1;				nb.el[3].y = y - 1;
					nb.el[4].x = x + 1;				nb.el[4].y = y;
					nb.el[5].x = x + 1;				nb.el[5].y = y + 1;
					nb.el[6].x = x;					nb.el[6].y = y + 1;
					nb.el[7].x = x - 1;				nb.el[7].y = y + 1;
					nb.el[8].x = x - 1;				nb.el[8].y = y;
				}
			}
			for (int i = 1; i <= 8; i++){
				if (ret_value(nb.el[i].x, nb.el[i].y, false) == true){ nb.count++; }
			}
			return nb;
		}
	};
	struct options {
		int x_main;
		int y_main;
		int rc_x_main;
		int rc_y_main;
		int size_of_rect;
		int open_file;
		int timer_sec;
		int change_with_window;
		int infinite_field;
		int all_steps;
		int all_steps_old;
	};
	
public:
	Workspace();
	~Workspace();
	void fToString(int fl, LPWSTR& s);
	//
	rectMatrix new_t, that, that_old, last_old, last, els, that_mult;
	options op;
	int fast_start_condition;
	int ers_bkg_param;
	int scroll_param;
	int on_scroll_Val;
	bool fast_start_func_exit;
	int start_cond;
	int set_change;
	int shapes;
	int l_mouse_ev, r_mouse_ev;
	int x_for_scroll = 0, y_for_scroll = 0;
	HANDLE hFieldMutex;
	HANDLE hRunMutex;
	CComAutoCriticalSection m_cs;
	HANDLE eWrite;
	HANDLE eStop;
	int stop;
	CPoint c_clean;
};



extern Workspace g_Workspace;
extern Workspace m1;
