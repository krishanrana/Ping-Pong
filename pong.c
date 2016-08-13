#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <cab202_graphics.h>
#include <cab202_sprites.h>
#include <cab202_timers.h>

#define DELAY (10)

int lives = 5;
int score = 0;
int level = 1;
bool game_over;
bool update_screen = true;
bool setup_complete = false;



char * help_image = 
/**/              "  CAB202 Assignment 1 - Pong      "
/**/              "                                  "
/**/              "         Krishan Rana             "
/**/              "           n9240781               "
/**/              "                                  "	
/**/              "  Controls:                       "
/**/              "    Arrow Keys: Move Up/Move Down "
/**/              "    h: Show this help screen      "
/**/              "    q: Quit Game                  "
/**/              "    l: Cycle Levels               "
/**/              "                                  "
/**/              "    Press any key to play...      ";


static char * count_down_image = 
/**/              "+---------+"
/**/              "|         |"
/**/              "|         |"
/**/              "|         |"
/**/              "+---------+";




static char * left_paddle_image = 
/**/     "|"
/**/     "|"
/**/     "|"
/**/     "|"
/**/     "|"
/**/     "|"
/**/     "|";



static char * right_paddle_image = 
/**/     "|"
/**/     "|"
/**/     "|"
/**/     "|"
/**/     "|"
/**/     "|"
/**/     "|";

static char * ball_image =
/**/     "o";

sprite_id ball;
sprite_id left_paddle;
sprite_id right_paddle;
sprite_id count_timer;

void setup_right_paddle(void);
void setup_level1(void);
void process_level1(void);
void help_screen(void);
void count_down(void);
void ball_setup(void);
void setup_left_paddle(void);
void setup_level2(void);
void process_level2(void);



bool collision_check(void){

int w = screen_width();
int h = screen_height();

bool collided = true;

int right_paddle_top = round(sprite_y(right_paddle));
int right_paddle_bottom = right_paddle_top + 7 -1;
int right_paddle_left = round(sprite_x(right_paddle));
int right_paddle_right = right_paddle_left; //not too sure if i should add 1

int ball_top = round(sprite_y(ball));
int ball_bottom = ball_top; // not too sure if i should add 1
int ball_left = round(sprite_x(ball));
int ball_right = ball_left; ///not too sure if i should add 1


if(right_paddle_bottom < ball_top){
	collided = false;
}
else if(right_paddle_top > ball_bottom){
	collided = false;
}
else if(right_paddle_right < ball_left){
	collided = false;
}
else if(right_paddle_left > ball_right){
	collided = false;
}

return collided;

}





void setup_right_paddle(void){

int w = screen_width();
int h = screen_height();
int r_paddle_x = w-4;
int r_paddle_y = (h-7)*0.5;  //two columns of empty space
right_paddle = sprite_create(r_paddle_x, r_paddle_y, 1, 7, right_paddle_image);
sprite_draw(right_paddle);

}

void setup_left_paddle(void){

int w = screen_width();
int h = screen_height();
int l_paddle_x = 4;
int l_paddle_y = (h-7)*0.5;  //two columns of empty space
left_paddle = sprite_create(l_paddle_x, l_paddle_y, 1, 7, left_paddle_image);
sprite_draw(left_paddle);

}


void setup_level1(void){

int w = screen_width();
int h = screen_height();

//draw framework
draw_line(0,0,w,0,'*');
draw_line(w-1,0,w-1,h-1,'*');
draw_line(0,h-1,w,h-1,'*');
draw_line(0,0,0,h-1,'*');
draw_line(0,2,w-1,2,'*');
count_down();

ball_setup();

//setup the right paddle
setup_right_paddle();

show_screen();

}

void setup_level2(void){

int w = screen_width();
int h = screen_height();

//draw framework
draw_line(0,0,w,0,'*');
draw_line(w-1,0,w-1,h-1,'*');
draw_line(0,h-1,w,h-1,'*');
draw_line(0,0,0,h-1,'*');
draw_line(0,2,w-1,2,'*');
count_down();

ball_setup();

//setup the right and left paddle
setup_right_paddle();
setup_left_paddle();

show_screen();

}







void ball_setup(void){

int w = screen_width();
int h = screen_height();

//setup the ball at the center of the screen
int ball_x = w/2;
int ball_y = h/2;
ball = sprite_create(ball_x, ball_y, 1, 1, ball_image);
sprite_draw(ball);

//set the ball in motion
int now = get_current_time();
srand(now);
sprite_turn_to(ball, 0.3, 0.0);
int angle = rand() % 45; 
sprite_turn(ball, angle);

}



