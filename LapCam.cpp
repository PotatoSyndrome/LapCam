// LapCam.cpp : Определяет точку входа для приложения.
//
#define _CRT_SECURE_NO_WARNINGS

#pragma comment(lib, "Comctl32.lib")

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include "framework.h"
#include "LapCam.h"
#include "CommCtrl.h"

#include "ControlElement.h"
#include "MakePhotoButton.h"
#include "DirectoryChooseButton.h"
#include "VideoRecordingButton.h"
#include "ExtensionDropDown.h"
#include "QualityDropDown.h"
#include "CamManager.h"

#define MAX_LOADSTRING 100
#define CHOOSE_BUTTON 1359
#define PHOTO_BUTTON 1360
#define VIDEO_BUTTON 1361
#define EXTENSION_DROP_DOWN 1362
#define QUALITY_DROP_DOWN 1363
#define WEB_CAMERA 1364

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

HANDLE camThread;

ControlElement* chooseDirectoryButton;
ControlElement* makePhotoButton;
ControlElement* videoRecordingButton;
ControlElement* extensionDropDown;
ControlElement* qualityDropDown;
CamManager*     camManager;

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
void                DeinitInstance();
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
DWORD WINAPI        WebCamProcessing(LPVOID lpParam);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);


    freopen("output.txt", "w", stdout);
    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LAPCAM, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAPCAM));

    MSG msg;

    // Цикл основного сообщения:

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    DeinitInstance();

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAPCAM));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAPCAM);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной
   InitCommonControls();
   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   camManager = new CamManager(25, 720, "", "AVI", hWnd);

   chooseDirectoryButton = new DirectoryChooseButton(40, 40, CHOOSE_BUTTON, hWnd);

   makePhotoButton = new MakePhotoButton(40, 120, PHOTO_BUTTON, hWnd, camManager);

   videoRecordingButton = new VideoRecordingButton(40, 200, VIDEO_BUTTON, hWnd, camManager);

   extensionDropDown = new ExtensionDropDown(40, 280, EXTENSION_DROP_DOWN, hWnd);

   qualityDropDown = new QualityDropDown(40, 360, QUALITY_DROP_DOWN, hWnd);

   camThread = CreateThread(
       NULL,                   // default security attributes
       0,                      // use default stack size  
       WebCamProcessing,       // thread function name
       0,                        // argument to thread function 
       0,                      // use default creation flags 
       NULL);
   
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

void DeinitInstance() {
    camManager->stopManager();
    WaitForSingleObject(camThread, 1000);

    delete makePhotoButton;
    delete chooseDirectoryButton;
    delete videoRecordingButton;
    delete extensionDropDown;
    delete qualityDropDown;
    delete camManager;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            
            // Разобрать выбор в меню:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case CHOOSE_BUTTON:
                chooseDirectoryButton->processMessage();
                break;
            case PHOTO_BUTTON:
                makePhotoButton->processMessage();
                break;
            case VIDEO_BUTTON:
                videoRecordingButton->processMessage();
                break;
            case EXTENSION_DROP_DOWN:
                extensionDropDown->processMessage();
                break;
            case QUALITY_DROP_DOWN:
                qualityDropDown->processMessage();
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT     ps;
            HDC             hdc;
            BITMAP          bitmap;
            HBITMAP         hBitmap;
            HDC             hdcMem;
            HGDIOBJ         oldBitmap;
            if (camManager) {
                hBitmap = camManager->getBitmap();
            }
            else
            {
                break;
            }
            hdc = BeginPaint(hWnd, &ps);
            //// TODO: Добавьте сюда любой код прорисовки, использующий HDC...


            if (camManager)
            {
                hdcMem = CreateCompatibleDC(hdc);
                oldBitmap = SelectObject(hdcMem, hBitmap);

                GetObject(hBitmap, sizeof(bitmap), &bitmap);
                BitBlt(hdc, 400, 50, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);

                SelectObject(hdcMem, oldBitmap);
                DeleteDC(hdcMem);
                DeleteObject(hBitmap);
            } 

            EndPaint(hWnd, &ps);
            
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

DWORD __stdcall WebCamProcessing(LPVOID lpParam)
{
    std::cout << "New thread working bitch" << std::endl;
    camManager->startManager();
    return 0;
}
