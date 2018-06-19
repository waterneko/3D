
//=============================================================================//
// 
// Name : D3DUtility.h
// Describe : インクルードファイル処理
// Author : テイキ
// Create Date : 2018/05/01
// Modify Date : 2018/05/17
//=============================================================================//

#ifndef _D3DUTILITY_H_
#define _D3DUTILITY_H_

//*****************************************************************************//
// (1) インクルードファイル
//*****************************************************************************//

//---- 1.必要なヘッダーファイルのインクルード ----//

#include <stdio.h>
#include <windows.h>
#include <tchar.h> 
#include <stdlib.h>
#include <time.h>
//====== Directx ======//
#include <d3d9.h>
#include <d3dx9.h>



//====== Directx Input ======//
#define DIRECTINPUT_VERSION (0x0800)	// 警告対処用
#include <dinput.h>

//====== XAudio2 ======//
#include <xaudio2.h>

//====== Directx Sound ======//
//#include <dsound.h>
//#include <mmsystem.h>

//---- 2.必要なライブラリファイルのロード ----//

//====== Directx ======//
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment (lib, "dxguid.lib")

//====== Directx Input ======//
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "winmm.lib")

//====== Directx Sound ======//
#pragma comment ( lib, "dsound.lib" )

//---- 3.Directx以外 ----//

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
// (2) マクロ定義
//*****************************************************************************//

//====== 解放処理 ======//
#ifndef SAFE_DELETE					
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }      //ポインター終了処理
#endif    

#ifndef SAFE_RELEASE			
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }     //COM終了処理
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }     //ポインター数列終了処理
#endif

//===== ウインドウ ======//
#define WINDOW_WIDTH		(1024)						// ウインドウの幅
#define WINDOW_HEIGHT		(768)						// ウインドウの高さ

//*****************************************************************************//
// (3) ユーザー定義データ型
//*****************************************************************************//

// { 列挙型 }

// { 構造体 }

// { クラス }

//*****************************************************************************//
// (4) プロトタイプ宣言(外部でも使える関数)
//*****************************************************************************//

// 関数コメントタイプ:

// [コメント] 
// 補足説明

// < コメント >
// 手順

// { コメント }
// 列挙型 クラス 構造体別
#endif