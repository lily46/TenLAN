#ifndef CLASS_UIBASE
#define CLASS_UIBASE class UIBase

//�Q�[���̃t���O
enum
{
	MF_TITLE,     // �^�C�g��
	MF_KEYCONFIG, // �L�[�R���t�B�O
	MF_SOUND,     // ��
	MF_SELECT,    // �Q�[���I�����
	MF_OKAY,      // �m�F��ʋy�ь���
	MF_PLAY,      // �Q�[�������쒆(Ten-LAN�͔w�i�`��or�^����)
};

// �J�e�S�����B
#define CATEGORY_MAX 11

// �J�e�S���̐F�B
extern unsigned long CATE_COLOR[ CATEGORY_MAX ];

// �Q�[���f�[�^�B
struct GAMEDATA
{
	int num;                       // �Q�[���ԍ��B
	char exe[1024];                // �N���p�X�B
	char title[2048];              // �Q�[�����B
	char text[2048];               // �Љ�B
	unsigned int date;             // ���t�B
	int pad2key;                   // Pad2Key���L�����ǂ����B
	int txnum;                     // �����摜�̃e�N�X�`���ԍ��B
	char txfile[ 512 ];            // �e�N�X�`���̃t�@�C���p�X�B
	int cnum;                      //
	char category[ CATEGORY_MAX ]; // �ǂ̃J�e�S���ɏ������Ă��邩�B
};

// ���\�[�X���݂̂��m�点
// �t�H���g�ԍ�0-9�̓V�X�e��Ten-LAN�{�̂Ŋm�ۂ��܂��B
// �e�N�X�`���͈ȉ��̂��̂�W���Ŏg���܂��B
//  * 0�� - ��{�w�i
//  * 1�� - ���S
//  * 2�� - �V�X�e���Ŏg���W���A�C�e��
// 3�Ԉȍ~9�Ԃ܂ł̃e�N�X�`�����g���Ă��������B

// UI�����Ƃ��Ɍp������N���X�B
class UIBase
{
protected:
	// Ten-LAN�̃f�[�^�Ǘ��Ȃǂ��s���N���X�̃C���X�^���X�B
	class TenLANBase *tenlan;
public:

	// �V�X�e�������g�������Ȃ̂ŁA��΂ɕύX���Ȃ����ƁB
	virtual int SetTenLAN( class TenLANBase *tenlanobj ){ tenlan = tenlanobj; return 0; }

	// UI�̖��O��Ԃ��B
	// �����ꕔ�ŕ`�悷��Ƃ��Ɏg�������B
	virtual char *UIName( void ){ return ""; }
	// �����������B
	virtual int Init( void ){ return 0; };
	// ���ʂ̔w�i�`��B
	virtual int BackGround( void ) = 0;
	// �I�ԉ�ʁB
	virtual int View( void ) = 0;
	// �J�������B
	virtual int Release( void ){ return 0; };
	// �e�N�X�`���̊J�������B
	virtual int ReleaseTexture( void ){ return 0; };
	// �e�N�X�`���̍ēǂݍ��ݏ����B
	virtual int ReloadTexture( void ){ return 0; };
};

// Ten-LAN�̐���p�N���X�B
// ��Ŏ󂯎���Ďg����B
class TenLANBase
{
public:

	// �g�p�\���\�b�h�B

	// �I�[�v�j���O�B
	virtual int OP( void ) = 0;
	// ���ʒ����B
	virtual int SetVolume( void ) = 0;
	// �L�[�R���t�B�O���
	virtual int KeyConfig( unsigned int padnum ) = 0;
	// �R�}���h�B
	virtual int Command( void ) = 0;

	// �V�X�e�����̎擾�B

	virtual struct GAMEDATA * GetGameData( int gnumber ) = 0;
	virtual int GetWidth( void ) = 0;
	virtual int GetHeight( void ) = 0;
	virtual int GetGameMax( void ) = 0;
	virtual int GetGameMode( void ) = 0;
	virtual int GetTimer( void ) = 0;
	virtual int GetMaxVolume( void ) = 0;

	// �V�X�e�����̐ݒ�B
	virtual int SetGameMode( int mode ) = 0;

	// ���̓��\�b�h�B
	virtual int InputUp( void ) = 0;
	virtual int InputDown( void ) = 0;
	virtual int InputRight( void ) = 0;
	virtual int InputLeft( void ) = 0;
	virtual int InputAct( void ) = 0;
	virtual int InputCancel( void ) = 0;
	virtual int InputESC( void ) = 0;
};

#endif
