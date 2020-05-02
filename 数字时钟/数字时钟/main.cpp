#include <windows.h>
#include"resource.h"
#include <math.h>
#define IDM_HELP 1001
#define IDM_TOPMOST 1002

#define IDM_TIMER 1003

// 窗口函数的函数原形
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
void DrawText(HDC hdc, int x, int y, int org, wchar_t szText[]);

int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	WCHAR szClassName[] = L"MainWClass";//窗口类的名称
	WNDCLASSEX wndclass;

	// 用描述主窗口的参数填充WNDCLASSEX结构
	wndclass.cbSize = sizeof(wndclass);	// 结构的大小
	wndclass.style = CS_HREDRAW | CS_VREDRAW;	// 指定如果大小改变就重画
	wndclass.lpfnWndProc = MainWndProc;	// 窗口函数指针
	wndclass.cbClsExtra = 0;		// 没有额外的类内存
	wndclass.cbWndExtra = 0;		// 没有额外的窗口内存
	wndclass.hInstance = hInstance;		// 实例句柄 
	wndclass.hIcon = LoadIcon(hInstance,
		MAKEINTRESOURCE(IDI_ICON1));	// 使用自定义图标	// 使用自定义图标
	wndclass.hCursor = LoadCursor(NULL,IDC_ARROW);		// 使用预定义的光标
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	// 使用白色背景画刷
	wndclass.lpszMenuName = NULL;		// 不指定菜单
	wndclass.lpszClassName = szClassName;	// 窗口类的名称	
	wndclass.hIconSm = NULL;		//类的小图标

	// 注册这个窗口类
	RegisterClassEx(&wndclass);

	// 创建主窗口
	HWND hwnd = CreateWindowEx(
		0,					//dwExStyle，扩展样式	
		szClassName,		// lpClassName，类名			
		L"时钟",	// lpWindowName，标题		
		WS_DLGFRAME |  WS_POPUP | WS_SYSMENU | WS_SIZEBOX,// dwStyle，窗口风格	
		0,		// X，初始 X 坐标		
		0,		// Y，初始 Y 坐标		
		GetSystemMetrics(SM_CXSCREEN),		// nWidth，宽度			
		GetSystemMetrics(SM_CXSCREEN),		// nHeight，高度			
		NULL,			// hWndParent，父窗口句柄			
		NULL,			// hMenu，菜单句柄		
		hInstance,		// hlnstance，程序实例句柄		
		NULL);			// lpParam，用户数据			

	if (hwnd == NULL)
	{
		MessageBox(NULL, L"创建窗口出错！", L"error", MB_OK);
		return -1;
	}
	// 显示窗口，刷新窗口客户区
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	SendMessage(hwnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
	// 从消息堆中取出消息
	MSG msg;
	while (::GetMessage(&msg, NULL, 0, 0))
	{
		// 转化键盘消息
		TranslateMessage(&msg);
		// 将消息发送到相应的窗口函数
		DispatchMessage(&msg);
	}

	// 当GetMessage返回0时程序结束
	return msg.wParam;
}

