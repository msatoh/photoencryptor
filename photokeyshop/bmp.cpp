/*****************************************************************************/
/*                                                                           */
/*     bmp.c: bmp �ե�����E����Υ饤�֥饁E                                  */
/*                                                                           */
/*             Kazutoshi Ando (Shizuoka Univ.)                               */
/*                                                                           */
/*                  Ver. 2004.11.30                                          */
/*                      WriteBmp: �إå�����η�������.                    */
/*                  Ver. 2004.11.26                                          */
/*                      Diminish:  ��Ĵ(?)��E�������ɲ�.                    */
/*                      PrintBmpInfo: bmp�ե�����Eʳ��Υե�����E��ɤ���Ȥ�  */
/*                               ��, ���顼����Ϥ���E褦���ѹ�.             */
/*                  Ver. 2004.08.20                                          */
/*                      ReadBmp: 24bit����bmp�ե�����Eʳ��Υե�����E�E       */
/*                               �ɤ���Ȥ���, ���顼����Ϥ���E褦���ѹ�.   */
/*                      PrintBmpInfo: ��ʿ, ��ľ����E٤���Ϥ���E褦���ѹ�.  */
/*                      WriteBmp: �إå�����η�������.                    */
/*                  Ver. 2004.08.18                                          */
/*                        Gray ���ɲ�.                                       */
/*                  Ver. 2004.08.17                                          */
/*                        4byte �����˹礁E���E���η׻��������,            */
/*                        Rotate90, Shrink, Mosaic ���ɲ�.                   */
/*                        ���顼��å������ν������ɸ�२�顼���Ϥ��ѹ�.    */
/*                  Ver. 2003.11.04                                          */
/*                  Ver. 2010.10.19                                          */
/*                        long int ��Eint �ǽ��Ϥ��Ƥ���Eȷٹ𤬤Ǥ�EΤǽ���        */
/*                                                                           */
/*****************************************************************************/
//#define _CRT_SECURE_NO_WARNINGS

#include"bmp.h"
#include"stdafx.h"

#pragma once


unsigned char Bmp_headbuf[HEADERSIZE];/* �إå����Ǽ����E�����ѿ�E         */
unsigned char Bmp_Pallet[PALLETSIZE]; /* ���顼�ѥ�EåȤ��Ǽ                */

char Bmp_type[2];                     /* �ե�����E����� "BM"                 */
unsigned long Bmp_size;               /* bmp�ե�����EΥ����� (�Х���)        */
unsigned int Bmp_info_header_size; /* ����إå��Υ����� = 40             */
unsigned int Bmp_header_size;      /* �إå������� = 54*/
long Bmp_height;                      /* �⤵ (�ԥ�����E                     */
long Bmp_width;                       /* ��   (�ԥ�����E                     */
unsigned short Bmp_planes;          /* �ץ�E���E��E� 1                   */
unsigned short Bmp_color;          /* �� (�ӥå�)     24                  */
long Bmp_comp;                        /* ������ˡ         0                  */
long Bmp_image_size;                  /* ����E�ʬ�Υե�����E����� (�Х���)   */
long Bmp_xppm;                        /* ��ʿ����E� (ppm)                    */
long Bmp_yppm;                        /* ��ľ����E� (ppm)                    */

/*
   �ؿ�̾: ReadBmp
   ����E : char *filename, img *imgp
   �֤�E�: void
   ư��E : bmp�����Υե�����Efilename �򳫤���, ���β���Eǡ�����E           2������΁Eimgp->data �˳�Ǽ����E Ʊ����, �إå������ɤ߹��ޤ�E�
   ����E����ȹ⤵�򥰥����Х�Eѿ�EBmp_width ��Bmp_height �˥��åȤ���E
   */
