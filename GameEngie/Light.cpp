
//=============================================================================//
// 
// Name : Light.cpp
// Describe : ライト処理
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//

//*****************************************************************************//
// (1) インクルードファイル
//*****************************************************************************//
#include "Light.h"				// ライト処理

//*****************************************************************************//
// (2) マクロ定義
//*****************************************************************************//

//*****************************************************************************//
// (3) ユーザー定義データ関数 (クラス関数)
//*****************************************************************************//

//-----------------------------------------------------------------------------
// NAME: BLight::BLight(LPDIRECT3DDEVICE9 D3dDevice, int id)
// DESC: ライト処理 コンストラクタ
//-----------------------------------------------------------------------------
BLight::BLight(LPDIRECT3DDEVICE9 D3dDevice, int id)
{
	pDevice = D3dDevice;

	Id = id;
	ZeroMemory(&Light, sizeof(Light));
}

//-----------------------------------------------------------------------------
// NAME: BLight::~BLight()
// DESC: ライト処理 デストラクタ
//-----------------------------------------------------------------------------
BLight::~BLight()
{

}

//-----------------------------------------------------------------------------
// NAME: VOID BLight::InitDirectionalLight(GameLight* Dlight, D3DXVECTOR3 vDir, D3DXCOLOR ambient, D3DXCOLOR diffuse, D3DXCOLOR specular)
// DESC: ライト処理 ディレクショナル・ライト初期化
//-----------------------------------------------------------------------------
VOID BLight::InitDirectionalLight(D3DXVECTOR3 vDir, D3DXCOLOR ambient, D3DXCOLOR diffuse, D3DXCOLOR specular)
{
	// ベクトルの正規化
	// D3DXVec3Normalize(&vDir, &vDir);
	Light.Type = D3DLIGHT_DIRECTIONAL; 					// ライトタイプ
	Light.Ambient = ambient;							// アンビエント
	Light.Diffuse = diffuse;							// ディフェーズ
	Light.Specular = specular;							// スペキュラ
	Light.Direction = vDir; 							// ライト方向ベクトル
}

//-----------------------------------------------------------------------------
// NAME: VOID BLight::InitPointLight(D3DXVECTOR3 vPos, D3DXCOLOR ambient, D3DXCOLOR diffuse, D3DXCOLOR specular, float fRange)
// DESC: ライト処理 ポイント・ライト初期化
//-----------------------------------------------------------------------------
VOID BLight::InitPointLight(D3DXVECTOR3 vPos, D3DXCOLOR ambient, D3DXCOLOR diffuse, D3DXCOLOR specular, float fRange)
{

	Light.Type = D3DLIGHT_POINT;						// ライトタイプ
	Light.Ambient = ambient;							// アンビエント
	Light.Diffuse = diffuse;							// ディフェーズ
	Light.Specular = specular;							// スペキュラ
	Light.Attenuation0 = 1.0f;							// ライトの減衰
	Light.Attenuation1 = 0.0f;							// ライトの減衰
	Light.Attenuation2 = 0.0f;							// ライトの減衰
	Light.Range = fRange;								// ライトの最大射程
	Light.Position = vPos;								// ライト空間座標
}

//-----------------------------------------------------------------------------
// NAME: VOID BLight::InitSpotLight(D3DXVECTOR3 vDir, D3DXVECTOR3 vPos, D3DXCOLOR ambient, D3DXCOLOR diffuse, D3DXCOLOR specular, float fRange)
// DESC: ライト処理 スポット・ライト初期化
//-----------------------------------------------------------------------------
VOID BLight::InitSpotLight(D3DXVECTOR3 vDir, D3DXVECTOR3 vPos, D3DXCOLOR ambient, D3DXCOLOR diffuse, D3DXCOLOR specular, float fRange)
{
	// ベクトルの正規化
	// D3DXVec3Normalize(&vDir, &vDir);
	Light.Type = D3DLIGHT_SPOT;							// ライトタイプ
	Light.Ambient = ambient;							// アンビエント
	Light.Diffuse = diffuse;							// ディフェーズ
	Light.Specular = specular;							// スペキュラ
	Light.Attenuation0 = 1.0f;							// ライトの減衰
	Light.Attenuation1 = 0.0f;							// ライトの減衰
	Light.Attenuation2 = 0.0f;							// ライトの減衰
	Light.Range = fRange;								// ライトの最大射程
	Light.Falloff = 1.0f;
	Light.Phi = D3DX_PI / 3.0f;
	Light.Theta = D3DX_PI / 6.0f;
	Light.Direction = vDir; 							// ライト方向ベクトル
	Light.Position = vPos;								// ライト空間座標

}

//-----------------------------------------------------------------------------
// NAME: VOID BLight::EnableLight()
// DESC: ライト処理 ライト使用関数
//-----------------------------------------------------------------------------
VOID BLight::EnableLight()
{
	// ライトをレンダリングパイプラインに設定
	pDevice->SetLight(Id, &Light);
	// ライトのを使用使用状態に使用
	pDevice->LightEnable(Id, true);
}

//-----------------------------------------------------------------------------
// NAME: VOID BLight::DisableLight()
// DESC: ライト処理 ライト禁止関数
//-----------------------------------------------------------------------------
VOID BLight::DisableLight()
{
	// ライトのを使用使用状態に禁止
	pDevice->LightEnable(Id, false);
}

//-----------------------------------------------------------------------------
// NAME: VOID DLight::SetDiffuse(D3DXCOLOR a)
// DESC: ライト処理 アンビエント設定関数
//-----------------------------------------------------------------------------
VOID BLight::SetAmbient(D3DXCOLOR a)
{
	Light.Ambient = a;
}

//-----------------------------------------------------------------------------
// NAME: VOID DLight::SetDiffuse(D3DXCOLOR d)
// DESC: ライト処理  ディフェーズ設定関数
//-----------------------------------------------------------------------------
VOID BLight::SetDiffuse(D3DXCOLOR d)
{
	Light.Diffuse = d;
}

//-----------------------------------------------------------------------------
// NAME: VOID BLight::SetSpecular(D3DXCOLOR s)
// DESC: ライト処理 スペキュラ設定関数
//-----------------------------------------------------------------------------
VOID BLight::SetSpecular(D3DXCOLOR s)
{
	Light.Specular = s;
}

//-----------------------------------------------------------------------------
// NAME: VOID DLight::EnableLight()
// DESC: ライト処理 ライト空間座標設定関数
//-----------------------------------------------------------------------------
VOID BLight::SetPosition(D3DXVECTOR3 pos)
{
	if ((Light.Type == D3DLIGHT_POINT) || (Light.Type == D3DLIGHT_SPOT))
	{
		Light.Position = pos;
	}
}

//-----------------------------------------------------------------------------
// NAME: VOID DLight::EnableLight()
// DESC: ライト処理 ライト方向ベクトル設定関数
//-----------------------------------------------------------------------------
VOID BLight::SetDirection(D3DXVECTOR3 dir)
{
	if ((Light.Type == D3DLIGHT_DIRECTIONAL) || (Light.Type == D3DLIGHT_SPOT))
	{
		Light.Direction = dir;
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
