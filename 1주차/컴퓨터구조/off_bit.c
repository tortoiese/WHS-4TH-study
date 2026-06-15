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

int main(){
    unsigned char value = 0b00000000; // 초기값: 모든 비트가 0인 상태

    // Bit 3이 설정되어 있는지 확인한다. (0b00001000)
    if(is_bit_set(value, 3)) {
        printf("Bit 3 is set.\n");
    } else {
        printf("Bit 3 is not set.\n");
    }

    // Bit 2을 1로 설정한다. (0b00000100)
    value = set_bit(value, 2); // Bit 2을 1로 설정
    printf("Value after setting bit 2: %u\n", value);

    return 0;

}