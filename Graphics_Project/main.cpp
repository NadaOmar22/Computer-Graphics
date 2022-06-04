#include <tchar.h>
#include <windows.h>
#include <iostream>
#include <math.h>
#include <stack>
#include <list>
#include<Vector>
#include <fstream>



#define MAXENTRIES 600;


using namespace std;

enum algorithms
{
    DDA, parametric, midPointLine,
    directCircle, polarCircle, iterativePolarCircle, midPointCircle, modifiedMidPoint,
    fillCircleWithLineQ1, fillCircleWithLineQ2, fillCircleWithLineQ3, fillCircleWithLineQ4,
    fillCircleWithCircleQ1, fillCircleWithCircleQ2, fillCircleWithCircleQ3, fillCircleWithCircleQ4,
    directEllipse, polarEllipse, midPointEllipse, cardinalSpline,
    square, rectangle, quad, squareFilled, rectangleFilled,
    rectangleClippingPoint, rectangleClippingLine, rectangleClippingPolygon,
    squareClippingPoint, squareClippingLine,
    circleClippingPoint, circleClippingLine,
    recursiveFill, nonRecursiveFill, convexFill, nonConvexFill
};

struct Point {
    int arr[2];
    Point(int x = 0, int y = 0)
    {
        arr[0] = x;
        arr[1] = y;
    }
    int& operator[](int i)
    {
        return arr[i];
    }

};

Point operator + (Point a, Point b) {
    return Point(a.arr[0] + b.arr[0], a.arr[1] + b.arr[1]);
}

Point operator - (Point a, Point b) {
    return Point(a.arr[0] - b.arr[0], a.arr[1] - b.arr[1]);
}

Point operator * (int s, Point a) {
    return Point(s * a.arr[0], s * a.arr[1]);
}
vector<Point>points;
vector<COLORREF>colors;

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain(HINSTANCE hThisInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpszArgument,
    int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */


    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof(WNDCLASSEX);


    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    ///Try to change the shape of your window mouse
    wincl.hCursor = LoadCursor(NULL, IDC_HAND);

    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;   /* structure or the window instance */

    ///Change the background of window to be white
    wincl.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx(&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx(
        0,                   /* Extended possibilites for variation */
        szClassName,         /* Classname */
        _T("Graphics Project"),       /* Title Text */
        WS_OVERLAPPEDWINDOW, /* default window */
        CW_USEDEFAULT,       /* Windows decides the position */
        CW_USEDEFAULT,       /* where the window ends up on the screen */
        900,                 /* The programs width */
        700,                 /* and height in pixels */
        HWND_DESKTOP,        /* The window is a child-window to desktop */
        HMENU(),                /* No menu */
        hThisInstance,       /* Program Instance handler */
        NULL                 /* No Window Creation data */
    );

    /* Make the window visible on the screen */
    ShowWindow(hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage(&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}

void AddMenu(HWND hwnd)
{

    HMENU hMenubar = CreateMenu();
    HMENU hMenuColor = CreateMenu();
    HMENU hMenuFile = CreateMenu();
    HMENU hMenuLine = CreateMenu();
    HMENU hMenuCircle = CreateMenu();
    HMENU hMenuEllipse = CreateMenu();
    HMENU hMenuFillCircle = CreateMenu();

    HMENU hMenuCurve = CreateMenu();
    HMENU hMenuFill = CreateMenu();

    HMENU hMenuPolygon = CreateMenu();
    HMENU hMenuFilledPolygon = CreateMenu();

    HMENU hMenueRectangleClipping = CreateMenu();

    HMENU hMenueSquareClipping = CreateMenu();

    HMENU hMenueCircleClipping = CreateMenu();

    AppendMenuW(hMenuFile, MF_STRING, 11, L"&Clear");
    AppendMenuW(hMenuFile, MF_STRING, 12, L"&Save");
    AppendMenuW(hMenuFile, MF_STRING, 13, L"&Load");

    ///Colors
    AppendMenuW(hMenuColor, MF_STRING, 21, L"&Red");
    AppendMenuW(hMenuColor, MF_STRING, 22, L"&Green");
    AppendMenuW(hMenuColor, MF_STRING, 23, L"&Blue");
    AppendMenuW(hMenuColor, MF_STRING, 24, L"&Fuchsia");
    AppendMenuW(hMenuColor, MF_STRING, 25, L"&Yellow");
    AppendMenuW(hMenuColor, MF_STRING, 26, L"&Cyan");

    ///Line [DDA, Midpoint and parametric]
    AppendMenuW(hMenuLine, MF_STRING, 31, L"&DDA");
    AppendMenuW(hMenuLine, MF_STRING, 32, L"&MidPoint");
    AppendMenuW(hMenuLine, MF_STRING, 33, L"&Parametric");

    ///Circle (Direct, Polar, iterative Polar, midpoint and modified Midpoint)
    AppendMenuW(hMenuCircle, MF_STRING, 41, L"&Direct");
    AppendMenuW(hMenuCircle, MF_STRING, 42, L"&Polar");
    AppendMenuW(hMenuCircle, MF_STRING, 43, L"&Iterative Polar");
    AppendMenuW(hMenuCircle, MF_STRING, 44, L"&MidPoint");
    AppendMenuW(hMenuCircle, MF_STRING, 45, L"&Modified Midpoint");

    ///Filling Circle with lines
    AppendMenuW(hMenuFillCircle, MF_STRING, 51, L"&WithLineQ1");
    AppendMenuW(hMenuFillCircle, MF_STRING, 52, L"&WithLineQ2");
    AppendMenuW(hMenuFillCircle, MF_STRING, 53, L"&WithLineQ3");
    AppendMenuW(hMenuFillCircle, MF_STRING, 54, L"&WithLineQ4");

    ///Filling Circle with other Circles
    AppendMenuW(hMenuFillCircle, MF_STRING, 55, L"&WithCircleQ1");
    AppendMenuW(hMenuFillCircle, MF_STRING, 56, L"&WithCircleQ2");
    AppendMenuW(hMenuFillCircle, MF_STRING, 57, L"&WithCircleQ3");
    AppendMenuW(hMenuFillCircle, MF_STRING, 58, L"&WithCircleQ4");


    ///Ellipse [Direct, polar and midpoint]
    AppendMenuW(hMenuEllipse, MF_STRING, 61, L"&Direct");
    AppendMenuW(hMenuEllipse, MF_STRING, 62, L"&Polar");
    AppendMenuW(hMenuEllipse, MF_STRING, 63, L"&MidPoint");

    AppendMenuW(hMenuCurve, MF_STRING, 71, L"&CardinalSplineCurve");


    AppendMenuW(hMenuFill, MF_STRING, 72, L"&ConvexFill");
    AppendMenuW(hMenuFill, MF_STRING, 73, L"&NonConvexFill");
    AppendMenuW(hMenuFill, MF_STRING, 74, L"&RecursiveFloodFill");
    AppendMenuW(hMenuFill, MF_STRING, 75, L"&NonRecursiveFloodFill");

    AppendMenuW(hMenuPolygon, MF_STRING, 80, L"&quad");
    AppendMenuW(hMenuPolygon, MF_STRING, 81, L"&Square");
    AppendMenuW(hMenuPolygon, MF_STRING, 82, L"&Rectangle");
    AppendMenuW(hMenuFilledPolygon, MF_STRING, 83, L"&FilledSquare");
    AppendMenuW(hMenuFilledPolygon, MF_STRING, 84, L"&FilledRectangle");


    AppendMenuW(hMenueRectangleClipping, MF_STRING, 91, L"&RectangleClippingPoint");
    AppendMenuW(hMenueRectangleClipping, MF_STRING, 92, L"&RectangleClippingLine");
    AppendMenuW(hMenueRectangleClipping, MF_STRING, 93, L"&RectangleClippingPolygon");

    AppendMenuW(hMenueSquareClipping, MF_STRING, 101, L"&SquareClippingPoint");
    AppendMenuW(hMenueSquareClipping, MF_STRING, 102, L"&SquareClippingLine");

    AppendMenuW(hMenueCircleClipping, MF_STRING, 111, L"&CircleClippingPoint");
    AppendMenuW(hMenueCircleClipping, MF_STRING, 112, L"&CircleClippingLine");


    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hMenuFile, L"&File");
    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hMenuColor, L"&Colors");
    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hMenuLine, L"&Line");
    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hMenuCircle, L"&Circle");
    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hMenuFillCircle, L"&FilledCircle");
    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hMenuEllipse, L"&Ellipse");
    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hMenuCurve, L"&Curve");

    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hMenuFill, L"&Fill");

    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hMenuPolygon, L"&Polygon");
    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hMenuFilledPolygon, L"&FilledPolygon");

    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hMenueRectangleClipping, L"&RectangleClipping");
    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hMenueSquareClipping, L"&SquareClipping");
    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hMenueCircleClipping, L"&CircleClipping");


    SetMenu(hwnd, hMenubar);
}

void swap(int& x1, int& y1, int& x2, int& y2)
{
    int tmp = x1;
    x1 = x2;
    x2 = tmp;
    tmp = y1;
    y1 = y2;
    y2 = tmp;
}

int Round(double x)
{
    return (int)(x + 0.5);
}


void get_RGB_values(COLORREF color, int& R, int& G, int& B)
{
    R = color & 0xff;
    G = (color >> 8) & 0xff;
    B = (color >> 16) & 0xff;
}