static BOOL s_bTopMost = 0;
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int nCenterX, nCenterY;
	static int cxClient, cyClient;
	switch (message)
	{
	case WM_CREATE:	//初始化
	{
		HMENU hSysMenu = GetSystemMenu(hwnd, FALSE);
		AppendMenu(hSysMenu, MF_SEPARATOR, 0, NULL);	//添加分割线
		AppendMenu(hSysMenu, MF_STRING, IDM_TOPMOST, L"总是在前");
		AppendMenu(hSysMenu, MF_STRING, IDM_HELP, L"帮助");

		//添加定时器
		SetTimer(hwnd, IDM_TIMER, 1000, NULL);
	}
	break;
	//case WM_NCHITTEST:	//拦截DefWindowProc消息
	//{
	//	UINT nHitTest;
	//	//截取DefWindowProc消息
	//	nHitTest = DefWindowProc(hwnd, WM_NCHITTEST, wParam, lParam);
	//	//如果鼠标左键按下 GetAsyncKeyState函数的返回值小于0
	//	if (nHitTest == HTCLIENT && GetAsyncKeyState(MK_LBUTTON) < 0)
	//	{
	//		//改变DefWindowProc消息
	//		nHitTest = HTCAPTION;
	//	}
	//	return nHitTest;
	//}
	//break;
	case WM_CONTEXTMENU:	//右键菜单
	{
		POINT pt;
		//lParam包含了鼠标的位置消息 低位x，高位y
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);
		//获取系统菜单
		HMENU hSysMenu = GetSystemMenu(hwnd, FALSE);
		//弹出系统菜单
		int nId = TrackPopupMenu(hSysMenu, TPM_LEFTALIGN | TPM_RETURNCMD, pt.x, pt.y, 0, hwnd, NULL);
		if (nId > 0)
			SendMessage(hwnd, WM_SYSCOMMAND, nId, 0);
		return 0;
	}
	break;
	case WM_SYSCOMMAND:	//对系统菜单进行响应
	{
		int nId = wParam;
		switch (nId)
		{
		case IDM_HELP:
			MessageBox(hwnd, L"一个小时钟程序", L"帮助", 0);
			break;
		case IDM_TOPMOST://窗口顶置
		{
			HMENU hSysMenu = GetSystemMenu(hwnd, FALSE);
			if (s_bTopMost)
			{
				//取消选中
				CheckMenuItem(hSysMenu, IDM_TOPMOST, MF_UNCHECKED);
				//取消顶置窗口
				SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOREDRAW | SWP_NOSIZE);
				s_bTopMost = FALSE;
			}
			else
			{
				//选中
				CheckMenuItem(hSysMenu, IDM_TOPMOST, MF_CHECKED);
				//顶置
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
	case WM_TIMER:	//对定时器消息进行响应
	{
		HDC hdc = GetDC(hwnd);
		RECT rt;
		GetClientRect(hwnd, &rt);	//获取客户区大小
		SetBkMode(hdc, TRANSPARENT);
		//简单清屏
		HBRUSH hb = CreateSolidBrush(RGB(255, 255, 255));
		SelectObject(hdc, hb);
		Rectangle(hdc, rt.left - 10, rt.top - 10, rt.right + 10, rt.bottom + 10);
		DeleteObject(hb);

		//设置坐标轴
		int cx = rt.right;
		int cy = rt.bottom;
		SetViewportExtEx(hdc, cx, cy, NULL);
		//设置原点
		SetViewportOrgEx(hdc, rt.right / 2, rt.bottom / 2, NULL);

		//获取系统时间
		wchar_t szTime[100];
		SYSTEMTIME currentTime;
		GetLocalTime(&currentTime);
		//定义相关变量
		int i = 0;
		int initOrg = 0;	//定义初始旋转角度
		int minSize = 0;	//数字时钟最小半径
		int date_x = 0;		//时间显示的x坐标
		//确定时钟最小半径
		if (rt.bottom > rt.right)
		{
			minSize = rt.right / 2;
		}
		else
			minSize = rt.bottom / 2;
		//绘制年
		wsprintf(szTime, L"%d 年", currentTime.wYear);
		DrawText(hdc, -30, 0, 0, szTime);

		//绘制月
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
		DrawText(hdc, date_x + 20, 0, 0, L"月");

		//绘制日
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
		DrawText(hdc, date_x + 20, 0, 0, L"日");

		//绘制时钟
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
		DrawText(hdc, date_x + 20, 0, 0, L"时");

		//绘制分钟
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
		DrawText(hdc, date_x + 20, 0, 0, L"分");

		//绘制秒钟
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
		DrawText(hdc, date_x + 20, 0, 0, L"秒");
		ReleaseDC(hwnd, hdc);
	}
	break;
	case WM_PAINT: // 窗口客户区需要重画
	{
		HDC hdc;
		PAINTSTRUCT ps;
		hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		return 0;
	}
	case WM_DESTROY: // 正在销毁窗口
					 // 向消息队列投递一个WM_QUIT消息，促使GetMessage函数返回0，结束消息循环
		PostQuitMessage(0);
		return 0;
	}
	// 将我们不处理的消息交给系统做默认处理
	return DefWindowProc(hwnd, message, wParam, lParam);
}

//绘制倾斜字体
void DrawText(HDC hdc, int x, int y, int org, wchar_t szText[])
{
	SetTextAlign(hdc, TA_BOTTOM | TA_LEFT);	//修改锚点
	HFONT ft = CreateFont(0, 0, org * 10, 0, 100, false, false, false, GB2312_CHARSET, 0, 0, 0, 0, NULL);//创建字体
	SelectObject(hdc, ft);
	TextOut(hdc, x, y, szText, wcslen(szText));
	DeleteObject(ft);
}