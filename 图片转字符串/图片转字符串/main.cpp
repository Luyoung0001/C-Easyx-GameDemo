#include<stdio.h>
#include<easyx.h>

char GetCh(int data);
char ch[] = { ' ', '`', '.', '^', ',', ':', '~', '"', '<', '!', 'c', 't', '+',
'{', 'i', '7', '?', 'u', '3', '0', 'p', 'w', '4', 'A', '8', 'D', 'X', '%', '#', 'H', 'W', 'M' };

char num[] = { 0, 5, 7, 9, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31, 33, 35, 37, 39, 41,
43, 45, 47, 49, 51, 53, 55, 59, 61, 63, 66, 68, 70 };

int main()
{
	IMAGE img;
	loadimage(&img, L"test.bmp");
	initgraph(img.getwidth(), img.getheight());
	putimage(0, 0, &img);
	FILE *fp = fopen("图片转ASCII.txt", "w");//创建文件
	if (!fp) return 0;

	int gray;	//灰度值
	for (int i = 0; i < img.getheight() / 16; ++i)
	{
		for (int j = 0; j < img.getwidth() / 16; ++j)
		{
			gray = 0;
			for (int x = 0; x < 16; ++x)
			{
				for (int y = 0; y < 16; ++y)
				{
					//统计这个方块中的灰度值 求平均
					gray += GetRValue(RGBtoGRAY(getpixel(j * 16 + x, i * 16 + y)));
				}
			}
			gray /= 16 * 16;	//取平均值
			//gray * (70 / 255)    将灰度值  转换成0~70之间的值 
			//由于0是黑色 255是白色 gray * (70 / 255) 得到的灰度值与所给字符灰度排列相反
			//所有 70 - gray * 70 / 255 才是我们需要的灰度值
			//匹配数组中的数字  找到对应字符
			fputc(GetCh(70 - gray * 70 / 255), fp);
		}
		fputc('\n', fp);	//每一行加个换行符号
	}
	fclose(fp);
	getchar();
	closegraph();
	return 0;
}

char GetCh(int data)
{
	//快速查找
	int left = 0, right = 31, index;	//num的最大元素下标是31
	while (left < right)
	{
		index = (left + right) / 2;
		if (num[index] == data) return ch[index];		//找到数字 直接返回对应的字符
		else if (num[index]>data) right = index - 1;	//往左找
		else left = index + 1;
	}
	//退出的条件  left>=right
	return ch[left];	//return ch[right]也可以			
}
