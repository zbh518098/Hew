#include <iostream>
#include <time.h>
#include <stdlib.h>
#include "myDirect3D.h"
#include "input.h"
#include "texture.h"
#include "debugproc.h"
#include "ground.h"
#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PLATE_SIZE		(30)						//�v���[�g�̑傫��

#define	PLATE_X_NUM		(10)						//�v���[�g�̐�
#define PLATE_Z_NUM		(10)						//�v���[�g�̐�
#define	PLATE_NUM		(PLATE_X_NUM)*(PLATE_Z_NUM)	//1�u���b�N���̃v���[�g�̑���




#define GROUND_NUM	(15)//�u���b�N�̑���
#define GROUND_X_SIZE		((PLATE_X_NUM) * (PLATE_SIZE))//�u���b�N��X�T�C�Y
#define GROUND_Z_SIZE		((PLATE_Z_NUM) * (PLATE_SIZE))//�u���b�N��Z�T�C�Y
//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffGround[GROUND_NUM];	// ���_�o�b�t�@�ւ̃|�C���^
static LPDIRECT3DINDEXBUFFER9	g_pIdxBuffGround[GROUND_NUM];	// ���_�o�b�t�@�ւ̃|�C���^



GROUND	ground[GROUND_NUM];	//�O���E���h��GROUN_NUM������
	
int		PrimCount = (PLATE_X_NUM * PLATE_Z_NUM) * 2 + (PLATE_Z_NUM - 1) * 4;;	//�|���S����

int		NumIndex = (PLATE_X_NUM + 1) * 2 * PLATE_Z_NUM + (PLATE_Z_NUM - 1) * 2;	//���_�C���f�b�N�X��

int		NumVertex = (PLATE_X_NUM + 1) * (PLATE_Z_NUM + 1);						//���_��

GROUND::GROUND()
{
	//�G���[���m���l
	this_ground_num = 999;
}


GROUND::~GROUND()
{
}

//Ground�֐��̏������i�������̓u���b�N�̒��S���W�j
void GROUND::Initialize(LPDIRECT3DDEVICE9 InDevice, D3DXVECTOR3 ground_pos) {


	LookVtx = new VERTEX_3D[NumVertex];//�@VRAM���玝���Ă���Vtx���̗e�ʊm��
	LookIdxNum = new int[NumIndex];

	// �ʒu�E��]�E�X�P�[���̏����ݒ�

	g_posGround = ground_pos;//���S�_
	g_rotGround = D3DXVECTOR3(0,0, 0.0f);//��]�p�B��{�I�ɂ͂�����Ȃ�
	g_sclGround = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	
	/*
		1�u���b�N���ォ�猩���Ƃ��iX-Z���W�j��4���_��Point�Ƃ���

		Point1�@�@Point2
		�@__________
		  |         |
		  |         |
		  |_________|
		Point0�@�@Point3
	*/

	Point0 = D3DXVECTOR3(g_posGround.x - GROUND_X_SIZE, g_posGround.y,g_posGround.z);
	Point1 = D3DXVECTOR3(g_posGround.x - GROUND_X_SIZE, g_posGround.y,g_posGround.z + GROUND_Z_SIZE);
	Point2 = D3DXVECTOR3(g_posGround.x,g_posGround.y,g_posGround.z + GROUND_Z_SIZE);
	Point3 = D3DXVECTOR3(g_posGround.x,g_posGround.y,g_posGround.z);

}

void GROUND::Finalize() {
	
}


int  GROUND::GetGroundNum() {
	return this_ground_num;
}

void GROUND::SetGroundNum(int num) {
	this_ground_num = num;
}


D3DXVECTOR3 GROUND::GetPoint0() {
	return Point0;
}
D3DXVECTOR3 GROUND::GetPoint1() {
	return Point1;
}
D3DXVECTOR3 GROUND::GetPoint2() {
	return Point2;
}
D3DXVECTOR3 GROUND::GetPoint3() {
	return Point3;
}



