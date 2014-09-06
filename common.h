#ifndef TENLAN_COMMON_HEADER
#define TENLAN_COMMON_HEADER

#include <MikanDev.h>
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


//スクショの元サイズ(暫定)
#define G_X 640
#define G_Y 480

// セーブのフラグ
enum
{
	SAVE_SOUND_VOLUME,
	SAVE_PAD,
	SAVE_PAD2KEY_DISTANCE,
};

class TenLAN : public TenLANBase
{
protected:
	//ウィンドウサイズ
	int width;
	int height;

	// ゲームの数。
	int gamemax;

	// ゲームデータ。
	struct GAMEDATA *gamedata;

	// ゲームのモード。
	int gamemode;

	// ゲームタイマー。
	int timer;
	int maxvolume;

	// キーコンフィグ用。
	int deckey;
	int keys[ 6 ];
	int pad2keysleep;

	// OP用。
	int op_anime;
	int op_timer;
	int op_dis;
	int op_game;
	int op_deftime;
	double op_x, op_basex, op_lastx;

	// 修了処理用。
	int end;

	// 裏コマンド用の処理。
	int Command_( void );
public:
	TenLAN( void );

	// 使用可能メソッド。
	// オープニング。
	virtual int OP( void );
	// 音量調整。
	virtual int SetVolume( void );
	// キーコンフィグ画面
	virtual int KeyConfig( unsigned int padnum );
	// コマンド。
	virtual int Command( void );

	// システム情報の取得。
	virtual struct GAMEDATA * GetGameData( int gnumber );
	virtual int GetWidth( void );
	virtual int GetHeight( void );
	virtual int GetGameMax( void );
	virtual int GetGameMode( void );
	virtual int GetTimer( void );
	virtual int GetMaxVolume( void );
	virtual int GetEndMode();

	// セーブ系。
	int SetKeyConfig( unsigned int padnum, int save );
	int SaveConfig( int mode, int arg );
	int LoadConfig( void );

	// システムが使うメソッド。
	virtual int SetWindowSize( int w, int h );
	virtual int SetPad2Key( unsigned int padnum, int flag );
	virtual int SetGameMax( int max );
	virtual int SetGameData( struct GAMEDATA *gd );
	virtual int SetGameMode( int mode );
	virtual int SetTimer( int newtimer );
	virtual int AddTimer( void );
	virtual int SetMaxVolume( int newmaxvolume );
	virtual int SetEndMode( int newend );

	// 入力メソッド。
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
	//ゲームパッドの番号
	//キーボードの場合は-1
	int GAMEPAD;
	//ウィンドウサイズ
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


// サーバー検索
int SearchTenLanServer( void );
// サーバー検索画面
int SearchServerNow( void );

// プログラムの実行
int ExecProgram( int num );

// 入力
// キーボードのz
int InputAct_( void );
// キーボードのx
int InputCancel_( void );


#endif