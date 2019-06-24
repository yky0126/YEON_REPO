#include <Windows.h>
#include <d3dx9.h>
#include <tchar.h>
#include <dinput.h>

//DirectXの本体
LPDIRECT3D9 pDirect3d;
//描画のためのデバイス情報
LPDIRECT3DDEVICE9 pDevice;
LPDIRECTINPUT8 pDinput = NULL;
LPDIRECTINPUTDEVICE8 pkey = NULL;
HRESULT InitDinput(HWND hWnd);
//入力キーの最大数

void UpdateKeyStatus();
//キー入力関数

bool GetKeyStatus(int KeyNumber);

HRESULT InitD3d(HWND hInst, const TCHAR* filePath);
const int D3DFVF_CUSTOMVERTEX(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
LPDIRECT3DTEXTURE9 pTexture;
void InitPresentParameters(HWND);
HRESULT BuildDxDevice(HWND hInst, const TCHAR* filePath);
//const int D3DFVF_CUSTOMVERTEX = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
static const int MAX_KEY_NUMBER = 256; //入力キー判定のマスク値
const int MASK_NUM = 0x80;
BYTE KeyState[MAX_KEY_NUMBER];
//キーステータス更新関数
LRESULT CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);

struct CUSTOMVERTEX {
	float x, y, z; //頂点座標
	float rhw; // 除算数
	DWORD dwColor; //頂点の色
	float tu, tv; // テクスチャの座標
};


HRESULT hr = DirectInput8Create(instance_handle,
	DIRECTINPUT_VERSION,
	IID_IDirectInput8,
	(void**)&g_pInputInterface,
	NULL); 
/* 
HINSTANCE アプリハンドル, DWORD Direct Input バージョン, REFIID インターフェースの識別子,
LPVOID* IDirectInput8 インターフェースポインタの受け取り用, LPUNKNOWN 基本的にNULL
*/

if (FAILED(hr))
{
	//　失敗
}

LPDIRECTINPUTDEVICE device;

hr = g_pInputInterface->CreateDevice(
	GUID_SysKeyboard,
	&device,
	NULL);
/*
REFGUID 使用するデバイスの指定　EX) Keyboard の場合 GUID_SysKeyBoard,
LPDIRECTINPUTDEVICE* IDirectInputDevice8インターフェース受け取り用,
LPUNKNOWN 基本的にNULL
*/

if (FAILED(hr))
{
	//失敗
}

//Device Format の設定
HRESULT hr = g_pKeyDevice->SetDataFormat(&c_dfDIKeyboard);
if (FAILED(hr))
/*
LPCDIDATAFORMAT 設定する入力デバイスのフォーマット（デバイスはDirectInput側で用意されてる)
Keyboard => c_dfDIKeyboard
Mouse => c_dfDIMouse
Joystick => c_dfDIJoystick
*/

{
	//失敗
}

//協調モードの設定
HRESULT hr = g_pKeyDevice->SetCooperativeLevel(window_handle,
	DISCL_BACKGROUND | DISCL_NONEXCLUSIVE); //　バックグラウンド　、　非排他的
/*
HWND 入力デバイスが関連付けられてるWindowsHandle(入力を受けるWindow),
DWORD 協調レベルの設定フラグ　（FOREGROUND , BACKGROUND と　EXCLUSIVE , NONEXCLUSIVE)
*/

if (FAILED(hr))
{
	//失敗
}

HRESULT hr = g_pKeyDevice->Acquire();
/*
Acquire 制御開始の成否(HRESULT)が返る
*/

if (FAILED(hr))
{
	//　制御開始失敗
}
