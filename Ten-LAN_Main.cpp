//Mikan���C�u����
#include "common.h"
#include "UI\UI.h"

//Ten-LAN�֘A�̊֐��Ƃ��̒�`
#include "Ten-LAN.h"

#define TENLANVERSION "3.0"

unsigned long CATE_COLOR[ CATEGORY_MAX ] =
{
	0x00ff2828, // �A�N�V����
	0x00ff0091, // �V���[�e�B���O
	0x00dbae00, // �X�|�[�c
	0x00c2cb10, // ���[�X
	0x0016f000, // �p�Y��
	0x0000a411, // �e�[�u��
	0x00008f9a, // �V�~�����[�V����
	0x00004eff, // ���[���v���C���O
	0x00cf00ff, // �ΐ�
	0x00f400ff, // �~�j�Q�[��
	0x00808080  // ���̑�
};

int START_SS = 0;
char readfile[][30] = {
	"IMG_BACK_OP",
	"IMG_LOGO",
	"IMG_ITEM",
	"IMG_CATE_ICON",
	""
};

#define Free(r) { if(r) free(r); r = NULL; }

#define GAMEDIR    "Game"
#define SETTING    "data.ini"
#define SCREENSHOT "ss.png"

struct GAMEDATA *gd;

struct MYSYSTEM sys;

int ret;

