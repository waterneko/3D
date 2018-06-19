
//=============================================================================//
// 
// Name : Scene2A.h
// Describe : �V�[��2A����
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//
#ifndef _SCENE2A_H_
#define _SCENE2A_H_

//*****************************************************************************//
// (1) �C���N���[�h�t�@�C��
//*****************************************************************************//
#include "D3DUtility.h"				// Directx9�C���N���[�h�t�@�C������
#include "Input.h"					// ���͏���
#include "Math.h"					// ���w�p�֐�

#include "Camera.h"					// �J��������
#include "Light.h"					// ���C�g����

#include "Mesh.h"					// ���b�V������
#include "SkinMesh.h"				// �X�L�����b�V������
#include "GameTexture.h"			// �Q�[���e�N�X�`������
#include "GameController.h"			// �Q�[���R���g���[���[����

#include "RayIntersect.h"			// ���C��������

#include "StageScene.h"				// �X�e�[�W�V�[������
//*****************************************************************************//
// (2) �}�N����`
//*****************************************************************************//

//*****************************************************************************//
// (3) ���[�U�[��`�f�[�^�^
//*****************************************************************************//

// { �񋓌^ }

// { �\���� }

// { �N���X }

// �V�[��2A�����N���X
class Scene2A : public SceneInstance
{
private:
	LPDIRECT3DDEVICE9		pDevice; 						// �f�o�C�X�I�u�W�F�N�g

	BThirdPersonCamera*		pTPCamera;						// �J�����N���X�ւ̃|�C���^�[
	BLight*					pLight;							// ���C�g�N���X�ւ̃|�C���^�[
	BMesh*					pMesh[MAXNUM_MESH];				// ���b�V���N���X�ւ̃|�C���^�[
	BSkinMesh*				pSkinMesh[MAXNUM_SKINMESH];		// �X�L�����b�V���N���X�ւ̃|�C���^�[
	GTexture*				pGTexture[MAXNUM_GAMETEXTURE];	// �Q�[���e�N�X�`���N���X�ւ̃|�C���^�[
	GController*			pGController;					// �Q�[���R���g���[���[�N���X�ւ̃|�C���^�[
	CRay*					pCameraRay;						// �J�����p���C�����N���X�ւ̃|�C���^�[
	CRay*					pCharacterRayZ;					// �L�����p���C�����N���X�ւ̃|�C���^�[
	CRay*					pCharacterRayY;					// �L�����p���C�����N���X�ւ̃|�C���^�[
	CRay*					pCharacterRayX;					// �L�����p���C�����N���X�ւ̃|�C���^�[
public:
	Scene2A(LPDIRECT3DDEVICE9 D3dDevice);				// �R���X�g���N�^
	~Scene2A();											// �f�X�g���N�^
public:
	HRESULT	InitScene();								// �V�[���̏�����
	VOID UpdateController();							// �V�[���R���g���[���[�̑��X�V
	VOID UpdateScene();									// �V�[���̑��X�V
	VOID RenderScene();									// �V�[���̑��`��

	VOID DebugImgui();									// �V�[���f�o�b�O�pIMGUI
};

//*****************************************************************************//
// (4) �v���g�^�C�v�錾(�O���ł��g����֐�)
//*****************************************************************************//

#endif