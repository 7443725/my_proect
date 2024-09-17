#include <iostream>
#include <windows.h>
#include <conio.h>
#include <time.h>
using namespace std;
class Elochka 
{
    HANDLE hStdOut;
    COORD leftTop; 
    char c = 'x'; 
    char h = '|'; 
    int size = 1;
    
public:
    Elochka(HANDLE StdOut, COORD Pos):
        hStdOut(StdOut),
        leftTop(Pos)
    {
         draw(leftTop);
    }
    void draw(COORD posis);
    void drawline(COORD cursorPos, char ch);
    void drawlinepn(COORD cursorPos, char ch);
    
};

class Girlind
{
    HANDLE hStdOut;
    char cc = 'o';

public:
    Girlind(HANDLE StdOut) :
        hStdOut(StdOut)
    {
        SetConsoleTextAttribute(hStdOut, 0);
        lamp(cc);
    }
    void lamp(char cc,int curss = 1);
    void lampone();
    void lamptwo();
    void lampthree();
    void lampfour();
};

class Zvezda
{
    HANDLE hStdOut;
    char cc = 'o';
    COORD leftTop1 = { 55, 4 };
public:
    Zvezda(HANDLE StdOut) :
        hStdOut(StdOut)
    {
        drawz(leftTop1);
    }
    void drawz(COORD posis);
    void drawlinez(COORD cursorPos, char ch, char ch1);
   
};






int main()
{
    setlocale(LC_ALL, "Russian");
    SetConsoleTitle(L"Елочка");
    HANDLE whStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    cout << "Введите номер программы от 1 до 4";
    COORD cursorPos = { 55, 5 }; 
    Elochka rect(whStdOut, cursorPos);
    Zvezda lamp1(whStdOut);
    Girlind lamp(whStdOut);
    cursorPos.Y = 31;
    SetConsoleTextAttribute(whStdOut, BACKGROUND_RED | BACKGROUND_GREEN |BACKGROUND_BLUE);
    SetConsoleCursorPosition(whStdOut, cursorPos);
    cout << "\n Для выхода нажми ESC";
    DWORD cWrittenChars;
    WORD attr = BACKGROUND_BLUE |BACKGROUND_GREEN |BACKGROUND_RED;
    FillConsoleOutputAttribute(whStdOut, attr, 2000,{ 21,32 }, &cWrittenChars);
    int iKey = 67;
    while (iKey != 27)
    { 
        if (_kbhit())
        {
            iKey = _getch();
            switch (iKey)
            {
            case 49: lamp.lampone(); break;
            case 50: lamp.lamptwo(); break;
            case 51: lamp.lampthree();break;
            case 52: lamp.lampfour(); break;
            default: break;
            }
        }
    }
    system("CLS");
}

