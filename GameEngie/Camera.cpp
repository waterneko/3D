//=============================================================================//
// 
// Name : Camera.cpp
// Describe : カメラ処理
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//

//*****************************************************************************//
// (1) インクルードファイル
//*****************************************************************************//
#include "Camera.h"				// カメラ処理

//*****************************************************************************//
// (2) マクロ定義
//*****************************************************************************//

//*****************************************************************************//
// (3) ユーザー定義データ関数 (クラス関数)
//*****************************************************************************//

//-----------------------------------------------------------------------------
// NAME: BFirstPersonCamera::BFirstPersonCamera(LPDIRECT3DDEVICE9 D3dDevice)
// DESC: 主視点カメラ処理 コンストラクタ
//-----------------------------------------------------------------------------
BFirstPersonCamera::BFirstPersonCamera(LPDIRECT3DDEVICE9 D3dDevice)
{
	pDevice = D3dDevice;

	fRadius = DEFAULT_CAMERA_RADIUS;

	vPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vRotation = D3DXVECTOR3(DEFAULT_CAMERA_ANGLE, 0.0f, 0.0f);

	vCameraPosition = D3DXVECTOR3(0.0f, 0.0f, -DEFAULT_CAMERA_RADIUS);
	vCameraTarget = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	vRightVector = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	vUpVector = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vLookVector = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	D3DXMatrixIdentity(&mtxWorld);
	D3DXMatrixIdentity(&mtxView);
	D3DXMatrixIdentity(&mtxProjection);
}

//-----------------------------------------------------------------------------
// NAME: BFirstPersonCamera::~BFirstPersonCamera()
// DESC: 主視点カメラ処理 デストラクタ
//-----------------------------------------------------------------------------
BFirstPersonCamera::~BFirstPersonCamera()
{

}

//-----------------------------------------------------------------------------
// NAME: BThirdPersonCamera::BThirdPersonCamera(LPDIRECT3DDEVICE9 D3dDevice)
// DESC: 第三者視点カメラ処理 コンストラクタ
//-----------------------------------------------------------------------------
BThirdPersonCamera::BThirdPersonCamera(LPDIRECT3DDEVICE9 D3dDevice)
{
	pDevice = D3dDevice;

	iMode = TP_FreeMoveMode;
	fRadius = DEFAULT_CAMERA_RADIUS;

	vRotation = D3DXVECTOR3(DEFAULT_CAMERA_ANGLE, 0.0f, 0.0f);

	vCameraPosition = D3DXVECTOR3(0.0f, 0.0f, -DEFAULT_CAMERA_RADIUS);
	vCameraTarget = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	vRightVector = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	vUpVector = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vLookVector = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	D3DXMatrixIdentity(&mtxWorld);
	D3DXMatrixIdentity(&mtxView);
	D3DXMatrixIdentity(&mtxProjection);
}

//-----------------------------------------------------------------------------
// NAME: BThirdPersonCamera::~BThirdPersonCamera()
// DESC: 第三者視点カメラ処理 デストラクタ
//-----------------------------------------------------------------------------
BThirdPersonCamera::~BThirdPersonCamera()
{

}

