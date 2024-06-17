#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>

#define fnd "/dev/fnd"
#define clcd "/dev/clcd"

// FND와 CLCD 장치 파일 디스크립터
int fnds;
int clcd_dev;

// 현재 플레이어를 추적하는 변수 (1 또는 2)
int current_player = 1;

// 숫자를 7세그먼트 FND에 표시하기 위한 테이블
unsigned char Time_Table[] = {~0x3f, ~0x06, ~0x5b, ~0x4f, ~0x66, ~0x6d, ~0x7d, ~0x07, ~0x7f, ~0x67, ~0x00};

// FND 디스플레이 업데이트 함수
void update_display(int m, int s);

// CLCD에 텍스트를 표시하는 함수
void display_clcd(const char* text);

int main() {
    fnds = open(fnd, O_RDWR);
    if (fnds < 0) {
        printf("Can't open FND.\n");
        exit(0);
    }

    clcd_dev = open(clcd, O_RDWR);
    if (clcd_dev < 0) {
        printf("Can't open CLCD.\n");
        close(fnds);
        exit(0);
    }

    int m = 3;  // 시작 분 설정 (3분)
    int s = 0;  // 시작 초 설정 (0초)

    while (m >= 0 && s >= 0) {
        update_display(m, s);
        sleep(1);

        if (s == 0) {
            if (m == 0) {
                break;
            }
            m--;
            s = 59;
        } else {
            s--;
        }
    }

    // 시간 종료 후 00:00 표시
    update_display(0, 0);

    // 현재 플레이어를 패배자로 표시
    char message[20];
    snprintf(message, sizeof(message), "Player %d loses!", current_player);
    display_clcd(message);

    close(clcd_dev);
    close(fnds);
    return 0;
}

void update_display(int m, int s) {
    unsigned char fnd_num[4];
    fnd_num[0] = Time_Table[m / 10]; // 분의 10의 자리
    fnd_num[1] = Time_Table[m % 10]; // 분의 1의 자리
    fnd_num[2] = Time_Table[s / 10]; // 초의 10의 자리
    fnd_num[3] = Time_Table[s % 10]; // 초의 1의 자리

    write(fnds, &fnd_num, sizeof(fnd_num));
}

void display_clcd(const char* text) {
    write(clcd_dev, text, strlen(text));
}
