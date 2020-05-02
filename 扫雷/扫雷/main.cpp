#include<stdio.h>
#include<easyx.h>
#include<time.h>
/**************ȫ�ֱ���**************/
int map_row, map_col;	//��ͼ��С
int times;	//�����Ĵ�ʱ
int mine;	//�׵�����
int isfail;	//�ж��Ƿ�ʧ��
int pass;	//�׵������ﵽ10% ��������Ϸ����
HWND hWnd;	//��ǰ����
int map[30][30] = { 0 };	//��ͼ	��map_row map_col���Ƶ�ͼ��С
IMAGE imgs[14];	//ͼƬ
/**************ȫ�ֺ���**************/
void init();
void DrawMap();
void initmap();	//Ӯ��֮���ʼ����Ϸ���� ���µ�ͼ
void Play();
void Judg();
void Empty(int row,int col);

int main()
{
	init();		//��ʼ��
	DrawMap();	//����ͼ��
	while (1)
	{
		Play();	//��Ҳ���
		Judg();	//�ж���Ӯ
	}
	getchar();
	closegraph();
	return 0;
}

void init()
{
	srand((unsigned)time(NULL));	//�����������
	map_row = 10;	//��ͼ��
	map_col = 10;	//��ͼ��
	isfail = 0;
	pass = 0;		
	//����ͼƬ
	loadimage(&imgs[0], L"�ز�/0.bmp", 25, 25);
	loadimage(&imgs[1], L"�ز�/1.bmp", 25, 25);
	loadimage(&imgs[2], L"�ز�/2.bmp", 25, 25);
	loadimage(&imgs[3], L"�ز�/3.bmp", 25, 25);
	loadimage(&imgs[4], L"�ز�/4.bmp", 25, 25);
	loadimage(&imgs[5], L"�ز�/5.bmp", 25, 25);
	loadimage(&imgs[6], L"�ز�/6.bmp", 25, 25);
	loadimage(&imgs[7], L"�ز�/7.bmp", 25, 25);
	loadimage(&imgs[8], L"�ز�/8.bmp", 25, 25);
	loadimage(&imgs[9], L"�ز�/���.bmp", 25, 25);
	loadimage(&imgs[10], L"�ز�/��.bmp", 25, 25);
	loadimage(&imgs[11], L"�ز�/��0.bmp", 25, 25);
	loadimage(&imgs[12], L"�ز�/��1.bmp", 25, 25);
	loadimage(&imgs[13], L"�ز�/��2.bmp", 25, 25);

	initmap();
}

void DrawMap()
{
	BeginBatchDraw();
	for (int y = 1; y <= map_row; y++)
	{
		for (int x = 1; x <= map_col; x++)
		{
			if (map[y][x] > 18)
			{
				//��ǲ��ֶ��⴦��
				putimage((x - 1) * 25, (y - 1) * 25, &imgs[9]);
				continue;
			}
			switch (map[y][x])
			{
			//�������ڿ��Լ���������뿴���׷ŵ�������
			//case -1:	//��
			//	putimage(x * 25, y * 25, &imgs[12]);
			//	break;
			case 9:		// -1 + 10 = 9 �ȵ�����
				putimage((x - 1) *25, (y - 1) *25, &imgs[13]);
				isfail = 1;
				break;
			case 10:	//û����
				putimage((x - 1) * 25, (y - 1) * 25, &imgs[0]);
				break;
			case 11:	//��һ����
				putimage((x - 1) * 25, (y - 1) * 25, &imgs[1]);
				break;
			case 12:	//��������
				putimage((x - 1) * 25, (y - 1) * 25, &imgs[2]);
				break;
			case 13:	//��������
				putimage((x - 1) * 25, (y - 1) * 25, &imgs[3]);
				break;
			case 14:	//���ĸ���
				putimage((x - 1) * 25, (y - 1) * 25, &imgs[4]);
				break;
			case 15:	//�������
				putimage((x - 1) * 25, (y - 1) * 25, &imgs[5]);
				break;
			case 16:	//��������
				putimage((x - 1) * 25, (y - 1) * 25, &imgs[6]);
				break;
			case 17:	//���߸���
				putimage((x - 1) * 25, (y - 1) * 25, &imgs[7]);
				break;
			case 18:	//�а˸���
				putimage((x - 1) * 25, (y - 1) * 25, &imgs[8]);
				break;
			default:
				putimage((x - 1) * 25, (y - 1) * 25, &imgs[10]);
				break;
			}
		}
	}
	EndBatchDraw();
}

