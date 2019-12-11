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
int Model::currentMenu = 1; //1 ~5 게임시작 맵선택 도움말 랭킹정보 게임종료
bool Model::isGameSelected = false; //true라면 게임이 선택됨
bool Model::isMenuSelected = false; //true라면 메뉴가 선택됨
int Model::currentMap = 0; //1~3 stage1 stage2 stage3
bool Model::isMapSelected = false; //true일때 선택된 맵을 출력.
bool Model::isEndFlag = false; //종료버튼에 enter일때 true로 변경.
bool Model::isGameClear = false;
int Model::count = 0; //플레이어의 이동횟수
int Model::stage = 0;//스테이지.
int Model::boxCount[3] = {16,19,13}; // 스테이지당 box Count
int Model::boxCount_inHome = 0; //홈에 들어가있는 박스의 개수, 초기값 0
int Model::playerX = 0; //플레이어 x 좌표 초기화
int Model::playerY = 0; //  "" 초기화
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
int Model::pushCount = 0; //0c초기화
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


char Model::character[][COLS]= { "  ", "■","☆","▩","♧","♣","★","▒" }; //빈 여백 7
  // ▩ 박스    ♧빈집   ♣ 꽉찬 집 ■ 벽  ☆ 플레이어 ★ 집에들어간 플레이어
int Model::map[3][ROW][COLS] =		//맵을 나타내주는 3차원 배열
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
int Model::registMap[3][ROW][COLS] =	//stage마다 맵의 초기값을 저장하는 변수
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

