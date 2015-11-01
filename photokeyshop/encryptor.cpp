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

bool Encryption(char *file_in_name){
	//初期化
	char *file_out_name;//画像の出力・入力ファイル名
	char *key_out_file;//鍵ファイル
	char *buf;
	img *imagein, *imageout;
	int h, w;
	int i = 0;
	int registered_num = 0;//色の登録数
	angou *code;//色を変換するテーブル
	FILE* fp;

	srand((unsigned)time(NULL));
	file_out_name = (char *)malloc(sizeof(char)*(strlen(file_in_name) + 10));
	strcpy(file_out_name, file_in_name);
	strcat(file_out_name, "_conv.bmp");
	key_out_file = (char *)malloc(sizeof(char)*(strlen(file_in_name) + 8));
	buf = (char *)malloc(sizeof(char)*(strlen(file_in_name) + 1));
	strcpy(buf, file_in_name);
	strcpy(key_out_file, buf);
	strcat(key_out_file, "_key.kp");
	imagein = (img *)malloc(sizeof(img));
	imageout = (img *)malloc(sizeof(img));
	ReadBmp(file_in_name, imagein);
	code = (angou *)malloc(sizeof(angou)*imagein->height*imagein->width);
	imageout = imagein;
	if ((fp = fopen(key_out_file, "w")) == NULL){
		MessageBox(NULL, TEXT("key file output error."), NULL, MB_OK);
		return false;
	}

	//画像処理  
	for (h = 0; h < imagein->height; h++){
		for (w = 0; w < imagein->width; w++){
			for (i = 0; i <= registered_num; i++){
				if (memcmp(&code[i].original_color,
					&imagein->data[h][w],
					sizeof(code[i].original_color)) == 0){
					//登録してある色が見つかったら代入
					imageout->data[h][w] = code[i].converted_color;
					goto searched_out;
				}
			}

			//変換する色の登録
			//変換前の色を登録
			code[registered_num].original_color = imagein->data[h][w];
		try_again:
			//変換後の色を登録
			code[registered_num].converted_color.r = rand() % 256;
			code[registered_num].converted_color.g = rand() % 256;
			code[registered_num].converted_color.b = rand() % 256;
			///*変換後の色が既に登録してある色だったら
			// 変換後の色の登録をやり直す*/
			for (i = 0; i < registered_num; i++){
				if (memcmp(&code[registered_num].converted_color,
					&code[i].converted_color,
					sizeof(code[registered_num].converted_color)) == 0){
					goto try_again;
				}
			}
			//出力画像に変換後の色を代入
			imageout->data[h][w] = code[registered_num].converted_color;
			registered_num++;
		searched_out:
			;
		}
	}
	fwrite(code, sizeof(angou), registered_num, fp);//鍵ファイルに変換情報を記録
	WriteBmp(file_out_name, imageout);

	//後処理  
	fclose(fp);
	//free(file_out_name);
	/*free(key_out_file);*/ free(buf);
	free(code);
	return true;
}

LRESULT CALLBACK EncProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp){
	PAINTSTRUCT ps;
	static HWND hButton;
	HDC hdc, hMdc;
	TCHAR buf[256];
	static int flag = 0;
	static WCHAR chr[MAXLENGTH + MAX_PATH];
	char filename[MAXLENGTH + MAX_PATH], outfilename[MAXLENGTH + MAX_PATH] = { '\0' };
	static BITMAP bm;
	static OPENFILENAME ofn = { 0 };
	static TCHAR strFile[MAX_PATH], strCustom[256] = TEXT("Before files\0*.*\0\0");
	static HBITMAP hBitmap;

	switch (msg){
	case WM_CREATE:
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
		if (flag==0){
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
			setlocale(LC_ALL, "japanese");
			//ひらいた画像ファイルを表示
			wcstombs_s(0, filename, MAXLENGTH + MAX_PATH, ofn.lpstrFile, _TRUNCATE);
			if (!Encryption(filename)){
				MessageBox(hWnd, TEXT("4545"), TEXT("変換に失敗した..."), MB_OK);
				return -1;
			}
			else{
				wcscpy(chr,ofn.lpstrFile);
				wcscat(chr,_T("_conv.bmp"));
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