#include "Main.h"
#include "CGame.h"

//�X���b�h�֐�.
DWORD WINAPI ThreadFunc( LPVOID vdParam )
{
	//�ϐ��錾
	CGame* pGame = nullptr;	//�|�C���^������
	pGame = new CGame((GameWindow*)vdParam);	//�Q�[���N���X�C���X�^���X

	DWORD frames = 0;		//�t���[����.
	DWORD beforeTime = 0;	//�O�̎���.
	char MsgFPS[128] = "";

	//���Ԃ̎擾.
	beforeTime = timeGetTime();	//���݂̎��Ԃ��擾.

//----------------------------------------------------------
//	�N������[WM_CREATE].
//----------------------------------------------------------
	
	if (pGame->Create() == false) return 0;

	//�E�B���h�E��������܂Ŗ������[�v.
	while(pGame->IsWindowClosed() == FALSE)
	{
		DWORD nowTime = 0;		//���݂̎���.
		DWORD progress = 0;		//�o�ߎ���.
		DWORD idealTime = 0;	//���z����.

		nowTime = timeGetTime();//���݂̎��Ԃ��擾.

		progress = nowTime - beforeTime;	//�o�ߎ���.

		//���z���Ԃ̎Z�o.
		// ( ���݂̃t���[���� �~ ( 1000 �� FPS�l ) ).
		idealTime = (DWORD)( frames * ( 1000.0F / pGame->GetFPS()) );
		
		//----------------------------------------------------------
		//  �Q�[������[WM_TIMER].
		//----------------------------------------------------------
		
		pGame->Update();

		//----------------------------------------------------------
		//	�`�揈��[WM_PAINT]
		//----------------------------------------------------------
		
		pGame->Draw();

		SetBkMode(pGame->GetScreenDC(), TRANSPARENT); // No background fill
		//FPS�̕\��.
		TextOut(pGame->GetScreenDC(),
			400, 0, MsgFPS, lstrlen( MsgFPS ) );

		//�`��̎��ԑ҂�.
		if( idealTime > progress ){
			Sleep( idealTime - progress );	//���ԑ҂�.
		}

		//�ĕ`��.
		InvalidateRect(pGame->GetWnd(), nullptr, FALSE);

		if( progress >= 1000 )	//1�b�o��.
		{
			wsprintf( MsgFPS, "FPS[%03d]", frames );
			beforeTime = nowTime;	//���݂̎��ԂɍX�V.
			frames = 0;
		}
		frames++;	//�t���[����UP.
	}

	//----------------------------------------------------------
	//	�������[WM_DESTROY].
	//----------------------------------------------------------
	
	pGame->Destroy();

	//�s�v�ɂȂ����̂ō폜
	if (pGame != nullptr) {
		delete pGame;
		pGame = nullptr;
	}

	//�X���b�h�I���ʒm.
	ExitThread( 0 );

	return TRUE;
}

