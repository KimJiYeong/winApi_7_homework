#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <atlImage.h>
#include <math.h>

//버튼값
#define ID_R1 100
#define ID_R2 200
#define ID_R3 300

#define ID_R_MIDDLE 400
#define ID_R_LARGE 410
#define ID_R_SMALL 420

#define ID_BUTTON 500
#define ID_SCRBLUE 600

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

	hWnd = CreateWindow("ParentClass", "Windows Program 7-1", WS_OVERLAPPEDWINDOW, 10, 10, 900, 800, NULL, (HMENU)NULL, hinstance, NULL);
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
static int f;
static int sin_x, sin_y;
static int spring_x, spring_y;
static double move_x, move_y;
static int pos_x, pos_y;
static int space = 0;
static int size = 20;
static BOOL RIGHT =	TRUE;
static int right=0;
static BOOL SIN, SPRING, ELLIPSE;
static int Speed = 50;
int TempPos;

LRESULT CALLBACK ChildProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
	HDC child_dc;
	PAINTSTRUCT child_ps;
	static RECT rect;
	
	
//	char str[80];

	switch ((uMsg))//간지
	{
	case WM_CREATE:
	
		GetClientRect(hWnd, &rect);
		KillTimer(hWnd, 1);
		SetTimer(hWnd, 1, 100, NULL);
		//move_x = rect.right / 4; //move_x 초기화
		pos_x = rect.right / 2; // pos_y = rect.bottom / 2;
		//move_x = 0; move_y = 0;
//		RIGHT = TRUE;
		break;

	case WM_TIMER:
		switch (wParam)
		{
		case 1:
			if (RIGHT) {
				move_x += Speed / 50;
				if (move_x > rect.bottom) {
					move_x = 0;
				}
			}
			else {
				move_x -= Speed / 50;
				if (move_x > rect.bottom) {
					move_x = 0;
				}
			}
			//스프링일때
			if (SIN) {
				//사인일때

				pos_x = move_x;
				//move_y = (int)(sin(move_x*3.14 / 50) * (90)) + rect.bottom / 2;
				pos_y = (int)(sin(move_x*3.14 / 60) * (90)) + rect.bottom / 2;

			}
			else if (SPRING) {
				//	pos_x = rect.right/ 4;
				//	pos_y = rect.bottom /2;

				pos_y = (int)(sin(move_x*3.14 / 50) * (90)) + rect.bottom / 2;
				pos_x = move_x + (int)(cos(move_x*3.14 / 50) * (90));

			}
			else if (ELLIPSE) {
				pos_y = (int)(sin(move_x*3.14 / 50) * (200) + rect.bottom / 2);
				pos_x = (int)(cos(move_x*3.14 / 50) * (200) + rect.right / 2);

			}
			break;
		}
	
		//move_y
		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_PAINT:
		child_dc = BeginPaint(hWnd, &child_ps);
		//그리기
		MoveToEx(child_dc,rect.right/2, rect.top , NULL);
		LineTo(child_dc, rect.right/2, rect.bottom);
		MoveToEx(child_dc, rect.left, rect.bottom/2, NULL);
		LineTo(child_dc, rect.right, rect.bottom/2);
		if (SIN) {
			//사인
			MoveToEx(child_dc, rect.right / 4, rect.bottom / 2, NULL);
			for (int i = rect.left; i < rect.right; i++) {
				sin_y = (int)(sin(i*3.14 / 60) * (90)) + rect.bottom / 2;
				LineTo(child_dc, (int)i, sin_y);
			}
		}
		else if (SPRING) {
			//스프링
			MoveToEx(child_dc, rect.right / 4, rect.bottom / 2, NULL);
			for (int i = rect.left; i < rect.right; i++) {
				spring_y = (int)(sin(i*3.14 / 50) * (90)) + rect.bottom / 2;
				spring_x = (int)(cos(i*3.14 / 50) * (90));
				LineTo(child_dc, (int)i + spring_x, spring_y);
			}
		}
		else if (ELLIPSE) {
			//원
			Arc(child_dc, rect.right / 2 - 200, rect.bottom / 2 - 200, rect.right / 2 + 200, rect.bottom / 2 + 200, rect.right, rect.bottom, rect.right, rect.bottom);
		}
		
	
		//움직이는 원
		//x랑 y값 설정
		Ellipse(child_dc, pos_x - size , pos_y - size , pos_x + size , pos_y + size );

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
	HWND CildHwnd[2];
	HWND child_hWnd;
	HWND r1, r2, r3;
	HWND r_L, r_M, r_S;
	static int wnd_x, wnd_y;
	RECT rectView;
	static HWND hButton;
	static HWND hSpeed;
	
	switch (iMessage) //메시지의 번호
	{
	case WM_CREATE:
		wnd_x = 600;
		wnd_y = 600;
		GetClientRect(hWnd, &rectView);
		r1 = CreateWindow("button", "Rectangle", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP, 700, 20, 100, 30, hWnd, (HMENU)ID_R1, g_hinst, NULL);
		r2 = CreateWindow("button", "Ellipse", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 700, 50, 100, 30, hWnd, (HMENU)ID_R2, g_hinst, NULL);
		r3 = CreateWindow("button", "Line", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 700, 80, 100, 30, hWnd, (HMENU)ID_R3, g_hinst, NULL);

		CheckRadioButton(hWnd, ID_R1, ID_R3, ID_R1);
		r_L = CreateWindow("button", "L", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP, 700, 20 + 100, 100, 30, hWnd, (HMENU)ID_R_LARGE, g_hinst, NULL);
		r_M = CreateWindow("button", "M", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 700, 50 + 100, 100, 30, hWnd, (HMENU)ID_R_MIDDLE, g_hinst, NULL);
		r_S = CreateWindow("button", "S", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 700, 80 + 100, 100, 30, hWnd, (HMENU)ID_R_SMALL, g_hinst, NULL);
		CheckRadioButton(hWnd, ID_R_LARGE, ID_R_MIDDLE, ID_R_SMALL);

		hButton = CreateWindow("button", "방향", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 700, 400, 100, 25, hWnd, (HMENU)ID_BUTTON, g_hinst, NULL);
		
		hSpeed = CreateWindow("scrollbar", NULL, WS_CHILD | WS_VISIBLE | SBS_HORZ, 700, 500, 200, 20, hWnd, (HMENU)ID_SCRBLUE, g_hinst, NULL);
		SetScrollRange(hSpeed, SB_CTL, 25, 100, TRUE); 
		
		break;
	case WM_HSCROLL:
		if ((HWND)lParam == hSpeed)
			TempPos=Speed;
		switch (LOWORD(wParam))
		{
		case SB_LINELEFT:
			TempPos = max(100, TempPos - 1);
			
					break;
		case SB_LINERIGHT:
			TempPos = min(25, TempPos + 1);
		
			break;
		case SB_PAGELEFT:
			TempPos = max(100, TempPos - 1);
			
				break;
		case SB_PAGERIGHT:
			TempPos = min(25, TempPos + 1);
			
			break;
		case SB_THUMBTRACK:
			TempPos = HIWORD(wParam);
			break;
		}
		
		if ((HWND)lParam == hSpeed) {
			Speed = TempPos;
		}
		SetScrollPos((HWND)lParam, SB_CTL, Speed, TRUE);
		
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_R1:
			SIN = TRUE;
			SPRING = FALSE;
			ELLIPSE = FALSE;

			break;
		case ID_R2:
			SIN = FALSE;
			SPRING = TRUE;
			ELLIPSE = FALSE;
			break;
		case ID_R3:
			SIN = FALSE;
			SPRING = FALSE;
			ELLIPSE = TRUE;
			break;

		case ID_R_LARGE:
			size = 40;
			break;
		case ID_R_MIDDLE:
			size = 20;
			break;
		case ID_R_SMALL:
			size = 10;
			break;
		case ID_BUTTON:
			right++;
			if (right % 2 == 1) {
				RIGHT = FALSE;
			}
			else {
				RIGHT = TRUE;
			}
			break;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
	
		child_hWnd = CreateWindow("ChildClass", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 10, 10, 600, 600, hWnd, NULL, g_hinst, NULL);
		EndPaint(hWnd, &ps);
		break;
	
	case WM_KEYDOWN:
		if (wParam == VK_SPACE) {
			space++;
			if (space % 3 == 1) {
				SIN = TRUE;
				SPRING = FALSE;
				ELLIPSE = FALSE;
							
			}
			else if (space % 3 == 0) {
				SIN = FALSE;
				SPRING = TRUE;
				ELLIPSE = FALSE;
			}
			else if (space % 3 == 2) {
				SIN = FALSE;
				SPRING = FALSE;
				ELLIPSE = TRUE;
			}
		}
		
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
	//case에서 정의되지 않은 메시지는 커널이 처리하도록 메시지 전달
}

BOOL CALLBACK Dlg_1Proc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	HWND hButton;

	switch (iMessage)
	{
	
	}
	
	return 0;
}


