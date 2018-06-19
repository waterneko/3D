
//=============================================================================//
// 
// Name : AnimXfile.cpp
// Describe : アニメーションxファイル処理
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//

//*****************************************************************************//
// (1) インクルードファイル
//*****************************************************************************//
#include "AnimXfile.h"				// アニメーションxファイル処理

//*****************************************************************************//
// (2) マクロ定義
//*****************************************************************************//

//*****************************************************************************//
// (3) ユーザー定義データ関数 (クラス関数)
//*****************************************************************************//

//--------------------------------------------------------------------------------------
// NAME: AllocateHierarchy::AllocateHierarchy(char* path, int type)
// DESC: ボーンメッシュクラス コンストラクタ
//--------------------------------------------------------------------------------------
AllocateHierarchy::AllocateHierarchy(char* path, int type)
{
	strPath = path;
	iSkinMeshType = type;
	pBoneMatrices = NULL;
	iNumBoneMatricesMax = 0;
}

//--------------------------------------------------------------------------------------
// NAME: AllocateHierarchy::~AllocateHierarchy()
// DESC: ボーンメッシュクラス デストラクタ
//--------------------------------------------------------------------------------------
AllocateHierarchy::~AllocateHierarchy()
{

}

//--------------------------------------------------------------------------------------
// NAME: HRESULT AllocateHierarchy:: AllocateName(LPCSTR Name, LPSTR* pNewName)
// DESC: ボーンメッシュクラス ボーンメッシュの名と内存分配
//--------------------------------------------------------------------------------------
HRESULT AllocateHierarchy::AllocateName(LPCSTR Name, LPSTR* pNewName)
{
	UINT cbLength;

	if (Name != NULL)
	{
		cbLength = (UINT)strlen(Name) + 1;
		*pNewName = new CHAR[cbLength];
		if (*pNewName == NULL)
			return E_OUTOFMEMORY;
		memcpy(*pNewName, Name, cbLength * sizeof(CHAR));
	}
	else
	{
		*pNewName = NULL;
	}

	return S_OK;
}

