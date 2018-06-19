
//=============================================================================//
// 
// Name : Game.cpp
// Describe : ゲーム総処理
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//

//*****************************************************************************//
// (1) インクルードファイル
//*****************************************************************************//
#include "Game.h"				// ゲーム総処理

//*****************************************************************************//
// (2) マクロ定義
//*****************************************************************************//

//*****************************************************************************//
// (3) ユーザー定義データ関数 (クラス関数)
//*****************************************************************************//


//-----------------------------------------------------------------------------
// NAME: GameInstance::GameInstance(LPDIRECT3DDEVICE9 D3dDevice)
// DESC: ゲーム総処理 コンストラクタ
//-----------------------------------------------------------------------------
GameInstance::GameInstance(LPDIRECT3DDEVICE9 D3dDevice)
{
	pDevice = D3dDevice;

	// ソース管理を取得する
	pSourceManager = SourceManager::GetInstance();
	pSourceManager->InitSourceManager(pDevice);

	iCurrentState = 0;
	iTargetState = FIRST_GAMESTATE;
	iLoadingTexture = 0;

	// {ゲームタイトル}
	pTitleInstance = new TitleInstance(pDevice);

	// {ロードテクスチャ}
	pGLoadingTexture[0] = new GTexture(pDevice);
	pGLoadingTexture[1] = new GTexture(pDevice);

	// {スクリーンゲージ}
	pLoadingGauge = new GScreenGauge(pDevice);

	// {フェードテクスチャ}
	pFade = new GFadeTexture(pDevice);
}

//-----------------------------------------------------------------------------
// NAME: GameInstance::~GameInstance()
// DESC: ゲーム総処理 デストラクタ
//-----------------------------------------------------------------------------
GameInstance::~GameInstance()
{
	// {ゲームタイトルの解放}
	if (pTitleInstance)
	{
		pTitleInstance->UninitTitle();
		SAFE_DELETE(pTitleInstance)
	}

	// {ゲームステージの解放}
	if (pStageInstance)
	{
		pStageInstance->UninitStage();
		SAFE_DELETE(pStageInstance)
	}
	// {ロードテクスチャの解放}
	SAFE_DELETE(pGLoadingTexture[0])
	// {スクリーンゲージの解放}
	SAFE_DELETE(pLoadingGauge)
	// {フェードテクスチャ}
	SAFE_DELETE(pFade)

	// ソース管理解放
	pSourceManager->UninitSourceManager();
}

//-----------------------------------------------------------------------------
// NAME: HRESULT GameInstance::InitGame()
// DESC: ゲーム総処理 ゲームの初期化
//-----------------------------------------------------------------------------
HRESULT	GameInstance::InitGame()
{
	// {ゲームタイトルロード開始}
	if (pSourceManager->GetLoadingState() == LS_Ready)
	{
		pSourceManager->ResetLoadingProcess();
		pSourceManager->SetLoadingState(LS_BeginLoad);
	}

	if (pSourceManager->GetLoadingState() == LS_BeginLoad)
	{
		pSourceManager->LoadTexturefile(0, PATH_LOADINGTEXTURE, "loading.png");
		pSourceManager->LoadTexturefile(1, PATH_LOADINGTEXTURE, "loading01.png");
		pSourceManager->LoadTexturefile(2, PATH_LOADINGTEXTURE, "loading02.png");
		pSourceManager->LoadTexturefile(3, PATH_LOADINGTEXTURE, "loading03.png");

		// {ロードテクスチャ}
		pGLoadingTexture[0] = new GTexture(pDevice);
		pGLoadingTexture[0]->InitGameTexture(D3DXVECTOR2(420.0f, 160.0f), D3DXVECTOR2(850.0f, 720.0f));

		pGLoadingTexture[1] = new GTexture(pDevice);
		pGLoadingTexture[1]->InitGameTexture(D3DXVECTOR2(1024.0f, 768.0f), D3DXVECTOR2(512.0f, 384.0f));

		// {スクリーンゲージの初期化}
		pLoadingGauge->InitGaugeTexture(D3DXVECTOR2(282.0f, 15.0f), D3DXVECTOR2(512.0f, 660.0f), 0);
		pLoadingGauge->InitGauge(false, MAX_TITLE_PROCESS);

		// {フェードテクスチャの初期化}
		pFade->InitFadeTexture();
		pFade->SetAlpha(255);
	}

	return S_OK;
}

