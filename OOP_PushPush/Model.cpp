#include "Model.h"
#include "Consola.h"
#include <iostream>
#include "keyCode.h"
#include <fstream>
#include <conio.h>
#include <stack>
#include "View.h"
#include <string>
#include <windows.h>

using namespace std;

int Model::currentStage = 0; // 0+1 1+1  2+1  stage
int Model::currentMenu = 1; //1 ~5 ���ӽ��� �ʼ��� ���� ��ŷ���� ��������
bool Model::isGameSelected = false; //true��� ������ ���õ�
bool Model::isMenuSelected = false; //true��� �޴��� ���õ�
int Model::currentMap = 0; //1~3 stage1 stage2 stage3
bool Model::isMapSelected = false; //true�϶� ���õ� ���� ���.
bool Model::isEndFlag = false; //�����ư�� enter�϶� true�� ����.
bool Model::isGameClear = false;
int Model::count = 0; //�÷��̾��� �̵�Ƚ��
int Model::stage = 0;//��������.
int Model::boxCount[3] = {16,19,13}; // ���������� box Count
int Model::boxCount_inHome = 0; //Ȩ�� ���ִ� �ڽ��� ����, �ʱⰪ 0
int Model::playerX = 0; //�÷��̾� x ��ǥ �ʱ�ȭ
int Model::playerY = 0; //  "" �ʱ�ȭ
int Model::rollbackChangeMap[ROW][COLS] = {};
int Model::rollbackChangeMap_2[ROW][COLS] = {};
int Model::rollbackChangeMap_3[ROW][COLS] = {};
int Model::rollbackChangeMap_4[ROW][COLS] = {};
int Model::rollbackChangeMap_5[ROW][COLS] = {};
int Model::saveMap[ROW][COLS] = {};
int Model::stage1_bestCount = 0;
int Model::stage2_bestCount = 0;
int Model::stage3_bestCount = 0;
string Model::stage1_bestPlayer = "";
string Model::stage2_bestPlayer = "";
string Model::stage3_bestPlayer = "";
stack<int(*)[ROW]> Model::mapStack;
int Model::pushCount = 0; //0c�ʱ�ȭ
int Model::popCount = 0;
bool Model::isSave = false;
bool Model::isStage1[3] = { false,false, false };
bool Model::isStage2[3] = { false,false, false };
bool Model::isStage3[2] = { false,false};

int Model::getKey() {
	Beep(1500,5);
	int key = (_getch() & 0x00ff);

	return (key == 0xe0) ? ((key << 8) | _getch()) : key;
}


char Model::character[][COLS]= { "  ", "��","��","��","��","��","��","��" }; //�� ���� 7
  // �� �ڽ�    ������   �� ���� �� �� ��  �� �÷��̾� �� ������ �÷��̾�
