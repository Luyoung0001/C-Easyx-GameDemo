#pragma once
class Block
{
	IMAGE m_block;	

	float m_PassTime;	//������ʱ��
	float m_Dps;		//֡ʱ��
	int m_iMoveSpeed;	//�ƶ��ٶ�
	bool m_isMove;		//�Ƿ�ʼ�ƶ�

	RECT m_BlockRect[10];
public:
	Block();
	~Block();
	void initBlock();
	void updateBlock(DWORD PassTime);
	void drawBlock();
public:
	RECT* GetBlockRect() { return m_BlockRect; }
	//�����Ƿ�ʼ�ƶ�
	void SetBlockIsMove(bool isMove) { m_isMove = isMove; }
	//�����п��Žӿ�
	//......
};

