
//=============================================================================//
// 
// Name : Xfile.cpp
// Describe : xファイル処理
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//

//*****************************************************************************//
// (1) インクルードファイル
//*****************************************************************************//
#include "Xfile.h"				// xファイル処理

//*****************************************************************************//
// (2) マクロ定義
//*****************************************************************************//

//*****************************************************************************//
// (3) ユーザー定義データ関数 (クラス関数)
//*****************************************************************************//

//-----------------------------------------------------------------------------
// NAME: SXfile::SXfile(LPDIRECT3DDEVICE9 D3dDevice)
// DESC: xファイル処理 コンストラクタ
//-----------------------------------------------------------------------------
SXfile::SXfile(LPDIRECT3DDEVICE9 D3dDevice)
{
	pDevice = D3dDevice;

	pMesh = NULL;
	pMeshMaterials = NULL;
	pMeshTextures = NULL;
	dwNumMaterials = 0;
}

//-----------------------------------------------------------------------------
// NAME: SXfile::~SXfile()
// DESC: xファイル処理 デストラクタ
//-----------------------------------------------------------------------------
SXfile::~SXfile()
{
	// マテリアルへのポインターの解放
	SAFE_DELETE_ARRAY(pMeshMaterials)

	// テクスチャへのポインターの解放
	for (UINT i = 0; i < dwNumMaterials; i++)
	{
		SAFE_RELEASE(pMeshTextures[i])
	}
	SAFE_DELETE_ARRAY(pMeshTextures);

	//メッシュへのポインターの解放
	SAFE_RELEASE(pMesh);
}

//==========================================================================================================//
// NAME: HRESULT SXfile::LoadXfile(char* path, char* name)
// DESC: xファイル処理 xファイル読み込み
//==========================================================================================================//
HRESULT SXfile::LoadXfile(char* path, char* name)
{
	LPD3DXBUFFER pAdjacencyBuffer = NULL;  //隣接性データバッファへのポインター
	LPD3DXBUFFER pD3DXMtrlBuffer = NULL;   //マテリアルバッファへのポインター

	 // ファイルのパス合并処理
	char FilePath[255];
	strcpy_s(FilePath, path);
	strcat_s(FilePath, name);

	// Xファイルからメッシュをロードする	
	if (FAILED(D3DXLoadMeshFromX(FilePath,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		&pAdjacencyBuffer,
		&pD3DXMtrlBuffer,
		NULL,
		&dwNumMaterials,
		&pMesh)))
	{
		MessageBox(NULL, "Xファイルの読み込みに失敗しました", NULL, MB_OK);
		return E_FAIL;
	}

	// マテリアルとテクスチャの初期化
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();

	pMeshMaterials = new D3DMATERIAL9[dwNumMaterials];
	pMeshTextures = new LPDIRECT3DTEXTURE9[dwNumMaterials];

	for (DWORD i = 0; i < dwNumMaterials; i++)
	{
		//マテリアルとライトの設定
		pMeshMaterials[i] = d3dxMaterials[i].MatD3D;
		pMeshMaterials[i].Ambient = pMeshMaterials[i].Diffuse;
		pMeshTextures[i] = NULL;
		if (d3dxMaterials[i].pTextureFilename != NULL &&
			lstrlen(d3dxMaterials[i].pTextureFilename) > 0)
		{
			// テクスチャのパス合并処理
			char textureFname[200];
			strcpy_s(textureFname, path);
			strcat_s(textureFname, d3dxMaterials[i].pTextureFilename);

			if (FAILED(D3DXCreateTextureFromFile(pDevice, textureFname, &pMeshTextures[i])))
			{
				MessageBox(NULL, "Xファイルのテクスチャの読み込みに失敗しました", NULL, MB_OK);
			}
		}
	}

	//メッシュを最適化
	pMesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_STRIPREORDER,
		(DWORD*)pAdjacencyBuffer->GetBufferPointer(), NULL, NULL, NULL);

	pD3DXMtrlBuffer->Release();
	pAdjacencyBuffer->Release();

	return S_OK;
}

//==========================================================================================================//
// NAME: VOID DXfile::SetShadingList(int charter)
// DESC: xファイル処理 シェーディング判断リスト
//==========================================================================================================//
//VOID DXfile::SetShadingList(int charter)
//{
//
//	pShadingList->OutLineList = new BOOL[dwNumMaterials];
//	pShadingList->ToonRenderList = new BOOL[dwNumMaterials];
//
//	for (int i = 0; i < dwNumMaterials; i++)
//	{
//		pShadingList->OutLineList[dwNumMaterials] = true;
//		pShadingList->ToonRenderList[dwNumMaterials] = true;
//	}
//
//	switch (charter)
//	{
//	case Mesh_Angela:
//		pShadingList->OutLineList[1] = false;
//		pShadingList->OutLineList[2] = false;
//		pShadingList->OutLineList[12] = false;
//		pShadingList->OutLineList[13] = false;
//		pShadingList->OutLineList[14] = false;
//		pShadingList->OutLineList[15] = false;
//		pShadingList->OutLineList[16] = false;
//		pShadingList->OutLineList[17] = false;
//		pShadingList->OutLineList[18] = false;
//		pShadingList->OutLineList[19] = false;
//
//		pShadingList->ToonRenderList[1] = false;
//		pShadingList->ToonRenderList[2] = false;
//		pShadingList->ToonRenderList[12] = false;
//
//		break;
//	case Mesh_Yukari:
//		// 0:皮膚 1：服 2：宝石 3.服 4.服 5.リンク 6. スカート 7. 髪 8. 髪 9.髪 10. 顔 11.目(T) 12.? 13. 瞳 14.
//
//		pShadingList->OutLineList[3] = false;
//		pShadingList->OutLineList[10] = false;
//		pShadingList->OutLineList[11] = false;
//		pShadingList->OutLineList[13] = false;
//
//		pShadingList->ToonRenderList[0] = false;
//		pShadingList->ToonRenderList[10] = false;
//		pShadingList->ToonRenderList[13] = false;
//		break;
//	}
//}

//*****************************************************************************//
// (4) グローバル変数:
//*****************************************************************************//

//*****************************************************************************//
// (5) プロトタイプ宣言(自分だけ使える関数)
//*****************************************************************************//

//*****************************************************************************//
// (6) 関数
//*****************************************************************************//