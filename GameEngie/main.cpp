//=============================================================================//
// 
// Name : main.cpp
// Describe : メイン関数
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//

//*****************************************************************************//
// (1) インクルードファイル
//*****************************************************************************//
#include "main.h"				// メイン関数

#include "Game.h"				// ゲーム総処理

//*****************************************************************************//
// (2) マクロ定義
//*****************************************************************************//

//*****************************************************************************//
// (3) ユーザー定義データ関数 (クラス関数)
//*****************************************************************************//

//*****************************************************************************//
// (4) グローバル変数:
//*****************************************************************************//

// ブール型
bool 					g_bWindows = true;									// フルスクリーンモード選択フラグ

// char型
char					g_strAdapterName[100] = { 0 };						// グラフィックボード名
// 整数型

// 浮動小数点数

// Imgui変数 
ImVec4 					g_BG_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);	// 背景色

// Directx ポインター
LPDIRECT3D9 			g_pD3D = NULL;										// Direct3D オブジェクト
LPDIRECT3DDEVICE9 		g_pDevice = NULL;									// デバイス オブジェクト(描画に必要)


// { 構造体 }

// { クラスポインター }
GameInstance*			g_pGame = NULL;										// ゲーム総処理クラスへのポインター
SourceManager* SourceManager::s_pInstance = new SourceManager;				// ソース管理(Singleton)


//*****************************************************************************//
// (5) プロトタイプ宣言(自分だけ使える関数)
//*****************************************************************************//
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);// Imguiマウスコールバック関数
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);		// Windows API のエントリー関数
HRESULT CheckAntialias(D3DPRESENT_PARAMETERS d3dpp, D3DMULTISAMPLE_TYPE AntialiasMode); // アンチエイリアシングのチェックと設定

// [初期化処理]
HRESULT InitDirectx(HWND, HINSTANCE, BOOL); 	// Directx初期化
HRESULT InitDirectxInput(HWND);					// Directx Input初期化
HRESULT InitXAudio2();							// XAudio2初期化
HRESULT LoadObject(HWND);						// ゲームオブジェクト初期化

// [更新処理]
VOID Update();									// 更新総処理
VOID UpdateWinAPI(HWND hWnd);					// WinAPI機能更新処理

// [描画処理]
VOID Render();									// 描画総処理

// [終了処理]
VOID Uninit();									// 解放処理
//*****************************************************************************//
// (6) 関数
//*****************************************************************************//

