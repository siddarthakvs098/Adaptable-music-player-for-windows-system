#define WINVER 0x0501 // WinXP and UP
#include<stdio.h>
#include<conio.h>
#include<windows.h>
#include<mmsystem.h>
#include<stdlib.h>
#include<time.h>
#include<dir.h>
#include<string.h>
void remove_console_buttons()
{
  LONG style;
  HWND ConsoleWindow;

  ConsoleWindow = GetConsoleWindow();

  style = GetWindowLong(ConsoleWindow, GWL_STYLE);
  style &= ~( WS_MINIMIZEBOX | WS_SYSMENU );
  SetWindowLongPtr(ConsoleWindow, GWL_STYLE, style);

  SetWindowPos(ConsoleWindow, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED |SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER);
  //Resize
     //system ("mode con cols=75 lines=50");
}
int detect_windows_drive(char *ch)
{
    char a='a';
    FILE *fp;
    char path[]="a:\\Windows\\System32\\control.exe";
    while(a<='z')
    {
        fp=fopen(path,"r");
        if(fp!=NULL)
        {
            *ch=a;
            fclose(fp);
            return 1;
        }
        a++;
        path[0]=a;
    }
    return 0;
}
void setcolor(int ForgC)
{
    WORD wColor;
    HANDLE hStdOut=GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if(GetConsoleScreenBufferInfo(hStdOut,&csbi))
    {
    wColor=(csbi.wAttributes & 0xF0)+(ForgC & 0x0F);
    // SetConsoleTextAttributes(hStdOut,wColor);
    SetConsoleTextAttribute(hStdOut,wColor);
    }
}

void make_frame(int x,int x1,int y,int y1)
{
    int i;
    for(i=x+1;i<=x1-1;i++)
    {
        gotoxy(i,y);
        printf("%c",196);
        gotoxy(i,y1);
        printf("%c",196);
    }
    for(i=y+1;i<=y1-1;i++)
    {
        gotoxy(x,i);
        printf("%c",179);
        gotoxy(x1,i);
        printf("%c",179);
    }
    gotoxy(x,y);
    printf("%c",218);
    gotoxy(x,y1);
    printf("%c",192);
    gotoxy(x1,y1);
    printf("%c",217);
    gotoxy(x1,y);
    printf("%c",191);
}
void rec_MSGinM(char *string,int x,int x1,int y)
{
    int H_lenth,middile_p;
    H_lenth=(strlen(string))/2;
    middile_p=x1-((x1-x)/2);
    gotoxy(middile_p-H_lenth+1,y);
    printf("%s",string);
}
void get_system_date(int *day,int *month,int *year)
{
    SYSTEMTIME st;
    GetSystemTime(&st);
    *day=st.wDay;
    *month=st.wMonth;
    *year=st.wYear;
}


void drowlineH(int x,int x1,int y,int type)
{
    int i;
    for(i=x;i<=x1;i++)
    {
        gotoxy(i,y);
        printf("%c",type);
    }
}
void drowlineV(int y,int y1,int x,int type)
{
    int i;
    for(i=y;i<=y1;i++)
    {
        gotoxy(x,i);
        printf("%c",type);
    }
}
void drowbox(int x,int x1,int y,int y1)
{
    int i;
    for(i=x;i<=x1;i++)
    {
        gotoxy(i,y);
        printf("%c",219);
        gotoxy(i,y1);
        printf("%c",219);
    }
    for(i=y;i<=y1;i++)
    {
        gotoxy(x,i);
        printf("%c",219);
        gotoxy(x1,i);
        printf("%c",219);
    }
}
struct SMALL_RECT {
    SHORT Left;
    SHORT Top;
    SHORT Right;
    SHORT Bottom;
};
void adjustWindowSize(int left,int top,int x,int y)
{
    struct SMALL_RECT test;

    HANDLE hStdout;
    COORD coord;
    BOOL ok;

    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

    coord.X = x;      //width 112 full size with same buffer size  or (111 width & 42 height with large buffer size;
    coord.Y = y;       //hieght 43
    //ok = SetConsoleScreenBufferSize(hStdout, coord);

    test.Left = left;	//0
    test.Top = top;		//0
    test.Right = coord.X-1;
    test.Bottom = coord.Y-1;

    SetConsoleWindowInfo(hStdout, ok, &test);

}
void Bk_color(int color,int text_color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),color);
    setcolor(text_color);
    /*  --------------------------------
         Color Code Of Background Color
        --------------------------------
            Black 			-- 	0
            Navy Blue  		-- 	28
            Green 			--	34
            Teal			--	49
            Maroon			--	68
            Purple			--	82
            Olive			--	100
            Silver, Grey	--	117
            Dark Grey		--	128
            Blue			--	147
            Light Green		--	162
            Cyan			--	180
            Red				--	197
            Magenta			--	210
            Yellow			--	225
            White			--	252
      */
}



void buffer_size(int x,int y)
{
    HANDLE wHnd;    // Handle to write to the console.
    wHnd=GetStdHandle(STD_OUTPUT_HANDLE);
    COORD bufferSize;
    bufferSize.X=x;
    bufferSize.Y=y;
    SetConsoleScreenBufferSize(wHnd, bufferSize);
}

int Hide_cursor()
{

	 HANDLE myconsole = GetStdHandle(STD_OUTPUT_HANDLE);

	 CONSOLE_CURSOR_INFO CURSOR;
	 BOOL result;
	 CURSOR.dwSize = 1;
	 CURSOR.bVisible = FALSE;
	 result=SetConsoleCursorInfo(myconsole, &CURSOR);
	 if(result){
	   return 1;
	 }
	 return 0;
}
void console_title(char *Title)
{
    SetConsoleTitle(Title);
}
int Show_cursor(int type)
{
	 //Get a console handle
	 HANDLE myconsole = GetStdHandle(STD_OUTPUT_HANDLE);
	/* It is already defined.
	 struct CONSOLE_CURSOR_INFO
	 {
	  DWORD dwSize;
	  BOOL bVisible;
	 };
	*/
	 //CONSOLE_CURSOR_INFO is defined type.
	 CONSOLE_CURSOR_INFO CURSOR;
	 BOOL result;
	 CURSOR.dwSize = type;
	 CURSOR.bVisible = TRUE;
	 result=SetConsoleCursorInfo(myconsole, &CURSOR);//second argument need pointer
	 if(result){//success
	   return 1;
	 }
	 return 0;
}
void clear_frame(int x,int x1,int y,int y1)
{
    int i;
    for(i=x;i<=x1;i++)
    {
        gotoxy(i,y);
        printf("%c",256);
        gotoxy(i,y1);
        printf("%c",256);
    }
    for(i=y;i<=y1;i++)
    {
        gotoxy(x,i);
        printf("%c",256);
        gotoxy(x1,i);
        printf("%c",256);
    }
}

void Clear_buffer()
{
    while(kbhit())
    {
        getch();
    }
}

void gotoxy(int x,int y)
{

    COORD c;
    c.X=x;
    c.Y=y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),c);
}
void delay(unsigned int msecond)
{
    clock_t goal=msecond+clock();
    while(goal>clock());
}

void MSGinM(char *a,int y)
{
    gotoxy(55-(strlen(a)/2),y);
    printf("%s",a);
}
