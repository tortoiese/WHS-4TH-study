#include <stdio.h>                            // 표준 입출력 함수(printf, fopen, fgetc 등) 사용을 위한 헤더

int main(void)                                 // 프로그램의 시작점(메인 함수)
{
  FILE *fp;                                    // 파일을 다루기 위한 파일 포인터(파일 손잡이) 선언
  int c;                                       // fgetc로 읽은 한 글자를 담을 변수 (EOF 비교 위해 int형)

  fp = fopen("test.txt", "r");                 // test.txt 파일을 "r"(읽기) 모드로 열기

  if(fp == NULL){                              // 파일 열기 실패 여부 확인 (없거나 권한 없으면 NULL 반환)
    printf("파일열기 실패\n");                 // 실패 시 메시지 출력
  } else {                                     // 성공한 경우
    printf("파일열기 성공\n");                 // 성공 메시지 출력
  }

  // 한문자씩 읽는 fgetc
  int count = 0;                               // 읽은 글자 수를 셀 카운터 초기화
  while((c = fgetc(fp)) != EOF){               // 파일에서 한 글자씩 읽어 c에 저장, 파일 끝(EOF)이면 종료
    putchar(c);                                // 읽은 글자를 화면에 그대로 출력
    count++;                                   // 글자 하나 읽을 때마다 카운트 증가
  }
  printf("%d \n", count);                      // 총 읽은 글자 수 출력

  while(1)                                     // 무한 반복 (내부의 break로 빠져나감)
  {
    char temp[2];                              // 읽은 문자열을 담을 버퍼 (1글자 + 끝표시 '\0')
    char *tStr = fgets(temp, 2, fp);           // 파일에서 최대 1글자를 읽어 temp에 저장 (반환값으로 끝 판단)
    if(tStr == NULL)                           // 더 읽을 내용이 없으면(파일 끝) NULL 반환
    {
      break;                                   // 반복문 탈출
    }
    printf("%s", temp);                        // 읽은 문자열 출력
  }

  fclose(fp);                                  // 사용한 파일을 닫아 자원 반환
  return 0;                                    // 프로그램 정상 종료
}