void Model::pushChangedMap() { //맵이 변경될떄마다. 즉 키보드 입력이 있을때마다 stack에 데이터가 들어가야한다.
	int(*address)[20];

	Model::pushCount++;

	for (int i = 0;i<ROW;i++) { //변경된  맵 정보 저장.
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

void Model::popChangedMap() { // 롤백키를 누르면 작동하게끔하자.
	int(*address)[ROW];
	int index = 0;//받아온 배열을 map에 저장하기위해 인덱스  why? 포인터로 받아오니까 순서대로 (1차원배열처럼 일렬로)  데이터가 연결되어있음.)
	
	if(Model::popCount <6)
	Model::popCount++;

	if (Model::popCount<=5) {//돌아가기는 총 5번까지만 가능.
		if (!mapStack.empty()) {
			index = 0;
			address = Model::mapStack.top();

			for (int i = 0; i < ROW; i++) {
				for (int j = 0; j < COLS; j++) {
					Model::map[Model::stage][i][j] = (*address)[index++];
				}
			}

			Model::mapStack.pop(); //데이터를 가지고왔으니, 뺴버리자.	cout << "asdsadads" << endl;
			Model::count += -1;
		}//inner if
	}//if
	
}
void Model::clearCondition() 
{
	if (Model::stage<3) { //아직 게임중
		Model::getBoxCountInHome();	//공이 들어있는 방의 갯수를 탐색하는 함수 호출
		
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
			if (Model::boxCount_inHome == Model::boxCount[Model::stage])	//해당하는 스테이지의 조건이 맞으면
			{
				
				Model::screenClean(); // 화면지워주고
				switch (Model::stage)
				{
				case 0:
					if (Model::stage1_bestCount != 0 && Model::stage1_bestCount > Model::count) { //처음 입력되는게 아니고, count가 최고기록보다 좋은 기록이라면,
						Model::stage1_bestCount = Model::count;
						gotoxy(5, 7); cout << "최고 기록 달성 ! , player의 이름을 입력해주세요 : ";
						getline(cin,Model::stage1_bestPlayer);
					}
					else if (Model::stage1_bestCount == 0) { //처음 랭킹에 저장되는거라면,
						Model::stage1_bestCount = Model::count;
						gotoxy(5, 7); cout << "최고 기록 달성 ! , player의 이름을 입력해주세요 : ";
						getline(cin, Model::stage1_bestPlayer);
					}
					break;
				case 1:
					if (Model::stage2_bestCount != 0 && Model::stage2_bestCount > Model::count) { //처음 입력되는게 아니고, count가 최고기록보다 좋은 기록이라면,
						Model::stage2_bestCount = Model::count;
						gotoxy(5, 7); cout << "최고 기록 달성 ! , player의 이름을 입력해주세요 : ";
						getline(cin, Model::stage2_bestPlayer);
					}
					else if (Model::stage2_bestCount == 0) { //처음 랭킹에 저장되는거라면,
						Model::stage2_bestCount = Model::count;
						gotoxy(5, 7); cout << "최고 기록 달성 ! , player의 이름을 입력해주세요 : ";
						getline(cin, Model::stage2_bestPlayer);
					}break;
				}

				Model::writeRanking();//////////////랭킹정보 저장하기

				while (!Model::mapStack.empty()) Model::mapStack.pop(); //stack초기화
				Model::pushCount = 0;
				Model::popCount = 0;
				Model::isSave = false;

					Model::screenClean(); //맵 CLEAN
					gotoxy(5, 5); cout << "이번 stage 움직인 횟수 :  " << Model::count << endl;
					gotoxy(5, 9); cout << "아무키나 입력하면 다음 단계 START " << endl;
					if (getKey())	//키를 입력 받으면
					{
						Model::screenClean();
						View::nextMap();
					}//getKey
			}//if
		}
		else if (Model::stage == 2) { // 마지막 단계라면

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

			if (Model::boxCount_inHome == Model::boxCount[Model::stage]){	//해당하는 스테이지의 조건이 맞으면
				Model::screenClean(); // 화면지워주고
				if (Model::stage3_bestCount != 0 && Model::stage3_bestCount > Model::count) { //처음 입력되는게 아니고, count가 최고기록보다 좋은 기록이라면,
					Model::stage3_bestCount = Model::count;
					gotoxy(5, 7); cout << "최고 기록 달성 ! , player의 이름을 입력해주세요 : ";
					getline(cin, Model::stage3_bestPlayer);
				}
				else if (Model::stage3_bestCount == 0) { //처음 랭킹에 저장되는거라면,
					Model::stage3_bestCount = Model::count;
					gotoxy(5, 7); cout << "최고 기록 달성 ! , player의 이름을 입력해주세요 : ";
					getline(cin, Model::stage3_bestPlayer);
				}
				Model::writeRanking();
				
				while (!Model::mapStack.empty()) Model::mapStack.pop(); //stack초기화
				Model::pushCount = 0;
				Model::popCount = 0;
				Model::isSave = false;

				Model::isGameClear = true;
			}
			
		}//마지막단계라면 , else if
	}
		
	

int Model::getBoxCountInHome()		//공이 들어가있는 방의 개수를 알려주는 함수.
{
	Model::boxCount_inHome = 0;
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			if (Model::map[Model::stage][i][j] == 5)	//공이 방안에 들어갔을 때
				Model::boxCount_inHome++;	//fullroom의 숫자를 증가

		}
	}
	return Model::boxCount_inHome;
};

