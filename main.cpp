//	�V���[�e�B���O�Q�[���T���v��
//
//	�P�N���̉ċx�ݑO�܂łɊo���Ă����������߂����ł����Ă��܂��B
//	�킩��Ȃ����߂͒��ׂ���A�搶�ɕ������肵�āA
//	�ċx�ݑO�ɗ����ł���悤�ɂ��܂��傤�I
//
//	Code by Masao Watanabe

//	DX���C�u�������g�p���邽�߂Ƀw�b�_�[�t�@�C�����C���N���[�h����B
#include "DxLib.h"
#include "screen.h"

//	------------------------------------------------------------------------------------------------------------------------
//	�f�t�@�C����`
//	------------------------------------------------------------------------------------------------------------------------

#define OBJECT_PLAYER	1			//	�v���C���[������ID
#define OBJECT_ENEMY	2			//	�G������ID

#define STATE_NONE		-1			//	�I�u�W�F�N�g�͑��݂��Ă��Ȃ�
#define STATE_LIVE		1			//	�I�u�W�F�N�g�͑��ݒ�
#define STATE_HIT		2			//	�I�u�W�F�N�g�͂Ԃ����Ă���

//	�v���C���[���p�f�[�^
#define PLAYER_START_X	(SCREEN_WIDTH / 2)				//	�v���C���[�̏���X�ʒu
#define PLAYER_START_Y	(SCREEN_HEIGHT - PLAYER_HEIGHT)	//	�v���C���[�̏���Y�ʒu
#define PLAYER_WIDTH	32								//	�v���C���[�̉���
#define PLAYER_HEIGHT	32								//	�v���C���[�̏c��
#define PLAYER_COLOR	0xFFFFFF						//	�v���C���[�̐F�i16�i���ŋL������Ă��܂��j
#define PLAYER_SHOT_WIDTH	16							//	�v���C���[�̃V���b�g�̉���
#define PLAYER_SHOT_HEIGHT	16							//	�v���C���[�̃V���b�g�̏c��
#define PLAYER_SHOT_SPEED	8							//	�v���C���[�̃V���b�g�̃X�s�[�h
#define PLAYER_MOVESPEED	4							//	�v���C���[�̈ړ���

//	�G���p�f�[�^
#define ENEMY_START_X	0								//	�G�̏���X�ʒu
#define ENEMY_START_Y	0								//	�G�̏���Y�ʒu
#define ENEMY_WIDTH		32								//	�G�̉���
#define ENEMY_HEIGHT	32								//	�G�̏c��
#define ENEMY_COLOR		0x00FF00						//	�G�̐F�i16�i���ŋL������Ă��܂��j
#define ENEMY_SHOT_WIDTH	16							//	�G�̃V���b�g�̉���
#define ENEMY_SHOT_HEIGHT	16							//	�G�̃V���b�g�̏c��
#define ENEMY_MOVESPEED		2							//	�G�̈ړ���
#define ENEMY_SHOT_SPEED	8							//	�G�̃V���b�g�̃X�s�[�h
#define ENEMY_SHOT_MIN_TIME	100							//	�G���V���b�g��ł܂ł̍Œ�Ԋu
#define ENEMY_SHOT_MAX_TIME	400							//	�G���V���b�g��ł܂ł̍ő�Ԋu

//	�w�i���
#define BG_COLOR 0x000000		//	�w�i�̐F

#define SCREEN_WIDTH 640			//	�X�N���[���̉���
#define SCREEN_HEIGHT 480			//	�X�N���[���̏c��

//	------------------------------------------------------------------------------------------------------------------------
//	�\���̒�`
//	------------------------------------------------------------------------------------------------------------------------

//	�I�u�W�F�N�g�\���́B�v���C���[�A�G�A�V���b�g�̌��ƂȂ�\����
struct OBJECT
{
	int x;			//	X���W
	int y;			//	Y���W
	int width;		//	����
	int height;		//	�c��
	int color;		//	�F
	int state;		//	��Ԃ�\���ϐ�(-1...���݂��Ă��Ȃ�, 1...�����Ă�, 2...�Ԃ�����)
};

//	------------------------------------------------------------------------------------------------------------------------
//	�O���[�o���ϐ���`
//	------------------------------------------------------------------------------------------------------------------------

int PlayerGraphic;
OBJECT Player;			//	�v���C���[���
OBJECT Enemy;			//	�G���
OBJECT PlayerShot;		//	�v���C���[�̃V���b�g���
OBJECT EnemyShot;		//	�G�̃V���b�g���

int enemyMoveDirc = 1;	//	�G�̈ړ�����
int enemyShotTime = 0;	//	�G���e�����^�C�~���O

void InitGame(void);
void GameMain(void);

