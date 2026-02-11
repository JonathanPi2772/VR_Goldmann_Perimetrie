// "WaveVR SDK 
// © 2021 HTC Corporation. All Rights Reserved.
//
// Unless otherwise required by copyright law and practice,
// upon the execution of HTC SDK license agreement,
// HTC grants you access to and use of the WaveVR SDK(s).
// You shall fully comply with all of HTC’s SDK license agreement terms and
// conditions signed by you and all SDK and API requirements,
// specifications, and documentation provided by HTC to You."


#ifndef wvr_tracker_h_
#define wvr_tracker_h_

#include "wvr_types.h"
#include "wvr_stdinc.h"

#include "begin_code.h"
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief The tracker role
 * Describes the role of the tracker device.
 * API Level 8 : 0 ~ 3, API Level 12 : 32~63, API Level 13 : 32~72
 * @version API Level 12
 */
typedef enum {
    WVR_TrackerRole_Undefined             =  0,
    WVR_TrackerRole_Standalone            =  1,
    WVR_TrackerRole_Pair1_Right           =  2,
    WVR_TrackerRole_Pair1_Left            =  3,
    WVR_TrackerRole_Shoulder_Right        = 32,
    WVR_TrackerRole_Upper_Arm_Right       = 33,
    WVR_TrackerRole_Elbow_Right           = 34,
    WVR_TrackerRole_Forearm_Right         = 35,
    WVR_TrackerRole_Wrist_Right           = 36,
    WVR_TrackerRole_Hand_Right            = 37,
    WVR_TrackerRole_Thigh_Right           = 38,
    WVR_TrackerRole_Knee_Right            = 39,
    WVR_TrackerRole_Calf_Right            = 40,
    WVR_TrackerRole_Ankle_Right           = 41,
    WVR_TrackerRole_Foot_Right            = 42,
    WVR_TrackerRole_Shoulder_Left         = 47,
    WVR_TrackerRole_Upper_Arm_Left        = 48,
    WVR_TrackerRole_Elbow_Left            = 49,
    WVR_TrackerRole_Forearm_Left          = 50,
    WVR_TrackerRole_Wrist_Left            = 51,
    WVR_TrackerRole_Hand_Left             = 52,
    WVR_TrackerRole_Thigh_Left            = 53,
    WVR_TrackerRole_Knee_Left             = 54,
    WVR_TrackerRole_Calf_Left             = 55,
    WVR_TrackerRole_Ankle_Left            = 56,
    WVR_TrackerRole_Foot_Left             = 57,
    WVR_TrackerRole_Chest                 = 62,
    WVR_TrackerRole_Waist                 = 63,
    WVR_TrackerRole_Camera                = 71,
    WVR_TrackerRole_Keyboard              = 72
} WVR_TrackerRole;

/**
 * @brief The capabilities of the tracker device.
 * @version API Level 8
 */
typedef struct WVR_TrackerCapabilities {
    bool supportsOrientationTracking;
    bool supportsPositionTracking;
    bool supportsInputDevice;
    bool supportsHapticVibration;
    bool supportsBatteryLevel;
    bool supportsExtendedData;
} WVR_TrackerCapabilities_t;

/**
 * @brief The callback function template of tracker info notify.
 *
 * @param rtrackerId Indicates what id of the tracker device. (Refer to @ref WVR_TrackerId.)
 * @param cbInfo tracker info content.
 * @param timestamp timestamp for tracker info content.
 * @version API Level 9
 */
typedef void (*WVR_TrackerInfoCallback)(WVR_TrackerId trackerId, const char *cbInfo, uint64_t *timestamp);

/**
 * @brief The tracker info notify (includes a callback function which needs to be implemented by the developer).
 * @version API Level 9
 */
typedef struct WVR_TrackerInfoNotify {
    WVR_TrackerInfoCallback callback;
} WVR_TrackerInfoNotify_t;

/**
 * @brief Function to register an info notify of tracker.
 *
 * @param notify which contains an info callback function. This callback function will be called if a tracker info notify has been triggered by tracker device. (Refer to @ref WVR_TrackerInfoNotify.)
 * @retval WVR_Success start the tracker feature successfully.
 * @retval others means failure. (Refer to @ref WVR_Result.)
 * @version API Level 9
 */
extern WVR_EXPORT WVR_Result WVR_RegisterTrackerInfoCallback(WVR_TrackerInfoNotify_t *notify);

