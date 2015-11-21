// LifeGAMEView.cpp : implementation of the CLifeGAMEView class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "Windowsx.h"
#include "SetSizeDlg.h"
#include "MainFrm.h"
#include <windows.h>
#include <process.h>
#include <fstream>
#include <string>
#include <thread>
#include "atltypes.h"
#include "LifeGAMEView.h"
#include "Workspace.h"


BOOL CLifeGAMEView::PreTranslateMessage(MSG* pMsg)
{
	pMsg;
	return FALSE;
}

LRESULT CLifeGAMEView::OnPaint(UINT uMsg, WPARAM /*wParam*/, LPARAM /*lParam*/ , BOOL& /*bHandled*/)
{
	CPaintDC dc(m_hWnd);
	CRect rcClient;
	GetClientRect(rcClient);
	dc.FillSolidRect(g_Workspace.op.x_main + 1, 0, rcClient.Width(), rcClient.Height(), RGB(255, 255, 255));
	dc.FillSolidRect(0, g_Workspace.op.y_main + 1, rcClient.Width(), rcClient.Height(), RGB(255, 255, 255));
	g_Workspace.op.rc_x_main = rcClient.Width() / g_Workspace.op.size_of_rect * g_Workspace.op.size_of_rect;
	g_Workspace.op.rc_y_main = rcClient.Height() / g_Workspace.op.size_of_rect * g_Workspace.op.size_of_rect;
	int count_x_1, count_y_1;
	
	//change size of field with rcClient size
	if (g_Workspace.op.change_with_window == 1 && (g_Workspace.op.x_main != rcClient.Width() / g_Workspace.op.size_of_rect * g_Workspace.op.size_of_rect || g_Workspace.op.y_main != rcClient.Height() / g_Workspace.op.size_of_rect * g_Workspace.op.size_of_rect)){
		g_Workspace.op.x_main = rcClient.Width() / g_Workspace.op.size_of_rect * g_Workspace.op.size_of_rect;
		g_Workspace.op.y_main = rcClient.Height() / g_Workspace.op.size_of_rect * g_Workspace.op.size_of_rect;
		ResizeAll();
	}


	//on scroll
	if (g_Workspace.set_change == 1){
		if (g_Workspace.op.rc_x_main / g_Workspace.op.size_of_rect < g_Workspace.op.x_main / g_Workspace.op.size_of_rect && g_Workspace.op.rc_y_main / g_Workspace.op.size_of_rect < g_Workspace.op.y_main / g_Workspace.op.size_of_rect){ ScrollProc(3); }
		else if (g_Workspace.op.rc_x_main / g_Workspace.op.size_of_rect < g_Workspace.op.x_main / g_Workspace.op.size_of_rect){ ScrollProc(2); }
		else if (g_Workspace.op.rc_y_main / g_Workspace.op.size_of_rect < g_Workspace.op.y_main / g_Workspace.op.size_of_rect){ ScrollProc(1); }
		else { ScrollProc(0); }
	}


	//if size were 

	//if window was resized or scrolled cleans view or some options were changed
	if (g_Workspace.ers_bkg_param == 1 || g_Workspace.scroll_param == 1 || g_Workspace.set_change == 1){ dc.FillSolidRect(0, 0, rcClient.Width(), rcClient.Height(), RGB(255, 255, 255)); }

	
	//draw fields
	for (count_x_1 = 0; count_x_1 <= g_Workspace.op.x_main; count_x_1 += g_Workspace.op.size_of_rect){
		for (count_y_1 = 0; count_y_1 <= g_Workspace.op.y_main; count_y_1 += g_Workspace.op.size_of_rect){
			if (g_Workspace.op.size_of_rect >= 8)
			{
				dc.FillSolidRect(0, count_y_1, g_Workspace.op.x_main, 1, RGB(0, 0, 0));
				dc.FillSolidRect(count_x_1, 0, 1, g_Workspace.op.y_main, RGB(0, 0, 0));
			}
			if (count_y_1 > rcClient.Height() + g_Workspace.op.size_of_rect) break;
		}
		if (count_x_1 > rcClient.Width() + g_Workspace.op.size_of_rect) break;
	}

	//draw full rect
	if (g_Workspace.that.index != -1)
	{
		for (int i = 0; i <= g_Workspace.that.index; i++)
		{
			int x_coord = 0, y_coord = 0;
			g_Workspace.that.ret_coord(i, x_coord, y_coord);
			dc.FillSolidRect(x_coord + 1 - g_Workspace.x_for_scroll, y_coord + 1 - g_Workspace.y_for_scroll, g_Workspace.op.size_of_rect - 1, g_Workspace.op.size_of_rect - 1, RGB(255, 0, 0));
		}
	}
	//clean disappeared rect by last array
	if (g_Workspace.last.index != -1)
	{
		for (int i = 0; i <= g_Workspace.last.index; i++)
		{
			int x_coord = 0, y_coord = 0;
			g_Workspace.last.ret_coord(i, x_coord, y_coord);
			if (g_Workspace.that.ret_value(x_coord, y_coord, true) == false)
			{
				dc.FillSolidRect(x_coord + 1 - g_Workspace.x_for_scroll, y_coord + 1 - g_Workspace.y_for_scroll, g_Workspace.op.size_of_rect - 1, g_Workspace.op.size_of_rect - 1, RGB(255, 255, 255));
			}
		}
	}
	
	//set to zero all variable that match erase bkg options
	g_Workspace.set_change = 0;
	g_Workspace.ers_bkg_param = 0;
	return 0;
}

