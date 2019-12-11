#pragma once
#define VIEW_H

#include <iostream>
#include "Model.h"

using namespace std;
class View
{
	
public:
	View();
	~View();
	static void mainScreen(); //  초기화면. 그려주기.
	static void printMap(); //맵 그려주기.
	static void printGuide(); // 게임의 정보 (유저의 이동횟수 , 키 도움말 등 ... ) 출력해줄 함수,
	static void moveMenu(int key); //위아래로 메뉴 선택할때
	static void selectMap(int key); //맵 선택시 그려줄 화면
	static void previousMap(); //이전맵으로 돌아가자.
	static void nextMap(); //다음맵으로 이동.
	static void rankingView(); //랭킹정보 화면.
	static void clearScreen();
	static void mapDataCheck(); //임시로 스택의 데이터가 어떻게 저장되어있느지 확인하기위함
	static void adviceView();	//도움말 화면
};

