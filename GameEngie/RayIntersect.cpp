

//=============================================================================//
// 
//Name : RayIntersect.cpp
// Describe :レイ交差処理
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//

//*****************************************************************************//
// (1) インクルードファイル
//*****************************************************************************//
#include "RayIntersect.h"				// レイ交差処理

//*****************************************************************************//
// (2) マクロ定義
//*****************************************************************************//

//*****************************************************************************//
// (3) ユーザー定義データ関数 (クラス関数)
//*****************************************************************************//

//-----------------------------------------------------------------------------
// NAME: CRay::CRay()
// DESC: レイ交差処理 コンストラクタ
//-----------------------------------------------------------------------------
CRay::CRay()
{
	bHit = false;
	fHitDistance = 0;
	vHitNormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vStart = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vDirection = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//-----------------------------------------------------------------------------
// NAME: CRay::~CRay()
// DESC: レイ交差処理 デストラクタ
//-----------------------------------------------------------------------------
CRay::~CRay()
{

}

//-----------------------------------------------------------------------------
// NAME: BOOL CRay::RayCollision(D3DXVECTOR3 position, D3DXVECTOR3 direction, LPD3DXMESH pTargetMesh, WorldTransform* Target)
// DESC: レイ交差処理 メッシュとレイ交差判定
//-----------------------------------------------------------------------------
BOOL CRay::RayCollision(D3DXVECTOR3 position, D3DXVECTOR3 direction, LPD3DXMESH pTargetMesh, WorldTransform* Target)
{
	D3DXMATRIX mtxInverseWorld, mtxInverseRot;
	bHit = false;

	// レイのワールド座標と方向を取得する
	vStart = position;
	// レイの方向
	D3DXVec3Normalize(&vDirection, &direction);

	// ワールド行列の逆行列を計算する
	D3DXMatrixInverse(&mtxInverseWorld, NULL, &Target->mtxWorld);
	D3DXMatrixInverse(&mtxInverseRot, NULL, &Target->mtxRotation);
	// ワールド座標系からローカル座標系からに変換する
	D3DXVec3TransformCoord(&vStart, &vStart, &mtxInverseWorld);
	D3DXVec3TransformCoord(&vDirection, &vDirection, &mtxInverseRot);

	// ローカルで交差判定計算
	DWORD dwPolyIndex;
	D3DXIntersect(pTargetMesh, &vStart, &vDirection, &bHit, &dwPolyIndex, NULL, NULL, &fHitDistance, NULL, NULL);
	if (bHit)
	{
		// 交差しているポリゴンの頂点を見つける
		D3DXVECTOR3 vVertex[3];
		FindVerticesOnPoly(pTargetMesh, dwPolyIndex, vVertex);
		D3DXPLANE p;
		// その頂点から平面方程式を得る
		D3DXPlaneFromPoints(&p, &vVertex[0], &vVertex[1], &vVertex[2]);
		// 平面方程式の係数が法線の成分
		vHitNormal.x = p.a;
		vHitNormal.y = p.b;
		vHitNormal.z = p.c;
		// レイ方向によって法線反転
		if (D3DXVec3Dot(&vDirection, &vHitNormal) > 0)
		{
			vHitNormal = -vHitNormal;
		}
		// ローカル座標系からワールド座標系に変換する
		D3DXVec3TransformCoord(&vHitNormal, &vHitNormal, &Target->mtxRotation);
		D3DXVec3Normalize(&vHitNormal, &vHitNormal);
		return true;
	}
	else
	{
		fHitDistance = 0.0f;
		vHitNormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		return false;
	}
}

//-----------------------------------------------------------------------------
// NAME: HRESULT CRay::FindVerticesOnPoly(LPD3DXMESH pMesh, DWORD dwPolyIndex, D3DXVECTOR3* pvVertices)
// DESC: レイ交差処理 交差点計算
//-----------------------------------------------------------------------------
HRESULT CRay::FindVerticesOnPoly(LPD3DXMESH pMesh, DWORD dwPolyIndex, D3DXVECTOR3* pvVertices)
{
	DWORD dwStride = pMesh->GetNumBytesPerVertex();
	DWORD dwVertexNum = pMesh->GetNumVertices();
	DWORD dwPolyNum = pMesh->GetNumFaces();
	WORD* pwPoly = NULL;
	pMesh->LockIndexBuffer(D3DLOCK_READONLY, (VOID**)&pwPoly);

	BYTE *pbVertices = NULL;
	FLOAT* pfVetices = NULL;
	LPDIRECT3DVERTEXBUFFER9 VB = NULL;
	pMesh->GetVertexBuffer(&VB);
	if (SUCCEEDED(VB->Lock(0, 0, (VOID**)&pbVertices, 0)))
	{
		pfVetices = (FLOAT*)&pbVertices[dwStride*pwPoly[dwPolyIndex * 3]];
		pvVertices[0].x = pfVetices[0];
		pvVertices[0].y = pfVetices[1];
		pvVertices[0].z = pfVetices[2];

		pfVetices = (FLOAT*)&pbVertices[dwStride*pwPoly[dwPolyIndex * 3 + 1]];
		pvVertices[1].x = pfVetices[0];
		pvVertices[1].y = pfVetices[1];
		pvVertices[1].z = pfVetices[2];

		pfVetices = (FLOAT*)&pbVertices[dwStride*pwPoly[dwPolyIndex * 3 + 2]];
		pvVertices[2].x = pfVetices[0];
		pvVertices[2].y = pfVetices[1];
		pvVertices[2].z = pfVetices[2];

		pMesh->UnlockIndexBuffer();
		VB->Unlock();
		VB->Release();
	}
	return S_OK;
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