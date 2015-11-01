/*****************************************************************************/
/*                                                                           */
/*     bmp.c: bmp •’•°•§•ÅEËÕ˝§Œ•È•§•÷•È•ÅE                                  */
/*                                                                           */
/*             Kazutoshi Ando (Shizuoka Univ.)                               */
/*                                                                           */
/*                  Ver. 2004.11.30                                          */
/*                      WriteBmp: •ÿ•√•¿æ Û§Œ∑ÁÕ˚¿ÚΩ§¿µ.                    */
/*                  Ver. 2004.11.26                                          */
/*                      Diminish:  ∏∫ƒ¥(?)•ÅEº•¡•Û§Úƒ…≤√.                    */
/*                      PrintBmpInfo: bmp•’•°•§•ÅE ≥∞§Œ•’•°•§•ÅEÚ∆…§Û§¿§»§≠  */
/*                               §À, •®•È°º§ÚΩ–Œœ§π§ÅEË§¶§À —ππ.             */
/*                  Ver. 2004.08.20                                          */
/*                      ReadBmp: 24bitøß§Œbmp•’•°•§•ÅE ≥∞§Œ•’•°•§•ÅEÅE       */
/*                               ∆…§Û§¿§»§≠§À, •®•È°º§ÚΩ–Œœ§π§ÅEË§¶§À —ππ.   */
/*                      PrintBmpInfo: øÂ ø, ø‚ƒæ≤Ú¡ÅEŸ§ÚΩ–Œœ§π§ÅEË§¶§À —ππ.  */
/*                      WriteBmp: •ÿ•√•¿æ Û§Œ∑ÁÕ˚¿ÚΩ§¿µ.                    */
/*                  Ver. 2004.08.18                                          */
/*                        Gray §Úƒ…≤√.                                       */
/*                  Ver. 2004.08.17                                          */
/*                        4byte ∂≠≥¶§ÀπÁ§ÅEª§ÅEø§·§Œ∑◊ªªº∞§Ú≤˛Œ…,            */
/*                        Rotate90, Shrink, Mosaic §Úƒ…≤√.                   */
/*                        •®•È°º•·•√•ª°º•∏§ŒΩ–Œœ¿Ë§Ú…∏Ω‡•®•È°ºΩ–Œœ§À —ππ.    */
/*                  Ver. 2003.11.04                                          */
/*                  Ver. 2010.10.19                                          */
/*                        long int §ÅEint §«Ω–Œœ§∑§∆§§§ÅE»∑Ÿπ§¨§«§ÅEŒ§«Ω§¿µ        */
/*                                                                           */
/*****************************************************************************/
//#define _CRT_SECURE_NO_WARNINGS

#include"bmp.h"
#include"stdafx.h"

#pragma once


unsigned char Bmp_headbuf[HEADERSIZE];/* •ÿ•√•¿§Ú≥ «º§π§ÅEø§·§Œ —øÅE         */
unsigned char Bmp_Pallet[PALLETSIZE]; /* •´•È°º•—•ÅE√•»§Ú≥ «º                */

char Bmp_type[2];                     /* •’•°•§•ÅEø•§•◊ "BM"                 */
unsigned long Bmp_size;               /* bmp•’•°•§•ÅEŒ•µ•§•∫ (•–•§•»)        */
unsigned int Bmp_info_header_size; /* æ Û•ÿ•√•¿§Œ•µ•§•∫ = 40             */
unsigned int Bmp_header_size;      /* •ÿ•√•¿•µ•§•∫ = 54*/
long Bmp_height;                      /* π‚§µ (•‘•Ø•ª•ÅE                     */
long Bmp_width;                       /* …˝   (•‘•Ø•ª•ÅE                     */
unsigned short Bmp_planes;          /* •◊•ÅEº•ÛøÅEæÅEÀ 1                   */
unsigned short Bmp_color;          /* øß (•”•√•»)     24                  */
long Bmp_comp;                        /* ∞µΩÃ ˝À°         0                  */
long Bmp_image_size;                  /* ≤Ë¡ÅEÙ ¨§Œ•’•°•§•ÅEµ•§•∫ (•–•§•»)   */
long Bmp_xppm;                        /* øÂ ø≤Ú¡ÅEŸ (ppm)                    */
long Bmp_yppm;                        /* ø‚ƒæ≤Ú¡ÅEŸ (ppm)                    */

