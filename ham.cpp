#include<stdio.h>
#include<Windows.h>
#include"ham.h"

//Kiem tra co phai file BMP khong?
bool kiemTraBMP(FILE* f)
{
	if (f == NULL)
		return false;

	bmpfile_magic magicbmp;
	fseek(f, 0L, SEEK_SET);
	fread(&magicbmp, sizeof(bmpfile_magic), 1, f);

	return magicbmp.magic[0] == 'B' && magicbmp.magic[1] == 'M';
}

void docHeader(FILE* f, bmpfile_header &header)
{
	if (f == NULL)
		return;

	fseek(f, 0L, SEEK_SET);
	fread(&header, sizeof(bmpfile_header), 1, f);
}

void ghiHeader(FILE* f, bmpfile_header header)
{
	if (f == NULL)
		return;

	fseek(f, 0L, SEEK_SET);
	fwrite(&header, sizeof(bmpfile_header), 1, f);
}

void docDIB(FILE *f, bmpfile_DIB &body)
{
	if (f == NULL)
		return;

	fseek(f, sizeof(bmpfile_header), SEEK_SET);
	fread(&body, sizeof(bmpfile_DIB), 1, f);
}

void ghiDIB(FILE *f, bmpfile_DIB body)
{
	if (f == NULL)
		return;

	fseek(f, sizeof(bmpfile_header), SEEK_SET);
	fwrite(&body, sizeof(bmpfile_DIB), 1, f);
}

//Doc tung dong pixel cua file BMP.
void docDongPixel(FILE* f, Color *&line, uint32_t length)
{
	if (f == NULL)
		return;

	line = new Color[length];
	fread(line, sizeof(Color), length, f);
}

char demPadding(uint32_t ronganh, uint16_t pixelSize)
{
	return (4 - (ronganh * (pixelSize / 8) % 4)) % 4;
}

//Bo di padding cuoi moi dong pixel.
void boPadding(FILE* f, char count)
{
	if (f == NULL)
		return;

	if (count == 0)
		return;

	char padding[3];
	fread(padding, count, 1, f);
}

void themPadding(FILE *f, char dem)
{
	if (f == NULL)
		return;

	if (dem == 0)
		return;

	char padding[3];
	fwrite(padding, dem, 1, f);
}

void ghiDongPixel(FILE *f, Color *line, uint32_t length)
{
	if (f == NULL)
		return;

	fwrite(line, sizeof(Color), length, f);
}

void docMangPixel(FILE *f, BmpFile &bmp)
{
	if (f == NULL)
		return;

	bmp.pixelArray.sodong = bmp.body.cao;
	bmp.pixelArray.socot = bmp.body.rong;
	bmp.pixelArray.pixels = new Color *[bmp.pixelArray.socot];

	char sopadding = demPadding(bmp.body.rong, bmp.body.dosaumau);

	fseek(f, bmp.header.thutubyte, SEEK_SET);

	for (int i = 0; i < bmp.pixelArray.sodong; i++)
	{
		docDongPixel(f, bmp.pixelArray.pixels[bmp.pixelArray.sodong - i - 1], bmp.body.rong);
		boPadding(f, sopadding);
	}
}

void ghiMangPixel(FILE *f, BmpFile bmp)
{
	if (f == NULL)
		return;

	char soppadding = demPadding(bmp.body.rong, bmp.body.dosaumau);

	fseek(f, bmp.header.thutubyte, SEEK_SET);

	for (int i = 0; i < bmp.pixelArray.sodong; i++)
	{
		ghiDongPixel(f, bmp.pixelArray.pixels[bmp.pixelArray.sodong - i - 1], bmp.body.rong);
		themPadding(f, soppadding);
	}
}

void docBmp(char *filename, BmpFile &bmp)
{
	FILE *f = fopen(filename, "rb");

	if (f == NULL || !kiemTraBMP(f))
	{
		printf("No found Bmp.\n");
		return;
	}

	docHeader(f, bmp.header);
	docDIB(f, bmp.body);
	docMangPixel(f, bmp);

	fclose(f);
}

