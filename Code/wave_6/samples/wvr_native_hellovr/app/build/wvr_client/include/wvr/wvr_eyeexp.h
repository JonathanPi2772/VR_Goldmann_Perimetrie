// "WaveVR SDK 
// © 2017 HTC Corporation. All Rights Reserved.
//
// Unless otherwise required by copyright law and practice,
// upon the execution of HTC SDK license agreement,
// HTC grants you access to and use of the WaveVR SDK(s).
// You shall fully comply with all of HTC’s SDK license agreement terms and
// conditions signed by you and all SDK and API requirements,
// specifications, and documentation provided by HTC to You."

#ifndef wvr_eyeexp_h_
#define wvr_eyeexp_h_

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
    WVR_EYEEXPRESSION_LEFT_BLINK      = 0,
    WVR_EYEEXPRESSION_LEFT_WIDE       = 1,
    WVR_EYEEXPRESSION_RIGHT_BLINK     = 2,
    WVR_EYEEXPRESSION_RIGHT_WIDE      = 3,
    WVR_EYEEXPRESSION_LEFT_SQUEEZE    = 4,
    WVR_EYEEXPRESSION_RIGHT_SQUEEZE   = 5,
    WVR_EYEEXPRESSION_LEFT_DOWN       = 6,
    WVR_EYEEXPRESSION_RIGHT_DOWN      = 7,
    WVR_EYEEXPRESSION_LEFT_OUT        = 8,
    WVR_EYEEXPRESSION_RIGHT_IN        = 9,
    WVR_EYEEXPRESSION_LEFT_IN         = 10,
    WVR_EYEEXPRESSION_RIGHT_OUT       = 11,
    WVR_EYEEXPRESSION_LEFT_UP         = 12,
    WVR_EYEEXPRESSION_RIGHT_UP        = 13,
    WVR_EYEEXPRESSION_MAX
} WVR_EyeExpression;

/**
 * @brief the eye expression data
 */
typedef struct WVR_EyeExp {
    float               weights[WVR_EYEEXPRESSION_MAX];
    int64_t             timestamp;
} WVR_EyeExp_t;

/**
 * @brief Use this function to start eye expression feature.
 *
 * Use this API to start eye expression module
 * This API must be called by main thread.
 *
 * @retval WVR_Success start eye expression feature successfully.
 * @retval others @ref WVR_Result mean failure.
 * @version API Level 10
*/
extern WVR_EXPORT WVR_Result WVR_StartEyeExp();

/**
 * @brief Use this function to stop the eye expression that was started.
 *
 * Use this API to stop eye expression that you already started,
 * and release related eye expression resources.
 * This API must be called by the main thread.
 * @version API Level 10
*/
extern WVR_EXPORT void WVR_StopEyeExp();

/**
 * @brief Use this function to get the eye expression data.
 *
 * @param data The eye expression data @ref WVR_EyeExp
 * @retval WVR_Success Successfully retrieved data.
 * @retval Others @ref WVR_Result mean failure.
 * @version API Level 10
*/
extern WVR_EXPORT WVR_Result WVR_GetEyeExpData(WVR_EyeExp_t *data);

#ifdef __cplusplus
} /* extern "C" */
#endif
#include "close_code.h"

#endif /* wvr_eyeexp_h_ */
