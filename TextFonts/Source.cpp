#include <windows.h>

#define ROWS 5
#define COLUMNS 5
#define WHITE_COLOR RGB(255, 255, 255)
#define BLACK_COLOR RGB(0, 0, 0)

RECT rectangle;
int winWidth, winHeight, cellWidth, cellHeight;
char array[ROWS * COLUMNS][50];

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void DrawTable(HDC);
void DrawRect(HWND);
void DrawLine(HDC, COLORREF, int, int, int, int);
void DrawHorizontalLines(HDC, COLORREF, int);
void DrawVerticalLines(HDC, COLORREF, int);
void ArrayGenerator();
LOGFONT GenerateFont(int);
LOGFONT SelectFont(HDC, LPCWSTR);


int CALLBACK wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex;
	HWND hWnd;
	MSG msg;
	HDC hdc;

	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hIconSm = wcex.hIcon;
	wcex.hInstance = hInstance;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_VREDRAW | CS_HREDRAW;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"FirstLabClass";

	RegisterClassEx(&wcex);

	hWnd = CreateWindow(L"FirstLabClass", L"Hello, World!", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	hdc = GetDC(hWnd);

	ShowWindow(hWnd, nCmdShow);

	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

	}

	return (int)msg.wParam;

}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(EXIT_SUCCESS);
		break;
	}

	case WM_CREATE:
	{
		winWidth = LOWORD(lParam);
		winHeight = HIWORD(lParam);
		GetWindowRect(hWnd, &rectangle);
		cellWidth = (rectangle.right - rectangle.left) / COLUMNS - 5;
		cellHeight = (rectangle.bottom - rectangle.top) / ROWS - 5;
		ArrayGenerator();
		DrawRect(hWnd);
		break;
	}

	case WM_SIZE:
	{
		winWidth = LOWORD(lParam);
		winHeight = HIWORD(lParam);
		GetWindowRect(hWnd, &rectangle);
		cellWidth = (rectangle.right - rectangle.left) / COLUMNS - 5;
		cellHeight = (rectangle.bottom - rectangle.top) / ROWS - 5;
		DrawRect(hWnd);
		break;
	}
	}
	return DefWindowProc(hWnd, message, wParam, lParam);

}

void DrawRect(HWND hWnd)
{
	INT indent = 5, maxRowHight = 0;
	RECT rect, textCell;

	PAINTSTRUCT ps;
	HDC dc = BeginPaint(hWnd, &ps);

	SelectObject(dc, GetStockObject(DC_BRUSH));
	SetDCBrushColor(dc, WHITE_COLOR);
	SelectObject(dc, GetStockObject(DC_PEN));
	SetDCPenColor(dc, WHITE_COLOR);
	Rectangle(dc, 0, 0, winWidth, winHeight);

	DrawVerticalLines(dc, BLACK_COLOR, cellWidth);
	DrawHorizontalLines(dc, BLACK_COLOR, cellHeight);
	for (int i = 0; i < ROWS; i++)
	{
		rect.top = cellHeight * i;
		for (int j = 0; j < COLUMNS; j++)
		{
			rect.left = cellWidth * j;
			rect.right = cellWidth * (j + 1);

			SetBkMode(dc, TRANSPARENT);
			SetTextColor(dc, BLACK_COLOR);

			textCell.top = rect.top + indent;
			textCell.right = rect.right - indent;
			textCell.left = rect.left + indent;

			LOGFONT lf = SelectFont(dc, (LPCWSTR)array[COLUMNS * i + j]);
			HFONT hFont = CreateFontIndirect(&lf);
			SelectObject(dc, hFont);

			DrawText(dc, (LPCWSTR)array[COLUMNS * i + j], -1, &textCell, DT_CALCRECT | DT_WORDBREAK | DT_CENTER | DT_EDITCONTROL);
			rect.right = cellWidth * (j + 1);
			rect.bottom = textCell.bottom + indent;

			DrawText(dc, (LPCWSTR)array[COLUMNS * i + j], -1, &textCell, DT_WORDBREAK);
			DeleteObject(hFont);
		}
	}


}
void DrawLine(HDC dc, COLORREF color, int x1, int y1, int x2, int y2)
{
	HPEN pen = CreatePen(PS_COSMETIC, 3, color);
	POINT point;
	SelectObject(dc, pen);
	MoveToEx(dc, x1, y1, &point);
	LineTo(dc, x2, y2);
}
void DrawVerticalLines(HDC dc, COLORREF color, int cellWidth)
{
	for (int i = 0; i < COLUMNS; i++)
	{
		DrawLine(dc, color, i * cellWidth, 0, i * cellWidth, winHeight);
	}
}
void DrawHorizontalLines(HDC dc, COLORREF color, int cellWidth)
{
	for (int i = 0; i < COLUMNS; i++)
	{
		DrawLine(dc, color, 0, i * cellHeight, winWidth, i * cellHeight);
	}
}
void ArrayGenerator()
{
	for (int i = 0; i < ROWS * COLUMNS; i++)
	{
		int tmp = rand() % 50;
		for (int j = 0; j < tmp; j++)
		{
			array[i][j] = rand() % 255;
		}
	}
}
LOGFONT GenerateFont(int i)
{
	LOGFONT lf;
	HFONT hFont;
	LONG fontHeight = i;
	LONG fontWidth = i;

	lf.lfHeight = fontHeight;
	lf.lfWidth = fontWidth;
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfPitchAndFamily = FIXED_PITCH;
	lf.lfWeight = 400;
	lf.lfEscapement = 0;
	lf.lfItalic = 0;
	lf.lfUnderline = 0;
	lf.lfStrikeOut = 0;
	return lf;
}
LOGFONT SelectFont(HDC dc, LPCWSTR str)
{
	RECT tmp, rect = { 0,0,cellWidth,cellHeight };
	BOOLEAN a = true;
	int size = 5;
	LOGFONT nowFont = GenerateFont(size);
	LOGFONT prewFont = nowFont;
	HFONT hFont = CreateFontIndirect(&nowFont);
	SelectObject(dc, hFont);
	while (a)
	{
		DrawText(dc, str, -1, &rect, DT_WORDBREAK | DT_CALCRECT);
		if (rect.right >= cellWidth || rect.bottom >= cellHeight)
		{
			a = false;
		}
		else
		{
			prewFont = nowFont;
			DeleteObject(hFont);
			nowFont = GenerateFont(size);
			hFont = CreateFontIndirect(&nowFont);
			SelectObject(dc, hFont);
			size += 5;
		}
	}
	return prewFont;
}



