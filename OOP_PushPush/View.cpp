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
	Model::isGameSelected = false;		//start_menu�� 1�϶� �޴�ȭ���� ��� ��µǰ� 0���� �ٲ�� ���� ������ ���۵ȴ�.
	Model::isMapSelected = false;		//start_map�� �ʱ�ȭ �����༭ �ٽ� �޴�ȭ���� ���������� �۵��ϰ� �Ѵ�.
	system("mode con cols=140 lines=70"); //mode â�� ���� 101, ���̸� 30�� �Ѵ�.
	Model::stage = 0;

	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			Model::map[Model::stage][i][j] = Model::registMap[Model::stage][i][j];	//Ȥ�ó� ������ �޴��� ���ƿԴ� �ٽ� �������� �� �� �ʱ�ȭ.
		}
	}
		Model::count = 0;	//�޴��� ���ƿ����� ������ Ƚ���� �ʱ�ȭ

		textbackground(WHITE);
		textcolor(BLACK);

		gotoxy(5, 6); cout << "                                                                " << endl;
		gotoxy(5, 7); cout << "                                                                " << endl;
		gotoxy(5, 8);  cout << "������    ��        ��          �����        ��        ��" << endl;
		gotoxy(5, 9);  cout << "��      ��    ��        ��        ��                ��        ��" << endl;
		gotoxy(5, 10);  cout << "��      ��    ��        ��        ��                ��        ��" << endl;
		gotoxy(5, 11);  cout << "������    ��        ��          �����        �������" << endl;
		gotoxy(5, 12);  cout << "��            ��        ��                ��        ��        ��" << endl;
		gotoxy(5, 13); cout << "��            ��        ��                ��        ��        ��" << endl;//
		gotoxy(5, 14); cout << "��              �����          �����          ��        ��" << endl;//
		gotoxy(5, 15); cout << "                                                                " << endl;

		gotoxy(5, 16);  cout << "������    ��        ��          �����        ��        ��" << endl;
		gotoxy(5, 17);  cout << "��      ��    ��        ��        ��                ��        ��" << endl;
		gotoxy(5, 18);  cout << "��      ��    ��        ��        ��                ��        ��" << endl;
		gotoxy(5, 19);  cout << "������    ��        ��          �����        �������" << endl;
		gotoxy(5, 20);  cout << "��            ��        ��                ��        ��        ��" << endl;
		gotoxy(5, 21); cout << "��            ��        ��                ��        ��        ��" << endl;//
		gotoxy(5, 22); cout << "��              �����          �����          ��        ��" << endl;//
		gotoxy(5, 23); cout << "                                                                " << endl;
		gotoxy(5, 24); cout << "                                                                " << endl;

		textbackground(BLACK);

		textcolor(YELLOW);
		gotoxy(75, 10); cout << "1 : ��  ���ӽ���          ";
		textcolor(WHITE);
		gotoxy(75, 12); cout << "2 :     ��  ����            ";
		gotoxy(75, 14); cout << "3 :     �� �� ��            ";
		gotoxy(75, 16); cout << "4 :     ��    ŷ            ";
		gotoxy(75, 18); cout << "5 :     ��    ��            ";
}

