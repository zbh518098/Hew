
#pragma once

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
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
	D3DXMATRIX				g_mtxWorldGround;		// ワールドマトリックス
	D3DXVECTOR3				g_posGround;				// 地面の位置
	D3DXVECTOR3				g_rotGround;				// 地面の向き(回転)
	D3DXVECTOR3				g_sclGround;				// 地面の大きさ(スケール)

	void Initialize(LPDIRECT3DDEVICE9 InDevice, D3DXVECTOR3 x_z_pos);
	void Finalize();
	
	int		GetGroundNum();			//ブロックナンバー取得
	void	SetGroundNum(int num);	//ブロックナンバー設定

	
	D3DXVECTOR3 GetPoint0();
	D3DXVECTOR3 GetPoint1();
	D3DXVECTOR3 GetPoint2();
	D3DXVECTOR3 GetPoint3();

	

	
	GROUND();
	~GROUND();

	VERTEX_3D *LookVtx;//ポインタのポインタ
	int		  *LookIdxNum;//インデックスのポインタ
private:


	int		this_ground_num;
	
	D3DXVECTOR3	Point0;
	D3DXVECTOR3	Point1;
	D3DXVECTOR3	Point2;
	D3DXVECTOR3	Point3;	
};