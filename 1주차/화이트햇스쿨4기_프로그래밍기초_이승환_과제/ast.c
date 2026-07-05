/* =====================================================================
 *  ast.c  -  AST(JSON) 분석기  [화이트햇스쿨 4기 / 프로그래밍 기초]
 *
 *  멘토가 제공한 ast.json (pycparser가 만든 C 소스코드의 AST)을 읽어서
 *  아래 정보를 추출한다.
 *    1) 함수 개수
 *    2) 각 함수의 리턴 타입
 *    3) 각 함수의 이름
 *    4) 각 함수의 파라미터 (타입 + 변수명)
 *    5) 각 함수 안의 if 조건 개수
 *
 *  JSON 파싱은 멘토 제공 라이브러리 json_c.c 를 그대로 사용한다.
 *  컴파일:  gcc ast.c -o ast.exe
 *  실행  :  ./ast.exe        (같은 폴더에 ast.json 이 있어야 함)
 * ===================================================================== */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "json_c.c"     /* 멘토 제공 JSON 파서 (함수 본문까지 포함됨) */

/* 값이 없을 때 돌려줄 "undefined" 표식 */
static const json_value JNONE = { JSON_UNDEFINED, NULL };

/* ---------------------------------------------------------------------
 *  파일 전체를 읽어 하나의 문자열(char*)로 돌려준다.
 *  (json_c.c 의 json_read()는 미구현이라 직접 만든다)
 * ------------------------------------------------------------------- */
char *read_file(const char *path)
{
    FILE *fp = fopen(path, "rb");          /* 바이너리로 열어 크기 계산 */
    if (fp == NULL) {
        fprintf(stderr, "파일 열기 실패: %s\n", path);
        return NULL;
    }
    fseek(fp, 0, SEEK_END);                /* 파일 끝으로 이동 */
    long size = ftell(fp);                 /* 현재 위치 = 파일 크기 */
    fseek(fp, 0, SEEK_SET);                /* 다시 처음으로 */

    char *buf = (char *)malloc(size + 1);  /* +1 은 끝의 '\0' 자리 */
    if (buf == NULL) { fclose(fp); return NULL; }

    fread(buf, 1, size, fp);               /* 통째로 읽기 */
    buf[size] = '\0';                      /* 문자열 끝 표시 */
    fclose(fp);
    return buf;
}

/* ---------------------------------------------------------------------
 *  보조 함수들 : json_c.c 의 json_get()은 키가 없으면 stderr로 에러를
 *  출력해서 화면이 지저분해진다. 그래서 구조체에 직접 접근하는
 *  조용한 버전을 따로 만든다.
 * ------------------------------------------------------------------- */

/* 객체(object)에서 key 에 해당하는 값을 찾는다. 없으면 JNONE */
json_value obj_get(json_value v, const char *key)
{
    if (v.type != JSON_OBJECT) return JNONE;
    json_object *o = (json_object *)v.value;
    for (int i = 0; i <= o->last_index; i++) {
        if (o->keys[i] && strcmp(o->keys[i], key) == 0)
            return o->values[i];
    }
    return JNONE;
}

/* 배열(array)에서 index 번째 값을 찾는다. 범위를 벗어나면 JNONE */
json_value arr_get(json_value v, int index)
{
    if (v.type != JSON_ARRAY) return JNONE;
    json_array *a = (json_array *)v.value;
    if (index < 0 || index > a->last_index) return JNONE;
    return a->values[index];
}

/* json_value 가 문자열이면 그 내용을, 아니면 NULL 을 돌려준다 */
const char *str_of(json_value v)
{
    return (v.type == JSON_STRING) ? (const char *)v.value : NULL;
}

/* ---------------------------------------------------------------------
 *  타입 노드를 사람이 읽을 수 있는 문자열로 변환한다.
 *  AST에서 타입은 PtrDecl→TypeDecl→IdentifierType 처럼 껍질이
 *  겹겹이 싸여 있어서 안쪽(IdentifierType)까지 따라 들어가야 한다.
 *  예) char* 는  PtrDecl -> TypeDecl -> IdentifierType("char")
 * ------------------------------------------------------------------- */
void type_to_str(json_value t, char *out)
{
    out[0] = '\0';
    char stars[32] = "";          /* 포인터(*) / 배열([]) 표시 모음 */

    for (int guard = 0; guard < 64; guard++) {      /* 무한루프 방지 */
        const char *nt = str_of(obj_get(t, "_nodetype"));
        if (nt == NULL) { strcpy(out, "?"); return; }

        if (strcmp(nt, "PtrDecl") == 0) {            /* 포인터 */
            strcat(stars, "*");
            t = obj_get(t, "type");
        } else if (strcmp(nt, "TypeDecl") == 0) {    /* 이름 껍질 */
            t = obj_get(t, "type");
        } else if (strcmp(nt, "ArrayDecl") == 0) {   /* 배열 */
            strcat(stars, "[]");
            t = obj_get(t, "type");
        } else if (strcmp(nt, "FuncDecl") == 0) {    /* 함수 포인터 등 */
            t = obj_get(t, "type");
        } else if (strcmp(nt, "IdentifierType") == 0) {  /* 실제 기본 타입 */
            json_value names = obj_get(t, "names");
            int n = json_len(names);
            for (int i = 0; i < n; i++) {
                const char *nm = str_of(arr_get(names, i));
                if (nm) { if (out[0]) strcat(out, " "); strcat(out, nm); }
            }
            if (stars[0]) { strcat(out, " "); strcat(out, stars); }
            return;
        } else if (strcmp(nt, "Struct") == 0) {      /* 구조체 */
            strcpy(out, "struct ");
            const char *nm = str_of(obj_get(t, "name"));
            if (nm) strcat(out, nm);
            if (stars[0]) { strcat(out, " "); strcat(out, stars); }
            return;
        } else {                                     /* 그 외 */
            strcpy(out, nt);
            return;
        }
    }
    strcpy(out, "?");
}