//-----------------------------------------------------------------------------
// NAME: VOID GameInstance::UpdateGame()
// DESC: ゲーム総処理 ゲームの更新
//-----------------------------------------------------------------------------
VOID GameInstance::UpdateGame()
{
	// ゲームの状態移行のフェード処理
	UpdateFade();
	
	// コントローラー更新
	if (pSourceManager->GetLoadingState() == LS_Ready)
	{
		if (iCurrentState == GS_Title)
		{
			// {ゲームタイトルコントローラーの更新}
			pTitleInstance->UpdateController();
		}
		else if (iCurrentState == GS_Game)
		{
			// {ゲームステージコントローラーの更新}
			pStageInstance->UpdateController();
		}
	}

	// ソース読込み状態の更新
	if (pSourceManager->GetLoadingState() == LS_Loading)
	{
		if (iTargetState == GS_Title)
		{
			// {ゲームタイトルの読込み} 
			if (SUCCEEDED(pTitleInstance->LoadTitle()))
			{
				if (pSourceManager->GetLoadingProcess() >= MAX_TITLE_PROCESS)
				{
					// 現在のゲーム状態を更新
					iCurrentState = GS_Title;
					// ゲームステージの解放処理
					if (pStageInstance)
					{
						pStageInstance->UninitStage();
					}
					// ソース待機状態へ移行
					pSourceManager->SetLoadingState(LS_EndLoad);
				}
			}
		}
		else if (iTargetState == GS_Game)
		{
			// {ゲームステージの読込み} 
			if (SUCCEEDED(pStageInstance->LoadStage()))
			{
				// 更新完了
				if (pSourceManager->GetLoadingProcess() >= MAX_STAGE_PROCESS)
				{
					// 現在のゲーム状態を更新
					iCurrentState = GS_Game;
					// タイトルの解放処理
					if (pTitleInstance)
					{
						pTitleInstance->UninitTitle();
					}
					// ソース待機状態へ移行
					pSourceManager->SetLoadingState(LS_EndLoad);
				}
			}
		}
	}
	// ゲームの総更新
	else
	{
		if (iCurrentState == GS_Title)
		{
			// {ゲームタイトルの更新}
			pTitleInstance->UpdateTitle();
			
			// ゲームステージへ移行 
			if (StartCheck())
			{
				switch (pTitleInstance->GetSelectStage())
				{
				case GameStage1:
					SAFE_DELETE(pStageInstance)
					pStageInstance = new Stage1(pDevice);
					break;
				case GameStage2:
					SAFE_DELETE(pStageInstance)
					pStageInstance = new Stage2(pDevice);
					break;
				}
				iTargetState = GS_Game;
				pSourceManager->SetLoadingState(LS_BeginLoad);
			}
		}
		else if (iCurrentState == GS_Game)
		{
			// {ゲームステージの更新} 
			if (pStageInstance->GetCurrentState() == SS_Loading)
			{
				// ステージ読込み
				pStageInstance->LoadStage();
			}
			else
			{
				pStageInstance->UpdateStage();
			}

			// ゲームタイトルへ移行 
			if (ReturnCheck())
			{
				iTargetState = GS_Title;
				pSourceManager->SetLoadingState(LS_BeginLoad);
			}
			// ステージ切替え
			else if (ChangeStageCheck())
			{
				if ((pStageInstance->GetScene() > 0) && (pStageInstance->GetScene() <= 4))
				{
					pStageInstance->UninitStage();
					SAFE_DELETE(pStageInstance)
					pStageInstance = new Stage1(pDevice);
				}
				else if (pStageInstance->GetScene() > 4)
				{
					pStageInstance->UninitStage();
					SAFE_DELETE(pStageInstance)
					pStageInstance = new Stage2(pDevice);
				}

				iCurrentState = GS_GameReload;
				pSourceManager->SetLoadingState(LS_BeginLoad);
			}
		}
	}

	// デバッグ用IMGUI
	DebugImgui();
}

