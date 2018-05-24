#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <atlImage.h>
#include <math.h>

//버튼값

#define ID_BUTTON1 500
#define ID_BUTTON2 550
#define ID_BUTTON3 560
#define ID_BUTTON4 570
#define ID_LISTBOX 100 


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); //나는 큰 잘못을 저질렀다 난 대학교 승준이랑 같이 들어야겠다.
LRESULT CALLBACK ChildProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE g_hinst;
LPCTSTR lpszClass = TEXT("First");
BOOL CALLBACK Dlg_1Proc(HWND, UINT, WPARAM, LPARAM);
WNDCLASSEX wc;

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) //h의 의미? = 핸들
{
	HWND hWnd;
	MSG Message;
	g_hinst = hinstance;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = g_hinst;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL; //메뉴 이름
	wc.lpszClassName = "ParentClass";
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&wc);

	//차일드 윈도우 클래스를 등록한다.
	wc.hCursor = LoadCursor(NULL, IDC_HELP);
	wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wc.lpszClassName = "ChildClass";
	wc.lpfnWndProc = ChildProc;   // 차일드 윈도우 프로시저 지정 
	RegisterClassEx(&wc); // 윈도우 클래스를 운영체제에 등록

	hWnd = CreateWindow("ParentClass", "Windows Program 7-1", WS_OVERLAPPEDWINDOW, 10, 10, 1200, 800, NULL, (HMENU)NULL, hinstance, NULL);
	//CreateWindow(클래스 이름,타이틀 이름,윈도우 스타일, 윈도우 위치좌표x , y,윈도우 가로크기, 윈도우 세로크기, 부모 윈도우 핸들, 메뉴 핸들, 응용프로그램 인스턴스, 생성 윈도우 정보(NULL))
	ShowWindow(hWnd, nCmdShow); //윈도우의 화면 출력
	UpdateWindow(hWnd); //OS에 WM_PAINT메시지 전송
	while (GetMessage(&Message, NULL, 0, 0))
	{
		//윈도우 프로시저에서 PostQuitMessage()를 호출할때 종료됨.
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}//메시지루프를 돌림.
	return (int)Message.wParam;
}
//전역 모음----
static int pos_x, pos_y;
static int space = 0;
static int page = 0;

int TempPos;


struct Image {
	int page = -1;
	int bit = -1;
	int sel = 0;
	BOOL RIGHT = FALSE;

};

static Image sel;
static Image save[6];
LRESULT CALLBACK ChildProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HDC child_dc , child_memdc;
	PAINTSTRUCT child_ps;
	static RECT rect;
	static HBITMAP hBitmap[6];
//	static HBITMAP hCanvus[7];
	static char text[10];
	//	char str[80];

	switch ((uMsg))//간지 //뭐한거지 어제의 나;;
	{
	case WM_CREATE:


		GetClientRect(hWnd, &rect);
		for (int i = 0; i < 6; i++) {
			hBitmap[i] = LoadBitmap(g_hinst, MAKEINTRESOURCE(108 + i));

		}
		break;


	case WM_PAINT:
		child_dc = BeginPaint(hWnd, &child_ps);
		child_memdc = CreateCompatibleDC(child_dc);

		if (save[page].bit != -1) {
			if (save[page].RIGHT) {
				(HBITMAP)SelectObject(child_memdc, (hBitmap[save[page].bit]));
				StretchBlt(child_dc, 0, 0, rect.right, rect.bottom, child_memdc, 0, 0, 256, 230, SRCCOPY);

			}

		}
		wsprintf(text, "%d", save[page].bit);
		TextOut(child_dc, 10, 10, text, strlen(text));

		for (int i = 0; i < 6; ++i)
		{
			SelectObject(child_memdc,  hBitmap[i]);
		}
			DeleteDC(child_memdc);
			
	//	}
	
		//그리기
	
		EndPaint(hWnd, &child_ps);
		break;

	case WM_DESTROY:
		//	KillTimer(hWnd, 1);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);

}
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) //CALLBACK(윈도우 핸들, 처리해야 할 메시지의 값,입력장치의 값1,2)
{
	PAINTSTRUCT ps;
	HDC hdc;
	HWND child_hWnd;
//	RECT rectView;
	static HWND hButton1;
	static HWND hButton2;
	static HWND hButton3;
	static HWND hButton4;
	static HWND hList , hButton;
	static char str[100];
	static int child_show = 0;
	static char list_str[][20] = {NULL};
	static int temp;
	switch (iMessage) //메시지의 번호
	{
	case WM_CREATE:

		page = 0;
		hButton1 = CreateWindow("button", "<<", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 10, 400, 100, 25, hWnd, (HMENU)ID_BUTTON1, g_hinst, NULL);
		hButton2 = CreateWindow("button", ">>", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 700, 400, 100, 25, hWnd, (HMENU)ID_BUTTON2, g_hinst, NULL);
		hButton3 = CreateWindow("button", "적용", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 700, 500, 100, 25, hWnd, (HMENU)ID_BUTTON3, g_hinst, NULL);
		hButton4 = CreateWindow("button", "완성", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 700, 600, 100, 25, hWnd, (HMENU)ID_BUTTON4, g_hinst, NULL);

		for (int i = 0; i < 6; i++) {
			wsprintf(list_str[i], "슬라이드 이미지 %d", i);
		}
		hList = CreateWindow("listbox", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_STANDARD, 800, 10, 300, 300, hWnd, (HMENU)ID_LISTBOX, g_hinst, NULL);
		for (int i = 0; i < 6; i++) {
			SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)list_str[i]);
		}
		for (int i = 0; i < 6; i++) {
			save[i].bit = -1;
			save[i].RIGHT = FALSE;
		}
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_BUTTON1:
			if (page > 0) {
				page--;
			}
			break;
		case ID_BUTTON4:
			hButton = GetDlgItem(hWnd, ID_BUTTON3);
			EnableWindow(hButton, FALSE);
			break;
		case ID_BUTTON2:
			if (page < 5) {
				page++;
			}
			break;
		case ID_BUTTON3:
			save[page].RIGHT = TRUE;
			save[page].bit = temp;
			break;
		case ID_LISTBOX:
			if (HIWORD(wParam) == LBN_SELCHANGE)
				temp = SendMessage(hList, LB_GETCURSEL, 0, 0);
			break;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		wsprintf(str, "슬라이드 번호 %d", page);
		TextOut(hdc, 500, 10, str, strlen(str));
		if (child_show == 0) {
			child_hWnd = CreateWindow("ChildClass", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 100, 100, 600, 600, hWnd, NULL, g_hinst, NULL);
		}
		child_show++;
		EndPaint(hWnd, &ps);
		break;
	
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
	//case에서 정의되지 않은 메시지는 커널이 처리하도록 메시지 전달
}