//==========================================================================================================//
// NAME: INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT iCmdShow)
// DESC: Windows API のエントリー関数
//==========================================================================================================//
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT iCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(iCmdShow);

	HWND hWnd = NULL;
	MSG msg;
	WNDCLASSEX  wndclass;

	// 高精度時間カウンタ
	LARGE_INTEGER nFrequency;			// 毎秒更新回数
	LARGE_INTEGER nCurrentTime;			// 現在時間
	LARGE_INTEGER nExecLastTime;		// 先回時間
	LARGE_INTEGER nFPSLastTime;			// FPS


	// [WindowAPI設定]< 1.ウィンドウクラスの登録 >
	wndclass.cbSize = sizeof(wndclass);									// ウインドウクラス構造体大小
	wndclass.style = CS_HREDRAW | CS_VREDRAW;							// ウインドウ様式
	wndclass.lpfnWndProc = WndProc;										// ウインドウへのポインタ
	wndclass.cbClsExtra = 0;											// ウインドウクラス内存
	wndclass.cbWndExtra = 0;											// ウインドウクラス額外内存
	wndclass.hInstance = hInstance;										// ウインドウクラスのインスタンスハンドル
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);					// ウインドウクラスのアイコン
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);						// ウインドウクラスのコンソール
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);		// ウインドウクラス背景色
	wndclass.lpszMenuName = NULL;										// ウインドウクラスのメニュー名
	wndclass.lpszClassName = CLASS_NAME;								// ウインドウクラスの名
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);					// ウインドウクラスの最小化アイコン

	RegisterClassEx(&wndclass);											// ウインドウクラスを登録します

																		// [WindowAPI設定]< 2. ウィンドウの作成 >
	hWnd = CreateWindow(
		CLASS_NAME,														// ウインドウのクラス名
		WINDOW_NAME,													// ウインドウのキャプション
		WS_OVERLAPPEDWINDOW, 											// ウィンドウスタイル
		CW_USEDEFAULT, 													// ウィンドウの初期座標x
		CW_USEDEFAULT, 													// ウィンドウの初期座標y
		WINDOW_WIDTH, 													// ウインドウの幅
		WINDOW_HEIGHT, 													// ウインドウの高さ
		NULL, 															// 親ウインドウのインスタンスハンドル
		NULL, 															// ウインドウクラスのメニューを設定
		hInstance,														// ウインドウクラスのインスタンスハンドル
		NULL);

	// フルスクリーンモード選択
	int iWindowMsg = (MessageBox(hWnd, "ウィンドウモードしますか？", "ゲームのメッセージウィンドウ", MB_YESNO));

	if (iWindowMsg == IDYES)
	{
		g_bWindows = true;
	}
	else if (iWindowMsg == IDNO)
	{
		g_bWindows = false;
	}

	// 時間カウント初期化
	memset(&nFrequency,		0x00,	sizeof(LARGE_INTEGER));
	memset(&nCurrentTime,	0x00,	sizeof(LARGE_INTEGER));
	memset(&nExecLastTime,	0x00,	sizeof(LARGE_INTEGER));
	memset(&nFPSLastTime,	0x00,	sizeof(LARGE_INTEGER));

	QueryPerformanceCounter(&nExecLastTime);
	nFPSLastTime = nExecLastTime;

	// ランダム数初期化
	srand((unsigned)time(NULL));

	// [WindowAPI設定]< 3. ウインドウの表示 >
	ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd);

	// [WindowAPI設定]< 4. Directxの初期化 >

	// ダイレクト3Dの初期化関数を呼ぶ
	if (FAILED(InitDirectx(hWnd, hInstance, g_bWindows)))
	{
		return 0;
	}
	// ダイレクトインプットの初期化関数を呼ぶ
	if (FAILED(InitDirectxInput(hWnd)))
	{
		return 0;
	}
	// XAudio2の初期化関数を呼ぶ
	if (FAILED(InitXAudio2()))
	{
		return 0;
	}

	// [WindowAPI設定]< 5.メッセージループ >
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// メッセージの翻訳とディスパッチ
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			QueryPerformanceFrequency(&nFrequency);
			QueryPerformanceCounter(&nCurrentTime);
			double dTime = (double)(nCurrentTime.QuadPart - nExecLastTime.QuadPart) * 1000.0 / (double)nFrequency.QuadPart;

			// 固定FPS化(60フレーム化)
			if (dTime >= GAME_FPS)
			{
				nExecLastTime = nCurrentTime;

				// WinAPI機能更新処理
				//UpdateWinAPI(hWnd);

				// Imgui総処理
				ImGui_ImplDX9_NewFrame();

				// 更新総処理
				Update();

				// 描画総処理
				Render();
			}
		}
	}

	// [WindowAPI設定]< 6.ウィンドウクラスの登録を解除 > //
	UnregisterClass(CLASS_NAME, wndclass.hInstance);

	// メッセージループから抜けたらオブジェクトを全て開放する(終了処理)
	Uninit();

	// OSに戻る（アプリケーションを終了する）
	return (INT)msg.wParam;
}