void View::moveMenu(int key) {
	if (key == UPKEY) {
		if (Model::currentMenu > 1) {//1 ~ 5 ������ �޴�
			Model::currentMenu--;//y ��ǥ �ö�
		}//inner if
	}
	else if (key == DOWNKEY) {
		if (Model::currentMenu < 5) {
			Model::currentMenu++;//y ��ǥ ������
		}//inner if
	}//else if

	if (Model::currentMenu == 2)	
	{
		textcolor(WHITE);
		gotoxy(75, 10); cout << "1 :     ���ӽ���            ";
		textcolor(YELLOW);
		gotoxy(75, 12); cout << "2 : ��  ��  ����            ";
		textcolor(WHITE);
		gotoxy(75, 14); cout << "3 :     �� �� ��            ";
		gotoxy(75, 16); cout << "4 :     ��    ŷ            ";
		gotoxy(75, 18); cout << "5 :     ��    ��            ";
	}
	else if (Model::currentMenu == 3)	
	{
		textcolor(WHITE);
		gotoxy(75, 10); cout << "1 :     ���ӽ���            ";
		gotoxy(75, 12); cout << "2 :     ��  ����            ";
		textcolor(YELLOW);
		gotoxy(75, 14); cout << "3 : ��  �� �� ��          ";
		textcolor(WHITE);
		gotoxy(75, 16); cout << "4 :     ��    ŷ            ";
		gotoxy(75, 18); cout << "5 :     ��    ��            ";
	}
	else if (Model::currentMenu == 4)	
	{
		textcolor(WHITE);
		gotoxy(75, 10); cout << "1 :     ���ӽ���            ";
		gotoxy(75, 12); cout << "2 :     ��  ����            ";
		gotoxy(75, 14); cout << "3 :     �� �� ��            ";
		textcolor(YELLOW);
		gotoxy(75, 16); cout << "4 : ��  ��    ŷ          ";
		textcolor(WHITE);
		gotoxy(75, 18); cout << "5 :     ��    ��            ";
	}
	else if (Model::currentMenu == 5)
	{
		textcolor(WHITE);
		gotoxy(75, 10); cout << "1 :     ���ӽ���            ";
		gotoxy(75, 12); cout << "2 :     ��  ����            ";
		gotoxy(75, 14); cout << "3 :     �� �� ��            ";
		gotoxy(75, 16); cout << "4 :     ��    ŷ            ";
		textcolor(YELLOW);
		gotoxy(75, 18); cout << "5 : ��  ��    ��          ";
	}
	else if (Model::currentMenu == 1)	// menu���� ���� 5�� �������� �� �������� 0�̸�
	{
		textcolor(YELLOW);
		gotoxy(75, 10); cout << "1 : ��  ���ӽ���          ";
		textcolor(WHITE);
		gotoxy(75, 12); cout << "2 :     ��  ����            ";
		gotoxy(75, 14); cout << "3 :     �� �� ��            ";
		gotoxy(75, 16); cout << "4 :     ��    ŷ            ";
		gotoxy(75, 18); cout << "5 :     ��    ��            ";
	}

}
void View::selectMap(int key) {
	//�ʼ���â���� ��,�� ����Ű ������ ���õǴ� �׸� ����,
}

void View::printGuide() {
	int i = 5; //�ڲ� �߰��Ҷ����� ��ǥ�ٲٱ� ������ �Ф�
	gotoxy(65, i+7);  cout << "";
	gotoxy(62, i + 8);  cout << "�� P U S H  P U S H ��";
	gotoxy(65, i + 10);  cout << "< S T A G E " << Model::stage + 1 << " >";
	gotoxy(62, i + 12);	cout << "       UP";
	gotoxy(62, i + 13);	cout << "       ��";
	gotoxy(62, i + 14);	cout << "LEFT ��  �� RIGHT";
	gotoxy(62, i + 15);	cout << "       ��";
	gotoxy(62, i + 16); cout << "      DOWN";
	gotoxy(62, i + 17); cout << "      P      :           �ѹ�";
	gotoxy(65, i + 18); cout << "  ESC     :           �޴�";
	gotoxy(65, i + 19); cout << "   R      :           ����";
	gotoxy(65, i + 20); cout << "   S      :         ���̺�";
	gotoxy(65, i + 21); cout << "   L      :           �ε�";
	gotoxy(65, i + 22); cout << "   W      :       �����ܰ�";
	gotoxy(65, i + 23); cout << "   <-           : �����ܰ�";
	gotoxy(65, i + 24); cout << "   Q            : ��������";

	if (Model::popCount == 6) { gotoxy(62, i+26);  cout << "rollBack ����Ƚ�� : " << 6 - Model::popCount; }
	else { gotoxy(62, i + 26); cout << " rollBack ����Ƚ�� : " << 5 - Model::popCount;  }
	gotoxy(62, i + 28); cout << " ������ Ƚ�� : " << Model::count<<"       ";	//������ Ƚ���� ���

	gotoxy(62 ,5); cout << "stage 1 - bestMinCount : " << Model::stage1_bestCount << "  bestPlayer : " << Model::stage1_bestPlayer;
	gotoxy(62, 6); cout << "stage 2 - bestMinCount : " << Model::stage2_bestCount << "  bestPlayer : " << Model::stage2_bestPlayer;
	gotoxy(62, 7); cout << "stage 3 - bestMinCount : " << Model::stage3_bestCount << "  bestPlayer : " << Model::stage3_bestPlayer;
}

