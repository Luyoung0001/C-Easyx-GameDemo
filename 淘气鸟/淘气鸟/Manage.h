#pragma once

class Manage
{
	Bird m_bird;		//小鸟类
	Background m_bk;	//背景类
	Block m_block;		//障碍物
	bool m_isStart;		//是否开始游戏
	DWORD m_passTime;	//经过的时间 用来计算分数
	int m_score;		//分数
public:
	Manage();
	~Manage();
	void initGame();					//初始化
	void updateGame(DWORD PassTime);	//数据更新
	void drawGame();					//绘制游戏
	void startUI();						//开始界面
};

