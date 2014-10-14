//
// アプリ雛形
//

#include "lib/appEnv.hpp"
#include <ctime>
#include <math.h>

#define MAP_X 14
#define MAP_Y 18

int size = 40;

bool enemyflag = 1;

bool enemy_course_flag = 1;

int enemy_x =  0;

int enemy_y = -500;

int enemy_move = 3;

int enemy_x_move = 0;

int enemy_y_move = 0;


int enemy_count = 0;

float t1 = -110;
float t2 = -510;
float t3 = -120;
float t4 = -520;
float t5 = -130;
float t6 = -530;



void EnemyMove(Vec2f map_num[]){
	if (enemyflag == 1)
	{
		if (enemy_count < 60 * 2){
			enemy_y += enemy_move;
			t1 += (enemy_x - t1) / 20;
			t2 += (enemy_y - t2) / 20;
			t3 += (t1 - t3) / 20;
			t4 += (t2 - t4) / 20;
			t5 += (t3 - t5) / 20;
			t6 += (t4 - t6) / 20;
		}
		else
		{
			//enemy_y += enemy_move;
			enemy_x += enemy_move * cos(atan2(map_num[MAP_X * 3 + 6].y() - enemy_y, map_num[MAP_X * 3 + 6].x() - enemy_x));
			enemy_y += enemy_move * sin(atan2(map_num[MAP_X * 3 + 6].y() - enemy_y, map_num[MAP_X * 3 + 6].x() - enemy_x));
         
			t1 += enemy_move * cos(atan2(map_num[14 * 3 + 7].y() - t2, map_num[14 * 3 + 7].x() - t1));
			t2 += enemy_move * sin(atan2(map_num[14 * 3 + 7].y() - t2, map_num[14 * 3 + 7].x() - t1));

			t3 += enemy_move * cos(atan2(map_num[14 * 4 + 6].y() - t4, map_num[14 * 4 + 6].x() - t3));
			t4 += enemy_move * sin(atan2(map_num[14 * 4 + 6].y() - t4, map_num[14 * 4 + 6].x() - t3));

			t5 += enemy_move * cos(atan2(map_num[14 * 4 + 7].y() - t6, map_num[14 * 4 + 7].x() - t5));
			t6 += enemy_move * sin(atan2(map_num[14 * 4 + 7].y() - t6, map_num[14 * 4 + 7].x() - t5));

		}
		enemy_count++;
		if ((-480/2 > enemy_x) || (enemy_x > 480/2) || (-720 / 2 > enemy_y) || (enemy_y > 720 / 2)){
			//enemyflag = 0;
			
		}
	}
}

void UpDate()
{
	enemy_x_move = (enemy_x / sqrt((enemy_x - 1000) ^ 2 + (enemy_y - 1000) ^ 2) * enemy_move);
	enemy_y_move = (enemy_y / sqrt((enemy_x - 1000) ^ 2 + (enemy_y - 1000) ^ 2) * enemy_move);
}

void Run()
{
	if (sqrt((enemy_x - 1000) ^ 2 + (enemy_y - 1000) ^ 2) < 100)
	{
		UpDate();
	}
	else
	{
		enemy_x += enemy_x_move*(60*3.14);
		enemy_y += enemy_y_move*(60*3.14);
	}
}

	void Draw()
{
	if (enemyflag == 1){
		drawFillBox(enemy_x, enemy_y, size, size, Color(1, 1, 1), 0, Vec2f(1, 1), Vec2f(0, 0));
		drawFillBox(t1, t2, size, size, Color(1, 0, 1), 0, Vec2f(1, 1), Vec2f(0, 0));
		drawFillBox(t3, t4, size, size, Color(1, 1, 0), 0, Vec2f(1, 1), Vec2f(0, 0));
		drawFillBox(t5, t6, size, size, Color(0, 0, 1), 0, Vec2f(1, 1), Vec2f(0, 0));

	}
}

	void Draw2(int draw_x,int draw_y)
	{
		drawBox(draw_x, draw_y, size, size,1, Color(1, 1, 1));
	}




// アプリのウインドウサイズ
enum Window {
	WIDTH = 560,
	HEIGHT = 720
};


int Pos_x(int pos_x){
	return pos_x - WIDTH / 2;
}

int Pos_y(int pos_y){
	return pos_y - HEIGHT / 2;
}


bool IsHit(float dx, float dy, float cr1, float cr2){   //dx x1-x2 dy y1-y2 cr 円の半径
	if (dx*dx + dy *dy > cr1*cr2)
	{
		return true;
	}
	return false;
}

//int Map_Date[MAP_Y][MAP_X] = 
//{ 0 };

// メインプログラム
// 
int main() {
	// アプリウインドウの準備
	AppEnv app_env(Window::WIDTH, Window::HEIGHT,
		false, true);

	Texture block1("res/block1.png");
	//Texture bg1("res/");

	int map_x = 0;
	int map_y = 0;
	int count = 0;

	Vec2f map_num[MAP_Y*MAP_X];

	for (map_y = MAP_Y - 1; map_y > -1 ;map_y--){
		for (map_x = 0; map_x < MAP_X; map_x++)
		{
			map_num[count] = Vec2f(Pos_x(size * (map_x)), Pos_y(size * map_y));
			count++;
		}
	}


	while (1)
	{
		if (!app_env.isOpen())return(0);
		app_env.setupDraw();

		Vec2f  mousepos = app_env.mousePosition();
		

			for (map_y = 0; map_y < MAP_Y; map_y++){
				for (map_x = 0; map_x < MAP_X; map_x++)
				{
					Draw2(map_num[(map_y*14)+map_x].x(), map_num[map_y*MAP_X+map_x].y());
				}
			}

		//EnemyMove();

		EnemyMove(map_num);

		Draw();



		drawFillBox(mousepos.x(), mousepos.y(), 20, 20, Color(1, 1, 1),0,Vec2f(1,1),Vec2f(10,10));

		app_env.update();
	}

	// アプリ終了
}