//==========================================================================================================//
// NAME: LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
// DESC: ウィンドウプロシージャ関数
//==========================================================================================================//
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// Imguiでマウスコントロールできる
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
		case VK_ESCAPE:					// [ESC]キーが押された
			if (g_bWindows)
			{
				int iESCWindowMsg = (MessageBox(hWnd, "ゲームを終わりますか？", "スクリーンモード選択", MB_YESNOCANCEL));

				if (iESCWindowMsg == IDYES)
				{
					DestroyWindow(hWnd);		// ウィンドウを破棄するよう指示する
				}
			}
			else
			{
				DestroyWindow(hWnd);		// ウィンドウを破棄するよう指示する
			}
			break;
		}
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//==========================================================================================================//
// NAME: HRESULT InitD3d(HWND hWnd, HINSTANCE hInstance)
// DESC: ダイレクト3Dの初期化関数
//==========================================================================================================//
HRESULT InitDirectx(HWND hWnd, HINSTANCE hInstance, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;
	D3DCAPS9 caps;
	// [Directx3D]< 1.「Direct3D」オブジェクトの作成 >
	if (NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		MessageBox(NULL, "Direct3Dオブジェクト作成を失敗しました", "", MB_OK);
		return E_FAIL;
	}

	// デバイス性能チェック
	g_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
	if (caps.VertexShaderVersion < D3DVS_VERSION(2, 0) || caps.PixelShaderVersion < D3DPS_VERSION(2, 0))
	{
		MessageBox(NULL, "シェーダー2.0が使用できませんでした", NULL, MB_OK);
	}

	// 現在のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// [Directx3D]< 2. デバイスのプレゼンテーションパラメータの設定 >
	ZeroMemory(&d3dpp, sizeof(d3dpp));							// ワークをゼロクリア
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;					// バックバッファフォーマット
	d3dpp.BackBufferCount = 1;									// バックバッファの数
	d3dpp.BackBufferWidth = WINDOW_WIDTH;						// バックバッファ画面サイズ(幅)
	d3dpp.BackBufferHeight = WINDOW_HEIGHT;						// バックバッファ画面サイズ(高さ)
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// 映像信号に同期してフリップする
	d3dpp.Windowed = bWindow;									// ウィンドウモード
	d3dpp.EnableAutoDepthStencil = true;						// デプスバッファ（Ｚバッファ）とステンシルバッファを作成

	if (bWindow)
	{// ウィンドウモード
	 //d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// デプスバッファとして16bitを使う
		d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;				// デプスバッファとして24ビットZバッファ8ビットステンシルバッファ作成を使う
		d3dpp.FullScreen_RefreshRateInHz = 0;						// リフレッシュレート
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;	// インターバル (フレーム制限なし)
	}
	else
	{// フルスクリーンモード
		d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;					// デプスバッファとして24ビットZバッファ8ビットステンシルバッファ作成を使う
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// リフレッシュレート
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		// インターバル (フレーム制限なし)
	}

	// アンチエイリアシング
	if (FAILED(CheckAntialias(d3dpp, D3DMULTISAMPLE_8_SAMPLES)))
	{
		MessageBox(0, "アンチエイリアシングが使用できませんでした", "", MB_OK);
		d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	}

	// [Directx3D]< 3. デバイス作成制御 >

	// [デバイス作成制御]<描画>と<頂点処理>をハードウェアで行なう
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &g_pDevice)))
	{
		// 上記の設定が失敗したら
		// [デバイス作成制御]<描画>をハードウェアで行い、<頂点処理>はCPUで行なう
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &g_pDevice)))
		{
			MessageBox(0, "HALモードでDIRECT3Dデバイスを作成できません\nREFモードで再試行します", NULL, MB_OK);
			// 上記の設定が失敗したら
			// [デバイス作成制御]<描画>をCPUで行なう、<頂点処理>はハードウェアで行なう
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd,
				D3DCREATE_HARDWARE_VERTEXPROCESSING,
				&d3dpp, &g_pDevice)))
			{
				// 上記の設定が失敗したら
				// [デバイス作成制御]<描画>と<頂点処理>をCPUで行なう
				if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd,
					D3DCREATE_SOFTWARE_VERTEXPROCESSING,
					&d3dpp, &g_pDevice)))
				{
					// 初期化失敗
					MessageBox(0, "デバイス作成を失敗しました", NULL, MB_OK);
					return E_FAIL;
				}
			}
		}
	}

	// グラフィックボード取得する
	D3DADAPTER_IDENTIFIER9 Adapter;						// グラフィックボード性能構造体
	g_pD3D->GetAdapterIdentifier(0, 0, &Adapter);		// グラフィックボード情報を取得する
	strcat_s(g_strAdapterName, Adapter.Description);

	// [Directx3D]< 4. 初期レタリング設定 >
	// 裏面カリングをする
	g_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	// Zバッファー処理を有効にする
	g_pDevice->SetRenderState(D3DRS_ZENABLE, true);
	// ライトを有効にする
	g_pDevice->SetRenderState(D3DRS_LIGHTING, true);
	// 頂点法線の自動正規化
	g_pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	// スペキュラ（光沢反射)を有効にする
	g_pDevice->SetRenderState(D3DRS_SPECULARENABLE, true);
	// アンチエイリアシングを有効にする
	g_pDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, true);

	// アルファブレンドを設定する
	g_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true); 			// αブレンドを行う
	g_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	g_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定

	// サンプラーステートパラメータの設定(テクスチャUV表示方式)
	g_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャＵ値の繰り返し設定
	g_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャＶ値の繰り返し設定
	g_pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);	// テクスチャ拡大時の補間設定
	g_pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);	// テクスチャ縮小時の補間設定
	//g_pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大時の補間設定
	//g_pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小時の補間設定

	// アルファブレンドの変更 (掛け算 ,テクスチャ透明度描画するため)
	g_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// アルファブレンディング処理
	g_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 最初のアルファ引数
	g_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// ２番目のアルファ引数

	//[Directx3D]< 5. 「Direct3D」オブジェクト解放 > 
	SAFE_RELEASE(g_pD3D);

	// ゲーム対象の初期化処理
	if (SUCCEEDED(LoadObject((hWnd))))
	{
		return S_OK;
	}
	else
	{
		MessageBox(0, "Directx初期化を失敗しました", NULL, MB_OK);
		return E_FAIL;
	}
}