void ReadBmp(char *filename, img *imgp) {
	int i, j;
	int Real_width;
	FILE *Bmp_Fp = fopen(filename, "rb");  /* �Х��ʥ�E⡼���ɤ߹����Ѥ˥����ץ�E */
	unsigned char *Bmp_Data;           /* ����Eǡ�����E��ʬ��Ǽ               */

	if (Bmp_Fp == NULL){
		MessageBox(NULL, TEXT("Error: file couldn\'t open for read!."), NULL, MB_OK);
		exit(1);
	}

	/* �إå��ɤ߹��� */
	fread(Bmp_headbuf, sizeof(unsigned char), HEADERSIZE, Bmp_Fp);

	CopyMemory(&Bmp_type, Bmp_headbuf, sizeof(Bmp_type));
	if (strncmp(Bmp_type, "BM", 2) != 0) {
		MessageBox(NULL, TEXT("Error: this is not a bmp file."), NULL, MB_OK);
		exit(1);
	}

	CopyMemory(&imgp->width, Bmp_headbuf + 18, sizeof(Bmp_width));
	CopyMemory(&imgp->height, Bmp_headbuf + 22, sizeof(Bmp_height));
	CopyMemory(&Bmp_color, Bmp_headbuf + 28, sizeof(Bmp_color));
	if (Bmp_color != 24) {
		MessageBox(NULL, TEXT("Error: Bmp_color = this is not implemented in this program."), NULL, MB_OK);
		exit(1);
	}

	if (imgp->width > MAXWIDTH) {
		MessageBox(NULL, TEXT("Error: Bmp_width > MAXWIDTH"), NULL, MB_OK);
		exit(1);
	}

	if (imgp->height > MAXHEIGHT) {
		MessageBox(NULL, TEXT("Error: Bmp_height > MAXHEIGHT"), NULL, MB_OK);
		exit(1);
	}

	Real_width = imgp->width * 3 + imgp->width % 4; /* 4byte �����ˤ���E���E���˼ºݤ����η׻� */

	/* �����ΰ��ưŪ����. ���Ԥ�����E�ϥ��顼��å���������Ϥ��ƽ�λ */
	if ((Bmp_Data = (unsigned char *)calloc(Real_width, sizeof(unsigned char))) == NULL) {
		MessageBox(NULL, TEXT("Error: Memory allocation failed for Bmp_Data"), NULL, MB_OK);
		exit(1);
	}

	/* ����Eǡ����ɤ߹��� */
	for (i = 0; i < imgp->height; i++) {
		fread(Bmp_Data, 1, Real_width, Bmp_Fp);
		for (j = 0; j < imgp->width; j++) {
			imgp->data[imgp->height - i - 1][j].b = Bmp_Data[j * 3];
			imgp->data[imgp->height - i - 1][j].g = Bmp_Data[j * 3 + 1];
			imgp->data[imgp->height - i - 1][j].r = Bmp_Data[j * 3 + 2];
		}
	}

	/* ưŪ�˳��ݤ��������ΰ�β�ʁE*/
	free(Bmp_Data);

	/* �ե�����E������� */
	fclose(Bmp_Fp);
}

/*
   �ؿ�̾: WriteBmp
   ����E : char *filename, img *tp
   �֤�E�: void
   ư��E : 2������΁Etp->data �����Ƥ����Eǡ����Ȥ���, 24�ӥå�
   bmp�����Υե�����Efilename �˽񤭽Ф�.
   */
