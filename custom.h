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

int match_two_string(char *a,char *b)
{
    char a1[65];
    char b1[65];
    int i,j,m1,m2,m3,m4;
    for(i=0;a[i]!='\0';i++)
    {
        if(a[i]>='a' && a[i]<='z')
            a1[i]=a[i]-32;
        else
            a1[i]=a[i];
    }
    a1[i]='\0';
    for(j=0;b[j]!='\0';j++)
    {
        if(b[j]>='a' && b[j]<='z')
            b1[j]=b[j]-32;
        else
            b1[j]=b[j];
    }
    b1[j]='\0';
    m1=m2=m3=m4=0;
    for(i=0;a1[i]!='\0';i++)
    {
        m1++;
        if(a1[i]==b1[i])
            m2++;
    }
    for(j=0;b1[j]!='\0';j++)
    {
        m3++;
        if(a1[j]==b1[j])
            m4++;
    }
    if(m1==m2 && m3==m4)
        return 1;
    else
        return 0;
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
void drowbox_by_type(int x,int x1,int y,int y1,int typeH,int typeV)
{
    int i;
    for(i=x;i<=x1;i++)
    {
        gotoxy(i,y);
        printf("%c",typeH);
        gotoxy(i,y1);
        printf("%c",typeH);
    }
    for(i=y;i<=y1;i++)
    {
        gotoxy(x,i);
        printf("%c",typeV);
        gotoxy(x1,i);
        printf("%c",typeV);
    }
}
void Clear_buffer()
{
    while(kbhit())
    {
        getch();
    }
}
int diffrent_between_in_dates(int day1, int mon1, int year1,int day2, int mon2, int year2) //FUNCTION START DIFFRENCE IN TWO DATES
{
    int ref,dd1,dd2,i;
    int tday;
    ref = year1;
    if(year2<year1)
    ref = year2;
    dd1=0;
    dd1=dater(mon1);
    for(i=ref;i<year1;i++)
    {
        if(i%4==0)
        dd1+=1;
    }
    dd1=dd1+day1+(year1-ref)*365;
    dd2=0;
    for(i=ref;i<year2;i++)
    {
        if(i%4==0)
        dd2+=1;
    }
    dd2=dater(mon2)+dd2+day2+((year2-ref)*365);
    tday=abs(dd2-dd1);
    return tday;
}
int dater(x)
{
    int y=0;
    switch(x)
    {
        case 1: y=0; break;
        case 2: y=31; break;
        case 3: y=59; break;
        case 4: y=90; break;
        case 5: y=120;break;
        case 6: y=151; break;
        case 7: y=181; break;
        case 8: y=212; break;
        case 9: y=243; break;
        case 10:y=273; break;
        case 11:y=304; break;
        case 12:y=334; break;
        default: return 0;
        return 0;
    }
    return(y);
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
void blinking_msg(char *a,int y)
{
    int i,x;
    char b[230];
    for(i=0;i<strlen(a);i++)
        b[i]=32;
    b[i]='\0';
    x=(44-(strlen(a)/2));
    fflush(stdin);
    while(!kbhit())
    {
        if(!kbhit())
        {
            gotoxy(x,y);
            printf("%s",a);
        }
        if(!kbhit())
            delay(500);
            gotoxy(x,y);
            printf("%s",b);
        if(!kbhit())
            delay(500);
    }
    fflush(stdin);
}
void MSGinM(char *a,int y)
{
    gotoxy(55-(strlen(a)/2),y);
    printf("%s",a);
}