void copyBmp(BmpFile nguon, BmpFile &dich, uint32_t x, uint32_t y, uint32_t rong, uint32_t cao)
{
	copyHeader(nguon, dich, rong, cao);
	copyDIB(nguon, dich, rong, cao);
	copyMangPixel(nguon, dich, x, y, rong, cao);
}

void copyHeader(BmpFile nguon, BmpFile &dich, uint32_t rong, uint32_t cao)
{
	dich.header.BM.magic[0] = 'B';
	dich.header.BM.magic[1] = 'M';
	dich.header.sizebmpdisk = (rong * nguon.body.dosaumau / 8 + demPadding(rong, nguon.body.dosaumau)) * cao + nguon.header.thutubyte;
	dich.header.thutubyte = nguon.header.thutubyte;
}

void copyDIB(BmpFile nguon, BmpFile &dich, uint32_t rong, uint32_t cao)
{
	dich.body = nguon.body;
	dich.body.rong = rong;
	dich.body.cao = cao;
	dich.body.sizebmp = (rong * nguon.body.dosaumau / 8 + demPadding(rong, nguon.body.dosaumau)) * cao;
}

void copyMangPixel(BmpFile nguon, BmpFile &dich, uint32_t x, uint32_t y, uint32_t rong, uint32_t cao)
{
	PixelArray *sArray = &nguon.pixelArray;
	PixelArray *dArray = &dich.pixelArray;

	dArray->sodong = cao;
	dArray->socot = rong;
	dArray->pixels = new Color *[dArray->sodong];

	for (int i = 0; i < dArray->sodong; i++)
	{
		dArray->pixels[i] = new Color[dArray->socot];
		for (int j = 0; j < dArray->socot; j++)
			dArray->pixels[i][j] = sArray->pixels[i + y][j + x];
	}
}

void ghiBmp(char *filename, BmpFile bmp)
{
	FILE *f = fopen(filename, "wb");

	if (f == NULL)
	{
		printf("Error.\n");
		return;
	}

	ghiHeader(f, bmp.header);
	ghiDIB(f, bmp.body);
	ghiMangPixel(f, bmp);

	fclose(f);
}

void drawBmp(BmpFile bmp)
{
	HWND console = GetConsoleWindow();
	HDC hdc = GetDC(console);

	for (int i = 0; i < bmp.body.cao; i++)
		for (int j = 0; j < bmp.body.rong; j++)
		{
			Color pixel = bmp.pixelArray.pixels[i][j];
			SetPixel(hdc, j, i, RGB(pixel.red, pixel.green, pixel.blue));
		}

	ReleaseDC(console, hdc);
}

void huyMang(PixelArray data)
{
	for (int i = 0; i < data.sodong; i++)
		delete[]data.pixels[i];

	delete[]data.pixels;
}

void huyMangPixel(BmpFile &bmp)
{
	for (int i = 0; i < bmp.pixelArray.sodong; i++)
		delete[]bmp.pixelArray.pixels[i];

	delete[]bmp.pixelArray.pixels;
}

void xuLiTenBmpDich(const char *tendich, const char *duoidich, int i, int j, char *filedich)
{
	sprintf(filedich, "%s%dx%d%s", tendich, i + 1, j + 1, duoidich);
}

