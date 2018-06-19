
//=============================================================================//
// 
// Name : WorldTransform.cpp
// Describe : ワールドトランスフォーム処理
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//

//*****************************************************************************//
// (1) インクルードファイル
//*****************************************************************************//
#include "WorldTransform.h"				// ワールドトランスフォーム処理

//*****************************************************************************//
// (2) マクロ定義
//*****************************************************************************//

//*****************************************************************************//
// (3) ユーザー定義データ関数 (クラス関数)
//*****************************************************************************//

//-----------------------------------------------------------------------------
// NAME: WorldTransform::WorldTransform()
// DESC: ワールドトランスフォーム処理 コンストラクタ
//-----------------------------------------------------------------------------
WorldTransform::WorldTransform()
{
	vAxisX = D3DXVECTOR3(1, 0, 0);
	vAxisY = D3DXVECTOR3(0, 1, 0);
	vAxisZ = D3DXVECTOR3(0, 0, 1);

	vPosition = D3DXVECTOR3(0, 0, 0);
	vRotation = D3DXVECTOR3(0, 0, 0);
	vScaling = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	vFixRotation = D3DXVECTOR3(0, 0, 0);
	vFixPosition = D3DXVECTOR3(0, 0, 0);

	D3DXMatrixIdentity(&mtxRotation);
	D3DXMatrixIdentity(&mtxTranslation);
	D3DXMatrixIdentity(&mtxWorld);
}

//-----------------------------------------------------------------------------
// NAME: WorldTransform::~WorldTransform()
// DESC: ワールドトランスフォーム処理 デストラクタ
//-----------------------------------------------------------------------------
WorldTransform::~WorldTransform()
{

}

//-----------------------------------------------------------------------------
// NAME: VOID WorldTransform::UpdateWorldTransform()
// DESC: ワールドトランスフォーム処理  ワールドマトリックス更新関数
//-----------------------------------------------------------------------------
VOID WorldTransform::UpdateWorldTransform()
{
	// ワールドトランスフォーム（ローカル座標→ワールド座標への変換)
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATRIX mtxFixRot, mtxFixTranslate;

	// 毎フレームマトリックスの初期化
	D3DXMatrixIdentity(&mtxRotation);
	D3DXMatrixIdentity(&mtxTranslation);

	// (スケール＞回転＞平移)
	// スケーリング（拡大縮小）行列を作成
	D3DXMatrixScaling(&mtxScl, vScaling.x, vScaling.y, vScaling.z);
	D3DXMatrixMultiply(&mtxRotation, &mtxRotation, &mtxScl);

	// 回転修正列を計算
	if (vFixRotation != D3DXVECTOR3(0, 0, 0))
	{
		D3DXMatrixRotationYawPitchRoll(&mtxFixRot, vFixRotation.y, vFixRotation.x, vFixRotation.z);
		D3DXMatrixMultiply(&mtxRotation, &mtxRotation, &mtxFixRot);
	}

	// 回転行列を作成
	D3DXMatrixRotationYawPitchRoll(&mtxRot, vRotation.y, vRotation.x, vRotation.z);
	D3DXMatrixMultiply(&mtxRotation, &mtxRotation, &mtxRot);

	// 平行修正列を計算
	if (vFixPosition != D3DXVECTOR3(0, 0, 0))
	{
		D3DXMatrixTranslation(&mtxFixTranslate, vFixPosition.x, vFixPosition.y, vFixPosition.z);
		D3DXMatrixMultiply(&mtxTranslation, &mtxTranslation, &mtxFixTranslate);
	}

	// 平行移動行列を作成
	D3DXMatrixTranslation(&mtxTranslate, vPosition.x, vPosition.y, vPosition.z);
	D3DXMatrixMultiply(&mtxTranslation, &mtxTranslation, &mtxTranslate);

	// ワールドマトリクス計算
	D3DXMatrixMultiply(&mtxWorld, &mtxRotation, &mtxTranslation);

	// 基底軸更新
	D3DXVec3TransformCoord(&vAxisX, &D3DXVECTOR3(1, 0, 0), &mtxRot);
	D3DXVec3TransformCoord(&vAxisY, &D3DXVECTOR3(0, 1, 0), &mtxRot);
	D3DXVec3TransformCoord(&vAxisZ, &D3DXVECTOR3(0, 0, 1), &mtxRot);

	// 角度更新
	if (vRotation.y > D3DX_PI)
	{
		vRotation.y -= D3DX_PI * 2.0f;

	}
	else if (vRotation.y < -D3DX_PI)
	{
		vRotation.y += D3DX_PI * 2.0f;
	}

	if (vRotation.x > D3DX_PI)
	{
		vRotation.x -= D3DX_PI * 2.0f;

	}
	else if (vRotation.x < -D3DX_PI)
	{
		vRotation.x += D3DX_PI * 2.0f;
	}

	if (vRotation.z > D3DX_PI)
	{
		vRotation.z -= D3DX_PI * 2.0f;
	}
	else if (vRotation.z < -D3DX_PI)
	{
		vRotation.z += D3DX_PI * 2.0f;
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID WorldTransform::MoveByVector(D3DXVECTOR3 move)
// DESC: ワールドトランスフォーム処理 ベクトルで移動する
//-----------------------------------------------------------------------------
VOID WorldTransform::MoveByVector(D3DXVECTOR3 move)
{
	vPosition += move;
}

//-----------------------------------------------------------------------------
// NAME: VOID WorldTransform::MoveByXAxis(float speed)
// DESC: ワールドトランスフォーム処理 x軸に移動
//-----------------------------------------------------------------------------
VOID WorldTransform::MoveByXAxis(float speed)
{
	vPosition += speed * vAxisX;
}
//-----------------------------------------------------------------------------
// NAME: VOID WorldTransform::MoveByYAxis(float speed)
// DESC: ワールドトランスフォーム処理 y軸に移動
//-----------------------------------------------------------------------------
VOID WorldTransform::MoveByYAxis(float speed)
{
	vPosition += speed * vAxisY;
}

//-----------------------------------------------------------------------------
// NAME: VOID WorldTransform::MoveByZAxis(float speed)
// DESC: ワールドトランスフォーム処理 z軸に移動
//-----------------------------------------------------------------------------
VOID WorldTransform::MoveByZAxis(float speed)
{
	vPosition += speed * vAxisZ;
}

//-----------------------------------------------------------------------------
// NAME: VOID WorldTransform::RotationByXAxis(float radian)
// DESC: ワールドトランスフォーム処理 x軸に中心にとって回転
//-----------------------------------------------------------------------------
VOID WorldTransform::RotationByXAxis(float radian)
{
	vRotation.x += D3DXToRadian(radian);
}

//-----------------------------------------------------------------------------
// NAME: VOID WorldTransform::RotationByYAxis(float radian)
// DESC: ワールドトランスフォーム処理 y軸に中心にとって回転
//-----------------------------------------------------------------------------
VOID WorldTransform::RotationByYAxis(float radian)
{
	vRotation.y += D3DXToRadian(radian);
}

//-----------------------------------------------------------------------------
// NAME: VOID WorldTransform::RotationByZAxis(float radian)
// DESC: ワールドトランスフォーム処理 z軸に中心にとって回転
//-----------------------------------------------------------------------------
VOID WorldTransform::RotationByZAxis(float radian)
{
	vRotation.z += D3DXToRadian(radian);
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