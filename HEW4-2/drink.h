#pragma once

#include "main.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT Drink_Initialize(void);
void Drink_Finalize(void);
void Drink_Update(void);
void Drink_Draw(void);

bool Get_DrinkHit();