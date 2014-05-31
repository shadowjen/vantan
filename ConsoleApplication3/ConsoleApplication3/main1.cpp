#include <stdio.h>
int main(){
	for (int i = 0; i < 10; i++)
	{
	}
	int game_mode;
	game_mode = 0;
	switch (game_mode)
	{
	case 0://タイトル
		printf("タイトル\n");
		break;
	case 1://ゲームの本体
		printf("ゲームの本体\n");
		break;

	case 2://エンディング
		printf("エンディング\n");
		break;

	}

	getchar();
	return 0;
}