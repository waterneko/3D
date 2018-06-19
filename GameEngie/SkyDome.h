
//=============================================================================//
// 
// Name : SkyDome.h
// Describe : �X�J�C�h�[������
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//
#ifndef _GAMESKY_H_
#define _GAMESKY_H_

//*****************************************************************************//
// (1) �C���N���[�h�t�@�C��
//*****************************************************************************//
#include "D3DUtility.h"				// Directx9�C���N���[�h�t�@�C������

#include "WorldTransform.h"			// ���[���h�g�����X�t�H�[������
//*****************************************************************************//
// (2) �}�N����`
//*****************************************************************************//

#define	FVF_VERTEXSKY	(D3DFVF_XYZ | D3DFVF_TEX1)  // �X�J�C�h�[��FVF

#define	AUTOROTATION_SKYDOME		(0.000005f)				// �X�J�C�{�b�N�X����]���x
#define	INIT_POSY_SKYDOME			(-4000.0f)				// �X�J�C�{�b�N�X������Y���W

#define	TEXTURE_SKYDOMENIGHT		("Data/Texture/Other/SkyDome/night.bmp")	// ��e�N�X�`��
#define	TEXTURE_SKYDOMECLOUD		("Data/Texture/Other/SkyDome/cloud.bmp")	// ���e�N�X�`��

//*****************************************************************************//
// (3) ���[�U�[��`�f�[�^�^
//*****************************************************************************//

// { �񋓌^ }

// { �\���� }
// �X�J�C�h�[��FVF�\����
struct VertexSkyDome
{
	D3DXVECTOR3 vtx;	// ���_���W
	D3DXVECTOR2 tex;	// �e�N�X�`�����W
};

// { �N���X }

// �X�J�C�h�[���N���X
class GSkyDome : public WorldTransform
{
private:
	LPDIRECT3DTEXTURE9      pTexture;				// �e�N�X�`���ւ̃|�C���^�[
	LPDIRECT3DINDEXBUFFER9  pIndexBuffer;			// �C���f�b�N�X�o�b�t�@�[
	LPDIRECT3DVERTEXBUFFER9 pVertexBuffer; 			// ���_�o�b�t�@�[
	LPDIRECT3DDEVICE9       pDevice;				// �f�o�C�X�I�u�W�F�N�g

	int     iNumLatitudes;							// �ܐ��̐�
	int     iNumLongitudes;							// �o���̐�
	int     iVertsPerLati;							// �ܐ��̒��_��
	int     iVertsPerLongi;							// �o���̒��_��
	int     iNumVertices;							// �X�t�B�A�̒��_��
	float   fSkySphereRadius;						// �X�t�B�A�̔��a

													// �X�K�C�X�t�B�AFVF�\����
	VertexSkyDome* 			VtxSkyDome; 			// �X�J�C�h�[��FVF�\����
public:
	bool					bUpdate;				// �X�V�t���O
	bool					bRender;				// �`��t���O
	bool                    bAutoRotation;			// ������]�t���O
public:
	GSkyDome(LPDIRECT3DDEVICE9 D3dDevice);
	~GSkyDome(void);
public:
	HRESULT LoadSkyTexture(char* Path);				// �e�N�X�`����Ǎ���
	HRESULT InitSkyDome(int nAlpha, int nBeta, float nRadius);	// �X�J�C�h�[��������
	VOID UpdateSkyDome();							// �X�J�C�h�[���X�V
	VOID RenderSkyDome(bool bRenderFrame);			// �X�J�C�h�[���`��
};

//*****************************************************************************//
// (4) �v���g�^�C�v�錾(�O���ł��g����֐�)
//*****************************************************************************//

#endif