void WriteBmp(char *filename, img *tp) {

	int i, j;
	int Real_width;
	FILE *Out_Fp = fopen(filename, "wb");  /* �ե�����E����ץ�E*/
	unsigned char *Bmp_Data;     /* ����Eǡ�����E��ʬ��Ǽ               */

	if (Out_Fp == NULL){
		fprintf(stderr, "Error: file %s couldn\'t open for write!\n", filename);
		exit(1);
	}

	Bmp_color = 24;
	Bmp_header_size = HEADERSIZE;
	Bmp_info_header_size = 40;
	Bmp_planes = 1;

	Real_width = tp->width * 3 + tp->width % 4;  /* 4byte �����ˤ���E���E���˼ºݤ����η׻� */

	/* �����ΰ��ưŪ����. ���Ԥ�����E�ϥ��顼��å���������Ϥ��ƽ�λ */
	if ((Bmp_Data = (unsigned char *)calloc(Real_width, sizeof(unsigned char))) == NULL) {
		MessageBox(NULL, TEXT("Error: Memory allocation failed for Bmp_Data"), NULL, MB_OK);
		exit(1);
	}
	/* �إå�����ν�ȁE*/
	Bmp_xppm = Bmp_yppm = 0;
	Bmp_image_size = tp->height*Real_width;
	Bmp_size = Bmp_image_size + HEADERSIZE;
	Bmp_headbuf[0] = 'B'; Bmp_headbuf[1] = 'M';
	CopyMemory(Bmp_headbuf + 2, &Bmp_size, sizeof(Bmp_size));
	Bmp_headbuf[6] = Bmp_headbuf[7] = Bmp_headbuf[8] = Bmp_headbuf[9] = 0;
	CopyMemory(Bmp_headbuf + 10, &Bmp_header_size, sizeof(Bmp_header_size));
	Bmp_headbuf[11] = Bmp_headbuf[12] = Bmp_headbuf[13] = 0;
	CopyMemory(Bmp_headbuf + 14, &Bmp_info_header_size, sizeof(Bmp_info_header_size));
	Bmp_headbuf[15] = Bmp_headbuf[16] = Bmp_headbuf[17] = 0;
	CopyMemory(Bmp_headbuf + 18, &tp->width, sizeof(Bmp_width));
	CopyMemory(Bmp_headbuf + 22, &tp->height, sizeof(Bmp_height));
	CopyMemory(Bmp_headbuf + 26, &Bmp_planes, sizeof(Bmp_planes));
	CopyMemory(Bmp_headbuf + 28, &Bmp_color, sizeof(Bmp_color));
	CopyMemory(Bmp_headbuf + 34, &Bmp_image_size, sizeof(Bmp_image_size));
	CopyMemory(Bmp_headbuf + 38, &Bmp_xppm, sizeof(Bmp_xppm));
	CopyMemory(Bmp_headbuf + 42, &Bmp_yppm, sizeof(Bmp_yppm));
	Bmp_headbuf[46] = Bmp_headbuf[47] = Bmp_headbuf[48] = Bmp_headbuf[49] = 0;
	Bmp_headbuf[50] = Bmp_headbuf[51] = Bmp_headbuf[52] = Bmp_headbuf[53] = 0;
	/* �إå�����񤭽Ф� */
	fwrite(Bmp_headbuf, sizeof(unsigned char), HEADERSIZE, Out_Fp);
	/* ����Eǡ����񤭽Ф� */
	for (i = 0; i < tp->height; i++) {
		for (j = 0; j < tp->width; j++) {
			Bmp_Data[j * 3] = tp->data[tp->height - i - 1][j].b;
			Bmp_Data[j * 3 + 1] = tp->data[tp->height - i - 1][j].g;
			Bmp_Data[j * 3 + 2] = tp->data[tp->height - i - 1][j].r;
		}
		for (j = tp->width * 3; j < Real_width; j++) {
			Bmp_Data[j] = 0;
		}
		fwrite(Bmp_Data, sizeof(unsigned char), Real_width, Out_Fp);
	}
	/* ưŪ�˳��ݤ��������ΰ�β�ʁE*/
	free(Bmp_Data);

	/* �ե�����E������� */
	fclose(Out_Fp);
}

/*
   �ؿ�̾: PrintBmpInfo
   ����E : char *filename
   �֤�E�: void
   ư��E : �����Ȥ���Ϳ���餁E�Eե�����E������ bmp �����β���Eե�����E           ��°������̤˽��Ϥ���E
   */
void PrintBmpInfo(char *filename) {

	FILE *Bmp_Fp = fopen(filename, "rb");        /* �Х��ʥ�E⡼���ɤ߹����Ѥ˥����ץ�E */
	if (Bmp_Fp == NULL){
		fprintf(stderr, "Error: file %s couldn\'t open for write!\n", filename);
		exit(1);
	}

	fread(Bmp_headbuf, sizeof(unsigned char), HEADERSIZE, Bmp_Fp);

	CopyMemory(&Bmp_type, Bmp_headbuf, sizeof(Bmp_type));
	if (strncmp(Bmp_type, "BM", 2) != 0) {
		fprintf(stderr, "Error: %s is not a bmp file.\n", filename);
		exit(1);
	}
	CopyMemory(&Bmp_size, Bmp_headbuf + 2, sizeof(Bmp_size));
	CopyMemory(&Bmp_width, Bmp_headbuf + 18, sizeof(Bmp_width));
	CopyMemory(&Bmp_height, Bmp_headbuf + 22, sizeof(Bmp_height));
	CopyMemory(&Bmp_color, Bmp_headbuf + 28, sizeof(Bmp_color));
	CopyMemory(&Bmp_comp, Bmp_headbuf + 30, sizeof(Bmp_comp));
	CopyMemory(&Bmp_image_size, Bmp_headbuf + 34, sizeof(Bmp_size));
	CopyMemory(&Bmp_xppm, Bmp_headbuf + 38, sizeof(Bmp_xppm));
	CopyMemory(&Bmp_yppm, Bmp_headbuf + 42, sizeof(Bmp_yppm));


	printf("�ե�����E�       = %s \n", filename);
	printf("�ե�����E�����   = %c%c \n", Bmp_type[0], Bmp_type[1]);
	printf("�ե�����E�����   = %ld (byte)\n", Bmp_size);
	printf("��               = %ld (pixel)\n", Bmp_width);
	printf("�⤵             = %ld (pixel)\n", Bmp_height);
	printf("��               = %d (bit)\n", Bmp_color);
	printf("����             = %ld\n", Bmp_comp);
	printf("����E�ʬ�Υ����� = %ld (byte)\n", Bmp_image_size);
	printf("��ʿ����E�       = %ld (ppm)\n", Bmp_xppm);
	printf("��ľ����E�       = %ld (ppm)\n", Bmp_yppm);

	fclose(Bmp_Fp);
}

