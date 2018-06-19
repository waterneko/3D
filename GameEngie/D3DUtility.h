
//=============================================================================//
// 
// Name : D3DUtility.h
// Describe : �C���N���[�h�t�@�C������
// Author : �e�C�L
// Create Date : 2018/05/01
// Modify Date : 2018/05/17
//=============================================================================//

#ifndef _D3DUTILITY_H_
#define _D3DUTILITY_H_

//*****************************************************************************//
// (1) �C���N���[�h�t�@�C��
//*****************************************************************************//

//---- 1.�K�v�ȃw�b�_�[�t�@�C���̃C���N���[�h ----//

#include <stdio.h>
#include <windows.h>
#include <tchar.h> 
#include <stdlib.h>
#include <time.h>
//====== Directx ======//
#include <d3d9.h>
#include <d3dx9.h>



//====== Directx Input ======//
#define DIRECTINPUT_VERSION (0x0800)	// �x���Ώ��p
#include <dinput.h>

//====== XAudio2 ======//
#include <xaudio2.h>

//====== Directx Sound ======//
//#include <dsound.h>
//#include <mmsystem.h>

//---- 2.�K�v�ȃ��C�u�����t�@�C���̃��[�h ----//

//====== Directx ======//
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment (lib, "dxguid.lib")

//====== Directx Input ======//
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "winmm.lib")

//====== Directx Sound ======//
#pragma comment ( lib, "dsound.lib" )

//---- 3.Directx�ȊO ----//

//====== Imgui ======//
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_dx9.h"

//====== Bullet ======//
//#ifdef _DEBUG
//#pragma comment (lib,"bullet/lib/BulletCollision_vs2010_debug.lib")
//#pragma comment (lib, "bullet/lib/BulletDynamics_vs2010_debug.lib")	
//#pragma comment (lib, "bullet/lib/LinearMath_vs2010_debug.lib")
//#else
//#pragma comment (lib,"bullet/lib/BulletCollision_vs2010.lib")
//#pragma comment (lib, "bullet/lib/BulletDynamics_vs2010.lib")
//#pragma comment (lib, "bullet/lib/LinearMath_vs2010.lib")
//#endif

//*****************************************************************************//
// (2) �}�N����`
//*****************************************************************************//

//====== ������� ======//
#ifndef SAFE_DELETE					
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }      //�|�C���^�[�I������
#endif    

#ifndef SAFE_RELEASE			
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }     //COM�I������
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }     //�|�C���^�[����I������
#endif

//===== �E�C���h�E ======//
#define WINDOW_WIDTH		(1024)						// �E�C���h�E�̕�
#define WINDOW_HEIGHT		(768)						// �E�C���h�E�̍���

//*****************************************************************************//
// (3) ���[�U�[��`�f�[�^�^
//*****************************************************************************//

// { �񋓌^ }

// { �\���� }

// { �N���X }

//*****************************************************************************//
// (4) �v���g�^�C�v�錾(�O���ł��g����֐�)
//*****************************************************************************//

// �֐��R�����g�^�C�v:

// [�R�����g] 
// �⑫����

// < �R�����g >
// �菇

// { �R�����g }
// �񋓌^ �N���X �\���̕�
#endif