void process_level1(void){

int key = get_char();

int w = screen_width();
int h = screen_height();	

int width = screen_width()-2;
int height = screen_height()-2;


int paddle_right_x = sprite_x(right_paddle);
int paddle_right_y = sprite_y(right_paddle);


//Controls used to move the right paddle up and down
if(key == 'w' && paddle_right_y > 3){
	sprite_move(right_paddle, 0, -1);
}
else if(key == 's' && paddle_right_y < h - 8){
	sprite_move(right_paddle, 0, +1);
}

//check for level change
if(key == 'l'){
	level++;
}



		// ball takes one step.
		sprite_step(ball);

		// Get screen location of ball.
		int bx = round( sprite_x( ball ) );
		int by = round( sprite_y( ball ) );

		// Get the displacement vector of the ball.
		double bdx = sprite_dx( ball );
		double bdy = sprite_dy( ball );
		bool dir_changed = false;


		// Test to see if the ball hit the left or right border.
		if ( bx < 1 ) {
			bdx = -bdx;
			dir_changed = true;
		}

		// Test to see if the wrecker hit the top or bottom border.
		if ( by < 3  || by > h - 2) {
			bdy = -bdy;
			dir_changed = true;
		}

		if(bx>w){
			lives--;
			setup_level1();
		}


		// Test to see if the wrecker needs to step back and change direction.
		if ( dir_changed ) {
			sprite_back( ball );
			sprite_turn_to( ball, bdx, bdy );
		}

collision_check();

bool collided = collision_check();

if(collided){    
	bdx = -bdx;
	sprite_turn_to( ball, bdx, bdy );
}


clear_screen();

draw_line(0,0,w,0,'*');
draw_line(w-1,0,w-1,h-1,'*');
draw_line(0,h-1,w,h-1,'*');
draw_line(0,0,0,h-1,'*');
draw_line(0,2,w-1,2,'*');

draw_formatted(2,1,"Lives = %d",lives);
draw_formatted((width*0.25),1,"* Score = %d",score);
draw_formatted((width*0.5),1,"* Level = %d",level);
draw_formatted((width*0.75),1,"* Time = %d",lives);

sprite_draw(ball);
sprite_draw(right_paddle);

}


void process_level2(void){

if(!setup_complete){
	setup_level2();
	setup_complete = true;
}




int key = get_char();

int w = screen_width();
int h = screen_height();	

int width = screen_width()-2;
int height = screen_height()-2;


int paddle_right_x = sprite_x(right_paddle);
int paddle_right_y = sprite_y(right_paddle);


//Controls used to move the right paddle up and down
if(key == 'w' && paddle_right_y > 3){
	sprite_move(right_paddle, 0, -1);
}
else if(key == 's' && paddle_right_y < h - 8){
	sprite_move(right_paddle, 0, +1);
}

//check for level change
if(key == 'l'){
	level++;
	setup_complete = false;
}

		// ball takes one step.
		sprite_step(ball);

		// Get screen location of ball.
		int bx = round( sprite_x( ball ) );
		int by = round( sprite_y( ball ) );

		// Get the displacement vector of the ball.
		double bdx = sprite_dx( ball );
		double bdy = sprite_dy( ball );
		bool dir_changed = false;


		// Test to see if the ball hit the left or right border.
		if ( bx < 1 ) {
			bdx = -bdx;
			dir_changed = true;
		}

		// Test to see if the wrecker hit the top or bottom border.
		if ( by < 3  || by > h - 2) {
			bdy = -bdy;
			dir_changed = true;
		}

		if(bx>w){
			lives--;
			setup_level1();
		}


		// Test to see if the wrecker needs to step back and change direction.
		if ( dir_changed ) {
			sprite_back( ball );
			sprite_turn_to( ball, bdx, bdy );
		}

collision_check();

bool collided = collision_check();

if(collided){    
	bdx = -bdx;
	sprite_turn_to( ball, bdx, bdy );
}


clear_screen();

draw_line(0,0,w,0,'*');
draw_line(w-1,0,w-1,h-1,'*');
draw_line(0,h-1,w,h-1,'*');
draw_line(0,0,0,h-1,'*');
draw_line(0,2,w-1,2,'*');

draw_formatted(2,1,"Lives = %d",lives);
draw_formatted((width*0.25),1,"* Score = %d",score);
draw_formatted((width*0.5),1,"* Level = %d",level);
draw_formatted((width*0.75),1,"* Time = %d",lives);

sprite_draw(ball);
sprite_draw(right_paddle);
sprite_draw(left_paddle);

}





void count_down(void){

int w = screen_width();
int h = screen_height();

count_timer = sprite_create((w-11)/2, (h - 5)/2, 11, 5, count_down_image);
sprite_draw(count_timer);

for(int counter = 3; counter>0; counter--){	
draw_formatted((w-1)/2, (h-1)/2, "%d", counter);
draw_formatted(2,1,"Lives = %d",lives);
draw_formatted(((w-2)*0.25),1,"* Score = %d",score);
draw_formatted(((w-2)*0.5),1,"* Level = %d",level);
show_screen();
timer_pause(300);
}

}





void help_screen(void){

int width = screen_width();
int height = screen_height();
int msg_height = 12;
int msg_width = 34;
sprite_id msg_box = sprite_create((width-msg_width)/2, (height - msg_height)/2,
	msg_width, msg_height, help_image);
sprite_draw(msg_box);
show_screen();

}


int main(void){

setup_screen();
help_screen();
wait_char();
clear_screen();
setup_level1();
show_screen();


    while ( !game_over ) {


    	if(level == 1){

    		process_level1();

		if ( update_screen ) {
            show_screen();
        }
			timer_pause(DELAY);
    	}

    	if(level == 2){

    		process_level2();

		if ( update_screen ) {
            show_screen();
        }
			timer_pause(DELAY);

    	}
        
    }

    return 0;
}


////NEED TO PREVENT PADDLES FROM REFRESHING AT START POSITION ALL THE TIME
////NEED TO WRITE AI FOR THE LEFT PADDLE
