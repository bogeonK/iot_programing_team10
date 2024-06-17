# iot_programing_team10
러시안룰렛&폭탄돌리기 (7-segment LED)
## 팀원
김남교 | 김보건 | 유진웅 | 한강빈
## 프로젝트 계획
- **폭탄 해체 게임**  dot matrix, 표현, 폭탄타이머 참고
- **인디언 포커 게임**  1:1 턴제를 참고

![프로젝트 사전 조사](https://github.com/kimnamgyo/iot_programing_team10/blob/main/img/%ED%94%84%EB%A1%9C%EC%A0%9D%ED%8A%B8%20%EC%82%AC%EC%A0%84%20%EC%A1%B0%EC%82%AC.png)
![프로젝트 계획](https://github.com/kimnamgyo/iot_programing_team10/blob/main/img/%ED%94%84%EB%A1%9C%EC%A0%9D%ED%8A%B8%20%EA%B3%84%ED%9A%8D.png)
## 플로우차트
![플로우차트](https://github.com/kimnamgyo/iot_programing_team10/blob/main/img/flowchart.png?raw=true)

## **게임 소개**

 - ### **게임 시작시**
    ![gif_01](video_clip/GIF/start.gif)

 - ### **타이머**
    ![gif_02](/video_clip/GIF/timer.gif)
   * 타이머가 다 될 시 폭탄을 같고 있던 플레이어의 패배

  - ### **번갈아가며 총쏘기**
    ![gif_03](/video_clip/GIF/dot.gif)
    * 폭탄에 번갈아 가며 총을 쏘고 폭탄과 총을 넘겨줌
    * 폭탄에 실탄을 쏜 플레이어의 패배

 - ### **타이머가 다 되거나 실탄을 쏘면 게임 종료**
    ![gif_04](/video_clip/GIF/end.gif)
    * clcd에 승리한 플레이어 표시

## **게임 코드**

 - **`'russian_roulette.c'` - 메인코드 작성**  
    앞의 모든 코드를 종합하여 메인 코드 제작 - 
    ['russian_roulette.c'](https://github.com/kimnamgyo/iot_programing_team10/blob/main/code/russian_roulette.c) 참고

---

## **게임 실행 영상**
- **러시안 룰렛 게임 실행 Youtube Link** : [H-Smart4412 활용 러시안 룰렛 게임(Russian Rulette Game)](https://youtu.be/yONbquB6PvU?feature=shared)

---

