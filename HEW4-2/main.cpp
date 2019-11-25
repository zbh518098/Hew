
#include "main.h"
#include "myDirect3D.h"
#include "input.h"
#include "texture.h"
#include "camera.h"
#include "light.h"
#include "grid.h"
#include "camera2.h"
#include "Player.h"
#include "ground.h"
#include "drink.h"
#include "debugproc.h"
#include <time.h>


#define _USE_MATH_DEFINES
#include <math.h>

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "winmm.lib")


/*------------------------------------------------------------------------------
�萔��`
------------------------------------------------------------------------------*/
#define CLASS_NAME     "GameWindow"       // �E�C���h�E�N���X�̖��O
#define WINDOW_CAPTION "�Q�[���E�B���h�E" // �E�B���h�E�̖��O


/*------------------------------------------------------------------------------
   �O���[�o���ϐ��錾
------------------------------------------------------------------------------*/
static HWND g_hWnd;                 // �E�B���h�E�n���h��

#ifdef _DEBUG
int					g_nCountFPS;			// FPS�J�E���^
#endif
bool				g_bDispDebug = true;	// �f�o�b�O�\��ON/OFF

/*------------------------------------------------------------------------------
�v���g�^�C�v�錾
------------------------------------------------------------------------------*/
// �E�B���h�E�v���V�[�W��(�R�[���o�b�N�֐�)
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// �Q�[���̏�����
static bool Initialize(HINSTANCE hInst);
static void Finalize(void);

// �Q�[���̍X�V�֐�
static void Update(void);
// �Q�[���̕`��֐�
static void Draw(void);


/*------------------------------------------------------------------------------
���C��
------------------------------------------------------------------------------*/
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// �g�p���Ȃ��ꎞ�ϐ��𖾎�
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// �E�B���h�E�N���X�\���̂̐ݒ�
	WNDCLASS wc = {};
	wc.lpfnWndProc = WndProc;                          // �E�B���h�E�v���V�[�W���̎w��
	wc.lpszClassName = CLASS_NAME;                     // �N���X���̐ݒ�
	wc.hInstance = hInstance;                          // �C���X�^���X�n���h���̎w��
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);          // �}�E�X�J�[�\�����w��
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1); // �E�C���h�E�̃N���C�A���g�̈�̔w�i�F��ݒ�

	// �N���X�o�^
	RegisterClass(&wc);


	// �E�B���h�E�X�^�C��
	DWORD window_style = WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX | WS_THICKFRAME);

	// ��{��`���W
	RECT window_rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	// �w�肵���N���C�A���g�̈���m�ۂ��邽�߂ɐV���ȋ�`���W���v�Z
	AdjustWindowRect(&window_rect, window_style, FALSE);

	// �V����Window�̋�`���W���畝�ƍ������Z�o
	int window_width = window_rect.right - window_rect.left;
	int window_height = window_rect.bottom - window_rect.top;

	// �v���C�}�����j�^�[�̉�ʉ𑜓x�擾
	int desktop_width = GetSystemMetrics(SM_CXSCREEN);
	int desktop_height = GetSystemMetrics(SM_CYSCREEN);

	// �f�X�N�g�b�v�̐^�񒆂ɃE�B���h�E�����������悤�ɍ��W���v�Z
	// ��������������A�f�X�N�g�b�v���E�B���h�E���傫���ꍇ�͍���ɕ\��
	int window_x = max((desktop_width - window_width) / 2, 0);
	int window_y = max((desktop_height - window_height) / 2, 0);

	// �E�B���h�E�̐���
	g_hWnd = CreateWindow(
		CLASS_NAME,     // �E�B���h�E�N���X
		WINDOW_CAPTION, // �E�B���h�E�e�L�X�g
		window_style,   // �E�B���h�E�X�^�C��
		window_x,       // �E�B���h�E���Wx
		window_y,       // �E�B���h�E���Wy
		window_width,   // �E�B���h�E�̕�
		window_height,  // �E�B���h�E�̍���
		NULL,           // �e�E�B���h�E�n���h��
		NULL,           // ���j���[�n���h��
		hInstance,      // �C���X�^���X�n���h��
		NULL            // �ǉ��̃A�v���P�[�V�����f�[�^
		);

	if (g_hWnd == NULL) {
		// �E�B���h�E�n���h�������炩�̗��R�Ő����o���Ȃ�����
		return -1;
	}

	// �w��̃E�B���h�E�n���h���̃E�B���h�E���w��̕��@�ŕ\��
	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

	// �Q�[���̏�����
	Initialize(hInstance);


	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;

	// ����\��ݒ�
	timeBeginPeriod(1);

	// �t���[���J�E���g������
	dwExecLastTime = dwFPSLastTime = timeGetTime();
	dwCurrentTime = dwFrameCount = 0;

	// Windows�Q�[���p���C�����[�v
	MSG msg = {}; // msg.message == WM_NULL
	while (WM_QUIT != msg.message) {

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			// ���b�Z�[�W������ꍇ�̓��b�Z�[�W������D��
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			dwCurrentTime = timeGetTime();
			if ((dwCurrentTime - dwFPSLastTime) >= 500)	// 0.5�b���ƂɎ��s
			{
#ifdef _DEBUG
				g_nCountFPS = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);
#endif
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
#ifdef _DEBUG
//				PrintDebugProc("FPS:%d\n", g_nCountFPS);
#endif
				dwExecLastTime = dwCurrentTime;

				// �X�V����
				Update();

				// �`�揈��
				Draw();

				dwFrameCount++;
			}
		}
	}

	// �Q�[���̏I������
	Finalize();

	return (int)msg.wParam;
}

