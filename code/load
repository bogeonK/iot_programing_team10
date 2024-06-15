#include <stdio.h>

void printMatrix(int matrix[8][8]) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            printf("%d", matrix[i][j]);
        }
        printf("\n");
    }
}

int main() {
    // 8x8 DOT matrix 초기화
    int dotMatrix[8][8] = {0};

    // 5발의 가짜탄을 설정
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 8; j++) {
            dotMatrix[j][i] = 1;
        }
    }

    // 1발의 실탄을 설정 (6번째 열에 실탄을 놓음)
    for (int j = 4; j < 8; j++) {
        dotMatrix[j][5] = 1;
    }

    // DOT matrix 출력
    printMatrix(dotMatrix);

    return 0;
}