LRESULT CLifeGAMEView::EraseBKG(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/){
	return 0;
}



LRESULT CLifeGAMEView::OnLMouseDown(UINT uMsg, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	//cope last and that to prev step
	g_Workspace.that_old = g_Workspace.that;
	g_Workspace.last_old = g_Workspace.last;

	int xPos = LOWORD(lParam) + g_Workspace.x_for_scroll;
	int yPos = HIWORD(lParam) + g_Workspace.y_for_scroll;
	if (xPos < g_Workspace.op.x_main && yPos < g_Workspace.op.y_main){
		if (g_Workspace.that.ret_value(xPos, yPos, true) == true && g_Workspace.shapes == 0){
			g_Workspace.that.del(xPos, yPos, true);
			RedrawWindow();
		}
		else if (g_Workspace.that.ret_value(xPos, yPos, true) == false && g_Workspace.shapes == 0){
			g_Workspace.that.put(xPos, yPos, true);
			RedrawWindow();
		}
		else if (g_Workspace.shapes != 0){
			Shapes(xPos / g_Workspace.op.size_of_rect, yPos / g_Workspace.op.size_of_rect);
			RedrawWindow();
		}
	}

	//update last array
	g_Workspace.last = g_Workspace.that;
	g_Workspace.l_mouse_ev = 1;
	return 0;
}

LRESULT CLifeGAMEView::OnLMouseUp(UINT uMsg, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	g_Workspace.l_mouse_ev = 0;
	return 0;
}

LRESULT CLifeGAMEView::OnRMouseDown(UINT uMsg, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	g_Workspace.r_mouse_ev = 1;
	return 0;
}

LRESULT CLifeGAMEView::OnRMouseUp(UINT uMsg, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	g_Workspace.r_mouse_ev = 0;
	return 0;
}

LRESULT CLifeGAMEView::OnMouseMove(UINT uMsg, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	int xPos = LOWORD(lParam) + g_Workspace.x_for_scroll;
	int yPos = HIWORD(lParam) + g_Workspace.y_for_scroll;
	//if left button is down and don`t chose some shape
	if (g_Workspace.l_mouse_ev == 1){
		if (xPos < g_Workspace.op.x_main && yPos < g_Workspace.op.y_main && g_Workspace.shapes == 0){
			if (g_Workspace.that.ret_value(xPos, yPos, true) == false){
				g_Workspace.that.put(xPos, yPos, true);
				RedrawWindow();
			}
		}
		g_Workspace.last = g_Workspace.that;
	}
	//if right button is down and don`t chose some shape
	else if (g_Workspace.r_mouse_ev == 1){
		if (xPos < g_Workspace.op.x_main && yPos < g_Workspace.op.y_main){
			if (g_Workspace.that.ret_value(xPos, yPos, true) == true){
				g_Workspace.that.del(xPos, yPos, true);
				RedrawWindow();
			}
		}
	}

	return 0;
}

