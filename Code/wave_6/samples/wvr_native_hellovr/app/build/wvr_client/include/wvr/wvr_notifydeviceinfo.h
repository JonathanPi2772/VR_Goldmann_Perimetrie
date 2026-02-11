// "WaveVR SDK 
// © 2017 HTC Corporation. All Rights Reserved.
//
// Unless otherwise required by copyright law and practice,
// upon the execution of HTC SDK license agreement,
// HTC grants you access to and use of the WaveVR SDK(s).
// You shall fully comply with all of HTC’s SDK license agreement terms and
// conditions signed by you and all SDK and API requirements,
// specifications, and documentation provided by HTC to You."


#ifndef wvr_notifydeviceinfo_h_
#define wvr_notifydeviceinfo_h_

#include "wvr_types.h"
#include "wvr_stdinc.h"

#include "begin_code.h"
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Function to start notification device information.
 *
 * This API will start notifydeviceinfo module
 * This API must be called by main thread.
 * @param type Indicates what device type. (refer to @ref WVR_DeviceType)
 * @param unBufferSize The size of the information data to device service. The maximum size is 512.
 * @retval WVR_Success start notifydeviceinfo feature successfully.
 * @retval others @ref WVR_Result mean failure.
 * @version API Level 9
*/
extern WVR_EXPORT WVR_Result WVR_StartNotifyDeviceInfo(WVR_DeviceType type, uint32_t unBufferSize);

/**
 * @brief Function to stop notifydeviceinfo that you already started..
 *
 * This API will stop notifydeviceinfo that you already started,
 * and release related source.
 * This API must be called by main thread.
 * @param type Indicates what device type. (refer to @ref WVR_DeviceType)
 * @version API Level 9
*/
extern WVR_EXPORT void WVR_StopNotifyDeviceInfo(WVR_DeviceType type);

/**
 * @brief Function to update a parameter string to the device service.
 *
 * @param type Indicates what device type. (refer to @ref WVR_DeviceType)
 * @param dataValue Send the string to let the app communicate with the device service.
 * @version API Level 9
*/
extern WVR_EXPORT void WVR_UpdateNotifyDeviceInfo(WVR_DeviceType type, const char *dataValue);

#ifdef __cplusplus
} /* extern "C" */
#endif
#include "close_code.h"

#endif /* wvr_notifydeviceinfo_h_ */

