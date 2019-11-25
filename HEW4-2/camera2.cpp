
#include "camera2.h"
#include "myDirect3D.h"
#include "input.h"
#include "debugproc.h" 

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	CAM_POS_V_X		(0.0f)					// �J�����̎��_�����ʒu(X���W)
#define	CAM_POS_V_Y		(100.0f)				// �J�����̎��_�����ʒu(Y���W)
#define	CAM_POS_V_Z		(-200.0f)				// �J�����̎��_�����ʒu(Z���W)
#define	CAM_POS_R_X		(0.0f)					// �J�����̒����_�����ʒu(X���W)
#define	CAM_POS_R_Y		(0.0f)					// �J�����̒����_�����ʒu(Y���W)
#define	CAM_POS_R_Z		(0.0f)					// �J�����̒����_�����ʒu(Z���W)
#define	VIEW_ANGLE		(D3DXToRadian(45.0f))							// �r���[���ʂ̎���p
#define	VIEW_ASPECT		((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// �r���[���ʂ̃A�X�y�N�g��
#define	VIEW_NEAR_Z		(10.0f)											// �r���[���ʂ�NearZ�l
#define	VIEW_FAR_Z		(1000.0f)										// �r���[���ʂ�FarZ�l

#define VALUE_MOVE_Camera2 1.0
#define	VALUE_ROTATE_Camera2	(D3DX_PI * 0.02f)				// ��]��

//*****************************************************************************
// �J�����̍\����
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 posV;			// ���_
	D3DXVECTOR3 posR;			// �����_
	D3DXVECTOR3 vecU;			// ������x�N�g��
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxProjection;	// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX mtxView;			// �r���[�}�g���b�N�X
	float fDstance;

} Camera2;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
Camera2 g_Camera2;		// �J�������

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
float fVecX, fVecZ;

//=============================================================================
// �J�����̏���������
//=============================================================================
void Camera2_Initialize(void)
{
	g_Camera2.posV = D3DXVECTOR3(CAM_POS_V_X, CAM_POS_V_Y, CAM_POS_V_Z);
	g_Camera2.posR = D3DXVECTOR3(CAM_POS_R_X, CAM_POS_R_X, CAM_POS_R_X);
	g_Camera2.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	g_Camera2.fDstance = 20.0f;

	fVecX = g_Camera2.posV.x - g_Camera2.posR.x;
	fVecZ = g_Camera2.posV.z - g_Camera2.posR.z;
	g_Camera2.fDstance = sqrtf(fVecX * fVecX + fVecZ * fVecZ);
}

//=============================================================================
// �J�����̏I������
//=============================================================================
void Camera2_Finalize(void)
{
}

