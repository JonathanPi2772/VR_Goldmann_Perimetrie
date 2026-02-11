// "WaveVR SDK
// © 2017 HTC Corporation. All Rights Reserved.
//
// Unless otherwise required by copyright law and practice,
// upon the execution of HTC SDK license agreement,
// HTC grants you access to and use of the WaveVR SDK(s).
// You shall fully comply with all of HTC’s SDK license agreement terms and
// conditions signed by you and all SDK and API requirements,
// specifications, and documentation provided by HTC to You."


/**
 *  \file wvr_compatibility.h
 *
 *  Main include header for the WVR debug library
 */


#ifndef wvr_compatibility_h_
#define wvr_compatibility_h_

#include "begin_code.h"
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Function to set if a compatibility checker enable
 *
 * This API can work after invoking WVR_Init
 */
extern WVR_EXPORT bool WVR_SetChecker(bool enable);

#ifdef __cplusplus
} /* extern "C" */
#endif
#include "close_code.h"

#endif /* wvr_compatibility_h_ */