/* ---------------------------------------------------------------------
 *  어떤 노드든 그 아래를 모두 훑으면서 "_nodetype":"If" 인 노드의
 *  개수를 센다. (재귀)
 * ------------------------------------------------------------------- */
int count_if(json_value v)
{
    int cnt = 0;
    if (v.type == JSON_OBJECT) {
        json_object *o = (json_object *)v.value;
        for (int i = 0; i <= o->last_index; i++) {
            /* 이 객체 자신이 If 노드인지 검사 */
            if (o->keys[i] && strcmp(o->keys[i], "_nodetype") == 0) {
                const char *s = str_of(o->values[i]);
                if (s && strcmp(s, "If") == 0) cnt++;
            }
            cnt += count_if(o->values[i]);   /* 자식으로 내려가기 */
        }
    } else if (v.type == JSON_ARRAY) {
        json_array *a = (json_array *)v.value;
        for (int i = 0; i <= a->last_index; i++)
            cnt += count_if(a->values[i]);
    }
    return cnt;
}

/* ---------------------------------------------------------------------
 *  한 함수(FuncDef)의 파라미터 목록을 보기 좋게 출력한다.
 * ------------------------------------------------------------------- */
void print_params(json_value decl)
{
    json_value fd   = obj_get(decl, "type");   /* FuncDecl */
    json_value args = obj_get(fd, "args");     /* ParamList 또는 없음 */

    if (args.type != JSON_OBJECT) {            /* 파라미터가 없는 경우 */
        printf("void");
        return;
    }
    json_value params = obj_get(args, "params");
    int n = json_len(params);
    if (n <= 0) { printf("void"); return; }

    for (int j = 0; j < n; j++) {
        json_value p = arr_get(params, j);
        const char *pnt = str_of(obj_get(p, "_nodetype"));

        if (pnt && strcmp(pnt, "EllipsisParam") == 0) {
            printf("...");                     /* 가변인자 ... */
        } else {
            char ts[256];
            type_to_str(obj_get(p, "type"), ts);
            const char *pname = str_of(obj_get(p, "name"));
            printf("%s %s", ts, pname ? pname : "(이름없음)");
        }
        if (j < n - 1) printf(", ");
    }
}

/* ===================================================================== */
int main(void)
{
    /* 1) ast.json 읽기 */
    char *text = read_file("ast.json");
    if (text == NULL) return 1;

    /* 2) JSON 파싱 → 최상위는 FileAST(object) */
    json_value root = json_create(text);
    json_value ext  = obj_get(root, "ext");   /* 최상위 선언/정의 배열 */
    int total = json_len(ext);

    /* 3) 먼저 함수(FuncDef) 개수만 센다 */
    int func_count = 0;
    for (int i = 0; i < total; i++) {
        const char *nt = str_of(obj_get(arr_get(ext, i), "_nodetype"));
        if (nt && strcmp(nt, "FuncDef") == 0) func_count++;
    }

    printf("=====================================================\n");
    printf(" AST 분석 결과 (ast.json)\n");
    printf("=====================================================\n");
    printf(" 최상위 항목 수 : %d개\n", total);
    printf(" 함수 정의 수   : %d개  <-- (1) 함수 개수\n", func_count);
    printf("=====================================================\n\n");

    /* 4) 함수별 상세 정보 출력 */
    printf("%-4s %-20s %-12s %-5s %s\n",
           "번호", "함수이름", "리턴타입", "if수", "파라미터(타입 변수명)");
    printf("-----------------------------------------------------"
           "-----------------------------\n");

    int idx = 0;
    for (int i = 0; i < total; i++) {
        json_value e  = arr_get(ext, i);
        const char *nt = str_of(obj_get(e, "_nodetype"));
        if (!(nt && strcmp(nt, "FuncDef") == 0)) continue;   /* 함수만 */

        json_value decl = obj_get(e, "decl");
        const char *name = str_of(obj_get(decl, "name"));        /* (3) 이름 */

        char ret[256];
        type_to_str(obj_get(obj_get(decl, "type"), "type"), ret);/* (2) 리턴타입 */

        int nif = count_if(obj_get(e, "body"));                  /* (5) if 개수 */

        printf("%-4d %-20s %-12s %-5d ", ++idx, name, ret, nif);
        print_params(decl);                                      /* (4) 파라미터 */
        printf("\n");
    }
    printf("\n분석 완료: 총 %d개 함수.\n", func_count);

    free(text);
    return 0;
}