/**
 * @brief Function to unregister an info notify of tracker.
 *
 * @retval WVR_Success start the tracker feature successfully.
 * @retval others means failure. (Refer to @ref WVR_Result.)
 * @version API Level 9
 */
extern WVR_EXPORT WVR_Result WVR_UnregisterTrackerInfoCallback();

/**
 * @brief Function to start the feature of the tracker.
 *
 * This API will start the feature of the tracker.
 * This API must be called by main thread.
 *
 * @retval WVR_Success start the tracker feature successfully.
 * @retval others means failure. (Refer to @ref WVR_Result.)
 * @version API Level 8
*/
extern WVR_EXPORT WVR_Result WVR_StartTracker();

/**
 * @brief Function to stop the feature of the tracker.
 *
 * This API will stop the feature of the tracker that already started, and release related sources.
 * This API must be called by main thread.
 * @version API Level 8
*/
extern WVR_EXPORT void WVR_StopTracker();

/**
 * @brief Function to know whether the tracker device is connected or not.
 *
 * @param trackerId Indicates what id of the tracker device. (Refer to @ref WVR_TrackerId.)
 * @return True means connected; false means disconnected.
 * @version API Level 8
 */
extern WVR_EXPORT bool WVR_IsTrackerConnected(WVR_TrackerId trackerId);

/**
 * @brief Function to get the tracker role of the tracker device.
 *
 * @param trackerId Indicates what id of the tracker device. (Refer to @ref WVR_TrackerId.)
 * @return The tracker role. (Refer to @ref WVR_TrackerRole.)
 * @version API Level 8
*/
extern WVR_EXPORT WVR_TrackerRole WVR_GetTrackerRole(WVR_TrackerId trackerId);

/**
 * @brief Function to get the capability of the tracker device.
 *
 * @param trackerId Indicates what id of the tracker device. (Refer to @ref WVR_TrackerId.)
 * @param info The capabilities of the tracker. (Refer to @ref WVR_TrackerCapabilities.)
 * @retval WVR_Success Get the capabilities of the tracker successfully.
 * @retval Others means failure. (Refer to @ref WVR_Result.)
 * @version API Level 8
*/
extern WVR_EXPORT WVR_Result WVR_GetTrackerCapabilities(WVR_TrackerId trackerId, WVR_TrackerCapabilities_t* capabilities);

/**
 * @brief Function to get the pose of the tracker device with or without prediction
 *
 * Recommend calling this get tracker pose API just after @ref WVR_GetSyncPose to make sure tracker model moving smooth.
 *
 * @param trackerId Indicates what id of the tracker device. (Refer to @ref WVR_TrackerId.)
 * @param originModel Specify the tracking universe of the origin tracking model. (Refer to @ref WVR_PoseOriginModel.)
 * @param predictedMilliSec Number of milliseconds from now to predict poses for. Positive numbers are in the future.
 * Pass 0 to get the state at the instant the function is called.
 * @param poseState Obtain the pose data. (Refer to @ref WVR_PoseState.)
 * @retval WVR_Success Get the pose of the tracker successfully.
 * @retval Others means failure. (Refer to @ref WVR_Result.)
 * @version API Level 8
*/
extern WVR_EXPORT WVR_Result WVR_GetTrackerPoseState(WVR_TrackerId trackerId, WVR_PoseOriginModel originModel, uint32_t predictedMilliSec, WVR_PoseState_t *poseState);

/**
 * @brief Function to get the input device capability of the tracker device for WVR_InputType
 *
 * @param trackerId Indicates what id of the tracker device. (Refer to @ref WVR_TrackerId.)
 * @param inputType One of @ref WVR_InputType.
 * @return int32_t, bitmask for @ref WVR_InputId. A value of -1 means an error has occurred.
 * @version API Level 8
*/
extern WVR_EXPORT int32_t WVR_GetTrackerInputDeviceCapability(WVR_TrackerId trackerId, WVR_InputType inputType);

/**
 * @brief Function to get the analog type of the input device of the tracker device
 *
 * @param trackerId Indicates what id of the tracker device. (Refer to @ref WVR_TrackerId.)
 * @param inputId One of @ref WVR_InputId.
 * @return Analog type for @ref WVR_InputId. (Refer to @ref WVR_AnalogType.)
 * @version API Level 8
*/
extern WVR_EXPORT WVR_AnalogType WVR_GetTrackerInputDeviceAnalogType(WVR_TrackerId trackerId, WVR_InputId inputId);

