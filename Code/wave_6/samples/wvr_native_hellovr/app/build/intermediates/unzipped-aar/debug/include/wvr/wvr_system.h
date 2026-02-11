// "WaveVR SDK
// © 2017 HTC Corporation. All Rights Reserved.
//
// Unless otherwise required by copyright law and practice,
// upon the execution of HTC SDK license agreement,
// HTC grants you access to and use of the WaveVR SDK(s).
// You shall fully comply with all of HTC’s SDK license agreement terms and
// conditions signed by you and all SDK and API requirements,
// specifications, and documentation provided by HTC to You."


#ifndef wvr_system_h_
#define wvr_system_h_

#include "wvr_stdinc.h"
#include "wvr_types.h"

#include "begin_code.h"
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Enum used for indicating the passthrough image's refresh rate.
 */
 typedef enum {
    WVR_PassthroughImageRate_Boost   = 0,    //!< Default passthrough image's refresh rate (default).
    WVR_PassthroughImageRate_Normal  = 1     //!< Reduce the passthrough image's refresh rate for performance improvement.
 } WVR_PassthroughImageRate;

/**
* @brief Enum used for indicating the quality of passthrough
*/
typedef enum {
    WVR_PassthroughImageQuality_DefaultMode        = 0,             //!< default passthrough image quality (default)
    WVR_PassthroughImageQuality_PerformanceMode    = 1,             //!< shorten passthrough image quality to reserve more GPU resource for content use
    WVR_PassthroughImageQuality_QualityMode        = 2,             //!< sharpen passthrough image to benefit reading nearby text but cost more GPU resource with trade-off
} WVR_PassthroughImageQuality;

/**
* @brief Enum used for indicating the focusing mode of passthrogh
*/
typedef enum {
    WVR_PassthroughImageFocus_Scale    = 0,             //!< Focus on the interactive device scale, controller or hand
    WVR_PassthroughImageFocus_View     = 1              //!< Focus on the surrounding environment view(default)
} WVR_PassthroughImageFocus;

/**
 * @brief Enum used for indicating the CPU/GPU performance levels
 *
*/
typedef enum {
    WVR_PerfLevel_System     = 0,            //!< System defined performance level (default)
    WVR_PerfLevel_Minimum    = 1,            //!< Minimum performance level
    WVR_PerfLevel_Medium     = 2,            //!< Medium performance level
    WVR_PerfLevel_Maximum    = 3,            //!< Maximum performance level
    WVR_PerfLevel_NumPerfLevels
} WVR_PerfLevel;

/**
 * @brief The folowing flags describe the strategies used by AdaptiveQuality when
 * rendering quality is insufficent.
 *
*/
typedef enum {
    WVR_QualityStrategy_Default          = 1,            /**< **WVR_QualityStrategy_Default**: Automatically adjusts CPU/GPU performance level if needed. This flag is always **On**.*/
    WVR_QualityStrategy_SendQualityEvent = 1 << 1,       /**< **WVR_QualityStrategy_QualityHint**: Specify if it is needed to send events when the recommended quality has changed.. */
    WVR_QualityStrategy_AutoFoveation    = 1 << 2,       /**< **WVR_QualityStrategy_AutoFoveation**: Automatically enable/disable foveation rendering if needed. */
    WVR_QualityStrategy_AutoAMC          = 1 << 3,       /**< **WVR_QualityStrategy_AutoAMC**: Experimental function */
    WVR_QualityStrategy_Reserved_2       = 1 << 28,      /**< **WVR_QualityStrategy_Reserved_2**: System reserved. */
    WVR_QualityStrategy_Reserved_1       = 1 << 29,      /**< **WVR_QualityStrategy_Reserved_1**: System reserved. */
    WVR_QualityStrategy_Reserved         = 1 << 30       /**< **WVR_QualityStrategy_Reserved**: System reserved. */
} WVR_QualityStrategy;