//-----------------------------------------------------------------------------
// NAME: VOID BThirdPersonCamera::SetTargetPosition(D3DXVECTOR3 target)
// DESC: 第三者視点カメラ処理 注視点座標設定
//-----------------------------------------------------------------------------
VOID BThirdPersonCamera::SetTargetPosition(D3DXVECTOR3 target)
{
	if (vCameraPosition == target)
	{
		vCameraTarget = D3DXVECTOR3(target.x, target.y, target.z + 1.0f);
	}
	else
	{
		vCameraTarget = target;
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID BThirdPersonCamera::SetTargetPosition(D3DXVECTOR3 target, float posY)
// DESC: 第三者視点カメラ処理 注視点座標設定
//-----------------------------------------------------------------------------
VOID BThirdPersonCamera::SetTargetPosition(D3DXVECTOR3 target, float posY)
{
	if (vCameraPosition == target)
	{
		vCameraTarget = D3DXVECTOR3(target.x, target.y, target.z + 1.0f);
	}
	else
	{
		vCameraTarget = target;
	}

	vCameraTarget.y += posY;
}

//-----------------------------------------------------------------------------
// NAME: VOID BThirdPersonCamera::SetRotation(D3DXVECTOR3 rot)
// DESC: 第三者視点カメラ処理 回転設定関数
//-----------------------------------------------------------------------------
VOID BThirdPersonCamera::SetRotation(D3DXVECTOR3 rot)
{
	vRotation = rot;
}

//-----------------------------------------------------------------------------
// NAME: VOID BThirdPersonCamera::SetRadius(float radius)
// DESC: 第三者視点カメラ処理 距離設定関数
//-----------------------------------------------------------------------------
VOID BThirdPersonCamera::SetRadius(float radius)
{
	if (radius > 0)
	{
		fRadius = radius;
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID BThirdPersonCamera::UpdateCamera()
// DESC: 第三者視点カメラ処理 カメラ更新処理
//-----------------------------------------------------------------------------
VOID BThirdPersonCamera::UpdateCamera()
{
	D3DXMATRIX mtxRot;

	// 角度更新
	if (vRotation.y >= D3DX_PI)
	{
		vRotation.y -= D3DX_PI * 2.0f; ;

	}
	else if (vRotation.y <= -D3DX_PI)
	{
		vRotation.y += D3DX_PI * 2.0f;;
	}

	// 上下視角制限
	if (vRotation.x >= D3DX_PI / 3)
	{
		vRotation.x = D3DX_PI / 3.0f;
	}
	else if (vRotation.x <= -D3DX_PI / 3)
	{
		vRotation.x = -D3DX_PI / 3.0f;
	}

	if (vRotation.z >= D3DX_PI)
	{
		vRotation.z -= D3DX_PI * 2.0f;
	}
	else if (vRotation.z <= -D3DX_PI)
	{
		vRotation.z += D3DX_PI * 2.0f;
	}

	// 中心マトリックスマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// 回転行列を作成
	D3DXMatrixRotationYawPitchRoll(&mtxRot, vRotation.y, vRotation.x, vRotation.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// ベクトル更新
	D3DXVec3TransformCoord(&vRightVector, &D3DXVECTOR3(1.0f, 0.0f, 0.0f), &mtxWorld);
	D3DXVec3TransformCoord(&vUpVector, &D3DXVECTOR3(0.0f, 1.0f, 0.0f), &mtxWorld);
	D3DXVec3TransformCoord(&vLookVector, &D3DXVECTOR3(0.0f, 0.0f, 1.0f), &mtxWorld);

	// 視点再計算
	vCameraPosition = vCameraTarget - fRadius * vLookVector;
	if (vCameraPosition == vCameraTarget)
	{
		vCameraPosition = vCameraTarget - D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	}
}


//-----------------------------------------------------------------------------
// NAME: VOID BThirdPersonCamera::SetViewMatrix()
// DESC: 第三者視点カメラ処理 ビューマトリックス設定関数
//-----------------------------------------------------------------------------
VOID BThirdPersonCamera::SetViewMatrix()
{
	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&mtxView);

	// 三点で左手ビューマトリックスの作成
	D3DXMatrixLookAtLH(&mtxView,
		&vCameraPosition,		// カメラの視点
		&vCameraTarget,			// カメラの注視点
		&vUpVector);			// カメラの上方向

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &mtxView);
}

//-----------------------------------------------------------------------------
// NAME: VOID BThirdPersonCameraSetProjMatrix()
// DESC: 第三者視点カメラ処理 プロジェクションマトリックス設定関数
//-----------------------------------------------------------------------------
VOID BThirdPersonCamera::SetProjMatrix()
{
	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&mtxProjection);

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&mtxProjection,
		VIEW_ANGLE,										// 視野角
		((float)WINDOW_WIDTH / (float)WINDOW_HEIGHT),	// アスペクト比
		VIEW_NEAR_Z,									// ビュー平面のNearZ値
		VIEW_FAR_Z);									// ビュー平面のFarZ値

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &mtxProjection);
}

//-----------------------------------------------------------------------------
// NAME: D3DXVECTOR3 BThirdPersonCamera::GetCameraDirection(bool normalize)
// DESC: 第三者視点カメラ処理 カメラ方向取得関数
//-----------------------------------------------------------------------------
D3DXVECTOR3 BThirdPersonCamera::GetCameraDirection(bool normalize)
{
	D3DXVECTOR3 vCameraDir;
	// 方向正規化
	if (normalize)
	{
		D3DXVec3Normalize(&vCameraDir, &(vCameraPosition - vCameraTarget));
	}
	else
	{
		vCameraDir = vCameraPosition - vCameraTarget;

	}
	return vCameraDir;
}

//-----------------------------------------------------------------------------
// NAME: D3DXVECTOR3 BThirdPersonCamera::GetRotationAngel()
// DESC: 第三者視点カメラ処理 カメラ回転角度取得関数
//-----------------------------------------------------------------------------
D3DXVECTOR3 BThirdPersonCamera::GetRotationAngel()
{
	D3DXVECTOR3 vRotationAngle;
	vRotationAngle.x = D3DXToDegree(vRotation.x);
	vRotationAngle.y = D3DXToDegree(vRotation.y);
	vRotationAngle.z = D3DXToDegree(vRotation.z);

	return vRotationAngle;
}

//-----------------------------------------------------------------------------
// NAME: VOID BThirdPersonCamera::MoveByVector(D3DXVECTOR3 move)
// DESC: 第三者視点カメラ処理 プロジェクションマトリックス設定関数
//-----------------------------------------------------------------------------
VOID BThirdPersonCamera::MoveByVector(D3DXVECTOR3 move)
{
	vCameraTarget += move;
}

