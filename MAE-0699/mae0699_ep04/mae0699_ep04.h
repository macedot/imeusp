#ifndef MAE0699_EP04_H
#define MAE0699_EP04_H

/******************************************************************************
 * Includes da aplicacao;
 ******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#ifdef __cplusplus
}
#endif

/******************************************************************************
 * Constantes da aplicacao
 ******************************************************************************/
#define NUM_PROC           (4)
#define NUM_PROC_PREF      (1)

#define TEMPO_SIMULACAO     (480)

/******************************************************************************
 * Includes das Classes da Aplicacao
 ******************************************************************************/
#include "mcTypeDef.h"
#include "CTime.h"
#include "CRand.h"
#include "CElem.h"
#include "CFila.h"
#include "CProc.h"

#endif // MAE0699_EP04_H