
//=============================================================================//
// 
// Name : Light.h
// Describe : ライト処理
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//
#ifndef _LIGHT_H_
#define _LIGHT_H_

//*****************************************************************************//
// (1) インクルードファイル
//*****************************************************************************//
#include "D3DUtility.h"				// Directx9インクルードファイル処理

//*****************************************************************************//
// (2) マクロ定義
//*****************************************************************************//
#define	MAXNUM_LIGHT		(10)					// 最大ライト数

//*****************************************************************************//
// (3) ユーザー定義データ型
//*****************************************************************************//

// { 列挙型 }

// { 構造体 }

// { クラス }

// ライトクラス
class BLight
{
private:
	LPDIRECT3DDEVICE9		pDevice;			// デバイスオブジェクト

	int 					Id;					// ライトのインデックス
	D3DLIGHT9				Light;				// D3Dライト
public:
	BLight(LPDIRECT3DDEVICE9 D3dDevice, int id);		// コンストラクタ
	~BLight();											// デストラクタ

	VOID EnableLight();																			// ライト使用状態設置
	VOID DisableLight();																		// ライト禁止状態設置

	// [初期化関数]
	VOID InitDirectionalLight(D3DXVECTOR3, D3DXCOLOR, D3DXCOLOR, D3DXCOLOR); 					// ディレクショナル・ライト初期化
	VOID InitPointLight(D3DXVECTOR3, D3DXCOLOR, D3DXCOLOR, D3DXCOLOR, float); 					// ポイント・ライト初期化
	VOID InitSpotLight(D3DXVECTOR3, D3DXVECTOR3, D3DXCOLOR, D3DXCOLOR, D3DXCOLOR, float); 		// スポット・ライト初期化

	// [設定関数]
	VOID SetAmbient(D3DXCOLOR a); 										// アンビエント設定関数
	VOID SetDiffuse(D3DXCOLOR d); 										// ディフェーズ設定関数
	VOID SetSpecular(D3DXCOLOR s); 										// スペキュラ設定関数
	VOID SetPosition(D3DXVECTOR3 pos); 									// ライト空間座標設定関数
	VOID SetDirection(D3DXVECTOR3 dir); 								// ライト方向ベクトル設定関数

	// [取得関数]
	D3DXCOLOR GetAmbient() { return D3DXCOLOR(Light.Ambient); };		// アンビエント取得関数
	D3DXCOLOR GetDiffuse() { return D3DXCOLOR(Light.Diffuse); };		// ディフェーズ取得関数
	D3DXCOLOR GetSpecular() { return D3DXCOLOR(Light.Specular); };		// スペキュラ取得関数
	D3DXVECTOR3 GetPosition() { return Light.Position; };				// ライト空間座標取得関数
	D3DXVECTOR3 GetDirection() { return Light.Direction; };				// ライト方向ベクトル取得関数
};
//*****************************************************************************//
// (4) プロトタイプ宣言(外部でも使える関数)
//*****************************************************************************//
#endif