void View::printMap() {
	textcolor(WHITE); 	

	Model::clearCondition();

	if (Model::stage < 3)	//���� �� CLEAR���� ������ �� /////////////////////���߿� �� ���� ������ ���� �ʿ���� ���� �̹� flag�������� ������ !
	{
		
		gotoxy(15, 11);
		for (int i = 0; i < ROW; i++)
		{
			for (int j = 0; j < COLS; j++)
			{
				cout << Model::character[Model::map[Model::stage][i][j]];	//	�� ���
			}
			cout << endl;
			gotoxy(15, 10 + i);
		}
		Model::getBoxCountInHome();	//���� ����ִ� ���� ���ڸ� ������ִ� �Լ� ȣ��
	}
}
void View::previousMap()	//���� �ܰ� ���� ����ϱ� ���� �Լ�
{
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
	if (Model::stage > 0)	//stage1 ���� �������� ������ �� �ƹ� ��ȭ�� ���� �ϱ� ���Ͽ�
	{
		Model::stage--;
		for (int i = 0; i < ROW; i++)
		{
			for (int j = 0; j <COLS; j++)
			{
				Model::map[Model::stage][i][j] = Model::registMap[Model::stage][i][j];			//stage�� �°� �ʱ� ���� �����س��Ҵٰ� �ٿ��ִ� ����.
			}
		}
		Model::count = 0;
	}
}
void View::nextMap()	//���� �ܰ� ���� ����ϱ� ���� �Լ�
{
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
	if (Model::stage < 2)		//stage3 ���� ���� ���� ������ �� �ƹ� ��ȭ�� ���� �ϱ� ���Ͽ�
	{
		Model::stage++;	// ���� �ܰ� ���� ����ϱ� ���� stage����
		for (int i = 0; i < ROW; i++)
		{
			for (int j = 0; j <COLS; j++)
			{
				Model::map[Model::stage][i][j] = Model::registMap[Model::stage][i][j];			//stage�� �°� �ʱ� ���� �����س��Ҵٰ� �ٿ��ִ� ����.
			}
		}
		Model::count = 0;
	}
}
void View::rankingView() {
	Model::screenClean(); //ȭ�� �ʱ�ȭ.

	system("mode con cols=70 lines=20");

	gotoxy(17, 6);
	cout << " �� ŷ �� ��";
	gotoxy(10,10);
	cout << "stage 1 - bestMinCount : " <<Model::stage1_bestCount <<"  bestPlayer : " << Model::stage1_bestPlayer;
	gotoxy(10, 12);
	cout << "stage 2 - bestMinCount : " << Model::stage2_bestCount << "  bestPlayer : " << Model::stage2_bestPlayer;
	gotoxy(10, 14);
	cout << "stage 3 - bestMinCount : " << Model::stage3_bestCount << "  bestPlayer : " << Model::stage3_bestPlayer;

	gotoxy(6,18);
	cout << "ESCŰ�� ������ ����ȭ������ ���ư��ϴ�.";

	while (Model::isMenuSelected) {
		switch (Model::getKey())
		{
		case ESCKEY:  View::mainScreen(); Model::isMenuSelected = false; break;
		}
	}
}

void View::mapDataCheck() { ///stack ������ �� ������ Ȯ���ϴ� �޼ҵ�
	Model::screenClean(); //ȭ�������
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
		Model::mapStack.pop(); // ���ְ�
	}
	if(Model::mapStack.empty()) { //stack ��  �� ���
		cout << "stack is empty." << endl;
	}
	cout << endl;
	cout << "ESCŰ�� ������ ����ȭ������ ���ư��ϴ�.";

	while (Model::isMenuSelected) {
		switch (Model::getKey())
		{
		case ESCKEY:  View::mainScreen(); Model::isMenuSelected = false; break;
		}
	}
}

void View::adviceView() {
	Model::screenClean(); //ȭ�������

	cout << endl;
	cout << "ESCŰ�� ������ ����ȭ������ ���ư��ϴ�.";

	while (Model::isMenuSelected) {
		switch (Model::getKey())
		{
		case ESCKEY:  View::mainScreen(); Model::isMenuSelected = false; break;
		}
	}
}
void View::clearScreen() {
	Model::screenClean(); //ȭ�������

	gotoxy(30, 30);
	cout << "stage clear";
}