//=============================================================================
// ����������
//=============================================================================
void Ground_Initialize(void)
{
	for (int i = 0; i < GROUND_NUM; i++)
	{
		g_pVtxBuffGround[i] = NULL;
		g_pIdxBuffGround[i] = NULL;
	}

	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	
	ground[0].SetGroundNum(1);//�u���b�N�̎�ޔԍ�
	ground[1].SetGroundNum(1);//�u���b�N�̎�ޔԍ�
	ground[2].SetGroundNum(1);//�u���b�N�̎�ޔԍ�
	ground[3].SetGroundNum(2);//�u���b�N�̎�ޔԍ�
	ground[4].SetGroundNum(1);//�u���b�N�̎�ޔԍ�
	ground[5].SetGroundNum(1);//�u���b�N�̎�ޔԍ�
	ground[6].SetGroundNum(0);//�u���b�N�̎�ޔԍ�
	ground[7].SetGroundNum(3);//�u���b�N�̎�ޔԍ�
	ground[8].SetGroundNum(1);//�u���b�N�̎�ޔԍ�
	ground[9].SetGroundNum(1);//�u���b�N�̎�ޔԍ�
	ground[10].SetGroundNum(1);//�u���b�N�̎�ޔԍ�



	for (int i = 0; i < 8; i++)
	{
		ground[i].Initialize(pDevice, D3DXVECTOR3(0.0f,0.0f, GROUND_Z_SIZE * i));
	
		// ���_���̍쐬
		MakeVertexGround(pDevice, i);//�����ɓ���̂͑S�Ă̒��_���ɂȂ�	
	}		
	ground[8].Initialize(pDevice, D3DXVECTOR3(GROUND_X_SIZE * 1.0f, 0.0f, GROUND_Z_SIZE * 7));
	MakeVertexGround(pDevice, 8);//�����ɓ���̂͑S�Ă̒��_���ɂȂ�	
	ground[9].Initialize(pDevice, D3DXVECTOR3(GROUND_X_SIZE * 2.0f, 0.0f, GROUND_Z_SIZE * 7));
	MakeVertexGround(pDevice, 9);//�����ɓ���̂͑S�Ă̒��_���ɂȂ�	
	ground[10].Initialize(pDevice, D3DXVECTOR3(GROUND_X_SIZE * 3.0f, 0.0f, GROUND_Z_SIZE * 7));
	MakeVertexGround(pDevice, 10);//�����ɓ���̂͑S�Ă̒��_���ɂȂ�	
}

//=============================================================================
// �I������
//=============================================================================
void Ground_Finalize(void)
{
	

	for (int i = 0; i < GROUND_NUM; i++)
	{
		delete[] ground[i].LookVtx;
		delete[] ground[i].LookIdxNum;
		if (g_pIdxBuffGround[i] != NULL)
		{// ���_�o�b�t�@�̊J��
			g_pIdxBuffGround[i]->Release();
			g_pIdxBuffGround[i] = NULL;
		}

		if (g_pVtxBuffGround[i] != NULL)
		{// ���_�o�b�t�@�̊J��
			g_pVtxBuffGround[i]->Release();
			g_pVtxBuffGround[i] = NULL;
		}
	}
	
}

void Ground_Update(void) {

	D3DXVECTOR3 a = GetPlayer_Pos();
	DebugProc_Print((char *) "�v���C���[�̈ʒu�y%f, %f, %f�z\n",a.x , a.y, a.z);

	
}

