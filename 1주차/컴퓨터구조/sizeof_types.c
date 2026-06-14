#include <stdio.h>  // 표준 입출력 라이브러리. printf 함수를 쓰기 위해 포함한다.

// 이 프로그램은 C의 기본 자료형(data type)들이 메모리에서 각각 "몇 바이트"를 차지하는지 출력한다.
//   - sizeof(자료형): 그 자료형의 크기를 바이트 단위로 알려주는 연산자
//   - %zu: sizeof가 돌려주는 값(size_t 타입)을 출력할 때 쓰는 형식 지정자,  \n: 줄바꿈
int main() {
    printf("Size of char: %zu bytes\n", sizeof(char));               // char: 문자 1개를 담는 가장 작은 단위. 항상 1바이트.
    printf("Size of short: %zu bytes\n", sizeof(short));             // short: 비교적 작은 정수. 보통 2바이트.
    printf("Size of int: %zu bytes\n", sizeof(int));                 // int: 가장 많이 쓰는 기본 정수형. 보통 4바이트.
    printf("Size of long: %zu bytes\n", sizeof(long));              // long: Windows는 4바이트, 리눅스/맥(64비트)은 8바이트 → 환경마다 다름.
    printf("Size of long long: %zu bytes\n", sizeof(long long));     // long long: 더 큰 정수를 담는 형. 보통 8바이트.
    printf("Size of float: %zu bytes\n", sizeof(float));            // float: 소수(실수)를 담는 형. 보통 4바이트(단정밀도).
    printf("Size of double: %zu bytes\n", sizeof(double));           // double: 더 정밀한 소수를 담는 형. 보통 8바이트(배정밀도).
    printf("Size of long double: %zu bytes\n", sizeof(long double)); // long double: 환경마다 8/12/16바이트 등으로 달라짐.
    printf("Size of void*: %zu bytes\n", sizeof(void*));             // void*: 주소값(포인터). 32비트→4바이트, 64비트→8바이트.

    return 0;  // 0을 반환하며 정상 종료.
}