//-----------------------------------------------------------------------------
// NAME: VOID GameInstance::RenderGame()
// DESC: ゲーム総処理 ゲームの描画
//-----------------------------------------------------------------------------
VOID GameInstance::RenderGame()
{

	// ゲームの総描画
	if (iCurrentState == GS_Title)
	{
		// {ゲームタイトルの描画}
		pTitleInstance->RenderTitle();
	}
	else if (iCurrentState == GS_Game)
	{
		// {ゲームステージの描画} 
		pStageInstance->RenderStage();
	}


	// フェードテクスチャの描画
	if (pFade->iFadeState != FS_Ready)
	{
		pFade->RenderFadeTexture(true);
	}

	// ソース読込み状態の描画
	if ((iTargetState != iCurrentState))
	{
		if (pFade->GetAlphaValue() > 128 )
		{
			// {ロードテクスチャの描画}
			if (iLoadingTexture != 0)
			{
				pGLoadingTexture[1]->RenderGameTexture(pSourceManager->GetTextureFile(iLoadingTexture)->GetTexture());
			}

			pGLoadingTexture[0]->RenderGameTexture(pSourceManager->GetTextureFile(0)->GetTexture());

			// {スクリーンゲージの描画}
			if (iLoadingTexture != 0)
			{
				pLoadingGauge->RenderGaugeTexture();
			}
		}
	}
}

//-----------------------------------------------------------------------------
// NAME: BOOL GameInstance::StartCheck()
// DESC: ゲーム総処理 ゲーム開始移行処理
//-----------------------------------------------------------------------------
BOOL GameInstance::StartCheck()
{
	// 移行条件チェック
	if ( (iCurrentState == GS_Title) && (pFade->iFadeState == FS_Ready) )
	{
		// タイトルチェック
		if ((pTitleInstance->GetSelectStage() != 0) && (pTitleInstance->GetCurrentState() == TS_Menu))
		{
			// ロード画面のテクスチャを決める
			iLoadingTexture = rand() % 3 + 1;

			// ロード画面のゲージ初期化
			pLoadingGauge->InitGauge(false, MAX_STAGE_PROCESS);
			return TRUE;
		}
	}

	return FALSE;
}

//-----------------------------------------------------------------------------
// NAME: BOOL GameInstance::ChangeStageCheck()
// DESC: ゲーム総処理 ステージ切替え処理チェック
//-----------------------------------------------------------------------------
BOOL GameInstance::ChangeStageCheck()
{
	// 移行条件チェック
	if ((iCurrentState == GS_Game) && (pFade->iFadeState == FS_Ready))
	{
		// ステージチェック
		if (pStageInstance->GetCurrentState() == SS_ChangeStage)
		{
			// ロード画面のテクスチャを決める
			iLoadingTexture = rand() % 3 + 1;

			// ロード画面のゲージ初期化
			pLoadingGauge->InitGauge(false, MAX_STAGE_PROCESS);
			return TRUE;
		}
	}

	return FALSE;
}

//-----------------------------------------------------------------------------
// NAME: BOOL GameInstance::ReturnCheck()
// DESC: ゲーム総処理 タイトル戻す処理チェック
//-----------------------------------------------------------------------------
BOOL GameInstance::ReturnCheck()
{
	// 移行条件チェック
	if ((iCurrentState == GS_Game) && (pFade->iFadeState == FS_Ready))
	{
		// ステージチェック
		if (pStageInstance->GetCurrentState() == SS_ReturnStart)
		{
			// ロード画面のテクスチャを決める
			iLoadingTexture = rand() % 3 + 1;

			// ロード画面のゲージ初期化
			pLoadingGauge->InitGauge(false, MAX_TITLE_PROCESS);
			return TRUE;
		}
	}

	return FALSE;
}

