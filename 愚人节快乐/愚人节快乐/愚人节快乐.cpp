// ���˽ڿ���.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "���˽ڿ���.h"

#define MAX_LOADSTRING 100

// ȫ�ֱ���: 
HINSTANCE hInst;                                // ��ǰʵ��
WCHAR szTitle[MAX_LOADSTRING];                  // �������ı�
WCHAR szWindowClass[MAX_LOADSTRING];            // ����������

// �˴���ģ���а����ĺ�����ǰ������: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void Loadimage(HWND hWnd, int x, int y, int cx, int cy, LPCWSTR path);
void outtextxy(HWND hWnd, int x, int y, LPCWSTR text);
void clear(HWND hWnd);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: �ڴ˷��ô��롣

    // ��ʼ��ȫ���ַ���
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ִ��Ӧ�ó����ʼ��: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY));

    MSG msg;

    // ����Ϣѭ��: 
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName = NULL;/*MAKEINTRESOURCEW(IDC_MY);*/
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_SYSMENU | WS_MINIMIZEBOX | WS_THICKFRAME | WS_VISIBLE, 
      CW_USEDEFAULT, 0, 640, 480, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND  - ����Ӧ�ó���˵�
//  WM_PAINT    - ����������
//  WM_DESTROY  - �����˳���Ϣ������
//
//
bool isTrue = false;
int index = 50;
int index_x = 50, index_y = 380;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
		CreateWindowEx(0, L"button", L"��", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 470, 380, 80, 30, hWnd, (HMENU)1001, hInst, NULL);
		CreateWindowEx(0, L"button", L"ث", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, index_x, index_y, 80, 30, hWnd, (HMENU)1002, hInst, NULL);
		break;
	case WM_MOUSEMOVE:	//WM_MOUSEMOVE WM_LBUTTONDOWN
	{
		//int x, y;
		//x = LOWORD(lParam); //ȡ�����x����ֵ(��λ�ֽڵ���Ϣ)
		//y = HIWORD(lParam); //ȡ�����y����ֵ(��λ�ֽڵ���Ϣ)
		//if ((x >= index_x&&x <= index_x + 100) && (y >= index_y&&y <= index_y + 50))
		//{
			index_x = (index_x + 100)%400;
			MoveWindow(GetDlgItem(hWnd, 1002), index_x, index_y, 80, 30, TRUE);
		//}
	}
	break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // �����˵�ѡ��: 
            switch (wmId)
            {
			case 1001:
				isTrue = true;	
				SendMessage(hWnd, WM_PAINT, NULL, NULL);
				break;
			case 1002:
	/*			index_x = rand() % 560;
				index_y = rand() % 450;
				MoveWindow(GetDlgItem(hWnd, 1002), index_x, index_y, 80, 30, TRUE);*/
				break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: �ڴ˴����ʹ�� hdc ���κλ�ͼ����...
			outtextxy(hWnd, 110, 40, L"����Ů���Ѻò���?");
			outtextxy(hWnd, 100, 100, L"��仰�ĸ����ظ���?");

			if (!isTrue)
			{
				Loadimage(hWnd, 210, 180, 190, 190, L"���ʾ.bmp");
				Loadimage(hWnd, 500 - index, 180, 120, 120, L"��Ҫǣ�����.bmp");
			}
			else
			{
				while (index != 200)
				{
					Loadimage(hWnd, index, 180, 120, 120, L"����ǣ.bmp");
					Loadimage(hWnd, 500 - index, 180, 120, 120, L"��Ҫǣ�����.bmp");
					Sleep(200);
					index += 10;
					clear(hWnd);
				}
				Loadimage(hWnd, 210, 180, 190, 190, L"ǣ�ֳɹ�.bmp");
			}
			
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// �����ڡ������Ϣ�������
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void Loadimage(HWND hWnd, int x, int y,int cx,int cy, LPCWSTR path)
{
	//��1�� ��λͼ���ڹ����ռ�
	HDC hdc = GetDC(hWnd);
	//��2�� ׼����Դdc
	HDC srcDc = CreateCompatibleDC(hdc);			//����hdcȥ����һ�����ݵ�dc
	HBITMAP srcBitmap = (HBITMAP)LoadImage(hInst,	//ʵ�����
		path,			//·���ļ����������·��
		IMAGE_BITMAP,		//��ʾ���ص�Ϊλͼ
		cx, cy,				//�ڴ�����ͼƬ���صĸ߿��Ƕ��٣���0����ͼƬ���ͼ���Ϊ���
		LR_LOADFROMFILE);	//���ļ�����
	SelectObject(srcDc, srcBitmap);	//����Դ�м��ص�λͼ����������dc����
	//��3���ĺ�����֮ǰͼ��������
	BitBlt(hdc,	//Ŀ��dc
		x, y,	//Ŀ�������x,y����
		cx, cy,	//Ŀ������Ŀ��
		srcDc,		//����Դ�ļ���dc
		0, 0,		//��Դ�������ʼ����
		SRCCOPY);	//��λͼ   SRCCOPY ��ʾ��srcDc������hdc
	DeleteObject(srcBitmap);	//��2�������ȡ��λͼ��Դ�Ĺ���
	DeleteDC(srcDc);			//�ͷ���Դdc
	ReleaseDC(hWnd, hdc);
}

void outtextxy(HWND hWnd, int x, int y, LPCWSTR text)
{
	HDC hdc = GetDC(hWnd);
	//Ĭ��ϵͳ���� 
	HFONT ft = CreateFont(50/*10*/,//�ַ��ĸ߶ȣ������0����Ĭ�ϸ߶�������
	 0/*200*/,//�ַ��Ŀ�ȣ������0�����߶Ⱥ��ݱ���ƥ����
	 0/*1800*/,//���ֵ���б�Ƕȣ�10��ʾ1��
	 0,//���ߵ���б�Ƕ�
	 100,//����Ĵ�ϸ��ȡֵ0-900��400����Ϊϸ�壬700����Ϊ����
	 false,//б�壬ȡֵfalse,true
	 false,//�»��ߣ�ȡֵfalse,true
	 false,//ˮƽ�ߣ�ȡֵfalse,true
	 /*ANSI_CHARSET*/GB2312_CHARSET,//�ַ���
	 0,//�������
	 0,//�ü�����
	 0,
	 0,
	 _T("��Բ"));//������ ��Բ ��������
	SelectObject(hdc, ft);
	TextOut(hdc, x, y, text, 10);
	DeleteObject(ft);
	ReleaseDC(hWnd, hdc);
}

void clear(HWND hWnd)
{
	HDC hdc = GetDC(hWnd);
	HBRUSH hb = CreateSolidBrush(RGB(255, 255, 255));
	HPEN hPen = CreatePen(PS_NULL, 1, 0);
	SelectObject(hdc, hb);
	SelectObject(hdc, hPen);
	Rectangle(hdc, -10, 180, 640, 380);
	DeleteObject(hb);
	DeleteObject(hPen);
	ReleaseDC(hWnd, hdc);
}
