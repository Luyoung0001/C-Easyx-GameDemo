#pragma once
class Bird
{
	IMAGE m_bird[4][2];
	int m_index;
	int m_x, m_y;		//С���״̬ ƽ �� ƽ ��

	float m_Dps;		//֡ʱ��
	float m_PassTime;	//������ʱ��
	bool m_isMove;		//�Ƿ��ƶ�
	bool m_isStart;		//�Ƿ�ʼ���볡��
	bool m_isUp;		//�Ƿ���Ծ
	float m_JumpTimer;	//��Ծ��ʱ��

	RECT m_BirdRect;	//С��ľ�������
public:
	Bird();
	~Bird();
	void initBird();	//��ʼ��
	void drawBird();	//����ͼ��
	void updateBird(DWORD PassTime);//���ݸ���
public:
	//��ȡ�Ƿ�ʼ״̬
	bool GetBirdIsStart() { return m_isStart; }	
	//�����Ƿ�ʼ״̬
	void SetBirdIsStart(bool isStart) { m_isStart = isStart; }
	void SetBirdIsUp(bool isUp) { m_isUp = isUp; }
	//��ȡС�����ھ���
	RECT GetBirdRect() { return m_BirdRect; }
};

