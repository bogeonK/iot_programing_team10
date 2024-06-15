#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <string.h>
#include <asm/ioctls.h>
#include <stdbool.h>
#include <time.h>
#include <sys/ioctl.h>
#include <sys/stat.h>

#define dot "/dev/dot"

unsigned char pattern[8] = {  // 새로운 패턴 배열
    0xFF,  // 11111111
    0xFF,  // 11111111
    0xFF,  // 11111111
    0xFF,  // 11111111
    0xFF,  // 11111111
    0xF0,  // 11110000
    0x00,  // 00000000
    0x00   // 00000000
};

// 패턴 배열과 "sleep" 값을 인자로 받아서 도트 매트릭스를 제어하는 함수
void DOT_control(int time_sleep){
    int dot_d;

    dot_d = open(dot, O_RDWR);
    if (dot_d < 0) {
        printf("dot Error\n");
        return; // 예외처리
    }

    write(dot_d, pattern, sizeof(pattern)); // 패턴 출력
    sleep(time_sleep); // 몇 초 동안 점등할지

    close(dot_d);
}

int main() {
    while (1) {
        DOT_control(3); // 새로운 패턴을 3초 동안 표시
    }

    return 0;
}