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
		while (!Model::mapStack.empty()) Model::mapStack.pop(); //stack초기화
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
		Model::saveMap[ROW][COLS] = {}; //save 맵 데이터초기화
		Model::currentMenu = 1;
		Model::screenClean();
		View::mainScreen();
		Model::isGameSelected = false; //게임화면에서든
		Model::isMenuSelected = false; // 메뉴화면에서든 뒤돌아가기 (esc) 하면 둘다 초기화시켜주자 그냥.
		
}

void Controller::resetMap() {
	while (!Model::mapStack.empty()) Model::mapStack.pop(); //stack초기화   rollback에 관련된 data 초기화
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
			Model::map[Model::stage][i][j] = Model::registMap[Model::stage][i][j];	//초기맵을 repush_map변수에 저장해놓았다가 다시 원래 맵에 덮어 쓰는 과정
		}

	}
	Model::count = 0; //맵 리셋시키면서.count도 초기화
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
	else if (Model::currentMenu == 2) { // 맵선택
		Model::isMenuSelected = true;
		Model::screenClean();
		Model::selectMapScreen();

	}
	else if (Model::currentMenu == 3) {
		Model::isMenuSelected = true;

		View::adviceView();
	}
	else if (Model::currentMenu == 4) { //랭킹정보
		Model::isMenuSelected = true;

		View::rankingView(); //랭킹정보 화면 출력함수
	}
	else if (Model::currentMenu == 5) {
		Model::isEndFlag = true;
		Model::writeRanking();
		return 0;
	}
}

