// Note : bullet에 lifetime 변수를 집어넣고 모든 경우의 수에서 bullet을 0으로 초기화 하지 않아서 문제가 발생했다.
// 아마 그 crash가 안없어지는거 10점 그게 안없어지는것도 같은 맥략에서 오는 것일지도 모르니 잘 생각해서 오류를 고쳐보도록 해 보자.
// omok.c 오목의 돌의 움직임을 표현
#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>


#include<MMSystem.h> //브금재생할때 인클루드하는거



// 색상 정의
#define BLACK	0
#define BLUE1	1
#define GREEN1	2
#define CYAN1	3
#define RED1	4
#define MAGENTA1 5
#define YELLOW1	6
#define GRAY1	7
#define GRAY2	8
#define BLUE2	9
#define GREEN2	10
#define CYAN2	11
#define RED2	12
#define MAGENTA2 13
#define YELLOW2	14
#define WHITE	15

#define STAR '*'
#define BLANK ' '

#define ESC 0x1b

#define SPECIAL1 0xe0 // 특수키는 0xe0 + key 값으로 구성된다.
#define SPECIAL2 0x00 // keypad 경우 0x00 + key 로 구성된다.

#define UP  0x48 // Up key는 0xe0 + 0x48 두개의 값이 들어온다.
#define DOWN 0x50
#define LEFT 0x4b
#define RIGHT 0x4d

#define YES		'y'
#define NO	    'n'

#define UP2		'w'
#define DOWN2	's'
#define LEFT2	'a'
#define RIGHT2	'd'
#define SPACE 0x20

#define MAXBULLET 60

#define MAXENEMY 40





struct {
	BOOL exist;
	int x, y;
	int oldx, oldy;
}Bullet[MAXBULLET];

struct {
	BOOL exist;
	int dying;
	int x, y;
	int frame;
	int stay;
	int havedied;
	int timeafterdead;
} Enemytemp[MAXENEMY];

struct {
	BOOL exist;
	int dying;
	int x, y;
	int type;
	int frame;
	int stay;
	int havedied;
	int timeafterdead;
} Itemtemp[7];

struct { //참고용 구조체(실작동 x)
	BOOL exist;
	int type;
	int x, y;
	int nFrame;
	int nStay;
} Enemy[MAXENEMY];

char *arrEnemy[] = {
	"(■)"
}; //참고용 배열(실작동 X)

int Delay = 35;
int mainpx = 35;
int bulletx = -1;
int pullety = -1;
int bullet = 0;
int numofbullets = 0;
int hardness = 51;
double life = 5.;
double score = 0;
int enemynum = 10;
int frame = 10;
double addscore = 0.01;

int islooped = 0;

int israpid = 0;
int rapidtime = 0;

int ishoming = 0;
int homingtime = 0;

int ispierce = 0;
int piercetime = 0;


void removeCursor(void) { // 커서를 안보이게 한다

	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

void gotoxy(int x, int y) //내가 원하는 위치로 커서 이동
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);// WIN32API 함수입니다. 이건 알필요 없어요
}

void erasestar(int x, int y)
{
	gotoxy(x, y);
	//putchar(' ');//(BLANK);
	//printf("  ");
	printf("┼");
}

void SetColor(int fg_color, int bg_color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), fg_color | bg_color << 4);
}

void cls(int bg_color, int text_color)
{
	char cmd[100];
	system("cls");
	sprintf(cmd, "COLOR %x%x", bg_color, text_color);
	system(cmd);

}
// 화면 지우기고 원하는 배경색으로 설정한다.

void VertLine() {
	int i;

	for (i = 0; i < 40; i++) {
		SetColor(WHITE, 0);
		gotoxy(70, i);
		printf("│");
	}
}

void LoadMain() {
	int text_title[2][5][27] = {

		0,0,0,0,0,1,1,0,1,1,1,0,0,1,0,0,0,1,1,0,1,1,1,0,0,0,0,
		0,0,0,0,1,0,0,0,1,0,1,0,1,0,1,0,1,0,0,0,1,0,0,0,0,0,0,
		0,0,0,0,1,1,1,0,1,1,0,0,1,1,1,0,1,0,0,0,1,1,1,0,0,0,0,
		0,0,0,0,0,0,1,0,1,0,0,0,1,0,1,0,1,0,0,0,1,0,0,0,0,0,0,
		0,0,0,0,1,1,0,0,1,0,0,0,1,0,1,0,0,1,1,0,1,1,1,0,0,0,0,

		0,1,1,0,1,0,1,0,0,1,0,0,0,1,0,0,1,1,1,0,1,1,1,0,1,1,0,
		1,0,0,0,1,0,1,0,1,0,1,0,1,0,1,0,0,1,0,0,1,0,0,0,1,0,1,
		1,1,1,0,1,1,1,0,1,0,1,0,1,0,1,0,0,1,0,0,1,1,1,0,1,1,0,
		0,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,0,1,0,0,1,0,0,0,1,0,1,
		1,1,0,0,1,0,1,0,0,1,0,0,0,1,0,0,0,1,0,0,1,1,1,0,1,0,1,

	};

	gotoxy(8, 5);
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 27; j++) {
			if (text_title[0][i][j] == 0) {
				printf("  ");
			}
			else {
				printf("■");

			}
			Sleep(3);
		}
		printf("\n");
		gotoxy(8, 6 + i);
	}

	gotoxy(27, 11);

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 27; j++) {
			if (text_title[1][i][j] == 0) {
				printf("  ");
			}
			else {
				printf("■");

			}
			Sleep(3);
		}
		printf("\n");
		gotoxy(27, 12 + i);
	}

	gotoxy(63, 7); printf("ver. 1.0");
	gotoxy(30, 19); printf("┌───<    Instructions    >───┐");
	gotoxy(30, 20); printf("│                                  │");
	gotoxy(30, 21); printf("│    \"Just Click Space to PLAY\"    │");
	gotoxy(30, 22); printf("│                                  │");
	gotoxy(30, 23); printf("│      <     I T E M S      >      │");
	gotoxy(30, 24); printf("│                                  │");
	gotoxy(30, 25); printf("│    BLUE Diamond =   Rapid Fire   │");
	gotoxy(30, 26); printf("│  Yellow Diamond = Homing Missile │");
	gotoxy(30, 27); printf("│   Green Diamond =  Armor Piercer │");
	gotoxy(30, 28); printf("│  White Diammond =  Clear field   │");
	gotoxy(30, 29); printf("│                                  │");
	gotoxy(30, 30); printf("└─────────────────┘");

	gotoxy(37, 34);
	printf("Press Any Key to Start");
	_getch();


}

void LoadEnd() {
	int text_title[2][5][27] = {

		0,0,0,0,0,0,1,1,0,0,0,1,0,0,1,0,1,0,1,1,1,0,0,0,0,0,0,
		0,0,0,0,0,1,0,0,0,0,1,0,1,0,1,1,1,0,1,0,0,0,0,0,0,0,0,
		0,0,0,0,0,1,0,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,0,0,0,0,0,
		0,0,0,0,0,1,0,0,1,0,1,0,1,0,1,0,1,0,1,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,1,1,0,0,1,0,1,0,1,0,1,0,1,1,1,0,0,0,0,0,0,

		0,0,0,0,0,0,1,1,0,0,1,0,1,0,1,1,1,0,1,1,0,0,0,0,0,0,0,
		0,0,0,0,0,1,0,0,1,0,1,0,1,0,1,0,0,0,1,0,1,0,0,0,0,0,0,
		0,0,0,0,0,1,0,0,1,0,1,0,1,0,1,1,1,0,1,1,0,0,0,0,0,0,0,
		0,0,0,0,0,1,0,0,1,0,1,0,1,0,1,0,0,0,1,0,1,0,0,0,0,0,0,
		0,0,0,0,0,0,1,1,0,0,0,1,0,0,1,1,1,0,1,0,1,0,0,0,0,0,0,

	};

	gotoxy(20, 5);
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 27; j++) {
			if (text_title[0][i][j] == 0) {
				printf("  ");
			}
			else {
				printf("■");

			}
			Sleep(1);
		}
		printf("\n");
		gotoxy(20, 6 + i);
	}

	gotoxy(20, 30);

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 27; j++) {
			if (text_title[1][i][j] == 0) {
				printf("  ");
			}
			else {
				printf("■");

			}
			Sleep(1);
		}
		printf("\n");
		gotoxy(20, 31 + i);
	}

	

}

