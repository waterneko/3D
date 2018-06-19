
//=============================================================================//
// 
// Name : SkinMesh.h
// Describe : �X�L�����b�V������
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//
#ifndef _SKINMESH_H_
#define _SKINMESH_H_

//*****************************************************************************//
// (1) �C���N���[�h�t�@�C��
//*****************************************************************************//
#include "D3DUtility.h"				// Directx9�C���N���[�h�t�@�C������

#include "AnimXfile.h"				// �A�j���[�V����x�t�@�C������
#include "AnimationController.h"	// �A�j���[�V�����R���g���[���[����
#include "WorldTransform.h"			// ���[���h�g�����X�t�H�[������
//*****************************************************************************//
// (2) �}�N����`
//*****************************************************************************//
#define	MAXNUM_SKINMESH			(30)			// �ő�X�L�����b�V����
//*****************************************************************************//
// (3) ���[�U�[��`�f�[�^�^
//*****************************************************************************//

// { �񋓌^ }

// { �\���� }

// { �N���X }
// �X�L�����b�V���N���X
class BSkinMesh : public WorldTransform
{
private:
	LPDIRECT3DDEVICE9			pDevice;					// �f�o�C�X�I�u�W�F�N�g

	SAnimXfile*					pAnimXfile;					// �A�j���[�V����x�t�@�C���ւ̃|�C���^�[
public:
	bool		bUpdate;									// �X�V�t���O
	bool		bRender;									// �`��t���O

	BAnimController*			pAnimController;			//  �A�j���[�V�����R���g���[���[�N���X�ւ̃|�C���^�[
public:
	BSkinMesh(LPDIRECT3DDEVICE9 D3dDevice);					// �R���X�g���N�^
	~BSkinMesh();											// �f�X�g���N�^
public:
	HRESULT InitSkinMesh(SAnimXfile* pAnimxfile);			// �X�L�����b�V��������
	VOID UpdateAnimation(FLOAT fDeltaTime);					// �X�L�����b�V���A�j���[�V�����X�V
	VOID UpdateSkinMesh();									// �X�L�����b�V���X�V
	VOID RenderSkinMesh();									// �X�L�����b�V���`��
private:
	// [�{�[���}�g���N�X]�ݒu
	HRESULT SetupBoneMatrixPointers(LPD3DXFRAME pFrame);							// �{�[���}�g���N�X�̌v�Z(�K�w)
	HRESULT SetupBoneMatrixPointersOnMesh(LPD3DXMESHCONTAINER pMeshContainerBase);	// �{�[���}�g���N�X�̌v�Z(�P��)
	VOID UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix);	// �{�[���}�g���N�X�̍X�V
	VOID DrawFrame(LPD3DXFRAME pFrame);												// �t���[���̕`��(�K�w)
	VOID DrawMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase);	// �t���[���̕`��(�P��)
};

//*****************************************************************************//
// (4) �v���g�^�C�v�錾(�O���ł��g����֐�)
//*****************************************************************************//

#endif