///------------------------------Line Algorithms---------------------------///
void drawLineDDA(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c)
{
    int dx = x2 - x1;
    int dy = y2 - y1;

    if (abs(dy) <= abs(dx))
    {
        if (x1 > x2)
            swap(x1, y1, x2, y2);
        SetPixel(hdc, x1, y1, c);
        Point p;
        p[0] = x1;
        p[1] = y1;
        points.push_back(p);
        colors.push_back(c);
        int x = x1;
        double y = y1;
        double m = (double)dy / dx;
        while (x < x2)
        {
            x++;
            y += m;
            SetPixel(hdc, x, Round(y), c);
            Point p;
            p[0] = x;
            p[1] = Round(y);
            points.push_back(p);
            colors.push_back(c);
        }
    }
    else
    {
        if (y1 > y2)
            swap(x1, y1, x2, y2);
        SetPixel(hdc, x1, y1, c);
        double x = x1;
        int y = y1;
        double m = (double)dx / dy;
        while (y < y2)
        {
            y++;
            x += m;
            SetPixel(hdc, Round(x), y, c);
            Point p;
            p[0] = Round(x);
            p[1] = y;
            points.push_back(p);
            colors.push_back(c);
        }
    }
}

void drawLineParametric(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c)
{
    int dx = x2 - x1;
    int dy = y2 - y1;

    for (double t = 0; t <= 1; t += 0.001)
    {
        int x = x1 + (int)(dx * t);
        int y = y1 + (int)(dy * t);
        SetPixel(hdc, x, y, c);
        Point p;
        p[0] = x;
        p[1] = y;
        points.push_back(p);
        colors.push_back(c);
    }
}

void drawLineMidPoint(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c)
{
    int dx = x2 - x1;
    int dy = y2 - y1;

    if (abs(dy) <= abs(dx))
    {

        if (x1 > x2)
        {
            swap(x1, y1, x2, y2);
            dx *= -1;
            dy *= -1;
        }

        double d = 0;
        int x = x1;
        int y = y1;

        SetPixel(hdc, x, y, c);
        Point p;
        p[0] = x;
        p[1] = y;
        points.push_back(p);
        colors.push_back(c);

        while (x < x2)
        {
            if (y1 >= y2)
            {
                d = dx * (y - 0.5 - y1) - dy * (x + 1 - x1);
                x++;
                if (d >= 0)
                    y--;
            }
            else
            {
                d = dx * (y + 0.5 - y1) - dy * (x + 1 - x1);
                x++;
                if (d <= 0)
                    y++;
            }
            SetPixel(hdc, x, y, c);
            Point p;
            p[0] = x;
            p[1] = y;
            points.push_back(p);
            colors.push_back(c);
        }
    }

    else
    {
        if (y1 > y2)
        {
            swap(x1, y1, x2, y2);
            dy *= -1;
            dx *= -1;
        }
        double d = 0;
        int x = x1;
        int y = y1;
        SetPixel(hdc, x, y, c);
        Point p;
        p[0] = x;
        p[1] = y;
        points.push_back(p);
        colors.push_back(c);

        while (y < y2)
        {
            if (x1 >= x2)
            {
                d = dx * (y + 1 - y1) - dy * (x - 0.5 - x1);
                y++;
                if (d < 0)
                    x--;
            }
            else
            {
                d = dx * (y + 1 - y1) - dy * (x + 0.5 - x1);
                y++;
                if (d > 0)
                    x++;
            }
            SetPixel(hdc, x, y, c);
            Point p;
            p[0] = x;
            p[1] = y;
            points.push_back(p);
            colors.push_back(c);
        }
    }
}


///------------------------------Circle Algorithms---------------------------///
void draw8Points(HDC hdc, int xc, int yc, int x, int y, COLORREF c)
{
    Point p;
    SetPixel(hdc, xc + x, yc + y, c);
    p[0] = xc + x;
    p[1] = yc + y;
    points.push_back(p);
    colors.push_back(c);

    SetPixel(hdc, xc - x, yc + y, c);
    p[0] = xc - x;
    p[1] = yc + y;
    points.push_back(p);
    colors.push_back(c);

    SetPixel(hdc, xc + x, yc - y, c);
    p[0] = xc + x;
    p[1] = yc - y;
    points.push_back(p);
    colors.push_back(c);

    SetPixel(hdc, xc - x, yc - y, c);
    p[0] = xc - x;
    p[1] = yc - y;
    points.push_back(p);
    colors.push_back(c);


    SetPixel(hdc, xc + y, yc + x, c);
    p[0] = xc + y;
    p[1] = yc + x;
    points.push_back(p);
    colors.push_back(c);

    SetPixel(hdc, xc - y, yc + x, c);
    p[0] = xc - y;
    p[1] = yc + x;
    points.push_back(p);
    colors.push_back(c);

    SetPixel(hdc, xc + y, yc - x, c);
    p[0] = xc + y;
    p[1] = yc - x;
    points.push_back(p);
    colors.push_back(c);

    SetPixel(hdc, xc - y, yc - x, c);
    p[0] = xc - y;
    p[1] = yc - x;
    points.push_back(p);
    colors.push_back(c);
}

void drawCircleDirect(HDC hdc, int xc, int yc, int r, COLORREF c)
{
    int x = 0;
    double y = r;
    draw8Points(hdc, xc, yc, 0, r, c);
    while (x < y)
    {
        x++;
        y = pow(r * r - x * x, 0.5);
        draw8Points(hdc, xc, yc, x, Round(y), c);
    }
}

void drawCirclePolar(HDC hdc, int xc, int yc, int r, COLORREF c)
{
    double x = r;
    double y = 0;
    double theta = 0;
    draw8Points(hdc, xc, yc, r, 0, c);
    while (x > y)
    {
        theta += 1.0 / r;
        x = r * cos(theta);
        y = r * sin(theta);
        draw8Points(hdc, xc, yc, Round(x), Round(y), c);
    }
}

void drawCirclePolarIterative(HDC hdc, int xc, int yc, int R, COLORREF c)
{
    double x = 0;
    double y = R;
    double dtheta = 1.0 / R;
    double ct = cos(dtheta);
    double st = sin(dtheta);
    draw8Points(hdc, xc, yc, 0, R, c);
    while (x < y)
    {
        double x1 = y * st + x * ct;
        y = y * ct - x * st;
        x = x1;
        draw8Points(hdc, xc, yc, Round(x), Round(y), c);
    }
}

void drawCircleMidPoint(HDC hdc, int xc, int yc, int r, COLORREF c)
{
    int x = 0;
    int y = r;
    draw8Points(hdc, xc, yc, r, 0, c);
    while (x < y)
    {
        double d = pow(x + 1, 2) + pow(y - 0.5, 2) - r * r;
        if (d < 0)
        {
            x++;
        }
        else
        {
            x++;
            y--;
        }
        draw8Points(hdc, xc, yc, x, y, c);
    }
}

void drawCircleModifiedMidpoint(HDC hdc, int xc, int yc, int r, COLORREF c)
{
    int x = 0;
    int y = r;
    int d = 1 - r;
    int d1 = 3;
    int d2 = 5 - 2 * r;
    draw8Points(hdc, xc, yc, r, 0, c);
    while (x < y)
    {
        if (d < 0)
        {
            d += d1;
            d1 += 2;
            d2 += 2;
            x++;
        }
        else
        {
            d += d2;
            d1 += 2;
            d2 += 4;
            x++;
            y--;
        }
        draw8Points(hdc, xc, yc, x, y, c);
    }
}

///Quarter Filled Circle
void draw2Lines(HDC hdc, int xc, int yc, int x, int y, COLORREF c, int choice)
{
    if (choice == 1)
    {
        SetPixel(hdc, xc + x, yc - y, c);
        Point p;
        p[0] = xc + x;
        p[1] = yc - y;
        points.push_back(p);
        colors.push_back(c);

        drawLineParametric(hdc, xc, yc, xc + x, yc - y, c);

        SetPixel(hdc, xc + y, yc - x, c);
        p[0] = xc + y;
        p[1] = yc - x;
        points.push_back(p);
        colors.push_back(c);

        drawLineParametric(hdc, xc, yc, xc + y, yc - x, c);
    }

    if (choice == 2)
    {
        SetPixel(hdc, xc - x, yc - y, c);
        Point p;
        p[0] = xc - x;
        p[1] = yc - y;
        points.push_back(p);
        colors.push_back(c);

        drawLineParametric(hdc, xc, yc, xc - x, yc - y, c);

        SetPixel(hdc, xc - y, yc - x, c);
        p[0] = xc - y;
        p[1] = yc - x;
        points.push_back(p);
        colors.push_back(c);

        drawLineParametric(hdc, xc, yc, xc - y, yc - x, c);
    }

    if (choice == 3)
    {
        SetPixel(hdc, xc - x, yc + y, c);
        Point p;
        p[0] = xc - x;
        p[1] = yc + y;
        points.push_back(p);
        colors.push_back(c);

        drawLineParametric(hdc, xc, yc, xc - x, yc + y, c);

        SetPixel(hdc, xc - y, yc + x, c);
        p[0] = xc - y;
        p[1] = yc + x;
        points.push_back(p);
        colors.push_back(c);

        drawLineParametric(hdc, xc, yc, xc - y, yc + x, c);
    }

    if (choice == 4)
    {
        SetPixel(hdc, xc + x, yc + y, c);
        Point p;
        p[0] = xc + x;
        p[1] = yc + y;
        points.push_back(p);
        colors.push_back(c);

        drawLineParametric(hdc, xc, yc, xc + x, yc + y, c);

        SetPixel(hdc, xc + y, yc + x, c);
        p[0] = xc + y;
        p[1] = yc + x;
        points.push_back(p);
        colors.push_back(c);

        drawLineParametric(hdc, xc, yc, xc + y, yc + x, c);
    }

}

