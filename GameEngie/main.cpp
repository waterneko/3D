//=============================================================================//
// 
// Name : main.cpp
// Describe : ���C���֐�
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//

//*****************************************************************************//
// (1) �C���N���[�h�t�@�C��
//*****************************************************************************//
#include "main.h"				// ���C���֐�

#include "Game.h"				// �Q�[��������

//*****************************************************************************//
// (2) �}�N����`
//*****************************************************************************//

//*****************************************************************************//
// (3) ���[�U�[��`�f�[�^�֐� (�N���X�֐�)
//*****************************************************************************//

//*****************************************************************************//
// (4) �O���[�o���ϐ�:
//*****************************************************************************//

// �u�[���^
bool 					g_bWindows = true;									// �t���X�N���[�����[�h�I���t���O

// char�^
char					g_strAdapterName[100] = { 0 };						// �O���t�B�b�N�{�[�h��
// �����^

// ���������_��

// Imgui�ϐ� 
ImVec4 					g_BG_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);	// �w�i�F

// Directx �|�C���^�[
LPDIRECT3D9 			g_pD3D = NULL;										// Direct3D �I�u�W�F�N�g
LPDIRECT3DDEVICE9 		g_pDevice = NULL;									// �f�o�C�X �I�u�W�F�N�g(�`��ɕK�v)


// { �\���� }

// { �N���X�|�C���^�[ }
GameInstance*			g_pGame = NULL;										// �Q�[���������N���X�ւ̃|�C���^�[
SourceManager* SourceManager::s_pInstance = new SourceManager;				// �\�[�X�Ǘ�(Singleton)


//*****************************************************************************//
// (5) �v���g�^�C�v�錾(���������g����֐�)
//*****************************************************************************//
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);// Imgui�}�E�X�R�[���o�b�N�֐�
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);		// Windows API �̃G���g���[�֐�
HRESULT CheckAntialias(D3DPRESENT_PARAMETERS d3dpp, D3DMULTISAMPLE_TYPE AntialiasMode); // �A���`�G�C���A�V���O�̃`�F�b�N�Ɛݒ�

// [����������]
HRESULT InitDirectx(HWND, HINSTANCE, BOOL); 	// Directx������
HRESULT InitDirectxInput(HWND);					// Directx Input������
HRESULT InitXAudio2();							// XAudio2������
HRESULT LoadObject(HWND);						// �Q�[���I�u�W�F�N�g������

// [�X�V����]
VOID Update();									// �X�V������
VOID UpdateWinAPI(HWND hWnd);					// WinAPI�@�\�X�V����

// [�`�揈��]
VOID Render();									// �`�摍����

// [�I������]
VOID Uninit();									// �������
//*****************************************************************************//
// (6) �֐�
//*****************************************************************************//