int Model::map[3][ROW][COLS] =		//���� ��Ÿ���ִ� 3���� �迭
{
	// 1 , 1 wall  ;  2  player  ;  4  home  ;  3  box
	{

		{0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0, 0,0,0},
		{1,1,1,1,1,1,1,1,  1,1,1,1,1,1,1,1,1, 0,0,0},
		{1,1,1,4,1,0,0,0,  0,0,0,0,1,1,1,1,1, 0,0,0},//3,5
		{1,1,1,0,1,1,1,1,  1,0,3,3,1,1,1,1,1, 0,0,0},
		{1,1,1,3,0,3,4,1,  1,0,3,0,1,0,1,4,1, 0,0,0},
		{1,4,0,3,2,1,1,1,  1,1,1,0,1,1,1,4,1, 0,0,0},
		{1,1,1,1,3,1,1,1,  1,1,1,0,0,0,0,4,1, 0,0,0},
		{1,1,1,1,4,1,1,1,  1,1,0,0,0,1,0,0,1, 0,0,0},
		{1,1,1,1,1,1,1,1,  1,1,0,0,0,1,1,1,1, 0,0,0},
		{0,0,0,0,0,0,0,0,  1,1,1,1,1,1,1,1,1, 0,0,0},   // 9,11
		{0,0,0,0,0,0,0,0,  1,1,1,0,1,1,1,1,1, 0,0,0},//10

		{1,1,1,1,1,1,1,1,1,1,  1,0,1,1,1,1, 1,0,0,0},
		{1,1,0,0,0,0,0,1,1,1,  1,0,0,0,1,1, 1,0,0,0},
		{1,1,3,1,1,1,0,0,0,0,  1,0,3,0,1,1, 1,0,0,0}, //13,10
		{1,0,0,0,3,0,0,3,0,1,  1,1,3,0,1,1, 1,0,0,0},
		{1,0,4,4,1,0,3,0,1,1,  1,1,0,3,0,1, 1,0,0,0},
		{1,1,4,4,1,0,0,0,1,0,  1,4,3,0,0,1, 1,0,0,0},
		{1,1,1,1,1,1,1,1,1,0,  1,4,4,5,4,1, 1,0,0,0},//8
		{1,1,1,1,1,1,1,1,1,1,  1,1,1,1,1,1, 1,0,0,0},

		{0,0,0,0,0,  0,0,0,0,0,  0,0,0,0,0, 0,0,0,0,0}
	}, // stage 1
	{
		{0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,  0,0},
		{0,1,1,1,1,1,0,0,  0,0,0,1,1,1,1,1,1,1,  1,1},
		{0,1,2,0,1,1,1,0,  0,0,1,1,0,0,1,0,0,1,  1,1},
		{0,1,0,3,0,0,1,0,  0,0,1,0,0,0,1,0,0,1,  1,1},
		{1,1,1,0,1,0,1,1,  0,0,1,3,0,3,0,3,0,1,  1,1},
		{1,4,1,0,1,0,0,1,  0,0,1,0,3,1,1,0,0,1,  1,1},
		{1,4,3,0,0,1,0,1,  1,1,1,0,3,0,1,0,1,1,  1,1},
		{1,4,0,0,0,3,0,1,  1,4,4,4,4,4,0,0,1,1,  1,1},
		{1,1,1,1,1,1,1,1,  1,1,1,1,1,1,1,0,1,1,  1,1},//8,4 -> 1

		{0,0,0,1,0, 1,0,0,0,0, 0,0,0,0,1, 0,1,0,0,0},

		{0,0,0,1,0,1,1,1,1,0,  1,1,1,1,1,1,1,1,1,0}, //10,15 ->3
		{0,1,1,1,0,0,0,0,1,0,  1,0,0,1,1,0,0,0,1,0},
		{1,1,4,0,3,1,1,0,1,1,  1,0,0,0,3,0,0,0,1,0},
		{1,4,4,3,0,3,0,0,0,1,  1,3,0,1,1,1,0,3,1,0},
		{1,4,4,0,3,0,3,0,1,1,  1,0,1,4,4,4,1,0,1,0},
		{1,1,1,1,1,1,0,0,1,1,  1,0,1,4,4,4,1,0,1,1},
		{0,0,0,0,0,1,1,0,1,1,  0,3,0,0,3,0,0,3,0,1},
		{0,0,0,0,0,0,1,0,1,0,  0,0,0,0,0,1,0,0,0,1}, //17,8 -> 2
		{0,0,0,0,0,0,1,1,1,1,  1,1,1,1,1,1,1,1,1,1},//9

		{0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0}
	}, // stage 2
	{
		{0,0,0,0,0,  0,0,0,0,0,  0,0,0,0,0, 0,0,0,0,0},
		{0,0,1,1,1,1,1,1,  1,1,1,1,1,0,0,1,1,1,1,1},
		{0,0,1,0,0,0,0,1,  1,1,0,0,1,0,0,1,0,0,0,1},
		{1,1,1,3,3,3,0,1,  1,0,3,0,1,1,1,1,3,0,0,1},
		{1,2,0,3,4,4,0,1,  1,0,0,3,4,4,4,4,0,3,0,1},// 4, 78  ->1
		{1,0,3,4,4,4,1,1,  1,1,0,0,0,0,1,0,0,0,1,1},
		{1,1,1,1,0,0,1,0,  0,1,1,0,0,1,1,1,1,1,1,0},
		{0,0,0,1,1,1,1,0,  0,0,1,0,0,1,0,0,0,0,0,0},

		{0,0,0,0,0,  0,0,0,0,0,  1,0,1,0,0, 0,0,0,0,0},

		{0,0,0,0,0,  0,0,0,1,1,  1,1,1,0,0, 0,0,0,0,0}, // 9 11
		{0,0,0,0,0,  0,1,1,1,0,  0,0,1,0,0, 0,0,0,0,0},
		{0,0,0,0,0,  0,1,0,0,3,  4,0,1,1,0, 0,0,0,0,0},
		{0,0,0,0,0,  0,1,0,0,4,  3,4,0,1,0, 0,0,0,0,0},
		{0,0,0,0,0,  0,1,1,1,0,  5,3,0,1,0, 0,0,0,0,0},
		{0,0,0,0,0,  0,0,0,1,0,  0,0,1,1,0, 0,0,0,0,0},
		{0,0,0,0,0,  0,0,0,1,1,  1,1,1,0,0, 0,0,0,0,0},

		{0,0,0,0,0,  0,0,0,0,0,  0,0,0,0,0, 0,0,0,0,0,},
		{0,0,0,0,0,  0,0,0,0,0,  0,0,0,0,0, 0,0,0,0,0,},
		{0,0,0,0,0,  0,0,0,0,0,  0,0,0,0,0, 0,0,0,0,0,},
		{0,0,0,0,0,  0,0,0,0,0,  0,0,0,0,0, 0,0,0,0,0,}
	}// stage 3

};
int Model::registMap[3][ROW][COLS] =	//stage���� ���� �ʱⰪ�� �����ϴ� ����
{
	{

		{0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0, 0,0,0},
		{1,1,1,1,1,1,1,1,  1,1,1,1,1,1,1,1,1, 0,0,0},
		{1,1,1,4,1,0,0,0,  0,0,0,0,1,1,1,1,1, 0,0,0},//3,5
		{1,1,1,0,1,1,1,1,  1,0,3,3,1,1,1,1,1, 0,0,0},
		{1,1,1,3,0,3,4,1,  1,0,3,0,1,0,1,4,1, 0,0,0},
		{1,4,0,3,2,1,1,1,  1,1,1,0,1,1,1,4,1, 0,0,0},
		{1,1,1,1,3,1,1,1,  1,1,1,0,0,0,0,4,1, 0,0,0},
		{1,1,1,1,4,1,1,1,  1,1,0,0,0,1,0,0,1, 0,0,0},
		{1,1,1,1,1,1,1,1,  1,1,0,0,0,1,1,1,1, 0,0,0},
		{0,0,0,0,0,0,0,0,  1,1,1,1,1,1,1,1,1, 0,0,0},   // 9,11
		{0,0,0,0,0,0,0,0,  1,1,1,0,1,1,1,1,1, 0,0,0},//10

		{1,1,1,1,1,1,1,1,1,1,  1,0,1,1,1,1, 1,0,0,0},
		{1,1,0,0,0,0,0,1,1,1,  1,0,0,0,1,1, 1,0,0,0},
		{1,1,3,1,1,1,0,0,0,0,  1,0,3,0,1,1, 1,0,0,0}, //13,10
		{1,0,0,0,3,0,0,3,0,1,  1,1,3,0,1,1, 1,0,0,0},
		{1,0,4,4,1,0,3,0,1,1,  1,1,0,3,0,1, 1,0,0,0},
		{1,1,4,4,1,0,0,0,1,0,  1,4,3,0,0,1, 1,0,0,0},
		{1,1,1,1,1,1,1,1,1,0,  1,4,4,5,4,1, 1,0,0,0},//8
		{1,1,1,1,1,1,1,1,1,1,  1,1,1,1,1,1, 1,0,0,0},

		{0,0,0,0,0,  0,0,0,0,0,  0,0,0,0,0, 0,0,0,0,0}
	}, // stage 1
	{
		{0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,  0,0},
		{1,1,1,1,1,1,1,1,  0,0,1,1,1,1,1,1,1,1,  0,0},
		{1,1,2,0,1,1,1,1,  0,0,1,1,0,0,1,0,0,1,  0,0},
		{1,1,0,3,0,0,1,1,  0,0,1,0,0,0,1,0,0,1,  0,0},
		{1,1,1,0,1,0,1,1,  0,0,1,3,0,3,0,3,0,1,  0,0},
		{1,4,1,0,1,0,0,1,  0,0,1,0,3,1,1,0,0,1,  0,0},
		{1,4,3,0,0,1,0,1,  1,1,1,0,3,0,1,0,1,1,  0,0},
		{1,4,0,0,0,3,0,1,  1,4,4,4,4,4,0,0,1,1,  0,0},
		{1,1,1,1,1,1,1,1,  1,1,1,1,1,1,1,0,1,1,  0,0},//8,4 -> 1

		{0,0,0,1,0, 1,0,0,0,0, 0,0,0,0,1, 0,1,0,0,0},

		{0,0,0,1,0,1,1,1,1,0,  1,1,1,1,1,1,1,1,1,1}, //10,15 ->3
		{1,1,1,1,0,0,0,0,1,0,  1,0,0,1,1,0,0,0,1,1},
		{1,1,4,0,3,1,1,0,1,1,  1,0,0,0,3,0,0,0,1,1},
		{1,4,4,3,0,3,0,0,0,1,  1,3,0,1,1,1,0,3,1,1},
		{1,4,4,0,3,0,3,0,1,1,  1,0,1,4,4,4,1,0,1,1},
		{1,1,1,1,1,1,0,0,1,1,  1,0,1,4,4,4,1,0,1,1},
		{0,0,0,0,0,1,1,0,1,1,  0,3,0,0,3,0,0,3,0,1},
		{0,0,0,0,0,1,1,0,1,0,  0,0,0,0,0,1,0,0,0,1}, //17,8 -> 2
		{0,0,0,0,0,1,1,1,1,1,  1,1,1,1,1,1,1,1,1,1},//9

		{0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0}
	}, // stage 2
	{
		{0,0,0,0,0,  0,0,0,0,0,  0,0,0,0,0, 0,0,0,0,0},
		{0,0,1,1,1,1,1,1,  1,1,1,1,1,0,0,1,1,1,1,1},
		{0,0,1,0,0,0,0,1,  1,1,0,0,1,0,0,1,0,0,0,1},
		{1,1,1,3,3,3,0,1,  1,0,3,0,1,1,1,1,3,0,0,1},
		{1,2,0,3,4,4,0,1,  1,0,0,3,4,4,4,4,0,3,0,1},// 4, 78  ->1
		{1,0,3,4,4,4,1,1,  1,1,0,0,0,0,1,0,0,0,1,1},
		{1,1,1,1,0,0,1,0,  0,1,1,0,0,1,1,1,1,1,1,0},
		{0,0,0,1,1,1,1,0,  0,0,1,0,0,1,0,0,0,0,0,0},

		{0,0,0,0,0,  0,0,0,0,0,  1,0,1,0,0, 0,0,0,0,0},

		{0,0,0,0,0,  0,0,0,1,1,  1,1,1,0,0, 0,0,0,0,0}, // 9 11
		{0,0,0,0,0,  0,1,1,1,0,  0,0,1,0,0, 0,0,0,0,0},
		{0,0,0,0,0,  0,1,0,0,3,  4,0,1,1,0, 0,0,0,0,0},
		{0,0,0,0,0,  0,1,0,0,4,  3,4,0,1,0, 0,0,0,0,0},
		{0,0,0,0,0,  0,1,1,1,0,  5,3,0,1,0, 0,0,0,0,0},
		{0,0,0,0,0,  0,0,0,1,0,  0,0,1,1,0, 0,0,0,0,0},
		{0,0,0,0,0,  0,0,0,1,1,  1,1,1,0,0, 0,0,0,0,0},

		{0,0,0,0,0,  0,0,0,0,0,  0,0,0,0,0, 0,0,0,0,0,},
		{0,0,0,0,0,  0,0,0,0,0,  0,0,0,0,0, 0,0,0,0,0,},
		{0,0,0,0,0,  0,0,0,0,0,  0,0,0,0,0, 0,0,0,0,0,},
		{0,0,0,0,0,  0,0,0,0,0,  0,0,0,0,0, 0,0,0,0,0,}
	} // stage 3
};

