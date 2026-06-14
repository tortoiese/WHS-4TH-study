// 표준 입출력(standard input/output) 라이브러리를 포함한다.
// printf 같은 입출력 함수를 쓰기 위해 필요하다.
#include <stdio.h>

// main: 프로그램이 시작될 때 가장 먼저 실행되는 함수(진입점, entry point).
int main() {
    // printf: 화면(콘솔)에 문자열을 출력하는 함수.
    // "Hello, World!" 라는 글자를 출력한다.
    // \n 은 줄바꿈(엔터)을 의미한다 → 출력 후 커서가 다음 줄로 내려간다.
    // 따라서 실행하면 콘솔에 다음과 같이 찍힌다:  Hello, World!
    printf("Hello, World!\n");

    // return 0: main 함수를 정상적으로 종료한다는 의미.
    // 0 은 "프로그램이 오류 없이 잘 끝났다"는 신호를 운영체제에 전달한다.
    return 0;
}