int InitGamelist( void )
{
	HANDLE hdir;
	WIN32_FIND_DATA status;
	char filepath[ 1024  ] = "";
	char *buf,*tok, *str;
	int n = 0, msel = 0, r, w;
	struct stat fstat;

	sprintf_s( filepath, 1023, "%s\\*", GAMEDIR );
	gd = (GDATA *)calloc( 36, sizeof( GDATA ) );
	sys.tenlan->SetGameMax( 0 );
	if ( ( hdir = FindFirstFile( filepath, &status ) ) != INVALID_HANDLE_VALUE )
	{
		do
		{
			if ( ( status.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
			{
				//�f�B���N�g��
				if ( strcmp( status.cFileName, "." ) != 0 &&
				     strcmp( status.cFileName, ".." ) != 0)
				{
					sprintf_s( filepath, 1023, "%s\\%s\\%s", GAMEDIR, status.cFileName, SETTING );
					stat( filepath, &fstat );
					if ( MikanFile->Open( 0, filepath, "r" ) >= 0 )
					{
						buf = ( char * )calloc( 2048, sizeof( char ) );
						// ������
						strcpy_s( gd[ msel ].exe, 1024, "" );
						strcpy_s( gd[ msel ].title, 2048, "NO TITLE" );
						strcpy_s( gd[ msel ].text, 2048, "" );
						gd[ msel ].date = 0;
						gd[ msel ].pad2key = 1;
						gd[ msel ].cnum = 0;
						gd[ msel ].category[ 0 ] = 0;
						while ( MikanFile->ReadLine( 0, buf, 512 ) )
						{
							str = strtok_s( buf, "=", &tok);
							if ( strcmp( str, "exe" ) == 0 )
							{
								r = w = 0;
								while ( tok[ r ] != '\0' )
								{
									gd[ msel ].exe[ w++ ] = tok[ r ];
									if ( tok[ r ] == '\n' )
									{
										break;
									}
									++r;
								}
								gd[ msel ].exe[ w - 1 ] = '\0';
								++w;
							} else if ( strcmp( str, "title" ) == 0 )
							{
								strcpy_s( gd[ msel ].title, 2048, strtok_s( NULL, "=", &tok) );
							} else if ( strcmp( str, "text" ) == 0 )
							{
								strcpy_s( gd[ msel ].text, 2048, strtok_s( NULL, "=", &tok) );
							} else if ( strcmp( str, "date" ) == 0 )
							{
								gd[ msel ].date = atoi( strtok_s( NULL, "=", &tok) );
							} else if ( strcmp( str, "pad2key" ) == 0 )
							{
								gd[ msel ].pad2key = atoi( strtok_s( NULL, "=", &tok) );
							} else if ( strcmp( str, "cate" ) == 0 )
							{
								// �J�e�S���ԍ����擾�B
								w = 0;
								for ( r = 0 ; tok[ r ] != '\0' ; ++r )
								{
									if ( '0' <= tok[ r ] && tok[ r ] <= '9' )
									{
										gd[ msel ].category[ gd[ msel ].cnum ] = gd[ msel ].category[ gd[ msel ].cnum ] * 10 + tok[ r ] - '0';
									} else if ( tok[ r ] == ',' )
									{
										++gd[ msel ].cnum;
									}
								}
								++gd[ msel ].cnum;
							}
						}
						MikanFile->Close( 0 );
						//fclose( fp );

						gd[ msel ].num = atoi( status.cFileName );
						gd[ msel ].txnum = START_SS + msel;
						GetCurrentDirectory( 512, gd[ msel ].txfile );
						sprintf_s( gd[ msel ].txfile, 512, "%s\\%s\\%s\\%s", gd[ msel ].txfile, GAMEDIR, status.cFileName, SCREENSHOT );
						MikanDraw->CreateTexture( gd[ msel ].txnum, gd[ msel ].txfile, TRC_NONE );
						Free( buf );
						++msel;
						//++sys.gamemax;
						sys.tenlan->SetGameMax( sys.tenlan->GetGameMax() + 1 );
					}
				}
			}
		} while ( FindNextFile( hdir, &status ) );
		FindClose( hdir );
	}
	sys.tenlan->SetGameData( gd );

	return 0;
}

int Init( void )
{
	//�E�B���h�E�n���h��
	HWND myhwnd = NULL;
	//RECT�\����
	RECT myrect;

	//�Q�[���p�b�h�ԍ��̏�����
	sys.GAMEPAD = -1;

	//�f�X�N�g�b�v�̃E�B���h�E�n���h���擾
	myhwnd = GetDesktopWindow();
	if ( myhwnd && GetClientRect( myhwnd, &myrect ) )
	{
		//�f�X�N�g�b�v�̑傫���̎擾
		//sys.width  = myrect.right;
		//sys.height = myrect.bottom;
		sys.tenlan->SetWindowSize( myrect.right, myrect.bottom );
	}

	_MikanWindow->SetWindow( WT_NORESIZEFULLSCREEN );
	return 0;
}

//�E�B���h�E�̃T�C�Y���Z�b�g������
//�����`���[���A��ɍs��
/*int ResetWindow(void)
{
_MikanWindow->SetPositionXY( 0, 0 );
_MikanWindow->SetWindowSize( sys.width, sys.height );
_MikanWindow->SetWindow();
return 0;
}*/

HWND GetWindowHandle ( unsigned long TargetID)
{
	HWND hWnd = GetTopWindow(NULL);
	DWORD ProcessID;
	do
	{
		if ( GetWindowLong( hWnd, GWL_HWNDPARENT) != 0 || !IsWindowVisible( hWnd))
		{
			continue;
		}
		GetWindowThreadProcessId(hWnd,&ProcessID);
		if ( TargetID == ProcessID )
		{
			return hWnd;
		}
	} while((hWnd = GetNextWindow( hWnd, GW_HWNDNEXT)) != NULL);

	return NULL;
}

// �^�X�N�o�[�̉B��
int HideTaskbar( void )
{
	OSVERSIONINFO OSver;
	HWND hWnd;

	OSver.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );
	GetVersionEx( &OSver );

	hWnd = FindWindow( "Shell_TrayWnd", NULL );

	if ( ShowWindow(hWnd, SW_HIDE) )
	{
		if ( (OSver.dwMajorVersion == 6 && OSver.dwMinorVersion >= 1) || OSver.dwMajorVersion > 6 )
		{
			hWnd = FindWindowEx( NULL, NULL, "Button", "�X�^�[�g" );
		}else
		{
			hWnd = FindWindow( "Button", "Start" );
		}
		ShowWindow( hWnd, SW_HIDE );
		return 0;
	}
	return 1;
}

// �^�X�N�o�[�̕���
int RestoreTaskbar( void )
{
	OSVERSIONINFO OSver;
	HWND hWnd;

	OSver.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );
	GetVersionEx( &OSver );

	hWnd = FindWindow( "Shell_TrayWnd", NULL );
	ShowWindow( hWnd, SW_RESTORE );

	if( ( OSver.dwMajorVersion == 6 && OSver.dwMinorVersion >= 1 ) || OSver.dwMajorVersion > 6 )
	{
		hWnd = FindWindowEx( NULL, NULL, "Button", "�X�^�[�g" );
	}else
	{
		hWnd = FindWindow( "Button", "Start" );
	}
	ShowWindow( hWnd, SW_RESTORE );
	return 0;
}