//	--------------------------------------------------------------------
//	���C���֐���`
//	--------------------------------------------------------------------
int WINAPI WinMain(
			HINSTANCE hInstance,      // ���݂̃C���X�^���X�̃n���h��
			HINSTANCE hPrevInstance,  // �ȑO�̃C���X�^���X�̃n���h��
			LPSTR lpCmdLine,          // �R�}���h���C��
			int nCmdShow              // �\�����
)
{
	//	DXLIB�̏���������
	DxLib_Init();

	//	�Q�[��������
	InitGame();

	//	�Q�[�����[�v
	//	�ȉ��̏������G�X�P�[�v�L�[���������܂ł��v���C���[�������Ă���ԌJ��Ԃ�

	while( ProcessMessage() == 0 && CheckHitKey( KEY_INPUT_ESCAPE ) == 0 && Player.state == STATE_LIVE)
	{
		GameMain();
	}

	//	DXLIB�̏I������
	DxLib_End();

	//	�A�v���P�[�V�����̏I��
	return 0;
}



//	--------------------------------------------------------------------
//	�v���C���[���̏������֐�
//	--------------------------------------------------------------------
void InitPlayer()
{
	int value = PLAYER_START_Y;
	PlayerGraphic = LoadGraph("player.png");


	//	�v���C���[�f�[�^�ɏ����l������
	Player.x = PLAYER_START_X;
	Player.y = PLAYER_START_Y;
	Player.width = PLAYER_WIDTH;
	Player.height = PLAYER_HEIGHT;
	Player.color = PLAYER_COLOR;
	Player.state = STATE_LIVE;

	//	�v���C���[�V���b�g�f�[�^�ɏ����l������
	//	�V���b�g�͑��݂��Ȃ���Ԃ���Q�[�����J�n�����̂�-1�ŏ���������
	//	���W�f�[�^�̓V���b�g��ł܂ł킩��Ȃ��̂ł܂�����Ȃ�
	PlayerShot.width = PLAYER_SHOT_WIDTH;
	PlayerShot.height = PLAYER_SHOT_HEIGHT;
	PlayerShot.color = PLAYER_COLOR;
	PlayerShot.state = STATE_NONE;

	return;
}

//	--------------------------------------------------------------------
//	�G���V���b�g�����܂ł̃^�C�~���O���v�Z����
//	--------------------------------------------------------------------
void CalcEnemyShotTiming()
{
	//	�V���b�g��ł܂ł̊Ԋu�����߂�
	//	ENEMY_SHOT_MIN_TIME����ENEMY_SHOT_MAX_TIME�܂ł̊Ԃ̗���������
	enemyShotTime = GetRand(ENEMY_SHOT_MAX_TIME - ENEMY_SHOT_MIN_TIME) + ENEMY_SHOT_MIN_TIME;
}

//	--------------------------------------------------------------------
//	�G���̏������֐�
//	--------------------------------------------------------------------
void InitEnemy()
{
	//	�G�f�[�^�̏����l������
	Enemy.x = GetRand(SCREEN_WIDTH - ENEMY_WIDTH);	//	���񃉃��_���ȏꏊ�ɏo��������
	Enemy.y = ENEMY_START_Y;
	Enemy.width = ENEMY_WIDTH;
	Enemy.height = ENEMY_HEIGHT;
	Enemy.color = ENEMY_COLOR;
	Enemy.state = STATE_LIVE;

	//	�G�V���b�g�f�[�^�ɏ����l������
	//	�V���b�g�͑��݂��Ȃ���Ԃ���Q�[�����J�n�����̂�-1�ŏ���������
	//	���W�f�[�^�̓V���b�g��ł܂ł킩��Ȃ��̂ł܂�����Ȃ�
	EnemyShot.width = ENEMY_SHOT_WIDTH;
	EnemyShot.height = ENEMY_SHOT_HEIGHT;
	EnemyShot.color = ENEMY_COLOR;
	EnemyShot.state = STATE_NONE;

	//	�G�̏����ړ��������v�Z
	//	���L�������Ă�������ֈړ�����
	if (Enemy.x < SCREEN_WIDTH / 2)
	{
		enemyMoveDirc = 1;
	}
	else
	{
		enemyMoveDirc = -1;
	}

	CalcEnemyShotTiming();

	return;
}

//	--------------------------------------------------------------------
//	�Q�[����������������֐�
//	--------------------------------------------------------------------
void InitGame()
{
	//	�v���C���[���̏�����
	InitPlayer();
	//	�G���̏�����
	InitEnemy();

	return;
}