//=============================================================================
// �`�揈��
//=============================================================================
void Ground_Draw(void)
{
	

	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();


	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;



	for (int i = 0; i < GROUND_NUM; i++)
	{

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&ground[i].g_mtxWorldGround);

		// �X�P�[�����]
		D3DXMatrixScaling(&mtxScl, ground[i].g_sclGround.x, ground[i].g_sclGround.y, ground[i].g_sclGround.z);
		D3DXMatrixMultiply(&ground[i].g_mtxWorldGround, &ground[i].g_mtxWorldGround, &mtxScl);

		// �ړ��𔽓]
		D3DXMatrixTranslation(&mtxTranslate, ground[i].g_posGround.x, ground[i].g_posGround.y, ground[i].g_posGround.z);
		//D3DXMatrixMultiply(&ground[i].g_mtxWorldGround, &ground[i].g_mtxWorldGround, &mtxTranslate);

		// ��]�𔽓]
		D3DXMatrixRotationYawPitchRoll(&mtxRot, ground[i].g_rotGround.y, ground[i].g_rotGround.x, ground[i].g_rotGround.z);
		D3DXMatrixMultiply(&ground[i].g_mtxWorldGround, &ground[i].g_mtxWorldGround, &mtxRot);



		pDevice->SetTransform(D3DTS_WORLD, &ground[i].g_mtxWorldGround);
		pDevice->SetStreamSource(0, g_pVtxBuffGround[i], 0, sizeof(VERTEX_3D));

		//�C���f�b�N�X�o�b�t�@�̃Z�b�g
		pDevice->SetIndices(g_pIdxBuffGround[i]);

		pDevice->SetFVF(FVF_VERTEX_3D);

		pDevice->SetTexture(0, Texture_GetTexture(TEXTURE_INDEX_FIELD01));

		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, NumVertex, 0, PrimCount);
	}

}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexGround(LPDIRECT3DDEVICE9 pDevice, int ground_num)
{
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NumVertex, //VRAM�Ƀ����������
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGround[ground_num],
		NULL)))
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		
		VERTEX_3D *pVtx;
		

		//Lock,unLock�����܂肵�Ȃ��̂��p�t�H�[�}���X���グ����@
		g_pVtxBuffGround[ground_num]->Lock(0, 0, (void**)&pVtx, 0); //VRAM�ɂ��������������V�X�e����Ɏ����Ă��Ă����
		
		switch (ground[ground_num].GetGroundNum())
		{
		case 0:
			Block0_Stage(pVtx, ground_num);//�u���b�N1���쐬
			break;
		case 1:
			Block1_Stage(pVtx, ground_num);//�u���b�N2���쐬
			break;
		case 2:
			Block2_Stage(pVtx, ground_num);//�u���b�N2���쐬
			break;
		case 3:
			Block3_Stage(pVtx, ground_num);//�u���b�N2���쐬
			break;
		default:
			break;
		}
		
	
		g_pVtxBuffGround[ground_num]->Unlock(); //VRAM�Ƀ�������Ԃ�(�d������)
	}

	
	if (FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) * NumIndex, //VRAM�Ƀ����������
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffGround[ground_num],
		NULL)))
	{
		return E_FAIL;
	}

	{//�C���f�b�N�X�o�b�t�@�̒��g�𖄂߂�
		WORD *pIdx;

		g_pIdxBuffGround[ground_num]->Lock(0, 0, (void**)&pIdx, 0);

		MakeIdx(pIdx, ground_num);

		g_pIdxBuffGround[ground_num]->Unlock();
	}
	
	return S_OK;
}

int Ground_Check() {
	//D3DXVECTOR3 player_pos = GetBike_Pos();
	D3DXVECTOR3 player_pos = GetPlayer_Pos();
	for (int i = 0; i < GROUND_NUM; i++)
	{
		if (ground[i].GetPoint0().x <= player_pos.x &&
			ground[i].GetPoint2().x >= player_pos.x &&
			ground[i].GetPoint0().z <= player_pos.z &&
			ground[i].GetPoint2().z >= player_pos.z )
		{
			
			return i;
		}
	}
	//DebugProc_Print((char *)"�ڂ��Ă���u���b�N�͂Ȃ���\n");
	return 0;//�G���[���m�����ق�����������
}