//-----------------------------------------------------------------------------
// NAME: HRESULT AllocateHierarchy::GenerateSkinnedMesh(IDirect3DDevice9* pd3dDevice, SkinMesh_MESHCONTAINER* pMeshContainer)
// DESC: ボーンメッシュクラス スキンメッシュの生成
//-----------------------------------------------------------------------------
HRESULT AllocateHierarchy::GenerateSkinnedMesh(IDirect3DDevice9* pd3dDevice, SkinMesh_MESHCONTAINER *pMeshContainer)
{
	HRESULT hr;

	D3DCAPS9 d3dCaps;
	pd3dDevice->GetDeviceCaps(&d3dCaps);

	// もしスキン情報存在しなかったら、処理終了
	if (pMeshContainer->pSkinInfo == NULL)
		return S_OK;

	SAFE_RELEASE(pMeshContainer->MeshData.pMesh);
	SAFE_RELEASE(pMeshContainer->pBoneCombinationBuf);

	if (iSkinMeshType == D3DNONINDEXED)	// インデックス情報無し
	{
		// ConvertToBlendedMeshでスキンメッシュ情報作成
		if (FAILED(pMeshContainer->pSkinInfo->ConvertToBlendedMesh(
			pMeshContainer->pOrigMesh,
			D3DXMESH_MANAGED | D3DXMESHOPT_VERTEXCACHE,
			pMeshContainer->pAdjacency,
			NULL, NULL, NULL,
			&pMeshContainer->NumInfl,
			&pMeshContainer->NumAttributeGroups,
			&pMeshContainer->pBoneCombinationBuf,
			&pMeshContainer->MeshData.pMesh)))

			return E_FAIL;

		// デバイスマトリクス２つマトリクスブレンディングだけなら、ConvertToBlendedMeshで全部メッシュに処理できない
		// メッシュに２つ部分に分かれて、ハードウェア(HW)で２つマトリクス、他の部分はソフトウェア(SW)で処理する
		LPD3DXBONECOMBINATION rgBoneCombinations = reinterpret_cast<LPD3DXBONECOMBINATION>(
			pMeshContainer->pBoneCombinationBuf->GetBufferPointer());

		// 処理できないマトリクスを探す。
		for (pMeshContainer->iAttributeSW = 0; pMeshContainer->iAttributeSW < pMeshContainer->NumAttributeGroups;
		pMeshContainer->iAttributeSW++)
		{
			DWORD cInfl = 0;

			for (DWORD iInfl = 0; iInfl < pMeshContainer->NumInfl; iInfl++)
			{
				if (rgBoneCombinations[pMeshContainer->iAttributeSW].BoneId[iInfl] != UINT_MAX)
				{
					++cInfl;
				}
			}

			if (cInfl > d3dCaps.MaxVertexBlendMatrices)
			{
				break;
			}
		}

		// SW処理
		if (pMeshContainer->iAttributeSW < pMeshContainer->NumAttributeGroups)
		{
			LPD3DXMESH pMeshTmp;

			hr = pMeshContainer->MeshData.pMesh->CloneMeshFVF(D3DXMESH_SOFTWAREPROCESSING |
				pMeshContainer->MeshData.pMesh->GetOptions(),
				pMeshContainer->MeshData.pMesh->GetFVF(),
				pd3dDevice, &pMeshTmp);
			if (FAILED(hr))
			{
				return E_FAIL;
			}

			pMeshContainer->MeshData.pMesh->Release();
			pMeshContainer->MeshData.pMesh = pMeshTmp;
			pMeshTmp = NULL;
		}
		return S_OK;
	}
	else if (iSkinMeshType == D3DINDEXED)	// インデックスあり
	{
		DWORD NumMaxFaceInfl;
		DWORD Flags = D3DXMESHOPT_VERTEXCACHE;

		LPDIRECT3DINDEXBUFFER9 pIB;
		hr = pMeshContainer->pOrigMesh->GetIndexBuffer(&pIB);
		if (FAILED(hr))
			return E_FAIL;

		hr = pMeshContainer->pSkinInfo->GetMaxFaceInfluences(pIB,
			pMeshContainer->pOrigMesh->GetNumFaces(),
			&NumMaxFaceInfl);

		pIB->Release();
		if (FAILED(hr))
			return E_FAIL;

		// ボーン上限の設置
		// 12 entry palette guarantees that any triangle (4 independent influences per vertex of a tri)
		// can be handled
		NumMaxFaceInfl = min(NumMaxFaceInfl, 12);

		// HW SW判断処理
		if (d3dCaps.MaxVertexBlendMatrixIndex + 1 < NumMaxFaceInfl)
		{
			// HWはインデックス頂点ブレンディング処理できない、SWで処理する
			pMeshContainer->NumPaletteEntries = min(256, pMeshContainer->pSkinInfo->GetNumBones());
			pMeshContainer->UseSoftwareVP = true;
			Flags |= D3DXMESH_SYSTEMMEM;
		}
		else
		{
			// HW処理(最大マトリクスの半分)
			//pMeshContainer->NumPaletteEntries = min((d3dCaps.MaxVertexBlendMatrixIndex + 1) / 2, pMeshContainer->pSkinInfo->GetNumBones());

			// Note :計算数間違いになったら,失敗になります,ここでデバイスと同じする
			pMeshContainer->NumPaletteEntries = min((d3dCaps.MaxVertexBlendMatrixIndex), pMeshContainer->pSkinInfo->GetNumBones());
			pMeshContainer->UseSoftwareVP = false;
			Flags |= D3DXMESH_MANAGED;
		}

		// ConvertToIndexedBlendedMeshでスキンメッシュ情報作成
		hr = pMeshContainer->pSkinInfo->ConvertToIndexedBlendedMesh
			(
				pMeshContainer->pOrigMesh,
				Flags,
				pMeshContainer->NumPaletteEntries,
				pMeshContainer->pAdjacency,
				NULL, NULL, NULL,
				&pMeshContainer->NumInfl,
				&pMeshContainer->NumAttributeGroups,
				&pMeshContainer->pBoneCombinationBuf,
				&pMeshContainer->MeshData.pMesh);

		if (FAILED(hr))
			return E_FAIL;
	}
	else if (iSkinMeshType == D3DINDEXEDHLSLVS)	// インデックスあり + バーテックス・シェーダー使う
	{
		// Get palette size
		// First 9 constants are used for other data.  Each 4x3 matrix takes up 3 constants.
		// (96 - 9) /3 i.e. Maximum constant count - used constants 
		// ボーン上限の設置
		UINT MaxMatrices = 26;
		pMeshContainer->NumPaletteEntries = min(MaxMatrices, pMeshContainer->pSkinInfo->GetNumBones());

		// バーテックス・シェーダーチェック
		DWORD Flags = D3DXMESHOPT_VERTEXCACHE;
		if (d3dCaps.VertexShaderVersion >= D3DVS_VERSION(1, 1))
		{
			Flags |= D3DXMESH_MANAGED;
		}
		else
		{
			MessageBox(NULL, "シェーダーが使えない！", "", MB_OK);
		}

		SAFE_RELEASE(pMeshContainer->MeshData.pMesh);

		// ConvertToIndexedBlendedMeshでスキンメッシュ情報作成
		hr = pMeshContainer->pSkinInfo->ConvertToIndexedBlendedMesh
			(
				pMeshContainer->pOrigMesh,
				Flags,
				pMeshContainer->NumPaletteEntries,
				pMeshContainer->pAdjacency,
				NULL, NULL, NULL,
				&pMeshContainer->NumInfl,
				&pMeshContainer->NumAttributeGroups,
				&pMeshContainer->pBoneCombinationBuf,
				&pMeshContainer->MeshData.pMesh);

		if (FAILED(hr))
			return hr;


		// FVF フォーマット最適化. シェーダー使うかためののバーテックス・シェーダーに合わせ設定
		DWORD NewFVF = (pMeshContainer->MeshData.pMesh->GetFVF() & D3DFVF_POSITION_MASK) | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_LASTBETA_UBYTE4;
		if (NewFVF != pMeshContainer->MeshData.pMesh->GetFVF())
		{
			LPD3DXMESH pMesh;
			hr = pMeshContainer->MeshData.pMesh->CloneMeshFVF(pMeshContainer->MeshData.pMesh->GetOptions(), NewFVF, pd3dDevice, &pMesh);
			if (!FAILED(hr))
			{
				pMeshContainer->MeshData.pMesh->Release();
				pMeshContainer->MeshData.pMesh = pMesh;
				pMesh = NULL;
			}
		}

		D3DVERTEXELEMENT9 pDecl[MAX_FVF_DECL_SIZE];
		LPD3DVERTEXELEMENT9 pDeclCur;
		hr = pMeshContainer->MeshData.pMesh->GetDeclaration(pDecl);
		if (FAILED(hr))
			return hr;

		// バーテックス・シェーダーでUBYTE4型からD3DCOLORを解釈する, だから更新 
		//   NOTE: this cannot be done with CloneMesh, that would convert the UBYTE4 data to float and then to D3DCOLOR
		//          this is more of a "cast" operation
		pDeclCur = pDecl;
		while (pDeclCur->Stream != 0xff)
		{
			if ((pDeclCur->Usage == D3DDECLUSAGE_BLENDINDICES) && (pDeclCur->UsageIndex == 0))
				pDeclCur->Type = D3DDECLTYPE_D3DCOLOR;
			pDeclCur++;
		}

		hr = pMeshContainer->MeshData.pMesh->UpdateSemantics(pDecl);
		if (FAILED(hr))
			return hr;

		// ボーン行列バッファ, サイズの設定
		if (iNumBoneMatricesMax < pMeshContainer->pSkinInfo->GetNumBones())
		{
			iNumBoneMatricesMax = pMeshContainer->pSkinInfo->GetNumBones();

			// ボーン混合行列のメモリ空間再配置
			delete[] pBoneMatrices;
			pBoneMatrices = new D3DXMATRIX[iNumBoneMatricesMax];
			if (pBoneMatrices == NULL)
			{
				hr = E_OUTOFMEMORY;
				return hr;
			}
		}
	}
	else
	{
		MessageBox(NULL, "スキンメッシュ処理方法は未選択", "", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

//-----------------------------------------------------------------------------
// NAME: HRESULT AllocateHierarchy::CreateFrame(LPCSTR Name, LPD3DXFRAME *ppNewFrame)
// DESC: ボーンメッシュクラス フレームの初期化
//-----------------------------------------------------------------------------
HRESULT AllocateHierarchy::CreateFrame(LPCSTR Name, LPD3DXFRAME *ppNewFrame)
{
	HRESULT hr = S_OK;
	// 派生フレーム構造体
	SkinMesh_XFRAME *pFrame;

	*ppNewFrame = NULL;
	// 派生フレームの生成
	pFrame = new SkinMesh_XFRAME;
	if (pFrame == NULL)
	{
		hr = E_OUTOFMEMORY;
		delete pFrame;
		return hr;
	}
	// フレーム名の設定
	hr = AllocateName(Name, &pFrame->Name);
	if (FAILED(hr))
	{
		delete pFrame;
		return hr;
	}

	// マトリクスの初期化
	D3DXMatrixIdentity(&pFrame->TransformationMatrix);
	D3DXMatrixIdentity(&pFrame->CombinedTransformationMatrix);

	pFrame->pMeshContainer = NULL;
	pFrame->pFrameSibling = NULL;
	pFrame->pFrameFirstChild = NULL;

	*ppNewFrame = pFrame;
	pFrame = NULL;

	delete pFrame;
	return hr;
}


//-----------------------------------------------------------------------------
// HRESULT AllocateHierarchy::CreateFrame(LPCSTR Name, LPD3DXFRAME *ppNewFrame)
// ボーンメッシュ処理 メッシュコンテナーの生成
//-----------------------------------------------------------------------------
HRESULT AllocateHierarchy::CreateMeshContainer(LPCSTR Name,
	CONST D3DXMESHDATA *pMeshData,
	CONST D3DXMATERIAL *pMaterials,
	CONST D3DXEFFECTINSTANCE *pEffectInstances,
	DWORD NumMaterials,
	CONST DWORD *pAdjacency,
	LPD3DXSKININFO pSkinInfo,
	LPD3DXMESHCONTAINER *ppNewMeshContainer)
{
	HRESULT hr;
	// 派生メッシュコンテナー
	SkinMesh_MESHCONTAINER *pMeshContainer = NULL;
	UINT NumFaces;
	UINT iMaterial;
	UINT iBone, cBones;
	LPD3DXMESH pMesh = NULL;
	LPDIRECT3DDEVICE9 pDevice = NULL;

	*ppNewMeshContainer = NULL;

	// もしD3DX用MESHを発見されなかったら、失敗になる
	if (pMeshData->Type != D3DXMESHTYPE_MESH)
	{
		hr = E_FAIL;
		return hr;
	}

	// メッシュデータを取得し、そのポインターをpMeshに渡す
	pMesh = pMeshData->pMesh;

	// もしメッシュデータはFVFじゃなく、失敗になる
	if (pMesh->GetFVF() == 0)
	{
		hr = E_FAIL;
		return hr;
	}

	// 派生メッシュコンテナーの初期化
	pMeshContainer = new SkinMesh_MESHCONTAINER;
	if (pMeshContainer == NULL)
	{
		hr = E_OUTOFMEMORY;
		return hr;
	}
	memset(pMeshContainer, 0, sizeof(SkinMesh_MESHCONTAINER));

	// メッシュコンテナー名とメモリを設置する
	hr = AllocateName(Name, &pMeshContainer->Name);
	if (FAILED(hr))
	{
		if (pMeshContainer != NULL)
		{
			DestroyMeshContainer(pMeshContainer);
		}
		return hr;
	}

	pMesh->GetDevice(&pDevice);
	NumFaces = pMesh->GetNumFaces();

	// もし法線存在していない、法線を再生成する(FVFフォーマット)
	if (!(pMesh->GetFVF() & D3DFVF_NORMAL))
	{
		pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

		// コピーメッシュ
		hr = pMesh->CloneMeshFVF(pMesh->GetOptions(),
			pMesh->GetFVF() | D3DFVF_NORMAL,
			pDevice, &pMeshContainer->MeshData.pMesh);
		if (FAILED(hr))
		{
			if (pMeshContainer != NULL)
			{
				DestroyMeshContainer(pMeshContainer);
			}
			return hr;
		}

		// get the new pMesh pointer back out of the mesh container to use
		// NOTE: we do not release pMesh because we do not have a reference to it yet
		pMesh = pMeshContainer->MeshData.pMesh;

		// メッシュに含まれる各頂点の法線を計算する。
		D3DXComputeNormals(pMesh, NULL);
	}
	else  //  もし法線存在している、続きメッシュコンテナーのデータを渡す
	{
		pMeshContainer->MeshData.pMesh = pMesh;
		pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

		pMesh->AddRef();
	}

	// メッシュのマテリアルとテクスチャのメモリ空間を分配する
	pMeshContainer->NumMaterials = max(1, NumMaterials);
	pMeshContainer->pMaterials = new D3DXMATERIAL[pMeshContainer->NumMaterials];
	pMeshContainer->ppTextures = new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials];
	pMeshContainer->pAdjacency = new DWORD[NumFaces * 3];
	if ((pMeshContainer->pAdjacency == NULL) || (pMeshContainer->pMaterials == NULL))
	{
		hr = E_OUTOFMEMORY;
		if (pMeshContainer != NULL)
		{
			DestroyMeshContainer(pMeshContainer);
		}
		return hr;
	}

	memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * NumFaces * 3);
	memset(pMeshContainer->ppTextures, 0, sizeof(LPDIRECT3DTEXTURE9) * pMeshContainer->NumMaterials);

	// メッシュのマテリアルとテクスチャを設置
	if (NumMaterials > 0)
	{
		memcpy(pMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * NumMaterials);

		for (iMaterial = 0; iMaterial < NumMaterials; iMaterial++)
		{
			if (pMeshContainer->pMaterials[iMaterial].pTextureFilename != NULL)
			{
				// テクスチャが存在している
				if (pMeshContainer->pMaterials[iMaterial].pTextureFilename != NULL)
				{
					// テクスチャパスを設定する
					char textureFname[200];
					strcpy_s(textureFname, strPath);
					strcat_s(textureFname, pMeshContainer->pMaterials[iMaterial].pTextureFilename);

					if (FAILED(D3DXCreateTextureFromFileA(pDevice, textureFname,
						&pMeshContainer->ppTextures[iMaterial])))
						pMeshContainer->ppTextures[iMaterial] = NULL;

					// don't remember a pointer into the dynamic memory, just forget the name after loading
					pMeshContainer->pMaterials[iMaterial].pTextureFilename = NULL;
				}
			}
		}
	}
	else // メッシュのデフォルト設置
	{
		pMeshContainer->pMaterials[0].pTextureFilename = NULL;
		memset(&pMeshContainer->pMaterials[0].MatD3D, 0, sizeof(D3DMATERIAL9));
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Specular = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
	}

	// 当該メッシュがスキニング情報を持っている場合（スキンメッシュ固有の処理）
	if (pSkinInfo != NULL)
	{
		// スキン情報とオリジナルメッシュを保存する
		pMeshContainer->pSkinInfo = pSkinInfo;
		pSkinInfo->AddRef();

		pMeshContainer->pOrigMesh = pMesh;
		pMesh->AddRef();

		// ボーンの数を取得する
		cBones = pSkinInfo->GetNumBones();

		// ボーンの初期マトリクスの内存を分配する
		pMeshContainer->pBoneOffsetMatrices = new D3DXMATRIX[cBones];
		if (pMeshContainer->pBoneOffsetMatrices == NULL)
		{
			hr = E_OUTOFMEMORY;
			if (pMeshContainer != NULL)
			{
				DestroyMeshContainer(pMeshContainer);
			}
			return hr;
		}

		// ボーンの初期マトリクスを取得する
		for (iBone = 0; iBone < cBones; iBone++)
		{
			pMeshContainer->pBoneOffsetMatrices[iBone] = *(pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(iBone));
		}

		// スキンメッシュ生成する
		hr = GenerateSkinnedMesh(pDevice, pMeshContainer);
		if (FAILED(hr))
		{
			if (pMeshContainer != NULL)
			{
				DestroyMeshContainer(pMeshContainer);
			}
			return hr;
		}
	}

	// ローカルに生成したメッシュコンテナーポインターのポインターを設置
	*ppNewMeshContainer = pMeshContainer;
	pMeshContainer = NULL;

	// メモリ解放処理
	if (pMeshContainer != NULL)
	{
		DestroyMeshContainer(pMeshContainer);
	}

	return hr;
}

//-----------------------------------------------------------------------------
// HRESULT AllocateHierarchy::DestroyFrame(LPD3DXFRAME pFrameToFree)
// ボーンメッシュ処理 フレームを破棄する
//-----------------------------------------------------------------------------
HRESULT AllocateHierarchy::DestroyFrame(LPD3DXFRAME pFrameToFree)
{
	SAFE_DELETE_ARRAY(pFrameToFree->Name);
	SAFE_DELETE(pFrameToFree);
	return S_OK;
}

//-----------------------------------------------------------------------------
// HRESULT AllocateHierarchy::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
// ボーンメッシュ処理 メッシュコンテナーを破棄する
//-----------------------------------------------------------------------------
HRESULT AllocateHierarchy::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
{
	UINT iMaterial;
	SkinMesh_MESHCONTAINER* pMeshContainer = (SkinMesh_MESHCONTAINER*)pMeshContainerBase;

	SAFE_DELETE_ARRAY(pMeshContainer->Name);
	SAFE_DELETE_ARRAY(pMeshContainer->pAdjacency);
	SAFE_DELETE_ARRAY(pMeshContainer->pMaterials);
	SAFE_DELETE_ARRAY(pMeshContainer->pBoneOffsetMatrices);

	// 全テクスチャ解放処理
	if (pMeshContainer->ppTextures != NULL)
	{
		for (iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++)
		{
			SAFE_RELEASE(pMeshContainer->ppTextures[iMaterial]);
		}
	}

	SAFE_DELETE_ARRAY(pMeshContainer->ppTextures);
	SAFE_DELETE_ARRAY(pMeshContainer->ppBoneMatrixPtrs);
	SAFE_RELEASE(pMeshContainer->pBoneCombinationBuf);
	SAFE_RELEASE(pMeshContainer->MeshData.pMesh);
	SAFE_RELEASE(pMeshContainer->pSkinInfo);
	SAFE_RELEASE(pMeshContainer->pOrigMesh);
	SAFE_DELETE(pMeshContainer);
	return S_OK;
}

//-----------------------------------------------------------------------------
// NAME: SAnimXfile::SAnimXfile(LPDIRECT3DDEVICE9 D3dDevice)
// DESC: アニメーションxファイル処理 コンストラクタ
//-----------------------------------------------------------------------------
SAnimXfile::SAnimXfile(LPDIRECT3DDEVICE9 D3dDevice)
{
	pDevice = D3dDevice;

	pFrameRoot = NULL;
	pOrigAnimController = NULL;
}

//-----------------------------------------------------------------------------
// NAME: SAnimXfile::~SAnimXfile()
// DESC: アニメーションxファイル処理 デストラクタ
//-----------------------------------------------------------------------------
SAnimXfile::~SAnimXfile()
{
	// メッシュとフレームを破棄する
	D3DXFrameDestroy(pFrameRoot, pAllocateHier);

	// アニメーションコントロールを破棄する
	SAFE_RELEASE(pOrigAnimController);
}

//-----------------------------------------------------------------------------
// NAME: HRESULT SAnimXfile::LoadAnimXfile(char* path, char* name, int type)
// DESC: アニメーションxファイル ファイル読み込み
//-----------------------------------------------------------------------------
HRESULT SAnimXfile::LoadAnimXfile(char* path, char* name, int type)
{
	// 派生ボーンメッシュクラス初期化
	pAllocateHier = new AllocateHierarchy(path, type);

	// ファイルのパス合并処理
	char FilePath[255];
	strcpy_s(FilePath, path);
	strcat_s(FilePath, name);

	// Xファイルからアニメーションメッシュを読み込み作成する
	if (FAILED(
		D3DXLoadMeshHierarchyFromX(FilePath,
			D3DXMESH_MANAGED,
			pDevice,             // デバイスオブジェクト
			pAllocateHier,       // ボーンクラス
			NULL,
			&pFrameRoot,         // フレーム構造体
			&pOrigAnimController)))  // アニメーションコントロール
	{
		MessageBox(NULL, "Xファイルの読み込みに失敗しました", FilePath, MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

//-----------------------------------------------------------------------------
// NAME: VOID DAnimXfile::SetShadingList(int charter)
// DESC: アニメーションxファイル シェーディング判断リスト
//-----------------------------------------------------------------------------
//VOID DAnimXfile::SetShadingList(int charter)
//{
//		switch (charter)
//		{
//		case SkinMesh_Angela:
//			break;
//		case SkinMesh_Yukari:
//			pShadingList->OutLineList = new BOOL[14];
//			pShadingList->ToonRenderList = new BOOL[14];
//
//			// 0:皮膚 1：皮膚 2：服(外) 3.服+目 4.服 5.宝石 6. 服(内) 7. ひも 8. 服2 9.リンク 10. スカート 11.前髪 12.後髪 13. 両髪 
//
//			for (int i = 0; i < 14; i++)
//			{
//				pShadingList->OutLineList[i] = true;
//				pShadingList->ToonRenderList[i] = true;
//			}
//
//			pShadingList->OutLineList[0] = false;
//			pShadingList->OutLineList[6] = false;
//			pShadingList->OutLineList[9] = false;
//			//pShadingList->OutLineList[11] = false;
//			//pShadingList->OutLineList[13] = false;
//
//			pShadingList->ToonRenderList[0] = false;
//			pShadingList->ToonRenderList[1] = false;
//			break;
//		}
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