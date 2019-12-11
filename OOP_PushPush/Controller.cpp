#include "Controller.h"
#include <iostream>
#include <conio.h>
#include "KeyCode.h"
#include "Consola.h"
#include "View.h"
#include "Model.h"
#include <stack>
#include <Windows.h>
#include <string>
#include <mmsystem.h>

#pragma comment(lib,"winmm.lib")

using namespace std;

Controller::Controller()
{
}
Controller::~Controller()
{
}


void Controller::back() {	
		while (!Model::mapStack.empty()) Model::mapStack.pop(); //stack�ʱ�ȭ
		Model::pushCount = 0;
		Model::popCount = 0;
		Model::isSave = false;
		for (int i = 0; i < 3; i++) {
			Model::isStage1[i] = false;
		}
		for (int i = 0; i < 3; i++) {
			Model::isStage2[i] = false;
		}
		for (int i = 0; i < 2; i++) {
			Model::isStage3[i] = false;
		}
		Model::saveMap[ROW][COLS] = {}; //save �� �������ʱ�ȭ
		Model::currentMenu = 1;
		Model::screenClean();
		View::mainScreen();
		Model::isGameSelected = false; //����ȭ�鿡����
		Model::isMenuSelected = false; // �޴�ȭ�鿡���� �ڵ��ư��� (esc) �ϸ� �Ѵ� �ʱ�ȭ�������� �׳�.
		
}

void Controller::resetMap() {
	while (!Model::mapStack.empty()) Model::mapStack.pop(); //stack�ʱ�ȭ   rollback�� ���õ� data �ʱ�ȭ
	Model::pushCount = 0;
	Model::popCount = 0;
	Model::isSave = false;

	for (int i = 0;i<3;i++) {
		Model::isStage1[i] = false;
	}

	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			Model::map[Model::stage][i][j] = Model::registMap[Model::stage][i][j];	//�ʱ���� repush_map������ �����س��Ҵٰ� �ٽ� ���� �ʿ� ���� ���� ����
		}

	}
	Model::count = 0; //�� ���½�Ű�鼭.count�� �ʱ�ȭ
}

void Controller::menuList() {
	switch (Model::getKey())
	{
	case UPKEY: View::moveMenu(UPKEY); break;
	case DOWNKEY: View::moveMenu(DOWNKEY); break;
	case ENTERKEY: selectMenu(); break;
	}
}

int Controller::selectMenu() {

	if (Model::currentMenu == 1) {
		Model::screenClean();
		Model::isGameSelected = true;
	}
	else if (Model::currentMenu == 2) { // �ʼ���
		Model::isMenuSelected = true;
		Model::screenClean();
		Model::selectMapScreen();

	}
	else if (Model::currentMenu == 3) {
		Model::isMenuSelected = true;

		View::adviceView();
	}
	else if (Model::currentMenu == 4) { //��ŷ����
		Model::isMenuSelected = true;

		View::rankingView(); //��ŷ���� ȭ�� ����Լ�
	}
	else if (Model::currentMenu == 5) {
		Model::isEndFlag = true;
		Model::writeRanking();
		return 0;
	}
}

