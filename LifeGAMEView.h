// LifeGAMEView.h : interface of the CLifeGAMEView class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CLifeGAMEView : public CWindowImpl<CLifeGAMEView>
{
public:
	DECLARE_WND_CLASS(NULL)

	BOOL PreTranslateMessage(MSG* pMsg);

	BEGIN_MSG_MAP(CLifeGAMEView)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_MOUSEWHEEL, OnMouseWheel)
		MESSAGE_HANDLER(WM_ERASEBKGND, EraseBKG)
		MESSAGE_HANDLER(WM_TIMER, MyTimerProc)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		MESSAGE_HANDLER(WM_LBUTTONUP, OnLMouseUp)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLMouseDown)
		MESSAGE_HANDLER(WM_RBUTTONUP, OnRMouseUp)
		MESSAGE_HANDLER(WM_RBUTTONDOWN, OnRMouseDown)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_VSCROLL, OnVScroll)
		MESSAGE_HANDLER(WM_HSCROLL, OnHScroll)
		MESSAGE_HANDLER(WM_ERASEBKGND, EraseBKG)
		
		//COMMAND_HANDLER(ID_BUT_SET, BN_CLICKED, OnBnClickedButSet)
		
		
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
#define IDT_TIMER1 5085
#define IDT_TIMER2 5086
#define IDC_SCROLLBARCTRL 5087
	int stop = 0;
	int stan = 0;
	int size_x_last = 0;
	int size_y_last = 0;
	int c_rect_hei = 0;
	int c_rect_wei = 0;
	int c_rect_hei_last = 0;
	int c_rect_wei_last = 0;
	int x_main_last = 0;
	int y_main_last = 0;
	int shapes = 0;
	int q = 300;
	
	void Start(int a);
	void Stop();
	void Clear();
	void NewGame();
	void ScrollProc(int param);
	void ResizeAll();
	void Shapes(int x,int y);
	LRESULT MyTimerProc(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	void SetSize(int x,int y);
	LRESULT OnPaint(UINT uMsg, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnVScroll(UINT uMsg, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnHScroll(UINT uMsg, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnMouseWheel(UINT uMsg, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnLMouseDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnLMouseUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnRMouseDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnRMouseUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT EraseBKG(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/);
	static void FastStartProc(void * id);
};