void DrawPlayer() {
	SetColor(GREEN1, 0);
	gotoxy(mainpx, 37);
	printf("▣");
}

void ErasePlayer() {
	gotoxy(mainpx, 37);
	printf(" ");
}

void MovePlayer()
{
	gotoxy(mainpx, 37); //꼬리좌표
	ErasePlayer();

	DrawPlayer();

}

void DrawBullet(int i) { 

	if (Bullet[i].exist == TRUE) {

		if (israpid == 1) {

			SetColor(BLUE1, 0);

		}
		else if (ishoming == 1) {

			SetColor(YELLOW1, 0);

		}
		else if (ispierce == 1) {

			SetColor(GREEN2, 0);

		}
		else {

			SetColor(YELLOW2, 0);

		}

		if (ispierce == 1) {
			gotoxy(Bullet[i].x, Bullet[i].y - 2);
			printf("▲");

			gotoxy(Bullet[i].x - 2, Bullet[i].y - 1);
			printf("■");

			gotoxy(Bullet[i].x, Bullet[i].y - 1);
			printf("□");

			gotoxy(Bullet[i].x + 2, Bullet[i].y - 1);
			printf("■");
		}
		else {
			gotoxy(Bullet[i].x, Bullet[i].y);
			printf("●");
		}

	}

	// 총알 디버깅용
	//gotoxy(Bullet[i].x, Bullet[i].y+1);
	//printf("x = %d y = %d", Bullet[i].x, Bullet[i].y);
}

void EraseBullet(int i) { //지금 필요한 것 : 호밍 상태일 때 bullet의 이전 포지션을 기억한 후 이전 포지션과 현재 포지션에 가서 둘다 지워줘야 함.
	if (ispierce == 1) {

		
		gotoxy(Bullet[i].oldx, Bullet[i].oldy-1);
		printf("  ");

		gotoxy(Bullet[i].oldx-2, Bullet[i].oldy);
		printf("  ");

		gotoxy(Bullet[i].oldx, Bullet[i].oldy);
		printf("  ");

		gotoxy(Bullet[i].oldx+2, Bullet[i].oldy);
		printf("  ");

		gotoxy(Bullet[i].oldx - 2, Bullet[i].oldy+1);
		printf("  ");

		gotoxy(Bullet[i].oldx, Bullet[i].oldy+1);
		printf("  ");

		gotoxy(Bullet[i].oldx + 2, Bullet[i].oldy+1);
		printf("  ");


	}
	else {

		gotoxy(Bullet[i].x, Bullet[i].y + 1);
		printf("  ");
		gotoxy(Bullet[i].oldx, Bullet[i].oldy);
		printf("  ");

	}
}


/*
void MoveBullet() {
	int i, j;
	for (i = 0; i < MAXBULLET; i++) {
		if (Bullet[i].exist == TRUE) {
			EraseBullet(i);

			if (Bullet[i].y == 0) {
				Bullet[i].exist = FALSE;
				gotoxy(Bullet[i].x, Bullet[i].y);
				printf("  ");
				gotoxy(Bullet[i].x, Bullet[i].y + 1);
				printf("  ");

			}
			else {
				if (ishoming == 0) { //호밍상태가 아닐 때
					Bullet[i].y--;
					DrawBullet(i);
				}
				else { //호밍상태일 때
					
					for (j = 0;j < MAXENEMY; j++) {

						if (abs(Bullet[i].x - Enemytemp[j].x) < 5 && abs(Bullet[i].y - Enemytemp[j].y) < 3 && Enemytemp[j].exist == TRUE) { //호밍을 적용시킬 수 있는 상태(충분히 접근한 상태)일 때
							
							
							if (Bullet[i].x <Enemytemp[i].x) {
								Bullet[i].x++;
								
							}
							else if (Bullet[i].x > Enemytemp[i].x) {
								Bullet[i].x--;
								
							}
							else {
								;
								
							}

							if (Bullet[i].y > Enemytemp[i].y) {
								Bullet[i].y--;
								
							}
							else if (Bullet[i].y < Enemytemp[i].y) {
								Bullet[i].y++;
								
							}
							else {
								Bullet[i].y--;
								
							}
							

							Bullet[i].y--;

						}else {
							Bullet[i].y--;
							
						}
					}



					DrawBullet(i);




				}
			}
		}
	}
}

*/
//망한 homing소스

void MoveBullettemp() {
	int i;
	for (i = 0; i < MAXBULLET; i++) {
		if (Bullet[i].exist == TRUE) {
			EraseBullet(i);

			if (Bullet[i].y == 0) {
				Bullet[i].exist = FALSE;
				gotoxy(Bullet[i].x, Bullet[i].y);
				printf("  ");
				gotoxy(Bullet[i].x, Bullet[i].y + 1);
				printf("  ");

			}
			else {
				Bullet[i].y--;
				DrawBullet(i);
			}
		}
	}
}
//호밍 망했을때 movebullet복구용

void MoveBullet() {
	int i, i2, j;
	for (i = 0; i < MAXBULLET; i++) {


		if (Bullet[i].exist == TRUE) { //움직이기 전에 현재 위치에 있는놈 지우기
			EraseBullet(i);

			if (Bullet[i].y - 2 == 0 && ispierce == 1) { //움직이다 뒤지는 소스(피어싱)

				Bullet[i].exist = FALSE;

				gotoxy(Bullet[i].x, Bullet[i].y - 3);
				printf("  ");

				gotoxy(Bullet[i].x - 2, Bullet[i].y - 2);
				printf("  ");

				gotoxy(Bullet[i].x, Bullet[i].y - 2);
				printf("  ");

				gotoxy(Bullet[i].x + 2, Bullet[i].y - 2);
				printf("  ");

				gotoxy(Bullet[i].oldx - 2, Bullet[i].oldy-1);
				printf("  ");

				gotoxy(Bullet[i].oldx, Bullet[i].oldy-1);
				printf("  ");

				gotoxy(Bullet[i].oldx + 2, Bullet[i].oldy-1);
				printf("  ");
				

			}



			if (Bullet[i].y == 0) { //움직이다 뒤지는 소스


				Bullet[i].exist = FALSE;
				gotoxy(Bullet[i].x, Bullet[i].y);
				printf("  ");
				gotoxy(Bullet[i].x, Bullet[i].y + 1);
				printf("  ");

			}
			else { //움직이다 뒤지는 게 아닐 경우
				if (ishoming == 1 && ispierce == 0) { //호밍중일 때

					for (j = 0; j < MAXENEMY; j++) {
						if (abs(Bullet[i].x - Enemytemp[j].x) < 10 && abs(Bullet[i].y - Enemytemp[j].y) < 11 && Enemytemp[j].exist == TRUE) { //호밍을 적용시킬 수 있는 상태(충분히 접근한 상태)일 때

							if (Bullet[i].x < Enemytemp[j].x) {
								Bullet[i].x++;

							}
							else if (Bullet[i].x > Enemytemp[j].x) {
								Bullet[i].x--;

							}
							else {
								;

							}

						}

					}
					Bullet[i].y--;
					DrawBullet(i);
					EraseBullet(i);
					Bullet[i].oldx = Bullet[i].x;
					Bullet[i].oldy = Bullet[i].y;

				}
				else { //호밍중이 아닐 때
					Bullet[i].y--;
					DrawBullet(i);
					EraseBullet(i);
					Bullet[i].oldx = Bullet[i].x;
					Bullet[i].oldy = Bullet[i].y;

				}
			}
		}
	}
}
		
	

			




