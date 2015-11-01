#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include "bmp.h"
#include "stdafx.h"
#include "photokeyshop.h"
#include<locale.h>
#define BUTTON_ID 1003

//色を変換するテーブル
typedef struct{
	color original_color;//変換前の色
	color converted_color;//変換後の色
}angou;

bool yonda = true;

bool Decryption(char *file_in_name, char *key_in_file){
	//初期化
	char *file_out_name;//画像の出力ファイル名
	img *imagein, *imageout;
	int h, w;
	int i = 0;
	angou *code;//色を変換するテーブル
	FILE* fp;

	file_out_name = (char *)malloc(sizeof(char)*(strlen(file_in_name) + 11));
	strcpy(file_out_name, file_in_name);
	strcat(file_out_name, "_recov.bmp");
	imagein = (img *)malloc(sizeof(img));
	imageout = (img *)malloc(sizeof(img));
	ReadBmp(file_in_name, imagein);
	code = (angou *)malloc(sizeof(angou)*imagein->height*imagein->width);
	imageout = imagein;
	if ((fp = fopen(key_in_file, "r")) == NULL){
		MessageBox(NULL, TEXT("key file output error."), NULL, MB_OK);
		return false;
	}
	fread(code, sizeof(angou), imagein->height*imagein->width, fp);//鍵ファイルに変換情報を記録
	//画像処理  
	for (h = 0; h < imagein->height; h++){
		for (w = 0; w < imagein->width; w++){
			for (i = 0; i <= imagein->height*imagein->width; i++){
				if (memcmp(&code[i].converted_color,
					&imagein->data[h][w],
					sizeof(imagein->data[h][w])) == 0){
					//登録してある色が見つかったら代入
					imageout->data[h][w] = code[i].original_color;
					break;
				}
			}
			//MessageBox(NULL, _T("登録された色が見つかりませんでした。鍵ファイルが正しくないかもしれません..."), NULL, MB_OK);
			//return false;

		//searched_out:
		//	/*
		//	あほ
		//	あほ
		//	*/;
		}
	}

	WriteBmp(file_out_name, imageout);

	//後処理  
	fclose(fp);
	//free(file_out_name);
	free(code);
	return true;
}

