#pragma once
class Background
{
	IMAGE m_bk;		//����ͼƬ
	DWORD m_PassTime;	//������ʱ��
	DWORD m_Dsp;		//֡ʱ��
	int m_iMoveSpeed;	//�ƶ��ٶ�
	bool m_isMove;		//�Ƿ��ƶ�

	int m_x, m_y;	//���Ƶ�λ��
public:
	Background();
	~Background();
	void initBackground();	//��ʼ��
	void updateBackground(DWORD PassTime);	//���ݸ���
	void drawBackground();					//����ͼ��
public:
	//�����Ƿ�ʼ�ƶ�
	void SetBkIsMove(bool isMove) { m_isMove = isMove; }
};