// �E�B���h�E�v���V�[�W��(�R�[���o�b�N�֐�)
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) {
			SendMessage(hWnd, WM_CLOSE, 0, 0); // WM_CLOSE���b�Z�[�W�̑��M
		}
		break;

	case WM_CLOSE:
//		if (MessageBox(hWnd, "�{���ɏI�����Ă�낵���ł����H", "�m�F", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK) {
			DestroyWindow(hWnd); // �w��̃E�B���h�E��WM_DESTROY���b�Z�[�W�𑗂�
//		}
		return 0; // DefWindowProc�֐��Ƀ��b�Z�[�W�𗬂����I�����邱�Ƃɂ���ĉ����Ȃ��������Ƃɂ���

	case WM_DESTROY: // �E�B���h�E�̔j�����b�Z�[�W
		PostQuitMessage(0); // WM_QUIT���b�Z�[�W�̑��M
		return 0;
	};

	// �ʏ탁�b�Z�[�W�����͂��̊֐��ɔC����
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

bool Initialize(HINSTANCE hInst)
{
	//�V�[�h�̏�����
	srand((unsigned int)time(NULL));

	// �Q�[���̏�����(Direct3D�̏�����)
	if (!D3D_Initialize(g_hWnd)){
		// �Q�[���̏������Ɏ��s����
		return false;
	}
	// DirectInput�̏������i�L�[�{�[�h�j
	if (!Keyboard_Initialize(hInst, g_hWnd)){
		return false;
	}
	// DirectInput�̏������i�Q�[���p�b�h�j
	if (!GamePad_Initialize(hInst, g_hWnd)){
		return false;
	}

	//�S�e�N�X�`���t�@�C���̃��[�h
	Texture_Load();

	// �f�o�b�O�\������ 
	DebugProc_Initialize();

	// �J�����̏���������
	Camera_Initialize();
	//Camera2_Initialize();

	// ���C�g�̏���������
	Light_Initialize();

	//�O���b�h�̏���������
	Grid_Initialize();

	// �v���C���[
	Player_Initialize();

	// �O���E���h
	Ground_Initialize();

	// �h�����N
	Drink_Initialize();

	return true;
}

void Finalize(void)
{
	// �J�����̏I������
	Camera_Finalize();
	//Camera2_Finalize();

	// ���C�g�̏I������
	Light_Finalize();

	// �O���b�h�̏I��
	Grid_Finalize();

	// �v���C���[
	Player_Finalize();

	// �O���E���h
	Ground_Finalize();

	// �h�����N
	Drink_Finalize();

	// �e�N�X�`���̉��
	Texture_Release();

	// DirectInput�̏I������
	GamePad_Finalize();

	// DirectInput�̏I������
	Keyboard_Finalize();

	// �Q�[���̏I������(Direct3D�̏I������)
	D3D_Finalize();
}

// �Q�[���̍X�V�֐�
void Update(void)
{
	//�L�[�{�[�h�̏�Ԃ��X�V����
	Keyboard_Update();

	//�Q�[���p�b�h�̏�Ԃ��X�V����
	GamePad_Update();

	// �J�����̍X�V����
	Camera_Update();
	//Camera2_Update();

	// ���C�g�̍X�V����
	Light_Update();

	//�O���b�h��
	Grid_Update();

	// �v���C���[
	Player_Update();

	// �O���E���h
	Ground_Update();

	// �h�����N
	Drink_Update();
}

// �Q�[���̕`��֐�
void Draw(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice = GetD3DDevice();

	// ��ʂ̃N���A
	pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(50, 50, 50, 255), 1.0f, 0);

	// �`��o�b�`���߂̊J�n
	pD3DDevice->BeginScene();


	// �J�����̐ݒ�
	Camera_SetCamera();
	//Camera2_SetCamera();

	//�O���b�h�̕`��
	Grid_Draw();
	
	// �v���C���[
	Player_Draw();
	
	// �O���E���h
	Ground_Draw();

	// �h�����N
	Drink_Draw();

	// �f�o�b�O�\���̕`�揈��
	if (g_bDispDebug)
	{
		DebugProc_Draw();
	}

	// �`��o�b�`���߂̏I��
	pD3DDevice->EndScene();

	// �o�b�N�o�b�t�@���t���b�v�i�^�C�~���O��D3DPRESENT_PARAMETERS�̐ݒ�ɂ��j
	pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//�E�B���h�E�n���h���̃Q�b�^�[
HWND GetHWND(){
	return g_hWnd;
}


double frand() {
	return (double)rand() / RAND_MAX;
}
