#include <Window.h>

// �^�����Ŗ��Q�ȉB���p�E�B���h�E�𐶐�����B

class Window *win = NULL;

int loop = 1;
int width = 10, height = 10;

int FullScreen( void )
{
  //RECT�\����
  RECT myrect;
  
  HWND myhwnd = NULL;

  //�f�X�N�g�b�v�̃E�B���h�E�n���h���擾
  myhwnd = GetDesktopWindow();
  //�f�X�N�g�b�v�̃T�C�Y�Ȃǂ��擾
  if( myhwnd && GetClientRect( myhwnd, &myrect ) )
  {
    //�ʒu�͌��_�ɐݒ�
    win->SetPositionXY( 0, 0 );
    //�T�C�Y�̓f�X�N�g�b�v�Ɠ����T�C�Y�ɐݒ�
    win->SetWindowSize( myrect.right, myrect.bottom );
  }
  width = myrect.right;
  height = myrect.bottom;

  //�E�B���h�E�X�^�C���̕ύX
  SetWindowLong( win->GetWindowHandle(), GWL_STYLE, win->GetWindowStyle() );

  SetWindowPos( win->GetWindowHandle(), HWND_TOP, 0, 0, myrect.right, myrect.bottom, SWP_SHOWWINDOW );
  return 0;
}

LRESULT CALLBACK WindowProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT ps;

  switch( uMsg )
  {
  case WM_KEYDOWN:
  case WM_KEYUP:
    if ( wParam == VK_SHIFT )
    {
      // ���[�v���I������B
      loop = 0;
      if( win )
      {
        delete( win );
        win = NULL;
      }
    }
    return 0;
  case WM_ACTIVATE:
    break;
  case WM_CLOSE://�E�B���h�E��������
  case WM_DESTROY://�E�B���h�E���j�����ꂽ��
    if( win )
    {
      delete( win );
      win = NULL;
    }
    loop = 0;
    return 0;
  case WM_GETMINMAXINFO:
    // ���T�C�Y�Ȃǂ���΃t���X�N���[���ɂ���悤�ɂ���B
    // �X�N���[����S�Ė��ߐs�����Ƃ����Ӗ��ł����āA�f�B�X�v���C�̉𑜓x�͕ς��Ȃ��B
    FullScreen();
    break;
  case WM_PAINT:
    // �E�B���h�E���ĕ`�悳��鎞�^�����ɂ���B
    hDC = BeginPaint( hWnd, &ps );
    SelectObject(hDC , GetStockObject(BLACK_BRUSH));
		Rectangle( hDC, 0, 0, width, height );
		EndPaint( hWnd, &ps );
    return 0;
  case WM_CREATE://�E�B���h�E�����ꂽ��
    //������ӂŃX�N���[���I����ʂ��o���̂�����B
    //�f�o�C�X�R���e�L�X�g�̎擾
    win->wd.Screen = CreateCompatibleDC( GetDC( hWnd ) );
    win->wd.WindowHandle = hWnd;
    break;
  case WM_SYSCOMMAND:
    if ( wParam == SC_SCREENSAVE )
    {
      // �X�N���[���Z�[�o�[�̋N����j�~�B
      return 1;
    }
  }
  //���ꂪ�Ȃ��ƃE�B���h�E�������ł��Ȃ��B
  return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
  tagMSG msg;

  //�N���X��錾
  win = new Window();

  if( win )
  {
    win->InitWindowClassEx( hInstance );
    win->SetWindowProc( ::WindowProc );

    if( win->RegistClassEx() )
    {
      //�o�^���s
      return 1;
    }

    if( win->MakeWindow( nCmdShow ) || win->wd.Screen == NULL )
    {
      //�E�B���h�E�����Ȃ������̂ŏI��
      return 2;
    }

    FullScreen();

    while( loop )
    {
      if( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
      {
        TranslateMessage( &msg );
        DispatchMessage( &msg );
      }
      Sleep( 200 );
    }

  }

  return 1;
}