
//=============================================================================//
// 
// Name : SourceManager.h
// Describe : ソース管理
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//
#ifndef _SOURCEMANAGER_H_
#define _SOURCEMANAGER_H_

//*****************************************************************************//
// (1) インクルードファイル
//*****************************************************************************//
#include "D3DUtility.h"				// Directx9インクルードファイル処理

#include "Texturefile.h"			// テクスチャファイル処理
#include "Xfile.h"					// xファイル処理
#include "AnimXfile.h"				// アニメーションxファイル処理
#include "Wavfile.h"				// Wavファイル処理
//*****************************************************************************//
// (2) マクロ定義
//*****************************************************************************//

//*****************************************************************************//
// (3) ユーザー定義データ型
//*****************************************************************************//

// { 列挙型 }
// 読込み状態
enum LoadingState
{
	LS_Ready = 0,		// ソース待機状態
	LS_BeginLoad,		// ソースロード開始(暗転処理)
	LS_Loading,			// ソースロード中
	LS_EndLoad,			// ソースロード終了(暗転回復)
};
// { 構造体 }

// { クラス }

// ソース管理クラス(Singleton)
class SourceManager
{
private:
	static SourceManager*	s_pInstance;						// (Singleton)インスタンス

	LPDIRECT3DDEVICE9		pDevice;							// デバイスオブジェクト
	
	UINT					iLoadingState;						// 読込み状態
	UINT					iLoadingProcess;					// 読込みプロセス

	UINT					iTextureNum;						// テクスチャファイル数
	UINT					iXfileNum;							// xファイルファイル数
	UINT					iAnimXfileNum;						// アニメーションxファイル数
	UINT					iWavfileNum;						// Wavファイル数

	STexturefile*			pTexturefile[MAXNUM_TEXTUREFILE];	// テクスチャクラスへのポインター
	SXfile*					pXfile[MAXNUM_XFILE];				// xファイルクラスへのポインター
	SAnimXfile*				pAnimXfile[MAXNUM_ANIMXFILE];		// アニメーションxファイルクラスへのポインター
	SWavfile*				pWavfile[MAXNUM_WAVFILE];			// Wavファイルへのポインター

	SourceManager();											// コンストラクタ
	~SourceManager();											// デストラクタ
public:
	static SourceManager* GetInstance();						// インスタンスを取得する

	HRESULT InitSourceManager(LPDIRECT3DDEVICE9 D3dDevice);		// ソース管理初期化
	VOID	UninitSourceManager();								// ソース管理解放

	VOID	AddLoadingProcess(int n);							// 読込みプロセス開始
	VOID	ResetLoadingProcess();								// 読込みプロセスリゼット

	// [読み込み] 
	HRESULT LoadTexturefile(int num, char* path, char* name);			// テクスチャを読み込み
	HRESULT LoadXfile(int num, char* path, char* name);					// xファイルを読み込み
	HRESULT LoadAnimXfile(int num, char* path, char* name, int type);	// アニメーションxファイルを読み込み
	HRESULT LoadWavfile(int num, char* path, char* name);				// Wavファイルを読み込み

	// [設置関数]
	VOID SetLoadingState(UINT state);							// 読込み状態設置

	// [取得関数]
	UINT	GetTextureNum() { return iTextureNum; };			// テクスチャファイル数を取得する
	UINT	GetXfileNum() { return iXfileNum; };				// xファイルを取得する
	UINT	GetAnimXfileNum() { return iAnimXfileNum; };		// アニメーションxファイル数を取得する
	UINT	GetWavfileNum() { return iWavfileNum; };			// Wavファイル数を取得する

	STexturefile* GetTextureFile(UINT n);						// テクスチャを取得する
	SXfile* GetXfile(UINT n);									// xファイルを取得する
	SAnimXfile* GetAnimXfile(UINT n);							// アニメーションを取得する
	SWavfile* GetWavfile(UINT n);								// Wavファイルを取得する

	VOID FreeTextureFile(UINT n);						// テクスチャを解放する
	VOID FreeXfile(UINT n);								// xファイルを解放する
	VOID FreeAnimXfile(UINT n);							// アニメーションxファイルを解放する
	VOID FreeWavfile(UINT n);							// Wavファイルを解放する

	UINT GetLoadingState() { return iLoadingState; };			// 読込み状態を取得する
	UINT GetLoadingProcess() { return iLoadingProcess; };		// 読込みプロセスを取得する
};
//*****************************************************************************//
// (4) プロトタイプ宣言(外部でも使える関数)
//*****************************************************************************//

#endif