/*
   ¥ÿøÙÃæ: ReadBmp
   ∞˙øÅE : char *filename, img *imgp
    ÷§ÅEÕ: void
   ∆∞∫ÅE : bmp∑¡º∞§Œ•’•°•§•ÅEfilename §Ú≥´§§§∆, §Ω§Œ≤Ë¡ÅE«°º•ø§ÅE           2º°∏µ«€ŒÅEimgp->data §À≥ «º§π§ÅE ∆±ª˛§À, •ÿ•√•¿§´§È∆…§ﬂπ˛§ﬁ§ÅEø
   ≤Ë¡ÅEŒ…˝§»π‚§µ§Ú•∞•˙Ωº•–•ÅE—øÅEBmp_width §»Bmp_height §À•ª•√•»§π§ÅE
   */
void ReadBmp(char *filename, img *imgp) {
	int i, j;
	int Real_width;
	FILE *Bmp_Fp = fopen(filename, "rb");  /* •–•§• •ÅE‚°º•…∆…§ﬂπ˛§ﬂÕ—§À•™°º•◊•ÅE */
	unsigned char *Bmp_Data;           /* ≤Ë¡ÅE«°º•ø§ÅEπ‘ ¨≥ «º               */

	if (Bmp_Fp == NULL){
		MessageBox(NULL, TEXT("Error: file couldn\'t open for read!."), NULL, MB_OK);
		exit(1);
	}

	/* •ÿ•√•¿∆…§ﬂπ˛§ﬂ */
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

	Real_width = imgp->width * 3 + imgp->width % 4; /* 4byte ∂≠≥¶§À§¢§ÅEª§ÅEø§·§Àº¬∫›§Œ…˝§Œ∑◊ªª */

	/* «€ŒÛŒŒ∞Ë§Œ∆∞≈™≥Œ ›. º∫«‘§∑§øæÅEÁ§œ•®•È°º•·•√•ª°º•∏§ÚΩ–Œœ§∑§∆Ω™Œª */
	if ((Bmp_Data = (unsigned char *)calloc(Real_width, sizeof(unsigned char))) == NULL) {
		MessageBox(NULL, TEXT("Error: Memory allocation failed for Bmp_Data"), NULL, MB_OK);
		exit(1);
	}

	/* ≤Ë¡ÅE«°º•ø∆…§ﬂπ˛§ﬂ */
	for (i = 0; i < imgp->height; i++) {
		fread(Bmp_Data, 1, Real_width, Bmp_Fp);
		for (j = 0; j < imgp->width; j++) {
			imgp->data[imgp->height - i - 1][j].b = Bmp_Data[j * 3];
			imgp->data[imgp->height - i - 1][j].g = Bmp_Data[j * 3 + 1];
			imgp->data[imgp->height - i - 1][j].r = Bmp_Data[j * 3 + 2];
		}
	}

	/* ∆∞≈™§À≥Œ ›§∑§ø«€ŒÛŒŒ∞Ë§Œ≤Ú ÅE*/
	free(Bmp_Data);

	/* •’•°•§•ÅEØ•˙Ωº•∫ */
	fclose(Bmp_Fp);
}

/*
   ¥ÿøÙÃæ: WriteBmp
   ∞˙øÅE : char *filename, img *tp
    ÷§ÅEÕ: void
   ∆∞∫ÅE : 2º°∏µ«€ŒÅEtp->data §Œ∆‚Õ∆§Ú≤Ë¡ÅE«°º•ø§»§∑§∆, 24•”•√•»
   bmp∑¡º∞§Œ•’•°•§•ÅEfilename §ÀΩÒ§≠Ω–§π.
   */
