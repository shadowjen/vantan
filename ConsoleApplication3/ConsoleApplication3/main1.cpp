#include <stdio.h>
int main(){
	for (int i = 0; i < 10; i++)
	{
	}
	int game_mode;
	game_mode = 0;
	switch (game_mode)
	{
	case 0://�^�C�g��
		printf("�^�C�g��\n");
		break;
	case 1://�Q�[���̖{��
		printf("�Q�[���̖{��\n");
		break;

	case 2://�G���f�B���O
		printf("�G���f�B���O\n");
		break;

	}

	getchar();
	return 0;
}