enum object { EMPTY=0, WALL = 1 , PLAYER = 2, BOX = 3, HOME = 4, BOX_IN_HOME=5 , PLAYER_IN_HOME = 6 }; //�����Ҷ� Ȱ������ ������ ���ؼ�.
//�����̸� -1 -2  �������̸� +1 +2 <- x  �����̸� -1 -2  �Ʒ����̸� +1 +2  <- y 
void Controller::Left_Right_Key(int key) {
	int prev_1, prev_2; //�÷��̾�κ��� ù��ĭ ��°ĭ����.


	switch (key) {
	case LEFTKEY: prev_1 = -1; prev_2 = -2; break;
	case RIGHTKEY: prev_1 = 1; prev_2 = 2; break;
	}

	if (Model::map[Model::stage][Model::playerY][Model::playerX] == PLAYER) // �� ��ο����� �÷��̾��� ���.
	{
		if (Model::map[Model::stage][Model::playerY][Model::playerX+prev_1] == EMPTY) //������ ����ִ�
		{
			Model::map[Model::stage][Model::playerY][Model::playerX + prev_1] = PLAYER;  //�ΰ��� ���� �ٲ��ش� ��θ� �÷��̾�� �÷��̾� �ڸ��� ��η�.
			Model::map[Model::stage][Model::playerY][Model::playerX] = EMPTY;
			
			Model::count++;
		}
		else if (Model::map[Model::stage][Model::playerY][Model::playerX + prev_1] == WALL) { //������ ���̴�
			return;
		}
		else if (Model::map[Model::stage][Model::playerY][Model::playerX + prev_1] == BOX )//������ �ڽ��϶�
		{
			if (Model::map[Model::stage][Model::playerY][Model::playerX + prev_2] == WALL) {// �ڽ��� ������ ��
				return;
			}
			if (Model::map[Model::stage][Model::playerY][Model::playerX + prev_2] == BOX) { // �ڽ� ������ ��
				return;
			}
			if (Model::map[Model::stage][Model::playerY][Model::playerX + prev_2] == HOME ) //	�ڽ� ������ ��
			{
				Model::map[Model::stage][Model::playerY][Model::playerX + prev_2] = BOX_IN_HOME; // ���ȿ� �ڽ��� ����
				Model::map[Model::stage][Model::playerY][Model::playerX + prev_1] = PLAYER; //�ڽ��ڸ��� �÷��̾�
				Model::map[Model::stage][Model::playerY][Model::playerX] = EMPTY; // �÷��̾� �ڸ��� �����
				Model::count++;
			}
			else if (Model::map[Model::stage][Model::playerY][Model::playerX + prev_2] == EMPTY)	//	�ڽ� ������ �����
			{
				Model::map[Model::stage][Model::playerY][Model::playerX + prev_2] = BOX; //������� �ڽ����� 
				Model::map[Model::stage][Model::playerY][Model::playerX + prev_1] = PLAYER; // �ڽ� �ڸ��� �÷��̾ ��ġ�ϰ�
				Model::map[Model::stage][Model::playerY][Model::playerX] = EMPTY; // r�÷��̾��ڸ��� ������� �����ϰ� �ȴ�.
				Model::count++;
			}
		} //�÷��̾� ������ �ڽ��϶�

		else if (Model::map[Model::stage][Model::playerY][Model::playerX + prev_1] == HOME)//	�÷��̾� ������ ���ϋ�.
		{
			Model::map[Model::stage][Model::playerY][Model::playerX + prev_1] = PLAYER_IN_HOME;
			Model::map[Model::stage][Model::playerY][Model::playerX] = EMPTY;
			Model::count++;
		}
		else if (Model::map[Model::stage][Model::playerY][Model::playerX + prev_1] ==BOX_IN_HOME)//		���ʿ� �ڽ��� ����ִ� ���϶�
		{
			if (Model::map[Model::stage][Model::playerY][Model::playerX + prev_2] == WALL
				|| Model::map[Model::stage][Model::playerY][Model::playerX + prev_2] == BOX
				|| Model::map[Model::stage][Model::playerY][Model::playerX + prev_2] == BOX_IN_HOME) {//	������ ��, �ڽ�  , �ڽ��� �� ���϶�.
				return;
			}
			else if (Model::map[Model::stage][Model::playerY][Model::playerX + prev_2] == EMPTY)//		���� ����ִ� �� ������ ������϶�
			{
				Model::map[Model::stage][Model::playerY][Model::playerX + prev_2] = BOX;  //�������� �ڽ� �̵�, 
				Model::map[Model::stage][Model::playerY][Model::playerX + prev_1] = PLAYER_IN_HOME; //�÷��̾ ������ ��.
				Model::map[Model::stage][Model::playerY][Model::playerX] = EMPTY; //�÷��̾��ڸ��� ��������� .
				Model::count++;

			}
			else if (Model::map[Model::stage][Model::playerY][Model::playerX + prev_2] == HOME)//		���� ����ִ� �� ������ ����ִ� ���϶�
			{
				Model::map[Model::stage][Model::playerY][Model::playerX + prev_2] = BOX_IN_HOME;
				Model::map[Model::stage][Model::playerY][Model::playerX + prev_1] = PLAYER_IN_HOME;
				Model::map[Model::stage][Model::playerY][Model::playerX] = EMPTY;
				Model::count++;
			}
		}

	}
	////////////////////////////// �� �ȿ����� �÷��̾�.
	else if (Model::map[Model::stage][Model::playerY][Model::playerX] == PLAYER_IN_HOME)
	{
		if (Model::map[Model::stage][Model::playerY][Model::playerX + prev_1] == WALL)//��
			return;
		else if (Model::map[Model::stage][Model::playerY][Model::playerX + prev_1] == EMPTY)//�����
		{
			Model::map[Model::stage][Model::playerY][Model::playerX + prev_1] = PLAYER;
			Model::map[Model::stage][Model::playerY][Model::playerX] =HOME;
			Model::count++;
		}
		else if (Model::map[Model::stage][Model::playerY][Model::playerX + prev_1] == BOX)// ������ �ڽ�
		{
			if (Model::map[Model::stage][Model::playerY][Model::playerX + prev_2] == WALL
				|| Model::map[Model::stage][Model::playerY][Model::playerX + prev_2] == BOX
					|| Model::map[Model::stage][Model::playerY][Model::playerX + prev_2] == BOX_IN_HOME) {//	�ڽ����� ���̰ų� �ڽ��ų� �ڽ��� �� ���̰ų�.
				return;
			}
			else if (Model::map[Model::stage][Model::playerY][Model::playerX + prev_2] == EMPTY)//		�� ������ ���̰� �� ������ ������϶� 
			{
				Model::map[Model::stage][Model::playerY][Model::playerX + prev_2] = BOX;
				Model::map[Model::stage][Model::playerY][Model::playerX + prev_1] = PLAYER;
				Model::map[Model::stage][Model::playerY][Model::playerX] = HOME;
				Model::count++;
			}
			else if (Model::map[Model::stage][Model::playerY][Model::playerX + prev_2] == HOME)//	 ������ ����ִ� ���϶�
			{
				Model::map[Model::stage][Model::playerY][Model::playerX + prev_2] =BOX_IN_HOME;
				Model::map[Model::stage][Model::playerY][Model::playerX + prev_1] = PLAYER;
				Model::map[Model::stage][Model::playerY][Model::playerX] = HOME;
				Model::count++;
			}
		} /////������ �ڽ��ΰ��
		else if (Model::map[Model::stage][Model::playerY][Model::playerX + prev_1] == HOME)//��
		{
			Model::map[Model::stage][Model::playerY][Model::playerX + prev_1] = PLAYER_IN_HOME;
			Model::map[Model::stage][Model::playerY][Model::playerX] = HOME;
			Model::count++;
		}
		else if (Model::map[Model::stage][Model::playerY][Model::playerX + prev_1] == BOX_IN_HOME)//������ �ڽ��� ����ִ� ���϶�
		{
			if (Model::map[Model::stage][Model::playerY][Model::playerX + prev_2] == EMPTY)//�� ������ ����� 
			{
				Model::map[Model::stage][Model::playerY][Model::playerX + prev_2] = BOX;
				Model::map[Model::stage][Model::playerY][Model::playerX +prev_1] = PLAYER_IN_HOME;
				Model::map[Model::stage][Model::playerY][Model::playerX] = HOME;
				Model::count++;
			}
			else if (Model::map[Model::stage][Model::playerY][Model::playerX + prev_2] == WALL
				|| Model::map[Model::stage][Model::playerY][Model::playerX + prev_2] == BOX
					|| Model::map[Model::stage][Model::playerY][Model::playerX + prev_2] == BOX_IN_HOME)// �� ������ ��, �ڽ�, �ڽ��� ����ִ���
				return;
			else if (Model::map[Model::stage][Model::playerY][Model::playerX + prev_2] == HOME)// �� ������ ����ִ� ��
			{
				Model::map[Model::stage][Model::playerY][Model::playerX + prev_2] = BOX_IN_HOME;
				Model::map[Model::stage][Model::playerY][Model::playerX + prev_1] = PLAYER_IN_HOME;
				Model::map[Model::stage][Model::playerY][Model::playerX] = HOME;
				Model::count++;
			}
		}//������ �ڽ��� �� ���϶�.
	} //���ȿ����� �÷��̾�

	switch (key) {
	case LEFTKEY: Model::playerX--; break;
	case RIGHTKEY: Model::playerX++; break;
	}
	
	

	return;
}


