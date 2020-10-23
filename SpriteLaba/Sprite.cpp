#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
ATOM RegisterClass(HINSTANCE, LPCTSTR);
int drawObj();
int refreshClientScreen();
int timerTick(HWND hWnd);
void mouseWheel(WPARAM wParam);
void correctXY();
int correct(int value, int maxValue, int* speed);
void UpdateSize(HWND hWnd);

int mouseDownFlag = 0;
int X, Y = 0;
int width_window = 700, height_window = 700;
int width_client_window, height_client_window;
int Speed_X = 0, Speed_Y = 0;
HICON hicon = LoadIcon(NULL, IDI_QUESTION);
RECT win_rect{ 0, 0, width_client_window, height_client_window };
HDC hdc;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASS w;
	memset(&w, 0, sizeof(WNDCLASS));
	w.lpfnWndProc = (WNDPROC)WndProc;
	w.hInstance = hInstance;
	w.lpszClassName = L"Sprite";
	w.hCursor = LoadCursor(NULL, IDC_ARROW);
	w.hbrBackground = (HBRUSH)0;
	RegisterClass(&w);

	RECT screen_rect, window_rect;
	GetWindowRect(GetDesktopWindow(), &screen_rect);
	int x = screen_rect.left;
	int y = screen_rect.top;

	HWND hWnd = CreateWindow(L"Sprite", TEXT("Sprite"), WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_BORDER, x, y, width_window, height_window, NULL, NULL, hInstance, NULL);

	GetClientRect(hWnd, &window_rect); 
	width_client_window = window_rect.right;
	height_client_window = window_rect.bottom;

	PAINTSTRUCT ps;
	hdc = BeginPaint(hWnd, &ps);	
	drawObj();

	SetTimer(hWnd, 1, 30, NULL);
	MSG msg = { 0 };
	
	while (GetMessage(&msg, NULL, 0, 0) != 0) 
	{
		//TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	return msg.wParam; 
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_TIMER:
		timerTick(hWnd);
		break;
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostQuitMessage(0); 
		break;
	case WM_LBUTTONDOWN: 
		mouseDownFlag = 1;
		break;
	case WM_LBUTTONUP:
		mouseDownFlag = 0;
		break;
	case WM_MOUSEWHEEL:
		mouseWheel(wParam);
		break;
	case WM_SIZE:
		UpdateSize(hWnd);
		break;
	case WM_KEYDOWN: 
		switch (wParam)
		{
		case VK_RIGHT: 
			Speed_X += 5;
			refreshClientScreen();
			break;
		case VK_LEFT: 
			Speed_X -= 5;
			refreshClientScreen();
			break;
		case VK_UP: 
			Speed_Y -= 5;
			refreshClientScreen();
			break;
		case VK_DOWN:
			Speed_Y += 5;
			refreshClientScreen();
			break;
		}
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
int refreshClientScreen()
{
	FillRect(hdc, &win_rect, (HBRUSH)0);
	correctXY();
	return drawObj();
}

void mouseWheel(WPARAM wParam)
{                  
	int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
	int fwKeys = GET_KEYSTATE_WPARAM(wParam);
	if (fwKeys) {
		if (zDelta > 0) 
			X += 5;		
		if (zDelta < 0) 
			X -= 5;		
	}
	else {
		if (zDelta > 0) 
			Y += 5;		
		if (zDelta < 0) 
			Y -= 5;		
	}
	refreshClientScreen();
}

int timerTick(HWND hWnd)
{
	if (mouseDownFlag) {
		POINT pos;
		RECT rc;
		GetCursorPos(&pos);
		GetClientRect(hWnd, &rc);
		MapWindowPoints(hWnd, GetParent(hWnd), (LPPOINT)&rc, 2);
		X = pos.x - rc.left;
		Y = pos.y - rc.top;
		refreshClientScreen();
	}

	if ((Speed_X == 0) && (Speed_Y == 0)) return 0; 	
	X = X + Speed_X;
	Y = Y + Speed_Y;
	Speed_X = div(Speed_X, 2).quot;
	Speed_Y = div(Speed_Y, 2).quot;
	return refreshClientScreen();
}
void UpdateSize(HWND hWnd)
{
	RECT window_rect;
	GetClientRect(hWnd, &window_rect);
	width_client_window = window_rect.right;
	height_client_window = window_rect.bottom;
	correctXY();
	win_rect = { 0, 0, width_client_window, height_client_window };
	refreshClientScreen();
}

void correctXY()
{
	X = correct(X, width_client_window, &Speed_X);
	Y = correct(Y, height_client_window, &Speed_Y);
	return;
}

int correct(int value,  int maxValue, int* speed)
{
	if ((value >= 0) && (value <= maxValue - 100)) return value;
	if (value < 0)
	{
		*speed += 50;
		return 0;
	}
	*speed -= 50;
	return maxValue;
}

int drawObj()
{		
	RECT rect{ X, Y, X + 100, Y + 100 };
		
	return FillRect(hdc, &rect, (HBRUSH)9);
}