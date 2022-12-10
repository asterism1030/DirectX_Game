// PingPong.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "PingPong.h"

#define MAX_LOADSTRING 100

#define BALL_R 10
#define TENNIS_W 10
#define TENNIS_H 60
#define STEP 5

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// TODO) 별도 클래스를 생성해서 분리
static UINT player1Score = 0;
static UINT player2Score = 0;

static POINT ptP1Score = { 50, 30 };    // left, top
static POINT ptP2Score = { 650, 30 };
static POINT ptBall = { 450, 200 };     // center
static POINT ptPlayer1 = { 100, 200 };  // left, top
static POINT ptPlayer2 = { 800, 200 };


// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PINGPONG, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PINGPONG));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PINGPONG));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PINGPONG);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // 좌표계 계산을 위함
    static POINT ptMouse;
    static TCHAR szPos[128];

    static RECT rtClient;

    

    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_LBUTTONDOWN:
    {
        // TODO ) 좌표 확인용으로 지울 예정
        ptMouse.x = LOWORD(lParam);
        ptMouse.y = HIWORD(lParam);
        wsprintf(szPos, _T("(%d, %d)"), ptMouse.x, ptMouse.y);

        InvalidateRect(hWnd, NULL, TRUE); // WM_PAINT 메시지 발생
    }
    case WM_KEYDOWN:
    {
        // ws, 방향키
        // TODO) 동시 입력 처리 (동시 입력이 되지 않음), 경계선 처리
        switch (wParam) {
        // Player 1
        case 'W':
            ptPlayer1.y -= STEP;
            break;
        case 'S':
            ptPlayer1.y += STEP;
            break;
        // Player 2
        case VK_UP:
            ptPlayer2.y -= STEP;
            break;
        case VK_DOWN:
            ptPlayer2.y += STEP;
            break;
        }

        InvalidateRect(hWnd, NULL, TRUE);
    }
    break;
    case WM_SIZE:
    {
        GetClientRect(hWnd, &rtClient);
    }
    break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            // 클릭한 위치
            DrawText(hdc, szPos, -1, &rtClient, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            ////// 상단 UI
            TCHAR szPlayer[128];

            wsprintf(szPlayer, _T("Player1 :  %d"), player1Score);
            TextOut(hdc, ptP1Score.x, ptP1Score.y, szPlayer, lstrlen(szPlayer));
            wsprintf(szPlayer, _T("Player2:  %d"), player2Score);
            TextOut(hdc, ptP2Score.x, ptP2Score.y, szPlayer, lstrlen(szPlayer));

            ////// 중단 UI
            // 각 플레이어의 막대기와 공 생성
            // TODO ) 색채우기, 화면 비율 위치로 변경, 더블 버퍼링 사용
            Rectangle(hdc, ptPlayer1.x, ptPlayer1.y, ptPlayer1.x + TENNIS_W, ptPlayer1.y + TENNIS_H);
            Rectangle(hdc, ptPlayer2.x, ptPlayer2.y, ptPlayer2.x + TENNIS_W, ptPlayer2.y + TENNIS_H);

            // TODO ) 공의 물리적 동작, 점수 계산
            Ellipse(hdc, ptBall.x - BALL_R, ptBall.y - BALL_R, ptBall.x + BALL_R, ptBall.y + BALL_R);

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

// 정보 대화 상자의 메시지 처리기입니다.
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
