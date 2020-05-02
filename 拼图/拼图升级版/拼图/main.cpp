/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, DeRoy

 ******************************************************************************
  �� �� ��   : main.cpp
  �� �� ��   : ����
  ��    ��   : DeRoy
  ��������   : 2019��10��29��
  ����޸�   : 2019��11��21��
  ��������   : ƴͼ��Ϸ
  �����б�   :
*
*       1.                DrawMap	���Ƶ�ͼ
*       2.                GameInit	��ʼ����Ϸ����
*       3.                init		������Դ
*       4.                inverseNumber	������
*       5.                Judg		�ж���Ӯ
*       6.                main		������
*       7.                play		��Ҳ���
*       8.                start		��ʼ����
*
  �޸���ʷ   :
  1.��    ��   : 2019��11��21��
    ��    ��   : DeRoy
    �޸�����   : ������ż�����жϣ���ͼ���ݴ����㷨����
				��������м���������ͼƬ
******************************************************************************/
#include<stdio.h>
#include<easyx.h>	//ͼ�ο�
#include<time.h>	//�����
#include<conio.h>	//getch()
//ʹ�����ֲ��ſ�
#include<Windows.h>
#pragma  comment (lib,"Winmm.lib") 

#define N 4				//��ͼ��4��4��ʽ

IMAGE img[4], imgs[16];	//img[4] 4��ԭͼ���ĸ��ؿ�;	imgs�ָ�ͼƬ
int aim_c, aim_r;		//�ٶ��հ״�ΪĿ��  ����ΪĿ������λ�õ�	�� ��
int map[4][4] = { 0 };	//��ͼ��ʼ��Ϊ0
int NUM = 0;			//�ؿ�

//��Ϸ���򣬿�ʼ�������
void start();

//������Դ
void init();

//��Ϸ���ݳ�ʼ��
void GameInit();

//��Ϸ��Ⱦ
void DrawMap();

//��Ҳ���
void play();

//�ж���Ӯ
void Judg();

//���� map ���ݵ�������
int inverseNumber();

int main()
{
	initgraph(N * 100, N * 100);	//��ʼ�����ڴ�СΪ400*400
	start();						//��ʼ����
	init();							//������Դ
	GameInit();						//��Ϸ��ʼ��
	DrawMap();						//��Ϸ��Ⱦ
	while (1)	
	{
		play();						//��Ҳ���
		Judg();						//�ж���Ӯ
	}
	closegraph();					//�رմ���
	return 0;
}

void start()
{
	setbkcolor(WHITE);		//���ô��ڱ���ɫΪ��ɫ
	cleardevice();			//ˢ����Ļ
	
	setbkmode(TRANSPARENT);		//�������屳��ɫΪ͸��
	settextcolor(BLACK);		//����������ɫΪ��ɫ
	/*****************��Ϸ����*****************/
	outtextxy(170, 20, L"��Ϸ����");
	outtextxy(50, 50, L"1.����������հ�ͼ����ΧͼƬ����λ��");
	outtextxy(50, 100, L"2.����Ҽ����⴦������ʾ����ͼƬ");
	outtextxy(50, 150, L"3.����н���������ͼƬ");
	outtextxy(50, 200, L"4.���س�����ʼ��Ϸ");

	outtextxy(20, 250, L"������ݣ�");
	outtextxy(50, 300, L"��ע΢�Ź��ں�: ���ѧϰ���� ��ȡԴ��");
	outtextxy(50, 350, L"���ѧϰ����Ⱥ:  879098023");
	outtextxy(50, 400, L"��������:  ����");
	getch();
}

void init()
{
	//������ԴͼƬ    4��ͼƬ4���ؿ�
	loadimage(&img[0], L"images/1.jpg", 400, 400);
	loadimage(&img[1], L"images/2.jpg", 400, 400);
	loadimage(&img[2], L"images/3.jpg", 400, 400);
	loadimage(&img[3], L"images/4.jpg", 400, 400);

	//�������һ��ͼƬΪ�հ�ͼƬ����ΪĿ��ͼƬ
	loadimage(&imgs[15], L"images/15.jpg", 100, 100);

	/********��������********/
	//�ļ�·���������·��	alias ȡ����
	mciSendString(L"open images/music/����.mp3 alias back", nullptr, 0, nullptr);	
	mciSendString(_T("play back repeat"), 0, 0, 0);		//repeatѭ������

	srand((unsigned)time(NULL));						//�����������
}

void GameInit()
{
	//��ƴͼͼƬ����ȥ
	putimage(0, 0, &img[NUM]);

	//���û�ͼĿ��Ϊ img ����		��ƴͼͼƬ�����и�
	SetWorkingImage(&img[NUM]);
	for (int y = 0, n = 0; y < N; y++)
	{
		for (int x = 0; x < N; x++)
		{
			if (n == 15)	break;
			//�����ȡ100*100����ͼƬ �洢��imgs����
			getimage(&imgs[n++], x * 100, y * 100, (x + 1) * 100, (y + 1) * 100);
		}
	}
	//���û�ͼĿ��Ϊ��ͼ����
	SetWorkingImage();
	map[N - 1][N - 1] = 15;
	do
	{
		int index;      //a������±�
		int size = 15;  //a�����Ԫ�ظ���
		int a[15] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				//���½ǿհ׿ɽ���ͼƬ����
				if (i == 3 && j == 3) continue;
				//��15�����������ȡһ��
				index = rand() % size;
				//�������ȡd��������������ͼ��
				map[i][j] = a[index];
				//����ȡ��λ�ú���d��������ǰŲһλ
				for (int k = index; k < size; k++)
				{
					a[k] = a[k + 1];
				}
				//�������鳤�� ��ȡ��һ��ͼƬ���ȼ�һ
				size--;
			}
		}
	} while (inverseNumber() % 2);	//�� map��������� ����ż����ʱִ������ѭ��
}

