
//=============================================================================//
// 
// Name : Camera.h
// Describe : カメラ処理
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//
#ifndef _CAMERA_H_
#define _CAMERA_H_

//*****************************************************************************//
// (1) インクルードファイル
//*****************************************************************************//
#include "D3DUtility.h"				// Directx9インクルードファイル処理

//*****************************************************************************//
// (2) マクロ定義
//*****************************************************************************//
// ビュー変換行列 
#define	DEFAULT_CAMERA_RADIUS		(400.0f)				// カメラ初期視点と注視点距離
#define	DEFAULT_CAMERA_ANGLE		(D3DXToRadian(30.0f))	// カメラ初期回転角度

// プロジェクション変換行列
#define	VIEW_ANGLE					(D3DXToRadian(45.0f))	// 視野角
#define	VIEW_NEAR_Z					(10.0f)					// ビュー平面のNearZ値
#define	VIEW_FAR_Z					(20000.0f)				// ビュー平面のFarZ値

//*****************************************************************************//
// (3) ユーザー定義データ型
//*****************************************************************************//

// { 列挙型 }
enum TPCamera_Mode
{
	TP_FreeMoveMode = 0,
	TP_TargetMoveMode
};

// { 構造体 }

// { クラス }

// 主視点カメラ
class BFirstPersonCamera
{
private:
	LPDIRECT3DDEVICE9	pDevice;				// デバイスオブジェクト

	float				fRadius;				// 視点と注視点距離

	D3DXVECTOR3			vPosition;				// 移動用座標
	D3DXVECTOR3			vRotation;				// 回転用座標

	D3DXVECTOR3			vCameraPosition;		// カメラ視点座標
	D3DXVECTOR3			vCameraTarget;			// カメラ注視点座標

	D3DXVECTOR3			vLookVector;			// 視点ベクトル
	D3DXVECTOR3			vRightVector;			// 右方向ベクトル
	D3DXVECTOR3			vUpVector;				// 上方向ベクトル

	D3DXMATRIX			mtxWorld;				// 中心マトリックス
	D3DXMATRIX			mtxView;				// ビューマトリックス
	D3DXMATRIX			mtxProjection;			// プロジェクションマトリックス
public:
	BFirstPersonCamera(LPDIRECT3DDEVICE9 D3dDevice);					// コンストラクタ
	~BFirstPersonCamera();												// デストラクタ

};

// 第三者視点カメラ
class BThirdPersonCamera
{
private:
	LPDIRECT3DDEVICE9	pDevice;				// デバイスオブジェクト

	UINT				iMode;					// カメラモード
	float				fRadius;				// 視点と注視点距離

	D3DXVECTOR3			vRotation;				// 回転用座標

	D3DXVECTOR3			vCameraPosition;		// カメラ視点座標
	D3DXVECTOR3			vCameraTarget;			// カメラ注視点座標

	D3DXVECTOR3			vLookVector;			// 視点ベクトル
	D3DXVECTOR3			vRightVector;			// 右方向ベクトル
	D3DXVECTOR3			vUpVector;				// 上方向ベクトル

	D3DXMATRIX			mtxWorld;				// 中心マトリックス
	D3DXMATRIX			mtxView;				// ビューマトリックス
	D3DXMATRIX			mtxProjection;			// プロジェクションマトリックス
public:
	BThirdPersonCamera(LPDIRECT3DDEVICE9 D3dDevice);					// コンストラクタ
	~BThirdPersonCamera();												// デストラクタ

	VOID UpdateCamera();												// カメラ更新処理
	// [設定関数]
	VOID SetTargetPosition(D3DXVECTOR3 target);							// カメラ注視点座標設定関数
	VOID SetTargetPosition(D3DXVECTOR3 target, float posY);				// カメラ注視点座標設定関数(Y修正)

	VOID SetRotation(D3DXVECTOR3 rot);									// 回転設定関数
	VOID SetRadius(float radius);										// 距離設定関数
	VOID SetViewMatrix();												// ビューマトリックス設定関数
	VOID SetProjMatrix();												// プロジェクションマトリックス設定関数
	VOID SetCameraMode(UINT n) { iMode = n; };							// カメラモード設定

	// [取得関数]
	D3DXVECTOR3 GetCameraPosition() { return vCameraPosition; };			// カメラ視点座標取得関数
	D3DXVECTOR3 GetTargetPosition() { return vCameraTarget; };				// カメラ注視点座標取得関数
	D3DXVECTOR3 GetCameraDirection(bool normalize);							// カメラ方向取得関数
	D3DXVECTOR3 GetLookVector() { return vLookVector; };					// カメラ視点ベクトル得関数
	D3DXVECTOR3 GetRightVector() { return vRightVector; };					// カメラ右方向ベクトル取得関数
	D3DXVECTOR3 GetvUpVector() { return vUpVector; };						// カメラ上方向ベクトル取得関数
	D3DXVECTOR3 GetRotation() { return vRotation; };						// カメラ回転取得関数
	D3DXVECTOR3 GetRotationAngel();											// カメラ回転角度取得関数
	D3DXMATRIX GetCenterMatrix() { return mtxWorld; };						// 中心マトリックス取得関数
	D3DXMATRIX GetViewMatrix() { return mtxView; };							// ビューマトリクス取得関数
	D3DXMATRIX GetProjectionMatrix() { return mtxProjection; };				// プロジェクションマトリックス取得関数

	// [移動関数]
	VOID MoveByVector(D3DXVECTOR3 move);		// ベクトルで移動
	VOID MoveByRightVector(float speed);		// 右方向ベクトルを軸にとって移動
	VOID MoveByUpVector(float speed);			// 上方向ベクトルを軸にとって移動
	VOID MoveByLookVector(float speed);			// 視点ベクトルを軸にとって移動

	VOID RotationByRightVector(float radian);	// 右方向ベクトルを軸にとって回転
	VOID RotationByUpVector(float radian);		// 上方向ベクトルを軸にとって回転
	VOID RotationByLookVector(float radian);	// 視点ベクトルを軸にとって回転

	VOID ResetPosition();						// カメラ偏移座標リセット
	VOID ResetCamera();							// カメラ注視点リセット
	VOID Zoom(float n); 						// カメラ視点拡大縮小処理

	VOID MoveOnTarget(D3DXVECTOR3 Target);		// ターゲットへ移動する

};
//*****************************************************************************//
// (4) プロトタイプ宣言(外部でも使える関数)
//*****************************************************************************//

#endif