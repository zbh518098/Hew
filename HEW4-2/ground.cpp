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
// マクロ定義
//*****************************************************************************
#define PLATE_SIZE		(30)						//プレートの大きさ

#define	PLATE_X_NUM		(10)						//プレートの数
#define PLATE_Z_NUM		(10)						//プレートの数
#define	PLATE_NUM		(PLATE_X_NUM)*(PLATE_Z_NUM)	//1ブロック内のプレートの総数




#define GROUND_NUM	(15)//ブロックの総数
#define GROUND_X_SIZE		((PLATE_X_NUM) * (PLATE_SIZE))//ブロックのXサイズ
#define GROUND_Z_SIZE		((PLATE_Z_NUM) * (PLATE_SIZE))//ブロックのZサイズ
//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffGround[GROUND_NUM];	// 頂点バッファへのポインタ
static LPDIRECT3DINDEXBUFFER9	g_pIdxBuffGround[GROUND_NUM];	// 頂点バッファへのポインタ



GROUND	ground[GROUND_NUM];	//グラウンドをGROUN_NUM分生成
	
int		PrimCount = (PLATE_X_NUM * PLATE_Z_NUM) * 2 + (PLATE_Z_NUM - 1) * 4;;	//ポリゴン数

int		NumIndex = (PLATE_X_NUM + 1) * 2 * PLATE_Z_NUM + (PLATE_Z_NUM - 1) * 2;	//頂点インデックス数

int		NumVertex = (PLATE_X_NUM + 1) * (PLATE_Z_NUM + 1);						//頂点数

GROUND::GROUND()
{
	//エラー検知数値
	this_ground_num = 999;
}


GROUND::~GROUND()
{
}