void drawQuarterFilledCircle(HDC hdc, int xc, int yc, int r, COLORREF c, int choice)
{
    int x = 0;
    int y = r;
    int d = 1 - r;
    int d1 = 3;
    int d2 = 5 - 2 * r;
    draw8Points(hdc, xc, yc, r, 0, c);
    draw2Lines(hdc, xc, yc, r, 0, c, choice);
    while (x < y)
    {
        if (d < 0)
        {
            d += d1;
            d1 += 2;
            d2 += 2;
            x++;
        }
        else
        {
            d += d2;
            d1 += 2;
            d2 += 4;
            x++;
            y--;
        }
        draw8Points(hdc, xc, yc, x, y, c);          //for drawing the whole circle
        draw2Lines(hdc, xc, yc, x, y, c, choice);   //for filling one quarter
    }
}

void drawArcOfCircle(HDC hdc, int xc, int yc, int r, int arc, COLORREF c)
{
    int x = 0;
    int y = r;
    int d = 1 - r;
    int d1 = 3;
    int d2 = 5 - 2 * r;

    if (arc == 1)
    {
        SetPixel(hdc, xc + r, yc - 0, c);
        Point p;
        p[0] = xc + r;
        p[1] = yc - 0;
        points.push_back(p);
        colors.push_back(c);

        SetPixel(hdc, xc + 0, yc - r, c);
        p[0] = xc + 0;
        p[1] = yc - r;
        points.push_back(p);
        colors.push_back(c);
    }
    else if (arc == 2)
    {
        SetPixel(hdc, xc - r, yc - 0, c);
        Point p;
        p[0] = xc - r;
        p[1] = yc - 0;
        points.push_back(p);
        colors.push_back(c);

        SetPixel(hdc, xc - 0, yc - r, c);
        p[0] = xc - 0;
        p[1] = yc - r;
        points.push_back(p);
        colors.push_back(c);
    }
    else if (arc == 3)
    {
        SetPixel(hdc, xc - r, yc + 0, c);
        Point p;
        p[0] = xc - r;
        p[1] = yc + 0;
        points.push_back(p);
        colors.push_back(c);

        SetPixel(hdc, xc - 0, yc + r, c);
        p[0] = xc - r;
        p[1] = yc - 0;
        points.push_back(p);
        colors.push_back(c);
    }
    else
    {
        SetPixel(hdc, xc + r, yc + 0, c);
        Point p;
        p[0] = xc + r;
        p[1] = yc + 0;
        points.push_back(p);
        colors.push_back(c);

        SetPixel(hdc, xc + 0, yc + r, c);
        p[0] = xc + 0;
        p[1] = yc + r;
        points.push_back(p);
        colors.push_back(c);
    }
    while (x < y)
    {
        if (d < 0)
        {
            d += d1;
            d1 += 2;
            d2 += 2;
            x++;
        }
        else
        {
            d += d2;
            d1 += 2;
            d2 += 4;
            x++;
            y--;
        }
        if (arc == 1)
        {
            SetPixel(hdc, xc + y, yc - x, c);
            Point p;
            p[0] = xc + y;
            p[1] = yc - x;
            points.push_back(p);
            colors.push_back(c);

            SetPixel(hdc, xc + x, yc - y, c);
            p[0] = xc + x;
            p[1] = yc - y;
            points.push_back(p);
            colors.push_back(c);
        }
        else if (arc == 2)
        {
            SetPixel(hdc, xc - y, yc - x, c);
            Point p;
            p[0] = xc - y;
            p[1] = yc - x;
            points.push_back(p);
            colors.push_back(c);

            SetPixel(hdc, xc - x, yc - y, c);
            p[0] = xc - x;
            p[1] = yc - y;
            points.push_back(p);
            colors.push_back(c);
        }
        else if (arc == 3)
        {
            SetPixel(hdc, xc - y, yc + x, c);
            Point p;
            p[0] = xc - y;
            p[1] = yc + x;
            points.push_back(p);
            colors.push_back(c);

            SetPixel(hdc, xc - x, yc + y, c);
            p[0] = xc - x;
            p[1] = yc + y;
            points.push_back(p);
            colors.push_back(c);
        }
        else
        {
            SetPixel(hdc, xc + y, yc + x, c);
            Point p;
            p[0] = xc + y;
            p[1] = yc + x;
            points.push_back(p);
            colors.push_back(c);

            SetPixel(hdc, xc + x, yc + y, c);
            p[0] = xc + x;
            p[1] = yc + y;
            points.push_back(p);
            colors.push_back(c);
        }
    }
}

void drawCircles(HDC hdc, int xc, int yc, int x, int y, COLORREF c, int choice)
{
    int xtemp, ytemp;
    double r;

    if (choice == 1)
    {
        SetPixel(hdc, xc + x, yc - y, c);
        Point p;
        p[0] = xc + x;
        p[1] = yc - y;
        points.push_back(p);
        colors.push_back(c);

        xtemp = xc + x;
        ytemp = yc - y;
        r = sqrt(((xtemp - xc) * (xtemp - xc)) + ((ytemp - yc) * (ytemp - yc)));
        while (r >= 0) {

            drawArcOfCircle(hdc, xc, yc, Round(r), 1, c);
            r--;
        }

        SetPixel(hdc, xc + y, yc - x, c);
        p[0] = xc + y;
        p[1] = yc - x;
        points.push_back(p);
        colors.push_back(c);

        xtemp = xc + y;
        ytemp = yc - x;
        r = sqrt(((xtemp - xc) * (xtemp - xc)) + ((ytemp - yc) * (ytemp - yc)));
        while (r >= 0) {
            drawArcOfCircle(hdc, xc, yc, Round(r), 1, c);
            r--;
        }
    }

    else if (choice == 2)
    {
        SetPixel(hdc, xc - x, yc - y, c);
        Point p;
        p[0] = xc - x;
        p[1] = yc - y;
        points.push_back(p);
        colors.push_back(c);

        xtemp = xc - x;
        ytemp = yc - y;
        r = sqrt(((xtemp - xc) * (xtemp - xc)) + ((ytemp - yc) * (ytemp - yc)));
        while (r >= 0) {
            drawArcOfCircle(hdc, xc, yc, Round(r), 2, c);
            r--;
        }

        SetPixel(hdc, xc - y, yc - x, c);
        p[0] = xc - y;
        p[1] = yc - x;
        points.push_back(p);
        colors.push_back(c);

        xtemp = xc - y;
        ytemp = yc - x;
        r = sqrt(((xtemp - xc) * (xtemp - xc)) + ((ytemp - yc) * (ytemp - yc)));
        while (r >= 0) {

            drawArcOfCircle(hdc, xc, yc, Round(r), 2, c);
            r--;
        }
    }

    else if (choice == 3)
    {
        SetPixel(hdc, xc - x, yc + y, c);
        Point p;
        p[0] = xc - x;
        p[1] = yc + y;
        points.push_back(p);
        colors.push_back(c);

        xtemp = xc - x;
        ytemp = yc + y;
        r = sqrt(((xtemp - xc) * (xtemp - xc)) + ((ytemp - yc) * (ytemp - yc)));
        while (r >= 0) {
            drawArcOfCircle(hdc, xc, yc, Round(r), 3, c);
            r--;
        }
        SetPixel(hdc, xc - y, yc + x, c);
        p[0] = xc - y;
        p[1] = yc + x;
        points.push_back(p);
        colors.push_back(c);
    }

    else if (choice == 4)
    {
        SetPixel(hdc, xc + x, yc + y, c);
        Point p;
        p[0] = xc + x;
        p[1] = yc + y;
        points.push_back(p);
        colors.push_back(c);

        xtemp = xc + x;
        ytemp = yc + y;
        r = sqrt(((xtemp - xc) * (xtemp - xc)) + ((ytemp - yc) * (ytemp - yc)));
        while (r >= 0) {
            drawArcOfCircle(hdc, xc, yc, Round(r), 4, c);
            r--;
        }

        SetPixel(hdc, xc + y, yc + x, c);
        p[0] = xc + y;
        p[1] = yc + x;
        points.push_back(p);
        colors.push_back(c);

        xtemp = xc + y;
        ytemp = yc + x;
        r = sqrt(((xtemp - xc) * (xtemp - xc)) + ((ytemp - yc) * (ytemp - yc)));
        while (r >= 0) {
            drawArcOfCircle(hdc, xc, yc, Round(r), 4, c);
            r--;
        }
    }

}

void drawQuarterFilledCircleWithCircles(HDC hdc, int xc, int yc, int r, COLORREF c, int choice)
{
    int x = 0;
    int y = r;
    int d = 1 - r;
    int d1 = 3;
    int d2 = 5 - 2 * r;
    draw8Points(hdc, xc, yc, r, 0, c);
    drawCircles(hdc, xc, yc, r, 0, c, choice);
    while (x < y)
    {
        if (d < 0)
        {
            d += d1;
            d1 += 2;
            d2 += 2;
            x++;
        }
        else
        {
            d += d2;
            d1 += 2;
            d2 += 4;
            x++;
            y--;
        }
        draw8Points(hdc, xc, yc, x, y, c);          //for drawing the whole circle
    }

    drawCircles(hdc, xc, yc, x, y, c, choice);   //for filling one quarter
}


///------------------------------Ellipse Algorithms--------------------------------///
void draw4Points(HDC hdc, int xc, int yc, int x, int y, COLORREF c)
{
    SetPixel(hdc, xc + x, yc + y, c);
    Point p;
    p[0] = xc + x;
    p[1] = yc + y;
    points.push_back(p);
    colors.push_back(c);

    SetPixel(hdc, xc + x, yc - y, c);
    p[0] = xc + x;
    p[1] = yc - y;
    points.push_back(p);
    colors.push_back(c);

    SetPixel(hdc, xc - x, yc - y, c);
    p[0] = xc - x;
    p[1] = yc - y;
    points.push_back(p);
    colors.push_back(c);

    SetPixel(hdc, xc - x, yc + y, c);
    p[0] = xc - x;
    p[1] = yc + y;
    points.push_back(p);
    colors.push_back(c);

}

