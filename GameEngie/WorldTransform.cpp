
//=============================================================================//
// 
// Name : WorldTransform.cpp
// Describe : ���[���h�g�����X�t�H�[������
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//

//*****************************************************************************//
// (1) �C���N���[�h�t�@�C��
//*****************************************************************************//
#include "WorldTransform.h"				// ���[���h�g�����X�t�H�[������

//*****************************************************************************//
// (2) �}�N����`
//*****************************************************************************//

//*****************************************************************************//
// (3) ���[�U�[��`�f�[�^�֐� (�N���X�֐�)
//*****************************************************************************//

//-----------------------------------------------------------------------------
// NAME: WorldTransform::WorldTransform()
// DESC: ���[���h�g�����X�t�H�[������ �R���X�g���N�^
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
// DESC: ���[���h�g�����X�t�H�[������ �f�X�g���N�^
//-----------------------------------------------------------------------------
WorldTransform::~WorldTransform()
{

}

//-----------------------------------------------------------------------------
// NAME: VOID WorldTransform::UpdateWorldTransform()
// DESC: ���[���h�g�����X�t�H�[������  ���[���h�}�g���b�N�X�X�V�֐�
//-----------------------------------------------------------------------------
VOID WorldTransform::UpdateWorldTransform()
{
	// ���[���h�g�����X�t�H�[���i���[�J�����W�����[���h���W�ւ̕ϊ�)
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATRIX mtxFixRot, mtxFixTranslate;

	// ���t���[���}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxRotation);
	D3DXMatrixIdentity(&mtxTranslation);

	// (�X�P�[������]������)
	// �X�P�[�����O�i�g��k���j�s����쐬
	D3DXMatrixScaling(&mtxScl, vScaling.x, vScaling.y, vScaling.z);
	D3DXMatrixMultiply(&mtxRotation, &mtxRotation, &mtxScl);

	// ��]�C������v�Z
	if (vFixRotation != D3DXVECTOR3(0, 0, 0))
	{
		D3DXMatrixRotationYawPitchRoll(&mtxFixRot, vFixRotation.y, vFixRotation.x, vFixRotation.z);
		D3DXMatrixMultiply(&mtxRotation, &mtxRotation, &mtxFixRot);
	}

	// ��]�s����쐬
	D3DXMatrixRotationYawPitchRoll(&mtxRot, vRotation.y, vRotation.x, vRotation.z);
	D3DXMatrixMultiply(&mtxRotation, &mtxRotation, &mtxRot);

	// ���s�C������v�Z
	if (vFixPosition != D3DXVECTOR3(0, 0, 0))
	{
		D3DXMatrixTranslation(&mtxFixTranslate, vFixPosition.x, vFixPosition.y, vFixPosition.z);
		D3DXMatrixMultiply(&mtxTranslation, &mtxTranslation, &mtxFixTranslate);
	}

	// ���s�ړ��s����쐬
	D3DXMatrixTranslation(&mtxTranslate, vPosition.x, vPosition.y, vPosition.z);
	D3DXMatrixMultiply(&mtxTranslation, &mtxTranslation, &mtxTranslate);

	// ���[���h�}�g���N�X�v�Z
	D3DXMatrixMultiply(&mtxWorld, &mtxRotation, &mtxTranslation);

	// ��ꎲ�X�V
	D3DXVec3TransformCoord(&vAxisX, &D3DXVECTOR3(1, 0, 0), &mtxRot);
	D3DXVec3TransformCoord(&vAxisY, &D3DXVECTOR3(0, 1, 0), &mtxRot);
	D3DXVec3TransformCoord(&vAxisZ, &D3DXVECTOR3(0, 0, 1), &mtxRot);

	// �p�x�X�V
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
// DESC: ���[���h�g�����X�t�H�[������ �x�N�g���ňړ�����
//-----------------------------------------------------------------------------
VOID WorldTransform::MoveByVector(D3DXVECTOR3 move)
{
	vPosition += move;
}

//-----------------------------------------------------------------------------
// NAME: VOID WorldTransform::MoveByXAxis(float speed)
// DESC: ���[���h�g�����X�t�H�[������ x���Ɉړ�
//-----------------------------------------------------------------------------
VOID WorldTransform::MoveByXAxis(float speed)
{
	vPosition += speed * vAxisX;
}
//-----------------------------------------------------------------------------
// NAME: VOID WorldTransform::MoveByYAxis(float speed)
// DESC: ���[���h�g�����X�t�H�[������ y���Ɉړ�
//-----------------------------------------------------------------------------
VOID WorldTransform::MoveByYAxis(float speed)
{
	vPosition += speed * vAxisY;
}

//-----------------------------------------------------------------------------
// NAME: VOID WorldTransform::MoveByZAxis(float speed)
// DESC: ���[���h�g�����X�t�H�[������ z���Ɉړ�
//-----------------------------------------------------------------------------
VOID WorldTransform::MoveByZAxis(float speed)
{
	vPosition += speed * vAxisZ;
}

//-----------------------------------------------------------------------------
// NAME: VOID WorldTransform::RotationByXAxis(float radian)
// DESC: ���[���h�g�����X�t�H�[������ x���ɒ��S�ɂƂ��ĉ�]
//-----------------------------------------------------------------------------
VOID WorldTransform::RotationByXAxis(float radian)
{
	vRotation.x += D3DXToRadian(radian);
}

//-----------------------------------------------------------------------------
// NAME: VOID WorldTransform::RotationByYAxis(float radian)
// DESC: ���[���h�g�����X�t�H�[������ y���ɒ��S�ɂƂ��ĉ�]
//-----------------------------------------------------------------------------
VOID WorldTransform::RotationByYAxis(float radian)
{
	vRotation.y += D3DXToRadian(radian);
}

//-----------------------------------------------------------------------------
// NAME: VOID WorldTransform::RotationByZAxis(float radian)
// DESC: ���[���h�g�����X�t�H�[������ z���ɒ��S�ɂƂ��ĉ�]
//-----------------------------------------------------------------------------
VOID WorldTransform::RotationByZAxis(float radian)
{
	vRotation.z += D3DXToRadian(radian);
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