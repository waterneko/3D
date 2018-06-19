
//=============================================================================//
// 
// Name : StageScene.cpp
// Describe : ステージシーン処理クラス
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//

//*****************************************************************************//
// (1) インクルードファイル
//*****************************************************************************//
#include "StageScene.h"				// ステージシーン処理

//*****************************************************************************//
// (2) マクロ定義
//*****************************************************************************//

//*****************************************************************************//
// (3) ユーザー定義データ関数 (クラス関数)
//*****************************************************************************//

//-----------------------------------------------------------------------------
// NAME: SceneInstance::SceneInstance()
// DESC: ステージシーン処理 コンストラクタ
//-----------------------------------------------------------------------------
SceneInstance::SceneInstance()
{
	bChangeFlag = false;
	iNextScene = 0;

	// ソース管理を取得する
	pSourceManager = SourceManager::GetInstance();
}

//-----------------------------------------------------------------------------
// NAME: SceneInstance::~SceneInstance()
// DESC: ステージシーン処理 デストラクタ
//-----------------------------------------------------------------------------
SceneInstance::~SceneInstance()
{

}

//-----------------------------------------------------------------------------
// NAME: HRESULT SceneInstance::InitScene()
// DESC: ステージシーン処理 ステージシーン読込み
//-----------------------------------------------------------------------------
HRESULT	SceneInstance::InitScene()
{
	return S_OK;
}

//-----------------------------------------------------------------------------
// NAME: VOID SceneInstance::UpdateController()
// DESC: ステージシーン処理 ステージシーンコントローラーの総更新処理
//-----------------------------------------------------------------------------
VOID SceneInstance::UpdateController()
{

}

//-----------------------------------------------------------------------------
// NAME: VOID SceneInstance::UpdateScene()
// DESC: ステージシーン処理 ステージシーンの総更新処理
//-----------------------------------------------------------------------------
VOID SceneInstance::UpdateScene()
{

}

//-----------------------------------------------------------------------------
// NAME: VOID SceneInstance::RenderStage()
// DESC: ステージシーン処理 ステージシーンの総描画
//-----------------------------------------------------------------------------
VOID SceneInstance::RenderScene()
{

}

//-----------------------------------------------------------------------------
// NAME: VOID SceneInstance::SetChangeSceneFlag(bool change)
// DESC: ステージシーン処理 シーン切り替えフラグ設置
//-----------------------------------------------------------------------------
VOID SceneInstance::SetChangeSceneFlag(bool change)
{
	bChangeFlag = change;
}

//-----------------------------------------------------------------------------
// NAME: VOID SceneInstance::SetNextScene(UINT nextscene)
// DESC: ステージシーン処理 シーン切り替え(0は戻す)
//-----------------------------------------------------------------------------
VOID SceneInstance::SetNextScene(UINT nextscene)
{
	iNextScene = nextscene;
}

//-----------------------------------------------------------------------------
// NAME: LPDIRECT3DTEXTURE9 SceneInstance::SetTexture(UINT n)
// DESC: ステージシーン処理 テクスチャ設置
//-----------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 SceneInstance::SetTexture(UINT n)
{
	if (n <= pSourceManager->GetTextureNum())
	{
		return pSourceManager->GetTextureFile(n)->GetTexture();
	}
	else
	{
		return pSourceManager->GetTextureFile(pSourceManager->GetTextureNum())->GetTexture();
	}
}

//-----------------------------------------------------------------------------
// NAME: LPDIRECT3DTEXTURE9 SceneInstance::SetTexture(UINT n)
// DESC: ステージシーン処理 xファイル設置
//-----------------------------------------------------------------------------
SXfile* SceneInstance::SetXfile(UINT n)
{
	if (n <= pSourceManager->GetTextureNum())
	{
		return pSourceManager->GetXfile(n);
	}
	else
	{
		return pSourceManager->GetXfile(pSourceManager->GetTextureNum());
	}
}

//-----------------------------------------------------------------------------
// NAME: SAnimXfile* SceneInstance::SetAnimXfile(UINT n)
// DESC: ステージシーン処理 アニメーションxファイル設置
//-----------------------------------------------------------------------------
SAnimXfile* SceneInstance::SetAnimXfile(UINT n)
{
	if (n <= pSourceManager->GetAnimXfileNum())
	{
		return pSourceManager->GetAnimXfile(n);
	}
	else
	{
		return pSourceManager->GetAnimXfile(pSourceManager->GetAnimXfileNum());
	}
}

//-----------------------------------------------------------------------------
// NAME: SWavfile* SceneInstance::SetWavfile(UINT n)
// DESC: ゲームステージ処理 Wavファイル設置
//-----------------------------------------------------------------------------
SWavfile* SceneInstance::SetWavfile(UINT n)
{
	if (n <= pSourceManager->GetWavfileNum())
	{
		return pSourceManager->GetWavfile(n);
	}
	else
	{
		return pSourceManager->GetWavfile(pSourceManager->GetWavfileNum());
	}
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