void DrawEnemy() {

	// 현재 문제
	// 적이 땅에 닿아서 파괴되면 즉각 위에서 재생성이 되는데,
	// 적이 총알에 닿아서 파괴되면 위에서 바로 생성되지 않고 좀 기다려야 생성됨.

	int i;

	if (rand() % 300 < enemynum) { //rand()%350의 값이 enemynum보다 적은 범위에 있으면, 즉 enemynum/300의 확률로, 새 적이 추가됨.
		for (i = 0; i <= MAXENEMY && Enemytemp[i].exist == TRUE && Enemytemp[i].dying == 1; i++) {
			; // 8마리 생성된 상태라면 i가 9가 됨. 그런 식이지 뭐 이게. 그리고 중간에 exist가 false인 애를 만나도(빈 공간을 찾아도) 빠져나온다.
		}

		if (i == MAXENEMY) { //MAXENEMY를 넘어서면 다시 i를 0으로 바꿔서 다시 출력시킴. 뭐 겹치니 어떻니 할 수도 있는데 그 전에 이미 아래로 꼬라박아서 없어질걸?
			i = 0;
		}

		if (i < MAXENEMY && Enemytemp[i].exist == FALSE) { //위에서 받아서, 예를들어 i가 9인데 9가 MAXENEMY보다 적고 죽어서 배열에 자리가 빈 상태라면

			Enemytemp[i].x = (rand() % 22) * 3;
			Enemytemp[i].y = 0;

			Enemytemp[i].frame = Enemytemp[i].stay = frame; //frame은 10.
			Enemytemp[i].timeafterdead = 0;
			Enemytemp[i].exist = TRUE;
			Enemytemp[i].havedied = 0;
			Enemytemp[i].dying = 0;
			
			SetColor(RED1, 0);
			gotoxy(Enemytemp[i].x, 0);
			printf("▼");

		}

	}
}

void MoveEnemy() {

	// stay 멤버변수를 만들어서 그것이 특정 값에서 1씩 감소하다가
	// 나중에 0이 되버리면 그때마다 한 칸씩 이동하게 만드는 것으로 딜레이와 속도를 조절하면 어떨까나?
	// 아, 그리고 stay가 0이 되면 다시 특정 값으로 되돌려줘야 하니까 frame이라는 변수를 새로 만들어서 stay가 0이 될 떄마다 재조정을 해 주자.

	// 현재 문제점 : enemy가 y좌표 40 이상으로 내려가서 땅에 닿으면 사라지지 않고 버그가 걸림.
	// 그리고 새로운 enemy가 MAXENEMY 이상으로 출력되지 않음.
	// 그리고 위에 새로운 친구가 생기면 y=0부터 떨어지지 않고 거기 잔상이 남은 후 수직으로 밑에 인접한 친구 옆/앞/뒤에 즉시 붙어서 앵김.
	// 죽은 후에 죽은 위치에 가서 지워야 하는데 총알 위치로 가서 적이 죽은 메시지가 출력된다.
	//------------해결-------------


	// 이제는, 맨 왼쪽에 붙어서 오는 친구가 죽는 처리가 안되는것좀 봐야하고
	// 그리고 좀 enemy끼리 붙어서 안 오면 안되는거니?
	//------------해결-------------


	// 또, 아주 가끔씩 총알 쐇는데 enemy가 안죽는다.
	// 아주 가끔식 erasecrash()가 안먹는다.
	//-----------시벌 내가 이거 다시는 해결하려고 하나 봐라------------

	int i;

	for (i = 0; i < MAXENEMY; i++) {
		if (Enemytemp[i].exist == FALSE) {


			continue; //Enemytemp[i]가 exist하지 않으므로 통째로 스킵한다.
		}
		if (Enemytemp[i].stay == 0) { //stay가 0이 아니면 한 턴 쉬고 스킵한다. 그리고 1 줄여둠.
			Enemytemp[i].stay = Enemytemp[i].frame; //stay가 0이 되면 진입하며, stay는 다시 frame과 동기화 시켜둔다.

			if (Enemytemp[i].y > 38) { //y축기준 38을 넘어버리면 몹 죽는거지 뭐.


				if (Enemytemp[i].x > 2) { //일반적인 경우
					gotoxy(Enemytemp[i].x - 2, Enemytemp[i].y); //죽으면 죽은 위치 가서
					SetColor(BLUE1, 0);
					printf("crashed"); // 지우거나 죽은 흔적을 남기거나.
					life-=0.5;
					PlaySound("lostlife.wav", NULL, SND_ASYNC);
				}
				else { // 벽에 붙어서 올 경우
					gotoxy(0, Enemytemp[i].y); //죽으면 죽은 위치 가서
					SetColor(BLUE1, 0);
					printf("crashed"); // 지우거나 죽은 흔적을 남기거나.
					life-=0.5;
					PlaySound("lostlife.wav", NULL, SND_ASYNC);
				}


				// 3프레임 뒤에 crashed 지우는 소스/////////////////////////////////////////////////////////

				Enemytemp[i].exist = FALSE; //적 죽은거로 판정하자.
				Enemytemp[i].havedied = 1;
				if (Enemytemp[i].exist == FALSE && Enemytemp[i].timeafterdead < 2 && Enemytemp[i].havedied == 1) {
					Enemytemp[i].timeafterdead++;
				}
				if (Enemytemp[i].timeafterdead == 2) {

					if (Enemytemp[i].x > 2) { //일반적인 경우
						gotoxy(Enemytemp[i].x - 2, Enemytemp[i].y); //죽으면 죽은 위치 가서
						printf("         "); // 지우거나 죽은 흔적을 남기거나.
					}
					else { // 벽에 붙어서 올 경우
						gotoxy(0, Enemytemp[i].y); //죽으면 죽은 위치 가서
						printf("         "); // 지우거나 죽은 흔적을 남기거나.
					}

					Enemytemp[i].x = (rand() % 22) * 3; //위치 초기화
					Enemytemp[i].y = 0; //위치 초기화
					Enemytemp[i].timeafterdead = 0; //time 초기화
					Enemytemp[i].havedied = 0; //havedied 초기화
					Enemytemp[i].dying = 0;
				}

				/////////////////////////////////////////////////////////////////////////////////////////////////

			}
			else { // y축 기준 40을 넘지 않으면(화면 내에 있으면)
				Enemytemp[i].y++; // 해당 적군의 [y좌표를 1 증가]시키자.//////////////////////////////////////////////////////위치변경

				if (Enemytemp[i].x < 3) { //enemy가 왼쪽 벽에 붙었을 경우
					gotoxy(0, Enemytemp[i].y - 1); // y좌표 1 증가시키기 전 위치로 가서
					printf("  "); //빈공간으로 만들어서 준비해놓자.
				}
				else {
					gotoxy(Enemytemp[i].x, Enemytemp[i].y - 1); // y좌표 1 증가시키기 전 위치로 가서
					printf("  "); //빈공간으로 만들어서 준비해놓자.
				}
			}
			SetColor(RED1, 0); //색 바꿔두고

			if (Enemytemp[i].exist == TRUE) { //아니 살아있어야 뭘 할 것 아닌가?

				gotoxy(Enemytemp[i].x, Enemytemp[i].y); //y좌표 1 증가된 곳으로 가서
				printf("▼"); //적군 다시 출력하자(이동시키자).
			}
			else {
				Enemytemp[i].exist = TRUE; // 죽은 bullet을 재사용하자.
			}
		}

		Enemytemp[i].stay--; //stay를 줄여야 언젠가 0이돼서 움직일거 아닌가

	}

}

