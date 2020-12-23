#include "custom.h"
#include<pthread.h>
#include<stdio.h>
#include<windows.h>
#include<dos.h>
#include<dirent.h>
#include<mciavi.h>
#pragma comment(lib, "Winmm.lib")
#define up 72
#define down 80
#define left 75
#define right 77
int page=1;
char Drives[50];
pthread_t ptimer;
char current_drive[20],current_playing_song[50];
int current_drive_music_count;
int System_Drive_Count;
volatile flag;
int play=0,cord_changed=0;
char music_lib[] = "music_library.lib";

struct node
{
    char data[1000];
    char music_file[1000];
    struct node *next;
    struct node *prev;
}*head=NULL,*tail=NULL;
struct music_library
{
    char data[1000];
    char music_file[1000];
    int day,month ,year;
    struct music_library *next;
    struct music_library *prev;
}*lhead=NULL,*ltail=NULL;
void back_freame(int x,int x1,int y, int y1)
{
    int i;
    for(i=x;i<=x1;i++)
    {
        gotoxy(i,y);
        printf("%c",220);
        gotoxy(i,y+1);
        printf("%c",219);
        gotoxy(i,y1);
        printf("%c",223);
    }
}
void ind_back_freame(int x,int x1,int y, int y1,int bottomLineshadow_color,int topLineShadow_color,int line_color)
{
    int i;
    for(i=x;i<=x1;i++)
    {
        Bk_color(topLineShadow_color,line_color);
        gotoxy(i,y);
        printf("%c",220);
        gotoxy(i,y+1);
        printf("%c",219);
        Bk_color(bottomLineshadow_color,line_color);
        gotoxy(i,y1);
        printf("%c",223);
    }
}
int is_music_file_already_exist(char *music_file)
{
    FILE *fp;
    struct music_library temp;
    fp=fopen(music_lib,"rb");
    if(fp==NULL)
    {
        fclose(fp);
        return 0;
    }
    while(fread(&temp,sizeof(struct music_library),1,fp)>0)
    {
        if(strcmp(music_file,temp.music_file)==0)
        {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}
void display_library()
{
    int n;
    struct music_library temp1,*temp;
    FILE *fp;
    lhead=NULL;
    ltail=NULL;
    fp = fopen(music_lib,"rb");
    if(fp == NULL )
        MessageBoxA(NULL,"Error","Error",16);
    while(n=fread(&temp1, sizeof(struct music_library), 1, fp)>0)
        {
            insert_library(temp1.data,temp1.music_file,temp1.day,temp1.day,temp1.year);
            printf("%s\n",temp1.music_file);
            printf("%d/%d/%d\n",temp1.day,temp1.month,temp1.year);
        }
    fclose(fp);
}
char* reprint_music_file(int n)
{
    struct node *temp;
    temp = head;
    for(int i=0;i<n;i++)
    {
        temp = temp->next;
    }
    return temp->music_file;
}
void main_menu_background()
{
    for(int i=6;i<=39;i++)
    {
        gotoxy(2,i);
        printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219);

    }
}
void working_area_background()
{
    for(int i=6;i<=39;i++)
   {    gotoxy(40,i);
        printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219);
   }
}
void Notification()
{

    Bk_color(197,15);
    gotoxy(70,4);
    printf("NOTIFICATIONS");
    Bk_color(117,1);
    gotoxy(42,7);
    printf("Library Information");
    notification_strip(42,8);
    gotoxy(44,10);
    printf("%c Total Music In Libary           : %d",249, total_music_in_library());
}
void menu_option(int x,int x1,int y,int y1)
{
    gotoxy(x,y);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220);
    gotoxy(x,y+1);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219);
    gotoxy(x,y1);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",223,223,223,223,223,223,223,223,223,223,223,223,223,223,223,223,223,223,223,223,223,223,223,223,223,223,223,223,223,223,223,223,223,223,223);
}
void song_selector(int x1,int x2,int y1,int y2)
{

    gotoxy(x1,y1);
    for(int i=44;i<95;i++)
    {
        printf("%c",220);
    }
    gotoxy(x1,y1+1);
    for(int i=44;i<95;i++)
    {
        printf("%c",219);
    }
    gotoxy(x1,y2);
    for(int i=44;i<95;i++)
    {
        printf("%c",223);
    }


}

void Update_status_bar(int x,int y)
{
    Bk_color(28,1);
    gotoxy(x,y);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219);
}

void notification_strip(int x,int y)
{
    gotoxy(x,y);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196);
}