void cutFileBmp(char *filenguon, char *tendich, int sophancao, int sophanrong)
{
	const char *BMP_EXT = ".bmp";

	BmpFile bmpnguon;
	docBmp(filenguon, bmpnguon);

	int chieurong = bmpnguon.body.rong / sophanrong;
	int chieucao = bmpnguon.body.cao / sophancao;

	if (bmpnguon.body.rong % sophanrong == 0 && bmpnguon.body.cao % sophancao == 0)
	{
		char *filedich = new char[strlen(tendich) + strlen(BMP_EXT) + 1];

		BmpFile bmpdich;
		int y = 0;
		for (int i = 0; i < sophancao; i++)
		{
			int x = 0;
			for (int j = 0; j < sophanrong; j++)
			{
				copyBmp(bmpnguon, bmpdich, x, y, chieurong, chieucao);
				xuLiTenBmpDich(tendich, BMP_EXT, i, j, filedich);
				ghiBmp(filedich, bmpdich);

				huyMangPixel(bmpdich);
				x += chieurong;
			}
			y += chieucao;
		}

		huyMangPixel(bmpnguon);
	}
	else	//Xu li cho truong hop chieu rong va chieu cao chia phan ra chieu cao va chieu rong file dich le.
	{
		int chieurongdu, chieucaodu;

		if (bmpnguon.body.rong % sophanrong != 0)
		{
			chieurong = chieurong + 1;
			chieurongdu = bmpnguon.body.rong - chieurong*(sophanrong - 1);
		}
		if (bmpnguon.body.cao % sophancao != 0)
		{
			chieucao = chieucao + 1;
			chieucaodu = bmpnguon.body.cao - chieucao*(sophancao - 1);
		}

		if (bmpnguon.body.rong % sophanrong != 0 && bmpnguon.body.cao % sophancao != 0)
		{
			char *filedich = new char[strlen(tendich) + strlen(BMP_EXT) + 1];

			BmpFile bmpdich;
			int y = 0;
			for (int i = 0; i < sophancao; i++)
			{
				int x = 0;
				for (int j = 0; j < sophanrong; j++)
				{
					copyBmp(bmpnguon, bmpdich, x, y, chieurong, chieucao);
					xuLiTenBmpDich(tendich, BMP_EXT, i, j, filedich);
					ghiBmp(filedich, bmpdich);

					huyMangPixel(bmpdich);

					x += chieurong;
					if ((bmpnguon.body.rong - x) / chieurong != 0)
					{
						chieurong = bmpnguon.body.rong / sophanrong + 1;
					}
					else
					{
						chieurong = chieurongdu;
					}

					//chieucao = bmpnguon.body.cao / sophancao + 1;
				}

				chieurong = bmpnguon.body.rong / sophanrong + 1;

				y += chieucao;
				if ((bmpnguon.body.cao - y) / chieucao != 0)
				{
					chieucao = bmpnguon.body.cao / sophancao + 1;
				}
				else
				{
					chieucao = chieucaodu;
				}
			}

			huyMangPixel(bmpnguon);
		}
		else
		{
			if (bmpnguon.body.rong % sophanrong == 0 && bmpnguon.body.cao % sophancao != 0)
			{
				char *filedich = new char[strlen(tendich) + strlen(BMP_EXT) + 1];

				BmpFile bmpdich;
				int y = 0;
				for (int i = 0; i < sophancao; i++)
				{
					int x = 0;
					for (int j = 0; j < sophanrong; j++)
					{
						copyBmp(bmpnguon, bmpdich, x, y, chieurong, chieucao);
						xuLiTenBmpDich(tendich, BMP_EXT, i, j, filedich);
						ghiBmp(filedich, bmpdich);

						huyMangPixel(bmpdich);
						x += chieurong;
					}
					y += chieucao;

					if ((bmpnguon.body.cao - y) / chieucao != 0)
					{
						chieucao = bmpnguon.body.cao / sophancao + 1;
					}
					else
					{
						chieucao = chieucaodu;
					}
				}

				huyMangPixel(bmpnguon);
			}
			else
			{
				char *filedich = new char[strlen(tendich) + strlen(BMP_EXT) + 1];

				BmpFile bmpdich;
				int y = 0;
				for (int i = 0; i < sophancao; i++)
				{
					int x = 0;
					for (int j = 0; j < sophanrong; j++)
					{
						copyBmp(bmpnguon, bmpdich, x, y, chieurong, chieucao);
						xuLiTenBmpDich(tendich, BMP_EXT, i, j, filedich);
						ghiBmp(filedich, bmpdich);

						huyMangPixel(bmpdich);
						x += chieurong;

						if ((bmpnguon.body.rong - x) / chieurong != 0)
						{
							chieurong = bmpnguon.body.rong / sophanrong + 1;
						}
						else
						{
							chieurong = chieurongdu;
						}
					}

					chieurong = bmpnguon.body.rong / sophanrong + 1;
					y += chieucao;
				}
			}
		}
	}
}