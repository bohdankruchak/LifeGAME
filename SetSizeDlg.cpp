// aboutdlg.cpp : implementation of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "resource.h"
#include "setsizedlg.h"
#include "aboutdlg.h"
#include "LifeGAMEView.h"
#include "LifeGAME.h"
#include "MainFrm.h"
#include "Workspace.h"


LRESULT CSetSizeDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}

LRESULT CSetSizeDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CenterWindow(GetParent());
	SetDlgItemInt(IDC_EDIT1, g_Workspace.op.x_main / g_Workspace.op.size_of_rect);
	SetDlgItemInt(IDC_EDIT2, g_Workspace.op.y_main / g_Workspace.op.size_of_rect);
	SetDlgItemInt(IDC_EDIT3, g_Workspace.op.timer_sec);
	SetDlgItemInt(IDC_EDIT4, g_Workspace.op.size_of_rect);
	CheckDlgButton(IDC_CHECK1, g_Workspace.op.change_with_window);
	CheckDlgButton(IDC_CHECK_INF, g_Workspace.op.infinite_field);

	return 0;
	return TRUE;
}

LRESULT CSetSizeDlg::OnBnClickedRadio2(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here

	return 0;
}




LRESULT CSetSizeDlg::OnBnClickedCheck1(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here

	return 0;
}

LRESULT CSetSizeDlg::OnBnClickedButSet(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	g_Workspace.op.size_of_rect = GetDlgItemInt(IDC_EDIT4);
	g_Workspace.op.x_main = GetDlgItemInt(IDC_EDIT1) * g_Workspace.op.size_of_rect;
	g_Workspace.op.y_main = GetDlgItemInt(IDC_EDIT2) * g_Workspace.op.size_of_rect;
	g_Workspace.op.timer_sec = GetDlgItemInt(IDC_EDIT3);
	g_Workspace.set_change = 1;
	if (IsDlgButtonChecked(IDC_CHECK1) != 0)
		g_Workspace.op.change_with_window = 1;
	else
		g_Workspace.op.change_with_window = 0;

	if (IsDlgButtonChecked(IDC_CHECK_INF) != 0)
		g_Workspace.op.infinite_field = 1;
	else
		g_Workspace.op.infinite_field = 0;
	g_Workspace.set_change = 1;
	g_Workspace.that.size_x = g_Workspace.op.x_main;
	g_Workspace.that.size_y = g_Workspace.op.y_main;
	g_Workspace.that.size_of_rect = g_Workspace.op.size_of_rect;
	g_Workspace.that.infinite_field = g_Workspace.op.infinite_field;
	g_Workspace.that_old = g_Workspace.that;
	g_Workspace.last_old = g_Workspace.that;
	g_Workspace.last = g_Workspace.that;
	g_Workspace.els = g_Workspace.that;
	g_Workspace.new_t = g_Workspace.that;

	RedrawWindow();
	EndDialog(wID);
	return 0;
}

