#include "View.h"
#include <iostream>
#include <conio.h>
#include "Consola.h"
#include "View.h"
#include "keyCode.h"
#include "Model.h"
#include "Windows.h"
#include "Controller.h"
#include <string>

using namespace std;

View::View()
{
}
View::~View()
{
}


void View::mainScreen() {
	int time = 0;

	Model::currentMenu = 1;
	Model::screenClean();
	Model::isGameSelected = false;		//start_menu가 1일때 메뉴화면이 계속 출력되고 0으로 바뀌는 순간 게임이 시작된다.
	Model::isMapSelected = false;		//start_map을 초기화 시켜줘서 다시 메뉴화면이 정상적으로 작동하게 한다.
	system("mode con cols=140 lines=70"); //mode 창의 길이 101, 높이를 30로 한다.
	Model::stage = 0;

	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			Model::map[Model::stage][i][j] = Model::registMap[Model::stage][i][j];	//혹시나 게임중 메뉴로 돌아왔다 다시 실행했을 시 맵 초기화.
		}
	}
		Model::count = 0;	//메뉴로 돌아왔을시 움직인 횟수를 초기화

		textbackground(WHITE);
		textcolor(BLACK);

		gotoxy(5, 6); cout << "                                                                " << endl;
		gotoxy(5, 7); cout << "                                                                " << endl;
		gotoxy(5, 8);  cout << "■■■■■    ■        ■          ■■■■        ■        ■" << endl;
		gotoxy(5, 9);  cout << "■      ■    ■        ■        ■                ■        ■" << endl;
		gotoxy(5, 10);  cout << "■      ■    ■        ■        ■                ■        ■" << endl;
		gotoxy(5, 11);  cout << "■■■■■    ■        ■          ■■■■        ■■■■■■" << endl;
		gotoxy(5, 12);  cout << "■            ■        ■                ■        ■        ■" << endl;
		gotoxy(5, 13); cout << "■            ■        ■                ■        ■        ■" << endl;//
		gotoxy(5, 14); cout << "■              ■■■■          ■■■■          ■        ■" << endl;//
		gotoxy(5, 15); cout << "                                                                " << endl;

		gotoxy(5, 16);  cout << "■■■■■    ■        ■          ■■■■        ■        ■" << endl;
		gotoxy(5, 17);  cout << "■      ■    ■        ■        ■                ■        ■" << endl;
		gotoxy(5, 18);  cout << "■      ■    ■        ■        ■                ■        ■" << endl;
		gotoxy(5, 19);  cout << "■■■■■    ■        ■          ■■■■        ■■■■■■" << endl;
		gotoxy(5, 20);  cout << "■            ■        ■                ■        ■        ■" << endl;
		gotoxy(5, 21); cout << "■            ■        ■                ■        ■        ■" << endl;//
		gotoxy(5, 22); cout << "■              ■■■■          ■■■■          ■        ■" << endl;//
		gotoxy(5, 23); cout << "                                                                " << endl;
		gotoxy(5, 24); cout << "                                                                " << endl;

		textbackground(BLACK);

		textcolor(YELLOW);
		gotoxy(75, 10); cout << "1 : ★  게임시작          ";
		textcolor(WHITE);
		gotoxy(75, 12); cout << "2 :     맵  선택            ";
		gotoxy(75, 14); cout << "3 :     도 움 말            ";
		gotoxy(75, 16); cout << "4 :     랭    킹            ";
		gotoxy(75, 18); cout << "5 :     종    료            ";
}