/**
 * @brief Function to get the button state of a specific input id of the tracker device
 *
 * @param trackerId Indicates what id of the tracker device. (Refer to @ref WVR_TrackerId.)
 * @param inputId One of @ref WVR_InputId. Indicates which button is pressed or unpressed.
 * @return bool, true for pressed and false means unpressed.
 * @version API Level 8
*/
extern WVR_EXPORT bool WVR_GetTrackerInputButtonState(WVR_TrackerId trackerId, WVR_InputId inputId);

/**
 * @brief Function to get the touch state of a specific input id of the tracker device
 *
 * @param trackerId Indicates what id of the tracker device. (Refer to @ref WVR_TrackerId.)
 * @param inputId One of @ref WVR_InputId. Indicates which input id is touched or untouched.
 * @return bool, true for touched and false means untouched.
 * @version API Level 8
*/
extern WVR_EXPORT bool WVR_GetTrackerInputTouchState(WVR_TrackerId trackerId, WVR_InputId inputId);

/**
 * @brief Function to get the analog data of a specific input id of the tracker device
 *
 * @param trackerId Indicates what id of the tracker device. (Refer to @ref WVR_TrackerId.)
 * @param inputId One of @ref WVR_InputId. This parameter specifies the physical touch on the tracker that retrieves the input data.
 * @return Analog data for @ref WVR_InputId. (Refer to @ref WVR_Axis.)
 * @version API Level 8
*/
extern WVR_EXPORT WVR_Axis_t WVR_GetTrackerInputAnalogAxis(WVR_TrackerId trackerId, WVR_InputId inputId);

/**
 * @brief Function to get the current battery level of the tracker device.
 *
 * @param trackerId Indicates what id of the tracker device. (Refer to @ref WVR_TrackerId.)
 * @return The current battery level of the tracker device. Valid interval: [0.0, 1.0].
 * Negative value means the tracker device does not support this feature or is not connected.
 * @version API Level 8
 */
extern WVR_EXPORT float WVR_GetTrackerBatteryLevel(WVR_TrackerId trackerId);

/**
 * @brief Function to trigger vibration with float amplitude of the tracker device
 *
 * @param trackerId Indicates what id of the tracker device. (Refer to @ref WVR_TrackerId.)
 * @param durationMicroSec The duration of the vibration in microsecond.
 * @param frequency The frequency of the vibration in Hz.
 * @param amplitude float intensity of the vibration. Valid interval: [0.0, 1.0].
 * @retval WVR_Success Trigger the vibration of the tracker device successfully.
 * @retval Others means failure. (Refer to @ref WVR_Result.)
 * @version API Level 8
*/
extern WVR_EXPORT WVR_Result WVR_TriggerTrackerVibration(WVR_TrackerId trackerId, uint32_t durationMicroSec = 0, float frequency = 0.0f, float amplitude = 0.0f);

/**
 * @brief Function to get extended data of tracker device.
 *
 * @param trackerId Indicates what id of the tracker device. (Refer to @ref WVR_TrackerId.)
 * @param validSize Output the size to determine the valid length of data content. If validSize is less than or equal to 0, the data content from the returned address are meaningless.
 * @param timestamp Output timestamp of tracker extended data. (API Level 12)
 * @retval int32_t* The first address of data content. If the returned address is nullptr, the data content are meaningless.
 * @remark This API works only when supportsExtendedData (obtained by WVR_GetTrackerCapabilities) is true .
 * @version API Level 8
*/
extern WVR_EXPORT int32_t* WVR_GetTrackerExtendedData(WVR_TrackerId trackerId, int32_t* validSize, uint64_t *timestamp = nullptr);

/**
 * @brief Function to get device name of tracker device.
 *
 * @param trackerId Indicates what id of the tracker device. (Refer to @ref WVR_TrackerId.)
 * @param nameSize Ouput name size of tracker device as the buffer size. First, set nullptr to parameter deviceName to get nameSize fisrt. Then use this obtained nameSize to create char* type variable. Finally, get device name by calling this function again with obtained nameSize (MUST be the number obtained from first calling) and created char* type variable (MUST NOT be nullptr).
 * @param deviceName Ouput device name of tracker device.
 * @retval WVR_Success get device name of tracker device successfully.
 * @version API Level 12
 */
extern WVR_EXPORT WVR_Result WVR_GetTrackerDeviceName(WVR_TrackerId trackerId, uint32_t *nameSize, char *deviceName);



#ifdef __cplusplus
} /* extern "C" */
#endif
#include "close_code.h"

#endif /* wvr_tracker_h_ */
