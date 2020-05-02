#include <windows.h>
#include"resource.h"
#include <math.h>
#define IDM_HELP 1001
#define IDM_TOPMOST 1002

#define IDM_TIMER 1003

// ���ں����ĺ���ԭ��
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
void DrawText(HDC hdc, int x, int y, int org, wchar_t szText[]);

int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	WCHAR szClassName[] = L"MainWClass";//�����������
	WNDCLASSEX wndclass;

	// �����������ڵĲ������WNDCLASSEX�ṹ
	wndclass.cbSize = sizeof(wndclass);	// �ṹ�Ĵ�С
	wndclass.style = CS_HREDRAW | CS_VREDRAW;	// ָ�������С�ı���ػ�
	wndclass.lpfnWndProc = MainWndProc;	// ���ں���ָ��
	wndclass.cbClsExtra = 0;		// û�ж�������ڴ�
	wndclass.cbWndExtra = 0;		// û�ж���Ĵ����ڴ�
	wndclass.hInstance = hInstance;		// ʵ����� 
	wndclass.hIcon = LoadIcon(hInstance,
		MAKEINTRESOURCE(IDI_ICON1));	// ʹ���Զ���ͼ��	// ʹ���Զ���ͼ��
	wndclass.hCursor = LoadCursor(NULL,IDC_ARROW);		// ʹ��Ԥ����Ĺ��
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	// ʹ�ð�ɫ������ˢ
	wndclass.lpszMenuName = NULL;		// ��ָ���˵�
	wndclass.lpszClassName = szClassName;	// �����������	
	wndclass.hIconSm = NULL;		//���Сͼ��

	// ע�����������
	RegisterClassEx(&wndclass);

	// ����������
	HWND hwnd = CreateWindowEx(
		0,					//dwExStyle����չ��ʽ	
		szClassName,		// lpClassName������			
		L"ʱ��",	// lpWindowName������		
		WS_DLGFRAME |  WS_POPUP | WS_SYSMENU | WS_SIZEBOX,// dwStyle�����ڷ��	
		0,		// X����ʼ X ����		
		0,		// Y����ʼ Y ����		
		GetSystemMetrics(SM_CXSCREEN),		// nWidth�����			
		GetSystemMetrics(SM_CXSCREEN),		// nHeight���߶�			
		NULL,			// hWndParent�������ھ��			
		NULL,			// hMenu���˵����		
		hInstance,		// hlnstance������ʵ�����		
		NULL);			// lpParam���û�����			

	if (hwnd == NULL)
	{
		MessageBox(NULL, L"�������ڳ���", L"error", MB_OK);
		return -1;
	}
	// ��ʾ���ڣ�ˢ�´��ڿͻ���
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	SendMessage(hwnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
	// ����Ϣ����ȡ����Ϣ
	MSG msg;
	while (::GetMessage(&msg, NULL, 0, 0))
	{
		// ת��������Ϣ
		TranslateMessage(&msg);
		// ����Ϣ���͵���Ӧ�Ĵ��ں���
		DispatchMessage(&msg);
	}

	// ��GetMessage����0ʱ�������
	return msg.wParam;
}