void CLifeGAMEView::Start(int a){
	//SetTimer(IDT_TIMER1, size_of_rect0, (TIMERPROC)MyTimerProc);
	///*CEvent*/ ev;
	//ev.
	BOOL ab = TRUE;
	//if normal start
	if (a == 0){
		if (!SetTimer(IDT_TIMER1, g_Workspace.op.timer_sec, NULL)){
			MyTimerProc(NULL, NULL, NULL, ab);
		}
	}
	//if fast start
	else if (a == 3){
		g_Workspace.hFieldMutex = CreateMutex(NULL, FALSE, NULL);
		g_Workspace.hRunMutex = CreateMutex(NULL, TRUE, NULL);
		if (!SetTimer(IDT_TIMER1, 1000, NULL)){
			_beginthread(this->FastStartProc, 4096, NULL);
			ReleaseMutex(g_Workspace.hRunMutex);
			RedrawWindow();
			if (g_Workspace.stop == 1){ KillTimer(IDT_TIMER1); }
		}
		g_Workspace.fast_start_condition = 1;
		
		
		/*CreateThread(NULL, 0, FastStartProc, &g_Workspace.hRunMutex, 0, NULL);*/

		
		CloseHandle(g_Workspace.hFieldMutex);
		CloseHandle(g_Workspace.hRunMutex);
	}
	//if pressed next step key
	else if (a == 1){ MyTimerProc(NULL, NULL, NULL, ab); }
	//if pressed prev step key
	else if (a == 2){
		g_Workspace.that.clear();
		g_Workspace.last.clear();
		g_Workspace.ers_bkg_param = 1;
		RedrawWindow();
		g_Workspace.that = g_Workspace.that_old;
		g_Workspace.last = g_Workspace.last_old;
		RedrawWindow();
	}
	g_Workspace.stop = 0;
}

void CLifeGAMEView::FastStartProc(void * ipMyID)
{
	do
	{
		WaitForSingleObject(g_Workspace.hFieldMutex, INFINITE);
		//cope last and that to prev step
		g_Workspace.that_old = g_Workspace.that;
		g_Workspace.last_old = g_Workspace.last;

		Workspace::neighbors nb, nb_temp;
		int ans = 1;
		ans = 0;
		int coord_x = 0, coord_y = 0;
		g_Workspace.new_t.clear();
		for (int i = 0; i < g_Workspace.that.index; i++){
			g_Workspace.that.ret_coord(i, coord_x, coord_y);
			nb = g_Workspace.that.get_neighbors(coord_x / g_Workspace.op.size_of_rect, coord_y / g_Workspace.op.size_of_rect);
			for (int j = 1; j < 9; j++){
				nb_temp = g_Workspace.that.get_neighbors(nb.el[j].x, nb.el[j].y);
				if (g_Workspace.that.ret_value(nb.el[j].x, nb.el[j].y, false) == true){
					if (nb_temp.count == 2 || nb_temp.count == 3){
						if (g_Workspace.new_t.ret_value(nb.el[j].x, nb.el[j].y, false) == false)
						{
							g_Workspace.new_t.put(nb.el[j].x, nb.el[j].y, false); ans = 1;
						}
					}
				}
				else if (g_Workspace.that.ret_value(nb.el[j].x, nb.el[j].y, false) == false){
					if (nb_temp.count == 3){
						if (g_Workspace.new_t.ret_value(nb.el[j].x, nb.el[j].y, false) == false)
						{
							g_Workspace.new_t.put(nb.el[j].x, nb.el[j].y, false);
						}
					}
				}
			}
		}
		ReleaseMutex(g_Workspace.hFieldMutex);	
		g_Workspace.last = g_Workspace.that;
		g_Workspace.that = g_Workspace.new_t;
	}
	while (WaitForSingleObject(g_Workspace.hRunMutex, INFINITE)==TRUE);
	
}

//next function main handler for life game algorithm
LRESULT CLifeGAMEView::MyTimerProc(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/){
	
	//cope last and that to prev step
	g_Workspace.that_old = g_Workspace.that;
	g_Workspace.last_old = g_Workspace.last;
	
	Workspace::neighbors nb,nb_temp;
	int ans = 1;
	ans = 0;
	int coord_x = 0, coord_y = 0;
	g_Workspace.new_t.clear();
	for (int i = 0; i < g_Workspace.that.index; i++){
		g_Workspace.that.ret_coord(i, coord_x, coord_y);
		nb = g_Workspace.that.get_neighbors(coord_x / g_Workspace.op.size_of_rect, coord_y / g_Workspace.op.size_of_rect);
		for (int j = 1; j < 9; j++){
			nb_temp = g_Workspace.that.get_neighbors(nb.el[j].x, nb.el[j].y);
			if (g_Workspace.that.ret_value(nb.el[j].x, nb.el[j].y, false) == true){
				if (nb_temp.count == 2 || nb_temp.count == 3){ 
					if (g_Workspace.new_t.ret_value(nb.el[j].x, nb.el[j].y, false) == false)
						{ g_Workspace.new_t.put(nb.el[j].x, nb.el[j].y, false); ans = 1; }
				}
			}
			else if (g_Workspace.that.ret_value(nb.el[j].x, nb.el[j].y, false) == false){
				if (nb_temp.count == 3){ if(g_Workspace.new_t.ret_value(nb.el[j].x, nb.el[j].y, false) == false)
					{g_Workspace.new_t.put(nb.el[j].x, nb.el[j].y, false);} 
				}
			}
		}
	}
	if (ans == 0){ g_Workspace.stop = 1; }

	g_Workspace.last = g_Workspace.that;
	g_Workspace.that = g_Workspace.new_t;
	RedrawWindow();
	if (g_Workspace.stop == 1){ KillTimer(IDT_TIMER1); }
	return 0;
}
void CLifeGAMEView::Stop(){
	g_Workspace.stop = 1;
}

