
#pragma once

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

// �Q�[���̏������֐�
// �߂�l:�������Ɏ��s�����Ƃ�false
bool D3D_Initialize(HWND hWnd);

// �Q�[���̏I������
void D3D_Finalize(void);

//Direct3DDevice�̃Q�b�^�[
LPDIRECT3DDEVICE9 GetD3DDevice();