inline WVR_QualityStrategy operator|(WVR_QualityStrategy x, WVR_QualityStrategy y) {
	return static_cast<WVR_QualityStrategy>(static_cast<int>(x) | static_cast<int>(y));
}

/**
 * @brief Function to check if input focus is captured by system.
 *
 * @return true when input focus is captured by system, otherwise return false.
 * @version API Level 1
*/
extern WVR_EXPORT bool WVR_IsInputFocusCapturedBySystem();

/**
 * @brief Set CPU and GPU performance level.
 *
 * This API is only supported on certain platform.
 * The API can be used to increase/decrease the CPU or GPU performance based on practical demands.
 *
 * @return true if set performance level success, otherwise return false.
 * @version API Level 2
 * @note Effective with Runtime version 2 or higher.
*/
extern WVR_EXPORT bool WVR_SetPerformanceLevels(WVR_PerfLevel cpuLevel, WVR_PerfLevel gpuLevel);

/**
 * @brief Enables the adaptive render quality and CPU/GPU performance level.
 *
 * Use this API to allow the WaveVR SDK runtime to adjust CPU/GPU performance level automatically.
 * It will use the strategies to improve the FPS when rendering performance is insufficient.
 *
 * If WVR_QualityStrategy is not specified, "WVR_QualityStrategy_Default" is used to adjust
 * the CPU/GPU performance level based on the system workload only.
 *
 * @param enable: Boolean to enable/disable WaveVR AdaptiveQuality.
 * @param strategyFlags: "bitwise-or" flags used to select strategies to improve the FPS when the rendering performance is insufficient.
 * @return true if enableAdaptiveQuality is successful, otherwise return false.
 * @version API Level 4
 * @note Supported since Runtime version 4 or higher.
*/
extern WVR_EXPORT bool WVR_EnableAdaptiveQuality(bool enable, uint32_t strategyFlags = WVR_QualityStrategy_Default);

/**
 * @brief This function checks if the adaptive render quality is enabled or disabled.
 *
 * Use this API to allow the WaveVR SDK runtime to check if adaptive quality is enabled.
 *
 * @return true if WVR_EnableAdaptiveQuality is enabled, otherwise returns false.
 * @version API Level 4
 * @note Supported since Runtime version 4 or higher.
*/
extern WVR_EXPORT bool WVR_IsAdaptiveQualityEnabled();

/**
 * @brief Function to set the quality of the passthrough image.
 *
 * The API adjusts the quality of passthrough to the WaveVR Runtime,
 * Affects the @ref WVR_ShowPassthroughOverlay(), @ref WVR_ShowPassthroughUnderlay and @ref WVR_ShowProjectedPassthrough
 *
 * @param quality Specify the passthrough quality to the WaveVR Runtime. (refer to @ref WVR_PassthroughImageQuality)
 * @return true if the setting of passthrough quality is successful, otherwise return false if not supported by the Runtime.
 * @version API Level 12
 * @note Supported since Runtime version 12 or higher.
*/
extern WVR_EXPORT bool WVR_SetPassthroughImageQuality(WVR_PassthroughImageQuality quality);

/**
 * @brief Function to set the focusing mode of passthrough image.
 *
 * The API adjusts the focusing mode of passthourgh to the WaveVR Runtime,
 * Affects the @ref WVR_ShowPassthroughOverlay(), @ref WVR_ShowPassthroughUnderlay and @ref WVR_ShowProjectedPassthrough
 *
 * @param focus Specify the passthrough focusing mode to the WaveVR Runtime. (refer to @ref WVR_PassthroughImageFocus)
 * @return true if the setting of passthrough focusing mode is successful, otherwise return false if not supported by the Runtime.
 * @version API Level 12
 * @note Supported since Runtime version 12 or higher.
*/
extern WVR_EXPORT bool WVR_SetPassthroughImageFocus(WVR_PassthroughImageFocus focus);

