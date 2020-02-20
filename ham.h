#pragma once

#include<stdint.h>

#pragma pack(1)
struct bmpfile_magic
{
	unsigned char magic[2]; // BM
};

struct bmpfile_header
{
	bmpfile_magic BM;
	uint32_t sizebmpdisk;
	uint16_t rieng1;
	uint16_t rieng2;
	uint32_t thutubyte;
};

struct bmpfile_DIB
{
	uint32_t size_DIB;
	uint32_t rong;
	uint32_t cao;
	uint16_t lopmau;
	uint16_t dosaumau;
	uint32_t phuongphapnen;
	uint32_t sizebmp;
	uint32_t phangiaingang;
	uint32_t phangiaidoc;
	uint32_t somau;
	uint32_t somauchinh;
};

struct Color
{
	unsigned char blue;
	unsigned char green;
	unsigned char red;
};

struct ColorTable
{
	Color	 *mau;
	uint32_t size;
};

struct PixelArray
{
	Color	 **pixels;
	uint32_t sodong;
	uint32_t socot;
};

struct BmpFile
{
	bmpfile_header	header;
	bmpfile_DIB		body;
	ColorTable	colorTable;
	PixelArray	pixelArray;
};

bool kiemTraBMP(FILE* f);

void docHeader(FILE* f, bmpfile_header &header);

void ghiHeader(FILE* f, bmpfile_header header);

void docDIB(FILE *f, bmpfile_DIB &body);

void ghiDIB(FILE *f, bmpfile_DIB body);

void docDongPixel(FILE* f, Color *&line, uint32_t length);

char demPadding(uint32_t ronganh, uint16_t pixelSize);

void boPadding(FILE* f, char count);

void themPadding(FILE *f, char dem);

void ghiDongPixel(FILE *f, Color *line, uint32_t length);

void docMangPixel(FILE *f, BmpFile &bmp);

void ghiMangPixel(FILE *f, BmpFile bmp);

void docBmp(char *filename, BmpFile &bmp);

void copyBmp(BmpFile nguon, BmpFile &dich, uint32_t x, uint32_t y, uint32_t rong, uint32_t cao);

void copyHeader(BmpFile nguon, BmpFile &dich, uint32_t rong, uint32_t cao);

void copyDIB(BmpFile nguon, BmpFile &dich, uint32_t rong, uint32_t cao);

void copyMangPixel(BmpFile nguon, BmpFile &dich, uint32_t x, uint32_t y, uint32_t rong, uint32_t cao);

void ghiBmp(char *filename, BmpFile bmp);

void drawBmp(BmpFile bmp);

void huyMang(PixelArray data);

void huyMangPixel(BmpFile &bmp);

void xuLiTenBmpDich(const char *tendich, const char *duoidich, int i, int j, char *filedich);

void cutFileBmp(char *filenguon, char *tendich, int sophancao, int sophanrong);

void doc(char **argv);

