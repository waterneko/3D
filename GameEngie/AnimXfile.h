
//=============================================================================//
// 
// Name : AnimXfile.h
// Describe : �A�j���[�V����x�t�@�C������
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//
#ifndef _ANIMXFILE_H_
#define _ANIMXFILE_H_

//*****************************************************************************//
// (1) �C���N���[�h�t�@�C��
//*****************************************************************************//
#include "D3DUtility.h"				// Directx9�C���N���[�h�t�@�C������

//*****************************************************************************//
// (2) �}�N����`
//*****************************************************************************//
#define	MAXNUM_ANIMXFILE		(50)							// �ő�A�j���[�V����x�t�@�C����

#define	PATH_ANIM_YUKARI		("Data/Animxfile/Yukari/")		// �A�j���[�V����x�t�@�C���p�XYukari
//*****************************************************************************//
// (3) ���[�U�[��`�f�[�^�^
//*****************************************************************************//

// { �񋓌^ }
// �X�L�����b�V���^�C�v
enum SKINMESH_TYPE
{
	D3DNONINDEXED,		// �C���f�b�N�X��񖳂�
	D3DINDEXED,			// �C���f�b�N�X����
	D3DINDEXEDHLSLVS,	// �C���f�b�N�X���� + �V�F�[�_�[�p
	NONE
};

// { �\���� }
// �h���t���[���\����
struct SkinMesh_XFRAME : public D3DXFRAME
{
	D3DXMATRIX CombinedTransformationMatrix; 	//�{�[���̃}�g���N�X
};

// �h�����b�V���R���e�i�[�\����
struct SkinMesh_MESHCONTAINER : public D3DXMESHCONTAINER
{
	LPDIRECT3DTEXTURE9*  ppTextures;            // �e�N�X�`��
	LPD3DXMESH           pOrigMesh;             // ���n���b�V��
	DWORD                NumAttributeGroups;    // �����O���[�v
	DWORD                NumInfl;               // �����_�̏d�݂̌��i�X�L���E�F�C�g�Ƃ͒��_�ւ̉e���B�j
	LPD3DXBUFFER         pBoneCombinationBuf;   // �{�[���E�e�[�u��
	D3DXMATRIX**         ppBoneMatrixPtrs;      // �{�[���̕ϊ��}�g���N�X�̐擪�|�C���^�[
	D3DXMATRIX*          pBoneOffsetMatrices;   // �{�[���̏����}�g���N�X�̃|�C���^�[
	DWORD                NumPaletteEntries;     // �{�[���ő吔
	DWORD                iAttributeSW;          // �C���f�b�N�X��񖳂��X�L�����b�V���� SW��HW�����g���ݒ�
	bool                 UseSoftwareVP;         // �\�t�g�E�F�A�Œ��_�������邩�ǂ���
};

// �V�F�[�f�B���O���f���X�g// �V�F�[�f�B���O���f���X�g
struct AnimShadingList
{
	BOOL* OutLineList;			// �`�Ӕ��f���X�g
	BOOL* ToonRenderList;		// Toon�����_�����O���f���X�g
};

// { �N���X }
// �h���{�[�����b�V���N���X
class AllocateHierarchy : public ID3DXAllocateHierarchy
{
public:
	char*					strPath;
	int						iSkinMeshType;
	D3DXMATRIX*				pBoneMatrices;
	UINT					iNumBoneMatricesMax;
public:
	AllocateHierarchy(char* path, int type);	// �R���X�g���N�^
	~AllocateHierarchy();						// �f�X�g���N�^
private:
	HRESULT AllocateName(LPCSTR Name, LPSTR *pNewName);														// �{�[�����b�V���̖��Ɠ������z
	HRESULT GenerateSkinnedMesh(IDirect3DDevice9* pd3dDevice, SkinMesh_MESHCONTAINER *pMeshContainer);		// �X�L�����b�V������

public:
	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame); 				// �t���[�����쐬����
	STDMETHOD(CreateMeshContainer)(THIS_ LPCSTR Name,      								// ���b�V���R���e�i�[���쐬����
		CONST D3DXMESHDATA*       pMeshData,
		CONST D3DXMATERIAL*       pMaterials,
		CONST D3DXEFFECTINSTANCE* pEffectInstances,
		DWORD                     NumMaterials,
		CONST DWORD *             pAdjacency,
		LPD3DXSKININFO pSkinInfo,
		LPD3DXMESHCONTAINER *ppNewMeshContainer);
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree); 							//�t���[����j������
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerBase); 		//���b�V���R���e�i�[��j������
};

// �A�j���[�V����x�t�@�C���N���X
class SAnimXfile
{
private:
	LPDIRECT3DDEVICE9		pDevice;						// �f�o�C�X�I�u�W�F�N�g
public:
	AllocateHierarchy*          pAllocateHier;				// �h���{�[�����b�V���N���X
	LPD3DXFRAME	                pFrameRoot;					// ���t���[��
	LPD3DXANIMATIONCONTROLLER	pOrigAnimController;		// ���n�A�j���[�V�����R���g���[���[
public:
	SAnimXfile(LPDIRECT3DDEVICE9 D3dDevice);					// �R���X�g���N�^
	~SAnimXfile();												// �f�X�g���N�^
public:
	HRESULT LoadAnimXfile(char* path, char* name, int type);	// �A�j���[�V����x�t�@�C��x�t�@�C���ǂݍ���
};
//*****************************************************************************//
// (4) �v���g�^�C�v�錾(�O���ł��g����֐�)
//*****************************************************************************//

#endif