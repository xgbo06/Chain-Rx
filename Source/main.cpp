#include <allegro.h>

/* This Class is Cell Which will be the place Where Atom/Ball 
    would kept */
class Cell{
       int cell_color ; //= makecol(200,200,200);
       int x,y;
       int max_ball,current_ball;
       int Owner;
       BITMAP *C;
    public:
           Cell();
           void set_x_y(int a,int b,int max);
           int Update_cell(int player,int color);
           void Draw_ball();
           void set_owner(int n){
                Owner = n;
                }
           int return_owner();
       };
       
     Cell::Cell(){
                  cell_color = makecol(200,200,200);
                  x = y = 0;
                  max_ball = current_ball = 0;
                  Owner = 0;
                  }

/* Simply Returns the Owner of cell for Evaluation of Reaction and other functions */
int Cell::return_owner(){
            return Owner;
            }

void Cell::set_x_y(int a,int b,int max){
            x = a;
            y = b;
            max_ball = max;
            C = create_bitmap(80,80);
            }

void Cell::Draw_ball(){
            clear_to_color(C,makecol(200,200,200));
            rect(C,0,0,80,80,makecol(0,0,0));
            switch (current_ball){
                   case 1:
                        circlefill(C,40,40,20,cell_color);
                        break;
                   case 2:
                        circlefill(C,20,40,18,cell_color);
                        circlefill(C,60,40,18,cell_color);
                        break;
                   case 3:
                        circlefill(C,40,20,16,cell_color);
                        circlefill(C,20,60,16,cell_color);
                        circlefill(C,60,60,16,cell_color);
                        break;
                       }// End of Switch;
             blit(C,screen,0,0,x,y,C->w,C->h);
             }

int Cell::Update_cell(int player,int color){
           /* if(Owner!=0 && (Owner==player)){
              
              return;
              }*/
            Owner = player;
            cell_color = color;
            current_ball++;
            if(current_ball > max_ball){
               current_ball = 0;
               Draw_ball();
               return 1;
               }
            Draw_ball();
            return 0;
            }
          
Cell C[8][6];


/***********************************************/
class player{
       public:
              int col;
              int no;
              player(int n,int c){
                         col = c;
                         no = n;
                         }
       };


player a(1,5),b(2,5534);   /* 31 = blue;  896= green green = 1334*/

player current_player(1,5);

/****************************************************/

/* this function will initilize the cell with values and
  draw the cells on screen accrodingly */ 
void draw_grid(){
      int i = 0 ,kx = 0;
      int j = 0,ky = 0;
      int max = 0;
      for(i = 0; i < 640; i += 80,kx++)
        for(j = 0,ky = 0; j < 480; j += 80,ky++){
         max = 0;
         if(((kx==ky)&&(kx==0))||((kx==7)&&(ky==5))||((kx==0)&&(ky==5))||((kx==7)&&(ky==0)))
             max = 1;
         else if((kx==0)||(ky==0)||(kx==7)||(ky==5))
                 max = 2;
               else
                  max = 3;
         C[kx][ky].set_x_y(i,j,max);
         C[kx][ky].Draw_ball();
         }//End of both for loop;
      
      }
/**************************************************************************/

/**************** toggle Player *********************/
/**** box will give information ******/
BITMAP *box;
void toggle_player(){
     if(current_player.no == a.no)
        current_player = b;
     else 
        current_player = a;
     
     clear_to_color(box,makecol(150,150,150));
     textprintf_ex(box,font,15,5,makecol(0,0,0),-1,"Player %d Turn",current_player.no);
     blit(box,screen,0,0,0,480,box->w,box->h);
     }
/**************************************************************************/	  