//==========================================================================================================//
// NAME: INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT iCmdShow)
// DESC: Windows API �̃G���g���[�֐�
//==========================================================================================================//
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT iCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(iCmdShow);

	HWND hWnd = NULL;
	MSG msg;
	WNDCLASSEX  wndclass;

	// �����x���ԃJ�E���^
	LARGE_INTEGER nFrequency;			// ���b�X�V��
	LARGE_INTEGER nCurrentTime;			// ���ݎ���
	LARGE_INTEGER nExecLastTime;		// ��񎞊�
	LARGE_INTEGER nFPSLastTime;			// FPS


	// [WindowAPI�ݒ�]< 1.�E�B���h�E�N���X�̓o�^ >
	wndclass.cbSize = sizeof(wndclass);									// �E�C���h�E�N���X�\���̑召
	wndclass.style = CS_HREDRAW | CS_VREDRAW;							// �E�C���h�E�l��
	wndclass.lpfnWndProc = WndProc;										// �E�C���h�E�ւ̃|�C���^
	wndclass.cbClsExtra = 0;											// �E�C���h�E�N���X����
	wndclass.cbWndExtra = 0;											// �E�C���h�E�N���X�z�O����
	wndclass.hInstance = hInstance;										// �E�C���h�E�N���X�̃C���X�^���X�n���h��
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);					// �E�C���h�E�N���X�̃A�C�R��
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);						// �E�C���h�E�N���X�̃R���\�[��
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);		// �E�C���h�E�N���X�w�i�F
	wndclass.lpszMenuName = NULL;										// �E�C���h�E�N���X�̃��j���[��
	wndclass.lpszClassName = CLASS_NAME;								// �E�C���h�E�N���X�̖�
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);					// �E�C���h�E�N���X�̍ŏ����A�C�R��

	RegisterClassEx(&wndclass);											// �E�C���h�E�N���X��o�^���܂�

																		// [WindowAPI�ݒ�]< 2. �E�B���h�E�̍쐬 >
	hWnd = CreateWindow(
		CLASS_NAME,														// �E�C���h�E�̃N���X��
		WINDOW_NAME,													// �E�C���h�E�̃L���v�V����
		WS_OVERLAPPEDWINDOW, 											// �E�B���h�E�X�^�C��
		CW_USEDEFAULT, 													// �E�B���h�E�̏������Wx
		CW_USEDEFAULT, 													// �E�B���h�E�̏������Wy
		WINDOW_WIDTH, 													// �E�C���h�E�̕�
		WINDOW_HEIGHT, 													// �E�C���h�E�̍���
		NULL, 															// �e�E�C���h�E�̃C���X�^���X�n���h��
		NULL, 															// �E�C���h�E�N���X�̃��j���[��ݒ�
		hInstance,														// �E�C���h�E�N���X�̃C���X�^���X�n���h��
		NULL);

	// �t���X�N���[�����[�h�I��
	int iWindowMsg = (MessageBox(hWnd, "�E�B���h�E���[�h���܂����H", "�Q�[���̃��b�Z�[�W�E�B���h�E", MB_YESNO));

	if (iWindowMsg == IDYES)
	{
		g_bWindows = true;
	}
	else if (iWindowMsg == IDNO)
	{
		g_bWindows = false;
	}

	// ���ԃJ�E���g������
	memset(&nFrequency,		0x00,	sizeof(LARGE_INTEGER));
	memset(&nCurrentTime,	0x00,	sizeof(LARGE_INTEGER));
	memset(&nExecLastTime,	0x00,	sizeof(LARGE_INTEGER));
	memset(&nFPSLastTime,	0x00,	sizeof(LARGE_INTEGER));

	QueryPerformanceCounter(&nExecLastTime);
	nFPSLastTime = nExecLastTime;

	// �����_����������
	srand((unsigned)time(NULL));

	// [WindowAPI�ݒ�]< 3. �E�C���h�E�̕\�� >
	ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd);

	// [WindowAPI�ݒ�]< 4. Directx�̏����� >

	// �_�C���N�g3D�̏������֐����Ă�
	if (FAILED(InitDirectx(hWnd, hInstance, g_bWindows)))
	{
		return 0;
	}
	// �_�C���N�g�C���v�b�g�̏������֐����Ă�
	if (FAILED(InitDirectxInput(hWnd)))
	{
		return 0;
	}
	// XAudio2�̏������֐����Ă�
	if (FAILED(InitXAudio2()))
	{
		return 0;
	}

	// [WindowAPI�ݒ�]< 5.���b�Z�[�W���[�v >
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			QueryPerformanceFrequency(&nFrequency);
			QueryPerformanceCounter(&nCurrentTime);
			double dTime = (double)(nCurrentTime.QuadPart - nExecLastTime.QuadPart) * 1000.0 / (double)nFrequency.QuadPart;

			// �Œ�FPS��(60�t���[����)
			if (dTime >= GAME_FPS)
			{
				nExecLastTime = nCurrentTime;

				// WinAPI�@�\�X�V����
				//UpdateWinAPI(hWnd);

				// Imgui������
				ImGui_ImplDX9_NewFrame();

				// �X�V������
				Update();

				// �`�摍����
				Render();
			}
		}
	}

	// [WindowAPI�ݒ�]< 6.�E�B���h�E�N���X�̓o�^������ > //
	UnregisterClass(CLASS_NAME, wndclass.hInstance);

	// ���b�Z�[�W���[�v���甲������I�u�W�F�N�g��S�ĊJ������(�I������)
	Uninit();

	// OS�ɖ߂�i�A�v���P�[�V�������I������j
	return (INT)msg.wParam;
}

