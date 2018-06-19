
//=============================================================================//
// 
// Name : AnimXfile.h
// Describe : アニメーションxファイル処理
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//
#ifndef _ANIMXFILE_H_
#define _ANIMXFILE_H_

//*****************************************************************************//
// (1) インクルードファイル
//*****************************************************************************//
#include "D3DUtility.h"				// Directx9インクルードファイル処理

//*****************************************************************************//
// (2) マクロ定義
//*****************************************************************************//
#define	MAXNUM_ANIMXFILE		(50)							// 最大アニメーションxファイル数

#define	PATH_ANIM_YUKARI		("Data/Animxfile/Yukari/")		// アニメーションxファイルパスYukari
//*****************************************************************************//
// (3) ユーザー定義データ型
//*****************************************************************************//

// { 列挙型 }
// スキンメッシュタイプ
enum SKINMESH_TYPE
{
	D3DNONINDEXED,		// インデックス情報無し
	D3DINDEXED,			// インデックスあり
	D3DINDEXEDHLSLVS,	// インデックスあり + シェーダー用
	NONE
};

// { 構造体 }
// 派生フレーム構造体
struct SkinMesh_XFRAME : public D3DXFRAME
{
	D3DXMATRIX CombinedTransformationMatrix; 	//ボーンのマトリクス
};

// 派生メッシュコンテナー構造体
struct SkinMesh_MESHCONTAINER : public D3DXMESHCONTAINER
{
	LPDIRECT3DTEXTURE9*  ppTextures;            // テクスチャ
	LPD3DXMESH           pOrigMesh;             // 原始メッシュ
	DWORD                NumAttributeGroups;    // 属性グループ
	DWORD                NumInfl;               // 毎頂点の重みの個数（スキンウェイトとは頂点への影響。）
	LPD3DXBUFFER         pBoneCombinationBuf;   // ボーン・テーブル
	D3DXMATRIX**         ppBoneMatrixPtrs;      // ボーンの変換マトリクスの先頭ポインター
	D3DXMATRIX*          pBoneOffsetMatrices;   // ボーンの初期マトリクスのポインター
	DWORD                NumPaletteEntries;     // ボーン最大数
	DWORD                iAttributeSW;          // インデックス情報無しスキンメッシュに SWとHW両方使う設定
	bool                 UseSoftwareVP;         // ソフトウェアで頂点処理するかどうか
};

// シェーディング判断リスト// シェーディング判断リスト
struct AnimShadingList
{
	BOOL* OutLineList;			// 描辺判断リスト
	BOOL* ToonRenderList;		// Toonレンダリング判断リスト
};

// { クラス }
// 派生ボーンメッシュクラス
class AllocateHierarchy : public ID3DXAllocateHierarchy
{
public:
	char*					strPath;
	int						iSkinMeshType;
	D3DXMATRIX*				pBoneMatrices;
	UINT					iNumBoneMatricesMax;
public:
	AllocateHierarchy(char* path, int type);	// コンストラクタ
	~AllocateHierarchy();						// デストラクタ
private:
	HRESULT AllocateName(LPCSTR Name, LPSTR *pNewName);														// ボーンメッシュの名と内存分配
	HRESULT GenerateSkinnedMesh(IDirect3DDevice9* pd3dDevice, SkinMesh_MESHCONTAINER *pMeshContainer);		// スキンメッシュ生成

public:
	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame); 				// フレームを作成する
	STDMETHOD(CreateMeshContainer)(THIS_ LPCSTR Name,      								// メッシュコンテナーを作成する
		CONST D3DXMESHDATA*       pMeshData,
		CONST D3DXMATERIAL*       pMaterials,
		CONST D3DXEFFECTINSTANCE* pEffectInstances,
		DWORD                     NumMaterials,
		CONST DWORD *             pAdjacency,
		LPD3DXSKININFO pSkinInfo,
		LPD3DXMESHCONTAINER *ppNewMeshContainer);
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree); 							//フレームを破棄する
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerBase); 		//メッシュコンテナーを破棄する
};

// アニメーションxファイルクラス
class SAnimXfile
{
private:
	LPDIRECT3DDEVICE9		pDevice;						// デバイスオブジェクト
public:
	AllocateHierarchy*          pAllocateHier;				// 派生ボーンメッシュクラス
	LPD3DXFRAME	                pFrameRoot;					// 根フレーム
	LPD3DXANIMATIONCONTROLLER	pOrigAnimController;		// 原始アニメーションコントローラー
public:
	SAnimXfile(LPDIRECT3DDEVICE9 D3dDevice);					// コンストラクタ
	~SAnimXfile();												// デストラクタ
public:
	HRESULT LoadAnimXfile(char* path, char* name, int type);	// アニメーションxファイルxファイル読み込み
};
//*****************************************************************************//
// (4) プロトタイプ宣言(外部でも使える関数)
//*****************************************************************************//

#endif