void DrawMap()
{
	BeginBatchDraw();	//��ʼ��ȾͼƬ
	for (int y = 0; y < N; y++)
	{
		for (int x = 0; x < N; x++)
		{
			putimage(x * 100, y * 100, &imgs[map[y][x]]);
		}
	}
	EndBatchDraw();		//������ȾͼƬ
}

void play()
{
	int col, row;			//�������λ�� �� ��
	MOUSEMSG msg;			//�����Ϣ
	msg = GetMouseMsg();	//��ȡ�����Ϣ
	switch (msg.uMsg)		//�������Ϣ����ƥ��
	{
	case WM_LBUTTONDOWN:	//�������Ϣ���������ʱ
		//��ȡ��갴��������
		col = msg.x / 100;
		if (msg.x == 400)
			col = 3;
		//��ȡ��갴��������
		row = msg.y / 100;
		if (msg.y == 400)
			row = 3;
		//�õ�Ŀ�������к���
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				if (map[i][j] == 15)	//�հ״�Ϊ����Ŀ��
				{
					aim_r = i;
					aim_c = j;
				}
			}
		}
		//���������λ�ú�Ŀ���Ƿ����ڣ����ڽ�������
		if (row == aim_r && col == aim_c + 1 ||
			row == aim_r && col == aim_c - 1 ||
			row == aim_r + 1 && col == aim_c ||
			row == aim_r - 1 && col == aim_c){
			//�����ͼƬ�Ϳհ�Ŀ��ͼƬ����
			map[aim_r][aim_c] = map[row][col];
			map[row][col] = 15;
		}
		DrawMap();
		break;
	case WM_RBUTTONDOWN:	//�������Ϣ���Ҽ�����ʱ
		putimage(0, 0, &img[NUM]);	//���ؿ�ͼƬ����������
		break;
	case WM_RBUTTONUP:		//�������Ϣ���Ҽ�̧��ʱ
		DrawMap();			//���»��Ƶ�ͼ
		break;
	case WM_MBUTTONDOWN:
	{
		//���ļ��е��õ���windows�ṩ��API���������ĵ�������ҵ�
		OPENFILENAME ofn;       // common dialog box structure
		wchar_t szFile[260];       // buffer for file name
		//HANDLE hf;              // file handle
								// Initialize OPENFILENAME
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.lpstrFile = szFile;
		// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
		// use the contents of szFile to initialize itself.
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = L"All\0*.*\0jpg\0*.jpg\0";
		//ofn.nFilterIndex = 1;
		//ofn.lpstrFileTitle = NULL;
		//ofn.nMaxFileTitle = 0;
		//ofn.lpstrInitialDir = NULL;
		//ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (GetOpenFileName(&ofn) == TRUE)
		{
			loadimage(&img[NUM], szFile, 400, 400);
			GameInit();	//��Ϸ��ʼ��
			DrawMap();	//��Ⱦ��ͼ
		}
	}
		break;
	}
}

void Judg()
{
	//�жϵ�ÿ��ͼƬ�Ƿ��ڶ�Ӧλ��
	if (inverseNumber() == 0)
	{
		//��ս�ɹ�֮��ȫͼ����
		putimage(0, 0, &img[NUM++]);
		//�ĸ��ؿ���ʤ��֮���˳�����
		if (NUM == 4){
			MessageBox(GetHWnd(), L"��ս�ɹ�", L"Vectory", MB_OK);
			exit(0);
			return;
		}
		//û��һ���ؿ��ж��Ƿ������һ���ؿ�
		if(MessageBox(GetHWnd(), L"�Ƿ������һ��", L"Vectory", MB_YESNO) == IDYES){
			//���¿�ʼ��Ϸ
			GameInit();	//��Ϸ��ʼ��
			DrawMap();	//��Ⱦ��ͼ
		}
		//�˳���Ϸ
		else  exit(0);
	}
}

/*****************************************************************************
 �� �� ��  : inverseNumber
 ��������  : ��ż�����жϺ���
 �������  : ��
 �������  : ��
 �� �� ֵ  : int
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2019��11��21��
    ��    ��   : DeRoy
    �޸�����   : �����ɺ���
*****************************************************************************/
int inverseNumber()
{
	int sum = 0, k = 0;		//sumΪ������
	int arr[ N * N ];		
	//��map��������ݵ��뵽һά����arr����ȥ
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			arr[k++] = map[i][j];
		}
	}
	//��������
	for (int i = 1; i < N * N; i++)		//�ӵڶ���������������
	{
		for (int j = 0; j < i; j++)		//ѭ������ ��i���� ǰ�����
		{
			if (arr[i] < arr[j])
			{
				sum++;
			}
		}
	}
	return sum;	//����������
}