void CLifeGAMEView::ScrollProc(int param){
	if (param == 1){
		ShowScrollBar(SB_VERT, 1);
		//	SetScrollRange(SB_VERT, 0, 200);
		SCROLLINFO si;
		si.cbSize = sizeof(si);
		si.fMask = SIF_PAGE | SIF_POS | SIF_RANGE;
		si.nMin = 0;
		si.nMax = (g_Workspace.op.y_main - g_Workspace.op.rc_y_main) / g_Workspace.op.size_of_rect + 1;
		si.nPage = si.nMax / g_Workspace.op.size_of_rect;
		si.nPos = 0;
		SetScrollInfo(SB_VERT, &si);
	}
	else if (param == 2){ 
		ShowScrollBar(SB_HORZ, 1);
		//	SetScrollRange(SB_VERT, 0, 200);
		SCROLLINFO si;
		si.cbSize = sizeof(si);
		si.fMask = SIF_PAGE | SIF_POS | SIF_RANGE;
		si.nMin = 0;
		si.nMax = (g_Workspace.op.x_main - g_Workspace.op.rc_x_main) / g_Workspace.op.size_of_rect + 1;
		si.nPage = si.nMax / g_Workspace.op.size_of_rect;
		si.nPos = 0;
		SetScrollInfo(SB_HORZ, &si);
	}
	else if (param == 3){
		ShowScrollBar(SB_VERT, 1);
		ShowScrollBar(SB_HORZ, 1);
		SCROLLINFO si1, si2;
		si1.cbSize = sizeof(si1);
		si1.fMask = SIF_PAGE | SIF_POS | SIF_RANGE;
		si1.nMin = 0;
		si1.nMax = (g_Workspace.op.x_main - g_Workspace.op.rc_x_main) / g_Workspace.op.size_of_rect + 1;
		si1.nPage = si1.nMax / g_Workspace.op.size_of_rect;
		si1.nPos = 0;
		si2.cbSize = sizeof(si2);
		si2.fMask = SIF_PAGE | SIF_POS | SIF_RANGE;
		si2.nMin = 0;
		si2.nMax = (g_Workspace.op.y_main - g_Workspace.op.rc_y_main) / g_Workspace.op.size_of_rect + 1;
		si2.nPage = si2.nMax / g_Workspace.op.size_of_rect;
		si2.nPos = 0;
		SetScrollInfo(SB_HORZ, &si1);
		SetScrollInfo(SB_VERT, &si2);
	}
	else if (param == 0){
		ShowScrollBar(SB_BOTH, 0);
	}
}
LRESULT CLifeGAMEView::OnVScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	SCROLLINFO si;
	si.cbSize = sizeof(si);
	si.fMask = SIF_PAGE | SIF_POS | SIF_RANGE;
	GetScrollInfo(SB_VERT, &si);
	switch (LOWORD(wParam))
	{
	case SB_PAGERIGHT:
	{if (si.nPos < si.nMax)		{
		si.nPos += 1;
		g_Workspace.y_for_scroll += g_Workspace.op.size_of_rect;
	}
	break; }
	case SB_LINERIGHT:
	{if (si.nPos < si.nMax)		{
		si.nPos += 1;
		g_Workspace.y_for_scroll += g_Workspace.op.size_of_rect;
	}
	break; }
	case SB_PAGELEFT:
	{si.nPos -= 1;
	g_Workspace.y_for_scroll -= g_Workspace.op.size_of_rect;
	break; }
	case SB_LINELEFT:
	{si.nPos -= 1;
	g_Workspace.y_for_scroll -= g_Workspace.op.size_of_rect;
	break; }
	case SB_TOP:
	{si.nPos = si.nMin; break; }
	case SB_BOTTOM:
	{si.nPos = si.nMax; break; }
	case SB_THUMBPOSITION:{
		WORD x1 = HIWORD(wParam);
		si.nPos = HIWORD(wParam);
		g_Workspace.y_for_scroll = x1 * g_Workspace.op.size_of_rect;
		break;
	}
	case SB_THUMBTRACK:
	{si.nPos = HIWORD(wParam); break; }

	}
	if (g_Workspace.x_for_scroll < 0)g_Workspace.x_for_scroll = 0;
	if (g_Workspace.y_for_scroll < 0)g_Workspace.y_for_scroll = 0;
	g_Workspace.scroll_param = 1;
	RedrawWindow();
	g_Workspace.scroll_param = 0;
	SetScrollInfo(SB_VERT, &si);

	return 0;
}
LRESULT CLifeGAMEView::OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	if(GetAsyncKeyState(VK_LCONTROL)==true);
	SCROLLINFO si1, si2; 
	si1.cbSize = sizeof(si1);
	si1.fMask = SIF_PAGE | SIF_POS | SIF_RANGE;
	si2.cbSize = sizeof(si2);
	si2.fMask = SIF_PAGE | SIF_POS | SIF_RANGE;
	GetScrollInfo(SB_HORZ, &si1);
	GetScrollInfo(SB_VERT, &si2);
	int m = HIWORD(wParam);
	if (LOWORD(wParam) == MK_CONTROL){ 
		if (m == 120){
			g_Workspace.op.size_of_rect += 1;
			g_Workspace.ers_bkg_param = 1;
			ResizeAll();
		}
		else if (m > 140){
			g_Workspace.op.size_of_rect -= 1;
			g_Workspace.ers_bkg_param = 1;
			ResizeAll();
		}
	}
	else if (LOWORD(wParam) == MK_SHIFT){
		if (m == 120){
			if (si1.nPos >= 0)
			{
				g_Workspace.x_for_scroll -= g_Workspace.op.size_of_rect;
				si1.nPos -= 1;
			}
		}
		else if (m > 140){
			if (si1.nPos < si1.nMax)
			{
				g_Workspace.x_for_scroll += g_Workspace.op.size_of_rect;
				si1.nPos += 1;
			}
		}
	}
	else{
		if (m == 120){
			if (si2.nPos >= 0)
			{
				g_Workspace.y_for_scroll -= g_Workspace.op.size_of_rect;
				si2.nPos -= 1;
			}
		}
		else if (m > 140){
			if (si2.nPos < si2.nMax)
			{
				g_Workspace.y_for_scroll += g_Workspace.op.size_of_rect;
				si2.nPos += 1;
			}
		}
		if (g_Workspace.x_for_scroll < 0)g_Workspace.x_for_scroll = 0;
		if (g_Workspace.y_for_scroll < 0)g_Workspace.y_for_scroll = 0;
	}

	g_Workspace.scroll_param = 1;
	RedrawWindow();
	g_Workspace.scroll_param = 0;
	SetScrollInfo(SB_VERT, &si2);
	SetScrollInfo(SB_HORZ, &si1);

	return 0;
}

