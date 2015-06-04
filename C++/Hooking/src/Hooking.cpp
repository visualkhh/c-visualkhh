//============================================================================
// Name        : Hooking.cpp
// Author      : HHK
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include "stdio.h"
#include "conio.h"
#include "windows.h"

#define DEF_DLL_NAME  "KeyHook.dll"
#define DEF_HOOKSTART  "HookStart"
#define DEF_HOOKSTOP  "HookStop"

typedef void (*PFN_HOOKSTART)();
typedef void (*PFN_HOOKSTOP)();

int main() {
	 printf("1\n");
    HMODULE   hDll = NULL;
    PFN_HOOKSTART HookStart = NULL;
    PFN_HOOKSTOP HookStop = NULL;
    char   ch = 0;

	 printf("2\n");
    // KeyHook.dll 로딩
    hDll = LoadLibrary(DEF_DLL_NAME);

    // export 함수 주소 얻기
	 printf("3\n");
    HookStart = (PFN_HOOKSTART)GetProcAddress(hDll, DEF_HOOKSTART);
    HookStop = (PFN_HOOKSTOP)GetProcAddress(hDll, DEF_HOOKSTOP);

	 printf("4\n");
    // 후킹 시작
    HookStart();
	 printf("5\n");

    // 사용자가 'q' 를 입력할 때까지 대기
    printf("press 'q' to quit!\n");
    while( getch() != 'q' ) ;

    // 후킹 종료
	 printf("6\n");
    HookStop();
	 printf("7\n");

    // KeyHook.dll 언로딩
    FreeLibrary(hDll);
	 printf("8\n");
}
