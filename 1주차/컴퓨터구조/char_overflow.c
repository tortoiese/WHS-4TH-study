#include <stdio.h>
#include <limits.h>  // CHAR_MAX 상수를 사용하기 위해 포함한다. CHAR_MAX는 char형이 가질 수 있는 최대값을 나타낸다.

int main() {
    char value = CHAR_MAX;  // char형이 가질 수 있는 최대값으로 초기화한다. 보통 127(0x7F)이다.

    printf("Original value: %d\n", value);  // 현재 value의 값을 출력한다. %d는 char형을 정수로 출력할 때 사용한다.

    value = value + 1;  // value에 1을 더한다. 이때 char형의 범위를 초과하여 오버플로우가 발생한다.

    printf("Value after adding 1: %d\n", value);  // 오버플로우가 발생한 후의 value 값을 출력한다. 예상과 달리 -128(0x80)이 출력될 것이다.
  
    //터미널열고, gcc -o char_overflow char_overflow.c -g 실행
    //gdb char_overflow 실행
    //b main 입력
    //r 입력
    //n 입력
    //n 입력
    //p/t value 입력  0111 1111 = 127(10진수) = 0x7F(16진수)
    //n 입력
    //p value 입력 1000 0000 = -128(10진수) = 0x80(16진수)
    return 0;
}