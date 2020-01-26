//-----头文件包含-----//
#include"TinyAVG.h"

//-----全局变量定义-----//

//窗口实例句柄.
HWND g_hwnd;

//TinyAVG实例化对象
TinyAVG g_TinyAVG;

//-----全局函数声明-----//

//窗口过程函数.
LRESULT CALLBACK WndProc(
	_In_ HWND   g_hwnd,
	_In_ UINT   uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
);

//-----Winmain()主函数定义-----//

int WINAPI WinMain(
	_In_		HINSTANCE	hInstance,
	_In_opt_	HINSTANCE	hPrevInstance,
	_In_		LPSTR		lpCmdLine,
	_In_		int			nShowCmd
) {
	HRESULT hr;

	hr = CoInitialize(NULL);

	hr = g_TinyAVG.CreateDeviceIndependentResources();

	WNDCLASSEX WndClass = { 0 };

	WndClass.cbSize = sizeof(WNDCLASSEX);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hIcon = (HICON)LoadImage(NULL, "icon.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = "TinyAVG";

	if (!RegisterClassEx(&WndClass)) {
		return -1;
	}

	g_hwnd = CreateWindow(
		"TinyAVG",
		"TinyAVG",
		WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);

	g_TinyAVG.Sethwnd(g_hwnd);

	ShowWindow(g_hwnd, nShowCmd);
	UpdateWindow(g_hwnd);

	MSG msg = { 0 };

	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, g_hwnd, 0, 0, PM_REMOVE) != 0) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			g_TinyAVG.OnRender();
			ValidateRect(g_hwnd, NULL);
		}
	}

	g_TinyAVG.~TinyAVG();

	CoUninitialize();
	return 0;
}

LRESULT CALLBACK WndProc(
	_In_ HWND   hwnd,
	_In_ UINT   uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
) {
	switch (uMsg) {
	case WM_PAINT:
		g_TinyAVG.OnRender();
		ValidateRect(hwnd, NULL);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) {
			DestroyWindow(hwnd);
			exit(0);
		}
		else if (wParam == VK_SPACE) {
			g_TinyAVG.OnSpaceKey();
		}
		break;

	case WM_SIZE:
		g_TinyAVG.OnResize(
			LOWORD(lParam),
			HIWORD(lParam)
		);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		exit(0);
		break;

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return 0;
}