float SetGroundHit( int g_num) {


	D3DXVECTOR3 player_botom_position = GetPlayer_Pos();//���݂̃v���C���[�̈ʒu;
	player_botom_position.y -= 15.0f;
	D3DXVECTOR3 next_player_botom_position = D3DXVECTOR3(player_botom_position.x, 0.0f, player_botom_position.z);


	D3DXVECTOR3 vector1, vector2, vector3;//�O�p�`�̎O�ӂ��x�N�g����


	D3DXVECTOR3 hosen1, hosen2, hosen3;
	for (int i = 0; i < NumIndex; i++)
	{
		if ((ground[g_num].LookIdxNum[i + 2]) > NumIndex || (ground[g_num].LookIdxNum[i + 2]) < 0)
		{
			//�k�ރ|���S���̃C���f�b�N�X�͔�΂�
			i += 3;
			continue;
		}				


		vector1 = D3DXVECTOR3(ground[g_num].LookVtx[ground[g_num].LookIdxNum[i]].pos - ground[g_num].LookVtx[ground[g_num].LookIdxNum[i + 1]].pos);
		vector1.y = 0;

		vector2 = D3DXVECTOR3(ground[g_num].LookVtx[ground[g_num].LookIdxNum[i + 1]].pos - ground[g_num].LookVtx[ground[g_num].LookIdxNum[i + 2]].pos);
		vector2.y = 0;

		vector3 = D3DXVECTOR3(ground[g_num].LookVtx[ground[g_num].LookIdxNum[i + 2]].pos - ground[g_num].LookVtx[ground[g_num].LookIdxNum[i]].pos);
		vector3.y = 0;

		
		D3DXVec3Cross(&hosen1, &(next_player_botom_position -
			(D3DXVECTOR3(ground[g_num].LookVtx[ground[g_num].LookIdxNum[i]].pos.x, 0.0f, ground[g_num].LookVtx[ground[g_num].LookIdxNum[i]].pos.z))), &vector1);

		D3DXVec3Cross(&hosen2, &(next_player_botom_position -
			(D3DXVECTOR3(ground[g_num].LookVtx[ground[g_num].LookIdxNum[i + 1]].pos.x, 0.0f, ground[g_num].LookVtx[ground[g_num].LookIdxNum[i + 1]].pos.z))), &vector2);



		//�v���C���[�̂���|���S���ʂ��擾
		
		if (hosen1.y >= 0 && hosen2.y >= 0 || hosen1.y <= 0 && hosen2.y <= 0)
		{
			if (hosen1.y == 0)//���_���ui�v�̂Ƃ��̂͌v�Z�����Ȃ��Ă����̂ł��̂܂ܒl��Ԃ�
			{
				return player_botom_position.y + 15.0f;
			}
			else if (hosen2.y == 0)//���_���ui�v�̂Ƃ��̂͌v�Z�����Ȃ��Ă����̂ł��̂܂ܒl��Ԃ�
			{
				return player_botom_position.y + 15.0f;
			}
			else
			{

				D3DXVec3Cross(&hosen3, &(next_player_botom_position -
					(D3DXVECTOR3(ground[g_num].LookVtx[ground[g_num].LookIdxNum[i + 2]].pos.x, 0.0f, ground[g_num].LookVtx[ground[g_num].LookIdxNum[i + 2]].pos.z))), &vector3);


				if (hosen2.y >= 0 && hosen3.y >= 0 || hosen2.y <= 0 && hosen3.y <= 0)
				{

					if (hosen3.y == 0)//���_���ui�v�̂Ƃ��̂͌v�Z�����Ȃ��Ă����̂ł��̂܂ܒl��Ԃ�
					{
						return player_botom_position.y + 15.0f;
					}
					else {
						D3DXVECTOR3 naiseki1, naiseki2;
						naiseki1 = D3DXVECTOR3(
							player_botom_position.x - ground[g_num].LookVtx[ground[g_num].LookIdxNum[i]].pos.x,
							player_botom_position.y - ground[g_num].LookVtx[ground[g_num].LookIdxNum[i]].pos.y,
							player_botom_position.z - ground[g_num].LookVtx[ground[g_num].LookIdxNum[i]].pos.z);						//PA

						naiseki2 = D3DXVECTOR3(
							(player_botom_position.x + GetNormalPlayer_Pos().x) - ground[g_num].LookVtx[ground[g_num].LookIdxNum[i]].pos.x,
							(player_botom_position.y + GetNormalPlayer_Pos().y) - ground[g_num].LookVtx[ground[g_num].LookIdxNum[i]].pos.y,
							(player_botom_position.z + GetNormalPlayer_Pos().z) - ground[g_num].LookVtx[ground[g_num].LookIdxNum[i]].pos.z);	//PB

						D3DXVECTOR3 n;//�@���x�N�g��
						D3DXVECTOR3 AB = GetNormalPlayer_Pos();

						//�x�N�g���Đݒ�
						vector1 = D3DXVECTOR3(ground[g_num].LookVtx[ground[g_num].LookIdxNum[i + 1]].pos - ground[g_num].LookVtx[ground[g_num].LookIdxNum[i]].pos);
						vector3 = D3DXVECTOR3(ground[g_num].LookVtx[ground[g_num].LookIdxNum[i + 2]].pos - ground[g_num].LookVtx[ground[g_num].LookIdxNum[i]].pos);
						//vector3.x *= -1.0f;
						//vector3.y *= -1.0f;
						//vector3.z *= -1.0f;



						//vector3 *=  -1.0f;//�}�C�i�X�x�N�g���ɂ���
						D3DXVec3Cross(&n, &vector1, &vector3);//�O�ςŖʖ@�����v�Z

						//�n�ʂ�艺�ɂ�����n�ʏ�ɍĐݒu
						if ((D3DXVec3Dot(&naiseki2, &n)) > 0.00 && (D3DXVec3Dot(&naiseki1, &n)) <= 0.00 ||
							(D3DXVec3Dot(&naiseki2, &n)) <= 0.00 && (D3DXVec3Dot(&naiseki1, &n)) > 0.00) {


							player_botom_position.y += AB.y * (fabsf(D3DXVec3Dot(&n, &naiseki1) / (fabsf(D3DXVec3Dot(&n, &naiseki1) + fabsf(D3DXVec3Dot(&n, &naiseki2))))));


							//DebugProc_Print((char *)"�C���f�b�N�X�F�y%d�z���W1�F�y%f,%f,%f�z\n", ground[g_num].LookIdxNum[i], ground[g_num].LookVtx[ground[g_num].LookIdxNum[i]].pos.x, ground[g_num].LookVtx[ground[g_num].LookIdxNum[i]].pos.y, ground[g_num].LookVtx[ground[g_num].LookIdxNum[i]].pos.z);
							//DebugProc_Print((char *)"�C���f�b�N�X�F�y%d�z���W2�F�y%f,%f,%f�z\n", ground[g_num].LookIdxNum[i + 1], ground[g_num].LookVtx[ground[g_num].LookIdxNum[i + 1]].pos.x, ground[g_num].LookVtx[ground[g_num].LookIdxNum[i + 1]].pos.y, ground[g_num].LookVtx[ground[g_num].LookIdxNum[i + 1]].pos.z);
							//DebugProc_Print((char *)"�C���f�b�N�X�F�y%d�z���W3�F�y%f,%f,%f�z\n", ground[g_num].LookIdxNum[i + 2], ground[g_num].LookVtx[ground[g_num].LookIdxNum[i + 2]].pos.x, ground[g_num].LookVtx[ground[g_num].LookIdxNum[i + 2]].pos.y, ground[g_num].LookVtx[ground[g_num].LookIdxNum[i + 2]].pos.z);

							//DebugProc_Print((char *)"\n�y%f�z\n", player_botom_position.y);
							return player_botom_position.y + 15.0f;
						}
						return player_botom_position.y + 15.0f;
					}
				}
			}
		}
		
	}

	return player_botom_position.y +15.0f;
}

