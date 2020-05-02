#include<stdio.h>
#include<easyx.h>
#include<conio.h>
#include<Windows.h>

#pragma  comment (lib,"Winmm.lib") //ʹ�����ֲ��ſ�

/**********����ȫ�ֺ���********/
void InitGame();
void RenderGame();
void UpdateGame();
bool isWin();
bool isFail();
/**********����ȫ�ֱ���********/
IMAGE img[6];
char dir;
int index_Row, index_Col;
int map[6][12] = {
	0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1,
	1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1,
	1, 0, 0, 0, 1, 1, 1, 1, 2, 2, 0, 1,
	1, 0, 0, 3, 6, 4, 4, 4, 2, 0, 0, 1,
	1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1,
	0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0
};

int main()
{
	initgraph(40*12, 40*6);	
	InitGame();				//������Ϸ
	RenderGame();			//������Ϸ
	while (1)
	{	
		UpdateGame();		//���ݸ���
		if (isWin())
		{
			MessageBox(GetHWnd(), L"��ϲ��Ӯ��", L"Vectory", MB_OK);
			break;
		}
		if (isFail())
		{
			MessageBox(GetHWnd(), L"��Ǹ������", L"Defeated", MB_OK);
			break;
		}
	}		
	closegraph();
	return 0;
}

void InitGame()
{	
	loadimage(&img[0], L"images/land.jpg", 40, 40);		//�յ�
	loadimage(&img[1], L"images/wall.jpg",40, 40);		//ǽ
	loadimage(&img[2], L"images/BoxYellow.jpg", 40, 40);	//����
	loadimage(&img[3], L"images/ManDOWN1.jpg", 40, 40);	//��
	loadimage(&img[4], L"images/aim.jpg", 40, 40);		//Ŀ��
	loadimage(&img[5], L"images/BoxRed.jpg", 40, 40);	//���ӵ���Ŀ�ĵ�

	mciSendString(L"open images/Music/back.mp3 alias back", nullptr, 0, nullptr);	//�ļ�·���������·��	alias ȡ����
	mciSendString(_T("play back repeat"), 0, 0, 0);		//repeatѭ������
}

void RenderGame()
{
	BeginBatchDraw();		//��ʼ��ͼ	������
	//ѭ�����Ƶ�ͼ
	for (int y = 0; y < 6; y++)		//ѭ��ÿһ��
	{
		for (int x = 0; x < 12; x++)//ѭ��ÿһ��
		{
			switch (map[y][x])
			{
			case 0:putimage(x * 40, y * 40, &img[0]);	//�յ�
				break;
			case 1:putimage(x * 40, y * 40, &img[1]);	//ǽ
				break;
			case 2:putimage(x * 40, y * 40, &img[2]);	//����
				break;
			case 3:										//���
			case 7:putimage(x * 40, y * 40, &img[3]);	//���վ��Ŀ�ĵ���
				break;
			case 4:putimage(x * 40, y * 40, &img[4]);	//Ŀ�ĵ�
				break;
			case 6:putimage(x * 40, y * 40, &img[5]);	//����վ��Ŀ�ĵ���
				break;
			}
		}
	}
	EndBatchDraw();			//������ͼ
}