//=============================================================================
// �J�����̍X�V����
//=============================================================================
void Camera2_Update(void)
{

	if (Keyboard_IsPress(DIK_A))
	{
		if (Keyboard_IsPress(DIK_W))
		{// �����ړ�
			g_Camera2.posV.x += sinf(-D3DX_PI * 0.75f - g_Camera2.rot.y) * VALUE_MOVE_Camera2;
			g_Camera2.posV.z -= cosf(-D3DX_PI * 0.75f - g_Camera2.rot.y) * VALUE_MOVE_Camera2;
		}
		else if (Keyboard_IsPress(DIK_S))
		{// ����O�ړ�
			g_Camera2.posV.x += sinf(-D3DX_PI * 0.25f - g_Camera2.rot.y) * VALUE_MOVE_Camera2;
			g_Camera2.posV.z -= cosf(-D3DX_PI * 0.25f - g_Camera2.rot.y) * VALUE_MOVE_Camera2;
		}
		else
		{// ���ړ�
			g_Camera2.posV.x += sinf(-D3DX_PI * 0.50f - g_Camera2.rot.y) * VALUE_MOVE_Camera2;
			g_Camera2.posV.z -= cosf(-D3DX_PI * 0.50f - g_Camera2.rot.y) * VALUE_MOVE_Camera2;
		}

		g_Camera2.posR.z = g_Camera2.posV.z + cosf(g_Camera2.rot.y) * g_Camera2.fDstance;
		g_Camera2.posR.x = g_Camera2.posV.x + sinf(g_Camera2.rot.y) * g_Camera2.fDstance;
	}
	else if (Keyboard_IsPress(DIK_D))
	{
		if (Keyboard_IsPress(DIK_W))
		{// �����ړ�
			g_Camera2.posV.x += sinf(D3DX_PI * 0.75f - g_Camera2.rot.y) * VALUE_MOVE_Camera2;
			g_Camera2.posV.z -= cosf(D3DX_PI * 0.75f - g_Camera2.rot.y) * VALUE_MOVE_Camera2;
		}
		else if (Keyboard_IsPress(DIK_S))
		{// ����O�ړ�
			g_Camera2.posV.x += sinf(D3DX_PI * 0.25f - g_Camera2.rot.y) * VALUE_MOVE_Camera2;
			g_Camera2.posV.z -= cosf(D3DX_PI * 0.25f - g_Camera2.rot.y) * VALUE_MOVE_Camera2;
		}
		else
		{// ���ړ�
			g_Camera2.posV.x += sinf(D3DX_PI * 0.50f - g_Camera2.rot.y) * VALUE_MOVE_Camera2;
			g_Camera2.posV.z -= cosf(D3DX_PI * 0.50f - g_Camera2.rot.y) * VALUE_MOVE_Camera2;
		}

		g_Camera2.posR.x = g_Camera2.posV.x + sinf(g_Camera2.rot.y) * g_Camera2.fDstance;
		g_Camera2.posR.z = g_Camera2.posV.z + cosf(g_Camera2.rot.y) * g_Camera2.fDstance;
	}
	else if (Keyboard_IsPress(DIK_W))
	{
	g_Camera2.posV.x += sinf(D3DX_PI * 1.0f - g_Camera2.rot.y) * VALUE_MOVE_Camera2;
	g_Camera2.posV.z -= cosf(D3DX_PI * 1.0f - g_Camera2.rot.y) * VALUE_MOVE_Camera2;

	g_Camera2.posR.x = g_Camera2.posV.x + sinf(g_Camera2.rot.y) * g_Camera2.fDstance;
	g_Camera2.posR.z = g_Camera2.posV.z + cosf(g_Camera2.rot.y) * g_Camera2.fDstance;
	}

	else if (Keyboard_IsPress(DIK_S))
	{
	g_Camera2.posV.x += sinf(D3DX_PI * 0.0f - g_Camera2.rot.y) * VALUE_MOVE_Camera2;
	g_Camera2.posV.z -= cosf(D3DX_PI * 0.0f - g_Camera2.rot.y) * VALUE_MOVE_Camera2;

	g_Camera2.posR.x = g_Camera2.posV.x + sinf(g_Camera2.rot.y) * g_Camera2.fDstance;
	g_Camera2.posR.z = g_Camera2.posV.z + cosf(g_Camera2.rot.y) * g_Camera2.fDstance;

	}

	if (Keyboard_IsPress(DIK_Z))
	{// Y������]
	g_Camera2.rot.y += VALUE_MOVE_Camera2/10;
	if (g_Camera2.rot.y > D3DX_PI)
	{
	g_Camera2.rot.y -= D3DX_PI * 2.0f;
	}

	g_Camera2.posV.x = g_Camera2.posR.x - sinf(g_Camera2.rot.y) * g_Camera2.fDstance;
	g_Camera2.posV.z = g_Camera2.posR.z - cosf(g_Camera2.rot.y) * g_Camera2.fDstance;

	}

	else if (Keyboard_IsPress(DIK_C))
	{// Y������]
	g_Camera2.rot.y -= VALUE_MOVE_Camera2/10;
	if (g_Camera2.rot.y < D3DX_PI)
	{
	g_Camera2.rot.y += D3DX_PI * 2.0f;
	}

	g_Camera2.posV.x = g_Camera2.posR.x - sinf(g_Camera2.rot.y) * g_Camera2.fDstance;
	g_Camera2.posV.z = g_Camera2.posR.z - cosf(g_Camera2.rot.y) * g_Camera2.fDstance;

	}

	if (Keyboard_IsPress(DIK_Y))
	{
	g_Camera2.posV.y += VALUE_MOVE_Camera2;
	}

	if (Keyboard_IsPress(DIK_N))
	{
	g_Camera2.posV.y -= VALUE_MOVE_Camera2;
	}

	if (Keyboard_IsPress(DIK_Q))
	{// X���E��]
	g_Camera2.rot.y -= VALUE_MOVE_Camera2/10;

	if (g_Camera2.rot.y < -D3DX_PI)
	{
	g_Camera2.rot.y += D3DX_PI * 2.0f;
	}

	g_Camera2.posR.x = g_Camera2.posV.x + sinf(g_Camera2.rot.y) * g_Camera2.fDstance;
	g_Camera2.posR.z = g_Camera2.posV.z + cosf(g_Camera2.rot.y) * g_Camera2.fDstance;
	}

	if (Keyboard_IsPress(DIK_E))
	{// X���E��]
	g_Camera2.rot.y += VALUE_MOVE_Camera2/10;

	if (g_Camera2.rot.y > D3DX_PI)
	{
	g_Camera2.rot.y -= D3DX_PI * 2.0f;
	}

	g_Camera2.posR.x = g_Camera2.posV.x + sinf(g_Camera2.rot.y) * g_Camera2.fDstance;
	g_Camera2.posR.z = g_Camera2.posV.z + cosf(g_Camera2.rot.y) * g_Camera2.fDstance;
	}

	if (Keyboard_IsPress(DIK_T))
	{// X���E��]
	g_Camera2.posR.y += VALUE_MOVE_Camera2;
	}

	if (Keyboard_IsPress(DIK_B))
	{// X���E��]
	g_Camera2.posR.y -= VALUE_MOVE_Camera2;
	}

	if (Keyboard_IsPress(DIK_U))
	{
		g_Camera2.posV.y -= VALUE_MOVE_Camera2;
	
		g_Camera2.posV.z += VALUE_MOVE_Camera2;

		g_Camera2.posR.z -= VALUE_MOVE_Camera2;
	}

	if (Keyboard_IsPress(DIK_M))
	{
		g_Camera2.posV.y += VALUE_MOVE_Camera2;
	
		g_Camera2.posV.z -= VALUE_MOVE_Camera2;

		g_Camera2.posR.z += VALUE_MOVE_Camera2;
	}

	if (Keyboard_IsPress(DIK_SPACE))
	{
	g_Camera2.posV = D3DXVECTOR3(CAM_POS_V_X, CAM_POS_V_Y, CAM_POS_V_Z);
	g_Camera2.posR = D3DXVECTOR3(CAM_POS_R_X, CAM_POS_R_Y, CAM_POS_R_Z);
	g_Camera2.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	fVecX = g_Camera2.posV.x - g_Camera2.posR.x;
	fVecZ = g_Camera2.posV.z - g_Camera2.posR.z;
	g_Camera2.fDstance = sqrtf(fVecX * fVecX + fVecZ * fVecZ);

	}



	/*
	PrintDebugProc((char *)"*** �R�c�|���S������ ***\n");
	PrintDebugProc((char *)"�ʒu [%f : %f : %f]\n", g_posField.x, g_posField.y, g_posField.z);
	PrintDebugProc((char *)"�O�ړ� : �v\n");
	PrintDebugProc((char *)"��ړ� : �r\n");
	*/

	DebugProc_Print((char *)"*** �J�������� ***\n");
	DebugProc_Print((char *)"�ʒuR [%f : %f : %f]\n", g_Camera2.posR.x, g_Camera2.posR.y, g_Camera2.posR.z);
	DebugProc_Print((char *)"�ʒuV [%f : %f : %f]\n", g_Camera2.posV.x, g_Camera2.posV.y, g_Camera2.posV.z);

}

