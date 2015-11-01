/*****************************************************************************/
/*                                                                           */
/*     bmp.h: bmp �ե�����E����Υ饤�֥��Τ���Υإå��ե�����E            */
/*                                                                           */
/*             Kazutoshi Ando (Shizuoka Univ.)                               */
/*                                                                           */
/*                  Ver. 2004.08.18                                          */
/*                  Ver. 2004.08.17                                          */
/*                  Ver. 2003.11.04                                          */
/*                                                                           */
/*****************************************************************************/
#pragma once
#ifndef INCLUDED
#define INCLUDED
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define HEADERSIZE   54               /* �إå��Υ����� 54 = 14 + 40         */
#define PALLETSIZE 1024               /* �ѥ�EåȤΥ�����                    */
#define MAXWIDTH   3000               /* ��(pixel)�ξ��                     */
#define MAXHEIGHT  3000               /* �⤵(pixel) �ξ��                  */
#define MAXLENGTH	80

/* x �� y �θ򴹤Τ���� �ޥ����ؿ�E*/
#define SWAP(x,y) {typeof(x) temp; temp=x; x=y; y=temp;}



typedef struct {                      /* 1�ԥ�����E�����������ĤγƵ���     */
  unsigned char r;
  unsigned char g;
  unsigned char b;
} color;

typedef struct {
  long height;
  long width;
  color data[MAXHEIGHT][MAXWIDTH];
} img;

void ReadBmp(char *filename, img *imgp);
void WriteBmp(char *filename, img *tp);
void PrintBmpInfo(char *filename);
void HMirror(img *sp, img *tp);
void VMirror(img *sp, img *tp);
void Rotate90(int a, img *sp, img *tp);
void Shrink(int a, img *sp, img *tp);
void Mosaic(int a, img *sp, img *tp);
void Gray(img *sp, img *tp);
void Diminish(img *sp, img *tp, unsigned char x);
#endif