void initmap()
{
	ZeroMemory(map, sizeof(int) * 30 * 30);		//��ʼ����ͼΪ0
	times = 0;	//��������
	mine += 5;	//�׵����� + 5
	float x = mine / (float)(map_row * map_col);
	if ( x > 0.15)
	{
		//���׵������ﵽ10%ʱ������Ϸ����	�Ⱥ�������5������������5��
		if (pass++ % 2)		//ʹ pass �� 0 �� 1֮��任		������0 ��Ȼ��0 1 ֮��ѭ��
		{
			map_row += 5;	//��������5������
		}
		else
		{
			map_col += 5;	//��������5������
		}
	}

	hWnd = initgraph(map_col * 25, map_row * 25);		//��ʼ����ͼ��С

	int mine_r, mine_c;		//�������к���
	for (int i = 0; i < mine;)
	{
		mine_r = rand() % map_row + 1;	//mine_r 1 ~ 8
		mine_c = rand() % map_col + 1;
		if (map[mine_r][mine_c] == 0)
		{
			map[mine_r][mine_c] = -1;	//�������з���
			i++;						//�����׺��׵�������һ
		}
	}
	//���׸���������һ
	for (int i = 1; i <= map_row; i++)
	{
		for (int j = 1; j <= map_col; j++)
		{
			if (map[i][j] != -1)
			{
				//�����Χ�׵ĸ���
				for (int m = i - 1; m <= i + 1; m++)
				{
					for (int n = j - 1; n <= j + 1; n++)
					{
						if (map[m][n] == -1) {	//�������ף����ּ�һ
							map[i][j]++;
						}
					}
				}
			}
		}
	}

	wchar_t arr[20];
	wsprintf(arr, L"ɨ�� ��:%d", mine);
	SetWindowText(hWnd, arr);
}

void Play()
{
	MOUSEMSG msg;//�����Ϣ
	int row, col;//��������к���
	msg = GetMouseMsg();	//��ȡ�����Ϣ
	row = msg.y / 25 + 1;	//�������	
	col = msg.x / 25 + 1;	//�������
	switch (msg.uMsg)
	{
	case WM_LBUTTONDOWN://�������
		if (map[row][col] >= 9)//�������� �ͱ�ǵ� �����в���
		{
			break;
		}
		if (map[row][col] == 0)//���� 0�� �� ��Χ��ͼƬ
		{
			Empty(row,col);
		}
		else//���������ͼƬ
		{
			map[row][col] += 10;//����
			times++;	//�������� +1
		}
		break;
	case WM_RBUTTONDOWN://�Ҽ����� WM_RBUTTONDOWN
		if (map[row][col] >= 9 && map[row][col]<19)//�����Ĳ����в���
		{
			break;
		}
		if (map[row][col] >= 19)//��ǵ�ȡ�����
		{
			map[row][col] -= 20;
		}
		else//û�з����ļ�20
		{
			map[row][col] += 20;	//20����������֣�ֻҪ�㹻�����
		}
		break;
	}
	DrawMap();
}

void Judg()
{
	//�Ȳ����Ƿ�㿪����
	if (isfail)
	{
		if (MessageBox(GetHWnd(), L"��ȵ�����,�Ƿ����¿�ʼ����", L"ʧ��", MB_YESNO) == IDYES)		//GetHWnd()�ǻ�ȡһ�����ô���
		{
			mine -= 5;
			isfail = 0;
			initmap();
			DrawMap();	//��Ⱦ��ͼ
		}
		else
		{
			_exit(0);
		}
	}

	if (times == map_row * map_col - mine)	//���е� - �� = ������
	{
		if (MessageBox(GetHWnd(), L"�Ƿ������һ��", L"�ɹ�", MB_YESNO) == IDYES) {
			closegraph();
			//���¿�ʼ��Ϸ
			initmap();
			DrawMap();	//��Ⱦ��ͼ
		}
		else
			_exit(0);
	}
}

void Empty(int row, int col)
{
	//�ݹ�ʵ���� 0 Ϊ �Ź�������� �Զ�����
	map[row][col] += 10;	//������ǰλ��
	times++;				//��������+1
	for (int i = row - 1; i <= row + 1; i++)
	{
		for (int j = col - 1; j <= col + 1; j++)
		{
			//��Ȧ�հ����򲻲���	���Կ��Կ�����������еݹ����
			if (i >= 1 && i <= map_row && j >= 1 && j <= map_col)
			{
				if (map[i][j] < 9)			//С��9 ��δ������δ����������
				{
					if (map[i][j] == 0)		//0����Χ�Ź��������ף�����
					{
						Empty(i, j);
					}
					else
					{
						map[i][j] += 10;	//���׵Ĳ����������˼���Ƿ���Ҫ����
						times++;
					}
				}
			}
		}
	}
}