//-----------------------------------------------------------------------------
// NAME: VOID GameInstance::UpdateFade()
// DESC: ゲーム総処理 ゲームの状態移行の暗転処理
//-----------------------------------------------------------------------------
VOID GameInstance::UpdateFade()
{
	if (iCurrentState != iTargetState)
	{
		// 暗転開始
		if (pSourceManager->GetLoadingState() == LS_BeginLoad)
		{
			// フェードイン開始処理
			if (pFade->iFadeState == FS_Ready)
			{
				pFade->iFadeState = FS_FadeIn;
			}

			// フェードイン終了処理
			if ((pFade->iFadeState == FS_FadeIn) && (pFade->GetAlphaValue() == 255))
			{
				// ソースロード中状態へ移行
				pSourceManager->SetLoadingState(LS_Loading);
			}
		}

		// {スクリーンゲージの更新}
		pLoadingGauge->fTargetValue = (float)pSourceManager->GetLoadingProcess();
		pLoadingGauge->UpdateGaugeTexture(5);
	}

	// 暗転回復
	if (pSourceManager->GetLoadingState() == LS_EndLoad)
	{
		// フェードアウト開始処理
		if (pFade->iFadeState == FS_FadeIn)
		{
			pFade->iFadeState = FS_FadeOut;
		}

		// フェードアウト終了処理
		if ((pFade->iFadeState == FS_FadeOut) && (pFade->GetAlphaValue() == 0))
		{
			// ソース待機状態へ移行
			pSourceManager->SetLoadingState(LS_Ready);
			pSourceManager->ResetLoadingProcess();
			// フェード状態回復
			pFade->iFadeState = FS_Ready;
		}
	}

	// フェードテクスチャの更新
	if (pFade->iFadeState != FS_Ready)
	{
		pFade->UpdateFadeTexture();
	}

}

//-----------------------------------------------------------------------------
// NAME: VOID GameInstance::DebugImgui()
// DESC: ゲーム総処理 デバッグ用IMGUI
//-----------------------------------------------------------------------------
VOID GameInstance::DebugImgui()
{
	ImGui::SetNextWindowPos(ImVec2(10, 120));
	ImGui::SetNextWindowSize(ImVec2(200, 240));
	ImGui::Begin(u8"ゲーム状態");
	ImGui::Text(u8"現在のゲーム状態：%d", iCurrentState);
	ImGui::Text(u8"次のゲーム状態：%d", iTargetState);
	ImGui::Text(u8"ソース管理状態: %d", pSourceManager->GetLoadingState());
	ImGui::Text(u8"ソースプロセス状態: %d", pSourceManager->GetLoadingProcess());
	ImGui::Text(u8"フェード状態: %d", pFade->iFadeState);
	ImGui::Text(u8"フェードアルファ値: %d", pFade->GetAlphaValue());
	ImGui::Text(u8"現在のタイトル状態: %d", pTitleInstance->GetCurrentState());
	ImGui::Text(u8"次のタイトル状態: %d", pTitleInstance->GetTargetState());
	if (pTitleInstance->GetFade())
	{
		ImGui::Text(u8"タイトルフェード状態: %d", pTitleInstance->GetFade()->iFadeState);
		ImGui::Text(u8"タイトルフェードアルファ値: %d", pTitleInstance->GetFade()->GetAlphaValue());
	}
	ImGui::End();
}

//*****************************************************************************//
// (4) グローバル変数:
//*****************************************************************************//

//*****************************************************************************//
// (5) プロトタイプ宣言(自分だけ使える関数)
//*****************************************************************************//

//*****************************************************************************//
// (6) 関数
//*****************************************************************************//