#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <tchar.h>
#include "resource.h"
#include "stdafx.h"
#include "photokeyshop.h"

#pragma comment(lib, "comctl32.lib")

#define pb_step 5
#define pb_min 0
#define pb_max 100

UINT iProg; // 現在のプログレスバーの位置

// このコード モジュールに含まれる関数の宣言を転送します:
BOOL CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR lpCmdLine, int nCmdShow)
{
	DialogBox(hInstance, TEXT("DIALOG_BOX"), NULL, DialogProc);
	return 0;
}

BOOL CALLBACK DialogProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HWND hProg;
	TCHAR str[10];
	hProg = GetDlgItem(hWnd, IDC_PROGRESS1);
	switch (msg)
	{
	case WM_INITDIALOG:
		InitCommonControls();
		SendMessage(hProg, PBM_SETRANGE, (WPARAM)0, MAKELPARAM(pb_min, pb_max));
		SendMessage(hProg, PBM_SETSTEP, (WPARAM)pb_step, 0);
		SetTimer(hWnd, IDC_TIMER, 1000, NULL);  /*1000[ms]タイマーイベント*/;
		break;
	case WM_TIMER:
		iProg += pb_step;
		if (iProg >= 100)
		{
			KillTimer(hWnd, IDC_TIMER);
		}
		SendMessage(GetDlgItem(hWnd, IDC_PROGRESS1), PBM_STEPIT, 0, 0);
		_stprintf_s(str, 10, TEXT("%d ％"), iProg);
		SetWindowText(GetDlgItem(hWnd, IDC_STATIC1), (LPCTSTR)str);
		break;
	case WM_CLOSE:
		KillTimer(hWnd, IDC_TIMER);
		EndDialog(hWnd, IDOK);
		break;
	}
	return FALSE;
}