//���_�����֐����쐬����

void Block0_Stage(VERTEX_3D *pvtx,int g_num) {
	srand((unsigned int)time(NULL));
	D3DXVECTOR3 base_position = ground[g_num].g_posGround;
	int katamuki_hani = 5;

	for (int z = 0; z < PLATE_Z_NUM + 1; z++)
	{
		for (int x = 0; x < PLATE_X_NUM + 1; x++)
		{
			pvtx[(PLATE_X_NUM + 1)* z + x].pos = D3DXVECTOR3(-(PLATE_SIZE * x) + base_position.x + ground[g_num].GetPoint3().x, x * z * 1.0f , (PLATE_SIZE * z) + base_position.z);
			/*if (((PLATE_X_NUM + 1)* z + x) % (PLATE_X_NUM + 1) < 5)
			{
				pvtx[(PLATE_X_NUM + 1)* z + x].pos.y = (5 - (((PLATE_X_NUM + 1)* z + x) % (PLATE_X_NUM + 1))) * 10;
			}*/

			pvtx[(PLATE_X_NUM + 1)* z + x].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pvtx[(PLATE_X_NUM + 1)* z + x].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pvtx[(PLATE_X_NUM + 1)* z + x].tex = D3DXVECTOR2(1.0f * x, 1.0f * z);

			ground[g_num].LookVtx[(PLATE_X_NUM + 1)* z + x] = pvtx[(PLATE_X_NUM + 1)* z + x];
		}
	}
}

void Block1_Stage(VERTEX_3D *pvtx, int g_num) {

	D3DXVECTOR3 base_position = ground[g_num].g_posGround;
	int katamuki_hani = 5;

	for (int z = 0; z < PLATE_Z_NUM + 1; z++)
	{
		for (int x = 0; x < PLATE_X_NUM + 1; x++)
		{
			pvtx[(PLATE_X_NUM + 1)* z + x].pos = D3DXVECTOR3( - (PLATE_SIZE * x) + base_position.x, 0.0f, (PLATE_SIZE * z) + base_position.z);
			pvtx[(PLATE_X_NUM + 1)* z + x].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pvtx[(PLATE_X_NUM + 1)* z + x].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pvtx[(PLATE_X_NUM + 1)* z + x].tex = D3DXVECTOR2(1.0f * x, 1.0f * z);

			ground[g_num].LookVtx[(PLATE_X_NUM + 1)* z + x] = pvtx[(PLATE_X_NUM + 1)* z + x];
		}
	}
}

