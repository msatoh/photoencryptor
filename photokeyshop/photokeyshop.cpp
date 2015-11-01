// photokeyshop.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "photokeyshop.h"

// グローバル変数:
HINSTANCE hInst;								// 現在のインターフェイス
TCHAR szTitle[MAX_LOADSTRING];					// タイトル バーのテキスト
TCHAR szWindowClass[MAX_LOADSTRING];			// メイン ウィンドウ クラス名

// このコード モジュールに含まれる関数の宣言を転送します:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	EncProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	DecProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: ここにコードを挿入してください。
	MSG msg;
	HACCEL hAccelTable;

	// グローバル文字列を初期化しています。
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_PHOTOKEYSHOP, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// アプリケーションの初期化を実行します:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PHOTOKEYSHOP));

	// メイン メッセージ ループ:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PHOTOKEYSHOP));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_PHOTOKEYSHOP);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します。
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // グローバル変数にインスタンス処理を格納します。

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND	- アプリケーション メニューの処理
//  WM_PAINT	- メイン ウィンドウの描画
//  WM_DESTROY	- 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	LPCWSTR str = TEXT("画像変換ソフト（仮）");
	RECT rec;
	HWND buttonenc, buttondec;

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 選択されたメニューの解析:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;

		case BUTTON_ENC_ID:
			// 子ウィンドウencの生成
			HWND windowenc;
			WNDCLASSW wc;
			wc.style = CS_HREDRAW | CS_VREDRAW;
			wc.lpfnWndProc = EncProc; // プロシージャ名
			wc.cbClsExtra = 0;
			wc.cbWndExtra = 0;
			wc.hInstance = hInst;
			wc.hIcon = NULL;
			wc.hCursor = NULL;
			wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(GRAY_BRUSH)); //グレーにする
			wc.lpszMenuName = NULL;  // 未サポート
			wc.lpszClassName = (LPCWSTR)TEXT("encryptor");
			RegisterClass(&wc);
			windowenc = CreateWindow(wc.lpszClassName, NULL,WS_VISIBLE | WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
				hWnd, NULL, hInst, NULL);
			break;
		case BUTTON_DEC_ID:
			// 子ウィンドウdecの生成
			HWND windowdec;
			WNDCLASSW wdc;
			wdc.style = CS_HREDRAW | CS_VREDRAW;
			wdc.lpfnWndProc = DecProc; // プロシージャ名
			wdc.cbClsExtra = 0;
			wdc.cbWndExtra = 0;
			wdc.hInstance = hInst;
			wdc.hIcon = NULL;
			wdc.hCursor = NULL;
			wdc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(GRAY_BRUSH)); //グレーにする
			wdc.lpszMenuName = NULL;  // 未サポート
			wdc.lpszClassName = (LPCWSTR)TEXT("decryptor");
			RegisterClass(&wdc);
			windowdec = CreateWindow(wdc.lpszClassName, NULL, WS_VISIBLE | WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
				hWnd, NULL, hInst, NULL);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 描画コードをここに追加してください...
		GetClientRect(hWnd, &rec);
		TextOut(hdc, rec.right / 2 - 92, 10, str, lstrlen(str));

		buttonenc = CreateWindow( TEXT("BUTTON"), TEXT("画像を変換する"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			rec.right / 2 - 200, 70, 400, 100,
			hWnd, (HMENU)BUTTON_ENC_ID, hInst, NULL
			);
		buttondec = CreateWindow( TEXT("BUTTON"), TEXT("変換した画像を元に戻す"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			rec.right / 2 - 200, rec.bottom - 150, 400, 100,
			hWnd, (HMENU)BUTTON_DEC_ID, hInst, NULL
			);


		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// バージョン情報ボックスのメッセージ ハンドラーです。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