enum object { EMPTY=0, WALL = 1 , PLAYER = 2, BOX = 3, HOME = 4, BOX_IN_HOME=5 , PLAYER_IN_HOME = 6 }; //조건할때 활용하자 가독성 위해서.
//왼쪽이면 -1 -2  오른쪽이면 +1 +2 <- x  위쪽이면 -1 -2  아래쪽이면 +1 +2  <- y 
void Controller::Left_Right_Key(int key) {
	int prev_1, prev_2; //플레이어로부터 첫쨰칸 둘째칸까지.


	switch (key) {
	case LEFTKEY: prev_1 = -1; prev_2 = -2; break;
	case RIGHTKEY: prev_1 = 1; prev_2 = 2; break;
	}

	if (Model::map[Model::stage][Model::playerY][Model::playerX] == PLAYER) // 빈 통로에서의 플레이어일 경우.
	{
		if (Model::map[Model::stage][Model::playerY][Model::playerX+prev_1] == EMPTY) //왼쪽이 비어있다
		{
			Model::map[Model::stage][Model::playerY][Model::playerX + prev_1] = PLAYER;  //두개의 값을 바꿔준다 통로를 플레이어로 플레이어 자리를 통로로.
			Model::map[Model::stage][Model::playerY][Model::playerX] = EMPTY;
			
			Model::count++;
		}
		else if (Model::map[Model::stage][Model::playerY][Model::playerX + prev_1] == WALL) { //왼쪽이 벽이다
			return;
		}
		else if (Model::map[Model::stage][Model::playerY][Model::playerX + prev_1] == BOX )//왼쪽이 박스일때
		{
			if (Model::map[Model::stage][Model::playerY][Model::playerX + prev_2] == WALL) {// 박스의 왼쪽이 벽
				return;
			}
			if (Model::map[Model::stage][Model::playerY][Model::playerX + prev_2] == BOX) { // 박스 왼쪽이 공
				return;
			}
			if (Model::map[Model::stage][Model::playerY][Model::playerX + prev_2] == HOME ) //	박스 왼쪽이 집
			{
				Model::map[Model::stage][Model::playerY][Model::playerX + prev_2] = BOX_IN_HOME; // 집안에 박스가 들어가고
				Model::map[Model::stage][Model::playerY][Model::playerX + prev_1] = PLAYER; //박스자리에 플레이어
				Model::map[Model::stage][Model::playerY][Model::playerX] = EMPTY; // 플레이어 자리에 빈공간
				Model::count++;
			}
			else if (Model::map[Model::stage][Model::playerY][Model::playerX + prev_2] == EMPTY)	//	박스 왼쪽이 빈공간
			{
				Model::map[Model::stage][Model::playerY][Model::playerX + prev_2] = BOX; //빈공간에 박스들어가고 
				Model::map[Model::stage][Model::playerY][Model::playerX + prev_1] = PLAYER; // 박스 자리에 플레이어가 위치하고
				Model::map[Model::stage][Model::playerY][Model::playerX] = EMPTY; // r플레이어자리에 빈공간이 존재하게 된다.
				Model::count++;
			}
		} //플레이어 왼쪽이 박스일때

		else if (Model::map[Model::stage][Model::playerY][Model::playerX + prev_1] == HOME)//	플레이어 왼쪽이 집일떄.
		{
			Model::map[Model::stage][Model::playerY][Model::playerX + prev_1] = PLAYER_IN_HOME;
			Model::map[Model::stage][Model::playerY][Model::playerX] = EMPTY;
			Model::count++;
		}
		else if (Model::map[Model::stage][Model::playerY][Model::playerX + prev_1] ==BOX_IN_HOME)//		왼쪽에 박스가 들어있는 집일때
		{
			if (Model::map[Model::stage][Model::playerY][Model::playerX + prev_2] == WALL
				|| Model::map[Model::stage][Model::playerY][Model::playerX + prev_2] == BOX
				|| Model::map[Model::stage][Model::playerY][Model::playerX + prev_2] == BOX_IN_HOME) {//	왼쪽이 벽, 박스  , 박스가 들어간 집일때.
				return;
			}
			else if (Model::map[Model::stage][Model::playerY][Model::playerX + prev_2] == EMPTY)//		공이 들어있는 집 왼쪽이 빈공간일때
			{
				Model::map[Model::stage][Model::playerY][Model::playerX + prev_2] = BOX;  //왼쪽으로 박스 이동, 
				Model::map[Model::stage][Model::playerY][Model::playerX + prev_1] = PLAYER_IN_HOME; //플레이어가 집으로 들어감.
				Model::map[Model::stage][Model::playerY][Model::playerX] = EMPTY; //플레이어자리가 빈공간으로 .
				Model::count++;

			}
			else if (Model::map[Model::stage][Model::playerY][Model::playerX + prev_2] == HOME)//		공이 들어있는 집 왼쪽이 비어있는 집일때
			{
				Model::map[Model::stage][Model::playerY][Model::playerX + prev_2] = BOX_IN_HOME;
				Model::map[Model::stage][Model::playerY][Model::playerX + prev_1] = PLAYER_IN_HOME;
				Model::map[Model::stage][Model::playerY][Model::playerX] = EMPTY;
				Model::count++;
			}
		}

	}
	////////////////////////////// 집 안에서의 플레이어.
	else if (Model::map[Model::stage][Model::playerY][Model::playerX] == PLAYER_IN_HOME)
	{
		if (Model::map[Model::stage][Model::playerY][Model::playerX + prev_1] == WALL)//벽
			return;
		else if (Model::map[Model::stage][Model::playerY][Model::playerX + prev_1] == EMPTY)//빈공간
		{
			Model::map[Model::stage][Model::playerY][Model::playerX + prev_1] = PLAYER;
			Model::map[Model::stage][Model::playerY][Model::playerX] =HOME;
			Model::count++;
		}
		else if (Model::map[Model::stage][Model::playerY][Model::playerX + prev_1] == BOX)// 완쪽이 박스
		{
			if (Model::map[Model::stage][Model::playerY][Model::playerX + prev_2] == WALL
				|| Model::map[Model::stage][Model::playerY][Model::playerX + prev_2] == BOX
					|| Model::map[Model::stage][Model::playerY][Model::playerX + prev_2] == BOX_IN_HOME) {//	박스옆에 벽이거나 박스거나 박스가 들어간 집이거나.
				return;
			}
			else if (Model::map[Model::stage][Model::playerY][Model::playerX + prev_2] == EMPTY)//		내 왼쪽이 공이고 그 왼쪽이 빈공간일때 
			{
				Model::map[Model::stage][Model::playerY][Model::playerX + prev_2] = BOX;
				Model::map[Model::stage][Model::playerY][Model::playerX + prev_1] = PLAYER;
				Model::map[Model::stage][Model::playerY][Model::playerX] = HOME;
				Model::count++;
			}
			else if (Model::map[Model::stage][Model::playerY][Model::playerX + prev_2] == HOME)//	 왼쪽이 비어있는 집일때
			{
				Model::map[Model::stage][Model::playerY][Model::playerX + prev_2] =BOX_IN_HOME;
				Model::map[Model::stage][Model::playerY][Model::playerX + prev_1] = PLAYER;
				Model::map[Model::stage][Model::playerY][Model::playerX] = HOME;
				Model::count++;
			}
		} /////왼쪽이 박스인경우
		else if (Model::map[Model::stage][Model::playerY][Model::playerX + prev_1] == HOME)//집
		{
			Model::map[Model::stage][Model::playerY][Model::playerX + prev_1] = PLAYER_IN_HOME;
			Model::map[Model::stage][Model::playerY][Model::playerX] = HOME;
			Model::count++;
		}
		else if (Model::map[Model::stage][Model::playerY][Model::playerX + prev_1] == BOX_IN_HOME)//왼쪽이 박스가 들어있는 집일때
		{
			if (Model::map[Model::stage][Model::playerY][Model::playerX + prev_2] == EMPTY)//그 왼쪽이 빈공간 
			{
				Model::map[Model::stage][Model::playerY][Model::playerX + prev_2] = BOX;
				Model::map[Model::stage][Model::playerY][Model::playerX +prev_1] = PLAYER_IN_HOME;
				Model::map[Model::stage][Model::playerY][Model::playerX] = HOME;
				Model::count++;
			}
			else if (Model::map[Model::stage][Model::playerY][Model::playerX + prev_2] == WALL
				|| Model::map[Model::stage][Model::playerY][Model::playerX + prev_2] == BOX
					|| Model::map[Model::stage][Model::playerY][Model::playerX + prev_2] == BOX_IN_HOME)// 그 왼쪽이 벽, 박스, 박스가 들어있는집
				return;
			else if (Model::map[Model::stage][Model::playerY][Model::playerX + prev_2] == HOME)// 그 왼쪽이 비어있는 집
			{
				Model::map[Model::stage][Model::playerY][Model::playerX + prev_2] = BOX_IN_HOME;
				Model::map[Model::stage][Model::playerY][Model::playerX + prev_1] = PLAYER_IN_HOME;
				Model::map[Model::stage][Model::playerY][Model::playerX] = HOME;
				Model::count++;
			}
		}//왼쪽이 박스가 들어간 집일때.
	} //집안에서의 플레이어

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

	if (Model::map[Model::stage][Model::playerY][Model::playerX] == PLAYER) // 빈 통로에서의 플레이어일 경우.
	{
		if (Model::map[Model::stage][Model::playerY+prev_1][Model::playerX ] == EMPTY)
		{
			Model::map[Model::stage][Model::playerY + prev_1][Model::playerX ] = PLAYER;  //두개의 값을 바꿔준다 통로를 플레이어로 플레이어 자리를 통로로.
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
				Model::map[Model::stage][Model::playerY + prev_2][Model::playerX ] = BOX_IN_HOME; // 집안에 박스가 들어가고
				Model::map[Model::stage][Model::playerY + prev_1][Model::playerX] = PLAYER; //박스자리에 플레이어
				Model::map[Model::stage][Model::playerY][Model::playerX] = EMPTY; // 플레이어 자리에 빈공간
				Model::count++;
			}
			else if (Model::map[Model::stage][Model::playerY + prev_2][Model::playerX ] == EMPTY)
			{
				Model::map[Model::stage][Model::playerY + prev_2][Model::playerX ] = BOX; //빈공간에 박스들어가고 
				Model::map[Model::stage][Model::playerY + prev_1][Model::playerX] = PLAYER; // 박스 자리에 플레이어가 위치하고
				Model::map[Model::stage][Model::playerY][Model::playerX] = EMPTY; // r플레이어자리에 빈공간이 존재하게 된다.
				Model::count++;
			}
		} //플레이어 왼쪽이 박스일때

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
				Model::map[Model::stage][Model::playerY + prev_1][Model::playerX ] = PLAYER_IN_HOME; //플레이어가 집으로 들어감.
				Model::map[Model::stage][Model::playerY][Model::playerX] = EMPTY; //플레이어자리가 빈공간으로 .
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
	////////////////////////////// 집 안에서의 플레이어.
	else if (Model::map[Model::stage][Model::playerY][Model::playerX] == PLAYER_IN_HOME)
	{
		if (Model::map[Model::stage][Model::playerY + prev_1][Model::playerX ] == WALL)//벽
			return;
		else if (Model::map[Model::stage][Model::playerY + prev_1][Model::playerX ] == EMPTY)//빈공간
		{
			Model::map[Model::stage][Model::playerY + prev_1][Model::playerX ] = PLAYER;
			Model::map[Model::stage][Model::playerY][Model::playerX] = HOME;
			Model::count++;
		}
		else if (Model::map[Model::stage][Model::playerY + prev_1][Model::playerX] == BOX)
		{
			if (Model::map[Model::stage][Model::playerY + prev_2][Model::playerX ] == WALL
				|| Model::map[Model::stage][Model::playerY + prev_2][Model::playerX ] == BOX
				|| Model::map[Model::stage][Model::playerY + prev_2][Model::playerX ] == BOX_IN_HOME) {//	박스옆에 벽이거나 박스거나 박스가 들어간 집이거나.
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
		} /////왼쪽이 박스인경우
		else if (Model::map[Model::stage][Model::playerY + prev_1][Model::playerX ] == HOME)//집
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
	} //집안에서의 플레이어

	switch (key) {
	case UPKEY: Model::playerY--; break;
	case DOWNKEY: Model::playerY++; break;
	}
	
	return;
}




int Controller::playGame() {
	Model::readRanking();	//랭킹을 출력하는 함수 호출
	PlaySound(TEXT("bgm2.wav"), NULL, SND_FILENAME |SND_ASYNC | SND_LOOP); //bgm 재생 반복재생
	_setcursortype(_NOCURSOR);	//커서 제거

	View::mainScreen();	//메뉴 출력 함수 호출

	while (!Model::isEndFlag) {
		if (!Model::isGameSelected) { //메인화면
			if (!Model::isMenuSelected) {
				menuList();
			}//inner if
		}//if
		else if (Model::isGameSelected) { //게임화면
				Model::clearCondition(); //게임이 클리어 됬는지 확인하고
				Model::readRanking(); //랭킹 정보 불러와서 이따 가이드 부분에 출략
				if (!Model::isGameClear) {//아직 게임이 진행중이라면.
					Model::playerLocation();//현재 플레이어의 위치 기억하기.
					View::printMap(); //현재 stage 맵 출력
					View::printGuide(); // 가이드 출력

				
					switch (Model::getKey())	//만일 키를 입력받았을 때
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
					case RESETKEY: Controller::resetMap(); break;	//R키를 입력받으면 맵 초기화
					case ESCKEY: Controller::back(); break;
					}
				}//게임 클리어가아니라면 계속 키 입력받기
				else if (Model::isGameClear) { //게임클리어라면
					Model::isGameClear = false;
					Model::screenClean(); //맵 CLEAN
					
					gotoxy(5, 5); cout << "stage clear   ";
					gotoxy(5, 7); cout << "아무키나 입력하면 메인화면 " << endl;

					if (Model::getKey())	//키를 입력 받으면
					{
						View::mainScreen();
					}//getKey
				}
		}//else if
	}//while
	
	return 0;
}