//=============================================================================//
// 
// Name : Camera.cpp
// Describe : �J��������
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//

//*****************************************************************************//
// (1) �C���N���[�h�t�@�C��
//*****************************************************************************//
#include "Camera.h"				// �J��������

//*****************************************************************************//
// (2) �}�N����`
//*****************************************************************************//

//*****************************************************************************//
// (3) ���[�U�[��`�f�[�^�֐� (�N���X�֐�)
//*****************************************************************************//

//-----------------------------------------------------------------------------
// NAME: BFirstPersonCamera::BFirstPersonCamera(LPDIRECT3DDEVICE9 D3dDevice)
// DESC: �压�_�J�������� �R���X�g���N�^
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
// DESC: �压�_�J�������� �f�X�g���N�^
//-----------------------------------------------------------------------------
BFirstPersonCamera::~BFirstPersonCamera()
{

}

//-----------------------------------------------------------------------------
// NAME: BThirdPersonCamera::BThirdPersonCamera(LPDIRECT3DDEVICE9 D3dDevice)
// DESC: ��O�Ҏ��_�J�������� �R���X�g���N�^
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
// DESC: ��O�Ҏ��_�J�������� �f�X�g���N�^
//-----------------------------------------------------------------------------
BThirdPersonCamera::~BThirdPersonCamera()
{

}

//-----------------------------------------------------------------------------
// NAME: VOID BThirdPersonCamera::SetTargetPosition(D3DXVECTOR3 target)
// DESC: ��O�Ҏ��_�J�������� �����_���W�ݒ�
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
// DESC: ��O�Ҏ��_�J�������� �����_���W�ݒ�
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
// DESC: ��O�Ҏ��_�J�������� ��]�ݒ�֐�
//-----------------------------------------------------------------------------
VOID BThirdPersonCamera::SetRotation(D3DXVECTOR3 rot)
{
	vRotation = rot;
}

//-----------------------------------------------------------------------------
// NAME: VOID BThirdPersonCamera::SetRadius(float radius)
// DESC: ��O�Ҏ��_�J�������� �����ݒ�֐�
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
// DESC: ��O�Ҏ��_�J�������� �J�����X�V����
//-----------------------------------------------------------------------------
VOID BThirdPersonCamera::UpdateCamera()
{
	D3DXMATRIX mtxRot;

	// �p�x�X�V
	if (vRotation.y >= D3DX_PI)
	{
		vRotation.y -= D3DX_PI * 2.0f; ;

	}
	else if (vRotation.y <= -D3DX_PI)
	{
		vRotation.y += D3DX_PI * 2.0f;;
	}

	// �㉺���p����
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

	// ���S�}�g���b�N�X�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// ��]�s����쐬
	D3DXMatrixRotationYawPitchRoll(&mtxRot, vRotation.y, vRotation.x, vRotation.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// �x�N�g���X�V
	D3DXVec3TransformCoord(&vRightVector, &D3DXVECTOR3(1.0f, 0.0f, 0.0f), &mtxWorld);
	D3DXVec3TransformCoord(&vUpVector, &D3DXVECTOR3(0.0f, 1.0f, 0.0f), &mtxWorld);
	D3DXVec3TransformCoord(&vLookVector, &D3DXVECTOR3(0.0f, 0.0f, 1.0f), &mtxWorld);

	// ���_�Čv�Z
	vCameraPosition = vCameraTarget - fRadius * vLookVector;
	if (vCameraPosition == vCameraTarget)
	{
		vCameraPosition = vCameraTarget - D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	}
}


//-----------------------------------------------------------------------------
// NAME: VOID BThirdPersonCamera::SetViewMatrix()
// DESC: ��O�Ҏ��_�J�������� �r���[�}�g���b�N�X�ݒ�֐�
//-----------------------------------------------------------------------------
VOID BThirdPersonCamera::SetViewMatrix()
{
	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxView);

	// �O�_�ō���r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&mtxView,
		&vCameraPosition,		// �J�����̎��_
		&vCameraTarget,			// �J�����̒����_
		&vUpVector);			// �J�����̏����

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &mtxView);
}

//-----------------------------------------------------------------------------
// NAME: VOID BThirdPersonCameraSetProjMatrix()
// DESC: ��O�Ҏ��_�J�������� �v���W�F�N�V�����}�g���b�N�X�ݒ�֐�
//-----------------------------------------------------------------------------
VOID BThirdPersonCamera::SetProjMatrix()
{
	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&mtxProjection,
		VIEW_ANGLE,										// ����p
		((float)WINDOW_WIDTH / (float)WINDOW_HEIGHT),	// �A�X�y�N�g��
		VIEW_NEAR_Z,									// �r���[���ʂ�NearZ�l
		VIEW_FAR_Z);									// �r���[���ʂ�FarZ�l

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &mtxProjection);
}