void View::moveMenu(int key) {
	if (key == UPKEY) {
		if (Model::currentMenu > 1) {//1 ~ 5 까지가 메뉴
			Model::currentMenu--;//y 좌표 올라가
		}//inner if
	}
	else if (key == DOWNKEY) {
		if (Model::currentMenu < 5) {
			Model::currentMenu++;//y 좌표 내려가
		}//inner if
	}//else if

	if (Model::currentMenu == 2)	
	{
		textcolor(WHITE);
		gotoxy(75, 10); cout << "1 :     게임시작            ";
		textcolor(YELLOW);
		gotoxy(75, 12); cout << "2 : ★  맵  선택            ";
		textcolor(WHITE);
		gotoxy(75, 14); cout << "3 :     도 움 말            ";
		gotoxy(75, 16); cout << "4 :     랭    킹            ";
		gotoxy(75, 18); cout << "5 :     종    료            ";
	}
	else if (Model::currentMenu == 3)	
	{
		textcolor(WHITE);
		gotoxy(75, 10); cout << "1 :     게임시작            ";
		gotoxy(75, 12); cout << "2 :     맵  선택            ";
		textcolor(YELLOW);
		gotoxy(75, 14); cout << "3 : ★  도 움 말          ";
		textcolor(WHITE);
		gotoxy(75, 16); cout << "4 :     랭    킹            ";
		gotoxy(75, 18); cout << "5 :     종    료            ";
	}
	else if (Model::currentMenu == 4)	
	{
		textcolor(WHITE);
		gotoxy(75, 10); cout << "1 :     게임시작            ";
		gotoxy(75, 12); cout << "2 :     맵  선택            ";
		gotoxy(75, 14); cout << "3 :     도 움 말            ";
		textcolor(YELLOW);
		gotoxy(75, 16); cout << "4 : ★  랭    킹          ";
		textcolor(WHITE);
		gotoxy(75, 18); cout << "5 :     종    료            ";
	}
	else if (Model::currentMenu == 5)
	{
		textcolor(WHITE);
		gotoxy(75, 10); cout << "1 :     게임시작            ";
		gotoxy(75, 12); cout << "2 :     맵  선택            ";
		gotoxy(75, 14); cout << "3 :     도 움 말            ";
		gotoxy(75, 16); cout << "4 :     랭    킹            ";
		textcolor(YELLOW);
		gotoxy(75, 18); cout << "5 : ★  종    료          ";
	}
	else if (Model::currentMenu == 1)	// menu변수 값이 5로 나누었을 때 나머지가 0이면
	{
		textcolor(YELLOW);
		gotoxy(75, 10); cout << "1 : ★  게임시작          ";
		textcolor(WHITE);
		gotoxy(75, 12); cout << "2 :     맵  선택            ";
		gotoxy(75, 14); cout << "3 :     도 움 말            ";
		gotoxy(75, 16); cout << "4 :     랭    킹            ";
		gotoxy(75, 18); cout << "5 :     종    료            ";
	}

}
void View::selectMap(int key) {
	//맵선택창에서 왼,오 방향키 누르면 선택되는 그림 들어갈곳,
}

void View::printGuide() {
	int i = 5; //자꾸 추가할때마다 좌표바꾸기 귀찮앙 ㅠㅠ
	gotoxy(65, i+7);  cout << "";
	gotoxy(62, i + 8);  cout << "★ P U S H  P U S H ☆";
	gotoxy(65, i + 10);  cout << "< S T A G E " << Model::stage + 1 << " >";
	gotoxy(62, i + 12);	cout << "       UP";
	gotoxy(62, i + 13);	cout << "       ↑";
	gotoxy(62, i + 14);	cout << "LEFT ←  → RIGHT";
	gotoxy(62, i + 15);	cout << "       ↓";
	gotoxy(62, i + 16); cout << "      DOWN";
	gotoxy(62, i + 17); cout << "      P      :           롤백";
	gotoxy(65, i + 18); cout << "  ESC     :           메뉴";
	gotoxy(65, i + 19); cout << "   R      :           리셋";
	gotoxy(65, i + 20); cout << "   S      :         세이브";
	gotoxy(65, i + 21); cout << "   L      :           로드";
	gotoxy(65, i + 22); cout << "   W      :       다음단계";
	gotoxy(65, i + 23); cout << "   <-           : 이전단계";
	gotoxy(65, i + 24); cout << "   Q            : 게임종료";

	if (Model::popCount == 6) { gotoxy(62, i+26);  cout << "rollBack 가능횟수 : " << 6 - Model::popCount; }
	else { gotoxy(62, i + 26); cout << " rollBack 가능횟수 : " << 5 - Model::popCount;  }
	gotoxy(62, i + 28); cout << " 움직인 횟수 : " << Model::count<<"       ";	//움직인 횟수를 출력

	gotoxy(62 ,5); cout << "stage 1 - bestMinCount : " << Model::stage1_bestCount << "  bestPlayer : " << Model::stage1_bestPlayer;
	gotoxy(62, 6); cout << "stage 2 - bestMinCount : " << Model::stage2_bestCount << "  bestPlayer : " << Model::stage2_bestPlayer;
	gotoxy(62, 7); cout << "stage 3 - bestMinCount : " << Model::stage3_bestCount << "  bestPlayer : " << Model::stage3_bestPlayer;
}