//	--------------------------------------------------------------------
//	�I�u�W�F�N�g�`����s���֐�
//	--------------------------------------------------------------------
void DrawObject(OBJECT target)
{
	//	�I�u�W�F�N�g�����݂��Ȃ��Ƃ��͕`�悵�Ȃ��Ă悢
	if (target.state == STATE_NONE) return;

	//	�I�u�W�F�N�g�`��
	DrawBox(target.x,			//	X1���W
			target.y,			//	Y1���W
			target.x + target.width - 1,	//	X2���W�@���̈����͕����w�肷��̂ł͂Ȃ����W�����Ă���̂�-1���Ă���B
			target.y + target.height - 1,	//	Y2���W�@���̈����͕����w�肷��̂ł͂Ȃ����W�����Ă���̂�-1���Ă���B
			target.color,		//	�F
			TRUE);				//	�h��Ԃ����[�h(TRUE�œh��Ԃ��AFALSE)

	return;
}

//	--------------------------------------------------------------------
//	�w�i��h��Ԃ��֐�
//	--------------------------------------------------------------------
void DrawBG()
{
	RECT screenRect;	//	�`��G���A���i�[����ϐ�

	//	�`��G���A���擾����
	GetDrawArea(&screenRect);

	//	�w�i��h��Ԃ�
	DrawBox(screenRect.left,		//	X1���W
			screenRect.top,			//	Y1���W
			screenRect.right - 1,	//	X2���W�@���̈����͕����w�肷��̂ł͂Ȃ����W�����Ă���̂�-1���Ă���B
			screenRect.bottom - 1,	//	Y2���W�@���̈����͕����w�肷��̂ł͂Ȃ����W�����Ă���̂�-1���Ă���B
			BG_COLOR,				//	�h��Ԃ��F
			TRUE);					//	�h��Ԃ����[�h(TRUE�œh��Ԃ��AFALSE)

	return;
}


//	--------------------------------------------------------------------
//	objA��objB���Փ˂��Ă��邩�𒲂ׂ�
//	�����FOBJECT objA,objB...�Փ˔��肵�����I�u�W�F�N�g
//	�߂�l�F1...�Փ˂��Ă��� 0...�Փ˂��Ă��Ȃ�
//	--------------------------------------------------------------------
int IsHit(OBJECT objA, OBJECT objB)
{
	int x0 = objA.x;
	int y0 = objA.y;
	int x1 = objA.x + objA.width - 1;
	int y1 = objA.y + objA.height - 1;

	int x2 = objB.x;
	int y2 = objB.y;
	int x3 = objB.x + objB.width - 1;
	int y3 = objB.y + objB.height - 1;

	//	��̋�`����������̌`�ŏd�Ȃ��Ă��邩�H
	if (x0 < x3 && x2 < x1 && y0 < y3 && y2 < y1)
	{
		int z = 0;
		z++;
		//	�d�Ȃ��Ă���
		return 1;
	}


	//	�d�Ȃ��ĂȂ�����
	return 0;
}

//	--------------------------------------------------------------------
//	�e�𐶐�����֐�
//	�����Fint shooterID...�e�����̂̓v���C���[���G�Ȃ̂��̏��
//	�����FOBJECT shooter...�e���������I�u�W�F�N�g���
//	--------------------------------------------------------------------
void CreateShot(int shooterID, OBJECT shooter)
{
	//	�����̒e���G�̒e�Ȃ̂��ŏ�����ύX
	switch(shooterID)
	{
	case OBJECT_PLAYER:
		PlayerShot.state = STATE_LIVE;
		PlayerShot.x = shooter.x + shooter.width / 2 - PlayerShot.width / 2;	//	�v���C���[�̐^�񒆂��甭��
		PlayerShot.y = shooter.y - PlayerShot.height;
		break;
	case OBJECT_ENEMY:
		EnemyShot.state = STATE_LIVE;
		EnemyShot.x = shooter.x + shooter.width / 2 - EnemyShot.width / 2;	//	�G�̐^�񒆂��甭��
		EnemyShot.y = shooter.y + shooter.height;
		break;
	}
}

//	--------------------------------------------------------------------
//	�v���C���[�̈ړ��E�V���b�g�̔��ˏ������s��
//	--------------------------------------------------------------------
void ControllPlayer()
{
	//	�E�J�[�\����������Ă��邩�ǂ������`�F�b�N
	if (CheckHitKey(KEY_INPUT_RIGHT) == 1)
	{
		Player.x += PLAYER_MOVESPEED;
	}
	//	���J�[�\����������Ă��邩�ǂ������`�F�b�N
	if (CheckHitKey(KEY_INPUT_LEFT) == 1)
	{
		Player.x -= PLAYER_MOVESPEED;
	}

	//	�V���b�g���˃`�F�b�N
	if (CheckHitKey(KEY_INPUT_SPACE) == 1 && PlayerShot.state == STATE_NONE)
	{
		//	�e�𐶐�
		CreateShot(OBJECT_PLAYER, Player);
	}
}

