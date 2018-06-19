

//=============================================================================//
// 
//Name : RayIntersect.cpp
// Describe :���C��������
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//

//*****************************************************************************//
// (1) �C���N���[�h�t�@�C��
//*****************************************************************************//
#include "RayIntersect.h"				// ���C��������

//*****************************************************************************//
// (2) �}�N����`
//*****************************************************************************//

//*****************************************************************************//
// (3) ���[�U�[��`�f�[�^�֐� (�N���X�֐�)
//*****************************************************************************//

//-----------------------------------------------------------------------------
// NAME: CRay::CRay()
// DESC: ���C�������� �R���X�g���N�^
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
// DESC: ���C�������� �f�X�g���N�^
//-----------------------------------------------------------------------------
CRay::~CRay()
{

}

//-----------------------------------------------------------------------------
// NAME: BOOL CRay::RayCollision(D3DXVECTOR3 position, D3DXVECTOR3 direction, LPD3DXMESH pTargetMesh, WorldTransform* Target)
// DESC: ���C�������� ���b�V���ƃ��C��������
//-----------------------------------------------------------------------------
BOOL CRay::RayCollision(D3DXVECTOR3 position, D3DXVECTOR3 direction, LPD3DXMESH pTargetMesh, WorldTransform* Target)
{
	D3DXMATRIX mtxInverseWorld, mtxInverseRot;
	bHit = false;

	// ���C�̃��[���h���W�ƕ������擾����
	vStart = position;
	// ���C�̕���
	D3DXVec3Normalize(&vDirection, &direction);

	// ���[���h�s��̋t�s����v�Z����
	D3DXMatrixInverse(&mtxInverseWorld, NULL, &Target->mtxWorld);
	D3DXMatrixInverse(&mtxInverseRot, NULL, &Target->mtxRotation);
	// ���[���h���W�n���烍�[�J�����W�n����ɕϊ�����
	D3DXVec3TransformCoord(&vStart, &vStart, &mtxInverseWorld);
	D3DXVec3TransformCoord(&vDirection, &vDirection, &mtxInverseRot);

	// ���[�J���Ō�������v�Z
	DWORD dwPolyIndex;
	D3DXIntersect(pTargetMesh, &vStart, &vDirection, &bHit, &dwPolyIndex, NULL, NULL, &fHitDistance, NULL, NULL);
	if (bHit)
	{
		// �������Ă���|���S���̒��_��������
		D3DXVECTOR3 vVertex[3];
		FindVerticesOnPoly(pTargetMesh, dwPolyIndex, vVertex);
		D3DXPLANE p;
		// ���̒��_���畽�ʕ������𓾂�
		D3DXPlaneFromPoints(&p, &vVertex[0], &vVertex[1], &vVertex[2]);
		// ���ʕ������̌W�����@���̐���
		vHitNormal.x = p.a;
		vHitNormal.y = p.b;
		vHitNormal.z = p.c;
		// ���C�����ɂ���Ė@�����]
		if (D3DXVec3Dot(&vDirection, &vHitNormal) > 0)
		{
			vHitNormal = -vHitNormal;
		}
		// ���[�J�����W�n���烏�[���h���W�n�ɕϊ�����
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
// DESC: ���C�������� �����_�v�Z
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
// (4) �O���[�o���ϐ�:
//*****************************************************************************//

//*****************************************************************************//
// (5) �v���g�^�C�v�錾(���������g����֐�)
//*****************************************************************************//

//*****************************************************************************//
// (6) �֐�
//*****************************************************************************//