/*
   �ؿ�̾: HMirror
   ����E : img *sp, img *tp
   �֤�E�: void
   ư��E : 2������΁Etp->data �β���E��ʿ�����ζ��Ǥ�Ȥä���Τ�E           2������΁Esp->data �˳�Ǽ����E
   */
void HMirror(img* sp, img *tp) {
	int i, j;
	long k;
	for (i = 0; i < tp->height; i++)
		for (j = 0; j < tp->width; j++)
			sp->data[tp->height - i - 1][j] = tp->data[i][j];
	sp->height = tp->height;
	sp->width = tp->width;
}

/*
   �ؿ�̾: VMirror
   ����E : img *sp, img *tp
   �֤�E�: void
   ư��E : 2������΁Etp->data �β���E��ľ�����ζ��Ǥ�Ȥä���Τ�E           2������΁Esp->data �˳�Ǽ����E
   */
void VMirror(img *sp, img *tp) {
	int i, j;
	long k;
	for (i = 0; i < tp->height; i++)
		for (j = 0; j < tp->width; j++)
			sp->data[i][tp->width - j - 1] = tp->data[i][j];
	sp->height = tp->height;
	sp->width = tp->width;
}


/*
   �ؿ�̾: Rotate90
   ����E : int a, img *sp, img *tp
   �֤�E�: void
   ư��E : 2������΁Etp->data �β���E�E90��a �ٻ��ײ��˲�ž������Τ�E           2������΁Esp->data �˳�Ǽ����E �����, ����E����ȹ⤵��E
   sp->height �� sp->width ����āE
   */
void Rotate90(int a, img *sp, img *tp) {
	int i, j;
	if ((a % 4) == 0) {
		for (i = 0; i < tp->height; i++)
			for (j = 0; j < tp->width; j++)
				sp->data[i][j] = tp->data[i][j];
		sp->width = tp->width;
		sp->height = tp->height;
	}
	else if ((a % 4) == 1) {
		for (i = 0; i < tp->height; i++)
			for (j = 0; j < tp->width; j++)
				sp->data[j][tp->height - i - 1] = tp->data[i][j];
		sp->height = tp->width;
		sp->width = tp->height;
	}
	else if ((a % 4) == 2) {
		for (i = 0; i < tp->height; i++)
			for (j = 0; j < tp->width; j++)
				sp->data[tp->height - i - 1][tp->width - j - 1] = tp->data[i][j];
		sp->width = tp->width;
		sp->height = tp->height;
	}
	else {
		for (i = 0; i < tp->height; i++)
			for (j = 0; j < tp->width; j++)
				sp->data[tp->width - j - 1][i] = tp->data[i][j];
		sp->height = tp->width;
		sp->width = tp->height;
	}
}


/*
   �ؿ�̾: Shrink
   ����E : int a, img *sp, img *tp
   �֤�E�: void
   ư��E : 2������΁Etp->data �β���E����ȹ⤵��E1/a �ܤ�������E�E           2������΁Esp->data �˳�Ǽ����E
   */