int detect_all_the_drives(char *drives,int *drive_count)
{
    int i=0;
    FILE *fp;
    char temp,drive[]="a:\\drives_detection.txt";
    temp='A';

    while(temp <= 'Z')
    {
        drive[0]=temp;
        fp = fopen(drive,"wb");
        if(fp != NULL)
        {
            if(temp != 'C')
            {
                drives[i]=temp;
                fclose(fp);
                remove(drive);
                i++;
            }

        }
        temp++;
    }
    drives[i]='\0';
    if(i==1)
    {
        return 0;
    }
    else
    {
         *drive_count=i;
        return 1;
    }
}
void add_music_to_library(char *data,char *music_file)
{
    struct music_library temp;
    get_system_date(&temp.day,&temp.month,&temp.year);
    strcpy(temp.data,data);
    strcpy(temp.music_file,music_file);
    FILE *fp;
    fp = fopen(music_lib,"ab");
    if(fp == NULL)
    {
    MessageBoxA(NULL,"Music player encounterd a problem","Eror",16);
    }
    else
    {
        fwrite(&temp,sizeof(struct music_library),1,fp);
        MessageBoxA(NULL,"Music file is successfully added into the library","Music files added",16);
    }
    fclose(fp);
}
void insert_library(char *data,char *music_file,int day,int month,int year)
{
    struct music_library *new,*temp;
    new = (struct music_library*)malloc(sizeof(struct music_library));
	strcpy(new->data,data);
	strcpy(new->music_file,music_file);
	new->day = day;
	new->month = month;
	new->year=year;
	new ->next = lhead;
	new ->prev = NULL;
	if(lhead == NULL)
	{
		lhead = new;
		ltail = new;

	}
	else
	{
		ltail->next = new;
		new->prev = ltail;
		ltail = new;

	}
}
int total_music_in_library()
{
    int count = 0;
    FILE *fp;
    fp = fopen(music_lib, "rb");
    struct music_library data;
    if(fp == NULL)
    {
        return 0;
    }
    else
    {
        while(fread(&data, sizeof(struct music_library), 1, fp)>0)
        {
            count++;
        }
    }
    return count;
}
void m_library()
{
    int n,j=0,i=7,a1=44,a2=95,b1=6,b2,x;
    struct music_library temp1;
    struct music_library *temp,*temp2;
    FILE *fp;
    restart:
    i=7;
    j=0;
    a1 =44;
    a2 = 95;
    b1=6;
    lhead=NULL;
    ltail=NULL;
    fp = fopen(music_lib,"rb");
    Bk_color(117,1);
    if(fp == NULL )
    {
        gotoxy(50,25);
        printf("No music files");
    }
    else
   {
       temp = lhead;
        while(n=fread(&temp1, sizeof(struct music_library), 1, fp)>0)
        {
           insert_library(temp1.data,temp1.music_file,temp1.day,temp1.day,temp1.year);
            gotoxy(44,i);

            printf("%s\n",temp1.music_file);

            gotoxy(98,i);
            printf("%d/%d/%d\n",temp1.day,temp1.month,temp1.year);

            i=i+2;
        }
    if(n==0 && i==7)
    {
        gotoxy(50,15);
        printf("No music files");
    }
   }
    fclose(fp);
    temp = lhead;
    temp2 = temp;
    x = 7;
    redo:

    if(total_music_in_library()>0)
    {
        b2 =b1+2;
        setcolor(14);
        song_selector(a1,a2,b1,b2);
        Bk_color(225,9);
        gotoxy(a1,b1+1);
        printf("%s",temp->music_file);
    }

        char ch = getch();
        if(ch == -32)
            {
                ch = getch();
                if(ch == up || ch == down || ch == left || ch == right)
                {

                    if(ch == up)
                    {
                        if(b1>6 && b1< 6+(total_music_in_library()*2))
                        {
                            cord_changed=1;
                            Bk_color(117,7);
                            song_selector(a1,a2,b1,b2);
                            gotoxy(a1,b1+1);
                            Bk_color(117,1);
                            printf("%s",temp->music_file);
                            temp = temp->prev;
                            b1=b1-2;
                            b2 = b1+2;
                            goto redo;
                        }
                        else
                            {
                                goto redo;
                            }



                    }
                    else if(ch == down)
                    {
                          if(b1>=6 && b1<6+((total_music_in_library()-1)*2))
                          {
                              cord_changed=1;
                              Bk_color(117,7);
                              song_selector(a1,a2,b1,b2);
                              gotoxy(a1,b1+1);
                              Bk_color(117,1);
                              printf("%s",temp->music_file);
                              temp = temp->next;
                              b1 = b1+2;
                              b2 = b1+2;
                              goto redo;
                          }
                          else if (b1 == 6+((total_music_in_library()-1)*2) )
                          {
                              cord_changed =1;
                              Bk_color(117,7);
                              song_selector(a1,a2,b1,b2);
                              gotoxy(a1,b1+1);
                              gotoxy(a1,b1+1);
                              Bk_color(117,1);
                              printf("%s",temp->music_file);
                              b1 =6;
                              temp = lhead;
                              goto redo;
                          }
                    }
                    else if(ch == right)
                    {

                        gotoxy(a1,x);
                        Bk_color(117,1);
                        printf("%s",temp2->music_file);
                        temp2 = temp2->next;
                        x=x+2;
                        if(x == 7+(total_music_in_library()*2))
                            x=7;
                        gotoxy(a1,x);
                        Bk_color(117,12);
                        printf("%s",temp2->music_file);
                        close_musicfile();

                        if(temp2 != NULL)
                        {
                            int re = play_music(temp2->data,0);
                            if(re ==0)
                            {
                                MessageBoxA(NULL,"Unsupported mp3 file","Error",16);
                            }
                            play =1;
                            goto redo;


                        }
                        else if (temp2 == lhead)
                        {

                             goto redo;
                        }
                    }
                    else if(ch == left)
                    {
                        gotoxy(a1,x);
                        Bk_color(117,1);
                        printf("%s",temp2->music_file);
                        temp2 = temp2->prev;
                        x=x-2;
                        gotoxy(a1,x);
                        Bk_color(117,12);
                        printf("%s",temp2->music_file);
                        close_musicfile();
                        if(temp2 != NULL && (x>7 || x<=7+(total_music_in_library()*2)))
                        {
                            int re = play_music(temp2->data,0);
                            if(re ==0)
                            {
                                MessageBoxA(NULL,"Unsupported mp3 file","Error",16);
                            }
                            play=1;
                            goto redo;

                        }

                    }
                    else
                        goto redo;

                }
                 else if(ch == 83)
                    {

                        int k=total_music_in_library();
                          if(strcmp(current_playing_song,temp->music_file)==0)
                        {
                            close_musicfile();
                        }
                        if(k==1)
                        {
                            Bk_color(180,7);
                            working_area_background();
                            Bk_color(117,1);
                            drowlineV(6,38,96,179);
                            lhead = NULL;
                            ltail=NULL;
                            fp=fopen(music_lib,"wb");
                            fclose(fp);
                            goto restart;
                        }
                           if(strcmp(current_playing_song,temp->music_file)==0)
                        {
                            close_musicfile();
                        }
                        fp = fopen("music_library.lib","wb");
                        struct music_library *temper;
                        if(b1 ==6)
                        {
                            lhead = lhead->next;
                            lhead->prev = NULL;
                        }
                        else
                        {
                            temp->prev->next=temp->next;
                            temp->next->prev=temp->prev;
                        }

                        temper = lhead;
                        if(fp != NULL)
                        for(i=0;i<k;i++)
                        {
                            if(i == (b1-6)/2)
                                {
                                    continue;
                                }
                            else
                                {
                                    fwrite(temper,sizeof(struct music_library),1,fp);
                                }
                            temper = temper->next;
                        }
                        fclose(fp);

                        MessageBoxA(NULL,"Music file deleted successfully","Notification",16);
                        Bk_color(180,7);
                        working_area_background();
                        Bk_color(117,1);
                        drowlineV(6,38,96,179);
                        lhead = NULL;
                        ltail=NULL;
                        goto restart;
                    }
                else
                    goto redo;
            }
            else if (ch == 27)
                {
                    Bk_color(180,7);
                    working_area_background();
                    ind_back_freame(40,109,3,5,117,180,12);
                    Bk_color(197,15);
                    gotoxy(70,4);
                    printf("Notification");
                    Notification();
                    return;

                }
            else if(ch == 13)
                {
                    cord_changed = 0;
                    close_musicfile();
                    strcpy(current_playing_song,temp->music_file);
                    int re2 = play_music(temp->data,0);
                    if(re2 ==0)
                        {
                            MessageBoxA(NULL,"Unsupported mp3 file","Error",16);
                            goto redo;
                        }
                    play =1;
                    ind_back_freame(1,110,40,42,28,180,1);
                    gotoxy(2,41);
                    Bk_color(28,15);
                    printf("playing : %s",temp->music_file);
                    Bk_color(117,1);
                    goto redo;
                }
            else if(ch == 'p')
                {
                    if(cord_changed ==0 &&play == 1)
                    {
                        play =0;
                        pause_music();

                        ind_back_freame(1,110,40,42,28,180,1);
                        gotoxy(2,41);
                        Bk_color(28,15);
                        printf("pause : %s",temp->music_file);
                        Bk_color(117,1);
                    }
                    else if(cord_changed == 0 && play==0)
                    {
                        play=1;
                        int re = play_music(temp->data,0);

                        ind_back_freame(1,110,40,42,28,180,1);
                        gotoxy(2,41);
                        Bk_color(28,15);
                        printf("playing : %s",temp->music_file);
                        strcpy(current_playing_song,temp->music_file);
                        Bk_color(117,1);
                        if(re == 0 && cord_changed==1)
                        {
                            MessageBoxA(NULL," Unsupportable mp3 files","Error",64);
                        }
                    }
                    else if(cord_changed==1)
                    {
                        cord_changed = 0;
                        play=1;
                        close_musicfile();
                        ind_back_freame(1,110,40,42,28,180,1);
                        gotoxy(2,41);
                        Bk_color(28,15);
                        printf("playing : %s",temp->music_file);
                        Bk_color(117,1);
                        int re=play_music(temp->data,0);
                        if(re == 0)
                        {
                            MessageBoxA(NULL," Unsupportable mp3 files","Error",64);
                        }
                    }
                    goto redo;
                }
            else
                goto redo;

}
void timer(int n)
{
    int count=0;

    flag = 0;
    for(int i=0;count != n;i++)
    {
      //  gotoxy(20,25);
      //  printf("%d",i);
        delay(1000);
        count++;
    }
  exit(0);
}
int main_menu()
{
    int i,x1,y1,x2,y2,n,point,pointx;
    char ch;

    system("cls");
    system("COLOR BC");
    console_title("Github link:https://github.com/siddarthakvs098");
    Hide_cursor();

    adjustWindowSize(0,0,111,42);
    Bk_color(28,1);
    back_freame(0,111,0,2);
    Bk_color(28,15);
    MSGinM("Adaptable Music Player For Windows System",2);

    Bk_color(180,17);
    drowlineH(0,111,42,219);
    Bk_color(117,17);
    drowlineV(3,42,1,219);
    drowlineV(3,42,111,219);

    ind_back_freame(1,110,40,42,28,180,1);
    ind_back_freame(2,38,3,5,117,180,12);
    ind_back_freame(40,109,3,5,117,180,12);

    adjustWindowSize(0,0,112,43);
    Bk_color(180,7);
    main_menu_background();
    working_area_background();
    buffer_size(116,45);
    gotoxy(2,41);
    Bk_color(28,15);
    printf("STATUS");
    gotoxy(17,4);
    Bk_color(197,15);
    printf("MAIN MENU");
    x1=3;
    x2=38;
    y1=6;
    pointx=6;
    Notification();

    getback:
        y2=y1+2;
        point = y1+1;
        Bk_color(117,1);
        if(x1==3)
        {
            gotoxy(12,7);
            printf("Music Library");

            gotoxy(12,9);
            printf("Add Music to Library");

            gotoxy(12,11);
            printf("User Manual");

            gotoxy(12,13);
            printf("About");

            gotoxy(12,15);
            printf("Timer");

            gotoxy(12,17);
            printf("Exit");

            setcolor(14);
            menu_option(x1,x2,y1,y2);
            Bk_color(225,9);
            if(y1+1==7)
            {
                gotoxy(12,7);
                printf("Music Library");
            }
            else if(y1+1==9)
            {
                gotoxy(12,9);
                printf("Add Music to Library");
            }
            else if(y1+1 == 11 )
            {
                gotoxy(12,11);
                printf("User Manual");
            }
            else if(y1+1 == 13 )
            {
                gotoxy(12,13);
                printf("About");
            }
            else if(y1+1 == 15 )
            {
                gotoxy(12,15);
                printf("Timer");
            }
            else if(y1+1 == 17)
            {
                gotoxy(12,17);
                printf("Exit");
            }
        }
        gotoxy(pointx,point);
        printf("%c",62);
        ch = getch();
        Bk_color(117,4);

       if(ch == -32)
       {
           ch = getch();
        if(ch == up || ch == down || ch == left || ch == right)
        {
            switch(ch)
            {
            case up:
                if(y1>6 && y1<= 17)
                {
                    gotoxy(pointx,point);
                    printf("%c",62);
                    setcolor(7);
                    menu_option(x1,x2,y1,y2);
                    y1 = y1-2;
                    goto getback;
                }
            case down:
                if(y1>=6 && y1<14)
                {
                    gotoxy(pointx,point);
                    printf("%c",62);

                    setcolor(7);

                    menu_option(x1,x2,y1,y2);
                    y1=y1+2;
                    goto getback;
                }
                 else if(y1==14)
                {
                    gotoxy(pointx,point);
                    setcolor(7);
                    menu_option(x1,x2,y1,y2);
                    y1=y1+2;
                    goto getback;
                }
                else
                {
                    goto getback;
                }

            }
        }

       }
       else if( ch == 13)
       {

           if(point == 7)
           {

                Bk_color(180,7);
                working_area_background();
                Bk_color(117,1);
                drowlineV(6,38,96,179);
                ind_back_freame(40,109,3,5,117,180,12);
                Bk_color(197,15);
                gotoxy(75-strlen("Name"),4);
                printf("Name");
                gotoxy(101,4);
                printf("DATE");
                m_library();
                goto getback;
           }
           else if(point == 11)
           {
               ind_back_freame(40,109,3,5,117,180,12);
                Bk_color(197,15);
                gotoxy(66,4);
                printf("User Manual");
                User_manual();
                goto getback;

           }
           else if(point == 13)
           {
                ind_back_freame(40,109,3,5,117,180,12);
                Bk_color(197,15);
                gotoxy(75-strlen("About"),4);
                printf("About");
                about();
                goto getback;
           }
           else if(point == 15)
           {
                Bk_color(180,7);
                working_area_background();
                ind_back_freame(40,109,3,5,117,180,12);
                Bk_color(197,15);
                gotoxy(75-strlen("Timer"),4);
                printf("Timer");
                gotoxy(44,7);
                Bk_color(117,1);
                printf("Enter time in sec             : ");
                scanf("%d",&n);
                if(flag ==1)
                    pthread_cancel(ptimer);
                flag = 1;
              //  pthread_t ptimer;

                pthread_create(&ptimer,NULL,timer,n);
                delay(10);
                flag = 0;
                goto getback;
           }
           else if(point == 17)
           {
               exit(0);

           }
           else if(point == 9)
           {
               Bk_color(180,7);
               working_area_background();
               ind_back_freame(40,109,3,5,117,180,12);
               Bk_color(197,15);
               gotoxy(75-strlen("Explore"),4);
               printf("Explore");
               detect_all_the_drives(Drives,&System_Drive_Count);
               File_explore(Drives);
               goto getback;
           }

       }
       else if(ch == 27)
       {
            Bk_color(180,7);
            working_area_background();
            ind_back_freame(40,109,3,5,117,180,12);
            Bk_color(197,15);
            gotoxy(70,4);
            printf("Notification");
            Notification();
            goto getback;
       }
       else{
        goto getback;
       }
}
void File_explore(char *Disk)
{

    int length,y1=7,i=0,a1=44,a2=95,b1,b2;
    char drive[50];
    struct node *temp;
    restart:
    length = strlen(Disk);


    if(length == 0)
    {
        Bk_color(117,1);
        gotoxy(60,20);
        printf("NO DRIVES");
    }
    else if(length ==1 && System_Drive_Count==0)
    {
        Bk_color(117,1);
        gotoxy(72,21);
        printf("NO DRIVES");
    }
    Bk_color(117,1);
    drive_background(45,7);

    if(length>=1)
    {
        Bk_color(117,2);
        drive_background(45,15);
    }
    if(length>=2)
    {
        Bk_color(117,5);
        drive_background(45,23);
    }
    if(length >=3)
    {
        Bk_color(117,20);
        drive_background(45,31);
    }



    Bk_color(28,15);
    gotoxy(66,10);
    printf("C Drive : System Drive ");
     if(length>=1)
    {
       //   detect_all_the_drives(Drives,System_Drive_Count);

        Bk_color(32,15);
        gotoxy(66,18);
    //  printf("%c",Drives[i]);
        printf("New voulume(%c:)",Disk[i]);
        i++;
    }
    if(length>=2)
    {
        Bk_color(82,15);
        gotoxy(66,26);
        printf("New Volume(%c:)",Disk[i]);
        i++;
    }
    if(length>=3)
    {
        Bk_color(68,15);
        gotoxy(66,34);
        printf("New Volume(%c:)",Disk[i]);
        i++;
    }
    getback:
    setcolor(12);
    drive_selector(41,y1);
    char ch=getch();
    if(ch == -32)
    {
        ch = getch();
        if(ch == up || ch == down)
        {
            if(ch == up)

            {
                if(y1>7 && y1<=(7+(length*8)))
                {
                    Bk_color(117,7);
                    drive_selector(41,y1);
                    y1 = y1-8;
                    goto getback;
                }
                else
                {
                    goto getback;
                }
            }
            if(ch == down)
            {
                if(y1>=7 && y1<(7+(length*8)))
                {

                    Bk_color(117,7);
                    drive_selector(41,y1);

                    y1=y1+8;
                    goto getback;

                }
                else if(y1 == (7+(length*8)))
                {
                    goto getback;
                }
            }
        }

        else
        {
            goto getback;
        }
    }


    else if(ch == 27 )
    {
            Bk_color(180,7);
            working_area_background();
            ind_back_freame(40,109,3,5,117,180,12);
            Bk_color(197,15);
            gotoxy(70,4);
            printf("Notification");
            Notification();
            return;

    }
    else if(ch ==13 )

    {
        b1 = 6;
        reset:
        b2 = b1+2;
        Bk_color(180,7);
        working_area_background();
        Bk_color(117,1);
        drowlineV(6,38,96,179);
        ind_back_freame(40,109,3,5,117,180,12);
        Bk_color(197,15);
        gotoxy(75-strlen("Name"),4);
        printf("Name");
        gotoxy(98,4);
        printf("FILE TYPE");
        gotoxy(50,25);
        song_selector(44,95,6,8);
        Bk_color(117,1);
        int i;
        strcpy(current_drive,"x:\\\\");
        setcolor(14);
        song_selector(a1,a2,b1,b2);
        temp = head;
        Bk_color(117,1);
        if((y1-7)/8 == 0)
        {
            i=7;

            current_drive_music_count=0;
            strcpy(current_drive,"C:\\\\");
            gotoxy(50,28);
            Bk_color(117,1);
            head = NULL;
            tail = NULL;
            Bk_color(117,1);
            listFilesRecursively("C:\\\\Users\\Sandhya.Raghu\\Downloads");
            temp = head;
            if(current_drive_music_count == 0)
            {
                gotoxy(55,20);
                Bk_color(117,1);
                printf("No Music Files");
            }
             else
            {
                struct node *temp;
                temp = head;
                Bk_color(117,1);
                while(temp != NULL)
                {
                    gotoxy(44,i);
                    printf("%s\n",temp->music_file);
                    gotoxy(100,i);
                    printf("mp3");
                    temp = temp->next;
                    i=i+2;
                }
                printf("\n");
            }
        }
        else
        {
            i=7;
            current_drive_music_count=0;
            current_drive[0] = Drives[(y1-7)/8 -1];
            head=NULL;
            tail=NULL;
            Bk_color(117,1);
            listFilesRecursively(current_drive);
            temp = head;
            if(current_drive_music_count == 0)
            {
                gotoxy(55,20);
                Bk_color(117,1);
                printf("No Music Files");
            }
             else
            {
                struct node *temp;
                temp = head;
              /*  for(int k=1;k<5;k++)
                {
                    gotoxy(44,i);
                    printf("%s\n",temp->music_file);
                    gotoxy(100,i);
                    printf("mp3");
                    temp = temp->next;
                    i=i+2;

                }*/
                if(current_drive_music_count > 17)
                 {
                     for(int k=1;k<17;k++)
                    {
                        gotoxy(44,i);
                        printf("%s\n",temp->music_file);
                        gotoxy(100,i);
                        printf("mp3");
                        temp = temp->next;
                        i=i+2;
                    }
                 }
                 else
                 {
                     while(temp != NULL)
                    {
                        gotoxy(44,i);
                        printf("%s\n",temp->music_file);
                        gotoxy(100,i);
                        printf("mp3");
                        temp = temp->next;
                        i=i+2;
                    }
                 }
                printf("\n");
            }


        }
        redo:
        if(current_drive_music_count >0)
        {

            setcolor(14);
            song_selector(a1,a2,b1,b2);
            Bk_color(225,9);
            gotoxy(a1,b1+1);
            printf("%s",temp->music_file);
        }
        char ch = getch();
        if(ch == -32)
            {
                ch = getch();
                if(ch == up || ch == down)
                {

                    if(ch == up)
                    {
                        if(b1>6 && b1< 6+(16*2))
                        {


                            cord_changed=1;
                            Bk_color(117,7);
                            song_selector(a1,a2,b1,b2);
                            gotoxy(a1,b1+1);
                            Bk_color(117,1);
                            printf("%s",temp->music_file);
                            temp = temp->prev;
                            b1=b1-2;
                            b2 = b1+2;
                            goto redo;
                        }
                        else if(b1 ==6 )
                            {
                                page = 1;
                                goto reset;
                            }
                    }
                    else if(ch == down)
                    {
                         //if(b1>=6 && b1<6+((current_drive_music_count-1)*2))
                          if(b1>=6 && b1<6+(15*2))
                          {
                               if(temp->next == NULL)
                                    goto redo;
                              cord_changed=1;
                              Bk_color(117,7);
                              song_selector(a1,a2,b1,b2);
                              gotoxy(a1,b1+1);
                              Bk_color(117,1);
                              printf("%s",temp->music_file);
                              temp = temp->next;
                              b1 = b1+2;
                              b2 = b1+2;
                              goto redo;
                          }
                          else if(b1 == 6+(15*2))
                          {

                                b1 = 6;
                                b2 = b1+2;
                                Bk_color(180,7);
                                working_area_background();
                                Bk_color(117,1);
                                drowlineV(6,38,96,179);
                                gotoxy(50,25);
                                song_selector(44,95,6,8);
                                Bk_color(117,1);
                                int i=b1+1;
                                setcolor(14);
                                song_selector(a1,a2,b1,b2);
                                Bk_color(117,1);
                                struct node *temp2 =temp;
                                while(temp != NULL)
                                {

                                    gotoxy(44,i);
                                    printf("%s\n",temp->music_file);
                                    gotoxy(100,i);
                                    printf("mp3");
                                    temp = temp->next;
                                    i=i+2;
                                }
                                temp = temp2;
                              goto redo;
                          }
                        else if(temp == NULL && b1>=6 && b1<6+(15*2))
                        {printf("hellooooo");
                            goto redo;
                        }
                    }
                    else if(ch == 8)
                    {
                        goto reset;
                    }
                }
                else
                {
                    goto redo;
                }
            }
        else if (ch == 27)
        {

            Bk_color(180,7);
            working_area_background();
            ind_back_freame(40,109,3,5,117,180,12);
            Bk_color(197,15);
            gotoxy(70,4);
            printf("Notification");
            Notification();
            return;

        }
        else if(ch == 8)
        {

            Bk_color(180,7);
            working_area_background();
            ind_back_freame(40,109,3,5,117,180,12);
            Bk_color(197,15);
            gotoxy(75-strlen("Explore"),4);
            printf("Explore");
            goto restart;
        }
        else if(ch == '+')
        {
            if(is_music_file_already_exist(temp->music_file) == 0)
                add_music_to_library(temp->data,temp->music_file);
            else
                MessageBoxA(NULL,"Music file already exist in library","Warning",16);
            goto redo;
        }
        else if(ch == 'p')
        {
          //  close_musicfile();
            if(cord_changed ==0 &&play == 1)
            {
                play =0;
                 ind_back_freame(1,110,40,42,28,180,1);
                gotoxy(2,41);
                Bk_color(28,15);
                printf("pause : %s",temp->music_file);
                Bk_color(117,1);
                pause_music();
            }
            else if(cord_changed == 0 && play==0)
            {
                 play=1;

                int re=play_music(temp->data,0);


                if(re == 0 && cord_changed==1)
                {
                    MessageBoxA(NULL," Unsupportable mp3 files","Error",64);
                }
            }
             else if(cord_changed==1)
             {
                cord_changed = 0;
                play=1;
                 ind_back_freame(1,110,40,42,28,180,1);
                gotoxy(2,41);
                Bk_color(28,15);
                printf("playing : %s",temp->music_file);
                Bk_color(117,1);
                close_musicfile();
                int re=play_music(temp->data,0);
                if(re == 0)
                {
                    MessageBoxA(NULL," Unsupportable mp3 files","Error",64);
                }
             }
            goto redo;
        }

        else
        {
            goto redo;
        }
    }
    else
    {
        goto getback;
    }

}
void drive_background(int x,int y)
{
    for(int i=0;i<=6;i++)
  { gotoxy(x,y+i);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219);
  }
}
void drive_selector(int x,int y)
{
    for(int i=0;i<=6;i++)
    {   gotoxy(x,y+i);
        printf("%c%c%c",219,219,219);
    }
}

