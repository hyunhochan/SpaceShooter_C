/*

// omok.c ������ ���� �������� ǥ��
#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>

// ���� ����
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

#define SPECIAL1 0xe0 // Ư��Ű�� 0xe0 + key ������ �����ȴ�.
#define SPECIAL2 0x00 // keypad ��� 0x00 + key �� �����ȴ�.

#define UP  0x48 // Up key�� 0xe0 + 0x48 �ΰ��� ���� ���´�.
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

struct { //����� ����ü(���۵� x)
	BOOL exist;
	int type;
	int x, y;
	int nFrame;
	int nStay;
} Enemy[MAXENEMY];

char *arrEnemy[] = {
	"(��)"
}; //����� �迭(���۵� X)

int Delay = 35;
int mainpx = 35;
int bulletx = -1;
int pullety = -1;
int hardness = 51;
int life = 100;
double score = 0.0;
int enemynum = 10;
int frame = 10;

void removeCursor(void) { // Ŀ���� �Ⱥ��̰� �Ѵ�

	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

void gotoxy(int x, int y) //���� ���ϴ� ��ġ�� Ŀ�� �̵�
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);// WIN32API �Լ��Դϴ�. �̰� ���ʿ� �����
}

void erasestar(int x, int y)
{
	gotoxy(x, y);
	//putchar(' ');//(BLANK);
	//printf("  ");
	printf("��");
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
// ȭ�� ������ ���ϴ� �������� �����Ѵ�.

void VertLine() {
	int i;

	for (i = 0; i < 40; i++) {
		SetColor(WHITE, 0);
		gotoxy(70, i);
		printf("��");
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
				printf("��");

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
				printf("��");

			}
			//Sleep(1);
		}
		printf("\n");
		gotoxy(27, 12 + i);
	}

	gotoxy(63, 7); printf("ver. 1.0");
	gotoxy(30, 19); printf("��������<    Instructions    >��������");
	gotoxy(30, 20); printf("��                                  ��");
	gotoxy(30, 21); printf("��    \"Just Click Space to PLAY\"    ��");
	gotoxy(30, 22); printf("��                                  ��");
	gotoxy(30, 23); printf("��      <     I T E M S      >      ��");
	gotoxy(30, 24); printf("��                                  ��");
	gotoxy(30, 25); printf("��     BLUE Diamond =     Lader     ��");
	gotoxy(30, 26); printf("��   Yellow Diamond =     LASER     ��");
	gotoxy(30, 27); printf("��    Green Diamond =   Rapid Fire  ��");
	gotoxy(30, 27); printf("��    Green Diamond =   Rapid Fire  ��");
	gotoxy(30, 28); printf("��   White Diammond =  Clear field  ��");
	gotoxy(30, 29); printf("��                                  ��");
	gotoxy(30, 30); printf("��������������������������������������");

	gotoxy(37, 34);
	printf("Press Any Key to Start");
	_getch();


}

void DrawPlayer() {
	SetColor(GREEN1, 0);
	gotoxy(mainpx, 37);
	printf("��");
}

void ErasePlayer() {
	gotoxy(mainpx, 37);
	printf(" ");
}

void MovePlayer()
{
	gotoxy(mainpx, 37); //������ǥ
	ErasePlayer();

	DrawPlayer();

}

void DrawBullet(int i) {
	SetColor(YELLOW1, 0);
	gotoxy(Bullet[i].x, Bullet[i].y);
	printf("��");
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

	// ���� ����
	// ���� ���� ��Ƽ� �ı��Ǹ� �ﰢ ������ ������� �Ǵµ�,
	// ���� �Ѿ˿� ��Ƽ� �ı��Ǹ� ������ �ٷ� �������� �ʰ� �� ��ٷ��� ������.

	int i;

	if (rand() % 300 < enemynum) { //rand()%350�� ���� enemynum���� ���� ������ ������, �� enemynum/300�� Ȯ����, �� ���� �߰���.
		for (i = 0; i <= MAXENEMY && Enemytemp[i].exist == TRUE && Enemytemp[i].dying == 1; i++) {
			; // 8���� ������ ���¶�� i�� 9�� ��. �׷� ������ �� �̰�. �׸��� �߰��� exist�� false�� �ָ� ������(�� ������ ã�Ƶ�) �������´�.
		}

		if (i == MAXENEMY) { //MAXENEMY�� �Ѿ�� �ٽ� i�� 0���� �ٲ㼭 �ٽ� ��½�Ŵ. �� ��ġ�� ��� �� ���� �ִµ� �� ���� �̹� �Ʒ��� ����ھƼ� ��������?
			i = 0;
		}

		if (i < MAXENEMY && Enemytemp[i].exist == FALSE) { //������ �޾Ƽ�, ������� i�� 9�ε� 9�� MAXENEMY���� ���� �׾ �迭�� �ڸ��� �� ���¶��

			Enemytemp[i].x = (rand() % 22) * 3;
			Enemytemp[i].y = 0;

			Enemytemp[i].frame = Enemytemp[i].stay = frame; //frame�� 10.
			Enemytemp[i].timeafterdead = 0;
			Enemytemp[i].exist = TRUE;
			Enemytemp[i].havedied = 0;
			Enemytemp[i].dying = 0;


			SetColor(RED1, 0);
			gotoxy(Enemytemp[i].x, 0);
			printf("��");

		}

	}
}

void MoveEnemy() {

	// stay ��������� ���� �װ��� Ư�� ������ 1�� �����ϴٰ�
	// ���߿� 0�� �ǹ����� �׶����� �� ĭ�� �̵��ϰ� ����� ������ �����̿� �ӵ��� �����ϸ� ��?
	// ��, �׸��� stay�� 0�� �Ǹ� �ٽ� Ư�� ������ �ǵ������ �ϴϱ� frame�̶�� ������ ���� ���� stay�� 0�� �� ������ �������� �� ����.

	// ���� ������ : enemy�� y��ǥ 40 �̻����� �������� ���� ������ ������� �ʰ� ���װ� �ɸ�.
	// �׸��� ���ο� enemy�� MAXENEMY �̻����� ��µ��� ����.
	// �׸��� ���� ���ο� ģ���� ����� y=0���� �������� �ʰ� �ű� �ܻ��� ���� �� �������� �ؿ� ������ ģ�� ��/��/�ڿ� ��� �پ �ޱ�.
	// ���� �Ŀ� ���� ��ġ�� ���� ������ �ϴµ� �Ѿ� ��ġ�� ���� ���� ���� �޽����� ��µȴ�.
	//------------�ذ�-------------


	// ������, �� ���ʿ� �پ ���� ģ���� �״� ó���� �ȵǴ°��� �����ϰ�
	// �׸��� �� enemy���� �پ �� ���� �ȵǴ°Ŵ�?
	//------------�ذ�-------------


	// ��, ���� ������ �Ѿ� �i�µ� enemy�� ���״´�.
	// ���� ������ erasecrash()�� �ȸԴ´�.
	//-----------�ù� ���� �̰� �ٽô� �ذ��Ϸ��� �ϳ� ����------------

	int i;

	for (i = 0; i < MAXENEMY; i++) {
		if (Enemytemp[i].exist == FALSE) {


			continue; //Enemytemp[i]�� exist���� �����Ƿ� ��°�� ��ŵ�Ѵ�.
		}
		if (Enemytemp[i].stay == 0) { //stay�� 0�� �ƴϸ� �� �� ���� ��ŵ�Ѵ�. �׸��� 1 �ٿ���.
			Enemytemp[i].stay = Enemytemp[i].frame; //stay�� 0�� �Ǹ� �����ϸ�, stay�� �ٽ� frame�� ����ȭ ���ѵд�.

			if (Enemytemp[i].y > 38) { //y����� 38�� �Ѿ������ �� �״°��� ��.


				if (Enemytemp[i].x > 2) { //�Ϲ����� ���
					gotoxy(Enemytemp[i].x - 2, Enemytemp[i].y); //������ ���� ��ġ ����
					SetColor(BLUE1, 0);
					printf("crashed"); // ����ų� ���� ������ ����ų�.
				}
				else { // ���� �پ �� ���
					gotoxy(0, Enemytemp[i].y); //������ ���� ��ġ ����
					SetColor(BLUE1, 0);
					printf("crashed"); // ����ų� ���� ������ ����ų�.
				}

				// 3������ �ڿ� crashed ����� �ҽ�/////////////////////////////////////////////////////////

				Enemytemp[i].exist = FALSE; //�� �����ŷ� ��������.
				Enemytemp[i].havedied = 1;
				if (Enemytemp[i].exist == FALSE && Enemytemp[i].timeafterdead < 2 && Enemytemp[i].havedied == 1) {
					Enemytemp[i].timeafterdead++;
				}
				if (Enemytemp[i].timeafterdead == 2) {

					if (Enemytemp[i].x > 2) { //�Ϲ����� ���
						gotoxy(Enemytemp[i].x - 2, Enemytemp[i].y); //������ ���� ��ġ ����
						printf("         "); // ����ų� ���� ������ ����ų�.
					}
					else { // ���� �پ �� ���
						gotoxy(0, Enemytemp[i].y); //������ ���� ��ġ ����
						printf("         "); // ����ų� ���� ������ ����ų�.
					}

					Enemytemp[i].x = (rand() % 22) * 3; //��ġ �ʱ�ȭ
					Enemytemp[i].y = 0; //��ġ �ʱ�ȭ
					Enemytemp[i].timeafterdead = 0; //time �ʱ�ȭ
					Enemytemp[i].havedied = 0; //havedied �ʱ�ȭ
				}

				/////////////////////////////////////////////////////////////////////////////////////////////////

			}
			else { // y�� ���� 40�� ���� ������(ȭ�� ���� ������)
				Enemytemp[i].y++; // �ش� ������ [y��ǥ�� 1 ����]��Ű��.//////////////////////////////////////////////////////��ġ����

				if (Enemytemp[i].x < 3) { //enemy�� ���� ���� �پ��� ���
					gotoxy(0, Enemytemp[i].y - 1); // y��ǥ 1 ������Ű�� �� ��ġ�� ����
					printf("  "); //��������� ���� �غ��س���.
				}
				else {
					gotoxy(Enemytemp[i].x, Enemytemp[i].y - 1); // y��ǥ 1 ������Ű�� �� ��ġ�� ����
					printf("  "); //��������� ���� �غ��س���.
				}
			}
			SetColor(RED1, 0); //�� �ٲ�ΰ�

			if (Enemytemp[i].exist == TRUE) { //�ƴ� ����־�� �� �� �� �ƴѰ�?

				gotoxy(Enemytemp[i].x, Enemytemp[i].y); //y��ǥ 1 ������ ������ ����
				printf("��"); //���� �ٽ� �������(�̵���Ű��).
			}
			else {
				Enemytemp[i].exist = TRUE; // ���� bullet�� ��������.
			}
		}

		Enemytemp[i].stay--; //stay�� �ٿ��� ������ 0�̵ż� �����ϰ� �ƴѰ�

	}

}

void EnemyCollide() {

	//���� : ����Ƽ�� �ƴϴ�. �� ������ �ȿ� ��� �ൿ���� �����ؾ� �Ѵٴ� �� ���� ����.
	// ��, ���� ������ �Ѿ� ���µ� enemy�� ���״´�. �ֱ׷���?
	// �� ��쿡�� ������ �װ� ���� ���� �����ӿ� ������ż� �� �����´�.

	int i, j;

	for (i = 0; i < MAXENEMY; i++) { //enemy ����
		if (Enemytemp[i].exist == FALSE) {
			continue; //Enemytemp[i]�� exist���� �����Ƿ� ��°�� ��ŵ�Ѵ�.
		}

		for (j = 0; j < MAXBULLET; j++) { //bullet ����

			if (abs(Enemytemp[i].x - Bullet[j].x) <= 2 && abs(Enemytemp[i].y - Bullet[j].y) <= 1 && Bullet[j].exist == TRUE) { //�Ѿ˰� ���� y��ǥ�� ����, x��ǥ�� ����ϸ�, �Ѿ��� ��ȿ������ ���
				if (Enemytemp[i].x > 2) { // �Ϲ����� ���



					Bullet[j].exist = FALSE; //�Ѿ˵� �����ŷ� ��������.
					gotoxy(Bullet[j].x, Bullet[j].y);
					printf("  ");
					gotoxy(Bullet[j].x, Bullet[j].y + 1);
					printf("  ");


					gotoxy(Enemytemp[i].x - 2, Enemytemp[i].y); //������ ���� ��ġ ����
					SetColor(BLUE1, 0);
					printf("+10��"); // ����ų� ���� ������ ����ų�.

				}
				else { // ���� �پ ���� ���



					Bullet[j].exist = FALSE; //�Ѿ˵� �����ŷ� ��������.
					gotoxy(Bullet[j].x, Bullet[j].y);
					printf("  ");
					gotoxy(Bullet[j].x, Bullet[j].y + 1);
					printf("  ");

					gotoxy(0, Enemytemp[i].y); //������ ���� ��ġ ����
					SetColor(BLUE1, 0);
					printf("+10��"); // ����ų� ���� ������ ����ų�.



				}

				Enemytemp[i].exist = FALSE; //�� �����ŷ� ��������.
				Enemytemp[i].havedied = 1;
				Enemytemp[i].dying = 1;


			}

		}

	}

}

void erasecrash() { // '�Ѹ¾� ����' enemy���� ���� crashed ���ű�
	int i;
	for (i = 0; i < MAXENEMY; i++) { //enemy ã��
		if (Enemytemp[i].exist == FALSE && Enemytemp[i].timeafterdead < 10 && Enemytemp[i].havedied == 1 && Enemytemp[i].dying == 1) { //���� ���¸�
			Enemytemp[i].timeafterdead++;
			Enemytemp[i].timeafterdead++;//time 1�� ������Ű��.

			//����� �ҽ�
			//gotoxy(Enemytemp[i].x, Enemytemp[i].y+1); //������ ���� ��ġ ����
			//printf("time %d, isdead %d, dying %d, Exist %d, stay %d frame %d  ", Enemytemp[i].timeafterdead, Enemytemp[i].havedied, Enemytemp[i].dying, Enemytemp[i].exist, Enemytemp[i].stay, Enemytemp[i].frame); // ����ų� ���� ������ ����ų�.
			
			// ����� ���, ���������� �õ��� timeafterdead�� �����ϴٰ� 4~6 ���̿��� ���Ȱ� ���ߴ� ���� Ȯ���� �� �־���.
			// �̸� ���� ȭ���ϳ� �ѹ� �ذ��� ������ ����.
		}

		if (Enemytemp[i].timeafterdead >= 10) { //������ 10��ŭ ������ crashed �����.

			if (Enemytemp[i].x > 2) { //�Ϲ����� ���
				gotoxy(Enemytemp[i].x - 2, Enemytemp[i].y); //������ ���� ��ġ ����
				printf("         "); // ����ų� ���� ������ ����ų�.
			}
			else { // ���� ���� �پ �� ���
				gotoxy(0, Enemytemp[i].y); //������ ���� ��ġ ����
				printf("         "); // ����ų� ���� ������ ����ų�.
			}

			Enemytemp[i].x = (rand() % 22) * 3;
			Enemytemp[i].y = 0;

			Enemytemp[i].frame = Enemytemp[i].stay = frame; //frame�� 10.
			Enemytemp[i].timeafterdead = 0;
			Enemytemp[i].exist = TRUE;
			Enemytemp[i].havedied = 0;
			//Enemytemp �����ϴ� ��� ���⼭ �ʱ�ȭ���Ѽ� ��������.
		}
	}

}

void DrawItem() {

	// ���� ����
	// ���� ���� ��Ƽ� �ı��Ǹ� �ﰢ ������ ������� �Ǵµ�,
	// ���� �Ѿ˿� ��Ƽ� �ı��Ǹ� ������ �ٷ� �������� �ʰ� �� ��ٷ��� ������.

	int i;
	int itemtype;

	itemtype = rand() % 10 + 1; //itemtype�� 1, 2, 3, 4 �� �ϳ��� ������, ������� �������� ���� Ȯ���� �����ؼ� 4:3:2:1�� ������ �����ϵ��� ��.
	if (rand() % 1000 < 700) { //rand()%350�� ���� Itemnum���� ���� ������ ������, �� 7/1000�� Ȯ����, �� �������� �߰���.
		for (i = 0; i <= 7 && Itemtemp[i].exist == TRUE && Itemtemp[i].dying == 1; i++) {
			; // 8���� ������ ���¶�� i�� 9�� ��. �׷� ������ �� �̰�. �׸��� �߰��� exist�� false�� �ָ� ������(�� ������ ã�Ƶ�) �������´�.
		}

		if (i == 7) { //7�� �Ѿ�� �ٽ� i�� 0���� �ٲ㼭 �ٽ� ��½�Ŵ. �� ��ġ�� ��� �� ���� �ִµ� �� ���� �̹� �Ʒ��� ����ھƼ� ��������?
			i = 0;
		}

		if (i < 7 && Itemtemp[i].exist == FALSE) { //������ �޾Ƽ�, ������� i�� 9�ε� 9�� 7���� ���� �׾ �迭�� �ڸ��� �� ���¶��

			switch (itemtype) {

			case 1:
			case 2:
			case 3:
			case 4:

				Itemtemp[i].x = (rand() % 22) * 3;
				Itemtemp[i].y = 0;

				Itemtemp[i].frame = Itemtemp[i].stay = 4; //frame�� 10.
				Itemtemp[i].type = itemtype;
				Itemtemp[i].timeafterdead = 0;
				Itemtemp[i].exist = TRUE;
				Itemtemp[i].havedied = 0;
				Itemtemp[i].dying = 0;


				SetColor(BLUE1, 0);
				gotoxy(Itemtemp[i].x, 0);
				printf("��");
				break;


			case 5:
			case 6:
			case 7:

				Itemtemp[i].x = (rand() % 22) * 3;
				Itemtemp[i].y = 0;

				Itemtemp[i].frame = Itemtemp[i].stay = 4; //frame�� 10.
				Itemtemp[i].type = itemtype;
				Itemtemp[i].timeafterdead = 0;
				Itemtemp[i].exist = TRUE;
				Itemtemp[i].havedied = 0;
				Itemtemp[i].dying = 0;


				SetColor(YELLOW2, 0);
				gotoxy(Itemtemp[i].x, 0);
				printf("��");

				break;


			case 8:
			case 9:

				Itemtemp[i].x = (rand() % 22) * 3;
				Itemtemp[i].y = 0;

				Itemtemp[i].frame = Itemtemp[i].stay = 4; //frame�� 10.
				Itemtemp[i].type = itemtype;
				Itemtemp[i].timeafterdead = 0;
				Itemtemp[i].exist = TRUE;
				Itemtemp[i].havedied = 0;
				Itemtemp[i].dying = 0;


				SetColor(GREEN2, 0);
				gotoxy(Itemtemp[i].x, 0);
				printf("��");

				break;


			case 10:

				Itemtemp[i].x = (rand() % 22) * 3;
				Itemtemp[i].y = 0;

				Itemtemp[i].frame = Itemtemp[i].stay = 4; //frame�� 10.
				Itemtemp[i].type = itemtype;
				Itemtemp[i].timeafterdead = 0;
				Itemtemp[i].exist = TRUE;
				Itemtemp[i].havedied = 0;
				Itemtemp[i].dying = 0;


				SetColor(WHITE, 0);
				gotoxy(Itemtemp[i].x, 0);
				printf("��");

				break;


			default:
				break;




			}



		}

	}
}

void MoveItem() {

	// stay ��������� ���� �װ��� Ư�� ������ 1�� �����ϴٰ�
	// ���߿� 0�� �ǹ����� �׶����� �� ĭ�� �̵��ϰ� ����� ������ �����̿� �ӵ��� �����ϸ� ��?
	// ��, �׸��� stay�� 0�� �Ǹ� �ٽ� Ư�� ������ �ǵ������ �ϴϱ� frame�̶�� ������ ���� ���� stay�� 0�� �� ������ �������� �� ����.

	// ���� ������ : Item�� y��ǥ 40 �̻����� �������� ���� ������ ������� �ʰ� ���װ� �ɸ�.
	// �׸��� ���ο� Item�� 7 �̻����� ��µ��� ����.
	// �׸��� ���� ���ο� ģ���� ����� y=0���� �������� �ʰ� �ű� �ܻ��� ���� �� �������� �ؿ� ������ ģ�� ��/��/�ڿ� ��� �پ �ޱ�.
	// ���� �Ŀ� ���� ��ġ�� ���� ������ �ϴµ� �Ѿ� ��ġ�� ���� ���� ���� �޽����� ��µȴ�.
	//------------�ذ�-------------


	// ������, �� ���ʿ� �پ ���� ģ���� �״� ó���� �ȵǴ°��� �����ϰ�
	// �׸��� �� Item���� �پ �� ���� �ȵǴ°Ŵ�?
	//------------�ذ�-------------


	// ��, ���� ������ �Ѿ� �i�µ� Item�� ���״´�.
	// ���� ������ erasecrash()�� �ȸԴ´�.
	//-----------�ù� ���� �̰� �ٽô� �ذ��Ϸ��� �ϳ� ����------------

	int i;

	for (i = 0; i < 7; i++) {
		if (Itemtemp[i].exist == FALSE) {


			continue; //Itemtemp[i]�� exist���� �����Ƿ� ��°�� ��ŵ�Ѵ�.
		}
		if (Itemtemp[i].stay == 0) { //stay�� 0�� �ƴϸ� �� �� ���� ��ŵ�Ѵ�. �׸��� 1 �ٿ���.
			Itemtemp[i].stay = Itemtemp[i].frame; //stay�� 0�� �Ǹ� �����ϸ�, stay�� �ٽ� frame�� ����ȭ ���ѵд�.

			if (Itemtemp[i].y > 38) { //y����� 38�� �Ѿ������ �� �״°��� ��.


				if (Itemtemp[i].x > 2) { //�Ϲ����� ���
					gotoxy(Itemtemp[i].x - 2, Itemtemp[i].y); //������ ���� ��ġ ����
					SetColor(BLUE1, 0);
					printf("crashed"); // ����ų� ���� ������ ����ų�.
				}
				else { // ���� �پ �� ���
					gotoxy(0, Itemtemp[i].y); //������ ���� ��ġ ����
					SetColor(BLUE1, 0);
					printf("crashed"); // ����ų� ���� ������ ����ų�.
				}

				// 3������ �ڿ� crashed ����� �ҽ�/////////////////////////////////////////////////////////

				Itemtemp[i].exist = FALSE; //�� �����ŷ� ��������.
				Itemtemp[i].havedied = 1;
				if (Itemtemp[i].exist == FALSE && Itemtemp[i].timeafterdead < 2 && Itemtemp[i].havedied == 1) {
					Itemtemp[i].timeafterdead++;
				}
				if (Itemtemp[i].timeafterdead == 2) {

					if (Itemtemp[i].x > 2) { //�Ϲ����� ���
						gotoxy(Itemtemp[i].x - 2, Itemtemp[i].y); //������ ���� ��ġ ����
						printf("         "); // ����ų� ���� ������ ����ų�.
					}
					else { // ���� �پ �� ���
						gotoxy(0, Itemtemp[i].y); //������ ���� ��ġ ����
						printf("         "); // ����ų� ���� ������ ����ų�.
					}

					Itemtemp[i].x = (rand() % 22) * 3; //��ġ �ʱ�ȭ
					Itemtemp[i].y = 0; //��ġ �ʱ�ȭ
					Itemtemp[i].timeafterdead = 0; //time �ʱ�ȭ
					Itemtemp[i].havedied = 0; //havedied �ʱ�ȭ
					Itemtemp[i].exist = FALSE;
				}

				/////////////////////////////////////////////////////////////////////////////////////////////////

			}
			else { // y�� ���� 40�� ���� ������(ȭ�� ���� ������)
				Itemtemp[i].y++; // �ش� ������ [y��ǥ�� 1 ����]��Ű��.//////////////////////////////////////////////////////��ġ����

				if (Itemtemp[i].x < 3) { //Item�� ���� ���� �پ��� ���
					gotoxy(0, Itemtemp[i].y - 1); // y��ǥ 1 ������Ű�� �� ��ġ�� ����
					printf("  "); //��������� ���� �غ��س���.
				}
				else {
					gotoxy(Itemtemp[i].x, Itemtemp[i].y - 1); // y��ǥ 1 ������Ű�� �� ��ġ�� ����
					printf("  "); //��������� ���� �غ��س���.
				}
			}
			SetColor(RED1, 0); //�� �ٲ�ΰ�

			if (Itemtemp[i].exist == TRUE) { //�ƴ� ����־�� �� �� �� �ƴѰ�?
				switch (Itemtemp[i].type) {

				case 1:
				case 2:
				case 3:
				case 4:
					gotoxy(Itemtemp[i].x, Itemtemp[i].y); //y��ǥ 1 ������ ������ ����
					SetColor(BLUE1, 0);
					printf("��"); //���� �ٽ� �������(�̵���Ű��).
					break;

				case 5:
				case 6:
				case 7:
					gotoxy(Itemtemp[i].x, Itemtemp[i].y); //y��ǥ 1 ������ ������ ����
					SetColor(YELLOW2, 0);
					printf("��"); //���� �ٽ� �������(�̵���Ű��).
					break;

				case 8:
				case 9:
					gotoxy(Itemtemp[i].x, Itemtemp[i].y); //y��ǥ 1 ������ ������ ����
					SetColor(GREEN2, 0);
					printf("��"); //���� �ٽ� �������(�̵���Ű��).
					break;

				case 10:
					gotoxy(Itemtemp[i].x, Itemtemp[i].y); //y��ǥ 1 ������ ������ ����
					SetColor(WHITE, 0);
					printf("��"); //���� �ٽ� �������(�̵���Ű��).
					break;


				}
			}

		}

		Itemtemp[i].stay--; //stay�� �ٿ��� ������ 0�̵ż� �����ϰ� �ƴѰ�

	}

}

void ItemCollide() {

	//���� : �� ������ �ȿ� ��� �ൿ���� �����ؾ� �Ѵٴ� �� ���� ����.
	// ��, ���� ������ �Ѿ� ���µ� Item�� ���״´�. �ֱ׷���?
	// �� ��쿡�� ������ �װ� ���� ���� �����ӿ� ������ż� �� �����´�.

	int i, j;

	for (i = 0; i < 7; i++) { //Item ����
		if (Itemtemp[i].exist == FALSE) {
			continue; //Itemtemp[i]�� exist���� �����Ƿ� ��°�� ��ŵ�Ѵ�.
		}

		for (j = 0; j < MAXBULLET; j++) { //bullet ����

			if (abs(Itemtemp[i].x - Bullet[j].x) <= 2 && abs(Itemtemp[i].y - Bullet[j].y) <= 1 && Bullet[j].exist == TRUE) { //�Ѿ˰� ���� y��ǥ�� ����, x��ǥ�� ����ϸ�, �Ѿ��� ��ȿ������ ���
				if (Itemtemp[i].x > 2) { // �Ϲ����� ���



					Bullet[j].exist = FALSE; //�Ѿ˵� �����ŷ� ��������.
					gotoxy(Bullet[j].x, Bullet[j].y);
					printf("  ");
					gotoxy(Bullet[j].x, Bullet[j].y + 1);
					printf("  ");

					switch (Itemtemp[i].type) {

					case 1:
					case 2:
					case 3:
					case 4:
						gotoxy(Itemtemp[i].x, Itemtemp[i].y); // ������ ���� ��ġ�� ����
						SetColor(BLUE1, 0);
						printf("��"); //���� �ٽ� �������(�̵���Ű��).
						break;

					case 5:
					case 6:
					case 7:
						gotoxy(Itemtemp[i].x, Itemtemp[i].y); //������ ���� ��ġ�� ����
						SetColor(YELLOW2, 0);
						printf("��"); //���� �ٽ� �������(�̵���Ű��).
						break;

					case 8:
					case 9:
						gotoxy(Itemtemp[i].x, Itemtemp[i].y); //������ ���� ��ġ�� ����
						SetColor(GREEN2, 0);
						printf("��"); //���� �ٽ� �������(�̵���Ű��).
						break;

					case 10:
						gotoxy(Itemtemp[i].x, Itemtemp[i].y); //������ ���� ��ġ�� ����
						SetColor(WHITE, 0);
						printf("��"); //���� �ٽ� �������(�̵���Ű��).
						break;


					}


				}
				else { // ���� �پ ���� ���



					Bullet[j].exist = FALSE; //�Ѿ˵� �����ŷ� ��������.
					gotoxy(Bullet[j].x, Bullet[j].y);
					printf("  ");
					gotoxy(Bullet[j].x, Bullet[j].y + 1);
					printf("  ");

					switch (Itemtemp[i].type) {

					case 1:
					case 2:
					case 3:
					case 4:
						gotoxy(Itemtemp[i].x, Itemtemp[i].y); //������ ���� ��ġ ����
						SetColor(BLUE1, 0);
						printf("��"); //���� �ٽ� �������(�̵���Ű��).
						break;

					case 5:
					case 6:
					case 7:
						gotoxy(Itemtemp[i].x, Itemtemp[i].y); //������ ���� ��ġ ����
						SetColor(YELLOW2, 0);
						printf("��"); //���� �ٽ� �������(�̵���Ű��).
						break;

					case 8:
					case 9:
						gotoxy(Itemtemp[i].x, Itemtemp[i].y); //������ ���� ��ġ ����
						SetColor(GREEN2, 0);
						printf("��"); //���� �ٽ� �������(�̵���Ű��).
						break;

					case 10:
						gotoxy(Itemtemp[i].x, Itemtemp[i].y); //������ ���� ��ġ ����
						SetColor(WHITE, 0);
						printf("��"); //���� �ٽ� �������(�̵���Ű��).
						break;


					}




				}

				Itemtemp[i].exist = FALSE; //�� �����ŷ� ��������.
				Itemtemp[i].havedied = 1;
				Itemtemp[i].dying = 1;


			}

		}

	}

}

void eraseItemcrash() { // '�Ѹ¾� ����' enemy���� ���� crashed ���ű�
	int i;
	for (i = 0; i < 7; i++) { //Item ã��
		if (Itemtemp[i].exist == FALSE && Itemtemp[i].timeafterdead < 10 && Itemtemp[i].havedied == 1 && Itemtemp[i].dying == 1) { //���� ���¸�
			Itemtemp[i].timeafterdead++;
			Itemtemp[i].timeafterdead++;//time 1�� ������Ű��.

			//����� �ҽ�
			//gotoxy(Itemtemp[i].x, Itemtemp[i].y+1); //������ ���� ��ġ ����
			//printf("time %d, isdead %d, dying %d, Exist %d, stay %d frame %d  ", Itemtemp[i].timeafterdead, Itemtemp[i].havedied, Itemtemp[i].dying, Itemtemp[i].exist, Itemtemp[i].stay, Itemtemp[i].frame); // ����ų� ���� ������ ����ų�.
			
			// ����� ���, ���������� �õ��� timeafterdead�� �����ϴٰ� 4~6 ���̿��� ���Ȱ� ���ߴ� ���� Ȯ���� �� �־���.
			// �̸� ���� ȭ���ϳ� �ѹ� �ذ��� ������ ����.
		}

		if (Itemtemp[i].timeafterdead >= 10) { //������ 10��ŭ ������ crashed �����.

			if (Itemtemp[i].x > 2) { //�Ϲ����� ���
				gotoxy(Itemtemp[i].x, Itemtemp[i].y); //������ ���� ��ġ ����
				printf("      "); // ����ų� ���� ������ ����ų�.
			}
			else { // ���� ���� �پ �� ���
				gotoxy(Itemtemp[i].x, Itemtemp[i].y); //������ ���� ��ġ ����
				printf("      "); // ����ų� ���� ������ ����ų�.
			}

			Itemtemp[i].x = (rand() % 22) * 3;
			Itemtemp[i].y = 0;

			Itemtemp[i].frame = Itemtemp[i].stay = frame; //frame�� 10.
			Itemtemp[i].timeafterdead = 0;
			Itemtemp[i].type = rand() % 10 + 1;
			Itemtemp[i].exist = FALSE;
			Itemtemp[i].havedied = 0;
			//Itemtemp �����ϴ� ��� ���⼭ �ʱ�ȭ���Ѽ� ��������.
		}
	}

}

void DrawEnemy1() {
	BOOL Found;
	int i, j;

	//���� : �� �ҽ��ڵ�� ���� �¿�� �̵��Ѵ�. �� ���߿� �Ʒ��θ� �̵��ϵ��� �ٲ� ��

	if (rand() % 10 < enemynum) { //rand()%100�� ���� enemynum���� ���� ������ ������, �� enemynum/100�� Ȯ����.
		for (i = 0; i < 7 && Enemy[i].exist == TRUE; i++) {
			; //8���� ������ ���¶�� i�� 9�� ��. �׷� ������ �� �̰�.
		}

		if (i != 7) { //������ �޾Ƽ�, ������� i�� 9�ε� 9�� 7���� ���ٸ�-

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
//����� �ڵ�, �� �۵�x

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
//����� �ڵ�, �� �۵�x

int main() {

	unsigned char last_ch, ch;
	int oldx, oldy, newx, newy;
	int keep_moving;
	int i;
	int bullet = 0;
	int bulletspeed = 1;


	////////////���۶���
	int example;
	srand(time(NULL)); // �����ʱ�ȭ
	example = rand() % 6; //����()������
	//6���γ���������(0~5) ���ϳ����ȴ�.
	////////////���۶���


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
	last_ch = RIGHT; // �ϴ� ���������� �̵��Ѵ�. 

	while (1) {
		if (_kbhit() == 1) {  // Ű���尡 ������ ������
			char c1;
			c1 = _getch(); // key ���� �д´�
			if (c1 == '\0') // VS �� NULL ���� Error�̴�.
				continue;
			else
				ch = c1; // NULL�� �ƴϸ� ���� �Է��̴�.
			// ESC ������ ���α׷� ����
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
			if (ch == SPACE) { // ���� �����̽����

				if (bullet > MAXBULLET) bullet = 0;
				//�Ʒ�, �Ѿ˸��� �߻��ϵ簡���簡
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


				switch (last_ch) { //last_ch�� üũ�� ��
				case RIGHT: //RIGHT�� ��쿡��
					last_ch = LEFT; //LEFT�� ������ȯ ��Ű��...(����)
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
		if (keep_moving) { // �����̰� ������
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
		Sleep(Delay);// Delay�� ���̸� �ӵ��� ��������.

	} //�����̴� �ҽ�

	_getch();
	return 0;
}

*/