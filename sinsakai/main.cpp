//
// タイトル　
// 
//
// std::cout << random_count << std::end;

#include "appEnv.hpp"

//デファイン定義
#define ENEMY_MAX 4
#define FLOORING 18
#define PLAYER_X 400//(-WIDTH / 2)
#define PLAYER_Y 0//(-HEIGHT / 3) + 16/2
//playerの移動（実装）
bool jumpswitch = true;
float player_move = 2; //playerの移動値（実装）
char nowhre = 0; //0が地上　１が空中ステータス。
//playerのjumpの為の変数
float jump_power_boost = 300; //jumpをし続けらる限界（実装）
float jump_cap = 0;  //	jumpの限界値の変数（実装）
float jump_power = 0.5; //jumpの力（実装）
float fall_power = 0.3;
int i = 0;
int onclearline = 0;
int link = 0;
int image_link = 0;
int pause = 0;
int waitmusic = 0;
bool jump_boost_true = true; //boostのスイッチ
// アプリのウインドウサイズ
enum Window {
	WIDTH = 1920,
	HEIGHT = 1080
};
struct Enemy{
	//敵のステータス
	float x, y,
	wide, height;
	Vec2f Vec;
	Color color;
};
struct Flooring{
	//足場の表示位置
	float flooring_x, flooring_y,
	flooringend_x, flooringend_y,
	flooring_size;
	Color flooring_color;
};

//マウスが右か左にでているか調べる。
float isMouseLeftOutBox(Vec2f mouse_pos, float x, float y, float width, float height)
{
	if (mouse_pos.x() < x){
		return player_move;
	}
	return 0;
}
float isMouseRightOutBox(Vec2f mouse_pos, float x, float y, float width, float height)
{
	if (mouse_pos.x() > (x + width)){
		return player_move;
	}

	return 0;
}
float isMousedownOutBox(Vec2f mouse_pos, float x, float y, float width, float height)
{
	if (mouse_pos.y() < y){
		return player_move;
	}
	return 0;
}
float isMouseUpOutBox(Vec2f mouse_pos, float x, float y, float width, float height)
{
	if (mouse_pos.y() > (y + height)){
		return player_move;
	}

	return 0;
}

//床処理
float isPlayerOnLine(float character_height, float character_x, float character_y,
	float object_x, float width, float height)
{
	if (character_x >= object_x - 1){
		if (character_x <= width - 1){
			if (character_y >= height - 1){
				if (character_y <= height - 1 + 16 / 2){
					return 1;
				}
			}
		}
	}
	return 0;
}

//当たり判定（実装）
float CheckHit(float objA_x, float objA_y,
	float objB_x, float objB_y,
	float objA_width, float objA_height,
	float objB_width, float objB_height)
{
	float x0 = objA_x;
	float y0 = objA_y;
	float x1 = objA_x + objA_width - 1;
	float y1 = objA_y + objA_height - 1;

	float x2 = objB_x;
	float y2 = objB_y;
	float x3 = objB_x + objB_width - 1;
	float y3 = objB_y + objB_height - 1;
	if (x0 < x3 && x2 < x1 && y0 < y3 && y2 < y1)
	{
		float z = 0;
		z++;
		//	重なっていた
		return 1;
	}
	return 0;
}


//　隠れるための為の関数(未実装)
bool Hyde(float y, float character_height, float outleft, float outright)
{
	if (outleft || outright == player_move){
		return true;
	}
	return false;
}
/*Flooring flooring_info[] = {
	{ 10, 10, 10, 10,10,Color(1, 1, 1) },
	{ 20,20,20,100,20,Color(1,1,1)}
	};*/
int flooring_index = 0;