//-----------------------------------------------------------------------------
// NAME: D3DXVECTOR3 BThirdPersonCamera::GetCameraDirection(bool normalize)
// DESC: ��O�Ҏ��_�J�������� �J���������擾�֐�
//-----------------------------------------------------------------------------
D3DXVECTOR3 BThirdPersonCamera::GetCameraDirection(bool normalize)
{
	D3DXVECTOR3 vCameraDir;
	// �������K��
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
// DESC: ��O�Ҏ��_�J�������� �J������]�p�x�擾�֐�
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
// DESC: ��O�Ҏ��_�J�������� �v���W�F�N�V�����}�g���b�N�X�ݒ�֐�
//-----------------------------------------------------------------------------
VOID BThirdPersonCamera::MoveByVector(D3DXVECTOR3 move)
{
	vCameraTarget += move;
}

//-----------------------------------------------------------------------------
// NAME: VOID BThirdPersonCamera::MoveByRightVector(float speed)
// DESC: ��O�Ҏ��_�J�������� �E�����x�N�g�������ɂƂ��Ĉړ�
//-----------------------------------------------------------------------------
VOID BThirdPersonCamera::MoveByRightVector(float speed)
{
	vCameraTarget += vRightVector * speed;
}

//-----------------------------------------------------------------------------
// NAME: VOID BThirdPersonCamera::MoveByUpVector(float speed)
// DESC: ��O�Ҏ��_�J�������� ������x�N�g�������ɂƂ��Ĉړ�
//-----------------------------------------------------------------------------
VOID BThirdPersonCamera::MoveByUpVector(float speed)
{
	vCameraTarget += vUpVector * speed;
}

//-----------------------------------------------------------------------------
// NAME: VOID BThirdPersonCamera::MoveByLookVector(float speed)
// DESC: ��O�Ҏ��_�J�������� ���_�x�N�g�������ɂƂ��Ĉړ�
//-----------------------------------------------------------------------------
VOID BThirdPersonCamera::MoveByLookVector(float speed)
{
	vCameraTarget += vLookVector * speed;
}

//-----------------------------------------------------------------------------
// NAME: VOID BThirdPersonCamera::RotationByRightVector(float radian)
// DESC: ��O�Ҏ��_�J�������� �E�����x�N�g�������ɂƂ��ĉ�]
//-----------------------------------------------------------------------------
VOID BThirdPersonCamera::RotationByRightVector(float radian)
{
	vRotation.x += D3DXToRadian(radian);
}

//-----------------------------------------------------------------------------
// NAME: VOID BThirdPersonCamera::RotationByUpVector(float radian)
// DESC: ��O�Ҏ��_�J�������� ������x�N�g�������ɂƂ��ĉ�]
//-----------------------------------------------------------------------------
VOID BThirdPersonCamera::RotationByUpVector(float radian)
{
	vRotation.y += D3DXToRadian(radian);
}

//-----------------------------------------------------------------------------
// NAME: VOID BThirdPersonCamera::RotationByLookVector(float radian)
// DESC: ��O�Ҏ��_�J�������� ���_�x�N�g�������ɂƂ��ĉ�]
//-----------------------------------------------------------------------------
VOID BThirdPersonCamera::RotationByLookVector(float radian)
{
	vRotation.z += D3DXToRadian(radian);
}

//-----------------------------------------------------------------------------
// NAME: VOID BThirdPersonCamera::ResetPosition()
// DESC: ��O�Ҏ��_�J�������� �����_���Z�b�g
//-----------------------------------------------------------------------------
VOID BThirdPersonCamera::ResetPosition()
{
	vCameraTarget = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//-----------------------------------------------------------------------------
// NAME: VOID BThirdPersonCamera::ResetPosition()
// DESC: ��O�Ҏ��_�J�������� �J�����ݒ胊�Z�b�g
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
// DESC: ��O�Ҏ��_�J�������� �J�������_�g��k������
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
// DESC: ��O�Ҏ��_�J�������� �^�[�Q�b�g�ֈړ�����
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
// (4) �O���[�o���ϐ�:
//*****************************************************************************//

//*****************************************************************************//
// (5) �v���g�^�C�v�錾(���������g����֐�)
//*****************************************************************************//

//*****************************************************************************//
// (6) �֐�
//*****************************************************************************//