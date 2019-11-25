
#include "player.h"
#include "myDirect3D.h"
#include "input.h"
#include "texture.h"
#include "debugproc.h"
#include "camera.h"
#include "drink.h"
#include "ground.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

#define	VALUE_ROTATE		(D3DX_PI * 0.01f)			// ��]��

#define	Player_WIDTH		(15.0f)						// �n�ʂ̕�(X����)
#define	Player_DEPTH		(15.0f)						// �n�ʂ̉��s(Z����)
#define	Player_HEIGHT		(15.0f)						// �n�ʂ̍���(Y����)

#define Player_ANGLE		(8)
#define MAXSPEED			(200.0f)						// �ړ���

#define SAFE_RELEASE(p) {if(p){(p)->Release();(p)=NULL;}}//���S�ɉ������

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void SetPlayer_Pos(float set);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9			g_pVtxBuffPlayer = NULL;	// ���_�o�b�t�@�ւ̃|�C���^

D3DXMATRIX						g_mtxWorldPlayer;			// ���[���h�}�g���b�N�X
D3DXVECTOR3						g_posPlayer;				// �v���C���[�̈ʒu
D3DXVECTOR3						g_rotPlayer;				// �v���C���[�̌���(��])
D3DXVECTOR3						g_sclPlayer;				// �v���C���[�̑傫��(�X�P�[��)
D3DXVECTOR3						g_normalPlayer;				// �v���C���[�̖@��

float							accel = 0.0f;				// �����̐��l
float							item = 0.0f;				// �A�C�e���ɂ���Ẳ����̐��l
float							drift = 0.0f;				// �h���t�g�ɂ���Ẳ����̐��l
float							drift_count = 0.0f;			// �h���t�g�J�E���g
float							drift_rot = 0.0f;			// �h���t�g���̌X��

static LPDIRECT3DDEVICE9		pDevice;
static DWORD					dwNumMaterials = 0;
static LPD3DXMESH				pMesh = NULL;				//���b�V��
static D3DMATERIAL9*			pMaterials = NULL;			//�}�e���A��
static LPDIRECT3DTEXTURE9*		pTextures = NULL;			//�e�N�X�`��

