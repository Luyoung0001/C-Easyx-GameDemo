#include"stafx.h"
int main()
{
	initgraph(400, 300);

	Manage manage;	//���ö���
	DWORD begin, end, passTime;
	begin = GetTickCount();
	manage.initGame();
	manage.startUI();
	while (1)
	{
		end = GetTickCount();
		passTime = end - begin;
		begin = end;
		manage.drawGame();
		manage.updateGame(passTime);
	}

	//Background back;	//���ö���
	//DWORD begin, end, passTime;
	//begin = GetTickCount();
	//back.initBackground();
	//back.SetBkIsMove(true);
	//while (1)
	//{
	//	end = GetTickCount();
	//	passTime = end - begin;
	//	begin = end;
	//	back.drawBackground();
	//	back.updateBackground(passTime);
	//}

	//Bird bird;	//���ö���
	//DWORD begin, end, passTime;
	//begin = GetTickCount();
	//bird.initBird();
	//bird.SetBirdIsStart(true);
	//while (1)
	//{
	//	end = GetTickCount();
	//	passTime = end - begin;
	//	begin = end;
	//	bird.drawBird();
	//	bird.updateBird(passTime);
	//}

	//Block block;	//���ö���
	//DWORD begin, end, passTime;
	//begin = GetTickCount();
	//block.initBlock();
	//block.SetBlockIsMove(true);
	//while (1)
	//{
	//	end = GetTickCount();
	//	passTime = end - begin;
	//	begin = end;
	//	block.drawBlock();
	//	block.updateBlock(passTime);
	//}
	getchar();
	closegraph();
	return 0;
}
