// 愚人节快乐.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "愚人节快乐.h"

#define MAX_LOADSTRING 100

// 全局变量: 
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明: 
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

    // TODO: 在此放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY));

    MSG msg;

    // 主消息循环: 
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
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
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
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

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
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
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
		CreateWindowEx(0, L"button", L"好", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 470, 380, 80, 30, hWnd, (HMENU)1001, hInst, NULL);
		CreateWindowEx(0, L"button", L"孬", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, index_x, index_y, 80, 30, hWnd, (HMENU)1002, hInst, NULL);
		break;
	case WM_MOUSEMOVE:	//WM_MOUSEMOVE WM_LBUTTONDOWN
	{
		//int x, y;
		//x = LOWORD(lParam); //取得鼠标x坐标值(低位字节的信息)
		//y = HIWORD(lParam); //取得鼠标y坐标值(高位字节的信息)
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
            // 分析菜单选择: 
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
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
			outtextxy(hWnd, 110, 40, L"做我女朋友好不好?");
			outtextxy(hWnd, 100, 100, L"这句话哪个字重复了?");

			if (!isTrue)
			{
				Loadimage(hWnd, 210, 180, 190, 190, L"疯狂暗示.bmp");
				Loadimage(hWnd, 500 - index, 180, 120, 120, L"我要牵你的手.bmp");
			}
			else
			{
				while (index != 200)
				{
					Loadimage(hWnd, index, 180, 120, 120, L"给你牵.bmp");
					Loadimage(hWnd, 500 - index, 180, 120, 120, L"我要牵你的手.bmp");
					Sleep(200);
					index += 10;
					clear(hWnd);
				}
				Loadimage(hWnd, 210, 180, 190, 190, L"牵手成功.bmp");
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

// “关于”框的消息处理程序。
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
	//第1步 将位图放在工作空间
	HDC hdc = GetDC(hWnd);
	//第2步 准备资源dc
	HDC srcDc = CreateCompatibleDC(hdc);			//根据hdc去创建一个兼容的dc
	HBITMAP srcBitmap = (HBITMAP)LoadImage(hInst,	//实例句柄
		path,			//路径文件名，用相对路径
		IMAGE_BITMAP,		//表示加载的为位图
		cx, cy,				//期待这张图片加载的高宽是多少，给0，是图片多大就加载为多大
		LR_LOADFROMFILE);	//从文件加载
	SelectObject(srcDc, srcBitmap);	//把资源中加载的位图关联进兼容dc里面
	//第3步的函数和之前图形有区别
	BitBlt(hdc,	//目标dc
		x, y,	//目标区域的x,y坐标
		cx, cy,	//目标区域的宽高
		srcDc,		//有资源的兼容dc
		0, 0,		//资源区域的起始坐标
		SRCCOPY);	//画位图   SRCCOPY 表示从srcDc拷贝到hdc
	DeleteObject(srcBitmap);	//第2步里面的取消位图资源的关联
	DeleteDC(srcDc);			//释放资源dc
	ReleaseDC(hWnd, hdc);
}

void outtextxy(HWND hWnd, int x, int y, LPCWSTR text)
{
	HDC hdc = GetDC(hWnd);
	//默认系统字体 
	HFONT ft = CreateFont(50/*10*/,//字符的高度，如果给0，按默认高度来设置
	 0/*200*/,//字符的宽度，如果给0，按高度横纵比来匹配宽度
	 0/*1800*/,//文字的倾斜角度，10表示1度
	 0,//基线的倾斜角度
	 100,//字体的粗细，取值0-900，400以下为细体，700以上为粗体
	 false,//斜体，取值false,true
	 false,//下划线，取值false,true
	 false,//水平线，取值false,true
	 /*ANSI_CHARSET*/GB2312_CHARSET,//字符集
	 0,//输出精度
	 0,//裁剪精度
	 0,
	 0,
	 _T("幼圆"));//字样名 幼圆 方正舒体
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