//==========================================================================================================//
// NAME: HRESULT InitDirectxInput(HWND hWnd, HINSTANCE hInstance)
// DESC: ダイレクトインプットの初期化関数
//==========================================================================================================//
HRESULT InitDirectxInput(HWND hWnd)
{
	// 入力初期化
	if (SUCCEEDED(InitInput(hWnd,true)))
	{
		return S_OK;
	}
	return E_FAIL;
}

//==========================================================================================================//
// NAME: HRESULT InitXAudio2()
// DESC: XAudio2の初期化関数
//==========================================================================================================//
HRESULT InitXAudio2()
{
	// サウンド初期化
	if (SUCCEEDED(InitSound()))
	{
		return S_OK;
	}
	return E_FAIL;
}

//==========================================================================================================//
// NAME: HRESULT LoadObject(HWND hWnd)
// DESC: ゲームオブジェクト初期化
//==========================================================================================================//
HRESULT LoadObject(HWND hWnd)
{
	// [Imgui]< 1.Imgui初期化処理 >
	// Imguiフォント設定
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Imguiキーボードコントロール
	io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\HGRSMP.TTF", 16.0f, NULL, io.Fonts->GetGlyphRangesChinese());

	// Imgui初期化
	ImGui_ImplDX9_Init(hWnd, g_pDevice);

	// Imguiスタイル実装(Dark)
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// ゲーム総処理
	g_pGame = new GameInstance(g_pDevice);

	// ゲーム総処理初期化
	g_pGame->InitGame();

	return S_OK;
}

//==========================================================================================================//
// NAME: VOID UpdateWinAPI(HWND hWnd)
// DESC: WinAPI機能更新処理
//==========================================================================================================//
VOID UpdateWinAPI(HWND hWnd)
{
	POINT lt, rb;
	RECT rect;
	GetClientRect(hWnd, &rect);  // ウインドウクラスクライアント領域を取得

								 // 左上座標をltに保存する
	lt.x = rect.left;
	lt.y = rect.top;
	// 右下座標をrbに保存する
	rb.x = rect.right;
	rb.y = rect.bottom;
	// スクリーン座標に変換する
	ClientToScreen(hWnd, &lt);
	ClientToScreen(hWnd, &rb);

	rect.left = lt.x;
	rect.top = lt.y;
	rect.right = rb.x;
	rect.bottom = rb.y;
	// マウスコンソールはスクリーン内だけ移動可能型
	ClipCursor(&rect);
	// マウスコンソールを隠れる
	ShowCursor(false);
}