void Model::pushChangedMap() { //���� ����ɋ�����. �� Ű���� �Է��� ���������� stack�� �����Ͱ� �����Ѵ�.
	int(*address)[20];

	Model::pushCount++;

	for (int i = 0;i<ROW;i++) { //�����  �� ���� ����.
		for (int j = 0;j<COLS;j++) {
			switch (Model::pushCount % 5) {
			case 1:Model::rollbackChangeMap[i][j] = Model::map[Model::stage][i][j]; break;
			case 2:Model::rollbackChangeMap_2[i][j] = Model::map[Model::stage][i][j]; break;
			case 3:Model::rollbackChangeMap_3[i][j] = Model::map[Model::stage][i][j]; break;
			case 4:Model::rollbackChangeMap_4[i][j] = Model::map[Model::stage][i][j]; break;
			case 0:Model::rollbackChangeMap_5[i][j] = Model::map[Model::stage][i][j]; break;
			}
		}//inner for
	}

	switch (Model::pushCount % 5) {
	case 1:address = Model::rollbackChangeMap; break;
	case 2:address = Model::rollbackChangeMap_2; break;
	case 3:address = Model::rollbackChangeMap_3; break;
	case 4:address = Model::rollbackChangeMap_4; break;
	case 0:address = Model::rollbackChangeMap_5; break;
	}

	Model::mapStack.push(address);
}