static BOOL s_bTopMost = 0;
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int nCenterX, nCenterY;
	static int cxClient, cyClient;
	switch (message)
	{
	case WM_CREATE:	//��ʼ��
	{
		HMENU hSysMenu = GetSystemMenu(hwnd, FALSE);
		AppendMenu(hSysMenu, MF_SEPARATOR, 0, NULL);	//��ӷָ���
		AppendMenu(hSysMenu, MF_STRING, IDM_TOPMOST, L"������ǰ");
		AppendMenu(hSysMenu, MF_STRING, IDM_HELP, L"����");

		//��Ӷ�ʱ��
		SetTimer(hwnd, IDM_TIMER, 1000, NULL);
	}
	break;
	//case WM_NCHITTEST:	//����DefWindowProc��Ϣ
	//{
	//	UINT nHitTest;
	//	//��ȡDefWindowProc��Ϣ
	//	nHitTest = DefWindowProc(hwnd, WM_NCHITTEST, wParam, lParam);
	//	//������������� GetAsyncKeyState�����ķ���ֵС��0
	//	if (nHitTest == HTCLIENT && GetAsyncKeyState(MK_LBUTTON) < 0)
	//	{
	//		//�ı�DefWindowProc��Ϣ
	//		nHitTest = HTCAPTION;
	//	}
	//	return nHitTest;
	//}
	//break;
	case WM_CONTEXTMENU:	//�Ҽ��˵�
	{
		POINT pt;
		//lParam����������λ����Ϣ ��λx����λy
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);
		//��ȡϵͳ�˵�
		HMENU hSysMenu = GetSystemMenu(hwnd, FALSE);
		//����ϵͳ�˵�
		int nId = TrackPopupMenu(hSysMenu, TPM_LEFTALIGN | TPM_RETURNCMD, pt.x, pt.y, 0, hwnd, NULL);
		if (nId > 0)
			SendMessage(hwnd, WM_SYSCOMMAND, nId, 0);
		return 0;
	}
	break;
	case WM_SYSCOMMAND:	//��ϵͳ�˵�������Ӧ
	{
		int nId = wParam;
		switch (nId)
		{
		case IDM_HELP:
			MessageBox(hwnd, L"һ��Сʱ�ӳ���", L"����", 0);
			break;
		case IDM_TOPMOST://���ڶ���
		{
			HMENU hSysMenu = GetSystemMenu(hwnd, FALSE);
			if (s_bTopMost)
			{
				//ȡ��ѡ��
				CheckMenuItem(hSysMenu, IDM_TOPMOST, MF_UNCHECKED);
				//ȡ�����ô���
				SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOREDRAW | SWP_NOSIZE);
				s_bTopMost = FALSE;
			}
			else
			{
				//ѡ��
				CheckMenuItem(hSysMenu, IDM_TOPMOST, MF_CHECKED);
				//����
				SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOREDRAW | SWP_NOSIZE);
				s_bTopMost = TRUE;
			}
			return DefWindowProc(hwnd, WM_SYSCOMMAND, nId, 0);
		}
		break;
		default:
			break;
		}
	}
	break;
	case WM_TIMER:	//�Զ�ʱ����Ϣ������Ӧ
	{
		HDC hdc = GetDC(hwnd);
		RECT rt;
		GetClientRect(hwnd, &rt);	//��ȡ�ͻ�����С
		SetBkMode(hdc, TRANSPARENT);
		//������
		HBRUSH hb = CreateSolidBrush(RGB(255, 255, 255));
		SelectObject(hdc, hb);
		Rectangle(hdc, rt.left - 10, rt.top - 10, rt.right + 10, rt.bottom + 10);
		DeleteObject(hb);

		//����������
		int cx = rt.right;
		int cy = rt.bottom;
		SetViewportExtEx(hdc, cx, cy, NULL);
		//����ԭ��
		SetViewportOrgEx(hdc, rt.right / 2, rt.bottom / 2, NULL);

		//��ȡϵͳʱ��
		wchar_t szTime[100];
		SYSTEMTIME currentTime;
		GetLocalTime(&currentTime);
		//������ر���
		int i = 0;
		int initOrg = 0;	//�����ʼ��ת�Ƕ�
		int minSize = 0;	//����ʱ����С�뾶
		int date_x = 0;		//ʱ����ʾ��x����
		//ȷ��ʱ����С�뾶
		if (rt.bottom > rt.right)
		{
			minSize = rt.right / 2;
		}
		else
			minSize = rt.bottom / 2;
		//������
		wsprintf(szTime, L"%d ��", currentTime.wYear);
		DrawText(hdc, -30, 0, 0, szTime);

		//������
		i = 1;
		date_x = minSize / 6;
		initOrg = -(currentTime.wMonth * 360 / 12) + 360 / 12;
		for (int org = initOrg; org < initOrg + 360; org += (360 / 12))
		{
			wchar_t tempArr[MAX_PATH];
			double temp = 3.1415926 * 2 * org / 360;
			int x = date_x * cos(temp);
			int y = -date_x * sin(temp);
			wsprintf(tempArr, L"%d", i++);
			DrawText(hdc, x, y, org, tempArr);
		}
		DrawText(hdc, date_x + 20, 0, 0, L"��");

		//������
		i = 1;
		date_x = minSize * 2 / 6;
		initOrg = -(currentTime.wDay * 360 / 30) + 360 / 30;
		for (int org = initOrg; org < initOrg + 360; org += (360 / 30))
		{
			wchar_t tempArr[MAX_PATH];
			double temp = 3.1415926 * 2 * org / 360;
			int x = date_x * cos(temp);
			int y = -date_x * sin(temp);
			wsprintf(tempArr, L"%d", i++);
			DrawText(hdc, x, y, org, tempArr);
		}
		DrawText(hdc, date_x + 20, 0, 0, L"��");

		//����ʱ��
		i = 0;
		date_x = minSize * 3 / 6;
		initOrg = -currentTime.wHour * 360 / 24;
		for (int org = initOrg; org < initOrg + 360; org += (360 / 24))
		{
			wchar_t tempArr[MAX_PATH];
			double temp = 3.1415926 * 2 * org / 360;
			int x = date_x * cos(temp);
			int y = -date_x * sin(temp);
			wsprintf(tempArr, L"%d", i++);
			DrawText(hdc, x, y, org, tempArr);
		}
		DrawText(hdc, date_x + 20, 0, 0, L"ʱ");

		//���Ʒ���
		i = 0;
		date_x = minSize * 4 / 6;
		initOrg = -currentTime.wMinute * 360 / 60;
		for (int org = initOrg; org < initOrg + 360; org += (360 / 60))
		{
			wchar_t tempArr[MAX_PATH];
			double temp = 3.1415926 * 2 * org / 360;
			int x = date_x * cos(temp);
			int y = -date_x * sin(temp);
			wsprintf(tempArr, L"%d", i++);
			DrawText(hdc, x, y, org, tempArr);
		}
		DrawText(hdc, date_x + 20, 0, 0, L"��");

		//��������
		i = 0;
		date_x = minSize * 5 / 6;
		initOrg = -currentTime.wSecond * 360 / 60;
		for (int org = initOrg; org < initOrg + 360; org += (360 / 60))
		{
			wchar_t tempArr[MAX_PATH];
			double temp = 3.1415926 * 2 * org / 360;
			int x = date_x * cos(temp);
			int y = -date_x * sin(temp);
			wsprintf(tempArr, L"%d", i++);
			DrawText(hdc, x, y, org, tempArr);
		}
		DrawText(hdc, date_x + 20, 0, 0, L"��");
		ReleaseDC(hwnd, hdc);
	}
	break;
	case WM_PAINT: // ���ڿͻ�����Ҫ�ػ�
	{
		HDC hdc;
		PAINTSTRUCT ps;
		hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		return 0;
	}
	case WM_DESTROY: // �������ٴ���
					 // ����Ϣ����Ͷ��һ��WM_QUIT��Ϣ����ʹGetMessage��������0��������Ϣѭ��
		PostQuitMessage(0);
		return 0;
	}
	// �����ǲ��������Ϣ����ϵͳ��Ĭ�ϴ���
	return DefWindowProc(hwnd, message, wParam, lParam);
}

//������б����
void DrawText(HDC hdc, int x, int y, int org, wchar_t szText[])
{
	SetTextAlign(hdc, TA_BOTTOM | TA_LEFT);	//�޸�ê��
	HFONT ft = CreateFont(0, 0, org * 10, 0, 100, false, false, false, GB2312_CHARSET, 0, 0, 0, 0, NULL);//��������
	SelectObject(hdc, ft);
	TextOut(hdc, x, y, szText, wcslen(szText));
	DeleteObject(ft);
}