#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <string.h>
#include <time.h>

#define CLCD "/dev/clcd"
#define TACT "/dev/tactsw"
#define DOT "/dev/dot"
#define FND "/dev/fnd" 	
#define DIP "/dev/dipsw"

int dipsw;
int clcds;

unsigned char fnd_num[4] = { 0, };	// 7-Segment 값 변수 

// 7-Segment의 0~9의 출력 값
// 참고로 음수 값으로 해야 제대로 출력됨 
unsigned char Time_Table[] = { ~0x3f,~0x06,~0x5b,~0x4f,~0x66,~0x6d,~0x7d,~0x07,~0x7f,~0x67,~0x00 };

// Timer 관련 변수 
int tm = 0; 	// Timer Minute 
int ts = 0;	// Timer Second(10의 자리 수) 
int* ptr_m;	// tm Point 
int* ptr_s;	// ts Point

// time() 이용한 Timer 설정을 위해 선언
// endTime은 고정, startTime은 흘러가게 하고 두 값의 차로 초 단위 타이머 구현 
int endTime;
int startTime;
int fnds;

unsigned char pattern[8] = {
    0xFF,  // 11111111
    0xFF,  // 11111111
    0xFF,  // 11111111
    0xFF,  // 11111111
    0xFF,  // 11111111
    0xF0,  // 11110000
    0x00,  // 00000000
    0x00   // 00000000
};

int update_dot_matrix() {
    // 패턴의 랜덤한 위치를 0x00으로 변경
    int index;
    do {
        index = rand() % 8;
    } while (pattern[index] == 0x00);

    int previous_value = pattern[index];
    pattern[index] = 0x00;

    return previous_value;
}

void display_updated_dot_matrix(int time_sleep) {
    int dot_d;

    dot_d = open(DOT, O_RDWR);
    if (dot_d < 0) {
        printf("dot Error\n");
        return; // 예외처리
    }

    write(dot_d, pattern, sizeof(pattern)); // 패턴 출력
    sleep(time_sleep);

    close(dot_d);
}

void display_winner(int clcd_dev, int player) {
    char* win_text = (player == 1) ? "Player 2 Win" : "Player 1 Win";
    write(clcd_dev, win_text, strlen(win_text)); // 승리 메시지 출력
    sleep(3); // 승리 메시지를 3초간 표시
    exit(0); // 게임 종료
}

// 게임 시작 시 CLCD에 출력하는 함수
// 그냥 PRINT랑 다른 점은 Tact Switch 입력 시 return 
int FIRST_PRINT() {
    unsigned char d; 			// Tact Switch 값 변수

    clcds = open(CLCD, O_RDWR);
    if (clcds < 0) { printf("Can't open Character LCD.\n"); exit(0); }
    char pr_clcd[40] = "RUSSIAN ROULETTEPRESS DIP SWITCH! ";
    write(clcds, pr_clcd, strlen(pr_clcd));
    close(clcds);

    while (1) {
        dipsw = open(DIP, O_RDWR);
        if (DIP < 0) { printf("Can't open dip\n"); exit(0); }
        read(dipsw, &d, sizeof(d));
        close(dipsw);

        if (d != 0) {
            return 0;
        }
    }
}

// CLCD 출력 함수 
int PRINT(char P[]) {
    clcds = open(CLCD, O_RDWR);
    if (clcds < 0) { printf("Can't open Character LCD.\n"); exit(0); }
    write(clcds, P, strlen(P));
    close(clcds);
}




int main() {
    int clcd_dev, tact_dev;
    char* player1_text = "Player 1";
    char* player2_text = "Player 2";
    char* player1_win_text = "Player 1 Win";
    char* player2_win_text = "Player 2 Win";
    unsigned char tact_data[2];
    int player = 1; // 처음 시작은 플레이어 1의 차례
    struct timeval start, end;
    long elapsed_time;

    FIRST_PRINT();
    PRINT("   GAME START!   ");
    usleep(2000000);

    endTime = (unsigned)time(NULL) + 70; 	// endTime에 time 값(19070.01.01)과 70초 할당 
    ptr_s = &ts;							// ts 위치 할당 
    ptr_m = &tm;							// tm 위치 할당

    clcd_dev = open(CLCD, O_RDWR); // CLCD 장치 열기
    if (clcd_dev < 0) {
        printf("Can't open CLCD.\n");
        exit(0); // 장치를 못 불러올 경우 예외 처리 후 종료
    }

    tact_dev = open(TACT, O_RDONLY); // Tact switch 장치 열기
    if (tact_dev < 0) {
        printf("Can't open Tact switch.\n");
        close(clcd_dev);
        exit(0); // 장치를 못 불러올 경우 예외 처리 후 종료
    }

    srand(time(NULL)); // 난수 생성 초기화

    while (1) {
        // 현재 플레이어에 따라 CLCD 설정
        if (player == 1) {
            write(clcd_dev, player1_text, strlen(player1_text)); // 플레이어 1의 차례
        }
        else {
            write(clcd_dev, player2_text, strlen(player2_text)); // 플레이어 2의 차례
        }

        display_updated_dot_matrix(1); // 도트 매트릭스 현재 패턴 표시

        // 타이머 시작
        gettimeofday(&start, NULL);

        while (1) {
            // 현재 시간 체크
            gettimeofday(&end, NULL);
            elapsed_time = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;

            int startTime = (unsigned)time(NULL) + 1;				// time 값과 +1 반복해서 선언 


            if (ts < 6) {
                // 7-Segment 장치 불러오기와 타이머 출력 부분 
                fnds = open(FND, O_RDWR);
                if (fnds < 0) { printf("Can't open FND.\n"); exit(0); }
                fnd_num[0] = Time_Table[0];
                fnd_num[1] = Time_Table[0];
                fnd_num[2] = Time_Table[5 - ts];
                fnd_num[3] = Time_Table[((endTime - startTime) % 10)];	// 초 단위 타이머 
                usleep(200000);
                write(fnds, &fnd_num, sizeof(fnd_num));
                close(fnds);

                if ((endTime - startTime) % 10 == 0) {
                    ts += 1;
                    usleep(750000);
                }
            }
            else {
                tm++;
                ts = 0;
            }

            // 타이머가 0이 되었을 때 패배 처리
            if ((endTime - startTime) <= 10) {
                PRINT("   BOOM!  ");
                usleep(1000000);
                display_winner(clcd_dev, player);
            }


            if (elapsed_time >= 5000) { // 5초가 경과하면
                player = (player == 1) ? 2 : 1; // 플레이어 차례 변경
                break;
            }

            // Tact switch 상태 읽기
            read(tact_dev, &tact_data, sizeof(tact_data));
            if (tact_data[0] & 0x01) { // Tact switch 1이 눌렸을 때
                int previous_value = update_dot_matrix(); // 도트 매트릭스 업데이트
                if (previous_value == 0xF0) {
                    if (player == 1) {
                        PRINT("   BOOM!  ");
                        usleep(1000000);
                        write(clcd_dev, player2_win_text, strlen(player2_win_text)); // Player 2 승리
                    }
                    else {
                        PRINT("   BOOM!  ");
                        usleep(1000000);
                        write(clcd_dev, player1_win_text, strlen(player1_win_text)); // Player 1 승리
                    }
                    sleep(3); // 승리 메시지를 3초간 표시
                    exit(0); // 게임 종료
                }
                player = (player == 1) ? 2 : 1; // 플레이어 차례 변경
                break;
            }

            usleep(100000); // 0.1초 대기 (폴링 주기)
        }
    }

    close(clcd_dev);
    close(tact_dev);

    return 0;
}