//==========================================================================================================//
// NAME: VOID Update();
// DESC: 更新総処理
//==========================================================================================================//
VOID Update()
{
	// 入力更新
	UpdateInput(true);

	// [Imgui]< 2.Imgui更新処理 >
	// Imguiコンソール更新
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize.x = WINDOW_WIDTH;
	io.DisplaySize.y = WINDOW_HEIGHT;

	// Imguiウィンドウ作る
	ImGui::SetNextWindowPos(ImVec2(10, 10));
	ImGui::SetNextWindowSize(ImVec2(380, 100), ImGuiCond_Once);
	ImGui::Begin(u8"情報ウィンドウ");
	ImGui::Text(u8"グラフィックボード: %s", g_strAdapterName);
	ImGui::Text(u8"ゲームFPS: %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::ColorEdit3(u8"ウィンドウ背景色", (float*)&g_BG_color); // Edit 3 floats representing a color
	ImGui::End();

	// コントロール処理
	if (GetKeyboardTrigger(DIK_F1))
	{
		g_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}
	else if (GetKeyboardTrigger(DIK_F2))
	{
		g_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}

	// ゲーム総処理更新
	g_pGame->UpdateGame();
}

//==========================================================================================================//
// NAME: VOID Drawing()
// DESC: 描画処理
//==========================================================================================================//
VOID Render()
{
	D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(g_BG_color.x*255.0f), (int)(g_BG_color.y*255.0f), (int)(g_BG_color.z*255.0f), (int)(g_BG_color.w*255.0f));

	// バックバッファ＆Ｚバッファのクリア
	g_pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), clear_col_dx, 1.0f, 0);

	// Direct3Dによる描画の開始
	if (SUCCEEDED(g_pDevice->BeginScene()))
	{
		// ゲーム総処理描画
		g_pGame->RenderGame();

		// [Imgui]< 3.Imgui描画処理 >
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

		// Direct3Dによる描画の終了
		g_pDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	g_pDevice->Present(NULL, NULL, NULL, NULL);
}

//==========================================================================================================//
// NAME: Uninit();
// DESC: 終了処理
//==========================================================================================================//
VOID Uninit()
{
	// ゲーム総処理終了
	SAFE_DELETE(g_pGame)

	// [Imgui]< 4.Imgui終了処理 >
	ImGui_ImplDX9_Shutdown();
	ImGui::DestroyContext();

	// 入力終了処理
	UninitInput();
	// サウンド終了処理
	UninitSound();

	// デバイスリリース
	SAFE_RELEASE(g_pDevice)
}

//==========================================================================================================//
// NAME: HRESULT CheckAntialias(D3DPRESENT_PARAMETERS d3dpp ,D3DMULTISAMPLE_TYPE AntialiasMode)
// DESC: アンチエイリアシングのチェックと設定
//==========================================================================================================//
HRESULT CheckAntialias(D3DPRESENT_PARAMETERS d3dpp, D3DMULTISAMPLE_TYPE AntialiasMode)
{
	HRESULT hr = E_FAIL;
	DWORD QualityBackBuffer = 0;
	DWORD QualityZBuffer = 0;
	DWORD m = (DWORD)AntialiasMode;

	while (m)
	{
		//レンダリングターゲットでアンチエイリアシングがサポートされているかをチェック
		hr = g_pD3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			d3dpp.BackBufferFormat,
			d3dpp.Windowed,
			(D3DMULTISAMPLE_TYPE)m,
			&QualityBackBuffer);
		if (SUCCEEDED(hr))
		{
			//深度ステンシル サーフェイスでアンチエイリアシングがサポートされているかをチェック
			hr = g_pD3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_HAL,
				d3dpp.AutoDepthStencilFormat,
				d3dpp.Windowed,
				(D3DMULTISAMPLE_TYPE)m,
				&QualityZBuffer);
			if (SUCCEEDED(hr))
			{
				//アンチエイリアシングが使用できるのでD3DPRESENT_PARAMETERSにタイプをセットする。
				d3dpp.MultiSampleType = (D3DMULTISAMPLE_TYPE)m;

				//QualityBackBufferとQualityZBufferで小さい方の値を有効にする。どんなパターンで値が返るのかわからんので、こうしたほうが安全かと。
				if (QualityBackBuffer < QualityZBuffer)
					d3dpp.MultiSampleQuality = QualityBackBuffer - 1;
				else
					d3dpp.MultiSampleQuality = QualityZBuffer - 1;

				break;
			}
		}

		//現在のタイプでアンチエイリアシングが使用できないので、１段階低いタイプで再チェックする。
		m--;
	}

	return hr;
}

//==========================================================================================================//
// NAME: LPDIRECT3DDEVICE9  GetDevice()
// DESC: デバイスを取得関数
//==========================================================================================================//
LPDIRECT3DDEVICE9  GetDevice()
{
	return g_pDevice;
}