LRESULT CLifeGAMEView::OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	SCROLLINFO si;
	si.cbSize = sizeof(si);
	si.fMask = SIF_PAGE | SIF_POS | SIF_RANGE;
	GetScrollInfo(SB_HORZ, &si);
	switch (LOWORD(wParam))
	{
	case SB_PAGERIGHT:
	{if (si.nPos <= si.nMax)	{
		si.nPos += 1;
		g_Workspace.x_for_scroll += g_Workspace.op.size_of_rect;
	}
	break; }
	case SB_LINERIGHT:
	{if (si.nPos <= si.nMax)		{
		si.nPos += 1;
		g_Workspace.x_for_scroll += g_Workspace.op.size_of_rect;
	}
	break; }
	case SB_PAGELEFT:
	{si.nPos -= 1;
	g_Workspace.x_for_scroll -= g_Workspace.op.size_of_rect;
	break; }
	case SB_LINELEFT:
	{si.nPos -= 1;
	g_Workspace.x_for_scroll -= g_Workspace.op.size_of_rect;
	break; }
	case SB_TOP:
	{si.nPos = si.nMin; break; }
	case SB_BOTTOM:
	{si.nPos = si.nMax; break; }
	case SB_THUMBPOSITION:{
		WORD x1 = HIWORD(wParam);
		si.nPos = HIWORD(wParam);
		g_Workspace.x_for_scroll = x1 * g_Workspace.op.size_of_rect;
		break;
	}
	case SB_THUMBTRACK:
	{si.nPos = HIWORD(wParam); break; }

	}
	if (g_Workspace.x_for_scroll < 0)g_Workspace.x_for_scroll = 0;
	if (g_Workspace.y_for_scroll < 0)g_Workspace.y_for_scroll = 0;
	g_Workspace.scroll_param = 1;
	RedrawWindow();
	g_Workspace.scroll_param = 0;
	SetScrollInfo(SB_HORZ, &si);

	return 0;
}