//=============================================================================
// �J�����̐ݒ菈��
//=============================================================================
void Camera2_SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_Camera2.mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&g_Camera2.mtxProjection,
		VIEW_ANGLE,				// �r���[���ʂ̎���p
		VIEW_ASPECT,			// �r���[���ʂ̃A�X�y�N�g��
		VIEW_NEAR_Z,			// �r���[���ʂ�NearZ�l
		VIEW_FAR_Z);			// �r���[���ʂ�FarZ�l

								// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_Camera2.mtxProjection);






	D3DXMATRIX mtxProjection;

	D3DXMatrixIdentity(&mtxProjection);


	float w = 1 / tanf(VIEW_ANGLE*VIEW_ASPECT*0.5);
	float h = 1 / tanf(VIEW_ANGLE*0.5);
	float Q = VIEW_FAR_Z / (VIEW_FAR_Z - VIEW_NEAR_Z);

	mtxProjection(0, 0) = w;
	mtxProjection(1, 1) = h;
	mtxProjection(2, 2) = Q;
	mtxProjection(2, 3) = 1.0f;
	mtxProjection(3, 2) = Q * VIEW_NEAR_Z * -1;



	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_Camera2.mtxView);

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&g_Camera2.mtxView,
		&g_Camera2.posV,		// �J�����̎��_
		&g_Camera2.posR,		// �J�����̒����_
		&g_Camera2.vecU);	// �J�����̏�����x�N�g��

	D3DXMATRIX mtxView;
	D3DXMatrixIdentity(&mtxView);


	D3DXVECTOR3 axisZ;

	axisZ = g_Camera2.posR - g_Camera2.posV;

	D3DXVec3Normalize(&axisZ, &axisZ);


	D3DXVECTOR3 axisX;

	D3DXVec3Cross(&axisX, &g_Camera2.vecU, &axisZ);


	D3DXVECTOR3 axisY;

	D3DXVec3Cross(&axisY, &axisZ, &axisY);



	mtxView(0, 0) = axisX.x;
	mtxView(1, 0) = axisX.y;
	mtxView(2, 0) = axisX.z;
	mtxView(0, 1) = axisY.x;
	mtxView(1, 1) = axisY.y;
	mtxView(2, 1) = axisY.z;
	mtxView(0, 2) = axisZ.x;
	mtxView(1, 2) = axisZ.y;
	mtxView(2, 2) = axisZ.z;

	mtxView(3, 0) = D3DXVec3Dot(&g_Camera2.posV, &axisX)*-1;
	mtxView(3, 1) = D3DXVec3Dot(&g_Camera2.posV, &axisY)*-1;
	mtxView(3, 2) = D3DXVec3Dot(&g_Camera2.posV, &axisZ)*-1;

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &g_Camera2.mtxView);
}

