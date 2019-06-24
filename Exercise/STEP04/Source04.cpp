#include <Windows.h>
#include <d3dx9.h>
#include <tchar.h>
#include <dinput.h>

//DirectX�̖{��
LPDIRECT3D9 pDirect3d;
//�`��̂��߂̃f�o�C�X���
LPDIRECT3DDEVICE9 pDevice;
LPDIRECTINPUT8 pDinput = NULL;
LPDIRECTINPUTDEVICE8 pkey = NULL;
HRESULT InitDinput(HWND hWnd);
//���̓L�[�̍ő吔

void UpdateKeyStatus();
//�L�[���͊֐�

bool GetKeyStatus(int KeyNumber);

HRESULT InitD3d(HWND hInst, const TCHAR* filePath);
const int D3DFVF_CUSTOMVERTEX(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
LPDIRECT3DTEXTURE9 pTexture;
void InitPresentParameters(HWND);
HRESULT BuildDxDevice(HWND hInst, const TCHAR* filePath);
//const int D3DFVF_CUSTOMVERTEX = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
static const int MAX_KEY_NUMBER = 256; //���̓L�[����̃}�X�N�l
const int MASK_NUM = 0x80;
BYTE KeyState[MAX_KEY_NUMBER];
//�L�[�X�e�[�^�X�X�V�֐�
LRESULT CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);

struct CUSTOMVERTEX {
	float x, y, z; //���_���W
	float rhw; // ���Z��
	DWORD dwColor; //���_�̐F
	float tu, tv; // �e�N�X�`���̍��W
};


HRESULT hr = DirectInput8Create(instance_handle,
	DIRECTINPUT_VERSION,
	IID_IDirectInput8,
	(void**)&g_pInputInterface,
	NULL); 
/* 
HINSTANCE �A�v���n���h��, DWORD Direct Input �o�[�W����, REFIID �C���^�[�t�F�[�X�̎��ʎq,
LPVOID* IDirectInput8 �C���^�[�t�F�[�X�|�C���^�̎󂯎��p, LPUNKNOWN ��{�I��NULL
*/

if (FAILED(hr))
{
	//�@���s
}

LPDIRECTINPUTDEVICE device;

hr = g_pInputInterface->CreateDevice(
	GUID_SysKeyboard,
	&device,
	NULL);
/*
REFGUID �g�p����f�o�C�X�̎w��@EX) Keyboard �̏ꍇ GUID_SysKeyBoard,
LPDIRECTINPUTDEVICE* IDirectInputDevice8�C���^�[�t�F�[�X�󂯎��p,
LPUNKNOWN ��{�I��NULL
*/

if (FAILED(hr))
{
	//���s
}

//Device Format �̐ݒ�
HRESULT hr = g_pKeyDevice->SetDataFormat(&c_dfDIKeyboard);
if (FAILED(hr))
/*
LPCDIDATAFORMAT �ݒ肷����̓f�o�C�X�̃t�H�[�}�b�g�i�f�o�C�X��DirectInput���ŗp�ӂ���Ă�)
Keyboard => c_dfDIKeyboard
Mouse => c_dfDIMouse
Joystick => c_dfDIJoystick
*/

{
	//���s
}

//�������[�h�̐ݒ�
HRESULT hr = g_pKeyDevice->SetCooperativeLevel(window_handle,
	DISCL_BACKGROUND | DISCL_NONEXCLUSIVE); //�@�o�b�N�O���E���h�@�A�@��r���I
/*
HWND ���̓f�o�C�X���֘A�t�����Ă�WindowsHandle(���͂��󂯂�Window),
DWORD �������x���̐ݒ�t���O�@�iFOREGROUND , BACKGROUND �Ɓ@EXCLUSIVE , NONEXCLUSIVE)
*/

if (FAILED(hr))
{
	//���s
}

HRESULT hr = g_pKeyDevice->Acquire();
/*
Acquire ����J�n�̐���(HRESULT)���Ԃ�
*/

if (FAILED(hr))
{
	//�@����J�n���s
}
