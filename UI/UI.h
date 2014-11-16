#ifndef UI_CLASS_BASE
#define UI_CLASS_BASE

#include "../common.h"

#include "../UIBase.h"

extern struct MYSYSTEM sys;
extern struct GAMEDATA *gd;
extern struct VISUAL vis;

class UI_Hiroki_Metro : public UIBase
{
protected:
	int rtimer;
	int waitsecond;

	int enable_input;

	int infobar_height;
	int icon_size;
	int sparse_icon;
	int sparse_game;
	int w_num;

	int select_x, select_y, select_w, select_size;
	int move_x, s_x;

	int tnum;

	int sstimer;
	int istimer;

	int info_w, info_h, info_x, info_y;
	int img_x, img_y, img_w, img_h, cate_h;
	int info_num;
	int info_bt_size;

	int font_size;
public:
	UI_Hiroki_Metro( class TenLANBase *tenlanbase );
	virtual char *UIName( void );
	virtual int Init( void );
	virtual int BackGround( void );
	virtual int SelectScreen( int enable_input = 0 );
	virtual int InfoScreen( int enable_input = 0 );
	virtual int DrawMotionBox( int x, int y, int width, int height, double prog );
	virtual int View( void );
	virtual int Release( void );
	virtual int ReleaseTexture( void );
	virtual int ReloadTexture( void );

};

#endif