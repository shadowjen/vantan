//
// アプリ雛形
//

#include "Lib/appEnv.hpp"
#include "Lib/random.hpp"
#include <ctime>
#include <math.h>
//デファイン定義
//マス目の数をデファイン定義で固定
#define MAP_X 14
#define MAP_Y 18
#define MAX_ENEMY 15

float ev = 1;

int size = 40;

int enemy_x = 0;

int  stop_count = 0;

int enemy_count = 0;


int link = 0;




//表示
void Draw(int enemy_x, int enemy_y, Color c)
{
		drawFillBox(enemy_x, enemy_y, size, size, c, 0, Vec2f(1, 1), Vec2f(0, 0));
}

	//マス目を表示
	void Draw2(int draw_x,int draw_y)
	{
		drawBox(draw_x, draw_y, size, size,1, Color(1, 1, 1));
	}


// アプリのウインドウサイズ
enum Window {
	WIDTH = 560,
	HEIGHT = 720
};

//ｘ座標０を左端に変換する
int Pos_x(int pos_x){
	return pos_x - WIDTH / 2;
}
//ｙ座標０を画面の一番下に変換する
int Pos_y(int pos_y){
	return pos_y - HEIGHT / 2;
}


void scroll(AppEnv& app_env, int map_x_max, int map_y_max, int size, int& count, Texture& bg){   //scroll
	for (int map_y = 0; map_y < map_y_max * 2 + size; map_y++){
		for (int map_x = 0; map_x < map_x_max; map_x++)
		{
			int a = Pos_y(map_y*size);
			a = a - count % (HEIGHT + size);


			drawTextureBox(Pos_x(map_x*size), a - HEIGHT / +size, size, size, 0, 0, 512, 512, bg, Color(1, 1, 1));
		}
	}



	//countが－を超えると画像の位置がずれる為変動する位置に強制的に移動させる。
	if (count < 0){
		count = HEIGHT * 2 + size;
	}
	//countを回して背景の移動
	if (!app_env.isPressKey('S')){
		count += 3;
	}
	//さらわれた時のflagの代わりにSを置く
	if (app_env.isPressKey('S')){
		count -= 10;
	}
}

// メインプログラム
// 
int main() {
	// アプリウインドウの準備
	AppEnv app_env(Window::WIDTH, Window::HEIGHT,
		false, false);
	Texture block("res/hosi.png");
	int map_x = 0;
	int map_y = 0;
	int count = 0;

	Vec2f map_num[MAP_Y*MAP_X];

	//　座標の位置を記憶させるためにmap_numでナンバリングする。　2列目以降から14をmap_Yにかけることで計算にて何番目かが把握可能　map_xは0〜１３の横の位置をを決めることができる
	//　ーから始まっているのは左上から始めるつもりだったが左下に０位置を持ってきてしまった為

	for (map_y = MAP_Y - 1; map_y > -1 ;map_y--){
		for (map_x = 0; map_x < MAP_X; map_x++)
		{
			map_num[count] = Vec2f(Pos_x(size * (map_x)), Pos_y(size * map_y));
			count++;
		}
	}

	count = 0;

	while (1)
	{
		if (!app_env.isOpen())return(0);
		app_env.setupDraw();

		Vec2f  mousepos = app_env.mousePosition();



			//画像を小さくして配列でループさせる
		scroll(app_env, MAP_X, MAP_Y, size, count, block);

			//座標の表示　//(白枠の表示)
			for (map_y = 0; map_y < MAP_Y; map_y++){
				for (map_x = 0; map_x < MAP_X; map_x++)
				{
					Draw2(map_num[(map_y * 14) + map_x].x(), map_num[map_y*MAP_X + map_x].y());
				}
			}
			


			//Y座標はMAP_X * 0~17で計算する　//敵の座標＋初期移動
			for (int i = 5; i < 9; i++){
				Draw(map_num[i].x() + enemy_x, map_num[MAP_X * 2].y(), Color(0, 0, 1));
			}

			for (int i = 3; i < 11; i++){
				Draw(map_num[i].x() + enemy_x, map_num[MAP_X * 3].y(), Color(1, 0, 0));
				Draw(map_num[i].x() + enemy_x, map_num[MAP_X * 4].y(), Color(1, 0, 0));
			}

			for (int i = 2; i < 12; i++){
				Draw(map_num[i].x() + enemy_x, map_num[MAP_X * 5].y() , Color(1, 1, 0));
				Draw(map_num[i].x() + enemy_x, map_num[MAP_X * 6].y() , Color(1, 1, 0));
			}

			link--;

			count++;

			enemy_x -= ev;   // エネミーにエネミーｖを追加して動かす

			stop_count++;

			//3往復半動いたらとまる。
			if ((abs(enemy_x) > abs(size))&&(stop_count < size*8)){
			ev = -ev;
			}
			//元の座標に戻すため7を入れる。
			if (stop_count > size * 8 + 7 ){
				ev = 0;
			}

		app_env.update();
	}

	// アプリ終了
}