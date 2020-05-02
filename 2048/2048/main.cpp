
#include<stdio.h>
#include<easyx.h>
#include<conio.h>
#define MAP_ROW 4	//��ͼ��
#define MAP_COL 4	//��ͼ��
#define PIXEL 75	//һ��������ռ����

int MaxScore = 0;	//��߷�
void init(int map[][MAP_COL],int *score);		//��ʼ����Ϸ
void DrawMap(int map[][MAP_COL], int score);	//��Ϸ��Ⱦ
void Play(int map[][MAP_COL], int*score);		//��Ҳ���
int Judg(int map[][MAP_COL]);					//�ж���Ӯ
void Add(int map[][MAP_COL]);					//�������
void WriteData(int map[][MAP_COL], int score);	//д������
void ReadData(int map[][MAP_COL], int* score);	//��ȡ����

int main()
{
	int map[MAP_ROW][MAP_COL];	//��ͼ��С
	int score;					//�ܳɼ�
	initgraph(MAP_ROW * PIXEL + 100, MAP_COL * PIXEL + 100);	//���ش���
	init(map, &score);			//��ʼ����ͼ����
	ReadData(map, &score);		//��ȡ����
	while (1)
	{
		DrawMap(map, score);	//���Ƶ�ͼ
		Play(map, &score);		//��Ҳ���
		switch (Judg(map))		//�ж���Ӯ  �Խ������ƥ��
		{
		case 0://Ӯ
			MessageBox(GetHWnd(), L"��Ӯ��", L"��ϲ", MB_OK);
			if (score > MaxScore)	
				MaxScore = score;
			WriteData(map, score);
			return 0;
			break;
		case -1://��
			MessageBox(GetHWnd(), L"������", L"���ź�", MB_OK);
			if (score > MaxScore)	
				MaxScore = score;
			init(map, &score);		//���ˣ��ȳ�ʼ������
			if (MessageBox(GetHWnd(), L"�Ƿ����¿�ʼ", L"��ʾ", MB_YESNO| MB_DEFBUTTON1) == IDNO)
			{
				WriteData(map, score);	//������Ϸ
				return 0;
			}
			break;
		case 1://û�п�λ��
			break;
		case 2://���п�λ��
			Add(map);
			break;
		default:
			break;
		}
		WriteData(map, score);	//������Ϸ
	}
	getchar();
	closegraph();
	return 0;
}

void init(int map[][MAP_COL], int *score)
{
	int x, y;	//���λ�ò���
	*score = 0;	//�Գɼ����г�ʼ��
	memset(map, 0, sizeof(int)*MAP_ROW*MAP_COL);	//��ʼ����ͼ
	//�Ե�ͼ���г�ʼ��
	for (int i = 0; i < 2;)
	{
		x = rand() % 4;
		y = rand() % 4;
		if (map[x][y] == 0)
		{
			if (rand() % 9 == 0)
				map[x][y] = 4;
			else
				map[x][y] = 2;
			i++;
		}
	}
}

void DrawMap(int map[][MAP_COL],int score)
{
	wchar_t arr[20];				//��ʱ����
	setbkcolor(RGB(0, 0, 0));	//������ɫ
	setbkmode(TRANSPARENT);		//�������屳����ɫ͸��
	setlinecolor(RGB(0, 0, 0));	//������ɫ
	setlinestyle(PS_SOLID, 3);		//�������
	settextcolor(RGB(0, 0, 0));	//������ɫ
	settextstyle(20, 0, L"����");	//������ �� �����С
	cleardevice();					//����
	BeginBatchDraw();				//��ʼ���Ƶ�ͼ
	IMAGE imgs;
	loadimage(&imgs, L"man.jpg");
	putimage(0, 0, &imgs);
	for (int i = 0; i <= 4; ++i)
	{
		line(PIXEL * i, 0 + 50, PIXEL * i, PIXEL * 4 + 50);//����
		line(0, PIXEL * i + 50, PIXEL * 4, PIXEL * i + 50);//����
	}
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			wsprintf(arr, L"%d", map[i][j]);					//������д���ַ���
			outtextxy(j * PIXEL + 32, i * PIXEL + 50 + 30, arr);//ָ��λ������ַ���
		}
	}
	//����
	settextcolor(RGB(0, 255, 0));	//������ɫ ��ɫ
	wsprintf(arr, L"score: %d", score);
	outtextxy(0, 15, arr);			//ָ��λ������ַ���
	//��߷�
	outtextxy(MAP_ROW * PIXEL + 10 , 5, L"��߷�:");			//ָ��λ������ַ���
	wsprintf(arr, L"%d", MaxScore);
	outtextxy(MAP_ROW * PIXEL + 15 , 25, arr);				//ָ��λ������ַ���
	outtextxy(MAP_ROW * PIXEL + 10, 80, L"��������");			//ָ��λ������ַ���
	outtextxy(MAP_ROW * PIXEL + 10, 120, L"WSAD");			//ָ��λ������ַ���
	outtextxy(MAP_ROW * PIXEL + 10, 160, L"������Ϸ");		//ָ��λ������ַ���
	outtextxy(MAP_ROW * PIXEL + 10, 280, L"QQȺ��");			//ָ��λ������ַ���
	outtextxy(MAP_ROW * PIXEL + 5, 320, L"879098023");		//ָ��λ������ַ���
	outtextxy(80, 360, L"΢�Ź��ںţ����ѧϰ����");		//ָ��λ������ַ���
	EndBatchDraw();		//�������Ƶ�ͼ
}