void UpdateGame()
{
	/*******��Ҳ���*******/
	if(kbhit())
	{
		/******�ҵ�С��******/
		for (int Row = 0; Row < 6; Row++)
		{
			for (int Col = 0; Col < 12; Col++)
			{
				if (map[Row][Col] == 3 || map[Row][Col] == 7)
				{
					index_Row = Row;
					index_Col = Col;
				}
			}
		}

		dir = getch();
		switch (dir)
		{
		case 'w':
		case 72:
			if (map[index_Row - 1][index_Col] == 0|| map[index_Row - 1][index_Col]==4)
			{
				//�����˵������� �յ� ���� Ŀ�ĵ�  --->ֱ����ȥ
				map[index_Row][index_Col] = map[index_Row][index_Col] - 3;
				map[index_Row - 1][index_Col] = map[index_Row - 1][index_Col] + 3;
			}
			if (map[index_Row - 1][index_Col] == 2 || map[index_Row - 1][index_Col] == 6)
			{
				//�����˵������� ����
				if (map[index_Row - 2][index_Col] == 0 || map[index_Row - 2][index_Col] == 4)
				{
					//���������ǿյػ���Ŀ�ĵ�
					map[index_Row][index_Col] = map[index_Row][index_Col] - 3;
					map[index_Row - 1][index_Col] = map[index_Row - 1][index_Col] + 3 - 2;
					map[index_Row - 2][index_Col] = map[index_Row - 2][index_Col] + 2;
				}
			}
			break;
		case 's':
		case 80:
			if (map[index_Row + 1][index_Col] == 0 || map[index_Row + 1][index_Col] == 4)
			{
				//�����˵������� �յ� ���� Ŀ�ĵ�  --->ֱ����ȥ
				map[index_Row][index_Col] = map[index_Row][index_Col] - 3;
				map[index_Row + 1][index_Col] = map[index_Row + 1][index_Col] + 3;
			}
			if (map[index_Row + 1][index_Col] == 2 || map[index_Row + 1][index_Col] == 6)
			{
				//�����˵������� ����
				if (map[index_Row + 2][index_Col] == 0 || map[index_Row + 2][index_Col] == 4)
				{
					//���������ǿյػ���Ŀ�ĵ�
					map[index_Row][index_Col] = map[index_Row][index_Col] - 3;
					map[index_Row + 1][index_Col] = map[index_Row + 1][index_Col] + 3 - 2;
					map[index_Row + 2][index_Col] = map[index_Row + 2][index_Col] + 2;
				}
			}
			break;
		case 'a':
		case 75:
			if (map[index_Row][index_Col - 1] == 0 || map[index_Row][index_Col - 1] == 4)
			{
				//�����˵������ �յ� ���� Ŀ�ĵ�  --->ֱ����ȥ
				map[index_Row][index_Col] = map[index_Row][index_Col] - 3;
				map[index_Row][index_Col - 1] = map[index_Row][index_Col - 1] + 3;
			}
			if (map[index_Row][index_Col - 1] == 2 || map[index_Row][index_Col - 1] == 6)
			{
				//�����˵������ ����
				if (map[index_Row][index_Col - 2] == 0 || map[index_Row][index_Col - 2] == 4)
				{
					//��������ǿյػ���Ŀ�ĵ�
					map[index_Row][index_Col] = map[index_Row][index_Col] - 3;
					map[index_Row][index_Col - 1] = map[index_Row][index_Col - 1] + 3 - 2;
					map[index_Row][index_Col - 2] = map[index_Row][index_Col - 2] + 2;
				}
			}
			break;
		case 'd':
		case 77:
			if (map[index_Row][index_Col + 1] == 0 || map[index_Row][index_Col + 1] == 4)
			{
				//�����˵��ұ��� �յ� ���� Ŀ�ĵ�  --->ֱ����ȥ
				map[index_Row][index_Col] = map[index_Row][index_Col] - 3;
				map[index_Row][index_Col + 1] = map[index_Row][index_Col + 1] + 3;
			}
			if (map[index_Row][index_Col + 1] == 2 || map[index_Row][index_Col + 1] == 6)
			{
				//�����˵��ұ��� ����
				if (map[index_Row][index_Col + 2] == 0 || map[index_Row][index_Col + 2] == 4)
				{
					//�����ұ��ǿյػ���Ŀ�ĵ�
					map[index_Row][index_Col] = map[index_Row][index_Col] - 3;
					map[index_Row][index_Col + 1] = map[index_Row][index_Col + 1] + 3 - 2;
					map[index_Row][index_Col + 2] = map[index_Row][index_Col + 2] + 2;
				}
			}
			break;
		}
		PlaySound(L"../images/Music/Boxmove.WAV", nullptr, SND_FILENAME | SND_ASYNC);
		RenderGame();	//������Ϸ
	}
}

bool isWin()
{
	for (int Row = 0; Row < 6; Row++)
	{
		for (int Col = 0; Col < 12; Col++)
		{
			//ѭ�������Ƿ��� û��Ŀ�ĵ� �� ����
			if (map[Row][Col] == 2)
			{
				return false;
			}
		}
	}
	return true;
}

bool isFail()
{
	for (int Row = 0; Row < 6; Row++)
	{
		for (int Col = 0; Col < 12; Col++)
		{
			if (map[Row][Col] == 2)
			{
				if (map[Row - 1][Col] == 1 || map[Row + 1][Col] == 1)
				{
					if (map[Row][Col - 1] == 1 || map[Row][Col + 1] == 1)
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}