void CLifeGAMEView::Clear(){
	//clear all
	g_Workspace.last = g_Workspace.that;
	g_Workspace.that.clear();
	g_Workspace.new_t.clear();
	g_Workspace.last_old.clear();
	g_Workspace.that_old.clear();

	g_Workspace.x_for_scroll = 0;
	g_Workspace.y_for_scroll = 0;
	RedrawWindow();
	g_Workspace.last.clear();

}
void CLifeGAMEView::SetSize(int x,int y){
	g_Workspace.op.x_main = x;
	g_Workspace.op.y_main = y;
}
LRESULT CLifeGAMEView::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/){
	g_Workspace.op.x_main = 300;
	g_Workspace.op.y_main = 300;
	g_Workspace.op.infinite_field = 1;
	g_Workspace.op.open_file = 0;
	g_Workspace.op.change_with_window = 0;
	g_Workspace.op.timer_sec = 100;
	g_Workspace.op.size_of_rect = 15;
	g_Workspace.that.size_x = g_Workspace.op.x_main;
	g_Workspace.that.size_y = g_Workspace.op.y_main;
	g_Workspace.that.size_of_rect = g_Workspace.op.size_of_rect;
	g_Workspace.last = g_Workspace.that;
	g_Workspace.new_t = g_Workspace.that;
	//g_Workspace.els = g_Workspace.that;
	//g_Workspace.old = g_Workspace.that;
//	ShowScrollBar(SB_BOTH, 1);
////	SetScrollRange(SB_VERT, 0, 200);
//	SCROLLINFO si;
//	si.cbSize = sizeof(si);
//	si.fMask = SIF_PAGE | SIF_POS | SIF_RANGE;
//	si.nMin = 0;
//	si.nMax = 100;
//	si.nPage = 5;
//	si.nPos = 50;
//	SetScrollInfo(SB_VERT, &si);
	return 0;
}


void CLifeGAMEView::NewGame(){
	g_Workspace.stop = 1;
	g_Workspace.that.clear();
	RedrawWindow();
}
void CLifeGAMEView::ResizeAll(){
	
	//change main size to rc size
	g_Workspace.that.set_size(g_Workspace.op.x_main, g_Workspace.op.y_main, g_Workspace.op.size_of_rect);
	g_Workspace.last.set_size(g_Workspace.op.x_main, g_Workspace.op.y_main, g_Workspace.op.size_of_rect);
	g_Workspace.new_t.set_size(g_Workspace.op.x_main, g_Workspace.op.y_main, g_Workspace.op.size_of_rect);
	g_Workspace.that_old.set_size(g_Workspace.op.x_main, g_Workspace.op.y_main, g_Workspace.op.size_of_rect);
	g_Workspace.last_old.set_size(g_Workspace.op.x_main, g_Workspace.op.y_main, g_Workspace.op.size_of_rect);
	g_Workspace.els.set_size(g_Workspace.op.x_main, g_Workspace.op.y_main, g_Workspace.op.size_of_rect);
	RedrawWindow();
}

LRESULT CMainFrame::OnFileSaveas(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	using namespace std;
	LPTSTR url;
	LPCTSTR szFilter(L"Text files (*.txt)|*.txt|");
	LPTSTR s1 = {L" "};
	// TODO: Add your command handler code here
	CFileDialog lg1(FALSE, L"txt", L"life_game_files", OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,
		L"Camera Data Files (*.txt)|*.txt|*.dat||", m_hWnd);
	if (lg1.DoModal() == IDOK){
		url=lg1.m_ofn.lpstrFile;
		char a[1024];
		string d;
		int i = 0;
		for (i = 0; i <= 1024; i++){
			if (!url[i]) break;
		}
		for (int j = 0; j < i; j++){
			d += url[j]; 
		}
		int index_of_arr = g_Workspace.that.index;
		ofstream fout(d);
		fout << g_Workspace.op.x_main << " / " << g_Workspace.op.y_main << " / " << g_Workspace.op.change_with_window << " / " << g_Workspace.op.size_of_rect << " / " << g_Workspace.op.timer_sec << " / " << g_Workspace.shapes << " / " << index_of_arr << " / ";
		for (int z = 0; z < index_of_arr; z++){
			fout << g_Workspace.that.coord[z].x << " / " << g_Workspace.that.coord[z].y << " / ";
		}
		
	}
	return 0;
}

