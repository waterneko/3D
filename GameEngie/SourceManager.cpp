
//=============================================================================//
// 
// Name : SourceManager.cpp
// Describe : ソース管理
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//

//*****************************************************************************//
// (1) インクルードファイル
//*****************************************************************************//
#include "SourceManager.h"				//  ソース管理

//*****************************************************************************//
// (2) マクロ定義
//*****************************************************************************//

//*****************************************************************************//
// (3) ユーザー定義データ関数 (クラス関数)
//*****************************************************************************//

//-----------------------------------------------------------------------------
// NAME: SourceManager::SourceManager()
// DESC: ソース管理 コンストラクタ
//-----------------------------------------------------------------------------
SourceManager::SourceManager()
{

}

//-----------------------------------------------------------------------------
// NAME: SourceManager::SourceManager(LPDIRECT3DDEVICE9 D3dDevice)
// DESC: ソース管理 デストラクタ
//-----------------------------------------------------------------------------
SourceManager::~SourceManager()
{

}

//-----------------------------------------------------------------------------
// NAME: SourceManager* SourceManager::GetInstance()
// DESC: ソース管理 インスタンスを取得する
//-----------------------------------------------------------------------------
SourceManager* SourceManager::GetInstance()
{
	return s_pInstance;
}

//-----------------------------------------------------------------------------
// NAME: HRESULT InitSourceManager(LPDIRECT3DDEVICE9 D3dDevice)
// DESC: ソース管理 ソース管理初期化
//-----------------------------------------------------------------------------
HRESULT SourceManager::InitSourceManager(LPDIRECT3DDEVICE9 D3dDevice)
{
	// インスタンス存在判断
	if (s_pInstance != NULL)
	{
		pDevice = D3dDevice;

		iTextureNum = 0;
		iXfileNum = 0;
		iAnimXfileNum = 0;
		iWavfileNum = 0;

		iLoadingState = LS_Ready;
		iLoadingProcess = 0;
	}
	else
	{
		MessageBox(0, "ソース管理初期化を失敗しました", "警告！", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

//-----------------------------------------------------------------------------
// NAME: HRESULT InitSourceManager(LPDIRECT3DDEVICE9 D3dDevice)
// DESC: ソース管理 ソース管理解放
//-----------------------------------------------------------------------------
VOID SourceManager::UninitSourceManager()
{
	for (UINT i = 0; i < iTextureNum + 1; i++)
	{
		SAFE_DELETE(pTexturefile[i])
	}

	for (UINT i = 0; i < iXfileNum + 1; i++)
	{
		SAFE_DELETE(pXfile[i])
	}

	for (UINT i = 0; i < iAnimXfileNum + 1; i++)
	{
		SAFE_DELETE(pAnimXfile[i])
	}

	for (UINT i = 0; i < iWavfileNum + 1; i++)
	{
		SAFE_DELETE(pWavfile[i])
	}
}

//-----------------------------------------------------------------------------
// NAME: HRESULT SourceManager::LoadTexturefile(int num, char* path, char* name)
// DESC: ソース管理 テクスチャを読み込み
//-----------------------------------------------------------------------------
HRESULT SourceManager::LoadTexturefile(int num, char* path, char* name)
{
	// ファイルチェック
	if (pTexturefile[num] == NULL)
	{ 
		pTexturefile[num] = new STexturefile(pDevice);

		if (SUCCEEDED(pTexturefile[num]->LoadTexturefile(path, name)))
		{
			iTextureNum++;
			return S_OK;
		}
		else
		{
			return E_FAIL;
		}
	}
	else
	{
		return S_OK;
	}
}

//-----------------------------------------------------------------------------
// NAME: HRESULT SourceManager::LoadXfile(int num, char* path, char* name)
// DESC: ソース管理 xファイルを読み込みを読み込み
//-----------------------------------------------------------------------------
HRESULT SourceManager::LoadXfile(int num, char* path, char* name)
{
	// ファイルチェック
	if (pXfile[num] == NULL)
	{
		pXfile[num] = new SXfile(pDevice);

		if (SUCCEEDED(pXfile[num]->LoadXfile(path, name)))
		{
			iXfileNum++;
			return S_OK;
		}
		else
		{
			return E_FAIL;
		}
	}
	else
	{
		return S_OK;
	}
}

//-----------------------------------------------------------------------------
// NAME: HRESULT LoadAnimXfile(int num, char* path, char* name, int type)
// DESC: ソース管理 アニメーションxファイルを読み込み
//-----------------------------------------------------------------------------
HRESULT SourceManager::LoadAnimXfile(int num, char* path, char* name, int type)
{
	if (pAnimXfile[num] == NULL)
	{
		pAnimXfile[num] = new SAnimXfile(pDevice);

		if (SUCCEEDED(pAnimXfile[num]->LoadAnimXfile(path, name, type)))
		{
			iAnimXfileNum++;
			return S_OK;
		}
		else
		{
			return E_FAIL;
		}
	}
	else
	{
		return S_OK;
	}
}

//-----------------------------------------------------------------------------
// NAME: HRESULT SourceManager::LoadWavfile(int num, char* path, char* name)
// DESC: ソース管理 Wavファイルを読み込み
//-----------------------------------------------------------------------------
HRESULT SourceManager::LoadWavfile(int num, char* path, char* name)
{
	if (pWavfile[num] == NULL)
	{
		pWavfile[num] = new SWavfile();

		if (SUCCEEDED(pWavfile[num]->LoadWavFile(path, name)))
		{
			iWavfileNum++;
			return S_OK;
		}
		else
		{
			return E_FAIL;
		}
	}
	else
	{
		return S_OK;
	}
}

//-----------------------------------------------------------------------------
// NAME: STexturefile* SourceManager::GetTextureFile(UINT n)
// DESC: ソース管理 テクスチャを取得する
//-----------------------------------------------------------------------------
STexturefile* SourceManager::GetTextureFile(UINT n)
{
	if (n > iTextureNum)
	{
		return pTexturefile[iTextureNum];
	}
	else
	{
		return pTexturefile[n];
	}
}

//-----------------------------------------------------------------------------
// NAME: SXfile* SourceManager::GetXfile(UINT n)
// DESC: ソース管理 xファイルを取得する
//-----------------------------------------------------------------------------
SXfile* SourceManager::GetXfile(UINT n)
{
	if (n > iXfileNum)
	{
		return pXfile[iXfileNum];
	}
	else
	{
		return pXfile[n];
	}
}

//-----------------------------------------------------------------------------
// NAME: SAnimXfile* SourceManager::GetAnimXfile(UINT n)
// DESC: ソース管理 アニメーションxファイルを取得する
//-----------------------------------------------------------------------------
SAnimXfile* SourceManager::GetAnimXfile(UINT n)
{
	if (n > iAnimXfileNum)
	{
		return pAnimXfile[iAnimXfileNum];
	}
	else
	{
		return pAnimXfile[n];
	}
}

//-----------------------------------------------------------------------------
// NAME: SWavfile* SourceManager::GetWavfile(UINT n)
// DESC: ソース管理 Wavファイルを取得する
//-----------------------------------------------------------------------------
SWavfile* SourceManager::GetWavfile(UINT n)
{
	if (n > iWavfileNum)
	{
		return pWavfile[iWavfileNum];
	}
	else
	{
		return pWavfile[n];
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID SourceManager::FreeTextureFile(UINT n)
// DESC: ソース管理 テクスチャを解放する
//-----------------------------------------------------------------------------
VOID SourceManager::FreeTextureFile(UINT n)
{
	if (n <= iTextureNum)
	{
		SAFE_DELETE(pTexturefile[n])
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID SourceManager::FreeTextureFile(UINT n)
// DESC: ソース管理 xファイルを解放する
//-----------------------------------------------------------------------------
VOID SourceManager::FreeXfile(UINT n)
{
	if (n <= iXfileNum)
	{
		SAFE_DELETE(pXfile[n])
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID SourceManager::FreeTextureFile(UINT n)
// DESC: ソース管理 アニメーションxファイルを解放する
//-----------------------------------------------------------------------------
VOID SourceManager::FreeAnimXfile(UINT n)
{
	if (n <= iAnimXfileNum)
	{
		SAFE_DELETE(pAnimXfile[n])
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID SourceManager::FreeWavfile(UINT n)
// DESC: ソース管理 Wavファイルを解放する
//-----------------------------------------------------------------------------
VOID SourceManager::FreeWavfile(UINT n)
{
	if (n <= iWavfileNum)
	{
		SAFE_DELETE(pWavfile[n])
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID SourceManager::SetLoadingState(UINT state)
// DESC: ソース管理 読込み状態設置
//-----------------------------------------------------------------------------
VOID SourceManager::SetLoadingState(UINT state)
{
	iLoadingState = state;
}

//-----------------------------------------------------------------------------
// NAME: VOID SourceManager::AddLoadingProcess(int n)
// DESC: ソース管理 読込みプロセス開始
//-----------------------------------------------------------------------------
VOID SourceManager::AddLoadingProcess(int n)
{
	if (iLoadingState == LS_Loading)
	{
		iLoadingProcess += n;
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID SourceManager::ResetLoadingProcess()
// DESC: ソース管理 読込みプロセスリゼット
//-----------------------------------------------------------------------------
VOID SourceManager::ResetLoadingProcess()
{
	if (iLoadingState != LS_Loading)
	{
		iLoadingProcess = 0;
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