//�E�B���h�E�֐�.
LRESULT CALLBACK WindowProc(
	HWND hWnd,		//�E�B���h�E�n���h��.
	UINT uMsg,		//���b�Z�[�W.
	WPARAM wParam,	//���b�Z�[�W�ɑΉ�������.
	LPARAM lParam )	//���b�Z�[�W�ɑΉ�������.
{
	HDC hDC;			//�f�o�C�X�R���e�L�X�g.
	PAINTSTRUCT ps;		//�`��\����.

	//----------------------------------------------------------
	//	�X���b�h�֘A.
	//----------------------------------------------------------
	static GameWindow gameWnd;	//�Q�[���E�B���h�E�\����.
	static HANDLE hThread;		//�X���b�h�n���h��.
	DWORD dwID;		//�X���b�hID.
	HBITMAP hBmp;	//�r�b�g�}�b�v�n���h��.

	//���b�Z�[�W�ʂ̏���.
	switch( uMsg ){
	case WM_DESTROY:	//�E�B���h�E���j�����ꂽ��.

		//�A�v���P�[�V�����̏I����Windows�ɒʒm����.
		PostQuitMessage( 0 );
		return 0;

	case WM_CREATE:		//�E�B���h�E����������鎞.
		//----------------------------------------------------------
		//	�X���b�h�̐ݒ�.
		//----------------------------------------------------------
		//�X���b�h�ɓn���l��ݒ�.
		gameWnd.hWnd = hWnd;		//�E�B���h�E�n���h��.
		gameWnd.size.cx = WND_W;	//�E�B���h�E��.
		gameWnd.size.cy = WND_H;	//�E�B���h�E����.
		gameWnd.dwFPS = FPS;		//FPS�l(�).

		//HDC�̍쐬.
		hDC = GetDC( hWnd );	// DC(�f�o�C�X�R���e�L�X�g)���擾.

		//�r�b�g�}�b�v�n���h�����쐬.
		hBmp = CreateCompatibleBitmap(
			hDC,
			gameWnd.size.cx,
			gameWnd.size.cy );

		//DC�ƌ݊����̂���DC���쐬.
		gameWnd.hScreenDC = CreateCompatibleDC( hDC );

		//hScreenDC���r�b�g�}�b�v�p�Ƃ��Ē�`.
		SelectObject( gameWnd.hScreenDC, hBmp );

		//�s�v�ɂȂ���DC�����.
		ReleaseDC( hWnd, hDC );

		//�X���b�h�̍쐬�Ǝ��s.
		hThread = CreateThread(
			nullptr,			//�n���h���𑼂̃v���Z�X�Ƌ��L����ꍇ�͎w��.
			0,					//�X�^�b�N�T�C�Y(�f�t�H���g:0).
			ThreadFunc,			//�X���b�h�֐���.
			(LPVOID)&gameWnd,	//�X���b�h�ɓn���\����.
			0,					//0:�쐬�Ɠ����Ɏ��s.
			&dwID );			//(out)�X���b�hID.

		return 0;

	case WM_KEYDOWN:
		//�L�[�ʂ̏���.
		switch( wParam ){
		case VK_ESCAPE:	//ESC�L�[.
			if( MessageBox( nullptr,
				"�Q�[�����I�����܂���", "�x��", MB_YESNO ) == IDYES )
			{
				//�E�B���h�E����邱�Ƃ�Windows�ɒm�点��.
				PostMessage( gameWnd.hWnd, WM_CLOSE, 0, 0 );
			}
			break;
		}
		return 0;

	case WM_CLOSE:
		//�E�B���h�E����t���O�L��.
		//���X���b�h���Ŋm�F���邽�߂̃t���O.
		gameWnd.isWndClose = TRUE;

		DWORD result;
		while( 1 ) {
			//�X���b�h���I��������`�F�b�N.
			GetExitCodeThread( hThread, &result );
			//�I�������n���h�������.
			if( STILL_ACTIVE != result ) {
				//closehandle�ŕ���.
				CloseHandle( hThread );
				//���[�v�𔲂���.
				break;
			}
		}
		//�E�B���h�E��j������.
		DestroyWindow( hWnd );
		return 0;

	case WM_PAINT:		//�E�B���h�E���X�V���ꂽ��.
		//�`��J�n.
		hDC = BeginPaint( hWnd, &ps );

		BitBlt(
			hDC,				//�\��DC.
			0, 0,				//�\����x,y���W.
			gameWnd.size.cx,	//�\�����镝.
			gameWnd.size.cy,	//�\�����鍂��.
			gameWnd.hScreenDC,	//�o�b�N�o�b�t�@(�X���b�h���ō쐬).
			0, 0,				//���摜��x,y.
			SRCCOPY );			//�R�s�[����.

		//�`��I��.
		EndPaint( hWnd, &ps );
		return 0;
	}

	//���C���ɕԂ����.
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

//���C���֐�.
int WINAPI WinMain(
	HINSTANCE hInstance,	//�C���X�^���X�ԍ�(�E�B���h�E�̔ԍ�).
	HINSTANCE hPrevInstance,
	PSTR lpCmdLine,
	int nCmdShow )
{
	WNDCLASS wc;	//�E�B���h�E�N���X�\����.
	HWND hWnd;		//�E�B���h�E�n���h��.
	MSG msg;        //���b�Z�[�W.

	//--------------------------------------------------
	//	����CPU�ɑΉ������� ��.
	//--------------------------------------------------
	DWORD ProcessAffinitMask;	//CPU���w��.
	HANDLE hProcess;			//�v���Z�X�n���h��.
	DWORD errnum;				//�G���[No.
	BOOL ret;					//�֐����A�l.

	//���g�̃v���Z�X�̃n���h�����擾.
	hProcess = GetCurrentProcess();

	//���g�𓮍삳����CPU���w�肷��.
	ProcessAffinitMask = 1;		//CPU 1���g��(2�i���Ŏw��).
	ret = SetProcessAffinityMask(
		hProcess,
		ProcessAffinitMask );
	if( ret == FALSE )
	{
		errnum = GetLastError();	//�G���[�����擾.
		return 0;
	}
	//--------------------------------------------------
	//	����CPU�ɑΉ������� ��.
	//--------------------------------------------------

	//--------------------------------------
	//  �E�B���h�E�N���X�̓o�^.
	//--------------------------------------
	//�X�^�C���̓o�^.
	// CS_HREDRAW : ���������̍ĕ`��.
	// CS_VREDRAW : ���������̍ĕ`��.
	wc.style = CS_HREDRAW | CS_VREDRAW;
	//�E�B���h�E�֐��̓o�^.
	// DefWindowProc : �f�t�H���g�E�B���h�E�֐�(��Ŏ����ō��).
	wc.lpfnWndProc = WindowProc;
	//�g��Ȃ�(0�Œ�).
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	//�C���X�^���X�ԍ��̓o�^.
	wc.hInstance = hInstance;
	//�A�C�R���̓o�^.
	// nullptr : �f�t�H���g.
	wc.hIcon = nullptr;
	//�J�[�\���̓o�^.
	// nullptr : �f�t�H���g.
	wc.hCursor = nullptr;
	//�E�B���h�E�̔w�i�F.
	// LTGRAY_BRUSH : ���邢�D�F.
	wc.hbrBackground = (HBRUSH)GetStockObject( LTGRAY_BRUSH );
	//���j���[�̓o�^.
	// nullptr : �f�t�H���g.
	wc.lpszMenuName = nullptr;
	//�A�v���P�[�V������.
	wc.lpszClassName = APP_NAME;

	//�E�B���h�E��Windows�ɓo�^.
	if( RegisterClass( &wc ) == 0 ){
		//�G���[���b�Z�[�W�̕\��.
		// MB_OK : OK���݂̂�.
		// MB_YESNO : �͂��E������.
		// MB_OKCANCEL : OK�E�L�����Z��.
		// MB_YESNOCANCEL : �͂��E�������E�L�����Z��.
		MessageBox( nullptr,
			"�E�B���h�E�N���X�o�^���s",
			"�G���[���b�Z�[�W",
			MB_YESNOCANCEL );
		return 0;
	}

	//--------------------------------------
	//	�E�B���h�E�̈�̒���.
	//--------------------------------------
	RECT rect;			//��`�\����.
	DWORD   dwStyle;	//�E�B���h�E�X�^�C��.
	rect.top = 0;			//��.
	rect.left = 0;			//��.
	rect.right = WND_W;		//�E.
	rect.bottom = WND_H;	//��.
	dwStyle = WS_OVERLAPPEDWINDOW;  //�E�B���h�E���.

	if( AdjustWindowRect(
		&rect,			//(in)��ʃT�C�Y����������`�\����.(out)�v�Z����.
		dwStyle,		//�E�B���h�E�X�^�C��.
		FALSE ) == 0 )	//���j���[�������ǂ����̎w��.
	{
		MessageBox( nullptr, "�E�B���h�E�̈�̒����Ɏ��s", "�G���[���b�Z�[�W", MB_OK );
		return 0;
	}

	//--------------------------------------
	//  �E�B���h�E�̍쐬.
	//--------------------------------------
	hWnd = CreateWindow(
		APP_NAME,				//�A�v���P�[�V������.
		WND_TITLE,				//�E�B���h�E�^�C�g��.
		dwStyle,				//�E�B���h�E���.
		100, 100,				//�E�B���h�E�̕\���ʒu(x,y).
		rect.right - rect.left,	//�E�B���h�E�̕�.
		rect.bottom - rect.top,	//�E�B���h�E�̍���.
		nullptr,				//�e�E�B���h�E�̃n���h��.
		nullptr,				//���j���[�̐ݒ�.
		hInstance,				//�C���X�^���X�ԍ�.
		nullptr );				//�E�B���h�E�쐬���ɔ�������C�x���g�ɓn���f�[�^.
	
	if( hWnd == nullptr ){
		MessageBox( nullptr,
			"�E�B���h�E�쐬���s",
			"�G���[���b�Z�[�W", MB_OK );
		return 0;
	}

	//�E�B���h�E�̕\��.
	// SW_SHOW : �\������.
	// SW_HIDE : �B��(��\��).
	ShowWindow( hWnd, SW_SHOW );



	//���b�Z�[�W���[�v.
	while( GetMessage( &msg, nullptr, 0, 0 ) > 0 ){
		//�E�B���h�E�֐��Ƀ��b�Z�[�W�𑗂�.
		DispatchMessage( &msg );
	}

	return 0;
}