void Elochka::draw(COORD posis)
{
    SetConsoleTextAttribute(hStdOut, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    COORD pos = posis;
    for (int j = 0; j < 22; j++)
    {
        drawline(pos, c);
        pos.Y += 1;
    }
    SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    for (int j = 0; j < 5; j++)
    {
        drawlinepn(pos, h);
        pos.Y += 1;
    }
    cout << "\n\n";
}

void Elochka::drawline(COORD cursorPos,  char ch)
{
    for (int i = 0; i < size; i++) 
    {
        SetConsoleCursorPosition(hStdOut, cursorPos);
        cout << ch;
        cursorPos.X -= 1;
    }
    cursorPos.X += size;
    for (int i = 0; i < size; i++)
    {
        SetConsoleCursorPosition(hStdOut, cursorPos);
        cout << ch;
        cursorPos.X += 1;
    }
    size++;
}

void Elochka::drawlinepn(COORD cursorPos, char ch)
{
    for (int i = 0; i < 4; i++)
    {
        SetConsoleCursorPosition(hStdOut, cursorPos);
        cout << ch;
        cursorPos.X -= 1;
    }
    cursorPos.X += 5;
    for (int i = 0; i < 4; i++)
    {
        SetConsoleCursorPosition(hStdOut, cursorPos);
        cout << ch;
        cursorPos.X += 1;
    }
   
}

void Girlind::lamp(char cc,int curss)
{
    COORD cursorPos1;
    int curs = 0;
    do
    {
        switch (curs)
        {
        case 0: cursorPos1 = { 55, 6 }; break;
        case 1: cursorPos1 = { 53, 9 }; break;
        case 2: cursorPos1 = { 57, 8 }; break;
        case 3: cursorPos1 = { 51, 12 }; break;
        case 4: cursorPos1 = { 55, 11 }; break;
        case 5: cursorPos1 = { 59, 10 }; break;
        case 6: cursorPos1 = { 49, 15 }; break;
        case 7: cursorPos1 = { 53, 14 }; break;
        case 8: cursorPos1 = { 57, 13 }; break;
        case 9: cursorPos1 = { 61, 12 }; break;
        case 10: cursorPos1 = { 45, 16 }; break;
        case 11: cursorPos1 = { 47, 18 }; break;
        case 12: cursorPos1 = { 51, 17 }; break;
        case 13: cursorPos1 = { 55, 16 }; break;
        case 14: cursorPos1 = { 59, 15 }; break;
        case 15: cursorPos1 = { 63, 14 }; break;
        case 16: cursorPos1 = { 42, 19 }; break;
        case 17: cursorPos1 = { 42, 22 }; break;
        case 18: cursorPos1 = { 47, 21 }; break;
        case 19: cursorPos1 = { 52, 20 }; break;
        case 20: cursorPos1 = { 57, 19 }; break;
        case 21: cursorPos1 = { 62, 18 }; break;
        case 22: cursorPos1 = { 66, 17 }; break;
        case 23: cursorPos1 = { 40, 25 }; break;
        case 24: cursorPos1 = { 46, 24 }; break;
        case 25: cursorPos1 = { 52, 23 }; break;
        case 26: cursorPos1 = { 58, 22 }; break;
        case 27: cursorPos1 = { 63, 21 }; break;
        case 28: cursorPos1 = { 68, 20 }; break;
        case 29: cursorPos1 = { 58, 25 }; break;
        case 30: cursorPos1 = { 64, 24 }; break;
        case 31: cursorPos1 = { 70, 23 }; break;
        case 32: cursorPos1 = { 34, 26 }; break;
        case 33: cursorPos1 = { 39, 26 }; break;
        case 34: cursorPos1 = { 44, 26 }; break;
        case 35: cursorPos1 = { 49, 26 }; break;
        case 36: cursorPos1 = { 54, 26 }; break;
        case 37: cursorPos1 = { 59, 26 }; break;
        case 38: cursorPos1 = { 64, 26 }; break;
        case 39: cursorPos1 = { 69, 26 }; break;
        case 40: cursorPos1 = { 74, 26 }; break;
        }
        SetConsoleCursorPosition(hStdOut, cursorPos1);
        cout << cc;
        curs+=curss;
    } while (curs <= 41);
}

void Girlind::lampone()
{
        while (!(_kbhit()))
        {
            SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_INTENSITY);
            lamp(cc);
            Sleep(500);
            SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            lamp(cc);
            Sleep(500);
        }
        SetConsoleTextAttribute(hStdOut, 0);
        lamp(cc);
}

void Girlind::lamptwo()
{
    while (!(_kbhit()))
    {
        SetConsoleTextAttribute(hStdOut, 0);
        lamp(cc);
        Sleep(200);
        SetConsoleTextAttribute(hStdOut, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
        lamp(cc,3);
        Sleep(200);
        SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        lamp(cc,1);
        Sleep(200);
        SetConsoleTextAttribute(hStdOut, FOREGROUND_BLUE |  FOREGROUND_INTENSITY);
        lamp(cc, 1);
        Sleep(200);
    }
    SetConsoleTextAttribute(hStdOut, 0);
    lamp(cc);
}

void Girlind::lampthree()
{ 
    int per = 8;
    while (!(_kbhit()))
    {
        for (int c = 1; c < 5; c++,per++)
        {
            if (per >= 15)per = 8;
            SetConsoleTextAttribute(hStdOut, per);
            lamp(cc, c);
            Sleep(200);
            SetConsoleTextAttribute(hStdOut, 0);
            lamp(cc);
            Sleep(200);
        }
    }
}

void Girlind::lampfour()
{
    int per, slep, per1=1;
    srand((unsigned int)time(NULL));
    while (!(_kbhit()))
    {
        for (int c = 1; c < 5; c++)
        {
            per1 = rand() % 4 + 1;
            per = rand() % 8+8;
            slep = rand() % 300+200;
            SetConsoleTextAttribute(hStdOut, per);
            lamp(cc, per1);
            Sleep(slep);
            SetConsoleTextAttribute(hStdOut, 0);
            lamp(cc);
            Sleep(slep);
        }
    }
    
}

void Zvezda::drawz(COORD posis)
{
    SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_INTENSITY);
    COORD pos = posis;
    int ds = 1;
    for (int j = 0; j < 3; j++)
    {
        drawlinez(pos, cc, ds);
        pos.Y -= 1;
        ds++;
    }
}

void Zvezda::drawlinez(COORD cursorPos, char ch, char ch1 )
{
    int ch2 = 0;
    if (ch1 == 3)
    {
        ch1 = 1;
    }

    if (ch1 == 2)
    {
        ch1++;
        ch2 = 2;
        cursorPos.X -= 1;
    }
    
    for (int i = 0; i < ch1; i++)
    {
        SetConsoleCursorPosition(hStdOut, cursorPos);
        cout << ch;
        if (ch2 == 2) cursorPos.X += 1;
    }
}