void WriteBmp(char *filename, img *tp) {

	int i, j;
	int Real_width;
	FILE *Out_Fp = fopen(filename, "wb");  /* •’•°•§•ÅE™°º•◊•ÅE*/
	unsigned char *Bmp_Data;     /* ≤Ë¡ÅE«°º•ø§ÅEπ‘ ¨≥ «º               */

	if (Out_Fp == NULL){
		fprintf(stderr, "Error: file %s couldn\'t open for write!\n", filename);
		exit(1);
	}

	Bmp_color = 24;
	Bmp_header_size = HEADERSIZE;
	Bmp_info_header_size = 40;
	Bmp_planes = 1;

	Real_width = tp->width * 3 + tp->width % 4;  /* 4byte ∂≠≥¶§À§¢§ÅEª§ÅEø§·§Àº¬∫›§Œ…˝§Œ∑◊ªª */

	/* «€ŒÛŒŒ∞Ë§Œ∆∞≈™≥Œ ›. º∫«‘§∑§øæÅEÁ§œ•®•È°º•·•√•ª°º•∏§ÚΩ–Œœ§∑§∆Ω™Œª */
	if ((Bmp_Data = (unsigned char *)calloc(Real_width, sizeof(unsigned char))) == NULL) {
		MessageBox(NULL, TEXT("Error: Memory allocation failed for Bmp_Data"), NULL, MB_OK);
		exit(1);
	}
	/* •ÿ•√•¿æ Û§ŒΩ‡»ÅE*/
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
	/* •ÿ•√•¿æ ÛΩÒ§≠Ω–§∑ */
	fwrite(Bmp_headbuf, sizeof(unsigned char), HEADERSIZE, Out_Fp);
	/* ≤Ë¡ÅE«°º•øΩÒ§≠Ω–§∑ */
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
	/* ∆∞≈™§À≥Œ ›§∑§ø«€ŒÛŒŒ∞Ë§Œ≤Ú ÅE*/
	free(Bmp_Data);

	/* •’•°•§•ÅEØ•˙Ωº•∫ */
	fclose(Out_Fp);
}

/*
   ¥ÿøÙÃæ: PrintBmpInfo
   ∞˙øÅE : char *filename
    ÷§ÅEÕ: void
   ∆∞∫ÅE : ∞˙øÙ§»§∑§∆Õø§®§È§ÅEÅE’•°•§•ÅEæ§Úª˝§ƒ bmp ∑¡º∞§Œ≤Ë¡ÅE’•°•§•ÅE           §Œ¬∞¿≠§Ú≤ËÃÃ§ÀΩ–Œœ§π§ÅE
   */
void PrintBmpInfo(char *filename) {

	FILE *Bmp_Fp = fopen(filename, "rb");        /* •–•§• •ÅE‚°º•…∆…§ﬂπ˛§ﬂÕ—§À•™°º•◊•ÅE */
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


	printf("•’•°•§•ÅEæ       = %s \n", filename);
	printf("•’•°•§•ÅEø•§•◊   = %c%c \n", Bmp_type[0], Bmp_type[1]);
	printf("•’•°•§•ÅEµ•§•∫   = %ld (byte)\n", Bmp_size);
	printf("…˝               = %ld (pixel)\n", Bmp_width);
	printf("π‚§µ             = %ld (pixel)\n", Bmp_height);
	printf("øß               = %d (bit)\n", Bmp_color);
	printf("∞µΩÃ             = %ld\n", Bmp_comp);
	printf("≤Ë¡ÅEÙ ¨§Œ•µ•§•∫ = %ld (byte)\n", Bmp_image_size);
	printf("øÂ ø≤Ú¡ÅEŸ       = %ld (ppm)\n", Bmp_xppm);
	printf("ø‚ƒæ≤Ú¡ÅEŸ       = %ld (ppm)\n", Bmp_yppm);

	fclose(Bmp_Fp);
}