void Model::popChangedMap() { // �ѹ�Ű�� ������ �۵��ϰԲ�����.
	int(*address)[ROW];
	int index = 0;//�޾ƿ� �迭�� map�� �����ϱ����� �ε���  why? �����ͷ� �޾ƿ��ϱ� ������� (1�����迭ó�� �Ϸķ�)  �����Ͱ� ����Ǿ�����.)
	
	if(Model::popCount <6)
	Model::popCount++;

	if (Model::popCount<=5) {//���ư���� �� 5�������� ����.
		if (!mapStack.empty()) {
			index = 0;
			address = Model::mapStack.top();

			for (int i = 0; i < ROW; i++) {
				for (int j = 0; j < COLS; j++) {
					Model::map[Model::stage][i][j] = (*address)[index++];
				}
			}

			Model::mapStack.pop(); //�����͸� �����������, ��������.	cout << "asdsadads" << endl;
			Model::count += -1;
		}//inner if
	}//if
	
}
void Model::clearCondition() 
{
	if (Model::stage<3) { //���� ������
		Model::getBoxCountInHome();	//���� ����ִ� ���� ������ Ž���ϴ� �Լ� ȣ��
		
		if (Model::stage < 2) {
			/////////////////
			if (Model::stage == 0) {
				switch (Model::boxCount_inHome)
				{
				case 5:
					if (!Model::isStage1[0]) {
						Model::map[Model::stage][3][5] = 0;
						Model::isStage1[0] = true; 
					}
					break;
				case 8:
					if (!Model::isStage1[1]) {
						Model::map[Model::stage][9][11] = 0;
						Model::isStage1[1] = true;
					}
					break;
				case 12:
					if (!Model::isStage1[2]) {
						Model::map[Model::stage][13][10] = 0;//13 10
						Model::isStage1[2] = true;
					}
					break;
				}
			}
			else if (Model::stage == 1) {
				switch (Model::boxCount_inHome)
				{
				case 3:
					if (!Model::isStage2[0]) {
						Model::map[Model::stage][8][4] = 0;
						Model::isStage2[0] = true;
					}
					break;
				case 8:
					if (!Model::isStage2[1]) {
						Model::map[Model::stage][17][8] = 0;
						Model::isStage2[1] = true;
					}
					break;
				case 14:
					if (!Model::isStage2[2]) {
						Model::map[Model::stage][10][15] = 0;//13 10
						Model::isStage2[2] = true;
					}
					break;
				}
			}
			////////////////
			if (Model::boxCount_inHome == Model::boxCount[Model::stage])	//�ش��ϴ� ���������� ������ ������
			{
				
				Model::screenClean(); // ȭ�������ְ�
				switch (Model::stage)
				{
				case 0:
					if (Model::stage1_bestCount != 0 && Model::stage1_bestCount > Model::count) { //ó�� �ԷµǴ°� �ƴϰ�, count�� �ְ��Ϻ��� ���� ����̶��,
						Model::stage1_bestCount = Model::count;
						gotoxy(5, 7); cout << "�ְ� ��� �޼� ! , player�� �̸��� �Է����ּ��� : ";
						getline(cin,Model::stage1_bestPlayer);
					}
					else if (Model::stage1_bestCount == 0) { //ó�� ��ŷ�� ����Ǵ°Ŷ��,
						Model::stage1_bestCount = Model::count;
						gotoxy(5, 7); cout << "�ְ� ��� �޼� ! , player�� �̸��� �Է����ּ��� : ";
						getline(cin, Model::stage1_bestPlayer);
					}
					break;
				case 1:
					if (Model::stage2_bestCount != 0 && Model::stage2_bestCount > Model::count) { //ó�� �ԷµǴ°� �ƴϰ�, count�� �ְ��Ϻ��� ���� ����̶��,
						Model::stage2_bestCount = Model::count;
						gotoxy(5, 7); cout << "�ְ� ��� �޼� ! , player�� �̸��� �Է����ּ��� : ";
						getline(cin, Model::stage2_bestPlayer);
					}
					else if (Model::stage2_bestCount == 0) { //ó�� ��ŷ�� ����Ǵ°Ŷ��,
						Model::stage2_bestCount = Model::count;
						gotoxy(5, 7); cout << "�ְ� ��� �޼� ! , player�� �̸��� �Է����ּ��� : ";
						getline(cin, Model::stage2_bestPlayer);
					}break;
				}

				Model::writeRanking();//////////////��ŷ���� �����ϱ�

				while (!Model::mapStack.empty()) Model::mapStack.pop(); //stack�ʱ�ȭ
				Model::pushCount = 0;
				Model::popCount = 0;
				Model::isSave = false;

					Model::screenClean(); //�� CLEAN
					gotoxy(5, 5); cout << "�̹� stage ������ Ƚ�� :  " << Model::count << endl;
					gotoxy(5, 9); cout << "�ƹ�Ű�� �Է��ϸ� ���� �ܰ� START " << endl;
					if (getKey())	//Ű�� �Է� ������
					{
						Model::screenClean();
						View::nextMap();
					}//getKey
			}//if
		}
		else if (Model::stage == 2) { // ������ �ܰ���

			switch (Model::boxCount_inHome)
			{
			case 6:
				if (!Model::isStage3[0]) {
					Model::map[Model::stage][4][7] = 0;
					Model::map[Model::stage][4][8] = 0;
					Model::isStage3[0] = true;
				}
				break;
			case 9:
				if (!Model::isStage3[1]) {
					Model::map[Model::stage][9][11] = 0;
					Model::isStage3[1] = true;
				}
				break;
			}
		}

			if (Model::boxCount_inHome == Model::boxCount[Model::stage]){	//�ش��ϴ� ���������� ������ ������
				Model::screenClean(); // ȭ�������ְ�
				if (Model::stage3_bestCount != 0 && Model::stage3_bestCount > Model::count) { //ó�� �ԷµǴ°� �ƴϰ�, count�� �ְ��Ϻ��� ���� ����̶��,
					Model::stage3_bestCount = Model::count;
					gotoxy(5, 7); cout << "�ְ� ��� �޼� ! , player�� �̸��� �Է����ּ��� : ";
					getline(cin, Model::stage3_bestPlayer);
				}
				else if (Model::stage3_bestCount == 0) { //ó�� ��ŷ�� ����Ǵ°Ŷ��,
					Model::stage3_bestCount = Model::count;
					gotoxy(5, 7); cout << "�ְ� ��� �޼� ! , player�� �̸��� �Է����ּ��� : ";
					getline(cin, Model::stage3_bestPlayer);
				}
				Model::writeRanking();
				
				while (!Model::mapStack.empty()) Model::mapStack.pop(); //stack�ʱ�ȭ
				Model::pushCount = 0;
				Model::popCount = 0;
				Model::isSave = false;

				Model::isGameClear = true;
			}
			
		}//�������ܰ��� , else if
	}
		
	