void EnemyCollide() {

	//참고 : 유니티가 아니다. 한 프레임 안에 모든 행동들을 구현해야 한다는 걸 잊지 말자.

	// 또, 아주 가끔씩 총알 쐈는데 enemy가 안죽는다. 왜그러냐?
	// 이 경우에는 강제로 죽게 만들어도 다음 프레임에 재생성돼서 또 내려온다.
	// 쐈는데 안죽는경우는 같은 자리에 두개 이상의 개체가 동시에 생성되어서 그런 것이다.
	// clearfield 등을 사용한 후에 종종 발생하는 것을 봐서 그럼에 확실하다.
	// enemy가 동일한 장소에서 나오지 않도록 하면 될 텐데, 화면 너비가 좁아서 그렇게 하기도 애매하다.

	int i, j;

	for (i = 0; i < MAXENEMY; i++) { //enemy 세기
		if (Enemytemp[i].exist == FALSE) {
			continue; //Enemytemp[i]가 exist하지 않으므로 통째로 스킵한다.
		}

		for (j = 0; j < MAXBULLET; j++) { //bullet 세기

			if (ispierce == 0) { //ispierce가 아닌 경우

				if (abs(Enemytemp[i].x - Bullet[j].x) <= 2 && abs(Enemytemp[i].y - Bullet[j].y) <= 1 && Bullet[j].exist == TRUE) { //총알과 적의 y과표가 같고, x좌표는 비슷하며, 총알이 유효판정일 경우
					if (Enemytemp[i].x > 2) { // 일반적인 경우


						Bullet[j].exist = FALSE; //총알도 죽은거로 판정하자.
						gotoxy(Bullet[j].x, Bullet[j].y);
						printf("  ");
						gotoxy(Bullet[j].x, Bullet[j].y + 1);
						printf("  ");


						gotoxy(Enemytemp[i].x - 2, Enemytemp[i].y); //죽으면 죽은 위치 가서
						SetColor(BLUE1, 0);
						printf("+10점"); // 지우거나 죽은 흔적을 남기거나.
						PlaySound("enemydead.wav", NULL, SND_ASYNC);
						score += 10;

					}
					else { // 벽에 붙어서 오는 경우



						Bullet[j].exist = FALSE; //총알도 죽은거로 판정하자.
						gotoxy(Bullet[j].x, Bullet[j].y);
						printf("  ");
						gotoxy(Bullet[j].x, Bullet[j].y + 1);
						printf("  ");

						gotoxy(0, Enemytemp[i].y); //죽으면 죽은 위치 가서
						SetColor(BLUE1, 0);
						printf("+10점"); // 지우거나 죽은 흔적을 남기거나.
						PlaySound("enemydead.wav", NULL, SND_ASYNC);
						score += 10;


					}



					Enemytemp[i].exist = FALSE; //적 죽은거로 판정하자.
					Enemytemp[i].havedied = 1;
					Enemytemp[i].dying = 1;
					Enemytemp[i].timeafterdead = 0;

					//gotoxy(Enemytemp[i].x, Enemytemp[i].y + 1); //죽으면 죽은 위치 가서
					//printf("time %d, isdead %d, dying %d, Exist %d, stay %d frame %d  ", Enemytemp[i].timeafterdead, Enemytemp[i].havedied, Enemytemp[i].dying, Enemytemp[i].exist, Enemytemp[i].stay, Enemytemp[i].frame); // 지우거나 죽은 흔적을 남기거나.


				}
							   
			}
			else { //ispierce인 경우

				if (abs(Enemytemp[i].x - Bullet[j].x) <= 6 && abs(Enemytemp[i].y - Bullet[j].y) <= 3 && Bullet[j].exist == TRUE) { //총알과 적의 y과표가 같고, x좌표는 비슷하며, 총알이 유효판정일 경우
					if (Enemytemp[i].x > 2) { // 일반적인 경우

						PlaySound("enemydead.wav", NULL, SND_ASYNC);

						gotoxy(Enemytemp[i].x - 2, Enemytemp[i].y); //죽으면 죽은 위치 가서
						SetColor(BLUE1, 0);
						printf("+10점"); // 지우거나 죽은 흔적을 남기거나.
						score += 10;

					}
					else { // 벽에 붙어서 오는 경우

						PlaySound("enemydead.wav", NULL, SND_ASYNC);

						gotoxy(0, Enemytemp[i].y); //죽으면 죽은 위치 가서
						SetColor(BLUE1, 0);
						printf("+10점"); // 지우거나 죽은 흔적을 남기거나.

						score += 10;


					}

					Enemytemp[i].exist = FALSE; //적 죽은거로 판정하자.
					Enemytemp[i].havedied = 1;
					Enemytemp[i].dying = 1;
					Enemytemp[i].timeafterdead = 0;

					//gotoxy(Enemytemp[i].x, Enemytemp[i].y + 1); //죽으면 죽은 위치 가서
					//printf("time %d, isdead %d, dying %d, Exist %d, stay %d frame %d  ", Enemytemp[i].timeafterdead, Enemytemp[i].havedied, Enemytemp[i].dying, Enemytemp[i].exist, Enemytemp[i].stay, Enemytemp[i].frame); // 지우거나 죽은 흔적을 남기거나.


				}

			}
		}

	}

}

void erasecrash() { // '총맞아 죽은' enemy에서 나온 crashed 제거기
	int i;
	for (i = 0; i < MAXENEMY; i++) { //enemy 찾기
		if (Enemytemp[i].exist == FALSE && Enemytemp[i].timeafterdead < 11 && Enemytemp[i].havedied == 1 && Enemytemp[i].dying == 1) { //뒤진 상태면
			Enemytemp[i].timeafterdead++;
			Enemytemp[i].timeafterdead++;//time 1씩 증가시키기.

			
			//gotoxy(Enemytemp[i].x, Enemytemp[i].y+1); //죽으면 죽은 위치 가서
			//printf("time %d, isdead %d, dying %d, Exist %d, stay %d frame %d  ", Enemytemp[i].timeafterdead, Enemytemp[i].havedied, Enemytemp[i].dying, Enemytemp[i].exist, Enemytemp[i].stay, Enemytemp[i].frame); // 지우거나 죽은 흔적을 남기거나.
			
			// 디버깅 결과, 안지워지는 련들은 timeafterdead가 증가하다가 4~6 사이에서 별안간 멈추는 것을 확인할 수 있었다.
			//
			//exist가 FALSE인 상태에서 erasecrash 하다가 drawenemy에서 exist 상태를 변경한 것일 수도 있음.
			//
			// 이를 토대로 화요일날 한번 해결해 보도록 하자.
		}

		if (Enemytemp[i].timeafterdead >= 10) { //뒤지고 10만큼 지나면 crashed 지우기.

			if (Enemytemp[i].x > 2) { //일반적인 경우
				gotoxy(Enemytemp[i].x - 2, Enemytemp[i].y); //죽으면 죽은 위치 가서
				printf("         "); // 지우거나 죽은 흔적을 남기거나.
			}
			else { // 왼쪽 벽에 붙어서 올 경우
				gotoxy(0, Enemytemp[i].y); //죽으면 죽은 위치 가서
				printf("         "); // 지우거나 죽은 흔적을 남기거나.
			}

			Enemytemp[i].x = (rand() % 22) * 3;
			Enemytemp[i].y = 0;

			Enemytemp[i].x = (rand() % 22) * 3;
			Enemytemp[i].y = 0;

			Enemytemp[i].frame = Enemytemp[i].stay = frame; //frame은 10.
			Enemytemp[i].timeafterdead = 0;
			Enemytemp[i].exist = TRUE;
			Enemytemp[i].havedied = 0;
			//Enemytemp 재사용하는 대신 여기서 초기화시켜서 내보낸다.
		}
	}

}

void clearfield() {

	int i;

	for (i = 0; i < MAXENEMY; i++) {
		if (Enemytemp[i].exist == FALSE) {


			continue; //Enemytemp[i]가 exist하지 않으므로 통째로 스킵한다.
		}

		if (Enemytemp[i].x > 2) { // 일반적인 경우



			gotoxy(Enemytemp[i].x - 2, Enemytemp[i].y); //죽으면 죽은 위치 가서
			SetColor(BLUE1, 0);
			printf("+10점"); // 지우거나 죽은 흔적을 남기거나.
			score += 10;

		}
		else { // 벽에 붙어서 오는 경우


			gotoxy(0, Enemytemp[i].y); //죽으면 죽은 위치 가서
			SetColor(BLUE1, 0);
			printf("+10점"); // 지우거나 죽은 흔적을 남기거나.

			score += 10;


		}

		Enemytemp[i].exist = FALSE; //적 죽은거로 판정하자.
		Enemytemp[i].havedied = 1;
		Enemytemp[i].dying = 1;
		
		Enemytemp[i].timeafterdead = 0;
	}

	PlaySound("enemydead.wav", NULL, SND_ASYNC);

}		/////////////////////////////////////////////////////////////////////////////////////////////////

