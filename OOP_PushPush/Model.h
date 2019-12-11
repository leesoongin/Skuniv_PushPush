#pragma once

#include <iostream>
#include <stack>

#define ROW 20
#define COLS 20
#define STAGE 3

using namespace std;

class Model
{
public:
	Model();
	~Model();
	static int currentMenu; //���� Ŀ���� ����Ű�� ����
	static bool isGameSelected; //���ӽ����� ���É����� true , �ƴ϶�� false
	static bool isMenuSelected; // �޴��� ���É����� true , �ƴ϶�� false
	static int currentMap; // �� ����â���� ���õ� map
	static bool isMapSelected; // ���� ���õǸ� �޴�ȭ�鿡�� ����ȭ������ ��ȯ.
	static bool isEndFlag; //�����ư���ȴ��� flag
	static bool isGameClear; //������ ���� �Ǿ�����
	static int count; //������ �̵��� Ƚ�� ī��Ʈ
	static int stage; //stage ����
	static int playerX;// �÷��̾��� x ����
	static int playerY;  // �÷��̾��� y ��ǥ
	static int boxCount[3]; //�ʴ� box�� ����
	static int boxCount_inHome; //Ȩ�� �� box�� ����.
	static int map[STAGE][ROW][COLS];	//���� ������ �����ϴ� map
	static int registMap[STAGE][ROW][COLS];	//�� stage�� �ܰ躰 �ʱ��.
	static char character[ROW][COLS]; // �� , ��� , �÷��̾� , �� ... �� ĳ���͹��ڿ��� ����. const�� 1�����迭�ص� �Ǵµ� ������.�׳� ũ���༭ 2�����迭 �ع����� ��
	static stack<int(*)[COLS]> mapStack;  //�ٲ� �� ���� �迭�� �ּҰ��� �����Ѵ�
	static int rollbackChangeMap[ROW][COLS]; //�ٲ� �� ������ �����ϴ� �迭.
	static int rollbackChangeMap_2[ROW][COLS]; //�ٲ� �� ������ �����ϴ� �迭.
	static int rollbackChangeMap_3[ROW][COLS]; //�ٲ� �� ������ �����ϴ� �迭.
	static int rollbackChangeMap_4[ROW][COLS]; //�ٲ� �� ������ �����ϴ� �迭.
	static int rollbackChangeMap_5[ROW][COLS]; //�ٲ� �� ������ �����ϴ� �迭.
	static int saveMap[ROW][COLS]; //���� ���� ���̺� �� 2���� �迭.
	static int currentStage; //�� �����Ҷ� �� stage ����. �ʱⰪ���ϴ� stage 1 
	static void moveLeftRight(int key); //�ʼ����Ҷ� �翷���� �̵������ϰ�.
	static int stage1_bestCount; //�� ���庰 �ְ� ����
	static int stage2_bestCount;
	static int stage3_bestCount;
	static bool isStage1[3];
	static bool isStage2[3];
	static bool isStage3[2];
	static string stage1_bestPlayer; //�� ���庰 �ְ����� ����� �÷��̾�.
	static string stage2_bestPlayer;
	static string stage3_bestPlayer;
	static int pushCount; //  stack�� ���������� count. 5�� ������ ����. ���ε����� �˾Ƽ� ��������.
	static int popCount; // ���������� count
	static void selectMapScreen(); //�� ����â �̵�
	static void selectMap(); //�� ����
	static void pushChangedMap(); // �ٲ� ���� ������ ���ÿ� �������ִ� �Լ�.
	static void popChangedMap(); //�ѹ� �������� ���ÿ��� �ٷ� ���� �� ������ ������ �������� ���ư��� �Լ�.
	static void clearCondition(); // Ŭ��������
	static int getBoxCountInHome(); //boxCount_inHome���� ��ȯ�ϴ� �Լ�. 
	static void screenClean();//ȭ�� Ŭ����.
	static void playerLocation(); //�÷��̾��� x,y ��ǥ�� �ҷ�����.
	static void writeRanking(); // ��ŷ���� �����ϱ�..
	static void readRanking();	//��ŷ�� ����ϴ� �Լ�
	static void save(); // map save �ϴ� �Լ�.
	static void load(); //save �� map load �Լ�.
	static bool isSave; //true�̸� load���� �������� �����ų� stageŬ����� false�� �ٲ� saveŰ ������ true loadŰ ������ false
	static int getKey();
};