/**
 * @brief Function to set passthrough image's refresh rate.
 * The API adjusts the refresh rate of passthourgh image to the WaveVR Runtime,
 * Affects the @ref WVR_ShowPassthroughOverlay(), @ref WVR_ShowPassthroughUnderlay and @ref WVR_ShowProjectedPassthrough
 *
 * @param rate Specify the refresh rate of image to the WaveVR Runtime. (refer to @ref WVR_PassthroughImageRate)
 * @retval WVR_SUCCESS if the setting of passthrough image's rate mode is successful.
 * @retval WVR_Error_SystemInvalid if the initialization has not finished yet. You may try this API later.
 * @retval WVR_Error_FeatureNotSupport if this feature is not support on this device or service.
 * @retval WVR_Error_RuntimeVersionNotSupport if the runtime version is too old to support the function call.
 * @version API Level 15
 * @note Supported since Runtime version 15 or higher.
 */
 extern WVR_EXPORT WVR_Result WVR_SetPassthroughImageRate(WVR_PassthroughImageRate rate);

/**
 * @brief Function to show the passthrough overlay.
 *
 * Use this function to also decide whether the passthrough overlay should be shown or not.
 * This function must be called after calling @ref WVR_RenderInit().
 *
 * @param show: Show or hide passthrough overlay. The system passthrough(surrounding or out-of-boundary) would be disabled if the param show is true.
 * @param delaySubmit: Delay the return of @ref WVR_SubmitFrame() when the passthrough overlay is showing to improve the latency. default: false
 * @param showIndicator: Show or hide the controller indicator on passthrough overlay, default: false.
 * @retval true The setting is valid.
 * @retval false The setting is invalid.
 * @version API Level 5
 * @note Supported from Runtime version 5 or higher (certain devices only). Make sure the target device supports passthrough overlay by calling @ref WVR_GetSupportedFeatures() and checking @ref WVR_SupportedFeature_PassthroughOverlay.
 * Calling this API on render thread is not recommended due to IPC with Passthrough service.
*/
extern WVR_EXPORT bool WVR_ShowPassthroughOverlay(bool show, bool delaySubmit = false, bool showIndicator = false);

/**
 * @brief Function to check if the passthrough overlay is showing or not.
 *
 * This function must be called after calling @ref WVR_RenderInit().
 *
 * @retval true The passthrough overlay is showing.
 * @retval false The passthrough overlay is not showing.
 * @version API Level 5
 * @note Supported from Runtime version 5 or higher (certain devices only). Make sure the target device supports passthrough overlay by calling @ref WVR_GetSupportedFeatures() and checking @ref WVR_SupportedFeature_PassthroughOverlay.
*/
extern WVR_EXPORT bool WVR_IsPassthroughOverlayVisible();

/**
 * @brief Function to enable or disable screen protection functions (cast, recording, snapshot) in OEM service.
 * @param enable True means enable screen protection functions; false means disable screen protection functions.
 * @version API Level 7
 */
extern WVR_EXPORT void WVR_SetScreenProtection(bool enable);

/**
 * @brief Function to set the alpha for the passthrough overlay.
 *
 * This function must be called after calling @ref WVR_RenderInit().
 *
 * @param alpha is the alpha factor in range 0.0 to 1.0.
 * @retval WVR_SUCCESS The setting is valid.
 * @retval WVR_Error_RuntimeVersionNotSupport This feature is supported from Runtime version 7 or higher.
 * @retval WVR_Error_FeatureNotSupport This feature is not supported on this device.
 * @version API Level 8
 * @note Supported from Runtime version 7 or higher (certain devices only). Make sure the target device supports passthrough overlay by calling @ref WVR_GetSupportedFeatures() and checking @ref WVR_SupportedFeature_PassthroughOverlay.
*/
extern WVR_EXPORT WVR_Result WVR_SetPassthroughOverlayAlpha(const float alpha);