LRESULT CMainFrame::OnFileOpen(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your command handler code here
	using namespace std;
	g_Workspace.that.clear();
	g_Workspace.last.clear();
	
	LPTSTR url;
	LPCTSTR szFilter(L"Text files (*.txt)|*.txt|");
	LPTSTR s1 = { L" " };
	// TODO: Add your command handler code here
	CFileDialog lg1(TRUE, L"txt", L"life_game_files", OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,
		L"Camera Data Files (*.txt)|*.txt|*.dat||", m_hWnd);
	if (lg1.DoModal() == IDOK){
		url = lg1.m_ofn.lpstrFile;
		char a[1024];
		string d;
		int i = 0;
		for (i = 0; i <= 1024; i++){
			if (!url[i]) break;
		}
		for (int j = 0; j < i; j++){
			d += url[j];
		}
		char bv;
		const string q = d;
		ifstream fin;
		fin.open(d, ios::in);
		int index_of_arr;
		fin >> g_Workspace.op.x_main >> a >> g_Workspace.op.y_main >> a >> g_Workspace.op.change_with_window >> a >> g_Workspace.op.size_of_rect >> a >> g_Workspace.op.timer_sec >> a >> g_Workspace.shapes >> a >> index_of_arr >> a;
		m_view.RedrawWindow();
		for (int z = 0; z < index_of_arr; z++){
			int x, y;
			fin >> x >> a >> y >> a;
			g_Workspace.that.put(x, y, true);
		}
	}
	g_Workspace.els = g_Workspace.that;
	g_Workspace.op.open_file = 1;
	g_Workspace.set_change = 1;
	m_view.RedrawWindow();
	
	return 0;
}

LRESULT CMainFrame::OnShapesRound(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your command handler code here
	g_Workspace.shapes = 1;
	return 0;
}

LRESULT CMainFrame::OnShapesRectangle(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your command handler code here
	g_Workspace.shapes = 2;
	return 0;
}

LRESULT CMainFrame::OnShapesTriangle(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your command handler code here
	g_Workspace.shapes = 3;
	return 0;
}

LRESULT CMainFrame::OnShapesEllipse(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your command handler code here
	g_Workspace.shapes = 4;
	return 0;
}

LRESULT CMainFrame::OnShapesStar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your command handler code here
	g_Workspace.shapes = 5;
	return 0;
}
     
LRESULT CMainFrame::OnShapesGliter(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your command handler code here
	g_Workspace.shapes = 6;
	return 0;
}

LRESULT CMainFrame::OnShapesDot(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your command handler code here
	g_Workspace.shapes = 0;
	return 0;
}

LRESULT CMainFrame::OnShapesSpaceshatle(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your command handler code here
	g_Workspace.shapes = 7;
	return 0;
}