// メインプログラム
// 
float main() {
	// アプリウインドウの準備
	AppEnv app_env(Window::WIDTH, Window::HEIGHT);
	float x = PLAYER_X;//(-WIDTH / 2);
	float y = PLAYER_Y;//(-HEIGHT / 3) + 16/2;
	//画像系の処理
	Texture op_image("res/1680x1050.png");
	Texture text1_image("res/erufutext1.png");
	Texture text2_image("res/erufutext2.png");
	Texture text3_image("res/erufutext3.png");
	Texture text4_image("res/erufutext4.png");
	Texture text5_image("res/erufutext5.png");
	Texture text6_image("res/erufutext6.png");
	Texture text7_image("res/erufutext7.png");
	Texture text8_image("res/erufutext8.png");
	Texture text9_image("res/erufutext9.png");
	Texture text10_image("res/erufutext10.png");
	Texture text11_image("res/erufutext11.png");
	Texture text12_image("res/erufutext12.png");
	Texture text13_image("res/erufutext13.png");
	Texture niwatori_image("res/niwatori.png");
	Texture title_image("res/title.png");
	Texture erufu("res/erufuno4.png");
	Texture backscreen("res/mo_165.png");
	Texture opbox("res/madot01a.png");
	Texture tbox("res/madot01d.png");
	//サウンドの系統の処理
	Media clearSE("res/Ambition.wav");
	Media sound("res/eruhunoyumi.wav");
	Media FallObjSE("res/strom.wav");
	Media title_sound("res/sodewonurasita.wav");
	//sound.play();
	title_sound.play();
	sound.gain(0.5);
	title_sound.gain(0.5);
	sound.looping(true);
	title_sound.looping(true);
	bool palaying = sound.isPlaying();
	int count = 0;
	while (1){
		{// メインループ
			while (1) {
				// ウィンドウが閉じられたらアプリを終了
				if (!app_env.isOpen()) return 0;
				// 描画準備
				app_env.setupDraw();
				image_link++;
				//マウスのボタンがクリックされたらカウントする。
				if (app_env.isPushButton(Mouse::LEFT))count++;


				// タイトルロゴ
				drawTextureBox(50, 0, 950, 512,
					0, 0, 900, 512,
					title_image,
					Color(1, 1, 1), 0, Vec2f(2.14, 2.3), Vec2f(900 / 2, 500 / 2));
				if (count >= 1){
					app_env.setupDraw();
					//1920, 1080
					drawTextureBox(0, 0,1920,1080,
						0, 0,1680,1050,
						op_image,
						Color(1, 1, 1), 0, Vec2f(1, 1), Vec2f(1920/2, 1080/2));

				}
				//opキャラ表示
				//if ((count >= 1)&&(!count==2))

					

				//±100 セリフ顔差分。
				if (count == 1)
					drawTextureBox(-710,-400,1024, 256,
					0, 0,1024,256,
					text1_image,
					Color(1, 1, 1),
					0,
					Vec2f(1, 1), Vec2f(0, 0));
				if (count == 2)
					drawTextureBox(-710, -400, 1024, 256,
					0, 0, 1024, 256,
					text2_image,
					Color(1, 1, 1),
					0,
					Vec2f(1, 1), Vec2f(0, 0));
				if (count == 3)
					drawTextureBox(-710, -400, 1024, 256,
					0, 0, 1024, 256,
					text3_image,
					Color(1, 1, 1),
					0,
					Vec2f(1, 1), Vec2f(0, 0));
				if (count == 4)
					drawTextureBox(-710, -400, 1024, 256,
					0, 0, 1024, 256,
					text4_image,
					Color(1, 1, 1),
					0,
					Vec2f(1, 1), Vec2f(0, 0));
				if (count == 5)	drawTextureBox(-710, -400, 1024, 256,
					0, 0, 1024, 256,
					text5_image,
					Color(1, 1, 1),
					0,
					Vec2f(1, 1), Vec2f(0, 0));
				if (count == 6)	
					drawTextureBox(-710, -400, 1024, 256,
					0, 0, 1024, 256,
					text6_image,
					Color(1, 1, 1),
					0,
					Vec2f(1, 1), Vec2f(0, 0));
				if (count == 7)	
					drawTextureBox(-710,-400,1024, 256,
					0, 0,1024,256,
					text7_image,
					Color(1, 1, 1),
					0,
					Vec2f(1, 1), Vec2f(0, 0));
				if (count == 8)	
					drawTextureBox(-710, -400, 1024, 256,
					0, 0, 1024, 256,
					text8_image,
					Color(1, 1, 1),
					0,
					Vec2f(1, 1), Vec2f(0, 0));
				if (count == 9)
					drawTextureBox(-710, -400, 1024, 256,
					0, 0, 1024, 256,
					text9_image,
					Color(1, 1, 1),
					0,
					Vec2f(1, 1), Vec2f(0, 0));
				if (count == 10)
					drawTextureBox(-710, -400, 1024, 256,
					0, 0, 1024, 256,
					text10_image,
					Color(1, 1, 1),
					0,
					Vec2f(1, 1), Vec2f(0, 0));
				if (count == 11)
					drawTextureBox(-710, -400, 1024, 256,
					0, 0, 1024, 256,
					text11_image,
					Color(1, 1, 1),
					0,
					Vec2f(1, 1), Vec2f(0, 0));
				if (count == 12)
					drawTextureBox(-710, -400, 1024, 256,
					0, 0, 1024, 256,
					text12_image,
					Color(1, 1, 1),
					0,
					Vec2f(1, 1), Vec2f(0, 0));
				if (count == 13)
					drawTextureBox(-710, -400, 1024, 256,
					0, 0, 1024, 256,
					text13_image,
					Color(1, 1, 1),
					0,
					Vec2f(1, 1), Vec2f(0, 0));
				if (count == 14)break;
				// 画面を更新
				app_env.update();
			}

			//入力のフラッシュ
			app_env.flushInput();
		}
		title_sound.stop();
		image_link = 0;
		sound.play();
		// メインループ
		while (1) {
			if (!app_env.isOpen())return 0;

			//描画準備
			app_env.setupDraw();

			//sine計算に使うインクルメント演算
			link++;
			image_link++;
			int image_count = ((image_link / 20) % 3);
			//床の位置

			Flooring flooring_info[] = {
				{ -200, -200,
				200, -200,
				20, Color(0, 1, 1) },

				{ -500, -300,
				-200, -300,
				10, Color(1, 1, 0) },
				{ WIDTH / 2 - 300, HEIGHT / 2 - 200,  //宝箱の足場
				WIDTH / 2 - 100, HEIGHT / 2 - 200,
				10, Color(1, 1, 0) },
				{ (std::cos(link / 30) * 50) - 600, -60,   //移動床１
				(std::cos(link / 30) * 50) - 400, -60,
				10, Color(1, 0.5, 0.35) },
				{ (std::sin(-link / 30) * 50) - 550, -120,   //移動床2
				(std::sin(-link / 30) * 50) - 350, -120,
				10, Color(1, 0.5, 0) },
				{ (std::sin(-link / 30) * 40) - 400, 0,    //移動床3
				(std::sin(-link / 30) * 20) - 300, 0,
				10, Color(0, 1, 0) },
				{ -200, 0,    //移動床２隣接の床
				-100, 0,
				10, Color(1, 1, 0) },
				{ (std::sin(-link / 30) * 20) - 700, 100,  //広がる床
				(std::sin(link / 30) * 20) - 500, 100,
				10, Color(1, 1, 0) },
				{ (std::sin(-link / 30) * 20) - 750, 200,  //広がる床2
				(std::sin(link / 30) * 20) - 550, 200,
				10, Color(1, 1, 0) },
				{ (std::sin(-link / 30) * 20) - 800, 300,  //広がる床3
				(std::sin(link / 30) * 20) - 600, 300,
				10, Color(1, 1, 0) },
				{ -270, 300,  //広がる床から飛び移る
				-100, 300,
				10, Color(1, 1, 0) },
				{ 230, 100,  //ダイブして乗る足場
				(std::sin(link / 30) * 20) + 450, 100,
				10, Color(1, 1, 0) },
				{ 300, 0,  //ダイブして乗る足場の接続床
				450, 0,
				10, Color(1, 1, 0) },
				{ 300, 0,  //宝箱に接続する床
				450, 0,
				10, Color(1, 1, 0) },
				{ 800, 80,  //宝箱に接続する床
				900, 80,
				10, Color(1, 1, 0) },
				{ 800, -45,  //宝箱に接続する床
				900, -45,
				10, Color(1, 1, 0) },
				{ 600, 0,  //宝箱に接続する床
				700, 0,
				10, Color(1, 1, 0) },
				{ 650, 200,  //宝箱に接続する床
				750, 200,
				10, Color(1, 1, 0) },
			};
			//敵の位置
			Enemy Enemy_info[]{
				{ 10, std::sin(link / 20) * 50 + 400, 10, 10,Vec2f(5,5), Color(1, 0, 0) },
				{ 10, std::sin(link / 20) * 50 + 0, 10, 10, Vec2f(5,5),Color(1, 0, 0) },
				{ 200,
				-300,
				100,
				100,Vec2f(1,1), Color(1, 0, 0) },
			};

			int flooring_index = 0;


			Flooring floorings = flooring_info[flooring_index];


			// ウィンドウが閉じられたらアプリを終了
			if (!app_env.isOpen()) return 0;
			//マウスの位置を変数にコピー
			Vec2f mouse_pos = app_env.mousePosition();
			// 描画準備
			app_env.setupDraw();

			drawTextureBox(0, 0, WIDTH, HEIGHT, 0, 0, WIDTH, HEIGHT, backscreen, Color(1, 1, 1), 0, Vec2f(1, 1), Vec2f(WIDTH / 2, HEIGHT / 2));

			if (onclearline == 0){
				x -= isMouseLeftOutBox(mouse_pos, x, y, (128 / 6), 32);
				x += isMouseRightOutBox(mouse_pos, x, y, (128 / 6), 32);

				//jumpの為の関数
				if ((jumpswitch == true) && (app_env.isPressKey('A')))
				{
					jump_power = 1;
					y += jump_power;
					std::cout << jump_power << std::endl;
					jump_cap += jump_power;
					jump_power_boost -= 10;
					if (jump_power_boost <= 0){ //ブーストが無くなったら0を維持
						jump_power_boost = 0;
					}
				}
			}
				//if (jump_cap <= 0){
				//jump_power_boost += 10;
				//}
				if (jump_cap >= 150 || /*(jump_power_boost == 0) ||*/ app_env.isPullKey('A')){ //降下スイッチ
					jumpswitch = false;
				}


				//床が関わる位置
				/*drawLine(flooring_info[i].flooring_x, flooring_info[i].flooring_y,
					flooring_info[i].flooringend_x, flooring_info[i].flooringend_y,
					flooring_info[i].flooring_size, flooring_info[i].flooring_color);
					drawFillBox(flooring_info[i].flooring_x, flooring_info[i].flooring_y,
					10, 10, flooring_info[i].flooring_color);
					drawFillBox(flooring_info[i].flooringend_x - 10, flooring_info[i].flooringend_y,
					10, 10, flooring_info[i].flooring_color);*/
				if ((jumpswitch == false) && (y >= -HEIGHT / 3) && (onclearline == 0) /*|| ((jump_power_boost <= 0)) && //降下処理
					(y >= -HEIGHT / 3)*/)
				{
					y += -jump_power;
					jump_cap += -jump_power;
					jump_power += fall_power;
					if (jump_power > 5){
						jump_power = 5;
					}
				}
				if (isPlayerOnLine(32, x, y,
					-WIDTH / 2, WIDTH / 2, (-HEIGHT / 3)) == 1
					&& (!app_env.isPressKey('A')) || (y <= -HEIGHT / 3) && (app_env.isPressKey(' '))) //床処理
				{
					jump_power = 0;
					jump_cap = 0;
					jump_power_boost += 10;
					jumpswitch = true;

				}

				for (i = 0; i < FLOORING; i++){
					if (isPlayerOnLine(32,
						x, y,
						flooring_info[i].flooring_x,
						flooring_info[i].flooringend_x, flooring_info[i].flooring_y) == 1
						&& (!app_env.isPressKey('A')) && (onclearline == 0))//足場処理
					{
						y += 0;
						jump_power = 0;
						jump_cap = 0;
						jump_power_boost += 10;
						jumpswitch = true;
					}
					if ((CheckHit(x + (128 / 12), y,
						flooring_info[i].flooring_x - 10, flooring_info[i].flooring_y,
						(128 / 6), 10,
						10, 10) == 1) || (CheckHit(x - (128 / 12), y,
						flooring_info[i].flooringend_x, flooring_info[i].flooring_y,
						(128 / 6), 10,
						10, 10) == 1))
					{
						y -= jump_power;
						jump_power += fall_power;
						jumpswitch = false;
						FallObjSE.play();
						FallObjSE.gain(0.5);
					}
				}
				//sqrt(ルート)

				//キャラ表示
				if (isMouseLeftOutBox(mouse_pos, x, y, (128 / 6), 32) == player_move){
					drawTextureBox(x, y, (128 / 6), 32,
						(128 / 6)*image_count, 32 * 1,
						(128 / 6), 32, erufu, Color(1, 1, 1), 0, Vec2f(2, 2), Vec2f(0, 0));
				}
				if (isMouseRightOutBox(mouse_pos, x, y, (128 / 6), 32) == player_move){
					drawTextureBox(x, y, (128 / 6), 32,
						(128 / 6)*image_count, 32 * 2,
						(128 / 6), 32, erufu, Color(1, 1, 1), 0, Vec2f(2, 2), Vec2f(0, 0));
				}
				if (!(isMouseLeftOutBox(mouse_pos, x, y, (128 / 6), 32) == player_move) &&
					(!(isMouseRightOutBox(mouse_pos, x, y, (128 / 6), 32) == player_move) &&
					(isMouseUpOutBox(mouse_pos, x, y, (128 / 6), 32) == player_move))){
					drawTextureBox(x, y, (128 / 6), 32,
						(128 / 6)*image_count, 32 * 3,
						(128 / 6), 32, erufu, Color(1, 1, 1), 0, Vec2f(2, 2), Vec2f(0, 0));
				}
				if (!(isMouseLeftOutBox(mouse_pos, x, y, (128 / 6), 32) == player_move) &&
					(!(isMouseRightOutBox(mouse_pos, x, y, (128 / 6), 32) == player_move) &&
					!(isMouseUpOutBox(mouse_pos, x, y, (128 / 6), 32) == player_move))){
					drawTextureBox(x, y, (128 / 6), 32,
						(128 / 6)*image_count, 32 * 0,
						(128 / 6), 32, erufu, Color(1, 1, 1), 0, Vec2f(2, 2), Vec2f(0, 0));
				}

				//マウス追従キャラ(実験)
				drawFillBox(mouse_pos.x(), mouse_pos.y(), 10, 10, Color(1, 1, 1), 0, Vec2f(1, 1), Vec2f(0, 0));

				//ステータスを-WIDTH / 4, -HEIGHT / 8, 0, -HEIGHT / 8, 100, Color(1, 0, 1)
				/*drawFillBox(10, 10,
					10, 10,
					Color(isPlayerOnLine(10, x, y, -WIDTH / 2, WIDTH / 2, (-HEIGHT / 3)), 1, 1));//床用
					drawFillBox(-WIDTH / 2, -300, 10, jump_cap, Color(1, 0, 1));
					drawFillBox(100, 100,
					100, 100,
					Color(isPlayerOnLine(10, x, y, -WIDTH / 4, 0, (-HEIGHT / 8)), 1, 1));//足場用*/
				//床表示
				drawLine(-WIDTH / 2, -HEIGHT / 3, WIDTH / 2, -HEIGHT / 3, 500, Color(0.7, 0.5, 0.01));
				drawFillBox(-WIDTH / 2, (-HEIGHT / 3) - 10, WIDTH, (-HEIGHT / 3) - 10, Color(0.7, 0.5, 0.01));
				//敵表示
				for (i = 0; i < ENEMY_MAX; i++){
					drawTextureBox(Enemy_info[i].x, Enemy_info[i].y,
						Enemy_info[i].wide, Enemy_info[i].height,
						(72 / 3)*image_count, (128/4)*1,
						(72 / 3), (128/4),
						niwatori_image,
						Color(1, 0.5, 0.5),
						0,
						Enemy_info[i].Vec,Vec2f(1,1));

					if ((CheckHit(x, y,
						Enemy_info[i].x, Enemy_info[i].y,
						(128 / 6), 32,
						Enemy_info[i].wide, Enemy_info[i].height) == 1))
					{
						y -= jump_power;
						jump_power += fall_power;
						jumpswitch = false;
						FallObjSE.play();
						FallObjSE.gain(0.5);
					}
				}
				//宝箱表示当たり判定未実装(位置確定,宝箱配置完了)
				if (isPlayerOnLine(32,
					x, y,
					flooring_info[2].flooring_x,
					flooring_info[2].flooringend_x, flooring_info[2].flooring_y) == 1
					&& (!app_env.isPressKey('A'))){
					drawTextureBox((WIDTH / 2 - 250) + 3, (HEIGHT / 2 - 200), 102, 90,
						0, 0, 51, 45,
						opbox,
						Color(1, 1, 1),
						0,
						Vec2f(1, 1), Vec2f(0, 0));
					onclearline++;
					sound.stop();
					if (onclearline == 1)
						clearSE.play();
					
				}
				else//足場処理
					drawTextureBox((WIDTH / 2 - 250), (HEIGHT / 2 - 200), 94, 68,
					0, 0, 47, 34,
					tbox,
					Color(1, 1, 1),
					0,
					Vec2f(1, 1), Vec2f(0, 0));
				if ((onclearline >= 1) && (waitmusic <= 29*60))
					waitmusic++;
				if (waitmusic >= 29*60)
					app_env.setupDraw();
				//足場表示
				//drawLine(-WIDTH / 4, -HEIGHT / 8, 0, -HEIGHT / 8, 100, Color(1, 0, 1));
				for (i = 0; i < FLOORING; i++){
					drawLine(flooring_info[i].flooring_x, flooring_info[i].flooring_y,
						flooring_info[i].flooringend_x, flooring_info[i].flooringend_y,
						flooring_info[i].flooring_size, flooring_info[i].flooring_color);
					drawFillBox(flooring_info[i].flooring_x - 10, flooring_info[i].flooring_y,
						10, 10, flooring_info[i].flooring_color);
					drawFillBox(flooring_info[i].flooringend_x, flooring_info[i].flooringend_y,
						10, 10, flooring_info[i].flooring_color);
				}
				//drawFillBox((-WIDTH / 4) - 10, -HEIGHT / 8, 10, 10, Color(1, 0, 1));
				//drawFillBox(0, -HEIGHT / 8, 10, 10, Color(1, 0, 1));
				if (waitmusic >= 29 * 60)
					app_env.setupDraw();
				// 画面を更新
				app_env.update();
			}
		}	// アプリ終了

	}