/*
   ¥ÿøÙÃæ: HMirror
   ∞˙øÅE : img *sp, img *tp
    ÷§ÅEÕ: void
   ∆∞∫ÅE : 2º°∏µ«€ŒÅEtp->data §Œ≤Ë¡ÅEÚøÂ ø ˝∏˛§Œ∂¿±«§Ú§»§√§ø§‚§Œ§ÅE           2º°∏µ«€ŒÅEsp->data §À≥ «º§π§ÅE
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
   ¥ÿøÙÃæ: VMirror
   ∞˙øÅE : img *sp, img *tp
    ÷§ÅEÕ: void
   ∆∞∫ÅE : 2º°∏µ«€ŒÅEtp->data §Œ≤Ë¡ÅEÚø‚ƒæ ˝∏˛§Œ∂¿±«§Ú§»§√§ø§‚§Œ§ÅE           2º°∏µ«€ŒÅEsp->data §À≥ «º§π§ÅE
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
   ¥ÿøÙÃæ: Rotate90
   ∞˙øÅE : int a, img *sp, img *tp
    ÷§ÅEÕ: void
   ∆∞∫ÅE : 2º°∏µ«€ŒÅEtp->data §Œ≤Ë¡ÅEÅE90°ﬂa ≈Ÿª˛∑◊≤Û§Í§À≤Û≈æ§∑§ø§‚§Œ§ÅE           2º°∏µ«€ŒÅEsp->data §À≥ «º§π§ÅE §µ§È§À, ≤Ë¡ÅEŒ…˝§»π‚§µ§ÅE
   sp->height §» sp->width §À¿ﬂƒÅE
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
   ¥ÿøÙÃæ: Shrink
   ∞˙øÅE : int a, img *sp, img *tp
    ÷§ÅEÕ: void
   ∆∞∫ÅE : 2º°∏µ«€ŒÅEtp->data §Œ≤Ë¡ÅEŒ…˝§»π‚§µ§ÅE1/a «‹§∑§ø≤Ë¡ÅEÅE           2º°∏µ«€ŒÅEsp->data §À≥ «º§π§ÅE
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
   ¥ÿøÙÃæ: Mosaic
   ∞˙øÅE : int a, img *sp, img *tp
    ÷§ÅEÕ: void
   ∆∞∫ÅE : 2º°∏µ«€ŒÅEtp->data §Œ≤Ë¡ÅEÀ•‚•∂•§•Ø§Ú§´§±§ø≤Ë¡ÅEÅE           2º°∏µ«€ŒÅEsp->data §À≥ «º§π§ÅE •‚•∂•§•Ø§Œ¬Á§≠§µ§œ
   a°ﬂa §«§¢§ÅE
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
   ¥ÿøÙÃæ: Gray
   ∞˙øÅE : img *sp, img *tp
    ÷§ÅEÕ: void
   ∆∞∫ÅE : 2º°∏µ«€ŒÅEtp->data §Œ≤Ë¡ÅEÚ•∞•ÅE§•π•±°º•ÅE—¥π§∑§∆,
   2º°∏µ«€ŒÅEsp->data §À≥ «º§π§ÅE
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
   ¥ÿøÙÃæ: Diminish
   ∞˙øÅE : img *sp, img *tp, unsigned char x
    ÷§ÅEÕ: void
   ∆∞∫ÅE : 2º°∏µ«€ŒÅEtp->data §Œ≤Ë¡ÅEÚ∏∫ƒ¥§∑ (≥∆RGB§Œµ±≈Ÿ§À§ƒ§§§∆
   2^x §»•ﬁ•π•Ø§ÚºË§√) §∆, 2º°∏µ«€ŒÅEsp->data §À≥ «º§π§ÅE
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
