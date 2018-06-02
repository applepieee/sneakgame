#include<iostream>
#include<ncurses.h>
#include<ctime>
#include<cstdlib>
#include<unistd.h>
#include<cstdio>
using namespace std;

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

#define random(x) (double)(rand()%x+1.0)


class game{
	public:
		game();
		void check();
		void input(int);
		void logic(int);
		void display();
		void displaysneak();
	private:
		double x,y,tailx[100],taily[100];
		double rx,ry,wx[3][3],wy[3][3],bx,by,fx,fy;
		double u,v,passtime;//speed,time
		int width,height,ntail;
		int dir,back_dir,score,v_sum,b_count;
		bool gameOver,eat,r_canEat,b_caneat;
		
};
game::game()
{
	 
	getmaxyx(stdscr,height,width);
	fx=(double)random(width);
	fy=(double)random(height);
 	rx=(double)random(30);
        ry=(double)random(30);
 	bx=(double)random(10);
        by=(double)random(10);
	for(int i=0; i<3 ;i++)
	{
 		wx[i][0]=(double)random(width);
        	wy[i][0]=(double)random(height);
		wx[i][1]=wx[i][0]+1;
		wy[i][1]=wy[i][0];
		wx[i][2]=wx[i][1];
		wy[i][2]=wy[i][1]+1;
	}
	b_count=0;
	passtime=0;
	v_sum-0;
	tailx[100]={0};
	taily[100]={0};
	u=v=1;
	x=y=2;
	dir=back_dir=0;
	ntail=0;
	score=0;
	r_canEat=true;
	b_caneat=true;
	eat=false;
	gameOver=false;
}
void game::check()
{
	while(x<0) x+=width;
	while(x>=width) x-=width;
	while(y<0) y+=height;
	while(y>=height) y-=height;
}
void game::input(int c)
{
	switch(c){ 
		case KEY_LEFT:
			if(dir != RIGHT)
			{       
				dir=LEFT;
			}break;
		case KEY_RIGHT:
			if(dir != LEFT)
			{       
				dir=RIGHT;
			}break;
		case KEY_UP:
			if(dir !=DOWN)
			{       
				dir=UP;
			}break;
		case KEY_DOWN:
			if(dir != UP)
			{      
				dir=DOWN;
			}break;
		default: break;	
	}	

}
void game::logic(int c)
{

	v_sum++;
	if(v_sum % 100 ==0) {
		v++;u++;r_canEat=false,b_caneat=false;
	}
	if(v_sum %205== 0) r_canEat=true,b_caneat=true;
	if(v_sum %5000 ==0){
		ntail++;
	}
    	mvprintw(3,14,"%d",v_sum);
	if(!gameOver)
	{	
		if(b_count != 0){
			if( (tailx[ntail-2]-tailx[ntail-1] ) > 0) back_dir=RIGHT;
			else if( (tailx[ntail-2]-tailx[ntail-1] ) < 0) back_dir=LEFT;
			if( (taily[ntail-2]-taily[ntail-1] ) > 0) back_dir=DOWN;
                	else if( (taily[ntail-2]-taily[ntail-1] ) < 0) back_dir=UP;
		}
                for(int i=ntail-1;i>0;i--){ 	
                        tailx[i]=tailx[i-1];
                        taily[i]=taily[i-1];
  		}
		tailx[0]=x;
                taily[0]=y;

		switch(c)
		{
                case KEY_LEFT:
			if(dir != RIGHT) 
			{       usleep(100000);
				x-=(u/10000)+1;
			}break;
                case KEY_RIGHT:
			if(dir != LEFT)
                        {       usleep(100000);
                                x+=(u/10000)+1;
                        }break;
                case KEY_UP:
			 if(dir != DOWN)
                        {       usleep(100000);
                                y-=(v/10000)+1;			
		       	}break;
                case KEY_DOWN:
			 if(dir != UP)
                        {       usleep(100000);
                                y+=(v/10000)+1;
                        }break;
		case ERR:
			switch(dir)
			{
				case LEFT:
					usleep(100000);
					x-=(u/10000)+1;
	   
                			break;
				case RIGHT:
					usleep(100000);
					x+=(u/10000)+1;
					break;
				case UP:
					usleep(100000);
					y-=(v/10000)+1;
					break;
				case DOWN:
					usleep(100000);
					y+=(v/10000)+1;
					break;
			}break;
        	}
		check();

	 	if((int)x == (int)fx && (int)y ==(int)fy)
        	{
                	score+=5;
                	fx=(double)random(width);
                	fy=(double)random(height);
       		 }
		if((int)x== (int)rx && (int)y==(int)ry)
		{	
			if(!eat && r_canEat) 
				gameOver=true;
		}
		for(int i=0;i<3;i++)
			for(int j=0;j<3;j++)
			{
				if((int)x==(int)wx[i][j] && (int)y==(int)wy[i][j])
					if(!eat)gameOver=true;
			}	
		
		if((int)x==(int)bx && (int)y==(int)by)
		{
			if(b_caneat){
				//int t=rand()%2+0;
				/*if(!t){
					b_count++; ntail+=4;
					if(b_count==1) back_dir=dir;
					for(int i=(b_count-1)*4; i< ntail ;i++){
						switch(back_dir){
						case LEFT:
							if(i==0){
								tailx[i]=x+1;
								taily[i]=y;
							}else{
								tailx[i]=tailx[i-1]+1;									
								taily[i]=taily[i-1];
							}break;
						case RIGHT:
					 		if(i==0){
                                                               	tailx[i]=x-1;
                                                               	taily[i]=y;
                                                       	}else{
                                                               	tailx[i]=tailx[i-1]-1;
                                                               	taily[i]=taily[i-1];
                                                    	}break;
						case UP:
					 		if(i==0){
                                                               	tailx[i]=x;
                                                              	taily[i]=y+1;
                                                       	}else{
                                                               	tailx[i]=tailx[i-1];
                                                               	taily[i]=taily[i-1]+1;
                                                     	}break;
						case DOWN:
							if(i==0){
                                                               	tailx[i]=x;
                                                               	taily[i]=y-1;
                                                       	}else{
                                                               	tailx[i]=tailx[i-1];
                                                               	taily[i]=taily[i-1]-1;
                                                       	}break;
					}	
				}					
			}else{
				eat=true;	
			}*/
			 
                         gameOver=true;
	
			 bx=(double)random(width);
                         by=(double)random(height);               
		}
	}
		for(int i=1;i<ntail;i++)
		{		
			if(x== tailx[i] && y==taily[i])
				gameOver=true;	
		}
	}
}
void game::display()
{	

	attrset(A_NORMAL);
	if(gameOver) mvaddstr(14,14,"Game Over");
	if(!gameOver)
	{
		passtime=(double)clock()/CLOCKS_PER_SEC;
	}
	char c[20];
	sprintf(c,"%f",passtime);
	mvaddstr(1,14,c);
	
	start_color();
        init_pair(1,COLOR_GREEN,COLOR_BLACK);
        attrset(COLOR_PAIR(1));
        mvaddstr(fy,fx,"$");
	init_pair(2,COLOR_RED,COLOR_BLACK);
	attrset(COLOR_PAIR(2));
	if(r_canEat)
		mvaddstr(ry,rx,"$");
	for(int i=0;i<3;i++)
		for(int j=0;j<3;j++)		
			mvaddstr(wy[i][j],wx[i][j],"#");
	init_pair(3,COLOR_BLUE,COLOR_BLACK);
	attrset(COLOR_PAIR(3));
	if(b_caneat)
		mvaddstr(by,bx,"?");
        attrset(A_NORMAL);
	char s[10];
	sprintf(s,"%d",score);	
	mvaddstr(0,14,"score:");
	mvaddstr(0,22,s);
	refresh();
}
void game::displaysneak()
{
	clear();	
	static int sum=0;       
       if(eat){
       		attrset(A_BLINK);
		sum++;
       }
        mvaddstr(y,x,"0");
       if(sum > 100) {
               attrset(A_NORMAL);eat=false;
	       sum=0;
       }

        for(int i=0; i< ntail ;i++){
                mvaddstr(taily[i],tailx[i],"o");
        }

}
int main()
{
	
	int c;
	srand(time(0));
	initscr();
	cbreak();
	noecho();
	keypad(stdscr,TRUE);
	nodelay(stdscr,TRUE);
	game Game;
	while(1)
	{
		c=getch();
		Game.display();
		Game.displaysneak();
		Game.input(c);
		Game.logic(c);
		//usleep(100000);
	}
	endwin();
}

