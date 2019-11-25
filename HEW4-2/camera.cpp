
#include "camera.h"
#include "myDirect3D.h"
#include "player.h"
#include "input.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	CAM_POS_V_X		(0.0f)					// �J�����̎��_�����ʒu(X���W)
#define	CAM_POS_V_Y		(50.0f)				// �J�����̎��_�����ʒu(Y���W)
#define	CAM_POS_V_Z		(-150.0f)				// �J�����̎��_�����ʒu(Z���W)
#define	CAM_POS_R_X		(20.0f)					// �J�����̒����_�����ʒu(X���W)
#define	CAM_POS_R_Y		(0.0f)					// �J�����̒����_�����ʒu(Y���W)
#define	CAM_POS_R_Z		(0.0f)					// �J�����̒����_�����ʒu(Z���W)
#define	VIEW_ANGLE		(D3DXToRadian(45.0f))							// �r���[���ʂ̎���p
#define	VIEW_ASPECT		((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// �r���[���ʂ̃A�X�y�N�g��
#define	VIEW_NEAR_Z		(10.0f)											// �r���[���ʂ�NearZ�l
#define	VIEW_FAR_Z		(2000.0f)										// �r���[���ʂ�FarZ�l

//*****************************************************************************
// �J�����̍\����
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 posV;			// ���_
	D3DXVECTOR3 posR;			// �����_
	D3DXVECTOR3 vecU;			// ������x�N�g��
	D3DXMATRIX mtxProjection;	// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX mtxView;			// �r���[�}�g���b�N�X
} CAMERA;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
CAMERA g_camera;		// �J�������

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//=============================================================================
// �J�����̏���������
//=============================================================================
void Camera_Initialize(void)
{
	g_camera.posV = D3DXVECTOR3(CAM_POS_V_X, CAM_POS_V_Y, CAM_POS_V_Z);
	g_camera.posR = D3DXVECTOR3(CAM_POS_R_X, CAM_POS_R_X, CAM_POS_R_X);
	g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
}

//=============================================================================
// �J�����̏I������
//=============================================================================
void Camera_Finalize(void)
{
}

//=============================================================================
// �J�����̍X�V����
//=============================================================================
void Camera_Update(void)
{
	g_camera.posV.x = GetPlayer_Pos().x + CAM_POS_V_Z * sinf(GetPlayer_Rot().y);
	//g_camera.posV.y += 0.3f;
	g_camera.posV.z = GetPlayer_Pos().z + CAM_POS_V_Z * cosf(GetPlayer_Rot().y);

	g_camera.posR.x = GetPlayer_Pos().x;
	//g_camera.posR.y += 10.0f;
	g_camera.posR.z = GetPlayer_Pos().z;

	//g_camera.vecU.x += 0.01f;
	//g_camera.vecU.y += 0.01f;
	//g_camera.vecU.z += 0.01f;
}

//=============================================================================
// �J�����̐ݒ菈��
//=============================================================================
void Camera_SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice(); 

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera.mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&g_camera.mtxProjection,
								VIEW_ANGLE,				// �r���[���ʂ̎���p
								VIEW_ASPECT,			// �r���[���ʂ̃A�X�y�N�g��
								VIEW_NEAR_Z,			// �r���[���ʂ�NearZ�l
								VIEW_FAR_Z);			// �r���[���ʂ�FarZ�l

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera.mtxView);

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&g_camera.mtxView, 
						&g_camera.posV,		// �J�����̎��_
						&g_camera.posR,		// �J�����̒����_
						&g_camera.vecU);	// �J�����̏�����x�N�g��

	D3DXMATRIX mtxProjection;
	//�P�ʍs��ɂ���
	D3DXMatrixIdentity(&mtxProjection);
	
	float w = 1 / tanf(VIEW_ANGLE*VIEW_ASPECT*0.5);

	float h = 1 / tanf(VIEW_ANGLE*0.5);

	float Q = VIEW_FAR_Z / (VIEW_FAR_Z - VIEW_NEAR_Z);

	mtxProjection(0, 0) = w;
	mtxProjection(1, 1) = h;
	mtxProjection(2, 2) = Q; 
	mtxProjection(2, 3) = 1.0f;
	mtxProjection(3, 2) = Q * VIEW_NEAR_Z*-1;;

	D3DXMATRIX mtxView;
	//�P�ʍs��ɂ���
	D3DXMatrixIdentity(&mtxView);

	D3DXVECTOR3 axisZ;
	//at - pos��z�������
	axisZ = g_camera.posR - g_camera.posV;
	//�P�ʃx�N�g���Ƃ���
	D3DXVec3Normalize(&axisZ, &axisZ);

	D3DXVECTOR3 axisX;
	//up��z���ŊO�ς��Ƃ�@���@x�����ł���
	D3DXVec3Cross(&axisX, &g_camera.vecU, &axisZ);

	D3DXVECTOR3 axisY;
	//z����x���ŊO�ς����Ƃ�@���@x�����ł���
	D3DXVec3Cross(&axisY, &axisZ, &axisX);

	mtxView(0, 0) = axisX.x;
	mtxView(1, 0) = axisX.y;
	mtxView(2, 0) = axisX.z;

	mtxView(0, 1) = axisY.x;
	mtxView(1, 1) = axisY.y;
	mtxView(2, 1) = axisY.z;

	mtxView(0, 2) = axisZ.x;
	mtxView(1, 2) = axisZ.y;
	mtxView(2, 2) = axisZ.z;

	mtxView(3, 0) = D3DXVec3Dot(&g_camera.posV, &axisX)*-1;
	mtxView(3, 1) = D3DXVec3Dot(&g_camera.posV, &axisY)*-1;
	mtxView(3, 2) = D3DXVec3Dot(&g_camera.posV, &axisZ)*-1;

	// �r���[�}�g���b�N�X�̐ݒ�
	//pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);
	pDevice->SetTransform(D3DTS_VIEW, &mtxView); //������������J����

}