/**
 * @brief Function to show the passthrough underlay.
 *
 * Use this function to also decide whether the passthrough underlay should be shown or not.
 * This function must be called after calling @ref WVR_RenderInit().
 *
 * @param show Show or hide passthrough underlay. The system passthrough(surrounding or out-of-boundary) would be disabled if the param show is true.
 * @retval WVR_SUCCESS The setting is valid.
 * @retval WVR_Error_RuntimeVersionNotSupport This feature is supported from Runtime version 7 or higher.
 * @retval WVR_Error_FeatureNotSupport This feature is not supported on this device.
 * @version API Level 7
 * @note Supported from Runtime version 7 or higher (certain devices only).
 * Calling this API on render thread is not recommended due to IPC with Passthrough service.
*/
extern WVR_EXPORT WVR_Result WVR_ShowPassthroughUnderlay(bool show);


/**
 * @brief Function to set the pose for the projected passthrough.
 *
 * This function must be called after calling @ref WVR_RenderInit().
 *
 * @param pose Set the pose for the projected passthrough.
 * @retval WVR_SUCCESS The setting is valid.
 * @retval WVR_Error_RuntimeVersionNotSupport This feature is supported from Runtime version 7 or higher.
 * @retval WVR_Error_FeatureNotSupport This feature is not supported on this device.
 * @version API Level 7
 * @note Supported from Runtime version 7 or higher (certain devices only).
*/
extern WVR_EXPORT WVR_Result WVR_SetProjectedPassthroughPose(const WVR_Pose_t *pose);

/**
 * @brief Function to set the rectangle area for the projected passthrough.
 *
 * This function must be called after calling @ref WVR_RenderInit().
 *
 * @param vertexBuffer Set the vertex data.
 * @param vertextCount Set the count of vertex data.
 * @param indices Set the triangle indices.
 * @param indexCount Set the count of indices.
 * @retval WVR_SUCCESS The setting is valid.
 * @retval WVR_Error_RuntimeVersionNotSupport This feature is supported from Runtime version 7 or higher.
 * @retval WVR_Error_FeatureNotSupport This feature is not supported on this device.
 * @version API Level 7
 * @note Supported from Runtime version 7 or higher (certain devices only).
*/
extern WVR_EXPORT WVR_Result WVR_SetProjectedPassthroughMesh(float *vertexBuffer, uint32_t vertextCount, uint32_t *indices, uint32_t indexCount);

/**
 * @brief Function to set the alpha for the projected passthrough.
 *
 * This function must be called after calling @ref WVR_RenderInit().
 *
 * @param alpha Set the alpha for the projected passthrough.
 * @retval WVR_SUCCESS The setting is valid.
 * @retval WVR_Error_RuntimeVersionNotSupport This feature is supported from Runtime version 7 or higher.
 * @retval WVR_Error_FeatureNotSupport This feature is not supported on this device.
 * @version API Level 7
 * @note Supported from Runtime version 7 or higher (certain devices only).
*/
extern WVR_EXPORT WVR_Result WVR_SetProjectedPassthroughAlpha(float alpha);

/**
 * @brief Function to show or hide the projected passthrough.
 *
 * This function must be called after calling @ref WVR_RenderInit().
 *
 * @param show Show or hide the projected passthrough. The system passthrough(surrounding or out-of-boundary) would be disabled if the param show is true.
 * @retval WVR_SUCCESS The setting is valid.
 * @retval WVR_Error_RuntimeVersionNotSupport This feature is supported from Runtime version 7 or higher.
 * @retval WVR_Error_FeatureNotSupport This feature is not supported on this device.
 * @version API Level 7
 * @note Supported from Runtime version 7 or higher (certain devices only).
 * Calling this API on render thread is not recommended due to IPC with Passthrough service.
*/
extern WVR_EXPORT WVR_Result WVR_ShowProjectedPassthrough(bool show);

#ifdef __cplusplus
} /* extern "C" */
#endif
#include "close_code.h"

#endif /* wvr_system_h_ */