//==========================================================================================================//
// NAME: LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
// DESC: �E�B���h�E�v���V�[�W���֐�
//==========================================================================================================//
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// Imgui�Ń}�E�X�R���g���[���ł���
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
	{
		return true;
	}

	switch (uMsg)
	{
		/*case WM_SIZE:
		if (g_pDevice != NULL && wParam != SIZE_MINIMIZED)
		{
		ImGui_ImplDX9_InvalidateDeviceObjects();
		ImGui_ImplDX9_CreateDeviceObjects();
		}*/
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:					// [ESC]�L�[�������ꂽ
			if (g_bWindows)
			{
				int iESCWindowMsg = (MessageBox(hWnd, "�Q�[�����I���܂����H", "�X�N���[�����[�h�I��", MB_YESNOCANCEL));

				if (iESCWindowMsg == IDYES)
				{
					DestroyWindow(hWnd);		// �E�B���h�E��j������悤�w������
				}
			}
			else
			{
				DestroyWindow(hWnd);		// �E�B���h�E��j������悤�w������
			}
			break;
		}
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//==========================================================================================================//
// NAME: HRESULT InitD3d(HWND hWnd, HINSTANCE hInstance)
// DESC: �_�C���N�g3D�̏������֐�
//==========================================================================================================//
HRESULT InitDirectx(HWND hWnd, HINSTANCE hInstance, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;
	D3DCAPS9 caps;
	// [Directx3D]< 1.�uDirect3D�v�I�u�W�F�N�g�̍쐬 >
	if (NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		MessageBox(NULL, "Direct3D�I�u�W�F�N�g�쐬�����s���܂���", "", MB_OK);
		return E_FAIL;
	}

	// �f�o�C�X���\�`�F�b�N
	g_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
	if (caps.VertexShaderVersion < D3DVS_VERSION(2, 0) || caps.PixelShaderVersion < D3DPS_VERSION(2, 0))
	{
		MessageBox(NULL, "�V�F�[�_�[2.0���g�p�ł��܂���ł���", NULL, MB_OK);
	}

	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// [Directx3D]< 2. �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ� >
	ZeroMemory(&d3dpp, sizeof(d3dpp));							// ���[�N���[���N���A
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;					// �o�b�N�o�b�t�@�t�H�[�}�b�g
	d3dpp.BackBufferCount = 1;									// �o�b�N�o�b�t�@�̐�
	d3dpp.BackBufferWidth = WINDOW_WIDTH;						// �o�b�N�o�b�t�@��ʃT�C�Y(��)
	d3dpp.BackBufferHeight = WINDOW_HEIGHT;						// �o�b�N�o�b�t�@��ʃT�C�Y(����)
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// �f���M���ɓ������ăt���b�v����
	d3dpp.Windowed = bWindow;									// �E�B���h�E���[�h
	d3dpp.EnableAutoDepthStencil = true;						// �f�v�X�o�b�t�@�i�y�o�b�t�@�j�ƃX�e���V���o�b�t�@���쐬

	if (bWindow)
	{// �E�B���h�E���[�h
	 //d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// �f�v�X�o�b�t�@�Ƃ���16bit���g��
		d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;				// �f�v�X�o�b�t�@�Ƃ���24�r�b�gZ�o�b�t�@8�r�b�g�X�e���V���o�b�t�@�쐬���g��
		d3dpp.FullScreen_RefreshRateInHz = 0;						// ���t���b�V�����[�g
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;	// �C���^�[�o�� (�t���[�������Ȃ�)
	}
	else
	{// �t���X�N���[�����[�h
		d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;					// �f�v�X�o�b�t�@�Ƃ���24�r�b�gZ�o�b�t�@8�r�b�g�X�e���V���o�b�t�@�쐬���g��
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// ���t���b�V�����[�g
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		// �C���^�[�o�� (�t���[�������Ȃ�)
	}

	// �A���`�G�C���A�V���O
	if (FAILED(CheckAntialias(d3dpp, D3DMULTISAMPLE_8_SAMPLES)))
	{
		MessageBox(0, "�A���`�G�C���A�V���O���g�p�ł��܂���ł���", "", MB_OK);
		d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	}

	// [Directx3D]< 3. �f�o�C�X�쐬���� >

	// [�f�o�C�X�쐬����]<�`��>��<���_����>���n�[�h�E�F�A�ōs�Ȃ�
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &g_pDevice)))
	{
		// ��L�̐ݒ肪���s������
		// [�f�o�C�X�쐬����]<�`��>���n�[�h�E�F�A�ōs���A<���_����>��CPU�ōs�Ȃ�
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &g_pDevice)))
		{
			MessageBox(0, "HAL���[�h��DIRECT3D�f�o�C�X���쐬�ł��܂���\nREF���[�h�ōĎ��s���܂�", NULL, MB_OK);
			// ��L�̐ݒ肪���s������
			// [�f�o�C�X�쐬����]<�`��>��CPU�ōs�Ȃ��A<���_����>�̓n�[�h�E�F�A�ōs�Ȃ�
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd,
				D3DCREATE_HARDWARE_VERTEXPROCESSING,
				&d3dpp, &g_pDevice)))
			{
				// ��L�̐ݒ肪���s������
				// [�f�o�C�X�쐬����]<�`��>��<���_����>��CPU�ōs�Ȃ�
				if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd,
					D3DCREATE_SOFTWARE_VERTEXPROCESSING,
					&d3dpp, &g_pDevice)))
				{
					// ���������s
					MessageBox(0, "�f�o�C�X�쐬�����s���܂���", NULL, MB_OK);
					return E_FAIL;
				}
			}
		}
	}

	// �O���t�B�b�N�{�[�h�擾����
	D3DADAPTER_IDENTIFIER9 Adapter;						// �O���t�B�b�N�{�[�h���\�\����
	g_pD3D->GetAdapterIdentifier(0, 0, &Adapter);		// �O���t�B�b�N�{�[�h�����擾����
	strcat_s(g_strAdapterName, Adapter.Description);

	// [Directx3D]< 4. �������^�����O�ݒ� >
	// ���ʃJ�����O������
	g_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	// Z�o�b�t�@�[������L���ɂ���
	g_pDevice->SetRenderState(D3DRS_ZENABLE, true);
	// ���C�g��L���ɂ���
	g_pDevice->SetRenderState(D3DRS_LIGHTING, true);
	// ���_�@���̎������K��
	g_pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	// �X�y�L�����i���򔽎�)��L���ɂ���
	g_pDevice->SetRenderState(D3DRS_SPECULARENABLE, true);
	// �A���`�G�C���A�V���O��L���ɂ���
	g_pDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, true);

	// �A���t�@�u�����h��ݒ肷��
	g_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true); 			// ���u�����h���s��
	g_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	g_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��

	// �T���v���[�X�e�[�g�p�����[�^�̐ݒ�(�e�N�X�`��UV�\������)
	g_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// �e�N�X�`���t�l�̌J��Ԃ��ݒ�
	g_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`���u�l�̌J��Ԃ��ݒ�
	g_pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);	// �e�N�X�`���g�厞�̕�Ԑݒ�
	g_pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);	// �e�N�X�`���k�����̕�Ԑݒ�
	//g_pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���g�厞�̕�Ԑݒ�
	//g_pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���k�����̕�Ԑݒ�

	// �A���t�@�u�����h�̕ύX (�|���Z ,�e�N�X�`�������x�`�悷�邽��)
	g_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// �A���t�@�u�����f�B���O����
	g_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// �ŏ��̃A���t�@����
	g_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// �Q�Ԗڂ̃A���t�@����

	//[Directx3D]< 5. �uDirect3D�v�I�u�W�F�N�g��� > 
	SAFE_RELEASE(g_pD3D);

	// �Q�[���Ώۂ̏���������
	if (SUCCEEDED(LoadObject((hWnd))))
	{
		return S_OK;
	}
	else
	{
		MessageBox(0, "Directx�����������s���܂���", NULL, MB_OK);
		return E_FAIL;
	}
}

