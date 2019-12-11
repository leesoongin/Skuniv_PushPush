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
	static void mainScreen(); //  �ʱ�ȭ��. �׷��ֱ�.
	static void printMap(); //�� �׷��ֱ�.
	static void printGuide(); // ������ ���� (������ �̵�Ƚ�� , Ű ���� �� ... ) ������� �Լ�,
	static void moveMenu(int key); //���Ʒ��� �޴� �����Ҷ�
	static void selectMap(int key); //�� ���ý� �׷��� ȭ��
	static void previousMap(); //���������� ���ư���.
	static void nextMap(); //���������� �̵�.
	static void rankingView(); //��ŷ���� ȭ��.
	static void clearScreen();
	static void mapDataCheck(); //�ӽ÷� ������ �����Ͱ� ��� ����Ǿ��ִ��� Ȯ���ϱ�����
	static void adviceView();	//���� ȭ��
};