void Play(int map[][MAP_COL], int * score)
{
	int len;
	switch (getch())
	{
	case 72://��
	case'W':
	case'w':
		//��0�����ƶ�
		for (int j = 0; j < 4; j++)		//4�зֱ����
		{
			len = 4;
			for (int i = 0; i < len - 1; i++)
			{
				if (map[i][j] == 0)
				{
					//���������������һ������
					for (int k = i; k < 3; k++)
					{
						map[k][j] = map[k + 1][j];
						map[k + 1][j] = 0;
					}
					len--;	//����һ�� 0 len��һ
					i--;	//����ԭλ�ü���Ƿ��� 0
				}
			}
			for (int i = 0; i < len - 1; i++)
			{
				if (map[i][j] == 0) break;		//��һ��Ϊ0 ���涼��0�������в���
				if (map[i][j] == map[i + 1][j])	//����������������Ƿ���ͬ����ͬ�ͺϲ�
				{
					map[i][j] *= 2;		//�ϲ�
					//�ϲ�֮������������ǰ�ƶ�һλ
					for (int k = i + 1; k < 3; k++)
					{
						map[k][j] = map[k + 1][j];
						map[k + 1][j] = 0;
					}
					//���д��
					*score += map[i][j];
				}
			}
		}
		break;
	case 80://��
	case'S':
	case's':
		//��0�����ƶ�
		for (int j = 0; j < 4; j++)//4�зֱ����
		{
			len = 0;
			for (int i = 3; i > len; i--)
			{
				if (map[i][j] == 0)
				{
					for (int k = i; k > 0; k--)
					{
						map[k][j] = map[k - 1][j];
						map[k - 1][j] = 0;
					}
					len++;//����һ�� 0     len + 1
					i++;//����ԭλ�ü���Ƿ��� 0
				}
			}
			for (int i = 3; i > 0; i--)
			{
				if (map[i][j] == 0) break;
				if (map[i][j] == map[i - 1][j])
				{
					map[i][j] *= 2;//�ϲ�
					for (int k = i - 1; k > 0; k--)
					{
						map[k][j] = map[k - 1][j];
						map[k - 1][j] = 0;
					}
					*score += map[i][j];
				}
			}
		}
		break;
	case 75://��
	case'A':
	case'a':
		for (int i = 0; i < 4; i++)
		{
			len = 4;
			for (int j = 0; j < len - 1; j++)
			{
				if (map[i][j] == 0)
				{
					for (int k = j; k < 3; k++)
					{
						map[i][k] = map[i][k + 1];
						map[i][k + 1] = 0;
					}
					len--;
					j--;
				}
			}
			for (int j = 0; j < 3; j++)
			{
				if (map[i][j] == 0) break;
				if (map[i][j] == map[i][j + 1])
				{
					map[i][j] *= 2;
					for (int k = j + 1; k < 3; k++)
					{
						map[i][k] = map[i][k + 1];
						map[i][k + 1] = 0;
					}
					*score += map[i][j];
				}
			}
		}
		break;
	case 77://��
	case'd':
	case'D':
		for (int i = 0; i < 4; i++)
		{
			len = 0;
			for (int j = 3; j > len; j--)
			{
				if (map[i][j] == 0)
				{
					for (int k = j; k > 0; k--)
					{
						map[i][k] = map[i][k - 1];
						map[i][k - 1] = 0;
					}
					len++;
					j++;
				}
			}
			for (int j = 3; j > 0; j--)
			{
				if (map[i][j] == map[i][j - 1])
				{
					map[i][j] *= 2;
					for (int k = j - 1; k > 0; k--)
					{
						map[i][k] = map[i][k - 1];
						map[i][k - 1] = 0;
					}
					*score += map[i][j];
				}
			}
		}
		break;
	default:break;
	}
}

int Judg(int map[][MAP_COL])
{
	int num = 0;	//��¼0�ĸ���
	int flag = 0;	//�������һ����Ժϲ���Ԫ�� ��ôflag��Ϊ1
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			if (map[i][j] >= 2048) return 0;//Ӯ
			if (map[i][j] == 0) num++;		//��¼0�ĸ���
			if (j + 1 < 4 && map[i][j] == map[i][j + 1]) flag = 1;//�ұ��ǲ���һ��
			if (i + 1 < 4 && map[i][j] == map[i + 1][j]) flag = 1;//�·��ǲ���һ��
		}
	}
	if (num == 0 && flag == 0)  return -1;	//���˲��Ҳ��ܺϲ�  ��
	if (num == 0) return 1;					//���� ���ǿ��Ժϲ�
	return 2;
}

void Add(int map[][MAP_COL])
{
	//�������
	int x, y;
	do
	{
		x = rand() % 4;
		y = rand() % 4;
	} while (map[x][y] != 0);	//�ҵ�����Ϊ���λ��
	if (rand() % 9 == 0)		//����4���ֵĸ���Ϊ1/9
		map[x][y] = 4;
	else
		map[x][y] = 2;
}

void WriteData(int map[][MAP_COL], int score)
{
	FILE*fp = fopen("D:\\2048.dat", "wb");
	if (fp == NULL) return;
	fwrite(map, sizeof(int)*MAP_ROW*MAP_COL, 1, fp);
	fwrite(&score, sizeof(int), 1, fp);
	fwrite(&MaxScore, sizeof(int), 1, fp);
	fclose(fp);
}

void ReadData(int map[][MAP_COL], int* score)
{
	FILE*fp = fopen("D:\\2048.dat", "rb");
	if (fp==NULL) return;
	fread(map, sizeof(int)*MAP_ROW*MAP_COL, 1, fp);
	fread(score, sizeof(int), 1, fp);
	fread(&MaxScore, sizeof(int), 1, fp);
	fclose(fp);
}

