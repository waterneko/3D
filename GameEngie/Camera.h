
//=============================================================================//
// 
// Name : Camera.h
// Describe : �J��������
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//
#ifndef _CAMERA_H_
#define _CAMERA_H_

//*****************************************************************************//
// (1) �C���N���[�h�t�@�C��
//*****************************************************************************//
#include "D3DUtility.h"				// Directx9�C���N���[�h�t�@�C������

//*****************************************************************************//
// (2) �}�N����`
//*****************************************************************************//
// �r���[�ϊ��s�� 
#define	DEFAULT_CAMERA_RADIUS		(400.0f)				// �J�����������_�ƒ����_����
#define	DEFAULT_CAMERA_ANGLE		(D3DXToRadian(30.0f))	// �J����������]�p�x

// �v���W�F�N�V�����ϊ��s��
#define	VIEW_ANGLE					(D3DXToRadian(45.0f))	// ����p
#define	VIEW_NEAR_Z					(10.0f)					// �r���[���ʂ�NearZ�l
#define	VIEW_FAR_Z					(20000.0f)				// �r���[���ʂ�FarZ�l

//*****************************************************************************//
// (3) ���[�U�[��`�f�[�^�^
//*****************************************************************************//

// { �񋓌^ }
enum TPCamera_Mode
{
	TP_FreeMoveMode = 0,
	TP_TargetMoveMode
};

// { �\���� }

// { �N���X }

// �压�_�J����
class BFirstPersonCamera
{
private:
	LPDIRECT3DDEVICE9	pDevice;				// �f�o�C�X�I�u�W�F�N�g

	float				fRadius;				// ���_�ƒ����_����

	D3DXVECTOR3			vPosition;				// �ړ��p���W
	D3DXVECTOR3			vRotation;				// ��]�p���W

	D3DXVECTOR3			vCameraPosition;		// �J�������_���W
	D3DXVECTOR3			vCameraTarget;			// �J���������_���W

	D3DXVECTOR3			vLookVector;			// ���_�x�N�g��
	D3DXVECTOR3			vRightVector;			// �E�����x�N�g��
	D3DXVECTOR3			vUpVector;				// ������x�N�g��

	D3DXMATRIX			mtxWorld;				// ���S�}�g���b�N�X
	D3DXMATRIX			mtxView;				// �r���[�}�g���b�N�X
	D3DXMATRIX			mtxProjection;			// �v���W�F�N�V�����}�g���b�N�X
public:
	BFirstPersonCamera(LPDIRECT3DDEVICE9 D3dDevice);					// �R���X�g���N�^
	~BFirstPersonCamera();												// �f�X�g���N�^

};

// ��O�Ҏ��_�J����
class BThirdPersonCamera
{
private:
	LPDIRECT3DDEVICE9	pDevice;				// �f�o�C�X�I�u�W�F�N�g

	UINT				iMode;					// �J�������[�h
	float				fRadius;				// ���_�ƒ����_����

	D3DXVECTOR3			vRotation;				// ��]�p���W

	D3DXVECTOR3			vCameraPosition;		// �J�������_���W
	D3DXVECTOR3			vCameraTarget;			// �J���������_���W

	D3DXVECTOR3			vLookVector;			// ���_�x�N�g��
	D3DXVECTOR3			vRightVector;			// �E�����x�N�g��
	D3DXVECTOR3			vUpVector;				// ������x�N�g��

	D3DXMATRIX			mtxWorld;				// ���S�}�g���b�N�X
	D3DXMATRIX			mtxView;				// �r���[�}�g���b�N�X
	D3DXMATRIX			mtxProjection;			// �v���W�F�N�V�����}�g���b�N�X
public:
	BThirdPersonCamera(LPDIRECT3DDEVICE9 D3dDevice);					// �R���X�g���N�^
	~BThirdPersonCamera();												// �f�X�g���N�^

	VOID UpdateCamera();												// �J�����X�V����
	// [�ݒ�֐�]
	VOID SetTargetPosition(D3DXVECTOR3 target);							// �J���������_���W�ݒ�֐�
	VOID SetTargetPosition(D3DXVECTOR3 target, float posY);				// �J���������_���W�ݒ�֐�(Y�C��)

	VOID SetRotation(D3DXVECTOR3 rot);									// ��]�ݒ�֐�
	VOID SetRadius(float radius);										// �����ݒ�֐�
	VOID SetViewMatrix();												// �r���[�}�g���b�N�X�ݒ�֐�
	VOID SetProjMatrix();												// �v���W�F�N�V�����}�g���b�N�X�ݒ�֐�
	VOID SetCameraMode(UINT n) { iMode = n; };							// �J�������[�h�ݒ�

	// [�擾�֐�]
	D3DXVECTOR3 GetCameraPosition() { return vCameraPosition; };			// �J�������_���W�擾�֐�
	D3DXVECTOR3 GetTargetPosition() { return vCameraTarget; };				// �J���������_���W�擾�֐�
	D3DXVECTOR3 GetCameraDirection(bool normalize);							// �J���������擾�֐�
	D3DXVECTOR3 GetLookVector() { return vLookVector; };					// �J�������_�x�N�g�����֐�
	D3DXVECTOR3 GetRightVector() { return vRightVector; };					// �J�����E�����x�N�g���擾�֐�
	D3DXVECTOR3 GetvUpVector() { return vUpVector; };						// �J����������x�N�g���擾�֐�
	D3DXVECTOR3 GetRotation() { return vRotation; };						// �J������]�擾�֐�
	D3DXVECTOR3 GetRotationAngel();											// �J������]�p�x�擾�֐�
	D3DXMATRIX GetCenterMatrix() { return mtxWorld; };						// ���S�}�g���b�N�X�擾�֐�
	D3DXMATRIX GetViewMatrix() { return mtxView; };							// �r���[�}�g���N�X�擾�֐�
	D3DXMATRIX GetProjectionMatrix() { return mtxProjection; };				// �v���W�F�N�V�����}�g���b�N�X�擾�֐�

	// [�ړ��֐�]
	VOID MoveByVector(D3DXVECTOR3 move);		// �x�N�g���ňړ�
	VOID MoveByRightVector(float speed);		// �E�����x�N�g�������ɂƂ��Ĉړ�
	VOID MoveByUpVector(float speed);			// ������x�N�g�������ɂƂ��Ĉړ�
	VOID MoveByLookVector(float speed);			// ���_�x�N�g�������ɂƂ��Ĉړ�

	VOID RotationByRightVector(float radian);	// �E�����x�N�g�������ɂƂ��ĉ�]
	VOID RotationByUpVector(float radian);		// ������x�N�g�������ɂƂ��ĉ�]
	VOID RotationByLookVector(float radian);	// ���_�x�N�g�������ɂƂ��ĉ�]

	VOID ResetPosition();						// �J�����Έڍ��W���Z�b�g
	VOID ResetCamera();							// �J���������_���Z�b�g
	VOID Zoom(float n); 						// �J�������_�g��k������

	VOID MoveOnTarget(D3DXVECTOR3 Target);		// �^�[�Q�b�g�ֈړ�����

};
//*****************************************************************************//
// (4) �v���g�^�C�v�錾(�O���ł��g����֐�)
//*****************************************************************************//

#endif