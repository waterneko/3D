
//=============================================================================//
// 
// Name : SourceManager.h
// Describe : �\�[�X�Ǘ�
// Author : 
// Create Date : 2018/05/01
// Modify Date : 2018/06/05
//=============================================================================//
#ifndef _SOURCEMANAGER_H_
#define _SOURCEMANAGER_H_

//*****************************************************************************//
// (1) �C���N���[�h�t�@�C��
//*****************************************************************************//
#include "D3DUtility.h"				// Directx9�C���N���[�h�t�@�C������

#include "Texturefile.h"			// �e�N�X�`���t�@�C������
#include "Xfile.h"					// x�t�@�C������
#include "AnimXfile.h"				// �A�j���[�V����x�t�@�C������
#include "Wavfile.h"				// Wav�t�@�C������
//*****************************************************************************//
// (2) �}�N����`
//*****************************************************************************//

//*****************************************************************************//
// (3) ���[�U�[��`�f�[�^�^
//*****************************************************************************//

// { �񋓌^ }
// �Ǎ��ݏ��
enum LoadingState
{
	LS_Ready = 0,		// �\�[�X�ҋ@���
	LS_BeginLoad,		// �\�[�X���[�h�J�n(�Ó]����)
	LS_Loading,			// �\�[�X���[�h��
	LS_EndLoad,			// �\�[�X���[�h�I��(�Ó]��)
};
// { �\���� }

// { �N���X }

// �\�[�X�Ǘ��N���X(Singleton)
class SourceManager
{
private:
	static SourceManager*	s_pInstance;						// (Singleton)�C���X�^���X

	LPDIRECT3DDEVICE9		pDevice;							// �f�o�C�X�I�u�W�F�N�g
	
	UINT					iLoadingState;						// �Ǎ��ݏ��
	UINT					iLoadingProcess;					// �Ǎ��݃v���Z�X

	UINT					iTextureNum;						// �e�N�X�`���t�@�C����
	UINT					iXfileNum;							// x�t�@�C���t�@�C����
	UINT					iAnimXfileNum;						// �A�j���[�V����x�t�@�C����
	UINT					iWavfileNum;						// Wav�t�@�C����

	STexturefile*			pTexturefile[MAXNUM_TEXTUREFILE];	// �e�N�X�`���N���X�ւ̃|�C���^�[
	SXfile*					pXfile[MAXNUM_XFILE];				// x�t�@�C���N���X�ւ̃|�C���^�[
	SAnimXfile*				pAnimXfile[MAXNUM_ANIMXFILE];		// �A�j���[�V����x�t�@�C���N���X�ւ̃|�C���^�[
	SWavfile*				pWavfile[MAXNUM_WAVFILE];			// Wav�t�@�C���ւ̃|�C���^�[

	SourceManager();											// �R���X�g���N�^
	~SourceManager();											// �f�X�g���N�^
public:
	static SourceManager* GetInstance();						// �C���X�^���X���擾����

	HRESULT InitSourceManager(LPDIRECT3DDEVICE9 D3dDevice);		// �\�[�X�Ǘ�������
	VOID	UninitSourceManager();								// �\�[�X�Ǘ����

	VOID	AddLoadingProcess(int n);							// �Ǎ��݃v���Z�X�J�n
	VOID	ResetLoadingProcess();								// �Ǎ��݃v���Z�X���[�b�g

	// [�ǂݍ���] 
	HRESULT LoadTexturefile(int num, char* path, char* name);			// �e�N�X�`����ǂݍ���
	HRESULT LoadXfile(int num, char* path, char* name);					// x�t�@�C����ǂݍ���
	HRESULT LoadAnimXfile(int num, char* path, char* name, int type);	// �A�j���[�V����x�t�@�C����ǂݍ���
	HRESULT LoadWavfile(int num, char* path, char* name);				// Wav�t�@�C����ǂݍ���

	// [�ݒu�֐�]
	VOID SetLoadingState(UINT state);							// �Ǎ��ݏ�Ԑݒu

	// [�擾�֐�]
	UINT	GetTextureNum() { return iTextureNum; };			// �e�N�X�`���t�@�C�������擾����
	UINT	GetXfileNum() { return iXfileNum; };				// x�t�@�C�����擾����
	UINT	GetAnimXfileNum() { return iAnimXfileNum; };		// �A�j���[�V����x�t�@�C�������擾����
	UINT	GetWavfileNum() { return iWavfileNum; };			// Wav�t�@�C�������擾����

	STexturefile* GetTextureFile(UINT n);						// �e�N�X�`�����擾����
	SXfile* GetXfile(UINT n);									// x�t�@�C�����擾����
	SAnimXfile* GetAnimXfile(UINT n);							// �A�j���[�V�������擾����
	SWavfile* GetWavfile(UINT n);								// Wav�t�@�C�����擾����

	VOID FreeTextureFile(UINT n);						// �e�N�X�`�����������
	VOID FreeXfile(UINT n);								// x�t�@�C�����������
	VOID FreeAnimXfile(UINT n);							// �A�j���[�V����x�t�@�C�����������
	VOID FreeWavfile(UINT n);							// Wav�t�@�C�����������

	UINT GetLoadingState() { return iLoadingState; };			// �Ǎ��ݏ�Ԃ��擾����
	UINT GetLoadingProcess() { return iLoadingProcess; };		// �Ǎ��݃v���Z�X���擾����
};
//*****************************************************************************//
// (4) �v���g�^�C�v�錾(�O���ł��g����֐�)
//*****************************************************************************//

#endif