int Model::getBoxCountInHome()		//���� ���ִ� ���� ������ �˷��ִ� �Լ�.
{
	Model::boxCount_inHome = 0;
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			if (Model::map[Model::stage][i][j] == 5)	//���� ��ȿ� ���� ��
				Model::boxCount_inHome++;	//fullroom�� ���ڸ� ����

		}
	}
	return Model::boxCount_inHome;
};

void Model::playerLocation() {
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			if (Model::map[stage][i][j] == 2)	//������� ��ġ�� Ž��
			{
				Model::playerX = j;	//y��ǥ
				Model::playerY = i;	//x��ǥ
			}
		}
	}
}

void Model::selectMapScreen() {
	Model::screenClean();

	textcolor(YELLOW);
	gotoxy(3, 5);
	
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			cout << Model::character[Model::registMap[0][i][j]];	//	�� ���
		}
		cout << endl;
		gotoxy(3, 5 + i);
	}
	gotoxy(12, 23);
	cout << " < STAGE 1 > ";
	////////////////////////stage 1
	textcolor(WHITE);
	gotoxy(43, 5);  //35 5

	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			cout << Model::character[Model::registMap[1][i][j]];	//	�� ���
		}
		cout << endl;
		gotoxy(43, 5 + i);//35 5
	}
	gotoxy(52, 23);//44 23
	cout << " < STAGE 2 > ";
	//////// stage 2

	gotoxy(85, 5);//70 5
	
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COLS; j++)
		{

			cout << Model::character[Model::registMap[2][i][j]];	//	�� ���
		}
		cout << endl;
		gotoxy(85, 5 + i);// 70 5
	}
	gotoxy(94, 23); // 79 23
	cout << " < STAGE 3 > ";
	////////// stage 3

	textcolor(WHITE);
	gotoxy(16, 25);//8 25
	cout << "����Ű (       <-     ) �� (        ->        ) �� ���� �����Ͻʽÿ� ";
	
	while (!Model::isGameSelected && Model::isMenuSelected) { //�޴��� ���õǾ��ְ� ������ ���۵����ʾ�����
		switch (Model::getKey())
		{
			case RIGHTKEY: Model::moveLeftRight(RIGHTKEY); break;
			case LEFTKEY: Model::moveLeftRight(LEFTKEY); break;
			case ENTERKEY: selectMap();  break;
			case ESCKEY:
				View::mainScreen(); Model::isMenuSelected = false; Model::currentStage = 0;
				break;
		}
	}//while
		//isMapSelected �� true��.
	
}
void Model::selectMap() {
	////�� �ʱ�ȭ ������
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			Model::map[Model::stage][i][j] = Model::registMap[Model::stage][i][j];	//Ȥ�ó� ������ �޴��� ���ƿԴ� �ٽ� �������� �� �� �ʱ�ȭ.
		}
	}
	Model::isMenuSelected = false;
	Model::isMapSelected = true;
	Model::isGameSelected = true;
	 if (Model::stage == 0)	//���� ù��° ���� �����Ͽ��� ��
	{
		Model::screenClean();
		View::printMap();
	}
	else if (Model::stage == 1)	//���� �ι�° ���� �����Ͽ��� ��
	{
		 Model::screenClean();
		 View::printMap();
	}
	else if (Model::stage == 2)	//���� ����° ���� �����Ͽ��� ��
	{
		 Model::screenClean();
		 View::printMap();
	}
	
}
void Model::moveLeftRight(int key) {
	if (key == RIGHTKEY) { //������ Ű��� 
		if (Model::currentStage<2) {
			Model::currentStage++;
		}
	}
	else if (key == LEFTKEY) {
		if (Model::currentStage > 0) { //���� Ű�� ���ȴٸ�.
			Model::currentStage--;
		}
	}
	if (Model::currentStage == 0) { Model::stage = Model::currentStage; textcolor(YELLOW); }
	else { textcolor(WHITE);}
		gotoxy(3, 5);

		for (int i = 0; i < ROW; i++)
		{
			for (int j = 0; j < COLS; j++)
			{

				cout << Model::character[Model::registMap[0][i][j]];	//	�� ���
			}
			cout << endl;
			gotoxy(3, 5 + i);
		}
		gotoxy(12, 23);
		cout << " < STAGE 1 > ";
		gotoxy(43, 5);

		if (Model::currentStage == 1) { Model::stage = Model::currentStage; textcolor(YELLOW); }
		else { textcolor(WHITE); }

		for (int i = 0; i < ROW; i++)
		{
			for (int j = 0; j < COLS; j++)
			{

				cout << Model::character[Model::registMap[1][i][j]];	//	�� ���
			}
			cout << endl;
			gotoxy(43, 5 + i);
		}
		gotoxy(52, 23);
		cout << " < STAGE 2 > ";
		
		gotoxy(85, 5);

		if (Model::currentStage == 2) { Model::stage = Model::currentStage; textcolor(YELLOW); }
		else { textcolor(WHITE); }
		for (int i = 0; i < ROW; i++)
		{
			for (int j = 0; j < COLS; j++)
			{

				cout << Model::character[Model::registMap[2][i][j]];	//	�� ���
			}
			cout << endl;
			gotoxy(85, 5 + i);
		}
		gotoxy(94, 23);
		cout << " < STAGE 3 > ";
	
}
void Model::readRanking() {
	int rankPoint[3];	//����� ������ �迭�� ����
	ifstream input("userRanking.txt"); //�Է½�Ʈ�� ����   
	for (int i = 0; i < 3; i++)	// �� stage�� 3�̹Ƿ� 3�� �ݺ����ش�.
	{
		char cstr[10];	//���ڿ��� ���� �迭 ������ ����
		input.getline(cstr, 10);	//���ڿ��� �ް� ���پ� �߶��ش�.
		rankPoint[i] = atoi(cstr);	//���ڿ� cstr������ ���� line�迭�� ���� (atoi�� char�� int������ ��ȯ���ش�.) 
	}
	Model::stage1_bestCount = rankPoint[0];	//1�ܰ� ��� line[0]�� 1�ܰ� �ְ��Ͽ� ����
	Model::stage2_bestCount = rankPoint[1];	//2�ܰ� ��� line[1]�� 2�ܰ� �ְ��Ͽ� ����
	Model::stage3_bestCount = rankPoint[2];	//3�ܰ� ��� line[2]�� 3�ܰ� �ְ��Ͽ� ����
	
	string playerName[3];	//����� ������ �迭�� ����
	ifstream input_2("userRanking_playerName.txt"); //�Է½�Ʈ�� ����   
	for (int i = 0; i < 3; i++)	// �� stage�� 3�̹Ƿ� 3�� �ݺ����ش�.
	{
		char str[20];	//���ڿ��� ���� �迭 ������ ����
		input_2.getline(str, 20);	//���ڿ��� �ް� ���پ� �߶��ش�.
		playerName[i] = str;	//���ڿ� cstr������ ���� line�迭�� ���� (atoi�� char�� int������ ��ȯ���ش�.) 
	}
	Model::stage1_bestPlayer = playerName[0];	//1�ܰ� ��� line[0]�� 1�ܰ� �ְ��Ͽ� ����
	Model::stage2_bestPlayer = playerName[1];	//2�ܰ� ��� line[1]�� 2�ܰ� �ְ��Ͽ� ����
	Model::stage3_bestPlayer = playerName[2];	//3�ܰ� ��� line[2]�� 3�ܰ� �ְ��Ͽ� ����
}
void Model::writeRanking() {
	ofstream out("userRanking.txt");	//txt������ ������� ���� ���� ���ش�.
	out <<Model::stage1_bestCount << endl;		// 1�ܰ� �ְ����� ����
	out << Model::stage2_bestCount << endl;		// 2�ܰ� �ְ����� ����
	out   << Model::stage3_bestCount << endl;		// 3�ܰ� �ְ����� ����
	out.close();   //������ ��������.�ٸ���� ���� 

	ofstream out_2("userRanking_playerName.txt");
	out_2 << Model::stage1_bestPlayer << endl;
	out_2 << Model::stage2_bestPlayer << endl;
	out_2 << Model::stage3_bestPlayer << endl;

	out_2.close();
}
void Model::save() {
	Model::isSave = true;
	for (int i = 0;i<ROW;i++) {
		for (int j = 0;j<COLS;j++) {
			Model::saveMap[i][j] = Model::map[Model::stage][i][j];
		}
	}
}

void Model::load() {
	while (!Model::mapStack.empty())Model::mapStack.pop();//stack �ʱ�ȭ

	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COLS; j++) {
			Model::map[Model::stage][i][j] = Model::saveMap[i][j];
		}
	}
	Model::isSave = false;
}
void Model::screenClean(){	//ȭ�� �ʱ�ȭ
	int i;

	for (i = 1; i < 50; i++){	
		gotoxy(1, i);
		cout << "                                                                                                                                               ";
	}
};

/*
t main() {
	int x = 42;
	string s = to_string(x);
	cout << s;
*/