//�Q�[���̎��s
int ExecProgram( int num )
{
	char *exe, dir[512];
	DWORD startpid, errcode;
	HWND hw;

	sys.gamewin = NULL;
	// ���[�h�ύX
	//sys.modeflag = MF_PLAY;
	sys.tenlan->SetGameMode( MF_PLAY );

	ZeroMemory( &( sys.si ), sizeof( sys.si ) );
	sys.si.cb = sizeof( sys.si );

	sys.process = 0;

	//sprintf_s( filepath, 2047, "%s\\%04d", GAMEDIR, gd[num].num );
	exe = gd[ num ].exe + strlen( gd[ nu m].exe );
	while ( *exe != '\\' )
	{
		--exe;
	}
	//  *exe = '\0';
	strncpy_s( dir, 512, gd[ num ].exe, strlen( gd[ num ].exe ) - strlen( exe ) + 1 );
	if ( SetCurrentDirectory( dir ) )
	{
		//  *exe = '\\';
		++exe;

		if ( gd[ num ].pad2key )
		{
			sys.tenlan->SetPad2Key( 0, gd[ num ].pad2key );
		}
		_MikanInput->UpdateKeyInput();
		_MikanInput->UpdateKeyInput();

		//memset( &(sys.pi), 0, sizeof( PROCESS_INFORMATION ) );

		if ( CreateProcess(NULL,exe,NULL,NULL,FALSE,NORMAL_PRIORITY_CLASS,NULL,NULL,&(sys.si),&(sys.pi)) )
		{
			//sys.gamewin = GetWindowHandle( sys.pi.dwProcessId );
			CountUp( gd[ num ].num );
			startpid = sys.pi.dwProcessId;

			// �^�C�����O����̂ł����Ńe�N�X�`�����
			//ReleaseTexture();

			do
			{
				hw = GetWindowHandle( startpid );
			} while( hw == NULL );
			sys.gamewin = hw;
			SetForegroundWindow( sys.gamewin );
			_MikanInput->AttachPad2Key( sys.gamewin, FALSE );
			if ( gd[ num ].pad2key )
			{
				_MikanInput->AttachPad2Key( sys.gamewin );
				//hw = MikanWindow->GetWindowHandle();
				SetForegroundWindow( sys.gamewin );
			}
			sys.MODE = 1;
		} else
		{
			errcode = GetLastError();
			GetCurrentDirectory( 512, dir );
			sys.process = 1;
			//sys.modeflag = MF_OKAY;
			sys.tenlan->SetGameMode( MF_OKAY );
		}
	}
	return 0;
}

int CheckEndProcess(void)
{
	unsigned long ExitCode;

	if ( sys.process == 0 )
	{
		GetExitCodeProcess( sys.pi.hProcess, &ExitCode );

		if ( ExitCode == STILL_ACTIVE )
		{
			return 0;
		}
		SetForegroundWindow( MikanWindow->GetWindowHandle() );
		CloseHandle( sys.pi.hThread );
		CloseHandle( sys.pi.hProcess );
	} else
	{
		SetForegroundWindow( MikanWindow->GetWindowHandle() );
	}
	SetCurrentDirectory( sys.CurDir );


	return 1;
}

