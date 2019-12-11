#pragma once


#include <iostream>
#include <conio.h>
#include "KeyCode.h"
#include "Consola.h"
#include <stack>

class Controller 
{
public:
	Controller();
	~Controller();
	void Left_Right_Key(int key); //////////
	void Up_Down_Key(int key);
	int playGame(); //실질적으로 게임 돌아가는 함수
	int selectMenu(); //메뉴셀렉 각 메뉴마다 어떻게 돌아갈지 기능이랑, 변수들의 초기화 담당하자.
    void menuList(); //메뉴 리스트에서 어떤거 선택할지 .
	static void back(); //esc눌렀을떄 초기화면으로 돌아오기 위한녀석
	static void resetMap(); //맵 초기화.
};

