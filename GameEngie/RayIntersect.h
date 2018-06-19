
//=============================================================================//
// 
// Name : RayIntersect.h
// Describe : ���C��������
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//
#ifndef _RAYINTERSECT_H_
#define _RAYINTERSECT_H_

//*****************************************************************************//
// (1) �C���N���[�h�t�@�C��
//*****************************************************************************//
#include "D3DUtility.h"				// Directx9�C���N���[�h�t�@�C������

#include "WorldTransform.h"				// ���[���h�g�����X�t�H�[������
//*****************************************************************************//
// (2) �}�N����`
//*****************************************************************************//

//*****************************************************************************//
// (3) ���[�U�[��`�f�[�^�^
//*****************************************************************************//

// { �񋓌^ }

// { �\���� }

// { �N���X }
// ���C�������N���X
class CRay
{
public:
	BOOL bHit;						// �����t���O
	FLOAT fHitDistance;				// ���C�ƌ����_�̋���
	D3DXVECTOR3 vHitNormal;			// �����ʂ̖@��

	D3DXVECTOR3 vStart;				// ���C�̍��W
	D3DXVECTOR3 vDirection;			// ���C�̕���
public:
	CRay();							// �R���X�g���N�^
	~CRay();						// �f�X�g���N�^
public:
	BOOL RayCollision(D3DXVECTOR3 position, D3DXVECTOR3 direction, LPD3DXMESH pTargetMesh, WorldTransform* Target);	// ���b�V���ƃ��C��������
private:
	HRESULT FindVerticesOnPoly(LPD3DXMESH pMesh, DWORD dwPolyIndex, D3DXVECTOR3* pvVertices);		// �����_�v�Z
};
//*****************************************************************************//
// (4) �v���g�^�C�v�錾(�O���ł��g����֐�)
//*****************************************************************************//

#endif