LRESULT CALLBACK DecProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp){
	PAINTSTRUCT ps;
	static HWND hButton;
	HDC hdc, hMdc;
	TCHAR buf[256];
	static int flag = 0;
	static WCHAR chr[MAXLENGTH + MAX_PATH];
	char filename[MAXLENGTH + MAX_PATH] = { '\0' }, keyfilename[MAXLENGTH + MAX_PATH] = { '\0' }, outfilename[MAXLENGTH + MAX_PATH] = { '\0' };
	static BITMAP bm;
	static OPENFILENAME ofn = { 0 }, keyfile = { 0 };
	static TCHAR strFile[MAX_PATH], strCustom[256] = TEXT("Before files\0*.*\0\0");
	static HBITMAP hBitmap;

	switch (msg){
	case WM_CREATE:
		setlocale(LC_ALL, "japanese");
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = hWnd;
		ofn.lpstrFilter = TEXT("Bitmap files {*.bmp}\0*.bmp\0")
			/*TEXT("他のファイルも同様の形式で")*/;
		ofn.lpstrCustomFilter = strCustom;
		ofn.nMaxCustFilter = 256;
		ofn.nFilterIndex = 0;
		ofn.lpstrFile = strFile;
		ofn.nMaxFile = MAX_PATH;
		ofn.Flags = OFN_FILEMUSTEXIST;
		GetOpenFileName(&ofn);
		// プッシュボタンを作成する
		hButton = CreateWindow(
			_T("BUTTON"),                            // ウィンドウクラス名
			_T("GO"),                             // キャプション
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,   // スタイル指定
			bm.bmWidth + 20, 200,                                // 座標
			60, 40,                                  // サイズ
			hWnd,                                    // 親ウィンドウのハンドル
			(HMENU)BUTTON_ID,                        // メニューハンドル
			((LPCREATESTRUCT)lp)->hInstance,                                   // インスタンスハンドル
			NULL                                     // その他の作成データ
			);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);     // 描画処理を開始します。
		SetBkMode(hdc, TRANSPARENT);    // 背景を透過モードにします
		if (flag == 0){
			hBitmap = (HBITMAP)LoadImage(NULL, (LPCWSTR)ofn.lpstrFile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			//エラー処理
			if (hBitmap == NULL){
				MessageBox(hWnd, TEXT("FailedToOpenFile"), TEXT("はい..."), MB_OK);
				PostMessage(hWnd, WM_CLOSE, 0, 0);
				EndPaint(hWnd, &ps);
				break;
			}
			hMdc = CreateCompatibleDC(hdc);
			SelectObject(hMdc, hBitmap);
			GetObject(hBitmap, sizeof(BITMAP), &bm);
			BitBlt(hdc, 5, 5, bm.bmWidth, bm.bmHeight, hMdc, 0, 0, SRCCOPY);
			flag = -1;
		}
		else{
			//ひらいた画像ファイルを表示
			hBitmap = (HBITMAP)LoadImage(NULL, (LPCWSTR)chr, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			if (hBitmap == NULL){
				MessageBox(hWnd, TEXT("FailedToOpenFileofOutput"), TEXT("はい..."), MB_OK);
				PostMessage(hWnd, WM_CLOSE, 0, 0);
				EndPaint(hWnd, &ps);
				break;
			}
			hMdc = CreateCompatibleDC(hdc);
			SelectObject(hMdc, hBitmap);
			if (!BitBlt(hdc, bm.bmWidth + 100, 5, bm.bmWidth, bm.bmHeight, hMdc, 0, 0, SRCCOPY))
				MessageBox(hWnd, TEXT("FailedTobitbltofoutput"), TEXT("はい..."), MB_OK);
		}
		MoveWindow(
			hButton,      // ウィンドウのハンドル
			bm.bmWidth + 25,          // 横方向の位置
			200,          // 縦方向の位置
			60,     // 幅
			40,    // 高さ
			false   // 再描画オプション
			);
		/*	DeleteDC(hMdc);
		DeleteObject(hBitmap);
		EndPaint(hWnd, &ps);*/
		break;
	case WM_COMMAND:
		GetClassName(hWnd, buf, 255);
		switch (LOWORD(wp)){
		case BUTTON_ID:
			if (yonda){
				wcstombs_s(0, filename, MAXLENGTH + MAX_PATH, ofn.lpstrFile, _TRUNCATE);
				yonda = false;
			}
			//ひらいた画像ファイルを表示
			keyfile.lStructSize = sizeof(OPENFILENAME);
			keyfile.hwndOwner = hWnd;
			keyfile.lpstrFilter = TEXT("key files {*.kp}\0*.kp\0")
				/*TEXT("他のファイルも同様の形式で")*/;
			keyfile.lpstrCustomFilter = strCustom;
			keyfile.nMaxCustFilter = 256;
			keyfile.nFilterIndex = 0;
			keyfile.lpstrFile = strFile;
			keyfile.nMaxFile = MAX_PATH;
			keyfile.Flags = OFN_FILEMUSTEXIST;
			GetOpenFileName(&keyfile);//ここで行うと画面更新して（まだflag=0なので）返還前画像読み込んじゃう
			wcstombs_s(0, keyfilename, MAXLENGTH + MAX_PATH, keyfile.lpstrFile, _TRUNCATE);
			if (!Decryption(filename, keyfilename)){
				MessageBox(hWnd, TEXT("4545"), TEXT("変換に失敗した..."), MB_OK);
				PostQuitMessage(0);
				break;
			}
			else{
				mbstowcs_s(0, chr, strlen(filename)+1, filename, _TRUNCATE);
				//wcscpy(chr, filename);
				wcscat(chr, _T("_recov.bmp"));
				flag = 0;
				++flag; //ここでスイッチを切れ変える。
				flag &= 1;
				InvalidateRect(
					hWnd,           // ウィンドウのハンドル
					NULL,  // 長方形の座標
					false          // 消去するかどうかの状態
					);
			}
			break;
		default:
			return (DefWindowProc(hWnd, msg, wp, lp));
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return (DefWindowProc(hWnd, msg, wp, lp));
	}
	return 0;
}