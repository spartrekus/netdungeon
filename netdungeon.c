    

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#include <limits.h>
#include <ncurses.h>




#ifdef _WIN32
/// WIN
#include <windows.h>
int termsize()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns, rows;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    printf("OS: Windows \n" );
    printf("columns: %d\n", columns);
    printf("rows: %d\n", rows);
}
#else
/// LINUX
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
int termsize()
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    printf("OS: Linux \n" );
    printf ("lines %d\n", w.ws_row);
    printf ("columns %d\n", w.ws_col);
}
#endif







int rows, cols;  
int cellx , celly;
int datadm[200][200];


void create_dm()
{
   int i , j ;  
   for( i = 1 ;  i<= 100; i++ )
    for( j = 1 ; j<= 100; j++ )
     datadm[j][i] = ' ' ;

   j = 3; 
   for( i = 15 ;  i<= 50; i++ )
     datadm[j][i] = '-' ;

   j = 25; 
   for( i = 14 ;  i<= 51; i++ )
     datadm[j][i] = '-' ;

   j = 15; 
   for( i = 14 ;  i<= 21; i++ )
     datadm[j][i] = '-' ;

   j = 18; 
   for( i = 14 ;  i<= 21; i++ )
     datadm[j][i] = '-' ;


   i = 14; for( j = 4 ;  j<= 24; j++ )
     datadm[j][i] = '|' ;
   i = 51; for( j = 4 ;  j<= 24; j++ )
     datadm[j][i] = '|' ;
}


void draw_dm()
{  
   int i , j ;  
   for( i = 1 ;  i<= 100; i++ )
    for( j = 1 ; j<= 100; j++ )
     mvprintw( j , i , "%c", datadm[j][i] );
}

void draw_player()
{
        mvprintw(  celly , cellx , "O" );
}

void draw_statusbar()
{
        mvprintw(rows-1 , 0, "<Press Key> ");
        printw( "[Y=%d, X=%d]", celly , cellx );
        printw( "[%c]", datadm[celly][cellx] );
}

void draw_init()
{
        erase();
        mvprintw( 0 , 0, "=NetDungeon=");	/* Print Hello World		  */
}


void draw_help()
{
        erase(); int i = 5 ; 
        mvprintw( 0 , 0, "=NetDungeon=");	/* Print Hello World		  */
        mvprintw( i++ , 0, "O : Player " );
        mvprintw( i++ , 0, "P : Net Player (Online Mode, if Client MMORPH Rogue)" );
        mvprintw( i++ , 0, "- : Wall " );
        mvprintw( i++ , 0, "| : Wall " );
        mvprintw( i++ , 0, "D : Door (d:open)" );
        mvprintw( i++ , 0, "C : Chest " );
        mvprintw( i++ , 0, "W : Weapon " );
        mvprintw( i++ , 0, "I : Item " );
        mvprintw( i++ , 0, "M : Monster " );
        getch();
}

void save_dm()
{
   int i , j , foo; 
   char charo[PATH_MAX];
   FILE *fpout;
   fpout = fopen( "datadm.dat", "wb+");
   for( i = 1 ;  i<= 100; i++ )
    for( j = 1 ; j<= 100; j++ )
    {  
       foo = snprintf( charo, 50 , "%d\t%d\t%c", j, i , datadm[j][i] );
       fputs( charo , fpout);
       fputs( "\n" , fpout);
    }
     //mvprintw( j , i , "%c", datadm[j][i] );
   fclose( fpout );
}


///////////////////////////////////////////////////////////
int main( int argc, char *argv[])
{
   printf("Hello to NetDungeon!\n");
   termsize();

   char cwd[PATH_MAX];
   int posy, posx; 
   celly = 8; 
   cellx = 17; 
   char cellcontent[PATH_MAX];
   char clipboard[PATH_MAX];
   double clippile = 0;
   int gameover_nsc = 0;
   int game_edit = 0;
   int i , j , foo;
   FILE *fp;

   initscr();			
   curs_set( 0 );
   int ch ; 
   getmaxyx( stdscr, rows, cols);

   int newcell;


	// initscr();			/* Start curses mode 		  */
        // curs_set( 0 );

        create_dm();

        while ( gameover_nsc == 0)
        {
           getmaxyx( stdscr, rows, cols);

           draw_init();
           draw_dm();
           draw_player();
           draw_statusbar();

	   refresh();			/* Print it on to the real screen */
           ch = getch();


           switch( ch )
           {
             case '?':
                 draw_help();
                 break;
             case 'j':
                 {if ( datadm[ celly+1 ][ cellx ] == ' ' ) celly+=1;}
                 break;
             case 'k':
                 {if ( datadm[ celly-1 ][ cellx ] == ' ' ) celly-=1;}
                 break;
             case 'l':
                 {if ( datadm[ celly ][ cellx+1 ] == ' ' ) cellx+=1;}
                 break;
             case 'h':
                 {if ( datadm[ celly ][ cellx-1 ] == ' ' ) cellx-=1;}
                 break;
             case 'o':
                 mvprintw( 0,0, "[ OPEN (N,S,W,E)? ]               " );
                 ch = getch();
                 if      ( ch == 'n' ) datadm[ celly-1 ][ cellx ] = ' ' ;
                 else if ( ch == 's' ) datadm[ celly+1 ][ cellx ] = ' ' ;
                 else if ( ch == 'w' ) datadm[ celly ][ cellx-1 ] = ' ' ;
                 else if ( ch == 'e' ) datadm[ celly ][ cellx+1 ] = ' ' ;

                 else if ( ch == 'k' ) datadm[ celly-1 ][ cellx ] = ' ' ;
                 else if ( ch == 'j' ) datadm[ celly+1 ][ cellx ] = ' ' ;
                 else if ( ch == 'h' ) datadm[ celly ][ cellx-1 ] = ' ' ;
                 else if ( ch == 'l' ) datadm[ celly ][ cellx+1 ] = ' ' ;
                 break;
             case 'c':
                 mvprintw( 0,0, "[ 1/1: CREATE CELL? ]               " );
                 ch = getch();
                 newcell = ch;
                 datadm[ celly ][ cellx ] = newcell ;
                 break;
             case 'S':
                 save_dm();
                 break;

             case '-':
                 if ( game_edit == 1 ) datadm[ celly ][ cellx ] = '-' ;
                 break;
             case '|':
                 if ( game_edit == 1 ) datadm[ celly ][ cellx ] = '|' ;
                 break;
           }



           if ( ch == 'E' ) game_edit = 1;
           if ( ch == 'q' ) gameover_nsc = 1;
           if ( ch == 'Q' ) gameover_nsc = 1;
           if ( ch == 'g' ) 
           {
              //ch = getch(); 
              //if ( ch == 'g' ) {  celly=0; cellx=0;   }
           }
 
        }

        curs_set( 1 );
	endwin();			/* End curses mode		  */


	printf("Bye from NetDungeon!\n");
	return 0;
}