void Shrink(int a, img *sp, img *tp) {
	int i, j, k, l, w, h, count;
	unsigned long tmp_r, tmp_g, tmp_b;

	sp->height = tp->height / a + ((tp->height%a == 0) ? 0 : 1);
	sp->width = tp->width / a + ((tp->width%a == 0) ? 0 : 1);

	for (i = 0; i < sp->height; i++) {
		h = (a*(i + 1) <= tp->height) ? a : tp->height - a*i;
		for (j = 0; j < sp->width; j++) {
			w = (a*(j + 1) <= tp->width) ? a : tp->width - a*j;
			tmp_r = 0; tmp_g = 0; tmp_b = 0; count = 0;
			for (k = 0; k < h; k++)
				for (l = 0; l < w; l++) {
					tmp_r += tp->data[i*a + k][j*a + l].r;
					tmp_g += tp->data[i*a + k][j*a + l].g;
					tmp_b += tp->data[i*a + k][j*a + l].b;
					count++;
				}
			sp->data[i][j].r = (unsigned char)(tmp_r / count);
			sp->data[i][j].g = (unsigned char)(tmp_g / count);
			sp->data[i][j].b = (unsigned char)(tmp_b / count);
		}
	}
}

/*
   �ؿ�̾: Mosaic
   ����E : int a, img *sp, img *tp
   �֤�E�: void
   ư��E : 2������΁Etp->data �β���E˥⥶�����򤫤�������E�E           2������΁Esp->data �˳�Ǽ����E �⥶�������礭����
   a��a �Ǥ���E
   */
void Mosaic(int a, img *sp, img *tp) {
	int i, j, k, l, w, h, t_height, t_width, count;
	unsigned long tmp_r, tmp_g, tmp_b;

	sp->height = tp->height;
	sp->width = tp->width;

	t_height = tp->height / a + ((tp->height%a == 0) ? 0 : 1);
	t_width = tp->width / a + ((tp->width%a == 0) ? 0 : 1);

	for (i = 0; i < t_height; i++) {
		h = (a*(i + 1) <= tp->height) ? a : tp->height - a*i;
		for (j = 0; j < t_width; j++) {
			w = (a*(j + 1) <= tp->width) ? a : tp->width - a*j;
			tmp_r = 0; tmp_g = 0; tmp_b = 0; count = 0;
			for (k = 0; k < h; k++)
				for (l = 0; l < w; l++) {
					tmp_r += tp->data[i*a + k][j*a + l].r;
					tmp_g += tp->data[i*a + k][j*a + l].g;
					tmp_b += tp->data[i*a + k][j*a + l].b;
					count++;
				}
			tmp_r = (unsigned char)(tmp_r / count);
			tmp_g = (unsigned char)(tmp_g / count);
			tmp_b = (unsigned char)(tmp_b / count);
			for (k = 0; k < h; k++)
				for (l = 0; l < w; l++) {
					sp->data[i*a + k][j*a + l].r = tmp_r;
					sp->data[i*a + k][j*a + l].g = tmp_g;
					sp->data[i*a + k][j*a + l].b = tmp_b;
				}
		}
	}

}

/*
   �ؿ�̾: Gray
   ����E : img *sp, img *tp
   �֤�E�: void
   ư��E : 2������΁Etp->data �β���E򥰥�E���������EѴ�����,
   2������΁Esp->data �˳�Ǽ����E
   */
void Gray(img *sp, img *tp) {
	int i, j;
	unsigned char tmp;

	sp->height = tp->height;
	sp->width = tp->width;

	for (i = 0; i < sp->height; i++) {
		for (j = 0; j < sp->width; j++) {
			tmp = (unsigned char)((tp->data[i][j].r + tp->data[i][j].g + tp->data[i][j].b) / 3);
			sp->data[i][j].r = sp->data[i][j].g = sp->data[i][j].b = tmp;
		}
	}

}

/*
   �ؿ�̾: Diminish
   ����E : img *sp, img *tp, unsigned char x
   �֤�E�: void
   ư��E : 2������΁Etp->data �β���E�Ĵ�� (��RGB�ε��٤ˤĤ���
   2^x �ȥޥ�������) ��, 2������΁Esp->data �˳�Ǽ����E
   */
void Diminish(img *sp, img *tp, unsigned char x) {
	int i, j;
	unsigned char y;

	x = x % 8;
	y = 255 << x;

	sp->height = tp->height;
	sp->width = tp->width;

	for (i = 0; i < sp->height; i++) {
		for (j = 0; j < sp->width; j++) {
			sp->data[i][j].r = tp->data[i][j].r & y;
			sp->data[i][j].g = tp->data[i][j].g & y;
			sp->data[i][j].b = tp->data[i][j].b & y;
		}
	}
}
