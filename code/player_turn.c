#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <string.h>

#define CLCD "/dev/clcd"
#define TACT "/dev/tactsw"

int main() {
    int clcd_dev, tact_dev;
    char* player1_text = "Player 1";
    char* player2_text = "Player 2";
    unsigned char tact_data[2];
    int player = 1; // 처음 시작은 플레이어 1의 차례
    struct timeval start, end;
    long elapsed_time;

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

    while (1) {
        // 현재 플레이어에 따라 CLCD 설정
        if (player == 1) {
            write(clcd_dev, player1_text, strlen(player1_text)); // 플레이어 1의 차례
        }
        else {
            write(clcd_dev, player2_text, strlen(player2_text)); // 플레이어 2의 차례
        }

        // 타이머 시작
        gettimeofday(&start, NULL);

        while (1) {
            // 현재 시간 체크
            gettimeofday(&end, NULL);
            elapsed_time = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;

            if (elapsed_time >= 5000) { // 5초가 경과하면
                player = (player == 1) ? 2 : 1; // 플레이어 차례 변경
                break;
            }

            // Tact switch 상태 읽기
            read(tact_dev, &tact_data, sizeof(tact_data));
            if (tact_data[0] & 0x01) { // Tact switch 1이 눌렸을 때
                player = (player == 1) ? 2 : 1; // 플레이어 차례 변경
                break;
            }

            usleep(100000); // 0.1초 대기 (폴링 주기)
        }

        // 루프 처음으로 돌아가서 다시 실행
        continue;
    }

    close(clcd_dev);
    close(tact_dev);

    return 0;
}
