#include <stdio.h>      // 표준 입출력 함수(printf, fopen 등)를 쓰기 위한 헤더

int main(void){
  FILE *fp;             // ① 파일을 다루기 위한 "파일 포인터" 선언
  int c;                // (선언만 하고 실제로는 안 쓰임)

  // w, a, r            // 주석: 파일 열기 모드 종류를 메모해 둔 것
  fp = fopen("test.txt", "a");   // ② test.txt 파일을 "a"(추가) 모드로 열기

  if(fp == NULL){       // ③ 파일 열기에 실패했는지 검사
    printf("파일열기 실패\n");
  } else {
    printf("파일열기 성공\n");
  }

  fclose(fp);           // ④ 열었던 파일을 닫기
  return 0;
}