void drawDirectEllipse(HDC hdc, int xc, int yc, int A, int B, COLORREF c)
{
    int x = 0;
    double y = B;
    draw4Points(hdc, xc, yc, 0, B, c);
    while (x * B * B < y * A * A)
    {
        x++;
        y = B * sqrt(1.0 - (double)(x * x) / (A * A));
        draw4Points(hdc, xc, yc, x, Round(y), c);
    }
    int y1 = 0;
    double x1 = A;
    draw4Points(hdc, xc, yc, A, 0, c);
    while (x1 * B * B > y1 * A * A)
    {
        y1++;
        x1 = A * sqrt(1.0 - (double)(y1 * y1) / (B * B));
        draw4Points(hdc, xc, yc, Round(x1), y1, c);
    }
}

void drawPolarEllipse(HDC hdc, int xc, int yc, int A, int B, COLORREF c)
{
    double x = 0;
    double y = B;
    double theta = 0;
    draw4Points(hdc, xc, yc, 0, B, c);
    while (x * B * B < y * A * A)
    {
        theta += 1.0 / max(A, B);
        x = A * sin(theta);
        y = B * cos(theta);
        draw4Points(hdc, xc, yc, Round(x), Round(y), c);
    }
    double x1 = A;
    double y1 = 0;
    double theta1 = 0;
    draw4Points(hdc, xc, yc, A, 0, c);
    while (x1 * B * B > y1 * A * A)
    {
        theta1 += 1.0 / max(A, B);
        x1 = A * cos(theta1);
        y1 = B * sin(theta1);
        draw4Points(hdc, xc, yc, Round(x1), Round(y1), c);
    }
}

void drawEllipseMidPoint(HDC hdc, int xc, int yc, int A, int B, COLORREF c)
{
    int x = 0;
    int y = B;
    int dx = 2 * B * B * x;
    int dy = 2 * A * A * y;

    double d1 = (B * B) - (A * A * B) + (0.25 * A * A); //d initial
    draw4Points(hdc, xc, yc, 0, B, c);
    while (x * B * B < y * A * A)
    {
        if (d1 < 0)
        {
            x++;
            dx += 2 * B * B;
            d1 += dx + (B * B);
        }
        else
        {
            x++;
            y--;
            dx += 2 * B * B;
            dy -= 2 * A * A;
            d1 += (dx - dy) + (B * B);
        }
        draw4Points(hdc, xc, yc, x, y, c);
    }

    double d2 = (((x + 0.5) * (x + 0.5)) * (B * B)) + (((y - 1) * (y - 1)) * (A * A)) - (A * A * B * B);
    draw4Points(hdc, xc, yc, A, 0, c);
    while (y >= 0)
    {
        if (d2 > 0)
        {
            y--;
            dy -= 2 * A * A;
            d2 += (A * A) - dy;
        }
        else
        {
            y--;
            x++;
            dx += 2 * B * B;
            dy -= 2 * A * A;
            d2 += dx - dy + (A * A);
        }
        draw4Points(hdc, xc, yc, x, y, c);
    }

}


///------------------------------Curve Algorithms--------------------------------///
void drawHermiteCurve(HDC hdc, Point& p1, Point& T1, Point& p2, Point& T2, COLORREF c)
{
    double a0 = p1[0], a1 = T1[0],
        a2 = -3 * p1[0] - 2 * T1[0] + 3 * p2[0] - T2[0],
        a3 = 2 * p1[0] + T1[0] - 2 * p2[0] + T2[0];
    double b0 = p1[1], b1 = T1[1],
        b2 = -3 * p1[1] - 2 * T1[1] + 3 * p2[1] - T2[1],
        b3 = 2 * p1[1] + T1[1] - 2 * p2[1] + T2[1];
    for (double t = 0; t <= 1; t += 0.001)
    {
        double t2 = t * t, t3 = t2 * t;
        double x = a0 + a1 * t + a2 * t2 + a3 * t3;
        double y = b0 + b1 * t + b2 * t2 + b3 * t3;
        SetPixel(hdc, Round(x), Round(y), c);
        Point p;
        p[0] = Round(x);
        p[1] = Round(y);
        points.push_back(p);
        colors.push_back(c);
    }
}

void drawBezierCurve(HDC hdc, Point& p1, Point& p2, Point& p3, Point& p4, COLORREF c)
{
    for (double t = 0; t <= 1; t += 0.001)
    {
        double x = pow(1 - t, 3) * p1[0] + 3 * t * pow(1 - t, 2) * p2[0] + 3 * pow(t, 2) * (1 - t) * p3[0] + pow(t, 3) * p4[0];
        double y = pow(1 - t, 3) * p1[1] + 3 * t * pow(1 - t, 2) * p2[1] + 3 * pow(t, 2) * (1 - t) * p3[1] + pow(t, 3) * p4[1];
        SetPixel(hdc, Round(x), Round(y), c);
        Point p;
        p[0] = Round(x);
        p[1] = Round(y);
        points.push_back(p);
        colors.push_back(c);
    }
}

void cardinalSplineCurve(HDC hdc, Point p[], int n, int c, COLORREF color)
{
    Point T[1000];
    for (int i = 1; i < n - 1; i++)
        T[i] = c / 2 * (p[i + 1] - p[i - 1]);

    T[0] = c / 2 * (p[1] - p[0]);
    T[n - 1] = c / 2 * (p[n - 1] - p[n - 2]);

    for (int i = 0; i < n - 1; i++)
        drawHermiteCurve(hdc, p[i], T[i], p[i + 1], T[i + 1], color);

}


///------------------------------Polygons Algorithms--------------------------------///

void drawQuad(HDC hdc, Point p[], COLORREF c)
{
    drawLineParametric(hdc, p[0][0], p[0][1], p[1][0], p[1][1], c);
    drawLineParametric(hdc, p[1][0], p[1][1], p[2][0], p[2][1], c);
    drawLineParametric(hdc, p[2][0], p[2][1], p[3][0], p[3][1], c);
    drawLineParametric(hdc, p[3][0], p[3][1], p[0][0], p[0][1], c);
}

Point* drawSquare(HDC hdc, int x1, int y1, int x2, int y2, bool filled, COLORREF c)
{
    int x3 = 0, x4 = 0, y3 = 0, y4 = 0;

    if (x1 != x2 && y1 != y2 && abs(x2 - x1) < abs(y2 - y1))
    {
        if (x2 > x1)
            x1 = x2;
        else
            x2 = x1;
    }

    if (x1 != x2 && y1 != y2 && abs(x2 - x1) > abs(y2 - y1))
    {
        if (y2 > y1)
            y1 = y2;
        else
            y2 = y1;
    }

    if (x1 == x2 && y1 != y2)
    {
        if (y2 < y1)
        {
            swap(x1, y1, x2, y2);
        }
        drawLineParametric(hdc, x1, y1, x2, y2, c);
        x3 = y2 - y1 + x1;
        y3 = y2;
        drawLineParametric(hdc, x2, y2, x3, y3, c);
        y4 = y1;
        x4 = y2 - y1 + x1;
        drawLineParametric(hdc, x1, y1, x4, y4, c);
        drawLineParametric(hdc, x3, y3, x4, y4, c);

        if (filled)
        {
            for (double t = x1; t <= x4; t += 0.1)
            {
                Point p1((int)t, y1);
                Point p2((int)t, y2);
                Point p3(1, 1);
                Point p4(1, 1);
                drawHermiteCurve(hdc, p1, p3, p2, p4, c);
            }
        }
    }

    if (y1 == y2 && x1 != x2)
    {
        if (x2 < x1)
        {
            swap(x1, y1, x2, y2);
        }
        x3 = x1;
        y3 = x2 - x1 + y1;
        x4 = x2;
        y4 = x2 - x1 + y1;
        drawLineParametric(hdc, x1, y1, x2, y2, c);
        drawLineParametric(hdc, x1, y1, x3, y3, c);
        drawLineParametric(hdc, x3, y3, x4, y4, c);
        drawLineParametric(hdc, x2, y2, x4, y4, c);

        if (filled)
        {
            for (double t = x3; t <= x4; t += 0.1)
            {
                Point p1((int)t, y3);
                Point p2((int)t, y1);
                Point p3(1, 1);
                Point p4(1, 1);
                drawHermiteCurve(hdc, p1, p3, p2, p4, c);
            }
        }

    }

    Point  points[4];
    points[0] = Point(x1, y1);
    points[1] = Point(x2, y2);
    points[2] = Point(x3, y3);
    points[3] = Point(x4, y4);

    return points;
}


Point* drawRectangle(HDC hdc, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, bool filled, COLORREF c)
{
    if (y1 != y2)
    {
        if (y2 > y1)
            y1 = y2;
        else
            y2 = y1;
    }
    if (x2 != x3)
    {
        if (x2 > x3)
            x3 = x2;
        else
            x2 = x3;
    }
    x4 = x1;
    y4 = y3;

    drawLineParametric(hdc, x1, y1, x2, y2, c);
    drawLineParametric(hdc, x2, y2, x3, y3, c);
    drawLineParametric(hdc, x3, y3, x4, y4, c);
    drawLineParametric(hdc, x4, y4, x1, y1, c);

    if (filled)
    {
        for (double t = y1; t <= y4; t += 0.1)
        {
            Point p1(x1, (int)t);
            Point p2(x2, (int)t);
            Point p3(x1, (int)t);
            Point p4(x2, (int)t);
            drawBezierCurve(hdc, p1, p3, p2, p4, c);
        }

    }
    Point  points[4];
    points[0] = Point(x1, y1);
    points[1] = Point(x2, y2);
    points[2] = Point(x3, y3);
    points[3] = Point(x4, y4);

    return points;
}


///------------------------------clipping (circle) Algorithms--------------------------------///