void CLifeGAMEView::Shapes(int x,int y){
	//SetTimer(IDT_TIMER1, size_of_rect0, (TIMERPROC)MyTimerProc);
	switch (g_Workspace.shapes)
	{
	case 1:
		//g_Workspace.that.cont[g_Workspace.that.ret_index(x, y, 1)] = 0;
		g_Workspace.that.put(x - 3, y, false);
		g_Workspace.that.put(x - 3, y + 1, false);
		g_Workspace.that.put(x - 1, y + 3, false);
		g_Workspace.that.put(x - 2, y + 2, false);
		g_Workspace.that.put(x, y + 3, false);
		g_Workspace.that.put(x + 1, y + 3, false);
		g_Workspace.that.put(x + 2, y + 2, false);
		g_Workspace.that.put(x + 3, y + 1, false);
		g_Workspace.that.put(x + 3, y, false);
		g_Workspace.that.put(x + 3, y - 1, false);
		g_Workspace.that.put(x + 2, y - 2, false);
		g_Workspace.that.put(x + 1, y - 3, false);
		g_Workspace.that.put(x, y - 3, false);
		g_Workspace.that.put(x - 1, y - 3, false);
		g_Workspace.that.put(x - 2, y - 2, false);
		g_Workspace.that.put(x - 3, y - 1, false);
		break;
	case 2:
		g_Workspace.that.put(x - 3, y, false);
		g_Workspace.that.put(x - 3, y + 1, false);
		g_Workspace.that.put(x - 1, y + 3, false);
		g_Workspace.that.put(x, y + 3, false);
		g_Workspace.that.put(x + 1, y + 3, false);
		g_Workspace.that.put(x + 3, y + 1, false);
		g_Workspace.that.put(x + 3, y, false);
		g_Workspace.that.put(x + 3, y - 1, false);
		g_Workspace.that.put(x + 1, y - 3, false);
		g_Workspace.that.put(x, y - 3, false);
		g_Workspace.that.put(x - 1, y - 3, false);
		g_Workspace.that.put(x - 3, y - 1, false);
		g_Workspace.that.put(x - 3, y - 3, false);
		g_Workspace.that.put(x + 3, y + 3, false);
		g_Workspace.that.put(x - 3, y + 3, false);
		g_Workspace.that.put(x + 3, y - 3, false);
		g_Workspace.that.put(x - 3, y - 2, false);
		g_Workspace.that.put(x - 3, y + 2, false);
		g_Workspace.that.put(x + 3, y + 2, false);
		g_Workspace.that.put(x + 3, y - 2, false);
		g_Workspace.that.put(x + 2, y - 3, false);
		g_Workspace.that.put(x - 2, y - 3, false);
		g_Workspace.that.put(x + 2, y + 3, false);
		g_Workspace.that.put(x - 2, y + 3, false);
		break;
	case 3:

		g_Workspace.that.put(x - 3, y + 1, false);
		g_Workspace.that.put(x - 2, y, false);
		g_Workspace.that.put(x - 1, y - 1, false);
		g_Workspace.that.put(x, y - 2, false);
		g_Workspace.that.put(x + 1, y - 1, false);
		g_Workspace.that.put(x+2, y, false);
		g_Workspace.that.put(x + 3, y + 1, false);
		g_Workspace.that.put(x + 2, y + 1, false);
		g_Workspace.that.put(x + 1, y + 1, false);
		g_Workspace.that.put(x, y + 1, false);
		g_Workspace.that.put(x - 1, y + 1, false);
		g_Workspace.that.put(x - 2, y + 1, false);
		break;
	case 4:
		g_Workspace.that.put(x - 4, y, false);
		g_Workspace.that.put(x - 4, y + 1, false);
		g_Workspace.that.put(x - 2, y + 2, false);
		g_Workspace.that.put(x - 1, y + 3, false);
		g_Workspace.that.put(x, y + 3, false);
		g_Workspace.that.put(x + 1, y + 3, false);
		g_Workspace.that.put(x + 2, y + 2, false);
		g_Workspace.that.put(x + 4, y + 1, false);
		g_Workspace.that.put(x + 4, y, false);
		g_Workspace.that.put(x + 4, y - 1, false);
		g_Workspace.that.put(x + 2, y - 2, false);
		g_Workspace.that.put(x + 1, y - 3, false);
		g_Workspace.that.put(x, y - 3, false);
		g_Workspace.that.put(x - 1, y - 3, false);
		g_Workspace.that.put(x - 2, y - 2, false);
		g_Workspace.that.put(x - 4, y - 1, false);
		g_Workspace.that.put(x - 3, y + 2, false);
		g_Workspace.that.put(x + 3, y + 2, false);
		g_Workspace.that.put(x - 3, y - 2, false);
		g_Workspace.that.put(x + 3, y - 2, false);
		break;
	case 5:
		g_Workspace.that.put(x - 1, y, false);
		g_Workspace.that.put(x - 2, y, false);
		g_Workspace.that.put(x + 1, y, false);
		g_Workspace.that.put(x + 2, y, false);
		g_Workspace.that.put(x, y - 1, false);
		g_Workspace.that.put(x, y - 2, false);
		g_Workspace.that.put(x, y + 1, false);
		g_Workspace.that.put(x - 1, y + 2, false);
		g_Workspace.that.put(x + 1, y + 2, false);
		break;
	case 6:
		g_Workspace.that.put(x - 1, y, false);
		g_Workspace.that.put(x, y + 1, false);
		g_Workspace.that.put(x + 1, y + 1, false);
		g_Workspace.that.put(x + 1, y, false);
		g_Workspace.that.put(x + 1, y - 1, false);
		break;
	case 7:
		g_Workspace.that.put(x, y, false);
		g_Workspace.that.put(x + 1, y - 1, false);
		g_Workspace.that.put(x + 1, y + 1, false);
		g_Workspace.that.put(x + 2, y, false);
		g_Workspace.that.put(x + 3, y, false);
		g_Workspace.that.put(x - 2, y, false);
		g_Workspace.that.put(x - 3, y, false);
		g_Workspace.that.put(x - 1, y, false);
		g_Workspace.that.put(x - 6, y, false);
		g_Workspace.that.put(x - 5, y, false);
		g_Workspace.that.put(x - 4, y - 1, false);
		g_Workspace.that.put(x - 4, y + 1, false);
		break;
	default:
		break;
	}
	g_Workspace.stop = 0;
}

