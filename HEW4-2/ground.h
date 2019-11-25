
#pragma once

#include "main.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void Ground_Initialize(void);
void Ground_Finalize(void);
void Ground_Update(void);
void Ground_Draw(void);
HRESULT MakeVertexGround(LPDIRECT3DDEVICE9 pDevice, int ground_num);
float SetGroundHit( int g_num);
void Block0_Stage(VERTEX_3D *pvtx, int g_num);
void Block1_Stage(VERTEX_3D *pvtx, int g_num);
void Block2_Stage(VERTEX_3D *pvtx, int g_num);
void Block3_Stage(VERTEX_3D *pvtx, int g_num);

void MakeIdx(WORD *Idx, int g_num);
int Ground_Check();

class GROUND
{
public:
	D3DXMATRIX				g_mtxWorldGround;		// ���[���h�}�g���b�N�X
	D3DXVECTOR3				g_posGround;				// �n�ʂ̈ʒu
	D3DXVECTOR3				g_rotGround;				// �n�ʂ̌���(��])
	D3DXVECTOR3				g_sclGround;				// �n�ʂ̑傫��(�X�P�[��)

	void Initialize(LPDIRECT3DDEVICE9 InDevice, D3DXVECTOR3 x_z_pos);
	void Finalize();
	
	int		GetGroundNum();			//�u���b�N�i���o�[�擾
	void	SetGroundNum(int num);	//�u���b�N�i���o�[�ݒ�

	
	D3DXVECTOR3 GetPoint0();
	D3DXVECTOR3 GetPoint1();
	D3DXVECTOR3 GetPoint2();
	D3DXVECTOR3 GetPoint3();

	

	
	GROUND();
	~GROUND();

	VERTEX_3D *LookVtx;//�|�C���^�̃|�C���^
	int		  *LookIdxNum;//�C���f�b�N�X�̃|�C���^
private:


	int		this_ground_num;
	
	D3DXVECTOR3	Point0;
	D3DXVECTOR3	Point1;
	D3DXVECTOR3	Point2;
	D3DXVECTOR3	Point3;	
};