void circleClippingPointAlgo(HDC hdc, int xc, int yc, int x, int y, int r, COLORREF c)
{
    int result = (x - xc) * (x - xc) + (y - yc) * (y - yc) - (r * r);
    if (result <= 0)
    {
        SetPixel(hdc, x, y, c);
        Point p;
        p[0] = x;
        p[1] = y;
        points.push_back(p);
        colors.push_back(c);
    }
    else
    {
        cout << "The point not inside the circle " << endl;
    }
}

void circleClippingLineAlgo(HDC hdc, int xc, int yc, int x1, int y1, int x2, int y2, int r, COLORREF c)
{
    int dx = x2 - x1;
    int dy = y2 - y1;
    bool check = false;
    for (double t = 0; t <= 1; t += 0.001)
    {
        int x = x1 + (int)(dx * t);
        int y = y1 + (int)(dy * t);
        int result = (x - xc) * (x - xc) + (y - yc) * (y - yc) - (r * r);
        if (result <= 0)
        {
            SetPixel(hdc, x, y, c);
            Point p;
            p[0] = x;
            p[1] = y;
            points.push_back(p);
            colors.push_back(c);
        }
        else
        {
            check = true;
        }
    }
    if (check == true)
    {
        cout << "There is part of the line outside the circle \n" << endl;
    }
}

///------------------------------clipping (Polygon) Algorithms--------------------------------///

void polygonClippingPoint(HDC hdc, int xTLeft, int yTLeft, int yBLeft, int xTRight, int x, int y, COLORREF c)
{
    if (x >= xTLeft && x <= xTRight && y <= yBLeft && y >= yTLeft)
    {
        SetPixel(hdc, x, y, c);
        Point p;
        p[0] = x;
        p[1] = y;
        points.push_back(p);
        colors.push_back(c);
        cout << "The point is inside the polygon\n";
    }
    else
    {
        cout << "The point is not inside the polygon, Clipping is done\n";
    }
}

void polygonClippingLine(HDC hdc, int xTLeft, int yTLeft, int yBLeft, int xTRight, int x1, int y1, int x2, int y2, COLORREF c)
{
    int dx = x2 - x1;
    int dy = y2 - y1;
    bool check = false;
    for (double t = 0; t <= 1; t += 0.001)
    {
        int x = x1 + (int)(dx * t);
        int y = y1 + (int)(dy * t);
        if (x >= xTLeft && x <= xTRight && y <= yBLeft && y >= yTLeft)
        {
            SetPixel(hdc, x, y, c);
            Point p;
            p[0] = x;
            p[1] = y;
            points.push_back(p);
            colors.push_back(c);

        }
        else
        {
            check = true;
        }
    }
    if (check == true)
    {
        cout << "There is part of the line outside the Square, Clipping is done" << endl;
    }
    else {
        cout << "The Line is inside the Square" << endl;
    }

}

void rectangleClippingPolygonAlgo(HDC hdc, int xTLeft, int yTLeft, int yBLeft, int xTRight, Point p1, Point p2, Point p3, Point p4, COLORREF c)
{
    polygonClippingLine(hdc, xTLeft, yTLeft, yBLeft, xTRight, p1[0], p1[1], p2[0], p2[1], c);
    polygonClippingLine(hdc, xTLeft, yTLeft, yBLeft, xTRight, p2[0], p2[1], p3[0], p3[1], c);
    polygonClippingLine(hdc, xTLeft, yTLeft, yBLeft, xTRight, p3[0], p3[1], p4[0], p4[1], c);
    polygonClippingLine(hdc, xTLeft, yTLeft, yBLeft, xTRight, p4[0], p4[1], p1[0], p1[1], c);
}


///------------------------------ Filling algorithms --------------------------------///

void recursiveFloodFill(HDC hdc, int x, int y, COLORREF bc, COLORREF fc) {
    COLORREF c = GetPixel(hdc, x, y);
    if (c == bc || c == fc) {
        return;
    }
    SetPixel(hdc, x, y, fc);
    Point p;
    p[0] = x;
    p[1] = y;
    points.push_back(p);
    colors.push_back(fc);
    recursiveFloodFill(hdc, x + 1, y, bc, fc);
    recursiveFloodFill(hdc, x - 1, y, bc, fc);
    recursiveFloodFill(hdc, x, y + 1, bc, fc);
    recursiveFloodFill(hdc, x, y - 1, bc, fc);
}

void nonRecursiveFloodFill(HDC hdc, int x, int y, COLORREF bc, COLORREF fc) {
    stack <Point>  s;
    s.push(Point(x, y));
    while (!s.empty()) {
        Point p = s.top();
        s.pop();
        COLORREF c = GetPixel(hdc, p[0], p[1]);
        if (c == bc || c == fc) {
            continue;
        }
        SetPixel(hdc, p[0], p[1], fc);
        points.push_back(p);
        colors.push_back(fc);
        s.push(Point(p[0] + 1, p[1]));
        s.push(Point(p[0] - 1, p[1]));
        s.push(Point(p[0], p[1] + 1));
        s.push(Point(p[0], p[1] - 1));
    }

}

//convex fill
struct Entry
{
    int xmin, xmax;
};

void InitEntries(Entry table[])
{
    for (int i = 0; i < 600; i++)
    {
        table[i].xmin = INT_MAX;
        table[i].xmax = -INT_MAX;
    }
}

void ScanEdge(Point v1, Point v2, Entry table[])
{
    if (v1[1] == v2[1])
        return;
    if (v1[1] > v2[1])
        swap(v1, v2);

    double minv = (double)(v2[0] - v1[0]) / (v2[1] - v1[1]);

    double x = v1[0];
    int y = v1[1];

    while (y < v2[1])
    {
        if (x < table[y].xmin)table[y].xmin = (int)ceil(x);
        if (x > table[y].xmax)table[y].xmax = (int)floor(x);
        y++;
        x += minv;
    }
}

void DrawSanLines(HDC hdc, Entry table[], COLORREF color)
{
    for (int y = 0; y < 600; y++) {
        if (table[y].xmin < table[y].xmax) {
            for (int x = table[y].xmin; x <= table[y].xmax; x++)
            {
                SetPixel(hdc, x, y, color);
                Point p;
                p[0] = x;
                p[1] = y;
                points.push_back(p);
                colors.push_back(color);
            }
        }
    }
}

void convexFillAlgo(HDC hdc, Point p[], int n, COLORREF color)
{
    Entry* table = new Entry[600];
    InitEntries(table);
    Point v1 = p[n - 1];
    for (int i = 0; i < n; i++)
    {
        Point v2 = p[i];
        ScanEdge(v1, v2, table);
        v1 = p[i];
    }
    DrawSanLines(hdc, table, color);
    delete table;
}

//non convex fill
struct EdgeRec
{
    double x;
    double minv;
    int ymax;
    bool operator<(EdgeRec r)
    {
        return x < r.x;
    }
};
typedef list <EdgeRec> EdgeList;

EdgeRec InitEdgeRec(Point& v1, Point& v2)
{
    if (v1[1] > v2[1])
        swap(v1, v2);
    EdgeRec rec;
    rec.x = v1[0];
    rec.ymax = v2[1];
    rec.minv = (double)(v2[0] - v1[0]) / (v2[1] - v1[1]);
    return rec;
}

void InitEdgeTable(Point* polygon, int n, EdgeList table[])
{
    Point v1 = polygon[n - 1];
    for (int i = 0; i < n; i++)
    {
        Point v2 = polygon[i];
        if (v1[1] == v2[1])
        {
            v1 = v2; continue;
        }
        EdgeRec rec = InitEdgeRec(v1, v2);
        int temp = v1[1];
        table[temp].push_back(rec);
        v1 = polygon[i];
    }
}

void generalPolygonFillAlgo(HDC hdc, Point* polygon, int n, COLORREF c)
{
    EdgeList* table = new EdgeList[600];
    InitEdgeTable(polygon, n, table);
    int y = 0;
    while (y < 600 && table[y].size() == 0)
        y++;
    if (y == 600)
        return;
    EdgeList ActiveList = table[y];
    while (ActiveList.size() > 0)
    {
        ActiveList.sort();
        for (EdgeList::iterator it = ActiveList.begin(); it != ActiveList.end(); it++)
        {
            int x1 = (int)ceil(it->x);
            it++;
            int x2 = (int)floor(it->x);
            for (int x = x1; x <= x2; x++) {
                SetPixel(hdc, x, y, c);
                Point p;
                p[0] = x;
                p[1] = y;
                points.push_back(p);
                colors.push_back(c);
            }
        }
        y++;
        EdgeList::iterator it = ActiveList.begin();
        while (it != ActiveList.end())
            if (y == it->ymax) it = ActiveList.erase(it);
            else it++;
        for (EdgeList::iterator it = ActiveList.begin(); it != ActiveList.end(); it++)
            it->x += it->minv;
        ActiveList.insert(ActiveList.end(), table[y].begin(), table[y].end());
    }
    delete[] table;
}


