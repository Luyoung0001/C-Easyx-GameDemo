#pragma once
class Background
{
	IMAGE m_bk;		//背景图片
	DWORD m_PassTime;	//经过的时间
	DWORD m_Dsp;		//帧时间
	int m_iMoveSpeed;	//移动速度
	bool m_isMove;		//是否移动

	int m_x, m_y;	//绘制的位置
public:
	Background();
	~Background();
	void initBackground();	//初始化
	void updateBackground(DWORD PassTime);	//数据更新
	void drawBackground();					//绘制图形
public:
	//设置是否开始移动
	void SetBkIsMove(bool isMove) { m_isMove = isMove; }
};