//	--------------------------------------------------------------------
//	�v���C���[�̃V���b�g�̈ړ��֐�
//	--------------------------------------------------------------------
void ControllPlayerShot()
{
	//	�e���������G�Ƀq�b�g���Ă���Ȃ�ړ������͂����ɏI��
	if (PlayerShot.state != STATE_LIVE) return;

	//	�e�̈ړ�
	PlayerShot.y -= PLAYER_SHOT_SPEED;

	//	�e�͓G�Ƀq�b�g���Ă���H
	if (IsHit(PlayerShot, Enemy) == TRUE)
	{
		//	�e������
		PlayerShot.state = STATE_NONE;
		//	�G���܂��o��
		InitEnemy();
		return;
	}

	//	�e�͉�ʊO�ɏ������H
	if (PlayerShot.y < 0)
	{
		PlayerShot.state = STATE_NONE;
	}

	return;
}

//	--------------------------------------------------------------------
//	�G�̃V���b�g�̈ړ��֐�
//	--------------------------------------------------------------------
void ControllEnemyShot()
{
	//	�e���������v���C���[�Ƀq�b�g���Ă���Ȃ�ړ������͂����ɏI��
	if (EnemyShot.state != STATE_LIVE) return;

	//	�e�̈ړ�
	EnemyShot.y += ENEMY_SHOT_SPEED;

	//	�e�̓v���C���[�Ƀq�b�g���Ă���H
	if (IsHit(EnemyShot, Player) == TRUE)
	{
		//	�e������
		EnemyShot.state = STATE_NONE;
		//	�v���C���[�����S��Ԃɂ���
		Player.state = STATE_HIT;
		return;
	}

	//	�e�͉�ʊO�ɏ������H
	if (EnemyShot.y > SCREEN_HEIGHT)
	{
		EnemyShot.state = STATE_NONE;
	}

	return;
}


//	--------------------------------------------------------------------
//	�G�̈ړ��E�V���b�g�̔��ˏ������s��
//	--------------------------------------------------------------------
void ControllEnemy()
{
	//	�[�����܂ŗ��Ă�����ړ������𔽓]
	switch (enemyMoveDirc)
	{
	case -1:
		if (Enemy.x <= 0)
		{
			enemyMoveDirc = 1;
		}
		break;
	case 1:
		if (Enemy.x >= SCREEN_WIDTH - ENEMY_WIDTH)
		{
			enemyMoveDirc = -1;
		}
		break;
	}

	//	�ړ�
	Enemy.x += ENEMY_MOVESPEED * enemyMoveDirc;

	//	�V���b�g���˃`�F�b�N
	enemyShotTime--;
	if (enemyShotTime == 0)
	{
		//	�e�����ݏo���Ă��Ȃ��̂Ȃ�e���o��
		if (EnemyShot.state == STATE_NONE)
		{
			//	�e�𐶐�
			CreateShot(OBJECT_ENEMY, Enemy);
		}
		//	���ɒe�����Ԋu�����߂�
		CalcEnemyShotTiming();
	}
}


//	--------------------------------------------------------------------
//	�v���C���[�A�G�A�e�̈ړ��A�����蔻�菈�����Ă�
//	--------------------------------------------------------------------
void ControllGame()
{
	//	�v���C���[�̈ړ�����
	ControllPlayer();

	//	�v���C���[�̃V���b�g�̈ړ�
	ControllPlayerShot();

	//	�G�̈ړ�����
	ControllEnemy();

	//	�G�̒e�̈ړ�����
	ControllEnemyShot();

	return;
}

//	--------------------------------------------------------------------
//	�v���C���[�A�G�A�e�A�w�i�̕`�揈�����Ă�
//	--------------------------------------------------------------------
void DrawGame()
{
	// ��ʂ�������
	ClearDrawScreen();

	// �`����ʂ𗠂ɂ���
	SetDrawScreen( DX_SCREEN_BACK );

	//	�w�i�`��
	DrawBG();

	//	�v���C���[�̕`�揈��
	DrawObject(Player);
	//	�v���C���[�̒e�`��
	DrawObject(PlayerShot);

	//	�G�̕`��
	DrawObject(Enemy);
	//	�G�̒e�`��
	DrawObject(EnemyShot);

	//	����ʂɕ`�������̂�\��ʂɔ��f����
	ScreenFlip();

	return;
}


//	--------------------------------------------------------------------
//	�l�X�ȃQ�[�����������ĂԊ֐�
//	--------------------------------------------------------------------
void GameMain()
{
	//	�R���g���[��
	ControllGame();
	//	�`��
	DrawGame();

	return;
}