int GameEnd( void )
{
	if ( sys.tenlan->InputESC() > 60 )
	{
		//sys.end = 1;
		sys.tenlan->SetEndMode( 1 );
	}
	if ( sys.tenlan->GetEndMode() )//sys.end )
	{
		if ( InputAct_() == 1 )
		{
			if ( sys.tenlan->GetEndMode() == 2 )//sys.end == 2 )
			{
				//�Q�[���I��
				return 1;
			}
			//sys.end = 0;
			sys.tenlan->SetEndMode( 0 );
		}
		if ( sys.tenlan->InputRight() % 20 == 1 || 
		     sys.tenlan->InputLeft()  % 20 == 1 ||
		     sys.tenlan->InputUp()    % 20 == 1 ||
		     sys.tenlan->InputDown()  % 20 == 1 )
		{
			//sys.end = ( sys.end + 2 ) % 2 + 1;
			sys.tenlan->SetEndMode( ( sys.tenlan->GetEndMode() + 2 ) % 2 + 1 );
		}
		MikanDraw->DrawTextureC( IMG_ITEM, sys.tenlan->GetWidth() / 2, sys.tenlan->GetHeight() / 2, 0, 150, 450, 280 );
		MikanDraw->DrawTextureC( IMG_ITEM, sys.tenlan->GetWidth() / 2 + (sys.tenlan->GetEndMode()==1?105:-105), sys.tenlan->GetHeight() / 2 + 90,
			250, 0 + (sys.tenlan->GetEndMode()==1?60:0), 200, 60 );
		/*    MikanDraw->DrawTextureC( IMG_ITEM, sys.width / 2, sys.height / 2, 20, 490, 680, 400 );
		MikanDraw->DrawTextureC( IMG_ITEM, sys.width / 2 + (sys.end==1?160:-160), sys.height / 2 + 10 + 120,
		710, 490 + (sys.end==1?80:0), 300, 80 );*/
	}

	return 0;
}

int AfterDeviceLost( void )
{
	// �f�o�C�X���X�g���A��̏���

	return 0;
}

int PrintVersion( void )
{
#ifdef _DEBUG
	char buf[ 256 ];
	sprintf_s( buf, 256, "Ten-LAN ver %s. UI:%s", TENLANVERSION, sys.ui->UIName() );
	//MikanDraw->Printf( 1, sys.width - 5 * strlen( buf ) - 5, sys.height - 15, buf );
#endif
	return 0;
}

char WTIT[] = TITLE;
HWND wnd;

//�E�B���h�E�����O��1�x�������s
void SystemInit( void )
{
	char a[] = MikanVersion;

	//    char *exe, dir[512];
	DWORD startpid, errcode;
	PROCESS_INFORMATION pi;
	STARTUPINFO si;


	ZeroMemory( &( si ), sizeof( si ) );
	si.cb=sizeof( si );

	if ( CreateProcess( NULL, "./blackwindow.exe", NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &(si), &(pi) ) )
	{
		Sleep( 1000 );
		startpid = pi.dwProcessId;
		do
		{
			wnd = GetWindowHandle( startpid );

		}while( wnd == NULL );

		/*    _MikanInput->AttachPad2Key( GetWindowHandle( pi.dwProcessId ) );
		_MikanInput->SendKey( VK_SHIFT, 1 );
		_MikanInput->SendKey( VK_SHIFT, 0 );*/

		//    SendMessage ( wnd, WM_CLOSE, 0, 0 );
	} else
	{
		errcode = GetLastError();
		errcode = 0;
	}

	sys.tenlan = new TenLAN();

	Init();

	MikanWindow->SetWindowIcon( "ICON_32" );
	MikanWindow->SetWindowName( WTIT );
	MikanSystem->SetInactiveWindow( 0 );

	_MikanSystem->SetAfterDeviceLostFunction( AfterDeviceLost );
	ShowCursor( 0 );
}

