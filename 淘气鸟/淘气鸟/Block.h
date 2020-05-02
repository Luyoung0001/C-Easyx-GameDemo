#pragma once
class Block
{
	IMAGE m_block;	

	float m_PassTime;	//经过的时间
	float m_Dps;		//帧时间
	int m_iMoveSpeed;	//移动速度
	bool m_isMove;		//是否开始移动

	RECT m_BlockRect[10];
public:
	Block();
	~Block();
	void initBlock();
	void updateBlock(DWORD PassTime);
	void drawBlock();
public:
	RECT* GetBlockRect() { return m_BlockRect; }
	//设置是否开始移动
	void SetBlockIsMove(bool isMove) { m_isMove = isMove; }
	//可另行开放接口
	//......
};