//==========================================================================================================//
// NAME: HRESULT InitDirectxInput(HWND hWnd, HINSTANCE hInstance)
// DESC: �_�C���N�g�C���v�b�g�̏������֐�
//==========================================================================================================//
HRESULT InitDirectxInput(HWND hWnd)
{
	// ���͏�����
	if (SUCCEEDED(InitInput(hWnd,true)))
	{
		return S_OK;
	}
	return E_FAIL;
}

//==========================================================================================================//
// NAME: HRESULT InitXAudio2()
// DESC: XAudio2�̏������֐�
//==========================================================================================================//
HRESULT InitXAudio2()
{
	// �T�E���h������
	if (SUCCEEDED(InitSound()))
	{
		return S_OK;
	}
	return E_FAIL;
}

//==========================================================================================================//
// NAME: HRESULT LoadObject(HWND hWnd)
// DESC: �Q�[���I�u�W�F�N�g������
//==========================================================================================================//
HRESULT LoadObject(HWND hWnd)
{
	// [Imgui]< 1.Imgui���������� >
	// Imgui�t�H���g�ݒ�
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Imgui�L�[�{�[�h�R���g���[��
	io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\HGRSMP.TTF", 16.0f, NULL, io.Fonts->GetGlyphRangesChinese());

	// Imgui������
	ImGui_ImplDX9_Init(hWnd, g_pDevice);

	// Imgui�X�^�C������(Dark)
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// �Q�[��������
	g_pGame = new GameInstance(g_pDevice);

	// �Q�[��������������
	g_pGame->InitGame();

	return S_OK;
}

//==========================================================================================================//
// NAME: VOID UpdateWinAPI(HWND hWnd)
// DESC: WinAPI�@�\�X�V����
//==========================================================================================================//
VOID UpdateWinAPI(HWND hWnd)
{
	POINT lt, rb;
	RECT rect;
	GetClientRect(hWnd, &rect);  // �E�C���h�E�N���X�N���C�A���g�̈���擾

								 // ������W��lt�ɕۑ�����
	lt.x = rect.left;
	lt.y = rect.top;
	// �E�����W��rb�ɕۑ�����
	rb.x = rect.right;
	rb.y = rect.bottom;
	// �X�N���[�����W�ɕϊ�����
	ClientToScreen(hWnd, &lt);
	ClientToScreen(hWnd, &rb);

	rect.left = lt.x;
	rect.top = lt.y;
	rect.right = rb.x;
	rect.bottom = rb.y;
	// �}�E�X�R���\�[���̓X�N���[���������ړ��\�^
	ClipCursor(&rect);
	// �}�E�X�R���\�[�����B���
	ShowCursor(false);
}

//==========================================================================================================//
// NAME: VOID Update();
// DESC: �X�V������
//==========================================================================================================//
VOID Update()
{
	// ���͍X�V
	UpdateInput(true);

	// [Imgui]< 2.Imgui�X�V���� >
	// Imgui�R���\�[���X�V
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize.x = WINDOW_WIDTH;
	io.DisplaySize.y = WINDOW_HEIGHT;

	// Imgui�E�B���h�E���
	ImGui::SetNextWindowPos(ImVec2(10, 10));
	ImGui::SetNextWindowSize(ImVec2(380, 100), ImGuiCond_Once);
	ImGui::Begin(u8"���E�B���h�E");
	ImGui::Text(u8"�O���t�B�b�N�{�[�h: %s", g_strAdapterName);
	ImGui::Text(u8"�Q�[��FPS: %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::ColorEdit3(u8"�E�B���h�E�w�i�F", (float*)&g_BG_color); // Edit 3 floats representing a color
	ImGui::End();

	// �R���g���[������
	if (GetKeyboardTrigger(DIK_F1))
	{
		g_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}
	else if (GetKeyboardTrigger(DIK_F2))
	{
		g_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}

	// �Q�[���������X�V
	g_pGame->UpdateGame();
}

//==========================================================================================================//
// NAME: VOID Drawing()
// DESC: �`�揈��
//==========================================================================================================//
VOID Render()
{
	D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(g_BG_color.x*255.0f), (int)(g_BG_color.y*255.0f), (int)(g_BG_color.z*255.0f), (int)(g_BG_color.w*255.0f));

	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	g_pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), clear_col_dx, 1.0f, 0);

	// Direct3D�ɂ��`��̊J�n
	if (SUCCEEDED(g_pDevice->BeginScene()))
	{
		// �Q�[���������`��
		g_pGame->RenderGame();

		// [Imgui]< 3.Imgui�`�揈�� >
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

		// Direct3D�ɂ��`��̏I��
		g_pDevice->EndScene();
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	g_pDevice->Present(NULL, NULL, NULL, NULL);
}

//==========================================================================================================//
// NAME: Uninit();
// DESC: �I������
//==========================================================================================================//
VOID Uninit()
{
	// �Q�[���������I��
	SAFE_DELETE(g_pGame)

	// [Imgui]< 4.Imgui�I������ >
	ImGui_ImplDX9_Shutdown();
	ImGui::DestroyContext();

	// ���͏I������
	UninitInput();
	// �T�E���h�I������
	UninitSound();

	// �f�o�C�X�����[�X
	SAFE_RELEASE(g_pDevice)
}

//==========================================================================================================//
// NAME: HRESULT CheckAntialias(D3DPRESENT_PARAMETERS d3dpp ,D3DMULTISAMPLE_TYPE AntialiasMode)
// DESC: �A���`�G�C���A�V���O�̃`�F�b�N�Ɛݒ�
//==========================================================================================================//
HRESULT CheckAntialias(D3DPRESENT_PARAMETERS d3dpp, D3DMULTISAMPLE_TYPE AntialiasMode)
{
	HRESULT hr = E_FAIL;
	DWORD QualityBackBuffer = 0;
	DWORD QualityZBuffer = 0;
	DWORD m = (DWORD)AntialiasMode;

	while (m)
	{
		//�����_�����O�^�[�Q�b�g�ŃA���`�G�C���A�V���O���T�|�[�g����Ă��邩���`�F�b�N
		hr = g_pD3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			d3dpp.BackBufferFormat,
			d3dpp.Windowed,
			(D3DMULTISAMPLE_TYPE)m,
			&QualityBackBuffer);
		if (SUCCEEDED(hr))
		{
			//�[�x�X�e���V�� �T�[�t�F�C�X�ŃA���`�G�C���A�V���O���T�|�[�g����Ă��邩���`�F�b�N
			hr = g_pD3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_HAL,
				d3dpp.AutoDepthStencilFormat,
				d3dpp.Windowed,
				(D3DMULTISAMPLE_TYPE)m,
				&QualityZBuffer);
			if (SUCCEEDED(hr))
			{
				//�A���`�G�C���A�V���O���g�p�ł���̂�D3DPRESENT_PARAMETERS�Ƀ^�C�v���Z�b�g����B
				d3dpp.MultiSampleType = (D3DMULTISAMPLE_TYPE)m;

				//QualityBackBuffer��QualityZBuffer�ŏ��������̒l��L���ɂ���B�ǂ�ȃp�^�[���Œl���Ԃ�̂��킩���̂ŁA���������ق������S���ƁB
				if (QualityBackBuffer < QualityZBuffer)
					d3dpp.MultiSampleQuality = QualityBackBuffer - 1;
				else
					d3dpp.MultiSampleQuality = QualityZBuffer - 1;

				break;
			}
		}

		//���݂̃^�C�v�ŃA���`�G�C���A�V���O���g�p�ł��Ȃ��̂ŁA�P�i�K�Ⴂ�^�C�v�ōă`�F�b�N����B
		m--;
	}

	return hr;
}

//==========================================================================================================//
// NAME: LPDIRECT3DDEVICE9  GetDevice()
// DESC: �f�o�C�X���擾�֐�
//==========================================================================================================//
LPDIRECT3DDEVICE9  GetDevice()
{
	return g_pDevice;
}