//=============================================================================
// ����������
//=============================================================================
HRESULT Player_Initialize(void)
{
	pDevice = GetD3DDevice();

	// X�t�@�C�����烁�b�V�������[�h���� 
	LPD3DXBUFFER pD3DXMtrlBuffer = NULL;

	if (FAILED(D3DXLoadMeshFromX("player.x", D3DXMESH_SYSTEMMEM,
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
	g_posPlayer = D3DXVECTOR3(20.0f, 100.0f, -50.0f);
	g_rotPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_sclPlayer = D3DXVECTOR3(50.0f, 50.0f, 50.0f);
	g_normalPlayer= D3DXVECTOR3(0.0f, 100.0f, 0.0f);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void Player_Finalize(void)
{
	if (g_pVtxBuffPlayer != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}

	SAFE_RELEASE(pMesh);
	SAFE_RELEASE(pDevice);
}

//=============================================================================
// �X�V����
//=============================================================================
void Player_Update(void)
{
	// �d��
	g_posPlayer.y -= GRAVITY;

	// ��key
	if (Keyboard_IsPress(DIK_UP))
	{
		// ����
		if (accel < MAXSPEED) accel += 10.0f;
	}
	else
	{
		// �������Ă��Ȃ��Ƃ����X�ɃX�s�[�h��������
		if (accel > 0) accel -= 5.0f;
		if (accel < 0) accel += 5.0f;
	}
	// �����Ă���Ƃ�
	if(accel+drift+item != 0)
	{
		// ��key
		if (Keyboard_IsPress(DIK_LEFT))
		{
			// Y������]
			g_rotPlayer.y -= VALUE_ROTATE;
			// Z���̉�]
			g_rotPlayer.z += VALUE_ROTATE;
			// �h���t�g���̌X��
			drift_rot -= VALUE_ROTATE;
			// �h���t�g�J�E���g
			drift_count += 1.0f;
		}
		// ��key
		else if (Keyboard_IsPress(DIK_RIGHT))
		{
			// Y���E��]
			g_rotPlayer.y += VALUE_ROTATE;
			// Z���̉�]
			g_rotPlayer.z -= VALUE_ROTATE;
			// �h���t�g���̌X��
			drift_rot += VALUE_ROTATE;
			// �h���t�g�J�E���g
			drift_count += 1.0f;
		}
		else
		{
			// �h���t�g�̂��܂��ŉ��������܂�
			if (drift_count > 50.0f) drift = 200.0f;
			if (drift_count > 100.0f) drift = 400.0f;
			drift_count = 0.0f;
		}

		// ��]����(Z��)
		if (g_rotPlayer.z > D3DX_PI / Player_ANGLE)
		{
			g_rotPlayer.z = D3DX_PI / Player_ANGLE;
		}
		if (g_rotPlayer.z < -D3DX_PI / Player_ANGLE)
		{
			g_rotPlayer.z = -D3DX_PI / Player_ANGLE;
		}

		// ��]�̐���(y��)
		if (g_rotPlayer.y > D3DX_PI)
		{
			g_rotPlayer.y -= D3DX_PI * 2.0f;
		}
		if (g_rotPlayer.y < -D3DX_PI)
		{
			g_rotPlayer.y += D3DX_PI * 2.0f;
		}

		// ��]�̐���(�h���t�g)
		if (drift_rot > D3DX_PI / Player_ANGLE)
		{
			drift_rot = D3DX_PI / Player_ANGLE;
		}
		if (drift_rot < -D3DX_PI / Player_ANGLE)
		{
			drift_rot = -D3DX_PI / Player_ANGLE;
		}
	}

	// �ړ�
	g_posPlayer.x += sinf(g_rotPlayer.y) * (accel + item + drift) / 20;
	g_posPlayer.z += cosf(g_rotPlayer.y) * (accel + item + drift) / 20;

	// �A�N�Z���̃Y�����C������
	if (accel<5.0f && accel>-5.0f) accel = 0.0f;

	// �h���t�g����
	if (drift != 0.0f) drift -= 5.0f;

	// �h���t�g�Y�����C��
	if (drift<1.0f&&drift>-1.0f) drift = 0.0f;

	// �G�i�W�[�h�����N�ɓ���������
	if (Get_DrinkHit()) item += 300.0f;
	else if (item != 0.0f) item -= 5.0f;

	// �A�C�e���Y�����C��
	if (item<1.0f&&item>-1.0f) item = 0.0f;

	// z���̌X����0�Ɏ���
	if (g_rotPlayer.z != 0.0f)
	{
		if (g_rotPlayer.z > 0) g_rotPlayer.z -= VALUE_ROTATE * 0.7f;
		if (g_rotPlayer.z < 0) g_rotPlayer.z += VALUE_ROTATE * 0.7f;
	}

	// �h���t�g���̌X����0�Ɏ���
	if (drift_rot != 0.0f)
	{
		if (drift_rot > 0) drift_rot -= VALUE_ROTATE * 0.7f;
		if (drift_rot < 0) drift_rot += VALUE_ROTATE * 0.7f;
	}

	// �O���E���h�����蔻��
	SetPlayer_Pos(SetGroundHit(Ground_Check()));

	// ���Z�b�g
	if (Keyboard_IsPress(DIK_RETURN))
	{
		g_posPlayer.x = -50.0f;
		g_posPlayer.y = 100.0f;
		g_posPlayer.z = 20.0f;

		g_rotPlayer.x = 0.0f;
		g_rotPlayer.y = 0.0f;
		g_rotPlayer.z = 0.0f;
	}

	DebugProc_Print((char *)"*** �R�c�|���S������ ***\n");
	DebugProc_Print((char *)"�ʒu [%f : %f : %f]\n", g_posPlayer.x, g_posPlayer.y, g_posPlayer.z);
	DebugProc_Print((char *)"�O�ړ� : ��\n");
	DebugProc_Print((char *)"��ړ� : ��\n");
	DebugProc_Print((char *)"���ړ� : ��\n");
	DebugProc_Print((char *)"�E�ړ� : ��\n");
	DebugProc_Print((char *)"\n");

	DebugProc_Print((char *)"���� [%f : %f : %f]\n", g_rotPlayer.x, g_rotPlayer.y, g_rotPlayer.z);
	DebugProc_Print((char *)"\n");

	DebugProc_Print((char *)"�h���t�g�J�E���g[%f]", drift_count);
	DebugProc_Print((char *)"\n");


	DebugProc_Print((char *)"�ʒu�E�������Z�b�g : ENTER\n");
}

//=============================================================================
// �`�揈��
//=============================================================================
void Player_Draw(void)
{
	//�f�o�C�X�������Ȃ�I��
	if (NULL == pDevice) { return; }

	D3DXMATRIXA16 mtxScl, mtxRot, mtxTranslate;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldPlayer);

	// �X�P�[�����]
	D3DXMatrixScaling(&mtxScl, g_sclPlayer.x, g_sclPlayer.y, g_sclPlayer.z);
	D3DXMatrixMultiply(&g_mtxWorldPlayer, &g_mtxWorldPlayer, &mtxScl);

	// ��]�𔽓]
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotPlayer.y+drift_rot, g_rotPlayer.x, g_rotPlayer.z);
	D3DXMatrixMultiply(&g_mtxWorldPlayer, &g_mtxWorldPlayer, &mtxRot);
	
	// �ړ��𔽓]
	D3DXMatrixTranslation(&mtxTranslate, g_posPlayer.x, g_posPlayer.y, g_posPlayer.z);
	D3DXMatrixMultiply(&g_mtxWorldPlayer, &g_mtxWorldPlayer, &mtxTranslate);

	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldPlayer);

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

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);		// ���C�g��L��
	pDevice->SetRenderState(D3DRS_AMBIENT, 0x00aaaaaa);	// ������ݒ�
	pDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);// ���ʔ��˗L��
	//pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);	// Z�o�b�t�@�[��L��
}

//-----------------------------------------------------------------------------
// �Q�b�^�[
//-----------------------------------------------------------------------------
D3DXVECTOR3 GetPlayer_Pos()
{
	return g_posPlayer;
}
D3DXVECTOR3 GetPlayer_Rot()
{
	return g_rotPlayer;
}

D3DXVECTOR3 GetNormalPlayer_Pos()
{
	return g_normalPlayer;
}

void SetPlayer_Pos(float set) 
{
	g_posPlayer.y = set;
}