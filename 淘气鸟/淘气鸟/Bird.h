#pragma once
class Bird
{
	IMAGE m_bird[4][2];
	int m_index;
	int m_x, m_y;		//小鸟的状态 平 飞 平 降

	float m_Dps;		//帧时间
	float m_PassTime;	//经过的时间
	bool m_isMove;		//是否移动
	bool m_isStart;		//是否开始进入场景
	bool m_isUp;		//是否跳跃
	float m_JumpTimer;	//跳跃的时间

	RECT m_BirdRect;	//小鸟的矩形区域
public:
	Bird();
	~Bird();
	void initBird();	//初始化
	void drawBird();	//绘制图形
	void updateBird(DWORD PassTime);//数据更新
public:
	//获取是否开始状态
	bool GetBirdIsStart() { return m_isStart; }	
	//设置是否开始状态
	void SetBirdIsStart(bool isStart) { m_isStart = isStart; }
	void SetBirdIsUp(bool isUp) { m_isUp = isUp; }
	//获取小鸟所在矩形
	RECT GetBirdRect() { return m_BirdRect; }
};