//Ground関数の初期化（第二引数はブロックの中心座標）
void GROUND::Initialize(LPDIRECT3DDEVICE9 InDevice, D3DXVECTOR3 ground_pos) {


	LookVtx = new VERTEX_3D[NumVertex];//　VRAMから持ってくるVtx情報の容量確保
	LookIdxNum = new int[NumIndex];

	// 位置・回転・スケールの初期設定

	g_posGround = ground_pos;//中心点
	g_rotGround = D3DXVECTOR3(0,0, 0.0f);//回転角。基本的にはいじらない
	g_sclGround = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	
	/*
		1ブロックを上から見たとき（X-Z座標）の4頂点をPointとする

		Point1　　Point2
		　__________
		  |         |
		  |         |
		  |_________|
		Point0　　Point3
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
// 初期化処理
//=============================================================================
void Ground_Initialize(void)
{
	for (int i = 0; i < GROUND_NUM; i++)
	{
		g_pVtxBuffGround[i] = NULL;
		g_pIdxBuffGround[i] = NULL;
	}

	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	
	ground[0].SetGroundNum(1);//ブロックの種類番号
	ground[1].SetGroundNum(1);//ブロックの種類番号
	ground[2].SetGroundNum(1);//ブロックの種類番号
	ground[3].SetGroundNum(2);//ブロックの種類番号
	ground[4].SetGroundNum(1);//ブロックの種類番号
	ground[5].SetGroundNum(1);//ブロックの種類番号
	ground[6].SetGroundNum(0);//ブロックの種類番号
	ground[7].SetGroundNum(3);//ブロックの種類番号
	ground[8].SetGroundNum(1);//ブロックの種類番号
	ground[9].SetGroundNum(1);//ブロックの種類番号
	ground[10].SetGroundNum(1);//ブロックの種類番号



	for (int i = 0; i < 8; i++)
	{
		ground[i].Initialize(pDevice, D3DXVECTOR3(0.0f,0.0f, GROUND_Z_SIZE * i));
	
		// 頂点情報の作成
		MakeVertexGround(pDevice, i);//ここに入るのは全ての頂点数になる	
	}		
	ground[8].Initialize(pDevice, D3DXVECTOR3(GROUND_X_SIZE * 1.0f, 0.0f, GROUND_Z_SIZE * 7));
	MakeVertexGround(pDevice, 8);//ここに入るのは全ての頂点数になる	
	ground[9].Initialize(pDevice, D3DXVECTOR3(GROUND_X_SIZE * 2.0f, 0.0f, GROUND_Z_SIZE * 7));
	MakeVertexGround(pDevice, 9);//ここに入るのは全ての頂点数になる	
	ground[10].Initialize(pDevice, D3DXVECTOR3(GROUND_X_SIZE * 3.0f, 0.0f, GROUND_Z_SIZE * 7));
	MakeVertexGround(pDevice, 10);//ここに入るのは全ての頂点数になる	
}

//=============================================================================
// 終了処理
//=============================================================================
void Ground_Finalize(void)
{
	

	for (int i = 0; i < GROUND_NUM; i++)
	{
		delete[] ground[i].LookVtx;
		delete[] ground[i].LookIdxNum;
		if (g_pIdxBuffGround[i] != NULL)
		{// 頂点バッファの開放
			g_pIdxBuffGround[i]->Release();
			g_pIdxBuffGround[i] = NULL;
		}

		if (g_pVtxBuffGround[i] != NULL)
		{// 頂点バッファの開放
			g_pVtxBuffGround[i]->Release();
			g_pVtxBuffGround[i] = NULL;
		}
	}
	
}

void Ground_Update(void) {

	D3DXVECTOR3 a = GetPlayer_Pos();
	DebugProc_Print((char *) "プレイヤーの位置【%f, %f, %f】\n",a.x , a.y, a.z);

	
}

//=============================================================================
// 描画処理
//=============================================================================
void Ground_Draw(void)
{
	

	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();


	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;



	for (int i = 0; i < GROUND_NUM; i++)
	{

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&ground[i].g_mtxWorldGround);

		// スケール反転
		D3DXMatrixScaling(&mtxScl, ground[i].g_sclGround.x, ground[i].g_sclGround.y, ground[i].g_sclGround.z);
		D3DXMatrixMultiply(&ground[i].g_mtxWorldGround, &ground[i].g_mtxWorldGround, &mtxScl);

		// 移動を反転
		D3DXMatrixTranslation(&mtxTranslate, ground[i].g_posGround.x, ground[i].g_posGround.y, ground[i].g_posGround.z);
		//D3DXMatrixMultiply(&ground[i].g_mtxWorldGround, &ground[i].g_mtxWorldGround, &mtxTranslate);

		// 回転を反転
		D3DXMatrixRotationYawPitchRoll(&mtxRot, ground[i].g_rotGround.y, ground[i].g_rotGround.x, ground[i].g_rotGround.z);
		D3DXMatrixMultiply(&ground[i].g_mtxWorldGround, &ground[i].g_mtxWorldGround, &mtxRot);



		pDevice->SetTransform(D3DTS_WORLD, &ground[i].g_mtxWorldGround);
		pDevice->SetStreamSource(0, g_pVtxBuffGround[i], 0, sizeof(VERTEX_3D));

		//インデックスバッファのセット
		pDevice->SetIndices(g_pIdxBuffGround[i]);

		pDevice->SetFVF(FVF_VERTEX_3D);

		pDevice->SetTexture(0, Texture_GetTexture(TEXTURE_INDEX_FIELD01));

		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, NumVertex, 0, PrimCount);
	}

}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexGround(LPDIRECT3DDEVICE9 pDevice, int ground_num)
{
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NumVertex, //VRAMにメモリを作る
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGround[ground_num],
		NULL)))
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		
		VERTEX_3D *pVtx;
		

		//Lock,unLockをあまりしないのがパフォーマンスを上げる方法
		g_pVtxBuffGround[ground_num]->Lock(0, 0, (void**)&pVtx, 0); //VRAMにあったメモリをシステム上に持ってきてくれる
		
		switch (ground[ground_num].GetGroundNum())
		{
		case 0:
			Block0_Stage(pVtx, ground_num);//ブロック1を作成
			break;
		case 1:
			Block1_Stage(pVtx, ground_num);//ブロック2を作成
			break;
		case 2:
			Block2_Stage(pVtx, ground_num);//ブロック2を作成
			break;
		case 3:
			Block3_Stage(pVtx, ground_num);//ブロック2を作成
			break;
		default:
			break;
		}
		
	
		g_pVtxBuffGround[ground_num]->Unlock(); //VRAMにメモリを返す(重い処理)
	}

	
	if (FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) * NumIndex, //VRAMにメモリを作る
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffGround[ground_num],
		NULL)))
	{
		return E_FAIL;
	}

	{//インデックスバッファの中身を埋める
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
	//DebugProc_Print((char *)"接しているブロックはないよ\n");
	return 0;//エラー検知したほうがいいかも
}

float SetGroundHit( int g_num) {


	D3DXVECTOR3 player_botom_position = GetPlayer_Pos();//現在のプレイヤーの位置;
	player_botom_position.y -= 15.0f;
	D3DXVECTOR3 next_player_botom_position = D3DXVECTOR3(player_botom_position.x, 0.0f, player_botom_position.z);


	D3DXVECTOR3 vector1, vector2, vector3;//三角形の三辺をベクトル化


	D3DXVECTOR3 hosen1, hosen2, hosen3;
	for (int i = 0; i < NumIndex; i++)
	{
		if ((ground[g_num].LookIdxNum[i + 2]) > NumIndex || (ground[g_num].LookIdxNum[i + 2]) < 0)
		{
			//縮退ポリゴンのインデックスは飛ばす
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



		//プレイヤーのいるポリゴン面を取得
		
		if (hosen1.y >= 0 && hosen2.y >= 0 || hosen1.y <= 0 && hosen2.y <= 0)
		{
			if (hosen1.y == 0)//頂点が「i」のときのは計算をしなくていいのでそのまま値を返す
			{
				return player_botom_position.y + 15.0f;
			}
			else if (hosen2.y == 0)//頂点が「i」のときのは計算をしなくていいのでそのまま値を返す
			{
				return player_botom_position.y + 15.0f;
			}
			else
			{

				D3DXVec3Cross(&hosen3, &(next_player_botom_position -
					(D3DXVECTOR3(ground[g_num].LookVtx[ground[g_num].LookIdxNum[i + 2]].pos.x, 0.0f, ground[g_num].LookVtx[ground[g_num].LookIdxNum[i + 2]].pos.z))), &vector3);


				if (hosen2.y >= 0 && hosen3.y >= 0 || hosen2.y <= 0 && hosen3.y <= 0)
				{

					if (hosen3.y == 0)//頂点が「i」のときのは計算をしなくていいのでそのまま値を返す
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

						D3DXVECTOR3 n;//法線ベクトル
						D3DXVECTOR3 AB = GetNormalPlayer_Pos();

						//ベクトル再設定
						vector1 = D3DXVECTOR3(ground[g_num].LookVtx[ground[g_num].LookIdxNum[i + 1]].pos - ground[g_num].LookVtx[ground[g_num].LookIdxNum[i]].pos);
						vector3 = D3DXVECTOR3(ground[g_num].LookVtx[ground[g_num].LookIdxNum[i + 2]].pos - ground[g_num].LookVtx[ground[g_num].LookIdxNum[i]].pos);
						//vector3.x *= -1.0f;
						//vector3.y *= -1.0f;
						//vector3.z *= -1.0f;



						//vector3 *=  -1.0f;//マイナスベクトルにする
						D3DXVec3Cross(&n, &vector1, &vector3);//外積で面法線を計算

						//地面より下にいたら地面上に再設置
						if ((D3DXVec3Dot(&naiseki2, &n)) > 0.00 && (D3DXVec3Dot(&naiseki1, &n)) <= 0.00 ||
							(D3DXVec3Dot(&naiseki2, &n)) <= 0.00 && (D3DXVec3Dot(&naiseki1, &n)) > 0.00) {


							player_botom_position.y += AB.y * (fabsf(D3DXVec3Dot(&n, &naiseki1) / (fabsf(D3DXVec3Dot(&n, &naiseki1) + fabsf(D3DXVec3Dot(&n, &naiseki2))))));


							//DebugProc_Print((char *)"インデックス：【%d】座標1：【%f,%f,%f】\n", ground[g_num].LookIdxNum[i], ground[g_num].LookVtx[ground[g_num].LookIdxNum[i]].pos.x, ground[g_num].LookVtx[ground[g_num].LookIdxNum[i]].pos.y, ground[g_num].LookVtx[ground[g_num].LookIdxNum[i]].pos.z);
							//DebugProc_Print((char *)"インデックス：【%d】座標2：【%f,%f,%f】\n", ground[g_num].LookIdxNum[i + 1], ground[g_num].LookVtx[ground[g_num].LookIdxNum[i + 1]].pos.x, ground[g_num].LookVtx[ground[g_num].LookIdxNum[i + 1]].pos.y, ground[g_num].LookVtx[ground[g_num].LookIdxNum[i + 1]].pos.z);
							//DebugProc_Print((char *)"インデックス：【%d】座標3：【%f,%f,%f】\n", ground[g_num].LookIdxNum[i + 2], ground[g_num].LookVtx[ground[g_num].LookIdxNum[i + 2]].pos.x, ground[g_num].LookVtx[ground[g_num].LookIdxNum[i + 2]].pos.y, ground[g_num].LookVtx[ground[g_num].LookIdxNum[i + 2]].pos.z);

							//DebugProc_Print((char *)"\n【%f】\n", player_botom_position.y);
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

//頂点を作る関数を作成する

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