void View::printMap() {
	textcolor(WHITE); 	

	Model::clearCondition();

	if (Model::stage < 3)	//만일 맵 CLEAR하지 못했을 때 /////////////////////나중에 이 조건 지우자 딱히 필요없음 내가 이미 flag세워놨기 때문에 !
	{
		
		gotoxy(15, 11);
		for (int i = 0; i < ROW; i++)
		{
			for (int j = 0; j < COLS; j++)
			{
				cout << Model::character[Model::map[Model::stage][i][j]];	//	맵 출력
			}
			cout << endl;
			gotoxy(15, 10 + i);
		}
		Model::getBoxCountInHome();	//공이 들어있는 방의 숫자를 출력해주는 함수 호출
	}
}
void View::previousMap()	//이전 단계 맵을 출력하기 위한 함수
{
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
	if (Model::stage > 0)	//stage1 에서 이전맵을 눌렀을 때 아무 변화가 없게 하기 위하여
	{
		Model::stage--;
		for (int i = 0; i < ROW; i++)
		{
			for (int j = 0; j <COLS; j++)
			{
				Model::map[Model::stage][i][j] = Model::registMap[Model::stage][i][j];			//stage의 맞게 초기 맵을 복사해놓았다가 붙여넣는 과정.
			}
		}
		Model::count = 0;
	}
}
void View::nextMap()	//다음 단계 맵을 출력하기 위한 함수
{
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
	if (Model::stage < 2)		//stage3 에서 다음 맵을 눌렀을 때 아무 변화가 없게 하기 위하여
	{
		Model::stage++;	// 다음 단계 맵을 출력하기 위해 stage증가
		for (int i = 0; i < ROW; i++)
		{
			for (int j = 0; j <COLS; j++)
			{
				Model::map[Model::stage][i][j] = Model::registMap[Model::stage][i][j];			//stage의 맞게 초기 맵을 복사해놓았다가 붙여넣는 과정.
			}
		}
		Model::count = 0;
	}
}
void View::rankingView() {
	Model::screenClean(); //화면 초기화.

	system("mode con cols=70 lines=20");

	gotoxy(17, 6);
	cout << " 랭 킹 정 보";
	gotoxy(10,10);
	cout << "stage 1 - bestMinCount : " <<Model::stage1_bestCount <<"  bestPlayer : " << Model::stage1_bestPlayer;
	gotoxy(10, 12);
	cout << "stage 2 - bestMinCount : " << Model::stage2_bestCount << "  bestPlayer : " << Model::stage2_bestPlayer;
	gotoxy(10, 14);
	cout << "stage 3 - bestMinCount : " << Model::stage3_bestCount << "  bestPlayer : " << Model::stage3_bestPlayer;

	gotoxy(6,18);
	cout << "ESC키를 누르면 메인화면으로 돌아갑니다.";

	while (Model::isMenuSelected) {
		switch (Model::getKey())
		{
		case ESCKEY:  View::mainScreen(); Model::isMenuSelected = false; break;
		}
	}
}

void View::mapDataCheck() { ///stack 데이터 잘 들어갔는지 확인하는 메소드
	Model::screenClean(); //화면지우고
	int(*address)[20];
	int index = 0;

	while (!Model::mapStack.empty()) {
		index = 0;
		cout << endl;
		address = Model::mapStack.top();
		for (int i = 0; i < ROW; i++) {
			for (int j = 0; j < COLS; j++) {
				cout << (*address)[index++];
			}
			cout << endl;
		}
		cout << endl;
		Model::mapStack.pop(); // 빼주고
	}
	if(Model::mapStack.empty()) { //stack 이  빈 경우
		cout << "stack is empty." << endl;
	}
	cout << endl;
	cout << "ESC키를 누르면 메인화면으로 돌아갑니다.";

	while (Model::isMenuSelected) {
		switch (Model::getKey())
		{
		case ESCKEY:  View::mainScreen(); Model::isMenuSelected = false; break;
		}
	}
}

void View::adviceView() {
	Model::screenClean(); //화면지우고

	cout << endl;
	cout << "ESC키를 누르면 메인화면으로 돌아갑니다.";

	while (Model::isMenuSelected) {
		switch (Model::getKey())
		{
		case ESCKEY:  View::mainScreen(); Model::isMenuSelected = false; break;
		}
	}
}
void View::clearScreen() {
	Model::screenClean(); //화면지우고

	gotoxy(30, 30);
	cout << "stage clear";
}