void Block2_Stage(VERTEX_3D *pvtx, int g_num) {

	D3DXVECTOR3 base_position = ground[g_num].g_posGround;
	int katamuki_hani = 5;

	for (int z = 0; z < PLATE_Z_NUM + 1; z++)
	{
		for (int x = 0; x < PLATE_X_NUM + 1; x++)
		{
			pvtx[(PLATE_X_NUM + 1)* z + x].pos = D3DXVECTOR3(-(PLATE_SIZE * x) + base_position.x, +(z * 5.0f), (PLATE_SIZE * z) + base_position.z );
			pvtx[(PLATE_X_NUM + 1)* z + x].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pvtx[(PLATE_X_NUM + 1)* z + x].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pvtx[(PLATE_X_NUM + 1)* z + x].tex = D3DXVECTOR2(1.0f * x, 1.0f * z);

			ground[g_num].LookVtx[(PLATE_X_NUM + 1)* z + x] = pvtx[(PLATE_X_NUM + 1)* z + x];
		}
	}
}

void Block3_Stage(VERTEX_3D *pvtx, int g_num) {
	srand((unsigned int)time(NULL));
	D3DXVECTOR3 base_position = ground[g_num].g_posGround;
	int katamuki_hani = 5;

	for (int z = 0; z < PLATE_Z_NUM + 1; z++)
	{
		for (int x = 0; x < PLATE_X_NUM + 1; x++)
		{
			pvtx[(PLATE_X_NUM + 1)* z + x].pos = D3DXVECTOR3(-(PLATE_SIZE * x) + base_position.x + ground[g_num].GetPoint3().x,( x * ((PLATE_Z_NUM+1) - z) * 1.0f), (PLATE_SIZE * z) + base_position.z);
			/*if (((PLATE_X_NUM + 1)* z + x) % (PLATE_X_NUM + 1) < 5)
			{
				pvtx[(PLATE_X_NUM + 1)* z + x].pos.y = (5 - (((PLATE_X_NUM + 1)* z + x) % (PLATE_X_NUM + 1))) * 10;
			}*/

			pvtx[(PLATE_X_NUM + 1)* z + x].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pvtx[(PLATE_X_NUM + 1)* z + x].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pvtx[(PLATE_X_NUM + 1)* z + x].tex = D3DXVECTOR2(1.0f * x, 1.0f * z);

			ground[g_num].LookVtx[(PLATE_X_NUM + 1)* z + x] = pvtx[(PLATE_X_NUM + 1)* z + x];
		}
	}
}

void MakeIdx(WORD *Idx, int g_num) {
	
	int j = 0;
	int i = 0;
	for (int k = 0; k < PLATE_Z_NUM; k++)
	{

		for (j; j <= PLATE_X_NUM; j++)
		{
			Idx[(j * 2) + i] = (PLATE_X_NUM + 1) + j + (PLATE_X_NUM + 1) * k;
			ground[g_num].LookIdxNum[(j * 2) + i] = Idx[(j * 2) + i];
			Idx[(j * 2) + i + 1] = j + (PLATE_X_NUM + 1) * k;
			ground[g_num].LookIdxNum[(j * 2) + i + 1] = Idx[(j * 2) + i + 1];
		}
		if (k == PLATE_Z_NUM - 1)
		{
			break;
		}
		Idx[(j * 2) + i + 2] = Idx[(j * 2) + i + 2];
		ground[g_num].LookIdxNum[(j * 2) + i + 2] = j + (PLATE_X_NUM + 1) * k;
		Idx[(j * 2) + i + 3] = j + (PLATE_X_NUM + 2) + (PLATE_X_NUM + 1) * k;
		ground[g_num].LookIdxNum[(j * 2) + i + 3] = j + (PLATE_X_NUM + 2) + (PLATE_X_NUM + 1) * k;

		i += (2 * (PLATE_X_NUM + 2));
		j = 0;
	}
}