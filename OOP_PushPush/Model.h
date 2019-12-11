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
	static int currentMenu; //현재 커서가 가리키는 방향
	static bool isGameSelected; //게임시작이 선택됬을때 true , 아니라면 false
	static bool isMenuSelected; // 메뉴가 선택됬을때 true , 아니라면 false
	static int currentMap; // 맵 선택창에서 선택된 map
	static bool isMapSelected; // 맵이 선택되면 메뉴화면에서 게임화면으로 전환.
	static bool isEndFlag; //종료버튼눌렸는지 flag
	static bool isGameClear; //게임이 종료 되었는지
	static int count; //유저가 이동한 횟수 카운트
	static int stage; //stage 정보
	static int playerX;// 플레이어의 x 좌포
	static int playerY;  // 플레이어의 y 좌표
	static int boxCount[3]; //맵당 box의 개수
	static int boxCount_inHome; //홈에 들어간 box의 개수.
	static int map[STAGE][ROW][COLS];	//맵의 정보를 저장하는 map
	static int registMap[STAGE][ROW][COLS];	//각 stage의 단계별 초기맵.
	static char character[ROW][COLS]; // 벽 , 통로 , 플레이어 , 집 ... 등 캐릭터문자열을 저장. const로 1차원배열해도 되는데 귀찮음.그냥 크기줘서 2차원배열 해버리장 ㅎ
	static stack<int(*)[COLS]> mapStack;  //바뀐 맵 정보 배열의 주소값을 저장한다
	static int rollbackChangeMap[ROW][COLS]; //바뀐 맵 정보를 저장하는 배열.
	static int rollbackChangeMap_2[ROW][COLS]; //바뀐 맵 정보를 저장하는 배열.
	static int rollbackChangeMap_3[ROW][COLS]; //바뀐 맵 정보를 저장하는 배열.
	static int rollbackChangeMap_4[ROW][COLS]; //바뀐 맵 정보를 저장하는 배열.
	static int rollbackChangeMap_5[ROW][COLS]; //바뀐 맵 정보를 저장하는 배열.
	static int saveMap[ROW][COLS]; //맵의 정보 세이브 할 2차원 배열.
	static int currentStage; //맵 선택할때 몇 stage 인지. 초기값은일단 stage 1 
	static void moveLeftRight(int key); //맵선택할때 양옆으로 이동가능하게.
	static int stage1_bestCount; //각 라운드별 최고 점수
	static int stage2_bestCount;
	static int stage3_bestCount;
	static bool isStage1[3];
	static bool isStage2[3];
	static bool isStage3[2];
	static string stage1_bestPlayer; //각 라운드별 최고점수 기록한 플레이어.
	static string stage2_bestPlayer;
	static string stage3_bestPlayer;
	static int pushCount; //  stack에 넣을떄마다 count. 5개 단위로 저장. 새로들어오면 알아서 지워주자.
	static int popCount; // 꺼낼때마다 count
	static void selectMapScreen(); //맵 선택창 이동
	static void selectMap(); //맵 선택
	static void pushChangedMap(); // 바뀐 맵의 정보를 스택에 저장해주는 함수.
	static void popChangedMap(); //롤백 눌렸을때 스택에서 바로 이전 맵 정보를 꺼내서 이전으로 돌아가는 함수.
	static void clearCondition(); // 클리어조건
	static int getBoxCountInHome(); //boxCount_inHome값을 반환하는 함수. 
	static void screenClean();//화면 클리어.
	static void playerLocation(); //플레이어의 x,y 좌표를 불러오는.
	static void writeRanking(); // 랭킹정보 저장하기..
	static void readRanking();	//랭킹을 출력하는 함수
	static void save(); // map save 하는 함수.
	static void load(); //save 한 map load 함수.
	static bool isSave; //true이면 load가능 메인으로 나가거나 stage클리어시 false로 바꿈 save키 누르면 true load키 누르면 false
	static int getKey();
};