void Controller::Up_Down_Key(int key) {
	int prev_1, prev_2;


	switch (key) {
	case UPKEY: prev_1 = -1; prev_2 = -2; break;
	case DOWNKEY: prev_1 = 1; prev_2 = 2; break;
	}

	if (Model::map[Model::stage][Model::playerY][Model::playerX] == PLAYER) // �� ��ο����� �÷��̾��� ���.
	{
		if (Model::map[Model::stage][Model::playerY+prev_1][Model::playerX ] == EMPTY)
		{
			Model::map[Model::stage][Model::playerY + prev_1][Model::playerX ] = PLAYER;  //�ΰ��� ���� �ٲ��ش� ��θ� �÷��̾�� �÷��̾� �ڸ��� ��η�.
			Model::map[Model::stage][Model::playerY][Model::playerX] = EMPTY;

			Model::count++;
		}
		else if (Model::map[Model::stage][Model::playerY + prev_1][Model::playerX ] == WALL) {
			return;
		}
		else if (Model::map[Model::stage][Model::playerY + prev_1][Model::playerX ] == BOX)
		{
			if (Model::map[Model::stage][Model::playerY + prev_2][Model::playerX ] == WALL) {
				return;
			}
			if (Model::map[Model::stage][Model::playerY + prev_2][Model::playerX ] == BOX) {
				return;
			}
			if (Model::map[Model::stage][Model::playerY + prev_2][Model::playerX ] == HOME)
			{
				Model::map[Model::stage][Model::playerY + prev_2][Model::playerX ] = BOX_IN_HOME; // ���ȿ� �ڽ��� ����
				Model::map[Model::stage][Model::playerY + prev_1][Model::playerX] = PLAYER; //�ڽ��ڸ��� �÷��̾�
				Model::map[Model::stage][Model::playerY][Model::playerX] = EMPTY; // �÷��̾� �ڸ��� �����
				Model::count++;
			}
			else if (Model::map[Model::stage][Model::playerY + prev_2][Model::playerX ] == EMPTY)
			{
				Model::map[Model::stage][Model::playerY + prev_2][Model::playerX ] = BOX; //������� �ڽ����� 
				Model::map[Model::stage][Model::playerY + prev_1][Model::playerX] = PLAYER; // �ڽ� �ڸ��� �÷��̾ ��ġ�ϰ�
				Model::map[Model::stage][Model::playerY][Model::playerX] = EMPTY; // r�÷��̾��ڸ��� ������� �����ϰ� �ȴ�.
				Model::count++;
			}
		} //�÷��̾� ������ �ڽ��϶�

		else if (Model::map[Model::stage][Model::playerY + prev_1][Model::playerX ] == HOME)
		{
			Model::map[Model::stage][Model::playerY + prev_1][Model::playerX ] = PLAYER_IN_HOME;
			Model::map[Model::stage][Model::playerY][Model::playerX] = EMPTY;
			Model::count++;
		}
		else if (Model::map[Model::stage][Model::playerY + prev_1][Model::playerX ] == BOX_IN_HOME)
		{
			if (Model::map[Model::stage][Model::playerY + prev_2][Model::playerX ] == WALL
				|| Model::map[Model::stage][Model::playerY + prev_2][Model::playerX] == BOX
				|| Model::map[Model::stage][Model::playerY + prev_2][Model::playerX ] == BOX_IN_HOME) {
				return;
			}
			else if (Model::map[Model::stage][Model::playerY + prev_2][Model::playerX ] == EMPTY)
			{
				Model::map[Model::stage][Model::playerY + prev_2][Model::playerX ] = BOX;
				Model::map[Model::stage][Model::playerY + prev_1][Model::playerX ] = PLAYER_IN_HOME; //�÷��̾ ������ ��.
				Model::map[Model::stage][Model::playerY][Model::playerX] = EMPTY; //�÷��̾��ڸ��� ��������� .
				Model::count++;

			}
			else if (Model::map[Model::stage][Model::playerY + prev_2][Model::playerX ] == HOME)
			{
				Model::map[Model::stage][Model::playerY + prev_2][Model::playerX ] = BOX_IN_HOME;
				Model::map[Model::stage][Model::playerY + prev_1][Model::playerX ] = PLAYER_IN_HOME;
				Model::map[Model::stage][Model::playerY][Model::playerX] = EMPTY;
				Model::count++;
			}
		}

	}
	////////////////////////////// �� �ȿ����� �÷��̾�.
	else if (Model::map[Model::stage][Model::playerY][Model::playerX] == PLAYER_IN_HOME)
	{
		if (Model::map[Model::stage][Model::playerY + prev_1][Model::playerX ] == WALL)//��
			return;
		else if (Model::map[Model::stage][Model::playerY + prev_1][Model::playerX ] == EMPTY)//�����
		{
			Model::map[Model::stage][Model::playerY + prev_1][Model::playerX ] = PLAYER;
			Model::map[Model::stage][Model::playerY][Model::playerX] = HOME;
			Model::count++;
		}
		else if (Model::map[Model::stage][Model::playerY + prev_1][Model::playerX] == BOX)
		{
			if (Model::map[Model::stage][Model::playerY + prev_2][Model::playerX ] == WALL
				|| Model::map[Model::stage][Model::playerY + prev_2][Model::playerX ] == BOX
				|| Model::map[Model::stage][Model::playerY + prev_2][Model::playerX ] == BOX_IN_HOME) {//	�ڽ����� ���̰ų� �ڽ��ų� �ڽ��� �� ���̰ų�.
				return;
			}
			else if (Model::map[Model::stage][Model::playerY + prev_2][Model::playerX] == EMPTY)
			{
				Model::map[Model::stage][Model::playerY + prev_2][Model::playerX ] = BOX;
				Model::map[Model::stage][Model::playerY + prev_1][Model::playerX ] = PLAYER;
				Model::map[Model::stage][Model::playerY][Model::playerX] = HOME;
				Model::count++;
			}
			else if (Model::map[Model::stage][Model::playerY + prev_2][Model::playerX ] == HOME)
			{
				Model::map[Model::stage][Model::playerY + prev_2][Model::playerX ] = BOX_IN_HOME;
				Model::map[Model::stage][Model::playerY + prev_1][Model::playerX ] = PLAYER;
				Model::map[Model::stage][Model::playerY][Model::playerX] = HOME;
				Model::count++;
			}
		} /////������ �ڽ��ΰ��
		else if (Model::map[Model::stage][Model::playerY + prev_1][Model::playerX ] == HOME)//��
		{
			Model::map[Model::stage][Model::playerY + prev_1][Model::playerX ] = PLAYER_IN_HOME;
			Model::map[Model::stage][Model::playerY][Model::playerX] = HOME;
			Model::count++;
		}
		else if (Model::map[Model::stage][Model::playerY + prev_1][Model::playerX ] == BOX_IN_HOME)
		{
			if (Model::map[Model::stage][Model::playerY + prev_2][Model::playerX ] == EMPTY)
			{
				Model::map[Model::stage][Model::playerY + prev_2][Model::playerX ] = BOX;
				Model::map[Model::stage][Model::playerY + prev_1][Model::playerX ] = PLAYER_IN_HOME;
				Model::map[Model::stage][Model::playerY][Model::playerX] = HOME;
				Model::count++;
			}
			else if (Model::map[Model::stage][Model::playerY + prev_2][Model::playerX ] == WALL
				|| Model::map[Model::stage][Model::playerY + prev_2][Model::playerX ] == BOX
				|| Model::map[Model::stage][Model::playerY + prev_2][Model::playerX ] == BOX_IN_HOME)
				return;
			else if (Model::map[Model::stage][Model::playerY + prev_2][Model::playerX ] == HOME)
			{
				Model::map[Model::stage][Model::playerY + prev_2][Model::playerX ] = BOX_IN_HOME;
				Model::map[Model::stage][Model::playerY + prev_1][Model::playerX ] = PLAYER_IN_HOME;
				Model::map[Model::stage][Model::playerY][Model::playerX] = HOME;
				Model::count++;
			}
		}
	} //���ȿ����� �÷��̾�

	switch (key) {
	case UPKEY: Model::playerY--; break;
	case DOWNKEY: Model::playerY++; break;
	}
	
	return;
}




