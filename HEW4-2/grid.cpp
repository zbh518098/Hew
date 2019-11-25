
#include "grid.h"
#include "myDirect3D.h"
#include "input.h"
#include "texture.h"
#include "debugproc.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

#define	VALUE_MOVE		(5.0f)							// �ړ���
#define	VALUE_ROTATE	(D3DX_PI * 0.02f)				// ��]��

#define	Grid_WIDTH		(50.0f)						// �n�ʂ̕�(X����)
#define	Grid_DEPTH		(50.0f)						// �n�ʂ̉��s(Z����)
#define	Grid_HEIGHT		(50.0f)						// �n�ʂ̍���(Y����)

#define Grid_AREA_X		100
#define Grid_AREA_Y		100
#define Grid_LINE		1

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexGrid(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGrid = NULL;	// ���_�o�b�t�@�ւ̃|�C���^

D3DXMATRIX				g_mtxWorldGrid;		// ���[���h�}�g���b�N�X
D3DXVECTOR3				g_posGrid;				// �n�ʂ̈ʒu

//=============================================================================
// ����������
//=============================================================================
HRESULT Grid_Initialize(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	// ���_���̍쐬
	MakeVertexGrid(pDevice);

	// �ʒu�̏����ݒ�
	g_posGrid = D3DXVECTOR3(-50.0f*(Grid_AREA_X-1), -90.0f, 50.0f*(Grid_AREA_Y-1));

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void Grid_Finalize(void)
{
	if (g_pVtxBuffGrid != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pVtxBuffGrid->Release();
		g_pVtxBuffGrid = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void Grid_Update(void)
{
	
}

//=============================================================================
// �`�揈��
//=============================================================================
void Grid_Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldGrid);

	// �ړ��𔽓]
	D3DXMatrixTranslation(&mtxTranslate, g_posGrid.x, g_posGrid.y, g_posGrid.z);
	D3DXMatrixMultiply(&g_mtxWorldGrid, &g_mtxWorldGrid, &mtxTranslate);

	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldGrid);

	pDevice->SetStreamSource(0, g_pVtxBuffGrid, 0, sizeof(VERTEX_3D));

	pDevice->SetFVF(FVF_VERTEX_3D);

	pDevice->SetTexture(0, NULL);

	pDevice->DrawPrimitive(D3DPT_LINELIST, 0, ((Grid_LINE + 1) * 2)*(Grid_AREA_X*Grid_AREA_Y));
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexGrid(LPDIRECT3DDEVICE9 pDevice)
{
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D)* ((Grid_LINE + 1) * 4) * (Grid_AREA_X*Grid_AREA_Y), //VRAM�Ƀ����������
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGrid,
		NULL)))
	{
		return E_FAIL;
	}

	{
		VERTEX_3D *pVtx;

		//Lock,unLock�����܂肵�Ȃ��̂��p�t�H�[�}���X���グ����@
		g_pVtxBuffGrid->Lock(0, 0, (void**)&pVtx, 0); //VRAM�ɂ��������������V�X�e����Ɏ����Ă��Ă����

		//���̑傫��
		int cou = 0;
		for (int y=0; y < Grid_AREA_Y; y++)
		{
			//�c�̑傫��
			for (int x=0; x < Grid_AREA_X; x++)
			{
				//�O���b�h�̐��̐�
				for (int i = 0; i <= Grid_LINE; i++)
				{
					float l_sp = 100.0f / Grid_LINE;

					// �|�W�V�����E�傫��
					pVtx[0 + cou * 4].pos = D3DXVECTOR3(-Grid_WIDTH + 100.0f*x, Grid_HEIGHT, Grid_DEPTH - l_sp*i - 100.0f*y);
					pVtx[1 + cou * 4].pos = D3DXVECTOR3(Grid_WIDTH + 100.0f*x, Grid_HEIGHT, Grid_DEPTH - l_sp*i - 100.0f*y);
					pVtx[2 + cou * 4].pos = D3DXVECTOR3(-Grid_WIDTH + l_sp*i + 100.0f*x, Grid_HEIGHT, Grid_DEPTH - 100.0f*y);
					pVtx[3 + cou * 4].pos = D3DXVECTOR3(-Grid_WIDTH + l_sp*i + 100.0f*x, Grid_HEIGHT, -Grid_DEPTH - 100.0f*y);
							 
					// �@���x�N�g��
					pVtx[0 + cou * 4].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
					pVtx[1 + cou * 4].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
					pVtx[2 + cou * 4].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
					pVtx[3 + cou * 4].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
							 
					// �F
					pVtx[0 + cou * 4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[1 + cou * 4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[2 + cou * 4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[3 + cou * 4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					// ����Ă����
					cou++;
				}
			}
		}
		g_pVtxBuffGrid->Unlock(); //VRAM�Ƀ�������Ԃ�(�d������)
	}

	return S_OK;
}