//-----------------------------------------------------------------------------
// NAME: VOID BThirdPersonCamera::MoveByRightVector(float speed)
// DESC: 第三者視点カメラ処理 右方向ベクトルを軸にとって移動
//-----------------------------------------------------------------------------
VOID BThirdPersonCamera::MoveByRightVector(float speed)
{
	vCameraTarget += vRightVector * speed;
}

//-----------------------------------------------------------------------------
// NAME: VOID BThirdPersonCamera::MoveByUpVector(float speed)
// DESC: 第三者視点カメラ処理 上方向ベクトルを軸にとって移動
//-----------------------------------------------------------------------------
VOID BThirdPersonCamera::MoveByUpVector(float speed)
{
	vCameraTarget += vUpVector * speed;
}

//-----------------------------------------------------------------------------
// NAME: VOID BThirdPersonCamera::MoveByLookVector(float speed)
// DESC: 第三者視点カメラ処理 視点ベクトルを軸にとって移動
//-----------------------------------------------------------------------------
VOID BThirdPersonCamera::MoveByLookVector(float speed)
{
	vCameraTarget += vLookVector * speed;
}

//-----------------------------------------------------------------------------
// NAME: VOID BThirdPersonCamera::RotationByRightVector(float radian)
// DESC: 第三者視点カメラ処理 右方向ベクトルを軸にとって回転
//-----------------------------------------------------------------------------
VOID BThirdPersonCamera::RotationByRightVector(float radian)
{
	vRotation.x += D3DXToRadian(radian);
}

//-----------------------------------------------------------------------------
// NAME: VOID BThirdPersonCamera::RotationByUpVector(float radian)
// DESC: 第三者視点カメラ処理 上方向ベクトルを軸にとって回転
//-----------------------------------------------------------------------------
VOID BThirdPersonCamera::RotationByUpVector(float radian)
{
	vRotation.y += D3DXToRadian(radian);
}

//-----------------------------------------------------------------------------
// NAME: VOID BThirdPersonCamera::RotationByLookVector(float radian)
// DESC: 第三者視点カメラ処理 視点ベクトルを軸にとって回転
//-----------------------------------------------------------------------------
VOID BThirdPersonCamera::RotationByLookVector(float radian)
{
	vRotation.z += D3DXToRadian(radian);
}

//-----------------------------------------------------------------------------
// NAME: VOID BThirdPersonCamera::ResetPosition()
// DESC: 第三者視点カメラ処理 注視点リセット
//-----------------------------------------------------------------------------
VOID BThirdPersonCamera::ResetPosition()
{
	vCameraTarget = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//-----------------------------------------------------------------------------
// NAME: VOID BThirdPersonCamera::ResetPosition()
// DESC: 第三者視点カメラ処理 カメラ設定リセット
//-----------------------------------------------------------------------------
VOID BThirdPersonCamera::ResetCamera()
{
	iMode = TP_FreeMoveMode;
	fRadius = DEFAULT_CAMERA_RADIUS;

	vRotation = D3DXVECTOR3(DEFAULT_CAMERA_ANGLE, 0.0f, 0.0f);

	vCameraPosition = D3DXVECTOR3(0.0f, 0.0f, -DEFAULT_CAMERA_RADIUS);
	vCameraTarget = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	vRightVector = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	vUpVector = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vLookVector = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
}

//-----------------------------------------------------------------------------
// NAME: VOID BThirdPersonCamera::RotationByLookVector(float radian)
// DESC: 第三者視点カメラ処理 カメラ視点拡大縮小処理
//-----------------------------------------------------------------------------
VOID BThirdPersonCamera::Zoom(float n)
{
	fRadius += n;

	if (fRadius < 0.1f * DEFAULT_CAMERA_RADIUS)
	{
		fRadius = 0.1f * DEFAULT_CAMERA_RADIUS;
	}
	else if (fRadius > 5.0f * DEFAULT_CAMERA_RADIUS)
	{
		fRadius = 5.0f * DEFAULT_CAMERA_RADIUS;
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID BThirdPersonCamera::MoveOnTarget(D3DXVECTOR3 Target)
// DESC: 第三者視点カメラ処理 ターゲットへ移動する
//-----------------------------------------------------------------------------
VOID BThirdPersonCamera::MoveOnTarget(D3DXVECTOR3 Target)
{
	D3DXVECTOR3 vDir;

	if (iMode == TP_TargetMoveMode)
	{
		if (D3DXVec3Length(&(vCameraTarget - Target)) >= 5.0f)
		{
			D3DXVec3Normalize(&vDir, &(Target - vCameraTarget));
			vCameraTarget += 10.0f* vDir;
		}
		else
		{
			vCameraTarget = Target;
		}
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