void clearitematend() {

	int i;

	for (i = 0; i < 7; i++) {
		if (Itemtemp[i].exist == FALSE) {


			continue; //Itemtemp[i]가 exist하지 않으므로 통째로 스킵한다.
		}

		Itemtemp[i].x = (rand() % 22) * 3;
		Itemtemp[i].y = 0;

		Itemtemp[i].frame = Itemtemp[i].stay = frame; //frame은 10.
		Itemtemp[i].timeafterdead = 0;
		Itemtemp[i].exist = FALSE;
	}

}		/////////////////////////////////////////////////////////////////////////////////////////////////

void clearfieldatend() {

	int i;

	for (i = 0; i < MAXENEMY; i++) {
		if (Enemytemp[i].exist == FALSE) {


			continue; //Enemytemp[i]가 exist하지 않으므로 통째로 스킵한다.
		}



		Enemytemp[i].x = (rand() % 22) * 3;
		Enemytemp[i].y = 0;

		Enemytemp[i].x = (rand() % 22) * 3;
		Enemytemp[i].y = 0;

		Enemytemp[i].frame = Enemytemp[i].stay = frame; //frame은 10.
		Enemytemp[i].timeafterdead = 0;
		Enemytemp[i].exist = FALSE;
		Enemytemp[i].havedied = 0;
	}

}


void DrawItem() {

	// 현재 문제
	// 적이 땅에 닿아서 파괴되면 즉각 위에서 재생성이 되는데,
	// 적이 총알에 닿아서 파괴되면 위에서 바로 생성되지 않고 좀 기다려야 생성됨.

	int i;
	int itemtype;

	itemtype = rand() % 7 + 1; //itemtype는 1, 2, 3, 4 중 하나로 결정됨.

	if (rand() % 1000 < 7) { //rand()%350의 값이 Itemnum보다 적은 범위에 있으면, 즉 7/1000의 확률로, 새 아이템이 추가됨.
		for (i = 0; i <= 7 && Itemtemp[i].exist == TRUE && Itemtemp[i].dying == 1; i++) {
			; // 8마리 생성된 상태라면 i가 9가 됨. 그런 식이지 뭐 이게. 그리고 중간에 exist가 false인 애를 만나도(빈 공간을 찾아도) 빠져나온다.
		}

		if (i == 7) { //7를 넘어서면 다시 i를 0으로 바꿔서 다시 출력시킴. 뭐 겹치니 어떻니 할 수도 있는데 그 전에 이미 아래로 꼬라박아서 없어질걸?
			i = 0;
		}

		if (i < 7 && Itemtemp[i].exist == FALSE) { //위에서 받아서, 예를들어 i가 9인데 9가 7보다 적고 죽어서 배열에 자리가 빈 상태라면

			switch (itemtype) {

			case 1:
			case 5:
				Itemtemp[i].x = (rand() % 22) * 3;
				Itemtemp[i].y = 0;

				Itemtemp[i].frame = Itemtemp[i].stay = 4; //frame은 10.
				Itemtemp[i].type = itemtype;
				Itemtemp[i].timeafterdead = 0;
				Itemtemp[i].exist = TRUE;
				Itemtemp[i].havedied = 0;
				Itemtemp[i].dying = 0;


				SetColor(BLUE1, 0);
				gotoxy(Itemtemp[i].x, 0);
				printf("◆");
				break;


			case 2:
			
				Itemtemp[i].x = (rand() % 22) * 3;
				Itemtemp[i].y = 0;

				Itemtemp[i].frame = Itemtemp[i].stay = 4; //frame은 10.
				Itemtemp[i].type = itemtype;
				Itemtemp[i].timeafterdead = 0;
				Itemtemp[i].exist = TRUE;
				Itemtemp[i].havedied = 0;
				Itemtemp[i].dying = 0;


				SetColor(YELLOW1, 0);
				gotoxy(Itemtemp[i].x, 0);
				printf("◆");

				break;


			case 3:
			case 6:
				Itemtemp[i].x = (rand() % 22) * 3;
				Itemtemp[i].y = 0;

				Itemtemp[i].frame = Itemtemp[i].stay = 4; //frame은 10.
				Itemtemp[i].type = itemtype;
				Itemtemp[i].timeafterdead = 0;
				Itemtemp[i].exist = TRUE;
				Itemtemp[i].havedied = 0;
				Itemtemp[i].dying = 0;


				SetColor(GREEN2, 0);
				gotoxy(Itemtemp[i].x, 0);
				printf("◆");

				break;


			case 4:
			case 7:
				Itemtemp[i].x = (rand() % 22) * 3;
				Itemtemp[i].y = 0;

				Itemtemp[i].frame = Itemtemp[i].stay = 4; //frame은 10.
				Itemtemp[i].type = itemtype;
				Itemtemp[i].timeafterdead = 0;
				Itemtemp[i].exist = TRUE;
				Itemtemp[i].havedied = 0;
				Itemtemp[i].dying = 0;


				SetColor(WHITE, 0);
				gotoxy(Itemtemp[i].x, 0);
				printf("◆");

				break;


			default:
				break;




			}



		}

	}
}

void MoveItem() {

	// stay 멤버변수를 만들어서 그것이 특정 값에서 1씩 감소하다가
	// 나중에 0이 되버리면 그때마다 한 칸씩 이동하게 만드는 것으로 딜레이와 속도를 조절하면 어떨까나?
	// 아, 그리고 stay가 0이 되면 다시 특정 값으로 되돌려줘야 하니까 frame이라는 변수를 새로 만들어서 stay가 0이 될 떄마다 재조정을 해 주자.

	// 현재 문제점 : Item가 y좌표 40 이상으로 내려가서 땅에 닿으면 사라지지 않고 버그가 걸림.
	// 그리고 새로운 Item가 7 이상으로 출력되지 않음.
	// 그리고 위에 새로운 친구가 생기면 y=0부터 떨어지지 않고 거기 잔상이 남은 후 수직으로 밑에 인접한 친구 옆/앞/뒤에 즉시 붙어서 앵김.
	// 죽은 후에 죽은 위치에 가서 지워야 하는데 총알 위치로 가서 적이 죽은 메시지가 출력된다.
	//------------해결-------------


	// 이제는, 맨 왼쪽에 붙어서 오는 친구가 죽는 처리가 안되는것좀 봐야하고
	// 그리고 좀 Item끼리 붙어서 안 오면 안되는거니?
	//------------해결-------------



	int i;

	for (i = 0; i < 7; i++) {
		if (Itemtemp[i].exist == FALSE) {


			continue; //Itemtemp[i]가 exist하지 않으므로 통째로 스킵한다.
		}
		if (Itemtemp[i].stay == 0) { //stay가 0이 아니면 한 턴 쉬고 스킵한다. 그리고 1 줄여둠.
			Itemtemp[i].stay = Itemtemp[i].frame; //stay가 0이 되면 진입하며, stay는 다시 frame과 동기화 시켜둔다.

			if (Itemtemp[i].y > 38) { //y축기준 38을 넘어버리면 몹 죽는거지 뭐.


				if (Itemtemp[i].x > 2) { //일반적인 경우
					gotoxy(Itemtemp[i].x, Itemtemp[i].y); //죽으면 죽은 위치 가서
					SetColor(BLUE1, 0);
					printf("  "); // 지우거나 죽은 흔적을 남기거나.
				}
				else { // 벽에 붙어서 올 경우
					gotoxy(Itemtemp[i].x, Itemtemp[i].y); //죽으면 죽은 위치 가서
					SetColor(BLUE1, 0);
					printf("  "); // 지우거나 죽은 흔적을 남기거나.
				}

				// 3프레임 뒤에 crashed 지우는 소스/////////////////////////////////////////////////////////

				Itemtemp[i].exist = FALSE; //적 죽은거로 판정하자.
				Itemtemp[i].havedied = 1;
				if (Itemtemp[i].exist == FALSE && Itemtemp[i].timeafterdead < 2 && Itemtemp[i].havedied == 1) {
					Itemtemp[i].timeafterdead++;
				}
				if (Itemtemp[i].timeafterdead == 2) {

					if (Itemtemp[i].x > 2) { //일반적인 경우
						gotoxy(Itemtemp[i].x , Itemtemp[i].y); //죽으면 죽은 위치 가서
						printf("  "); // 지우거나 죽은 흔적을 남기거나.
					}
					else { // 벽에 붙어서 올 경우
						gotoxy(Itemtemp[i].x, Itemtemp[i].y); //죽으면 죽은 위치 가서
						printf("  "); // 지우거나 죽은 흔적을 남기거나.
					}

					Itemtemp[i].x = (rand() % 22) * 3; //위치 초기화
					Itemtemp[i].y = 0; //위치 초기화
					Itemtemp[i].timeafterdead = 0; //time 초기화
					Itemtemp[i].havedied = 0; //havedied 초기화
					Itemtemp[i].exist = FALSE;
				}

				/////////////////////////////////////////////////////////////////////////////////////////////////

			}
			else { // y축 기준 40을 넘지 않으면(화면 내에 있으면)
				Itemtemp[i].y++; // 해당 적군의 [y좌표를 1 증가]시키자.//////////////////////////////////////////////////////위치변경

				if (Itemtemp[i].x < 3) { //Item가 왼쪽 벽에 붙었을 경우
					gotoxy(0, Itemtemp[i].y - 1); // y좌표 1 증가시키기 전 위치로 가서
					printf("  "); //빈공간으로 만들어서 준비해놓자.
				}
				else {
					gotoxy(Itemtemp[i].x, Itemtemp[i].y - 1); // y좌표 1 증가시키기 전 위치로 가서
					printf("  "); //빈공간으로 만들어서 준비해놓자.
				}
			}
			SetColor(RED1, 0); //색 바꿔두고

			if (Itemtemp[i].exist == TRUE) { //아니 살아있어야 뭘 할 것 아닌가?
				switch (Itemtemp[i].type) {

				case 1:
				case 5:
					gotoxy(Itemtemp[i].x, Itemtemp[i].y); //y좌표 1 증가된 곳으로 가서
					SetColor(BLUE1, 0);
					printf("◆"); //적군 다시 출력하자(이동시키자).
					break;

				case 2:
				
					gotoxy(Itemtemp[i].x, Itemtemp[i].y); //y좌표 1 증가된 곳으로 가서
					SetColor(YELLOW1, 0);
					printf("◆"); //적군 다시 출력하자(이동시키자).
					break;

				case 3:
				case 6:
					gotoxy(Itemtemp[i].x, Itemtemp[i].y); //y좌표 1 증가된 곳으로 가서
					SetColor(GREEN2, 0);
					printf("◆"); //적군 다시 출력하자(이동시키자).
					break;

				case 4:
				case 7:
					gotoxy(Itemtemp[i].x, Itemtemp[i].y); //y좌표 1 증가된 곳으로 가서
					SetColor(WHITE, 0);
					printf("◆"); //적군 다시 출력하자(이동시키자).
					break;


				}
			}

		}

		Itemtemp[i].stay--; //stay를 줄여야 언젠가 0이돼서 움직일거 아닌가

	}

}

