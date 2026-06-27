#include <stdio.h>

//특정위치의 비트를 끄는 함수
//끄고자 하는 위치의 비트를 1로바꿈
//1<<(n) : n번째 비트 위치에 1을 만들어냄
//~연산으로 모든 비트를 반전시킴
//그 상태에서 & 연산을 하면 특정 위치의 비트만 0이 되고 나머지는 그대로 유지됨



//특정 위치의 비트 값이 1인지 확인하는 함수
int is_bit_set(unsigned char value, int bit_position) {
    return (value & (1 << bit_position)) != 0;
}

//특정 위치의 비트 값을 1로 설정하는 함수
unsigned char set_bit(unsigned char value, int bit_position) {
    return value | (1 << bit_position);
}

//특정 위치의 비트를 끄는(0으로 만드는) 함수
unsigned char clear_bit(unsigned char value, int bit_position) {
    return value & ~(1 << bit_position);
}

//unsigned char 값을 8자리 2진수 문자열로 변환하는 함수
//MSB(7번 비트)부터 LSB(0번 비트)까지 순서대로 채운다.
void to_binary(unsigned char value, char *buffer) {
    for (int i = 7; i >= 0; i--) {
        buffer[7 - i] = (value & (1 << i)) ? '1' : '0';
    }
    buffer[8] = '\0'; // 문자열 끝을 표시
}

int main(){
    unsigned char value = 0b11111111; // 초기값: 모든 비트가 1인 상태
    int bit_position;                  // 사용자가 끌 비트 위치
    char bin[9];                       // 2진수 표현을 담을 버퍼 (8비트 + 널문자)

    to_binary(value, bin);
    printf("Initial value: %u (binary: %s)\n", value, bin);

    // 끄고자 하는 비트 위치를 사용자로부터 입력받는다. (0 ~ 7)
    printf("Enter the bit position to clear (0-7): ");
    if (scanf("%d", &bit_position) != 1) {
        printf("Invalid input.\n");
        return 1;
    }

    // 입력값이 유효한 범위(0~7)인지 검사한다.
    if (bit_position < 0 || bit_position > 7) {
        printf("Bit position must be between 0 and 7.\n");
        return 1;
    }

    unsigned char original = value; // 원래값을 보관해 둔다.

    // 입력받은 위치의 비트를 끈다.
    value = clear_bit(value, bit_position);

    // 원래값과 끈 값을 10진수와 2진수로 함께 출력한다.
    to_binary(original, bin);
    printf("Original value : %3u (binary: %s)\n", original, bin);

    to_binary(value, bin);
    printf("Cleared value  : %3u (binary: %s)\n", value, bin);

    return 0;

}