void Model::playerLocation() {
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			if (Model::map[stage][i][j] == 2)	//사용자의 위치를 탐색
			{
				Model::playerX = j;	//y좌표
				Model::playerY = i;	//x좌표
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
			cout << Model::character[Model::registMap[0][i][j]];	//	맵 출력
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
			cout << Model::character[Model::registMap[1][i][j]];	//	맵 출력
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

			cout << Model::character[Model::registMap[2][i][j]];	//	맵 출력
		}
		cout << endl;
		gotoxy(85, 5 + i);// 70 5
	}
	gotoxy(94, 23); // 79 23
	cout << " < STAGE 3 > ";
	////////// stage 3

	textcolor(WHITE);
	gotoxy(16, 25);//8 25
	cout << "방향키 (       <-     ) 와 (        ->        ) 로 맵을 선택하십시오 ";
	
	while (!Model::isGameSelected && Model::isMenuSelected) { //메뉴가 선택되어있고 게임이 시작되지않았을떄
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
		//isMapSelected 는 true임.
	
}
void Model::selectMap() {
	////맵 초기화 시켜줌
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			Model::map[Model::stage][i][j] = Model::registMap[Model::stage][i][j];	//혹시나 게임중 메뉴로 돌아왔다 다시 실행했을 시 맵 초기화.
		}
	}
	Model::isMenuSelected = false;
	Model::isMapSelected = true;
	Model::isGameSelected = true;
	 if (Model::stage == 0)	//만일 첫번째 맵을 선택하였을 때
	{
		Model::screenClean();
		View::printMap();
	}
	else if (Model::stage == 1)	//만일 두번째 맵을 선택하였을 때
	{
		 Model::screenClean();
		 View::printMap();
	}
	else if (Model::stage == 2)	//만일 세번째 맵을 선택하였을 때
	{
		 Model::screenClean();
		 View::printMap();
	}
	
}
void Model::moveLeftRight(int key) {
	if (key == RIGHTKEY) { //오른쪽 키라면 
		if (Model::currentStage<2) {
			Model::currentStage++;
		}
	}
	else if (key == LEFTKEY) {
		if (Model::currentStage > 0) { //왼쪽 키가 눌렸다면.
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

				cout << Model::character[Model::registMap[0][i][j]];	//	맵 출력
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

				cout << Model::character[Model::registMap[1][i][j]];	//	맵 출력
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

				cout << Model::character[Model::registMap[2][i][j]];	//	맵 출력
			}
			cout << endl;
			gotoxy(85, 5 + i);
		}
		gotoxy(94, 23);
		cout << " < STAGE 3 > ";
	
}
void Model::readRanking() {
	int rankPoint[3];	//기록을 저장할 배열을 선언
	ifstream input("userRanking.txt"); //입력스트림 열고   
	for (int i = 0; i < 3; i++)	// 총 stage가 3이므로 3번 반복해준다.
	{
		char cstr[10];	//문자열을 받을 배열 변수를 선언
		input.getline(cstr, 10);	//문자열을 받고 한줄씩 잘라준다.
		rankPoint[i] = atoi(cstr);	//문자열 cstr변수의 값을 line배열에 저장 (atoi는 char를 int형으로 변환해준다.) 
	}
	Model::stage1_bestCount = rankPoint[0];	//1단계 기록 line[0]을 1단계 최고기록에 저장
	Model::stage2_bestCount = rankPoint[1];	//2단계 기록 line[1]을 2단계 최고기록에 저장
	Model::stage3_bestCount = rankPoint[2];	//3단계 기록 line[2]을 3단계 최고기록에 저장
	
	string playerName[3];	//기록을 저장할 배열을 선언
	ifstream input_2("userRanking_playerName.txt"); //입력스트림 열고   
	for (int i = 0; i < 3; i++)	// 총 stage가 3이므로 3번 반복해준다.
	{
		char str[20];	//문자열을 받을 배열 변수를 선언
		input_2.getline(str, 20);	//문자열을 받고 한줄씩 잘라준다.
		playerName[i] = str;	//문자열 cstr변수의 값을 line배열에 저장 (atoi는 char를 int형으로 변환해준다.) 
	}
	Model::stage1_bestPlayer = playerName[0];	//1단계 기록 line[0]을 1단계 최고기록에 저장
	Model::stage2_bestPlayer = playerName[1];	//2단계 기록 line[1]을 2단계 최고기록에 저장
	Model::stage3_bestPlayer = playerName[2];	//3단계 기록 line[2]을 3단계 최고기록에 저장
}
void Model::writeRanking() {
	ofstream out("userRanking.txt");	//txt파일이 없을경우 직접 생성 해준다.
	out <<Model::stage1_bestCount << endl;		// 1단계 최고기록을 저장
	out << Model::stage2_bestCount << endl;		// 2단계 최고기록을 저장
	out   << Model::stage3_bestCount << endl;		// 3단계 최고기록을 저장
	out.close();   //파일을 직접만듬.다만들고 닫음 

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
	while (!Model::mapStack.empty())Model::mapStack.pop();//stack 초기화

	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COLS; j++) {
			Model::map[Model::stage][i][j] = Model::saveMap[i][j];
		}
	}
	Model::isSave = false;
}
void Model::screenClean(){	//화면 초기화
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