void ItemCollide() {

	//참고 : 유니티가 아니다. 한 프레임 안에 모든 행동들을 구현해야 한다는 걸 잊지 말자.
	// 또, 아주 가끔씩 총알 쐈는데 Item가 안죽는다. 왜그러냐?
	// 이 경우에는 강제로 죽게 만들어도 다음 프레임에 재생성돼서 또 내려온다.

	int i, j;

	for (i = 0; i < 7; i++) { //Item 세기
		if (Itemtemp[i].exist == FALSE) {
			continue; //Itemtemp[i]가 exist하지 않으므로 통째로 스킵한다.
		}

		for (j = 0; j < MAXBULLET; j++) { //bullet 세기

			if (ispierce == 0) {

				if (abs(Itemtemp[i].x - Bullet[j].x) <= 2 && abs(Itemtemp[i].y - Bullet[j].y) <= 1 && Bullet[j].exist == TRUE) { //총알과 적의 y과표가 같고, x좌표는 비슷하며, 총알이 유효판정일 경우
					if (Itemtemp[i].x > 2) { // 일반적인 경우



						Bullet[j].exist = FALSE; //총알도 죽은거로 판정하자.
						gotoxy(Bullet[j].x, Bullet[j].y);
						printf("  ");
						gotoxy(Bullet[j].x, Bullet[j].y + 1);
						printf("  ");

						switch (Itemtemp[i].type) {

						case 1:
						case 5:
							gotoxy(Itemtemp[i].x, Itemtemp[i].y); // 죽으면 죽은 위치로 가서
							SetColor(BLUE1, 0);
							printf("◇"); //적군 다시 출력하자(이동시키자).
							israpid = 1;
							rapidtime = 0;
							PlaySound("getitem.wav", NULL, SND_ASYNC);
							break;

						case 2:
							gotoxy(Itemtemp[i].x, Itemtemp[i].y); //죽으면 죽은 위치로 가서
							SetColor(YELLOW1, 0);
							printf("◇"); //적군 다시 출력하자(이동시키자).
							ishoming = 1;
							homingtime = 0;
							PlaySound("getitem.wav", NULL, SND_ASYNC);
							break;

						case 3:
						case 6:
							gotoxy(Itemtemp[i].x, Itemtemp[i].y); //죽으면 죽은 위치로 가서
							SetColor(GREEN2, 0);
							printf("◇"); //적군 다시 출력하자(이동시키자).
							ispierce = 1;
							piercetime = 0;
							PlaySound("getitem.wav", NULL, SND_ASYNC);
							break;

						case 4:
						case 7:
							gotoxy(Itemtemp[i].x, Itemtemp[i].y); //죽으면 죽은 위치로 가서
							SetColor(WHITE, 0);
							printf("◇"); //적군 다시 출력하자(이동시키자).
							clearfield();
							PlaySound("getitem.wav", NULL, SND_ASYNC);
							break;


						}


					}
					else { // 벽에 붙어서 오는 경우



						Bullet[j].exist = FALSE; //총알도 죽은거로 판정하자.
						gotoxy(Bullet[j].x, Bullet[j].y);
						printf("  ");
						gotoxy(Bullet[j].x, Bullet[j].y + 1);
						printf("  ");

						switch (Itemtemp[i].type) {

						case 1:
						case 5:
							gotoxy(Itemtemp[i].x, Itemtemp[i].y); //죽으면 죽은 위치 가서
							SetColor(BLUE1, 0);
							printf("◇"); //적군 다시 출력하자(이동시키자).
							israpid = 1;
							rapidtime = 0;
							PlaySound("getitem.wav", NULL, SND_ASYNC);
							break;

						case 2:
							gotoxy(Itemtemp[i].x, Itemtemp[i].y); //죽으면 죽은 위치 가서
							SetColor(YELLOW1, 0);
							printf("◇"); //적군 다시 출력하자(이동시키자).
							ishoming = 1;
							homingtime = 0;
							PlaySound("getitem.wav", NULL, SND_ASYNC);
							break;

						case 3:
						case 6:
							gotoxy(Itemtemp[i].x, Itemtemp[i].y); //죽으면 죽은 위치 가서
							SetColor(GREEN2, 0);
							printf("◇"); //적군 다시 출력하자(이동시키자).
							ispierce = 1;
							piercetime = 0;
							PlaySound("getitem.wav", NULL, SND_ASYNC);
							break;

						case 4:
						case 7:
							gotoxy(Itemtemp[i].x, Itemtemp[i].y); //죽으면 죽은 위치 가서
							SetColor(WHITE, 0);
							printf("◇"); //적군 다시 출력하자(이동시키자).
							clearfield();
							PlaySound("getitem.wav", NULL, SND_ASYNC);
							break;


						}




					}

					Itemtemp[i].exist = FALSE; //적 죽은거로 판정하자.
					Itemtemp[i].havedied = 1;
					Itemtemp[i].dying = 1;

				}

			}else { //ispierce인 경우

			if (abs(Itemtemp[i].x - Bullet[j].x) <= 6 && abs(Itemtemp[i].y - Bullet[j].y) <= 1 && Bullet[j].exist == TRUE) { //총알과 적의 y과표가 같고, x좌표는 비슷하며, 총알이 유효판정일 경우
				if (Itemtemp[i].x > 2) { // 일반적인 경우



					switch (Itemtemp[i].type) {

					case 1:
					case 5:
						gotoxy(Itemtemp[i].x, Itemtemp[i].y); // 죽으면 죽은 위치로 가서
						SetColor(BLUE1, 0);
						printf("◇"); //적군 다시 출력하자(이동시키자).
						israpid = 1;
						rapidtime = 0;
						PlaySound("getitem.wav", NULL, SND_ASYNC);
						break;

					case 2:
						gotoxy(Itemtemp[i].x, Itemtemp[i].y); //죽으면 죽은 위치로 가서
						SetColor(YELLOW1, 0);
						printf("◇"); //적군 다시 출력하자(이동시키자).
						ishoming = 1;
						homingtime = 0;
						PlaySound("getitem.wav", NULL, SND_ASYNC);
						break;

					case 3:
					case 6:
						gotoxy(Itemtemp[i].x, Itemtemp[i].y); //죽으면 죽은 위치로 가서
						SetColor(GREEN2, 0);
						printf("◇"); //적군 다시 출력하자(이동시키자).
						ispierce = 1;
						piercetime = 0;
						PlaySound("getitem.wav", NULL, SND_ASYNC);
						break;

					case 4:
					case 7:
						gotoxy(Itemtemp[i].x, Itemtemp[i].y); //죽으면 죽은 위치로 가서
						SetColor(WHITE, 0);
						printf("◇"); //적군 다시 출력하자(이동시키자).
						clearfield();
						PlaySound("getitem.wav", NULL, SND_ASYNC);
						break;


					}


				}
				else { // 벽에 붙어서 오는 경우



					switch (Itemtemp[i].type) {

					case 1:
					case 5:
						gotoxy(Itemtemp[i].x, Itemtemp[i].y); //죽으면 죽은 위치 가서
						SetColor(BLUE1, 0);
						printf("◇"); //적군 다시 출력하자(이동시키자).
						israpid = 1;
						rapidtime = 0;
						PlaySound("getitem.wav", NULL, SND_ASYNC);
						break;

					case 2:
						 
						gotoxy(Itemtemp[i].x, Itemtemp[i].y); //죽으면 죽은 위치 가서
						SetColor(YELLOW1, 0);
						printf("◇"); //적군 다시 출력하자(이동시키자).
						ishoming = 1;
						homingtime = 0;
						PlaySound("getitem.wav", NULL, SND_ASYNC);
						break;

					case 3:
					case 6:
						gotoxy(Itemtemp[i].x, Itemtemp[i].y); //죽으면 죽은 위치 가서
						SetColor(GREEN2, 0);
						printf("◇"); //적군 다시 출력하자(이동시키자).
						ispierce = 1;
						piercetime = 0;
						PlaySound("getitem.wav", NULL, SND_ASYNC);
						break;

					case 4:
					case 7:
						gotoxy(Itemtemp[i].x, Itemtemp[i].y); //죽으면 죽은 위치 가서
						SetColor(WHITE, 0);
						printf("◇"); //적군 다시 출력하자(이동시키자).
						clearfield();
						PlaySound("getitem.wav", NULL, SND_ASYNC);
						break;


					}




				}

				Itemtemp[i].exist = FALSE; //적 죽은거로 판정하자.
				Itemtemp[i].havedied = 1;
				Itemtemp[i].dying = 1;

			}


			}

		}

	}

}

