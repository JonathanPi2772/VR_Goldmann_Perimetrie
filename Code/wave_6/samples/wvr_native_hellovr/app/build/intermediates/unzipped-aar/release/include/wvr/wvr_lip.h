// "WaveVR SDK
// © 2017 HTC Corporation. All Rights Reserved.
//
// Unless otherwise required by copyright law and practice,
// upon the execution of HTC SDK license agreement,
// HTC grants you access to and use of the WaveVR SDK(s).
// You shall fully comply with all of HTC’s SDK license agreement terms and
// conditions signed by you and all SDK and API requirements,
// specifications, and documentation provided by HTC to You."

#ifndef wvr_lip_h_
#define wvr_lip_h_

#include "wvr_stdinc.h"
#include "wvr_types.h"

#include "begin_code.h"
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief The expression type of lower face
 */
typedef enum {
    WVR_LipExpression_Jaw_Right              = 0,
    WVR_LipExpression_Jaw_Left               = 1,
    WVR_LipExpression_Jaw_Forward            = 2,
    WVR_LipExpression_Jaw_Open               = 3,
    WVR_LipExpression_Mouth_Ape_Shape        = 4,
    WVR_LipExpression_Mouth_Upper_Right      = 5,
    WVR_LipExpression_Mouth_Upper_Left       = 6,
    WVR_LipExpression_Mouth_Lower_Right      = 7,
    WVR_LipExpression_Mouth_Lower_Left       = 8,
    WVR_LipExpression_Mouth_Upper_Overturn   = 9,
    WVR_LipExpression_Mouth_Lower_Overturn   = 10,
    WVR_LipExpression_Mouth_Pout             = 11,
    WVR_LipExpression_Mouth_Raiser_Right     = 12,
    WVR_LipExpression_Mouth_Raiser_Left      = 13,
    WVR_LipExpression_Mouth_Stretcher_Right  = 14,
    WVR_LipExpression_Mouth_Stretcher_Left   = 15,
    WVR_LipExpression_Cheek_Puff_Right       = 16,
    WVR_LipExpression_Cheek_Puff_Left        = 17,
    WVR_LipExpression_Cheek_Suck             = 18,
    WVR_LipExpression_Mouth_Upper_Upright    = 19,
    WVR_LipExpression_Mouth_Upper_Upleft     = 20,
    WVR_LipExpression_Mouth_Lower_Downright  = 21,
    WVR_LipExpression_Mouth_Lower_Downleft   = 22,
    WVR_LipExpression_Mouth_Upper_Inside     = 23,
    WVR_LipExpression_Mouth_Lower_Inside     = 24,
    WVR_LipExpression_Mouth_Lower_Overlay    = 25,
    WVR_LipExpression_Tongue_Longstep1       = 26,
    WVR_LipExpression_Tongue_Left            = 27,
    WVR_LipExpression_Tongue_Right           = 28,
    WVR_LipExpression_Tongue_Up              = 29,
    WVR_LipExpression_Tongue_Down            = 30,
    WVR_LipExpression_Tongue_Roll            = 31,
    WVR_LipExpression_Tongue_Longstep2       = 32,
    WVR_LipExpression_Tongue_Upright_Morph   = 33,
    WVR_LipExpression_Tongue_Upleft_Morph    = 34,
    WVR_LipExpression_Tongue_Downright_Morph = 35,
    WVR_LipExpression_Tongue_Downleft_Morph  = 36,
    WVR_LipExpression_Max
} WVR_LipExpression;

/**
 * @brief Use this function to start lip expression feature.
 *
 * Use this API to start lip expression module
 * This API must be called by main thread.
 *
 * @retval WVR_Success start lip expression feature successfully.
 * @retval others @ref WVR_Result mean failure.
 * @version API Level 9
*/
extern WVR_EXPORT WVR_Result WVR_StartLipExp();

/**
 * @brief Use this function to stop the lip expression that was started.
 *
 * Use this API to stop lip expression that you already started,
 * and release related lip expression resources.
 * This API must be called by the main thread.
 * @version API Level 9
*/
extern WVR_EXPORT void WVR_StopLipExp();

/**
 * @brief Use this function to get the lip expression data.
 *
 * @param value The lip expression data @ref WVR_LipExpression
 * @retval WVR_Success Successfully retrieved data.
 * @retval Others @ref WVR_Result mean failure.
 * @version API Level 9
*/
extern WVR_EXPORT WVR_Result WVR_GetLipExpData(float *value);

#ifdef __cplusplus
} /* extern "C" */
#endif
#include "close_code.h"

#endif /* wvr_lip_h_ */