void about()
{
    Bk_color(225,1);
    working_area_background();
    Bk_color(28,15);
    gotoxy(45,9);
    printf("Batch No:26");
    gotoxy(45,11);
    printf("Name : K.V.S.SIDDARTHA");
    gotoxy(45,13);
    printf("Roll : 1602-19-737-058");
    gotoxy(45,15);
    printf("Name : Pranay Teja ");
    gotoxy(45,17);
    printf("Roll : 1602-19-737-028");
}
void User_manual()
{

    Bk_color(180,1);
    working_area_background();
    Bk_color(28,15);
    gotoxy(42,7);
    printf("Press 'p' to play or pause music");
    gotoxy(42,9);
    printf("Press 'delete' key to delete a music file in library");
    gotoxy(42,11);
    printf("press 'Esc' key to get into the main menu");
    gotoxy(42,13);
    printf("Only music files in Downloads are visible in C:Drive");

}
void welcome_screen()
{
    Hide_cursor();
    gotoxy(40,10);
    printf("Loading....");
    gotoxy(40,11);
    for(int i=40;i<65;i++)
    {
        printf("%c",219);

    }
    gotoxy(40,11);
    setcolor(12);
    for(int i=40;i<65;i++)
    {
        printf("%c",219);
        delay(110);
    }
}
int is_mp3_file(char *f_path)
{
    int lenth;
    lenth=strlen(f_path);
    if(f_path[lenth]=='\0' && f_path[lenth-1]=='3' && (f_path[lenth-2]=='p' || f_path[lenth-2]=='P') && (f_path[lenth-3]=='m' || f_path[lenth-3]=='M') && f_path[lenth-4]=='.')
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
void listFilesRecursively(char *basePath)
{

    char path[1000];
    char a[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);
    if (!dir)
    {
       return;
    }

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0  && strcmp(dp->d_name,"$RECYCLE.BIN") !=0 && strcmp(dp->d_name,"$Recycle.Bin")!= 0)
        {
            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);
            listFilesRecursively(path);
            if(is_mp3_file(path) == 1)
            {
              insert(path,dp->d_name);
              current_drive_music_count++;

            }

        }
    }

    closedir(dir);
}
void insert(char* n,char *m)
{
    struct node *new,*temp;
    new = (struct node*)malloc(sizeof(struct node));
	strcpy(new->data,n);
	strcpy(new->music_file,m);
	new ->next = NULL;
	new ->prev = NULL;
	if(head == NULL)
	{
		head = new;
		tail = new;

	}
	else
	{
		tail->next = new;
		new->prev = tail;
		tail = new;

	}
}
void remove_slash()
{
    char *tem;
    struct node *temp;
    temp = head;
    int l = strlen(temp->data),j=0,i,k=0;
    char a[l+1],b[l+1];
    while(temp != NULL)
    {
        if(temp->data[3] == 47)
            temp->data[3] = 92;

        temp = temp->next;


    }
    printf("\n");
}
void display(char *head)
{
    struct node *temp;
    temp = head;
    while(temp != NULL)
    {
        printf("%s\n",temp->music_file);
        temp = temp->next;
    }
    printf("\n");
}
void display_rev(char *tail)
{
    struct node *temp;
    temp = tail;
    while(temp != NULL)
    {
        printf("%s\n",temp->music_file);
        temp = temp->prev;

    }
    printf("\n");
}
int play_music(char *file_name,int restart)
{
    char temp[250],invated_coma;
    int lenth=0,re1,re2,n;
    temp[0]='\0';
    invated_coma=34;
    strcat(temp,"open ");
    lenth=strlen(temp);
    temp[lenth]=invated_coma;
    lenth++;
    temp[lenth]='\0';
    strcat(temp,file_name);
    lenth=strlen(temp);
    temp[lenth]=invated_coma;
    lenth++;
    temp[lenth]='\0';
    strcat(temp," type mpegvideo alias mp3");
    re1=mciSendStringA(temp,NULL,0,NULL);
    re2=mciSendString("play mp3",NULL,0,NULL);
    if(restart == 1)
    {
        mciSendStringA("play mp3 repeat",NULL,0,NULL);
    }
    if(re1==0 && re2==0)
        return 1;
    else
        return 0;

}
void pause_music()
{
    int re=mciSendString("pause mp3",NULL,0,NULL);
    if(re == 0)
        return 1;
    else
        return 1;
}
int stop_music()
{
    int re1;
    re1=mciSendStringA("stop mp3",NULL,0,NULL);
    if(re1==0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
int close_musicfile()
{
    int re;
    re=mciSendStringA("close mp3",NULL,0,NULL);
    if(re==1)
        return 1;
    else
        return 0;
}
void main()
{
    welcome_screen();
    main_menu();
}
