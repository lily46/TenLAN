#ifndef TENLAN_COMMON_HEADER
#define TENLAN_COMMON_HEADER

#include <Mikan.h>
#include <sys/stat.h>
#include <stdio.h>
#include <time.h>

#include "UIBase.h"

#ifdef _DEBUG
#define TITLE "Ten-LAN v3.0"
#else
#define TITLE "Ten-LAN"
#endif

#define CONFIG_FILE "config.txt"
#define LOG_FILE    "log.txt"
#define SERVER_LOCK 2
#define SERVER_SEARCH_COUNT 3
#define SERVER_SEARCH_TIMER 3 * 60
#define THREAD_SEARCH 2
#define NET_SEARCH 2



enum{ IMG_OP,IMG_LOGO, IMG_ITEM, IMG_MAX };

typedef struct GAMEDATA GDATA;


//�X�N�V���̌��T�C�Y(�b��)
#define G_X 640
#define G_Y 480

// �Z�[�u�̃t���O
enum
{
	SAVE_SOUND_VOLUME,
	SAVE_PAD,
	SAVE_PAD2KEY_DISTANCE,
};

class TenLAN : public TenLANBase
{
protected:
	//�E�B���h�E�T�C�Y
	int width;
	int height;

	// �Q�[���̐��B
	int gamemax;

	// �Q�[���f�[�^�B
	struct GAMEDATA *gamedata;

	// �Q�[���̃��[�h�B
	int gamemode;

	// �Q�[���^�C�}�[�B
	int timer;
	int maxvolume;

	// �L�[�R���t�B�O�p�B
	int deckey;
	int keys[ 6 ];
	int pad2keysleep;

	// OP�p�B
	int op_anime;
	int op_timer;
	int op_dis;
	int op_game;
	int op_deftime;
	double op_x, op_basex, op_lastx;

	// �C�������p�B
	int end;

	// ���R�}���h�p�̏����B
	int Command_( void );
public:
	TenLAN( void );

	// �g�p�\���\�b�h�B
	// �I�[�v�j���O�B
	virtual int OP( void );
	// ���ʒ����B
	virtual int SetVolume( void );
	// �L�[�R���t�B�O���
	virtual int KeyConfig( unsigned int padnum );
	// �R�}���h�B
	virtual int Command( void );

	// �V�X�e�����̎擾�B
	virtual struct GAMEDATA * GetGameData( int gnumber );
	virtual int GetWidth( void );
	virtual int GetHeight( void );
	virtual int GetGameMax( void );
	virtual int GetGameMode( void );
	virtual int GetTimer( void );
	virtual int GetMaxVolume( void );
	virtual int GetEndMode();

	// �Z�[�u�n�B
	int SetKeyConfig( unsigned int padnum, int save );
	int SaveConfig( int mode, int arg );
	int LoadConfig( void );

	// �V�X�e�����g�����\�b�h�B
	virtual int SetWindowSize( int w, int h );
	virtual int SetPad2Key( unsigned int padnum, int flag );
	virtual int SetGameMax( int max );
	virtual int SetGameData( struct GAMEDATA *gd );
	virtual int SetGameMode( int mode );
	virtual int SetTimer( int newtimer );
	virtual int AddTimer( void );
	virtual int SetMaxVolume( int newmaxvolume );
	virtual int SetEndMode( int newend );

	// ���̓��\�b�h�B
	virtual int InputUp( void );
	virtual int InputDown( void );
	virtual int InputRight( void );
	virtual int InputLeft( void );
	virtual int InputAct( void );
	virtual int InputCancel( void );
	virtual int InputESC( void );
};

struct MYSYSTEM
{
	//int timer;
	//�Q�[���p�b�h�̔ԍ�
	//�L�[�{�[�h�̏ꍇ��-1
	int GAMEPAD;
	//�E�B���h�E�T�C�Y
	//int width;
	//int height;
	int MODE;
	//int end;
	int error;
	char CurDir[1024];
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	int process;
	HWND gamewin;
	//int gamemax;
	//int modeflag;
	class UIBase *ui;
	class TenLAN *tenlan;
	//int maxvolume;
	int searchserver;
};

class LOAD_UI_DLL
{
protected:
	HMODULE module;
public:
	LOAD_UI_DLL( void );
	~LOAD_UI_DLL( void );
	int Init( void );
	int Load( const char *dll );
	int Release( void );
};

int CountUp( int num );

int ReleaseTexture( void );
int ReloadTexture( void );


// �T�[�o�[����
int SearchTenLanServer( void );
// �T�[�o�[�������
int SearchServerNow( void );

// �v���O�����̎��s
int ExecProgram( int num );

// ����
// �L�[�{�[�h��z
int InputAct_( void );
// �L�[�{�[�h��x
int InputCancel_( void );


#endif