void eraseItemcrash() { // '총맞아 죽은' enemy에서 나온 crashed 제거기
	int i;
	for (i = 0; i < 7; i++) { //Item 찾기
		if (Itemtemp[i].exist == FALSE && Itemtemp[i].timeafterdead < 10 && Itemtemp[i].havedied == 1 && Itemtemp[i].dying == 1) { //뒤진 상태면
			Itemtemp[i].timeafterdead++;
			Itemtemp[i].timeafterdead++;//time 1씩 증가시키기.

			/*디버깅 소스
			gotoxy(Itemtemp[i].x, Itemtemp[i].y+1); //죽으면 죽은 위치 가서
			printf("time %d, isdead %d, dying %d, Exist %d, stay %d frame %d  ", Itemtemp[i].timeafterdead, Itemtemp[i].havedied, Itemtemp[i].dying, Itemtemp[i].exist, Itemtemp[i].stay, Itemtemp[i].frame); // 지우거나 죽은 흔적을 남기거나.
			*/
			// 디버깅 결과, 안지워지는 련들은 timeafterdead가 증가하다가 4~6 사이에서 별안간 멈추는 것을 확인할 수 있었다.
			// 이를 토대로 화요일날 한번 해결해 보도록 하자.
		}

		if (Itemtemp[i].timeafterdead >= 10) { //뒤지고 10만큼 지나면 crashed 지우기.

			if (Itemtemp[i].x > 2) { //일반적인 경우
				gotoxy(Itemtemp[i].x, Itemtemp[i].y); //죽으면 죽은 위치 가서
				printf("  "); // 지우거나 죽은 흔적을 남기거나.
			}
			else { // 왼쪽 벽에 붙어서 올 경우
				gotoxy(Itemtemp[i].x, Itemtemp[i].y); //죽으면 죽은 위치 가서
				printf("  "); // 지우거나 죽은 흔적을 남기거나.
			}

			Itemtemp[i].x = (rand() % 22) * 3;
			Itemtemp[i].y = 0;

			Itemtemp[i].frame = Itemtemp[i].stay = frame; //frame은 10.
			Itemtemp[i].timeafterdead = 0;
			Itemtemp[i].exist = FALSE;
			//Itemtemp 재사용하는 대신 여기서 초기화시켜서 내보낸다.
		}
	}

}

void rapidfire() {

	if (bullet > MAXBULLET) bullet = 0;
	//아래, 총알먼저 발사하든가말든가
	Bullet[bullet].x = mainpx;
	Bullet[bullet].y = 37;
	Bullet[bullet].exist = TRUE;
	bullet++;
	hardness++;
	if (hardness % 30 == 0) {
		if (Delay > 15) {
			Delay--;
			enemynum += 3;
			addscore += 0.02;
		}
	}
	if (hardness % 700 == 0) {
		if (Delay > 12) {
			Delay--;
			addscore += 0.07;
			if (frame > 3) {
				frame--;
			}
			enemynum = +10;
		}
	}

}

void rapidhalt() {
	if (israpid = 1) {


		rapidtime++;
		if (rapidtime > 100) {
			rapidtime = 0;
			israpid = 0;
		}

	}

}

void hominghalt() {
	if (ishoming = 1) {


		homingtime++;
		if (homingtime > 350) {
			homingtime = 0;
			ishoming = 0;
		}

	}

}

void piercehalt() {

	int i;

	if (ispierce = 1) {


		piercetime++;
		if (piercetime > 200) {
			piercetime = 0;
			ispierce = 0;

			for (i = 0; i < MAXBULLET; i++) {

				gotoxy(Bullet[i].x, Bullet[i].y - 3);
				printf("  ");

				gotoxy(Bullet[i].x - 2, Bullet[i].y - 2);
				printf("  ");

				gotoxy(Bullet[i].x, Bullet[i].y - 2);
				printf("  ");

				gotoxy(Bullet[i].x + 2, Bullet[i].y - 2);
				printf("  ");

				gotoxy(Bullet[i].oldx - 2, Bullet[i].oldy - 1);
				printf("  ");

				gotoxy(Bullet[i].oldx, Bullet[i].oldy - 1);
				printf("  ");

				gotoxy(Bullet[i].oldx + 2, Bullet[i].oldy - 1);
				printf("  ");

				gotoxy(Bullet[i].oldx-2, Bullet[i].oldy);
				printf("  ");

				gotoxy(Bullet[i].oldx, Bullet[i].oldy);
				printf("  ");

				gotoxy(Bullet[i].oldx + 2, Bullet[i].oldy);
				printf("  ");

			}

		}

	}

}