/*  This function is called by the Windows function DispatchMessage()  */
int x_1, x_2, y_1, y_2;
int xc, yc;
int a, b, r;
int algorithm = -1;
int check = 0;
bool drawedsquare = false;
Point polygonPoints[4];
COLORREF color = RGB(0, 0, 0);
static Point p[8];
static int index = 0;
ofstream saveToFile;
ifstream loadFromFile;
int R, G, B;
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;

    switch (message)
    {

    case WM_CREATE:
        AddMenu(hwnd);
        break;

    case WM_COMMAND:

        switch (LOWORD(wParam))
        {
        case 11:
            InvalidateRect(hwnd, NULL, true);
            cout << "Screen has been cleared successfully. \n \n";
            break;

        case 12:
        {
            cout << "NOW You are Saving the shapes in your PC" << endl;

            //if the file text is not exist it will creat the file
            saveToFile.open("data.txt", ios::out | ios::trunc);

            // saving the data into the file
            for (int i = 0; i < points.size(); ++i)
            {
                saveToFile << points[i][0] << " " << points[i][1] << " ";
                get_RGB_values(colors[i], R, G, B);
                saveToFile << R << " ";
                saveToFile << G << " ";
                saveToFile << B << '\n';

            }
            saveToFile.close();
            cout << "The data has bees saved successfuly :D " << endl;
            break;
        }

        case 13:
        {
            cout << "NOW You are loading the data from your PC " << endl;

            loadFromFile.open("data.txt", ios::in);

            int i = 0;
            while (!loadFromFile.eof())
            {
                int x, y;
                loadFromFile >> x >> y >> R >> G >> B;
                points.push_back(Point(x, y));
                colors.push_back(RGB(R, G, B));

                hdc = GetDC(hwnd);
                SetPixel(hdc, points[i][0], points[i][1], RGB(R, G, B));
                ReleaseDC(hwnd, hdc);
                i++;
            }
            loadFromFile.close();
            cout << "The Shapes has bees loaded successfuly :D" << endl;
            break;
        }

        case 21:
            color = RGB(255, 0, 0);
            cout << "The color has been changed into red. \n\n";
            break;

        case 22:
            color = RGB(0, 255, 0);
            cout << "The color has been changed into green. \n\n";
            break;

        case 23:
            color = RGB(0, 0, 255);
            cout << "The color has been changed into blue. \n\n";
            break;

        case 24:
            color = RGB(255, 0, 255);
            cout << "The color has been changed into Fuchsia. \n\n";
            break;

        case 25:
            color = RGB(255, 255, 0);
            cout << "The color has been changed into Yellow. \n\n";
            break;

        case 26:
            color = RGB(0, 255, 255);
            cout << "The color has been changed into Cyan. \n\n";
            break;

        case 31:
            algorithm = DDA;
            break;

        case 32:
            algorithm = midPointLine;
            break;

        case 33:
            algorithm = parametric;
            break;

        case 41:
            algorithm = directCircle;
            break;

        case 42:
            algorithm = polarCircle;
            break;

        case 43:
            algorithm = iterativePolarCircle;
            break;

        case 44:
            algorithm = midPointCircle;
            break;

        case 45:
            algorithm = modifiedMidPoint;
            break;

        case 51:
            algorithm = fillCircleWithLineQ1;
            break;

        case 52:
            algorithm = fillCircleWithLineQ2;
            break;

        case 53:
            algorithm = fillCircleWithLineQ3;
            break;

        case 54:
            algorithm = fillCircleWithLineQ4;
            break;

        case 55:
            algorithm = fillCircleWithCircleQ1;
            break;

        case 56:
            algorithm = fillCircleWithCircleQ2;
            break;

        case 57:
            algorithm = fillCircleWithCircleQ3;
            break;

        case 58:
            algorithm = fillCircleWithCircleQ4;
            break;

        case 61:
            algorithm = directEllipse;
            break;

        case 62:
            algorithm = polarEllipse;
            break;

        case 63:
            algorithm = midPointEllipse;
            break;

        case 71:
            algorithm = cardinalSpline;
            break;

        case 72:
            algorithm = convexFill;
            break;

        case 73:
            algorithm = nonConvexFill;
            break;

        case 74:
            algorithm = recursiveFill;
            break;

        case 75:
            algorithm = nonRecursiveFill;
            break;

        case 80:
            algorithm = quad;
            break;

        case 81:
            algorithm = square;
            break;

        case 82:
            algorithm = rectangle;
            break;

        case 83:
            algorithm = squareFilled;
            break;

        case 84:
            algorithm = rectangleFilled;
            break;

        case 91:
            algorithm = rectangleClippingPoint;
            break;

        case 92:
            algorithm = rectangleClippingLine;
            break;

        case 93:
            algorithm = rectangleClippingPolygon;
            break;

        case 101:
            algorithm = squareClippingPoint;
            break;

        case 102:
            algorithm = squareClippingLine;
            break;

        case 111:
            algorithm = circleClippingPoint;
            break;

        case 112:
            algorithm = circleClippingLine;
            break;
        }
        break;

    case WM_LBUTTONDBLCLK:
        switch (algorithm)
        {
        case DDA:
        {
            if (check == 0)
            {
                cout << "You are now in the DDA Line Algorithm. \n \n";
                x_1 = LOWORD(lParam);
                y_1 = HIWORD(lParam);
                check = 1;
            }
            else
            {
                x_2 = LOWORD(lParam);
                y_2 = HIWORD(lParam);
                hdc = GetDC(hwnd);
                drawLineDDA(hdc, x_1, y_1, x_2, y_2, color);
                cout << "The Line has been drawn successfully (using DDA line algorithm).\n \n";
                check = 0;
                ReleaseDC(hwnd, hdc);
            }
            break;
        }

        case parametric:
        {
            if (check == 0)
            {
                cout << "You are now in the parametric Line Algorithm. \n \n";
                x_1 = LOWORD(lParam);
                y_1 = HIWORD(lParam);
                check = 1;
            }
            else
            {
                x_2 = LOWORD(lParam);
                y_2 = HIWORD(lParam);
                hdc = GetDC(hwnd);
                drawLineParametric(hdc, x_1, y_1, x_2, y_2, color);
                cout << "The Line has been drawn successfully (using parametric line algorithm).\n \n";
                check = 0;
                ReleaseDC(hwnd, hdc);
            }
            break;
        }

        case midPointLine:
        {
            if (check == 0)
            {
                cout << "You are now in the midPoint Line Algorithm.\n \n";
                x_1 = LOWORD(lParam);
                y_1 = HIWORD(lParam);
                check = 1;
            }
            else
            {
                x_2 = LOWORD(lParam);
                y_2 = HIWORD(lParam);
                hdc = GetDC(hwnd);
                drawLineMidPoint(hdc, x_1, y_1, x_2, y_2, color);
                cout << "The Line has been drawn successfully (using midPointLine algorithm).\n \n";
                check = 0;
                ReleaseDC(hwnd, hdc);
            }
            break;
        }

        case directCircle:
        {
            if (check == 0)
            {
                cout << "You are now in the direct circle Algorithm. \n\n";
                xc = LOWORD(lParam);
                yc = HIWORD(lParam);
                check = 1;
            }
            else
            {
                x_1 = LOWORD(lParam);
                y_1 = HIWORD(lParam);
                r = (int)sqrt(((x_1 - xc) * (x_1 - xc)) + ((y_1 - yc) * (y_1 - yc)));
                hdc = GetDC(hwnd);
                drawCircleDirect(hdc, xc, yc, r, color);
                cout << "The circle has been drawn successfully (using direct algorithm). \n \n";
                check = 0;
                ReleaseDC(hwnd, hdc);

            }
            break;
        }

        case polarCircle:
        {
            if (check == 0)
            {
                cout << "You are now in the Polar circle Algorithm. \n\n";
                xc = LOWORD(lParam);
                yc = HIWORD(lParam);
                check = 1;
            }
            else
            {
                x_1 = LOWORD(lParam);
                y_1 = HIWORD(lParam);
                r = (int)sqrt(((x_1 - xc) * (x_1 - xc)) + ((y_1 - yc) * (y_1 - yc)));
                hdc = GetDC(hwnd);
                drawCirclePolar(hdc, xc, yc, r, color);
                cout << "The circle has been drawn successfully (using Polar algorithm).\n\n";
                check = 0;
                ReleaseDC(hwnd, hdc);

            }
            break;
        }

        case iterativePolarCircle:
        {
            if (check == 0)
            {
                cout << "You are now in the iterative Polar circle Algorithm. \n \n";
                xc = LOWORD(lParam);
                yc = HIWORD(lParam);
                check = 1;
            }
            else
            {
                x_1 = LOWORD(lParam);
                y_1 = HIWORD(lParam);
                r = (int)sqrt(((x_1 - xc) * (x_1 - xc)) + ((y_1 - yc) * (y_1 - yc)));
                hdc = GetDC(hwnd);
                drawCirclePolarIterative(hdc, xc, yc, r, color);
                cout << "The circle has been drawn successfully (using iterative Polar algorithm). \n\n";
                check = 0;
                ReleaseDC(hwnd, hdc);

            }
            break;
        }

        case midPointCircle:
        {
            if (check == 0)
            {
                cout << "You are now in the MidPoint circle Algorithm. \n\n";
                xc = LOWORD(lParam);
                yc = HIWORD(lParam);
                check = 1;
            }
            else
            {
                x_1 = LOWORD(lParam);
                y_1 = HIWORD(lParam);
                r = (int)sqrt(((x_1 - xc) * (x_1 - xc)) + ((y_1 - yc) * (y_1 - yc)));
                hdc = GetDC(hwnd);
                drawCircleMidPoint(hdc, xc, yc, r, color);
                cout << "The circle has been drawn successfully (using MidPoint algorithm).\n\n";
                check = 0;
                ReleaseDC(hwnd, hdc);
            }
            break;
        }

        case modifiedMidPoint:
        {
            if (check == 0)
            {
                cout << "You are now in the modifiedMidPoint circle Algorithm. \n \n";
                xc = LOWORD(lParam);
                yc = HIWORD(lParam);
                check = 1;
            }
            else
            {
                x_1 = LOWORD(lParam);
                y_1 = HIWORD(lParam);
                r = (int)sqrt(((x_1 - xc) * (x_1 - xc)) + ((y_1 - yc) * (y_1 - yc)));
                hdc = GetDC(hwnd);
                drawCircleMidPoint(hdc, xc, yc, r, color);
                cout << "The circle has been drawn successfully (using modifiedMidPoint algorithm). \n\n";
                check = 0;
                ReleaseDC(hwnd, hdc);

            }
            break;
        }

        case directEllipse:
        {
            p[index] = Point(LOWORD(lParam), HIWORD(lParam));
            if (index == 2)
            {
                cout << "Press Point number " << index + 1 << endl;
                hdc = GetDC(hwnd);
                //first point for center, second point for b, third point for a
                a = (int)sqrt(((p[0][0] - p[2][0]) * (p[0][0] - p[2][0])) + ((p[0][1] - p[2][1]) * (p[0][1] - p[2][1])));
                b = (int)sqrt(((p[0][0] - p[1][0]) * (p[0][0] - p[1][0])) + ((p[0][1] - p[1][1]) * (p[0][1] - p[1][1])));
                drawDirectEllipse(hdc, p[0][0], p[0][1], a, b, color);
                cout << "The Ellipse has been drawn successfully (using direct algorithm). \n\n";
                ReleaseDC(hwnd, hdc);
                index = 0;
            }
            else
            {
                if (index == 0)
                    cout << "Press the Points in these direction: \n(center -> (UP or DOWN) -> (Right or Left) )\n" << endl;

                index++;
                cout << "Press Point number " << index << endl;
            }
            break;
        }

        case polarEllipse:
        {
            p[index] = Point(LOWORD(lParam), HIWORD(lParam));
            if (index == 2)
            {
                cout << "Press Point number " << index + 1 << endl;
                hdc = GetDC(hwnd);
                //first point for center, second point for b, third point for a
                a = (int)sqrt(((p[0][0] - p[2][0]) * (p[0][0] - p[2][0])) + ((p[0][1] - p[2][1]) * (p[0][1] - p[2][1])));
                b = (int)sqrt(((p[0][0] - p[1][0]) * (p[0][0] - p[1][0])) + ((p[0][1] - p[1][1]) * (p[0][1] - p[1][1])));
                drawPolarEllipse(hdc, p[0][0], p[0][1], a, b, color);
                cout << "The Ellipse has been drawn successfully (using polar algorithm). \n \n";
                ReleaseDC(hwnd, hdc);
                index = 0;
            }
            else
            {
                if (index == 0)
                    cout << "Press the Points in these direction: \n(center -> (UP or DOWN) -> (Right or Left) )\n" << endl;

                index++;
                cout << "Press Point number " << index << endl;
            }
            break;
        }

        case midPointEllipse:
        {
            p[index] = Point(LOWORD(lParam), HIWORD(lParam));
            if (index == 2)
            {
                cout << "Press Point number " << index + 1 << endl;
                hdc = GetDC(hwnd);
                //first point for center, second point for b, third point for a
                a = (int)sqrt(((p[0][0] - p[2][0]) * (p[0][0] - p[2][0])) + ((p[0][1] - p[2][1]) * (p[0][1] - p[2][1])));
                b = (int)sqrt(((p[0][0] - p[1][0]) * (p[0][0] - p[1][0])) + ((p[0][1] - p[1][1]) * (p[0][1] - p[1][1])));
                drawEllipseMidPoint(hdc, p[0][0], p[0][1], a, b, color);
                cout << "The Ellipse has been drawn successfully (using polar algorithm). \n \n";
                ReleaseDC(hwnd, hdc);
                index = 0;
            }
            else
            {
                if (index == 0)
                    cout << "Press the Points in these direction: \n(center -> (UP or DOWN) -> (Right or Left) )\n" << endl;

                index++;
                cout << "Press Point number " << index << endl;
            }
            break;
        }

        case fillCircleWithLineQ1:
        {
            if (check == 0)
            {
                cout << "You are now in the filled quarter circle Algorithm. \n \n";
                xc = LOWORD(lParam);
                yc = HIWORD(lParam);
                check = 1;
            }
            else
            {
                x_1 = LOWORD(lParam);
                y_1 = HIWORD(lParam);
                r = (int)sqrt(((x_1 - xc) * (x_1 - xc)) + ((y_1 - yc) * (y_1 - yc)));
                hdc = GetDC(hwnd);
                drawQuarterFilledCircle(hdc, xc, yc, r, color, 1);
                cout << "The circle has been drawn successfully (with filling the first quarter with lines). \n\n";
                check = 0;
                ReleaseDC(hwnd, hdc);
            }
            break;
        }

        case fillCircleWithLineQ2:
        {
            if (check == 0)
            {
                cout << "You are now in the filled quarter circle Algorithm. \n \n";
                xc = LOWORD(lParam);
                yc = HIWORD(lParam);
                check = 1;
            }
            else
            {
                x_1 = LOWORD(lParam);
                y_1 = HIWORD(lParam);
                r = (int)sqrt(((x_1 - xc) * (x_1 - xc)) + ((y_1 - yc) * (y_1 - yc)));
                hdc = GetDC(hwnd);
                drawQuarterFilledCircle(hdc, xc, yc, r, color, 2);
                cout << "The circle has been drawn successfully (with filling the second quarter with lines). \n\n";
                check = 0;
                ReleaseDC(hwnd, hdc);
            }
            break;
        }

        case fillCircleWithLineQ3:
        {
            if (check == 0)
            {
                cout << "You are now in the filled quarter circle Algorithm. \n \n";
                xc = LOWORD(lParam);
                yc = HIWORD(lParam);
                check = 1;
            }
            else
            {
                x_1 = LOWORD(lParam);
                y_1 = HIWORD(lParam);
                r = (int)sqrt(((x_1 - xc) * (x_1 - xc)) + ((y_1 - yc) * (y_1 - yc)));
                hdc = GetDC(hwnd);
                drawQuarterFilledCircle(hdc, xc, yc, r, color, 3);
                cout << "The circle has been drawn successfully (with filling the third quarter with lines). \n\n";
                check = 0;
                ReleaseDC(hwnd, hdc);
            }
            break;

        }

        case fillCircleWithLineQ4:
        {
            if (check == 0)
            {
                cout << "You are now in the filled quarter circle Algorithm. \n \n";
                xc = LOWORD(lParam);
                yc = HIWORD(lParam);
                check = 1;
            }
            else
            {
                x_1 = LOWORD(lParam);
                y_1 = HIWORD(lParam);
                r = (int)sqrt(((x_1 - xc) * (x_1 - xc)) + ((y_1 - yc) * (y_1 - yc)));
                hdc = GetDC(hwnd);
                drawQuarterFilledCircle(hdc, xc, yc, r, color, 4);
                cout << "The circle has been drawn successfully (with filling the forth quarter with lines). \n\n";
                check = 0;
                ReleaseDC(hwnd, hdc);
            }
            break;
        }

        case fillCircleWithCircleQ1:
        {
            if (check == 0)
            {
                cout << "You are now in the filled quarter circle Algorithm. \n \n";
                xc = LOWORD(lParam);
                yc = HIWORD(lParam);
                check = 1;
            }
            else
            {
                x_1 = LOWORD(lParam);
                y_1 = HIWORD(lParam);
                r = (int)sqrt(((x_1 - xc) * (x_1 - xc)) + ((y_1 - yc) * (y_1 - yc)));
                hdc = GetDC(hwnd);
                drawQuarterFilledCircleWithCircles(hdc, xc, yc, r, color, 1);
                cout << "The circle has been drawn successfully (with filling the first quarter with circles). \n\n";
                check = 0;
                ReleaseDC(hwnd, hdc);
            }
            break;
        }

        case fillCircleWithCircleQ2:
        {
            if (check == 0)
            {
                cout << "You are now in the filled quarter circle Algorithm. \n \n";
                xc = LOWORD(lParam);
                yc = HIWORD(lParam);
                check = 1;
            }
            else
            {
                x_1 = LOWORD(lParam);
                y_1 = HIWORD(lParam);
                r = (int)sqrt(((x_1 - xc) * (x_1 - xc)) + ((y_1 - yc) * (y_1 - yc)));
                hdc = GetDC(hwnd);
                drawQuarterFilledCircleWithCircles(hdc, xc, yc, r, color, 2);
                cout << "The circle has been drawn successfully (with filling the second quarter with circles). \n\n";
                check = 0;
                ReleaseDC(hwnd, hdc);
            }
            break;
        }

        case fillCircleWithCircleQ3:
        {
            if (check == 0)
            {
                cout << "You are now in the filled quarter circle Algorithm. \n \n";
                xc = LOWORD(lParam);
                yc = HIWORD(lParam);
                check = 1;
            }
            else
            {
                x_1 = LOWORD(lParam);
                y_1 = HIWORD(lParam);
                r = (int)sqrt(((x_1 - xc) * (x_1 - xc)) + ((y_1 - yc) * (y_1 - yc)));
                hdc = GetDC(hwnd);
                drawQuarterFilledCircleWithCircles(hdc, xc, yc, r, color, 3);
                cout << "The circle has been drawn successfully (with filling the third quarter with circles). \n\n";
                check = 0;
                ReleaseDC(hwnd, hdc);
            }
            break;
        }

        case fillCircleWithCircleQ4:
        {
            if (check == 0)
            {
                cout << "You are now in the filled quarter circle Algorithm. \n \n";
                xc = LOWORD(lParam);
                yc = HIWORD(lParam);
                check = 1;
            }
            else
            {
                x_1 = LOWORD(lParam);
                y_1 = HIWORD(lParam);
                r = (int)sqrt(((x_1 - xc) * (x_1 - xc)) + ((y_1 - yc) * (y_1 - yc)));
                hdc = GetDC(hwnd);
                drawQuarterFilledCircleWithCircles(hdc, xc, yc, r, color, 4);
                cout << "The circle has been drawn successfully (with filling the forth quarter with circles). \n\n";
                check = 0;
                ReleaseDC(hwnd, hdc);
            }
            break;
        }

        case cardinalSpline:
        {
            p[index] = Point(LOWORD(lParam), HIWORD(lParam));
            if (index == 7) {
                hdc = GetDC(hwnd);
                cout << "Press Point number " << index + 1 << endl;
                cout << "The Cardinal Spline Curve with  " << index + 1 << " points has been drawn successfully " << endl;
                cardinalSplineCurve(hdc, p, 8, 2, color); //c=2 tension of the curve
                ReleaseDC(hwnd, hdc);
                index = 0;
            }
            else
            {
                index++;
                cout << "Press Point number " << index << endl;
            }
            break;
        }

        case quad:
        {

            polygonPoints[index] = Point(LOWORD(lParam), HIWORD(lParam));
            if (index == 3)
            {
                cout << "Press Point number " << index + 1 << endl;
                hdc = GetDC(hwnd);
                drawQuad(hdc, polygonPoints, color);
                drawedsquare = false;
                ReleaseDC(hwnd, hdc);
                index = 0;
            }
            else
            {
                index++;
                cout << "Press Point number " << index << endl;
            }
            break;
        }

        case square:
        {
            p[index] = Point(LOWORD(lParam), HIWORD(lParam));
            if (index == 1)
            {
                cout << "Press Point number " << index + 1 << endl;
                hdc = GetDC(hwnd);

                drawSquare(hdc, p[0][0], p[0][1], p[1][0], p[1][1], 0, color);
                cout << "The square has been drawn\n\n";
                drawedsquare = true;
                for (int i = 0; i < 4; i++) {
                    polygonPoints[i][0] = drawSquare(hdc, p[0][0], p[0][1], p[1][0], p[1][1], 0, color)[i][0];
                    polygonPoints[i][1] = drawSquare(hdc, p[0][0], p[0][1], p[1][0], p[1][1], 0, color)[i][1];

                }
                ReleaseDC(hwnd, hdc);
                index = 0;
            }
            else
            {
                index++;
                cout << "Press Point number " << index << endl;
            }
            break;
        }

        case rectangle:
        {
            p[index] = Point(LOWORD(lParam), HIWORD(lParam));
            if (index == 3)
            {
                cout << "Press Point number " << index + 1 << endl;
                hdc = GetDC(hwnd);
                drawRectangle(hdc, p[0][0], p[0][1], p[1][0], p[1][1], p[2][0], p[2][1], p[3][0], p[3][1], 0, color);
                cout << "The rectangle has been drawn\n\n";
                for (int i = 0; i < 4; i++) {
                    polygonPoints[i][0] = drawRectangle(hdc, p[0][0], p[0][1], p[1][0], p[1][1], p[2][0], p[2][1], p[3][0], p[3][1], 0, color)[i][0];
                    polygonPoints[i][1] = drawRectangle(hdc, p[0][0], p[0][1], p[1][0], p[1][1], p[2][0], p[2][1], p[3][0], p[3][1], 0, color)[i][1];

                }
                drawedsquare = false;
                ReleaseDC(hwnd, hdc);
                index = 0;
            }
            else
            {
                if (index == 0)
                    cout << "Press the Points in the clockwise direction: \n(TopLeft -> TopRight -> BottomRight -> BottomLeft)\n" << endl;

                index++;
                cout << "Press Point number " << index << endl;
            }
            break;
        }

        case squareFilled:
        {
            p[index] = Point(LOWORD(lParam), HIWORD(lParam));
            if (index == 1)
            {
                cout << "Press Point number " << index + 1 << endl;
                hdc = GetDC(hwnd);
                drawSquare(hdc, p[0][0], p[0][1], p[1][0], p[1][1], 1, color);
                cout << "The square is filled\n\n";
                ReleaseDC(hwnd, hdc);
                index = 0;
            }
            else
            {
                index++;
                cout << "Press Point number " << index << endl;
            }
            break;
        }

        case rectangleFilled:
        {
            p[index] = Point(LOWORD(lParam), HIWORD(lParam));
            if (index == 3)
            {
                cout << "Press Point number " << index + 1 << endl;
                hdc = GetDC(hwnd);
                drawRectangle(hdc, p[0][0], p[0][1], p[1][0], p[1][1], p[2][0], p[2][1], p[3][0], p[3][1], 1, color);
                cout << "The rectangle is filled\n\n";
                ReleaseDC(hwnd, hdc);
                index = 0;
            }
            else
            {
                if (index == 0)
                    cout << "Press the Points in the clockwise direction: \n(TopLeft -> TopRight -> BottomRight -> BottomLeft)\n" << endl;

                index++;
                cout << "Press Point number " << index << endl;
            }
            break;
        }

        case rectangleClippingPoint:
        {
            x_1 = LOWORD(lParam);
            y_1 = HIWORD(lParam);
            hdc = GetDC(hwnd);
            //int xTLeft,int yTLeftو int yBLeft, int xTRight, int x, int y
            polygonClippingPoint(hdc, polygonPoints[0][0], polygonPoints[0][1], polygonPoints[3][1], polygonPoints[1][0], x_1, y_1, color);
            ReleaseDC(hwnd, hdc);

            break;
        }

        case rectangleClippingLine:
        {
            if (check == 0)
            {
                x_1 = LOWORD(lParam);
                y_1 = HIWORD(lParam);
                check = 1;
            }
            else
            {
                x_2 = LOWORD(lParam);
                y_2 = HIWORD(lParam);
                hdc = GetDC(hwnd);
                polygonClippingLine(hdc, polygonPoints[0][0], polygonPoints[0][1], polygonPoints[3][1], polygonPoints[1][0], x_1, y_1, x_2, y_2, color);
                check = 0;
                ReleaseDC(hwnd, hdc);
            }
            break;

        }

        case rectangleClippingPolygon:
        {
            p[index] = Point(LOWORD(lParam), HIWORD(lParam));
            if (index == 3) {
                hdc = GetDC(hwnd);
                cout << "Press Point number " << index + 1 << endl;
                rectangleClippingPolygonAlgo(hdc, polygonPoints[0][0], polygonPoints[0][1], polygonPoints[3][1], polygonPoints[1][0], p[0], p[1], p[2], p[3], color);
                ReleaseDC(hwnd, hdc);
                index = 0;
            }
            else
            {
                index++;
                cout << "Press Point number " << index << endl;
            }
            break;
            break;
        }

        case squareClippingPoint:
        {
            x_1 = LOWORD(lParam);
            y_1 = HIWORD(lParam);
            hdc = GetDC(hwnd);
            //int xTLeft,int yTLeftو int yBLeft, int xTRight, int x, int y
            polygonClippingPoint(hdc, polygonPoints[0][0], polygonPoints[0][1], polygonPoints[3][1], polygonPoints[1][0], x_1, y_1, color);
            ReleaseDC(hwnd, hdc);
            break;
        }

        case squareClippingLine:
        {
            if (check == 0)
            {

                x_1 = LOWORD(lParam);
                y_1 = HIWORD(lParam);
                check = 1;
            }
            else
            {
                x_2 = LOWORD(lParam);
                y_2 = HIWORD(lParam);
                hdc = GetDC(hwnd);
                polygonClippingLine(hdc, polygonPoints[0][0], polygonPoints[0][1], polygonPoints[3][1], polygonPoints[1][0], x_1, y_1, x_2, y_2, color);
                check = 0;
                ReleaseDC(hwnd, hdc);
            }

            break;
        }

        case circleClippingPoint:
        {
            x_1 = LOWORD(lParam);
            y_1 = HIWORD(lParam);
            hdc = GetDC(hwnd);
            circleClippingPointAlgo(hdc, xc, yc, x_1, y_1, r, color);
            cout << "CircleClippingPoint \n" << endl;
            ReleaseDC(hwnd, hdc);
            break;

        }

        case circleClippingLine:
        {
            hdc = GetDC(hwnd);
            if (index == 0)
            {
                x_1 = LOWORD(lParam);
                y_1 = HIWORD(lParam);
                index++;
            }
            else if (index == 1)
            {
                x_2 = LOWORD(lParam);
                y_2 = HIWORD(lParam);
                circleClippingLineAlgo(hdc, xc, yc, x_1, y_1, x_2, y_2, r, color);
                ReleaseDC(hwnd, hdc);
                index = 0;
                cout << "CircleClippingLine \n" << endl;
            }
            break;

        }

        case recursiveFill:
        {
            hdc = GetDC(hwnd);
            recursiveFloodFill(hdc, xc, yc, color, color);
            ReleaseDC(hwnd, hdc);
            break;
        }

        case nonRecursiveFill:
        {
            hdc = GetDC(hwnd);
            nonRecursiveFloodFill(hdc, xc, yc, color, color);
            ReleaseDC(hwnd, hdc);
            break;
        }

        case convexFill:
        {
            hdc = GetDC(hwnd);
            if (drawedsquare)
            {
                int tempx = polygonPoints[2][0];
                int tempy = polygonPoints[2][1];

                polygonPoints[2][0] = polygonPoints[3][0];
                polygonPoints[2][1] = polygonPoints[3][1];

                polygonPoints[3][0] = tempx;
                polygonPoints[3][1] = tempy;

            }
            convexFillAlgo(hdc, polygonPoints, 4, color);
            ReleaseDC(hwnd, hdc);
            break;
        }

        case nonConvexFill:
        {
            if (drawedsquare)
            {
                int tempx = polygonPoints[2][0];
                int tempy = polygonPoints[2][1];

                polygonPoints[2][0] = polygonPoints[3][0];
                polygonPoints[2][1] = polygonPoints[3][1];

                polygonPoints[3][0] = tempx;
                polygonPoints[3][1] = tempy;

            }

            hdc = GetDC(hwnd);
            generalPolygonFillAlgo(hdc, polygonPoints, 4, color);
            ReleaseDC(hwnd, hdc);
            break;
        }

        }//end of switch
        break;
    case WM_DESTROY:
        PostQuitMessage(0);       /* send a WM_QUIT to the message queue */
        break;
    default:                      /* for messages that we don't deal with */
        return DefWindowProc(hwnd, message, wParam, lParam);
    }

    return 0;
}
