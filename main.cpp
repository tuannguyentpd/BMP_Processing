#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"ham.h"

void main(int argc, char**argv)
{
	/*cutFileBmp("lena.bmp", "copy", 3, 5);*/

	//Doc tham so dong lenh va xu li (voi tham so truyen vao theo cau truc cho truoc).
	if (argc == 4)
	{
		if (strcmp(argv[2], "-h") == 0)
		{
			int hang = atoi(argv[3]);

			cutFileBmp(argv[1], "copy", hang, 1);
		}
		else
			if (strcmp(argv[2], "-w") == 0)
			{
				int cot = atoi(argv[3]);

				cutFileBmp(argv[1], "copy", 1, cot);
			}
	}
	else
	{
		if (argc == 6)
		{
			if (strcmp(argv[2], "-h") == 0 && strcmp(argv[4], "-w") == 0)
			{
				int hang = atoi(argv[3]);
				int cot = atoi(argv[5]);

				cutFileBmp(argv[1], "copy", hang, cot);
			}
			else
			{
				if (strcmp(argv[2], "-w") == 0 && strcmp(argv[4], "-h") == 0)
				{
					int hang = atoi(argv[5]);
					int cot = atoi(argv[3]);

					cutFileBmp(argv[1], "copy", hang, cot);
				}
				else
					printf("Tham so khong hop le! \nVui long nhap lai!\n");
			}
		}
		else
			printf("Nhap tham so khong hop le!\nVui long nhap lai!\n");
	}
}