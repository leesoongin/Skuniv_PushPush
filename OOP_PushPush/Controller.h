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
	int playGame(); //���������� ���� ���ư��� �Լ�
	int selectMenu(); //�޴����� �� �޴����� ��� ���ư��� ����̶�, �������� �ʱ�ȭ �������.
    void menuList(); //�޴� ����Ʈ���� ��� �������� .
	static void back(); //esc�������� �ʱ�ȭ������ ���ƿ��� ���ѳ༮
	static void resetMap(); //�� �ʱ�ȭ.
};

