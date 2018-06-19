
//=============================================================================//
// 
// Name : WorldTransform.h
// Describe : ワールドトランスフォーム処理
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//
#ifndef _WORLDTRANSFORM_H_
#define _WORLDTRANSFORM_H_

//*****************************************************************************//
// (1) インクルードファイル
//*****************************************************************************//
#include "D3DUtility.h"				// Directx9インクルードファイル処理

//*****************************************************************************//
// (2) マクロ定義
//*****************************************************************************//

//*****************************************************************************//
// (3) ユーザー定義データ型
//*****************************************************************************//

// { 列挙型 }

// { 構造体 }

// { クラス }
// ワールドトランスフォームクラス
class WorldTransform
{
public:
	D3DXVECTOR3			vAxisX;			// X基底軸ベクトル
	D3DXVECTOR3			vAxisY;			// Y基底軸ベクトル
	D3DXVECTOR3			vAxisZ;			// Z基底軸ベクトル

	D3DXVECTOR3			vScaling;		// スケール座標
	D3DXVECTOR3			vRotation;		// 回転座標
	D3DXVECTOR3			vPosition;		// 平移座標

	D3DXVECTOR3			vFixRotation;	// 修正回転座標
	D3DXVECTOR3			vFixPosition;	// 修正空間座標

	D3DXMATRIX			mtxRotation;	// 回転マトリクス
	D3DXMATRIX			mtxTranslation;	// 平移マトリクス
	D3DXMATRIX			mtxWorld;		// ワールドマトリクス
public:
	WorldTransform();
	~WorldTransform();
protected:
	VOID UpdateWorldTransform();			// ワールドトランスフォーム
public:
	// [移動関数]
	VOID MoveByVector(D3DXVECTOR3 move);	// ベクトルで移動
	VOID MoveByXAxis(float speed);			// x軸に移動
	VOID MoveByYAxis(float speed);			// y軸に移動
	VOID MoveByZAxis(float speed);			// z軸に移動

	VOID RotationByXAxis(float radian);		// x軸に中心にとって回転
	VOID RotationByYAxis(float radian);		// y軸に中心にとって回転
	VOID RotationByZAxis(float radian);		// z軸に中心にとって回転
};

//*****************************************************************************//
// (4) プロトタイプ宣言(外部でも使える関数)
//*****************************************************************************//

#endif