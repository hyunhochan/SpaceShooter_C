/*

// omok.c 오목의 돌의 움직임을 표현
#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>

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

#define UP2		'w'
#define DOWN2	's'
#define LEFT2	'a'
#define RIGHT2	'd'
#define SPACE 0x20

#define MAXBULLET 70

#define MAXENEMY 40



struct {
	BOOL exist;
	int x, y;
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
int hardness = 51;
int life = 100;
double score = 0.0;
int enemynum = 10;
int frame = 10;

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
			//Sleep(1);
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
			//Sleep(1);
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
	gotoxy(30, 25); printf("│     BLUE Diamond =     Lader     │");
	gotoxy(30, 26); printf("│   Yellow Diamond =     LASER     │");
	gotoxy(30, 27); printf("│    Green Diamond =   Rapid Fire  │");
	gotoxy(30, 27); printf("│    Green Diamond =   Rapid Fire  │");
	gotoxy(30, 28); printf("│   White Diammond =  Clear field  │");
	gotoxy(30, 29); printf("│                                  │");
	gotoxy(30, 30); printf("└─────────────────┘");

	gotoxy(37, 34);
	printf("Press Any Key to Start");
	_getch();


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
	SetColor(YELLOW1, 0);
	gotoxy(Bullet[i].x, Bullet[i].y);
	printf("●");
}

void EraseBullet(int i) {
	gotoxy(Bullet[i].x, Bullet[i].y + 1);
	printf("  ");
}

void MoveBullet() {
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
				}
				else { // 벽에 붙어서 올 경우
					gotoxy(0, Enemytemp[i].y); //죽으면 죽은 위치 가서
					SetColor(BLUE1, 0);
					printf("crashed"); // 지우거나 죽은 흔적을 남기거나.
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

	int i, j;

	for (i = 0; i < MAXENEMY; i++) { //enemy 세기
		if (Enemytemp[i].exist == FALSE) {
			continue; //Enemytemp[i]가 exist하지 않으므로 통째로 스킵한다.
		}

		for (j = 0; j < MAXBULLET; j++) { //bullet 세기

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



				}

				Enemytemp[i].exist = FALSE; //적 죽은거로 판정하자.
				Enemytemp[i].havedied = 1;
				Enemytemp[i].dying = 1;


			}

		}

	}

}

void erasecrash() { // '총맞아 죽은' enemy에서 나온 crashed 제거기
	int i;
	for (i = 0; i < MAXENEMY; i++) { //enemy 찾기
		if (Enemytemp[i].exist == FALSE && Enemytemp[i].timeafterdead < 10 && Enemytemp[i].havedied == 1 && Enemytemp[i].dying == 1) { //뒤진 상태면
			Enemytemp[i].timeafterdead++;
			Enemytemp[i].timeafterdead++;//time 1씩 증가시키기.

			//디버깅 소스
			//gotoxy(Enemytemp[i].x, Enemytemp[i].y+1); //죽으면 죽은 위치 가서
			//printf("time %d, isdead %d, dying %d, Exist %d, stay %d frame %d  ", Enemytemp[i].timeafterdead, Enemytemp[i].havedied, Enemytemp[i].dying, Enemytemp[i].exist, Enemytemp[i].stay, Enemytemp[i].frame); // 지우거나 죽은 흔적을 남기거나.
			
			// 디버깅 결과, 안지워지는 련들은 timeafterdead가 증가하다가 4~6 사이에서 별안간 멈추는 것을 확인할 수 있었다.
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

			Enemytemp[i].frame = Enemytemp[i].stay = frame; //frame은 10.
			Enemytemp[i].timeafterdead = 0;
			Enemytemp[i].exist = TRUE;
			Enemytemp[i].havedied = 0;
			//Enemytemp 재사용하는 대신 여기서 초기화시켜서 내보낸다.
		}
	}

}

void DrawItem() {

	// 현재 문제
	// 적이 땅에 닿아서 파괴되면 즉각 위에서 재생성이 되는데,
	// 적이 총알에 닿아서 파괴되면 위에서 바로 생성되지 않고 좀 기다려야 생성됨.

	int i;
	int itemtype;

	itemtype = rand() % 10 + 1; //itemtype는 1, 2, 3, 4 중 하나로 결정됨, 사기적인 아이템은 나올 확률을 조정해서 4:3:2:1의 비율로 등장하도록 함.
	if (rand() % 1000 < 700) { //rand()%350의 값이 Itemnum보다 적은 범위에 있으면, 즉 7/1000의 확률로, 새 아이템이 추가됨.
		for (i = 0; i <= 7 && Itemtemp[i].exist == TRUE && Itemtemp[i].dying == 1; i++) {
			; // 8마리 생성된 상태라면 i가 9가 됨. 그런 식이지 뭐 이게. 그리고 중간에 exist가 false인 애를 만나도(빈 공간을 찾아도) 빠져나온다.
		}

		if (i == 7) { //7를 넘어서면 다시 i를 0으로 바꿔서 다시 출력시킴. 뭐 겹치니 어떻니 할 수도 있는데 그 전에 이미 아래로 꼬라박아서 없어질걸?
			i = 0;
		}

		if (i < 7 && Itemtemp[i].exist == FALSE) { //위에서 받아서, 예를들어 i가 9인데 9가 7보다 적고 죽어서 배열에 자리가 빈 상태라면

			switch (itemtype) {

			case 1:
			case 2:
			case 3:
			case 4:

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


			case 5:
			case 6:
			case 7:

				Itemtemp[i].x = (rand() % 22) * 3;
				Itemtemp[i].y = 0;

				Itemtemp[i].frame = Itemtemp[i].stay = 4; //frame은 10.
				Itemtemp[i].type = itemtype;
				Itemtemp[i].timeafterdead = 0;
				Itemtemp[i].exist = TRUE;
				Itemtemp[i].havedied = 0;
				Itemtemp[i].dying = 0;


				SetColor(YELLOW2, 0);
				gotoxy(Itemtemp[i].x, 0);
				printf("◆");

				break;


			case 8:
			case 9:

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


			case 10:

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


	// 또, 아주 가끔씩 총알 쐇는데 Item가 안죽는다.
	// 아주 가끔식 erasecrash()가 안먹는다.
	//-----------시벌 내가 이거 다시는 해결하려고 하나 봐라------------

	int i;

	for (i = 0; i < 7; i++) {
		if (Itemtemp[i].exist == FALSE) {


			continue; //Itemtemp[i]가 exist하지 않으므로 통째로 스킵한다.
		}
		if (Itemtemp[i].stay == 0) { //stay가 0이 아니면 한 턴 쉬고 스킵한다. 그리고 1 줄여둠.
			Itemtemp[i].stay = Itemtemp[i].frame; //stay가 0이 되면 진입하며, stay는 다시 frame과 동기화 시켜둔다.

			if (Itemtemp[i].y > 38) { //y축기준 38을 넘어버리면 몹 죽는거지 뭐.


				if (Itemtemp[i].x > 2) { //일반적인 경우
					gotoxy(Itemtemp[i].x - 2, Itemtemp[i].y); //죽으면 죽은 위치 가서
					SetColor(BLUE1, 0);
					printf("crashed"); // 지우거나 죽은 흔적을 남기거나.
				}
				else { // 벽에 붙어서 올 경우
					gotoxy(0, Itemtemp[i].y); //죽으면 죽은 위치 가서
					SetColor(BLUE1, 0);
					printf("crashed"); // 지우거나 죽은 흔적을 남기거나.
				}

				// 3프레임 뒤에 crashed 지우는 소스/////////////////////////////////////////////////////////

				Itemtemp[i].exist = FALSE; //적 죽은거로 판정하자.
				Itemtemp[i].havedied = 1;
				if (Itemtemp[i].exist == FALSE && Itemtemp[i].timeafterdead < 2 && Itemtemp[i].havedied == 1) {
					Itemtemp[i].timeafterdead++;
				}
				if (Itemtemp[i].timeafterdead == 2) {

					if (Itemtemp[i].x > 2) { //일반적인 경우
						gotoxy(Itemtemp[i].x - 2, Itemtemp[i].y); //죽으면 죽은 위치 가서
						printf("         "); // 지우거나 죽은 흔적을 남기거나.
					}
					else { // 벽에 붙어서 올 경우
						gotoxy(0, Itemtemp[i].y); //죽으면 죽은 위치 가서
						printf("         "); // 지우거나 죽은 흔적을 남기거나.
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
				case 2:
				case 3:
				case 4:
					gotoxy(Itemtemp[i].x, Itemtemp[i].y); //y좌표 1 증가된 곳으로 가서
					SetColor(BLUE1, 0);
					printf("◆"); //적군 다시 출력하자(이동시키자).
					break;

				case 5:
				case 6:
				case 7:
					gotoxy(Itemtemp[i].x, Itemtemp[i].y); //y좌표 1 증가된 곳으로 가서
					SetColor(YELLOW2, 0);
					printf("◆"); //적군 다시 출력하자(이동시키자).
					break;

				case 8:
				case 9:
					gotoxy(Itemtemp[i].x, Itemtemp[i].y); //y좌표 1 증가된 곳으로 가서
					SetColor(GREEN2, 0);
					printf("◆"); //적군 다시 출력하자(이동시키자).
					break;

				case 10:
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

	//참고 : 한 프레임 안에 모든 행동들을 구현해야 한다는 걸 잊지 말자.
	// 또, 아주 가끔씩 총알 쐈는데 Item가 안죽는다. 왜그러냐?
	// 이 경우에는 강제로 죽게 만들어도 다음 프레임에 재생성돼서 또 내려온다.

	int i, j;

	for (i = 0; i < 7; i++) { //Item 세기
		if (Itemtemp[i].exist == FALSE) {
			continue; //Itemtemp[i]가 exist하지 않으므로 통째로 스킵한다.
		}

		for (j = 0; j < MAXBULLET; j++) { //bullet 세기

			if (abs(Itemtemp[i].x - Bullet[j].x) <= 2 && abs(Itemtemp[i].y - Bullet[j].y) <= 1 && Bullet[j].exist == TRUE) { //총알과 적의 y과표가 같고, x좌표는 비슷하며, 총알이 유효판정일 경우
				if (Itemtemp[i].x > 2) { // 일반적인 경우



					Bullet[j].exist = FALSE; //총알도 죽은거로 판정하자.
					gotoxy(Bullet[j].x, Bullet[j].y);
					printf("  ");
					gotoxy(Bullet[j].x, Bullet[j].y + 1);
					printf("  ");

					switch (Itemtemp[i].type) {

					case 1:
					case 2:
					case 3:
					case 4:
						gotoxy(Itemtemp[i].x, Itemtemp[i].y); // 죽으면 죽은 위치로 가서
						SetColor(BLUE1, 0);
						printf("◇"); //적군 다시 출력하자(이동시키자).
						break;

					case 5:
					case 6:
					case 7:
						gotoxy(Itemtemp[i].x, Itemtemp[i].y); //죽으면 죽은 위치로 가서
						SetColor(YELLOW2, 0);
						printf("◇"); //적군 다시 출력하자(이동시키자).
						break;

					case 8:
					case 9:
						gotoxy(Itemtemp[i].x, Itemtemp[i].y); //죽으면 죽은 위치로 가서
						SetColor(GREEN2, 0);
						printf("◇"); //적군 다시 출력하자(이동시키자).
						break;

					case 10:
						gotoxy(Itemtemp[i].x, Itemtemp[i].y); //죽으면 죽은 위치로 가서
						SetColor(WHITE, 0);
						printf("◇"); //적군 다시 출력하자(이동시키자).
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
					case 2:
					case 3:
					case 4:
						gotoxy(Itemtemp[i].x, Itemtemp[i].y); //죽으면 죽은 위치 가서
						SetColor(BLUE1, 0);
						printf("◇"); //적군 다시 출력하자(이동시키자).
						break;

					case 5:
					case 6:
					case 7:
						gotoxy(Itemtemp[i].x, Itemtemp[i].y); //죽으면 죽은 위치 가서
						SetColor(YELLOW2, 0);
						printf("◇"); //적군 다시 출력하자(이동시키자).
						break;

					case 8:
					case 9:
						gotoxy(Itemtemp[i].x, Itemtemp[i].y); //죽으면 죽은 위치 가서
						SetColor(GREEN2, 0);
						printf("◇"); //적군 다시 출력하자(이동시키자).
						break;

					case 10:
						gotoxy(Itemtemp[i].x, Itemtemp[i].y); //죽으면 죽은 위치 가서
						SetColor(WHITE, 0);
						printf("◇"); //적군 다시 출력하자(이동시키자).
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

void eraseItemcrash() { // '총맞아 죽은' enemy에서 나온 crashed 제거기
	int i;
	for (i = 0; i < 7; i++) { //Item 찾기
		if (Itemtemp[i].exist == FALSE && Itemtemp[i].timeafterdead < 10 && Itemtemp[i].havedied == 1 && Itemtemp[i].dying == 1) { //뒤진 상태면
			Itemtemp[i].timeafterdead++;
			Itemtemp[i].timeafterdead++;//time 1씩 증가시키기.

			//디버깅 소스
			//gotoxy(Itemtemp[i].x, Itemtemp[i].y+1); //죽으면 죽은 위치 가서
			//printf("time %d, isdead %d, dying %d, Exist %d, stay %d frame %d  ", Itemtemp[i].timeafterdead, Itemtemp[i].havedied, Itemtemp[i].dying, Itemtemp[i].exist, Itemtemp[i].stay, Itemtemp[i].frame); // 지우거나 죽은 흔적을 남기거나.
			
			// 디버깅 결과, 안지워지는 련들은 timeafterdead가 증가하다가 4~6 사이에서 별안간 멈추는 것을 확인할 수 있었다.
			// 이를 토대로 화요일날 한번 해결해 보도록 하자.
		}

		if (Itemtemp[i].timeafterdead >= 10) { //뒤지고 10만큼 지나면 crashed 지우기.

			if (Itemtemp[i].x > 2) { //일반적인 경우
				gotoxy(Itemtemp[i].x, Itemtemp[i].y); //죽으면 죽은 위치 가서
				printf("      "); // 지우거나 죽은 흔적을 남기거나.
			}
			else { // 왼쪽 벽에 붙어서 올 경우
				gotoxy(Itemtemp[i].x, Itemtemp[i].y); //죽으면 죽은 위치 가서
				printf("      "); // 지우거나 죽은 흔적을 남기거나.
			}

			Itemtemp[i].x = (rand() % 22) * 3;
			Itemtemp[i].y = 0;

			Itemtemp[i].frame = Itemtemp[i].stay = frame; //frame은 10.
			Itemtemp[i].timeafterdead = 0;
			Itemtemp[i].type = rand() % 10 + 1;
			Itemtemp[i].exist = FALSE;
			Itemtemp[i].havedied = 0;
			//Itemtemp 재사용하는 대신 여기서 초기화시켜서 내보낸다.
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

	unsigned char last_ch, ch;
	int oldx, oldy, newx, newy;
	int keep_moving;
	int i;
	int bullet = 0;
	int bulletspeed = 1;


	////////////슈퍼란듐
	int example;
	srand(time(NULL)); // 난수초기화
	example = rand() % 6; //란듐()돌려서
	//6으로나눈나머지(0~5) 중하나가된다.
	////////////슈퍼란듐


	newx = oldx = 10;
	newy = oldy = 10;

	system("mode con cols=100 lines=40");
	cls(0, WHITE);

	removeCursor();
	LoadMain();

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
					SetColor(WHITE, 0);
					gotoxy(32, 17);
					printf("%d", Delay);
				}
			if (ch == LEFT)
				if (Delay < 99) {
					if (Delay > 99) {
						Delay = 99;
					}
					else {
						Delay++;
					}
					SetColor(WHITE, 0);
					gotoxy(32, 17);
					printf("%d", Delay);
				}
			if (ch == SPACE) { // 만약 스페이스라면

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
						enemynum = +3;
					}
				}
				if (hardness % 700 == 0) {
					if (Delay > 12) {
						Delay--;
						if (frame > 3) {
							frame--;
						}
						enemynum = +10;
					}
				}
				SetColor(WHITE, 0);
				gotoxy(32, 15);
				printf("%d", hardness);
				gotoxy(32, 17);
				printf("%d", Delay);


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



		}
		Sleep(Delay);// Delay를 줄이면 속도가 빨라진다.

	} //움직이는 소스

	_getch();
	return 0;
}

*/