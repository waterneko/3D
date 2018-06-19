
//=============================================================================//
// 
// Name : WorldTransform.h
// Describe : ���[���h�g�����X�t�H�[������
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//
#ifndef _WORLDTRANSFORM_H_
#define _WORLDTRANSFORM_H_

//*****************************************************************************//
// (1) �C���N���[�h�t�@�C��
//*****************************************************************************//
#include "D3DUtility.h"				// Directx9�C���N���[�h�t�@�C������

//*****************************************************************************//
// (2) �}�N����`
//*****************************************************************************//

//*****************************************************************************//
// (3) ���[�U�[��`�f�[�^�^
//*****************************************************************************//

// { �񋓌^ }

// { �\���� }

// { �N���X }
// ���[���h�g�����X�t�H�[���N���X
class WorldTransform
{
public:
	D3DXVECTOR3			vAxisX;			// X��ꎲ�x�N�g��
	D3DXVECTOR3			vAxisY;			// Y��ꎲ�x�N�g��
	D3DXVECTOR3			vAxisZ;			// Z��ꎲ�x�N�g��

	D3DXVECTOR3			vScaling;		// �X�P�[�����W
	D3DXVECTOR3			vRotation;		// ��]���W
	D3DXVECTOR3			vPosition;		// ���ڍ��W

	D3DXVECTOR3			vFixRotation;	// �C����]���W
	D3DXVECTOR3			vFixPosition;	// �C����ԍ��W

	D3DXMATRIX			mtxRotation;	// ��]�}�g���N�X
	D3DXMATRIX			mtxTranslation;	// ���ڃ}�g���N�X
	D3DXMATRIX			mtxWorld;		// ���[���h�}�g���N�X
public:
	WorldTransform();
	~WorldTransform();
protected:
	VOID UpdateWorldTransform();			// ���[���h�g�����X�t�H�[��
public:
	// [�ړ��֐�]
	VOID MoveByVector(D3DXVECTOR3 move);	// �x�N�g���ňړ�
	VOID MoveByXAxis(float speed);			// x���Ɉړ�
	VOID MoveByYAxis(float speed);			// y���Ɉړ�
	VOID MoveByZAxis(float speed);			// z���Ɉړ�

	VOID RotationByXAxis(float radian);		// x���ɒ��S�ɂƂ��ĉ�]
	VOID RotationByYAxis(float radian);		// y���ɒ��S�ɂƂ��ĉ�]
	VOID RotationByZAxis(float radian);		// z���ɒ��S�ɂƂ��ĉ�]
};

//*****************************************************************************//
// (4) �v���g�^�C�v�錾(�O���ł��g����֐�)
//*****************************************************************************//

#endif