/*****************CHECK FOR WINNER***************************************/
int check_winer(){
    int p1=0,p2=0;
    int i,j;
    
    for(i=0;i<8;i++)
      for(j=0;j<6;j++){
        if(C[i][j].return_owner()==1)
          p1++;
          else if(C[i][j].return_owner()==2)
                  p2++;
            }
    
    if(p1==0){
     clear_to_color(box,makecol(150,150,150));
     textprintf_ex(box,font,15,5,makecol(0,0,0),-1,"Player 2 Win Press Key to Exit");
     blit(box,screen,0,0,0,480,box->w,box->h);
     allegro_message("Player 2 Won The Game");
     return 2;
     }
    
    if(p2 == 0){
           clear_to_color(box,makecol(150,150,150));
           textprintf_ex(box,font,15,5,makecol(0,0,0),-1,"Player 1 Win Press Key to Exit");
           blit(box,screen,0,0,0,480,box->w,box->h);
           allegro_message("Player 1 Won The Game");
           return 1;
           }
      
    return 0;
    
    }
/******************************************************************************************/

/*************IMPRTANT REACTION***************/
int count_rx = 0;
/* here count_rx is counter this will stop the reaction when all the cells are
   filled or they will be reacting forever :( */
void reaction(int x,int y){
     int tx,ty;
     tx = x;
     ty = y;
     
     count_rx++;
     if(count_rx > 50){
                 check_winer();
                 exit(0);
                 return;
                 }
     
     rest(200);
     C[x][y].set_owner(0);
     if((tx-1)>=0){
                   if(C[tx-1][y].Update_cell(current_player.no,current_player.col))
                   {
                     reaction(tx-1,y);
                     }
                   } 
          
     if((tx+1)<=7){
                   if(C[tx+1][y].Update_cell(current_player.no,current_player.col))
                   {
                     reaction(tx+1,y);
                     }
                 }
     
     if((ty-1)>=0){
                   if(C[x][ty-1].Update_cell(current_player.no,current_player.col))
                     {
                      reaction(x,ty-1);
                      }
                  }
     
     if((ty+1)<=5){
                   if(C[x][ty+1].Update_cell(current_player.no,current_player.col))
                      {
                       reaction(x,ty+1);
                       }
                   }
     }
/**********REACTION ABOVE ****************/
	  

/*** fuction to read mouse click and act accrodingly *****/
int click(){
     int x,y;
     int flag = 1;
      while(flag){
              if((mouse_b & 1)){
                          x = int(mouse_x);
                          y = int(mouse_y);
                          flag = 0;
                          }
              }
      x /= 80;
      y /= 80;
      
      if( (C[x][y].return_owner() != current_player.no) && (C[x][y].return_owner() != 0) )
          goto out;
      
      if(C[x][y].Update_cell(current_player.no,current_player.col)){
                 count_rx = 0;
                 reaction(x,y);
                 }
      toggle_player(); 
            
      rest(500);
      
      out : { rest(3); }
      }
/************************************************/
	  
	  
/* this is very basic fuction wich will initilaze the allegro for work
   to begian */
int init_graphics() 
{	
	allegro_init();
	install_keyboard();
	install_mouse();
	set_color_depth(32);
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 520, 0, 0);
	set_window_title("Chain Rx    :-To Exit Press Esc or Any Key");
    clear_to_color(screen,makecol(200,200,200));
    box = create_bitmap(640,40);
    clear_to_color(box,makecol(150,150,150));
    textprintf_ex(box,font,15,5,makecol(0,0,0),-1,"Player 1 Turn");
    blit(box,screen,0,0,0,480,box->w,box->h);
    show_mouse(screen);
	return 0;
}

//textprintf_ex(screen,font,x*70,y*70,makecol(0,0,0),-1,"Color is %d",current_player.col);


/************GAME*******************/
void game(){
     
     }

//void player

int main(){
    init_graphics();
    draw_grid();
    int k=0;
    click();
    while(!keypressed())
      {
        //clear_keybuf();
        click();
        k = check_winer();
        if(k==1){
           //allegro_message("Player 1 Won The Game");
           break;
           }
        else if(k==2){
             //allegro_message("Player 2 Won The Game");
             break;
             }
       // clear_keybuf();
        }
    //readkey();
    }END_OF_MAIN();
