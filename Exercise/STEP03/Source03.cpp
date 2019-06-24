#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <mmsystem.h>
#include <tchar.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "winmm.lib")

LPDIRECT3D9 pDirect3d;
LPDIRECT3DDEVICE9 pDevice;

LRESULT CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);

#define D3DXCreateTextureFromFile()
#define WINAPI_stdcall

HRESULT InitD3d(HWND hInst, const TCHAR* filePath);
const int D3DFVF_CUSTOMVERTEX(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
LPDIRECT3DTEXTURE9 pTexture;
void InitPresentParameters(HWND);
HRESULT BuildDxDevice(HWND hInst, const TCHAR* filePath);

LRESULT CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);
struct CUSTOMVERTEX  //�g�p�҂̒��_���`����\����
{
	FLOAT x, y, z;
	FLOAT rhw;
	DWORD dwColor;
	FLOAT tu, tv;
};

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR szstr, INT iCmdshow)
{
	MSG msg;
	static char szAppName[] = "Exercise";
	WNDCLASSEX wndclass;

	CUSTOMVERTEX v[4] =			//���W
	{
	{170.0f, 110.0f, 0.0f, 1.0f, 0xffffffff,0.0f,0.0f},
	{470.0f, 110.0f, 0.0f, 1.0f, 0xffffffff,1.0f,0.0f},
	{470.0f, 410.0f, 0.0f, 1.0f, 0xffffffff,1.0f,1.0f},
	{170.0f, 410.0f, 0.0f, 1.0f, 0xffffffff,0.0f,1.0f}
	};

	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = wndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInst;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&wndclass);
	//    �E�B���h�E����
	HWND hInsT = CreateWindow(szAppName, szAppName, WS_OVERLAPPEDWINDOW, 0, 0, 640, 480, NULL, NULL, hInst, NULL);

	ShowWindow(hInsT, SW_SHOW);
	UpdateWindow(hInsT);

	BuildDxDevice(hInsT, _T("pinos.jpg"));

	timeBeginPeriod(1);
	//���̎��Ԃ�time�ɕۑ��B
	DWORD time = timeGetTime();
	DWORD prevtime = 0;

	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			time = timeGetTime();
			if (time - prevtime < 1000 / 60) {

				pDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0x00, 0x00, 0x00), 1.0, 0);
				pDevice->BeginScene();


			    }
				pDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
				pDevice->SetTexture(0, pTexture);
				pDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, v, sizeof(CUSTOMVERTEX));
				pDevice->EndScene();
				pDevice->Present(0, 0, 0, 0);
			}


			prevtime = time;

		}
		timeEndPeriod(1);
	}

HRESULT BuildDxDevice(HWND hInst, const TCHAR* filePath) {
	if (FAILED(InitD3d(hInst, filePath))) {
		return E_FAIL;
	}

	//D3D�̃|�C���^�ϐ���Direct3DCreate9�֐�(Direct3D�����֐�)�ō쐬�������̂���
	pDirect3d = Direct3DCreate9(D3D_SDK_VERSION);

	//D3D�̃|�C���^�ϐ���NULL��������
	if (pDirect3d == NULL) {
		//D3D�̍쐬�Ɏ��s�������̏���
		MessageBox(0,_T("Direct3D�̍쐬�Ɏ��s���܂���"), NULL, MB_OK);
		return E_FAIL;
	}
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	//���_�����Z�b�g
	//pDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	return S_OK;
}

LRESULT CALLBACK wndProc(HWND hInst, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		switch ((CHAR)wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		break;
	}

	return DefWindowProc(hInst, iMsg, wParam, lParam);
}

HRESULT InitD3d(HWND hInst, const TCHAR* filePath)
{
	if (NULL == (pDirect3d = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		MessageBox(0, "Direct3D���s", "", MB_OK);
		return E_FAIL;
	}

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed = true;
	if (FAILED(pDirect3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hInst, D3DCREATE_MIXED_VERTEXPROCESSING, &d3dpp, &pDevice))) {
		MessageBox(0, "HAL���[�h��DIRECT�RD", NULL, MB_OK);
		return E_FAIL;
	}
	if (FAILED(D3DXCreateTextureFromFileEx(pDevice, "pinos.jpg", 450, 351, 0, 0, D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT,
		0xff000000, NULL, NULL, &pTexture)))
	{
		MessageBox(0, _T("�e�N�X�`���I�u�W�F�N�g�̍쐬�Ɏ��s���܂���"), NULL, MB_OK);
		return E_FAIL;
	}
	return S_OK;
}

void initrender()
{
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
}