void DrawEnemy1() {
	BOOL Found;
	int i, j;

	//참고 : 이 소스코드는 몹이 좌우로 이동한다. 꼭 나중에 아래로만 이동하도록 바꿀 것

	if (rand() % 10 < enemynum) { //rand()%100의 값이 enemynum보다 적은 범위에 있으면, 즉 enemynum/100의 확률로.
		for (i = 0; i < 7 && Enemy[i].exist == TRUE; i++) {
			; //8마리 생성된 상태라면 i가 9가 됨. 그런 식이지 뭐 이게.
		}

		if (i != 7) { //위에서 받아서, 예를들어 i가 9인데 9가 7보다 적다면-

			Enemy[i].x = rand() % 60;

			while (1) {
				Enemy[i].y = rand() % 10 + 1;
				for (Found = FALSE, j = 0; j < 7; j++) {
					if (Enemy[i].exist == TRUE && Enemy[j].x == Enemy[i].x) {
						Found = TRUE;
						break;
					}
				}
				if (Found == FALSE) {
					break;
				}
			}
			Enemy[i].nFrame = Enemy[i].nStay = rand() % 6 + 1;
			Enemy[i].type = rand() % (sizeof(arrEnemy) / sizeof(arrEnemy[0]));
			Enemy[i].exist = TRUE;

		}

	}
}
//참고용 코드, 실 작동x

void MoveEnemy1() {

	int i;

	for (i = 0; i < MAXENEMY; i++) {
		if (Enemy[i].exist == FALSE) {
			continue;
		}
		if (--Enemy[i].nStay == 0) {
			Enemy[i].nStay = Enemy[i].nFrame;

			if (Enemy[i].x > 40) {
				Enemy[i].exist = FALSE;
				gotoxy(Enemy[i].x - 3, Enemy[i].y);
				printf("       ");
			}
			else {
				Enemy[i].y++;
				gotoxy(Enemy[i].x - 3, Enemy[i].y - 1);
				printf("       ");
			}
			SetColor(RED1, 0);
			gotoxy(Enemy[i].x - 1, Enemy[i].y);
			printf(arrEnemy[Enemy[i].type]);
		}

	}

}
//참고용 코드, 실 작동x

int main() {

	unsigned char last_ch, ch, chforend;
	int oldx, oldy, newx, newy;
	int keep_moving;
	int i;
	
	int bulletspeed = 1;


	////////////슈퍼란듐
	int example;
	srand(time(NULL)); // 난수초기화
	example = rand() % 6; //란듐()돌려서
	//6으로나눈나머지(0~5) 중하나가된다.
	////////////슈퍼란듐

	do {

		life = 5.;

		newx = oldx = 10;
		newy = oldy = 10;

		system("mode con cols=100 lines=40");
		cls(0, WHITE);

		removeCursor();
		LoadMain();
		//LoadEnd();

		cls(0, WHITE);
		VertLine();
		DrawPlayer();

		gotoxy(32, 15);
		printf("R E A D Y");
		_getch();
		gotoxy(32, 15);
		printf("         ");

		removeCursor();
		keep_moving = 1;
		last_ch = RIGHT; // 일단 오른쪽으로 이동한다. 

		while (1) {
			if (_kbhit() == 1) {  // 키보드가 눌려져 있으면
				char c1;
				c1 = _getch(); // key 값을 읽는다
				if (c1 == '\0') // VS 의 NULL 값은 Error이다.
					continue;
				else
					ch = c1; // NULL이 아니면 정상 입력이다.
				// ESC 누르면 프로그램 종료
				if (ch == ESC)
					exit(0);
				if (ch == RIGHT)
					if (Delay > 12) {
						Delay--;
						//SetColor(WHITE, 0);
						//gotoxy(32, 17);
						//printf("%d", Delay);
					}
				if (ch == LEFT)
					if (Delay < 99) {
						if (Delay > 99) {
							Delay = 99;
						}
						else {
							Delay++;
						}
						//SetColor(WHITE, 0);
						//gotoxy(32, 17);
						//printf("%d", Delay);
					}
				if (ch == SPACE) { // 만약 스페이스라면

					if (bullet > MAXBULLET) bullet = 0;
					//아래, 총알먼저 발사하든가말든가
					Bullet[bullet].x = mainpx;
					Bullet[bullet].y = 37;
					Bullet[bullet].exist = TRUE;
					DrawBullet(bullet);

					if (ishoming == 1) {

						PlaySound("homingshot.wav", NULL, SND_ASYNC);
					
					}
					else if (ispierce == 1) {

						PlaySound("cannonshot.wav", NULL, SND_ASYNC);

					}
					else {
						PlaySound("shot.wav", NULL, SND_ASYNC);
						//출처: https://anisyw.tistory.com/entry/API-PlaySound를-이용한-wav파일-재생 [Be the Best Engineer]
					}
					bullet++;
					numofbullets++;
					hardness++;
					if (hardness % 30 == 0) {
						if (Delay > 15) {
							Delay--;
							enemynum += 3;
							addscore += 0.02;
						}
					}
					if (hardness % 700 == 0) {
						if (Delay > 12) {
							Delay--;
							addscore += 0.07;
							if (frame > 3) {
								frame--;
							}
							enemynum = +10;
						}
					}
					//SetColor(WHITE, 0);
					//gotoxy(32, 15);
					//printf("%d", hardness);
					//gotoxy(32, 17);
					//printf("%d", Delay);


					switch (last_ch) { //last_ch를 체크한 후
					case RIGHT: //RIGHT일 경우에는
						last_ch = LEFT; //LEFT로 방향전환 시키고...(생략)
						break;
					case LEFT:
						last_ch = RIGHT;
						break;
					default:
						break;
					}
					keep_moving = 1;
				}
			}
			if (keep_moving) { // 움직이고 있으면
				switch (last_ch) {
				case LEFT:
					if (mainpx > 0)
						mainpx--;
					else {
						mainpx = 0;
					}
					break;
				case RIGHT:
					if (mainpx < 68)
						mainpx++;
					else {
						mainpx = 68;
					}
					break;
				}

				MovePlayer();
				for (int k = 0; k < bulletspeed; k++) {
					MoveBullet();
				}
				keep_moving = 1;

				VertLine();

				erasecrash();
				EnemyCollide();

				DrawEnemy();
				erasecrash();
				MoveEnemy();

				ItemCollide();
				eraseItemcrash();
				DrawItem();
				MoveItem();
				eraseItemcrash();


				if (israpid == 1) {

					if (ispierce == 0) {
						rapidfire();
					}

					rapidhalt();

				}

				if (ishoming == 1) {

					hominghalt();

				}

				if (ispierce == 1) {

					piercehalt();

				}

				if (life < 0) {
					break;
				}


				SetColor(WHITE, 0);
				gotoxy(80, 14);
				printf("Score : %.f", score);
				gotoxy(80, 17);
				printf("Life : %.f", life);
				gotoxy(80, 20);
				printf("Bullet : %d", numofbullets);
				score += addscore;
				gotoxy(80, 23);
				printf("Hard : %d", abs(Delay - 35));
				//gotoxy(80, 22);
				//printf("israpid : %d", israpid);

			}
			Sleep(Delay);// Delay를 줄이면 속도가 빨라진다.

		} //움직이는 소스

		cls(0, WHITE);


		//게임종료 & 리트라이 화면 꾸미기.
		
		LoadEnd();

		gotoxy(40, 19);
		printf("Retry? (Y/N)");

		while (1) {

			

			if (_kbhit() == 1) {  // 키보드가 눌려져 있으면
				char c13;
				c13 = _getch(); // key 값을 읽는다
				if (c13 == '\0') // VS 의 NULL 값은 Error이다.
					continue;
				else
					chforend = c13; // NULL이 아니면 정상 입력이다.
				// ESC 누르면 프로그램 종료
				if (chforend == YES)
					islooped = 1;
					
					clearfieldatend();
					clearitematend();

					Delay = 35;
					mainpx = 35;
					bulletx = -1;
					pullety = -1;
					bullet = 0;
					numofbullets = 0;
					hardness = 51;
					life = 5.;
					score = 0;
					enemynum = 10;
					frame = 10;
					addscore = 0.01;

					israpid = 0;
					rapidtime = 0;
					ishoming = 0;
					homingtime = 0;

					ispierce = 0;
					piercetime = 0;

					bulletspeed = 1;

					keep_moving = 1;

					

					break;

				if (chforend == NO)
					break;
					break;
			}
		}
	}while (islooped == 1);

	cls(0, WHITE);

	gotoxy(42, 19);
	printf("GAME OVER");

	_getch();//키를 누르면 종료

	return 0;
}