#include "Drink.h"
#include "myDirect3D.h"
#include "input.h"
#include "texture.h"
#include "debugproc.h"
#include "camera.h"
#include "debugproc.h"
#include "player.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************

#define	VALUE_MOVE		(1.0f)							// �ړ���
#define	VALUE_ROTATE	(D3DX_PI * 0.02f)				// ��]��

#define	Drink_WIDTH		(15.0f)						// �n�ʂ̕�(X����)
#define	Drink_DEPTH		(15.0f)						// �n�ʂ̉��s(Z����)
#define	Drink_HEIGHT	(15.0f)						// �n�ʂ̍���(Y����)

#define Drink_ANGLE		(8)
#define Drink_NUM		(5)

#define SAFE_RELEASE(p) {if(p){(p)->Release();(p)=NULL;}}//���S�ɉ������
//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffDrink = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
struct DRINK
{
	D3DXMATRIX				g_mtxWorldDrink;		// ���[���h�}�g���b�N�X
	D3DXVECTOR3				g_posDrink;				// �n�ʂ̈ʒu
	D3DXVECTOR3				g_rotDrink;				// �n�ʂ̌���(��])
	D3DXVECTOR3				g_sclDrink;				// �n�ʂ̑傫��(�X�P�[��)
	bool					isUse=true;					// �h�����N�̎g�p����
};

DRINK Drink[Drink_NUM];
static bool Hit = false;

static LPDIRECT3DDEVICE9		pDevice;
static DWORD					dwNumMaterials = 0;
static LPD3DXMESH				pMesh = NULL;				//���b�V��
static D3DMATERIAL9*			pMaterials = NULL;			//�}�e���A��
static LPDIRECT3DTEXTURE9*		pTextures = NULL;			//�e�N�X�`��

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void Collision_Drink();

//=============================================================================
// ����������
//=============================================================================
HRESULT Drink_Initialize(void)
{
	pDevice = GetD3DDevice();

	// X�t�@�C�����烁�b�V�������[�h���� 
	LPD3DXBUFFER pD3DXMtrlBuffer = NULL;

	if (FAILED(D3DXLoadMeshFromX("drink.x", D3DXMESH_SYSTEMMEM,
		pDevice, NULL, &pD3DXMtrlBuffer, NULL,
		&dwNumMaterials, &pMesh)))
	{
		MessageBox(NULL, "X�t�@�C���̓ǂݍ��݂Ɏ��s���܂���", NULL, MB_OK);
		return E_FAIL;
	}
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	pMaterials = new D3DMATERIAL9[dwNumMaterials];
	pTextures = new LPDIRECT3DTEXTURE9[dwNumMaterials];

	for (DWORD i = 0; i < dwNumMaterials; i++)
	{
		pMaterials[i] = d3dxMaterials[i].MatD3D;
		pMaterials[i].Ambient = pMaterials[i].Diffuse;
		pTextures[i] = NULL;
		if (d3dxMaterials[i].pTextureFilename != NULL &&
			lstrlen(d3dxMaterials[i].pTextureFilename) > 0)
		{
			if (FAILED(D3DXCreateTextureFromFile(pDevice,
				d3dxMaterials[i].pTextureFilename,
				&pTextures[i])))
			{
				MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���", NULL, MB_OK);
			}
		}
	}
	pD3DXMtrlBuffer->Release();

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	for (int i = 0; i < Drink_NUM; i++)
	{
		Drink[i].g_posDrink = D3DXVECTOR3(100.0f*i, 0.0f, 100.0f);
		Drink[i].g_rotDrink = D3DXVECTOR3(D3DX_PI / 6, 0.0f, 0.0f);
		Drink[i].g_sclDrink = D3DXVECTOR3(5.0f, 5.0f, 5.0f);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void Drink_Finalize(void)
{
	if (g_pVtxBuffDrink != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pVtxBuffDrink->Release();
		g_pVtxBuffDrink = NULL;
	}

	SAFE_RELEASE(pMesh);
	SAFE_RELEASE(pDevice);
}

//=============================================================================
// �X�V����
//=============================================================================
void Drink_Update(void)
{
	Collision_Drink();
	// Y������]
	for (int i = 0; i < Drink_NUM; i++)
	{
		if(Drink[i].isUse) Drink[i].g_rotDrink.y -= VALUE_ROTATE;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void Drink_Draw(void)
{
	//�f�o�C�X�������Ȃ�I��
	if (NULL == pDevice) { return; }

	D3DXMATRIXA16 mtxScl, mtxRot, mtxTranslate;
	for (int i = 0; i < Drink_NUM; i++)
	{
		if (Drink[i].isUse)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&Drink[i].g_mtxWorldDrink);

			// �X�P�[�����]
			D3DXMatrixScaling(&mtxScl, Drink[i].g_sclDrink.x, Drink[i].g_sclDrink.y, Drink[i].g_sclDrink.z);
			D3DXMatrixMultiply(&Drink[i].g_mtxWorldDrink, &Drink[i].g_mtxWorldDrink, &mtxScl);

			// ��]�𔽓]
			D3DXMatrixRotationYawPitchRoll(&mtxRot, Drink[i].g_rotDrink.y, Drink[i].g_rotDrink.x, Drink[i].g_rotDrink.z);
			D3DXMatrixMultiply(&Drink[i].g_mtxWorldDrink, &Drink[i].g_mtxWorldDrink, &mtxRot);

			// �ړ��𔽓]
			D3DXMatrixTranslation(&mtxTranslate, Drink[i].g_posDrink.x, Drink[i].g_posDrink.y, Drink[i].g_posDrink.z);
			D3DXMatrixMultiply(&Drink[i].g_mtxWorldDrink, &Drink[i].g_mtxWorldDrink, &mtxTranslate);

			pDevice->SetTransform(D3DTS_WORLD, &Drink[i].g_mtxWorldDrink);

			//���_�V�F�[�_���Z�b�g(�g��Ȃ��ꍇ��NULL)
			GetD3DDevice()->SetVertexShader(NULL);

			//���_�t�H�[�}�b�g���Z�b�g
			GetD3DDevice()->SetFVF(pMesh->GetFVF());

			//�����ɏ������e������
			for (DWORD i = 0; i < dwNumMaterials; i++)
			{
				pDevice->SetMaterial(&pMaterials[i]);
				pDevice->SetTexture(0, pTextures[i]);
				pMesh->DrawSubset(i);
			}
		}
	}
}

// �h�����N�̓����蔻��
void Collision_Drink()
{
	D3DXVECTOR3 Player = GetPlayer_Pos();
	D3DXVECTOR3 dst1(Player.x, Player.y, Player.z);

	Hit = false;
	for (int i = 0; i < Drink_NUM; i++)
	{
		if (Drink[i].isUse)
		{
			D3DXVECTOR3 dst2(Drink[i].g_posDrink.x, Drink[i].g_posDrink.y, Drink[i].g_posDrink.z);
			D3DXVECTOR3 distance;
			distance = dst2 - dst1;
			float length = D3DXVec3Length(&distance);
			float size = 15;
			if (length < size)
			{
				Hit = true;
				Drink[i].isUse = false;
				return;
			}
		}
	}
	return;
}

bool Get_DrinkHit()
{
	return Hit;
}