//DirectX���������1�x�������s
void UserInit( void )
{
	MikanDraw->CreateFont( 1, 10, 0xff000000 );
	MikanDraw->CreateFont( 2, 30, 0xffffffff );

	_MikanSystem->SetPermitScreenSaver( 0 );

	//sys.maxvolume = 100;
	sys.tenlan->SetMaxVolume( 100 );

	do
	{
		MikanDraw->CreateTexture( START_SS, NULL, readfile[ START_SS ] );
	} while( *readfile[ ++START_SS ] );
	START_SS = 10;
	sys.MODE = 0;
	//sys.end = 0;

	InitGamelist();

	GetCurrentDirectory( 1023, sys.CurDir );

	//sys.timer = 0;
	sys.tenlan->SetTimer( 0 );
	if( MikanInput->GetPadMount() > 0 )
	{
		sys.GAMEPAD = 0;
	}

	HideTaskbar();

	// UI�͂����Ŏw�肷��B
	// UI��UIBase�N���X���p�������N���X�ł���Ή��ł��ǂ��B
	sys.ui = new UI_Hiroki_Metro( sys.tenlan );//new UI_Hiroki();//new UI_Tour();
	sys.ui->Init();

	sys.tenlan->SetGameMode( MF_TITLE );

	sys.tenlan->LoadConfig();

	MikanSound->Load( 0, NULL, "BGM_TITLE" );
	MikanSound->SetVolume( 0, sys.tenlan->GetMaxVolume() );
	MikanSound->Play( 0, true );

	sys.searchserver = 0;
	MikanSystem->CreateLock( SERVER_LOCK );
	SearchTenLanServer();

	sys.tenlan->SetPad2Key( 0, 1 );

	SetForegroundWindow( MikanWindow->GetWindowHandle() );
}

//1�b�Ԃ�60����s�����
int MainLoop( void )
{
	MikanDraw->ClearScreen( 0xffffffff );

	if( sys.MODE )
	{
		if( CheckEndProcess() )
		{
			Sleep( 100 );
			sys.MODE = 0;
			//ReloadTexture();
			//ResetWindow();
			//_MikanDraw->SetScreenSize( sys.width, sys.height );
			_MikanDraw->SetScreenSize( sys.tenlan->GetWidth(), sys.tenlan->GetHeight() );
			// Pad2Key�̃o�b�t�@���J���ƌ������ADown����Up���ĂȂ����Ԃ�h���B
			_MikanInput->UpdateKeyInput();
			// Ten-LAN��Pad2Key�L���Ȃ̂Ō��ɖ߂��B
			sys.tenlan->SetPad2Key( 0, 1 );
			sys.ui->Init();
		}else if( sys.gamewin != NULL )
		{
			SetForegroundWindow( sys.gamewin );
		}
		sys.ui->View();

	}else
	{
		if ( sys.tenlan->GetGameMax() )
		{
			/*if( InputAct()>0)
			{
			sys.modeflag=MF_PLAY;
			ExecProgram( 0 );
			}*/
			sys.ui->View();

			if( sys.tenlan->GetGameMode() <= MF_SELECT )
			{
				//SetForegroundWindow( MikanWindow->GetWindowHandle() );
			}else if( sys.gamewin != NULL )
			{
				SetForegroundWindow( sys.gamewin );
			}

		}else
		{
			if( sys.tenlan->InputAct() == 1 )
			{
				if( sys.error == 0 )
				{
					//�Q�[���I��
					return 1;
				}
			}
			if( sys.tenlan->InputRight() % 20 == 1 || 
				sys.tenlan->InputLeft()  % 20 == 1 ||
				sys.tenlan->InputUp()    % 20 == 1 ||
				sys.tenlan->InputDown()  % 20 == 1 )
			{
				sys.error = ( sys.error + 1 ) % 2;
			}
			sys.ui->BackGround();
			sys.tenlan->AddTimer();
			MikanDraw->DrawTextureScalingC( IMG_LOGO, sys.tenlan->GetWidth() / 2, 100, 0, 0, 1024, 160, (double)sys.tenlan->GetHeight() / 1024 * 0.7 );
			MikanDraw->DrawTextureC( IMG_ITEM, sys.tenlan->GetWidth() / 2, sys.tenlan->GetHeight() / 2, 0, 450, 450, 420 );
			MikanDraw->DrawTextureC( IMG_ITEM, sys.tenlan->GetWidth() / 2 + (sys.error?105:-105), sys.tenlan->GetHeight() / 2 + 160,
				250, 0 + (sys.error?60:0), 200, 60 );
		}
	}

	PrintVersion();
	return GameEnd();
}

//�Ō�Ɉ�x�������s�����
void CleanUp( void )
{
	SendMessage ( wnd, WM_CLOSE, 0, 0 );

	RestoreTaskbar();
	sys.ui->Release();
	delete( sys.tenlan );
	delete( sys.ui );
	free( gd );
	RestoreTaskbar();
}