int Controller::playGame() {
	Model::readRanking();	//��ŷ�� ����ϴ� �Լ� ȣ��
	PlaySound(TEXT("bgm2.wav"), NULL, SND_FILENAME |SND_ASYNC | SND_LOOP); //bgm ��� �ݺ����
	_setcursortype(_NOCURSOR);	//Ŀ�� ����

	View::mainScreen();	//�޴� ��� �Լ� ȣ��

	while (!Model::isEndFlag) {
		if (!Model::isGameSelected) { //����ȭ��
			if (!Model::isMenuSelected) {
				menuList();
			}//inner if
		}//if
		else if (Model::isGameSelected) { //����ȭ��
				Model::clearCondition(); //������ Ŭ���� ����� Ȯ���ϰ�
				Model::readRanking(); //��ŷ ���� �ҷ��ͼ� �̵� ���̵� �κп� �ⷫ
				if (!Model::isGameClear) {//���� ������ �������̶��.
					Model::playerLocation();//���� �÷��̾��� ��ġ ����ϱ�.
					View::printMap(); //���� stage �� ���
					View::printGuide(); // ���̵� ���

				
					switch (Model::getKey())	//���� Ű�� �Է¹޾��� ��
					{
					case LEFTKEY: Model::pushChangedMap(); Left_Right_Key(LEFTKEY); break;
					case RIGHTKEY: Model::pushChangedMap(); Left_Right_Key(RIGHTKEY); break;
					case DOWNKEY:  Model::pushChangedMap(); Up_Down_Key(DOWNKEY);  break;
					case UPKEY:	  Model::pushChangedMap(); Up_Down_Key(UPKEY);  break;
					case PREVKEY: return 0;
					case NEXTKEY: View::nextMap(); break;
					case BACKKEY: View::previousMap(); break;
					case SAVEKEY: Model::save(); break;
					case LOADKEY: if(Model::isSave) Model::load(); break;
					case ROLLBACKKEY: Model::popChangedMap();  break;
					case RESETKEY: Controller::resetMap(); break;	//RŰ�� �Է¹����� �� �ʱ�ȭ
					case ESCKEY: Controller::back(); break;
					}
				}//���� Ŭ����ƴ϶�� ��� Ű �Է¹ޱ�
				else if (Model::isGameClear) { //����Ŭ������
					Model::isGameClear = false;
					Model::screenClean(); //�� CLEAN
					
					gotoxy(5, 5); cout << "stage clear   ";
					gotoxy(5, 7); cout << "�ƹ�Ű�� �Է��ϸ� ����ȭ�� " << endl;

					if (Model::getKey())	//Ű�� �Է� ������
					{
						View::mainScreen();
					}//getKey
				}
		}//else if
	}//while
	
	return 0;
}