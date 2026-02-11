// "WaveVR SDK
// © 2023 HTC Corporation. All Rights Reserved.
//
// Unless otherwise required by copyright law and practice,
// upon the execution of HTC SDK license agreement,
// HTC grants you access to and use of the WaveVR SDK(s).
// You shall fully comply with all of HTC’s SDK license agreement terms and
// conditions signed by you and all SDK and API requirements,
// specifications, and documentation provided by HTC to You."


#ifndef wvr_marker_h_
#define wvr_marker_h_

#include "wvr_stdinc.h"
#include "wvr_types.h"

#include "begin_code.h"
#ifdef __cplusplus
extern "C" {
#endif

#define WVR_MAX_MARKER_NAME_SIZE 256   /**< maximum size for marker's name */

/**
* @brief Function is used to start the marker features.
*
* This API will start marker device service to manage the system resource of the markers.
*
* @retval WVR_Success Start marker successfully.
* @retval others @ref WVR_Result mean failure.
* @version API Level 13
*/
extern WVR_EXPORT WVR_Result WVR_StartMarker();

/**
 * @brief Function is used to stop marker features.
 *
 * This API will stop operations related to markers.
 *
 * @version API Level 13
*/
extern WVR_EXPORT void WVR_StopMarker();

/**
 * @brief enumerate different marker observer types.
 */
typedef enum {
    WVR_MarkerObserverTarget_Aruco         = 0,           /**< Specifies the observer target is Aruco. */
    WVR_MarkerObserverTarget_Max           = 0x7FFFFFFF
} WVR_MarkerObserverTarget;

/**
 * @brief Function is used to enable marker observer with the target parameter type.
 *
 * This API will enable marker observer type related to the input observer target.
 *
 * @param target the observer target that developers would like to use. (refer to @ref WVR_MarkerObserverTarget)
 * @retval WVR_Success Start marker observer successfully.
 * @retval others @ref WVR_Result mean failure.
 * @version API Level 13
*/
extern WVR_EXPORT WVR_Result WVR_StartMarkerObserver(WVR_MarkerObserverTarget target);

/**
 * @brief Function is used to disable marker observer with the target parameter type.
 *
 * This API will disable marker observer related to input observer target type and release the underlying related resources.
 *
 * @param target the observer target that developers would like to use. (refer to @ref WVR_MarkerObserverTarget)
 * @retval WVR_Success Stop marker observer successfully.
 * @retval others @ref WVR_Result mean failure.
 * @version API Level 13
*/
extern WVR_EXPORT WVR_Result WVR_StopMarkerObserver(WVR_MarkerObserverTarget target);

/**
 * @brief enumerate different marker observer state
 */
typedef enum {
    WVR_MarkerObserverState_Idle       = 0,            /**< indicates that marker observer is idle */
    WVR_MarkerObserverState_Detecting  = 1,            /**< indicates that the surrounding markers are detected */
    WVR_MarkerObserverState_Tracking   = 2,            /**< indicates that the indicated markers are tracked */
    WVR_MarkerObserverState_Max        = 0x7FFFFFFF
} WVR_MarkerObserverState;

/**
 * @brief Function is used to get the corresponding marker observer target state.
 *
 * Developers can inspect the marker observer state by polling @ref WVR_GetMarkerObserverState.
 * Check state (refer to @ref WVR_MarkerObserverState) to know the current status.
 *
 * @param target input marker observer target , refer to @ref WVR_MarkerObserverTarget
*  @param state the returned @ref WVR_MarkerObserverState value
 * @retval WVR_Success Get marker observer state successfully.
 * @retval others @ref WVR_Result mean failure.
 * @version API Level 13
*/
extern WVR_EXPORT WVR_Result WVR_GetMarkerObserverState(
    WVR_MarkerObserverTarget      target,
    WVR_MarkerObserverState*      state);

/**
 * @brief Function is used to start marker detection with the target parameter.
 *
 * This API will start marker detection related to the input observer target.
 *
 * @param target the observer target that developers would like to detect. (refer to @ref WVR_MarkerObserverTarget)
 * @retval WVR_Success Start marker detection successfully.
 * @retval others @ref WVR_Result mean failure.
 * @version API Level 13
*/
extern WVR_EXPORT WVR_Result WVR_StartMarkerDetection(WVR_MarkerObserverTarget target);

/**
 * @brief Function is used to stop marker detection with the target parameter.
 *
 * This API will stop marker detection related to the input observer target.
 *
 * @param target the observer target that developers would like to stop detecting. (refer to @ref WVR_MarkerObserverTarget)
 * @retval WVR_Success stop marker detection successfully.
 * @retval others @ref WVR_Result mean failure.
 * @version API Level 13
*/
extern WVR_EXPORT WVR_Result WVR_StopMarkerDetection(WVR_MarkerObserverTarget target);

/**
 * @brief name is a null terminated character array of size WVR_MAX_MARKER_NAME_SIZE
 */
typedef struct WVR_MarkerName {
    char    name[WVR_MAX_MARKER_NAME_SIZE];   /**< The name of the marker */
} WVR_MarkerName;

/**
 * @brief enumerate different marker tracking state
 */
typedef enum {
    WVR_MarkerTrackingState_Detected,       /**< Detected state */
    WVR_MarkerTrackingState_Tracked,        /**< Tracked state */
    WVR_MarkerTrackingState_Paused,         /**< Paused state */
    WVR_MarkerTrackingState_Stopped         /**< Stopped state */
} WVR_MarkerTrackingState;

/**
 * @brief returned aruco marker information
 */
typedef struct WVR_ArucoMarker {
    WVR_Uuid                    uuid;          /**< indicates the uuid of the aruco marker */
    uint64_t                    trackerId;     /**< indicates the tracker id of the aruco marker */
    float                       size;          /**< indicates the size */
    WVR_MarkerTrackingState     state;         /**< indicates the state */
    WVR_Pose_t                  pose;          /**< indicates the pose of the aruco marker */
    WVR_MarkerName              markerName;    /**< indicates the name */
} WVR_ArucoMarker;

/**
 * @brief Function is used to retrieve aruco markers.
 *
 * This is two calls API. Developers should call this API with the value of markerCapacityInput equals to 0 to retrieve
 * the size of aruco markers from markerCountOutput. Then developers allocate the array of @ref WVR_ArucoMarker data and
 * assign the markerCapacityInput and call the API in the second time. Then runtime will fill the @ref WVR_ArucoMarker array.
 *
 * Note: that only WVR_PoseOriginModel_OriginOnHead and WVR_PoseOriginModel_OriginOnGround cases are supported in originModel.
 * Note: Since there is a time gap between the first and the second calls, developers should verify the markerCountOutput when retrieving the
 * data that filled by runtime during the second time.
 *
 * @param markerCapacityInput the capacity of the aruco markers array, or 0 to indicate a request to retrieve the required capacity.
 * @param markerCountOutput a pointer to the count of aruco markers written, or a pointer to the required capacity in the case that markerCapacityInput is insufficient.
 * @param originModel Only WVR_PoseOriginModel_OriginOnHead and WVR_PoseOriginModel_OriginOnGround are supported, refer to @ref WVR_PoseOriginModel.
 * @param markers An array of @ref WVR_ArucoMarker will be filled by the runtime.
 * @retval WVR_Success Get aruco markers successfully.
 * @retval others @ref WVR_Result mean failure.
 * @version API Level 13
 */
extern WVR_EXPORT WVR_Result WVR_GetArucoMarkers(
    uint32_t                    markerCapacityInput,
    uint32_t*                   markerCountOutput,
    WVR_PoseOriginModel         originModel,
    WVR_ArucoMarker*            markers);

/**
 * @brief returned aruco marker data
 */
typedef struct WVR_ArucoMarkerData {
    uint64_t                    trackerId;     /**< indicates the tracker id of the aruco marker */
    float                       size;          /**< indicates the size */
} WVR_ArucoMarkerData;

/**
 * @brief Function is used to retrieve aruco marker data @ref WVR_ArucoMarkerData with input uuid @ref WVR_Uuid
 *
 * @param uuid the input uuid @ref WVR_Uuid that developers would like to retrieve more data
 * @param data A struct of @ref WVR_ArucoMarkerData will be filled by the runtime.
 * @retval WVR_Success Get the aruco marker data successfully.
 * @retval others @ref WVR_Result mean failure.
 * @version API Level 13
 */
extern WVR_EXPORT WVR_Result WVR_GetArucoMarkerData(
    WVR_Uuid                    uuid,
    WVR_ArucoMarkerData*        data);

/**
 * @brief Function is used to enumerate the markers that have been created as trackable markers related to the target parameter. .
 *
 * This is two calls API. Developers should call this API with the value of markerCapacityInput equals to 0 to retrieve
 * the size of markers from markerCountOutput. Then developers allocate the array of @ref WVR_Uuid and
 * assign the markerCapacityInput and call the API in the second time. Then runtime will fill the @ref WVR_Uuid array.
 *
 * Note: Since there is a time gap between the first and the second calls, developers should verify the markerCountOutput when retrieving the
 * data that filled by runtime during the second time.
 *
 * @param target the observer target that developers would like to get the trackable markers. (refer to @ref WVR_MarkerObserverTarget)
 * @param markerCapacityInput the capacity of the markers array, or 0 to indicate a request to retrieve the required capacity.
 * @param markerCountOutput a pointer to the count of markers written, or a pointer to the required capacity in the case that markerCapacityInput is insufficient.
 * @param markers a pointer to an array of @ref WVR_Uuid, but can be NULL if markerCapacityInput is 0.
 * @retval WVR_Success Enumerate trackable markers successfully.
 * @retval others @ref WVR_Result mean failure.
 * @version API Level 13
 */
extern WVR_EXPORT WVR_Result WVR_EnumerateTrackableMarkers(
    WVR_MarkerObserverTarget    target,
    uint32_t                    markerCapacityInput,
    uint32_t*                   markerCountOutput,
    WVR_Uuid*                   markers);

/**
 * @brief The parameters to create a new trackable marker used in @ref WVR_CreateTrackableMarker API.
 *
 * Developers provide information to create an trackable, including uuid and markerName.
 */
typedef struct WVR_TrackableMarkerCreateInfo {
    WVR_Uuid                    uuid;
    WVR_MarkerName              markerName;
} WVR_TrackableMarkerCreateInfo;

/**
 * @brief Function is used to create a trackable marker.
 *
 * Developers use this API to create a trackable marker with input @ref WVR_TrackableMarkerCreateInfo structure.
 *
 * @param createInfo creation information, refer to @ref WVR_TrackableMarkerCreateInfo
 * @retval WVR_Success create a trackable marker successfully.
 * @retval others @ref WVR_Result mean failure.
 * @version API Level 13
*/
extern WVR_EXPORT WVR_Result WVR_CreateTrackableMarker(
    const WVR_TrackableMarkerCreateInfo*    createInfo);

/**
 * @brief Function is used to destroy a trackable marker.
 *
 * Developers use this API to destroy an trackable marker with input uuid @ref WVR_Uuid.
 *
 * @param uuid the trackable marker with input uuid @ref WVR_Uuid that developers would like to destroy
 * @retval WVR_Success Destroy a trackable marker successfully.
 * @retval others @ref WVR_Result mean failure.
 * @version API Level 13
 */
extern WVR_EXPORT WVR_Result WVR_DestroyTrackableMarker(
    WVR_Uuid                    uuid);

/**
 * @brief Function is used to start tracking the trackable marker.
 *
 * Developers use this API to track the trackable marker with input uuid @ref WVR_Uuid.
 *
 * @param uuid the trackable marker with input uuid @ref WVR_Uuid that developers would like to track
 * @retval WVR_Success start the trackable marker successfully.
 * @retval others @ref WVR_Result mean failure.
 * @version API Level 13
*/
extern WVR_EXPORT WVR_Result WVR_StartTrackableMarkerTracking(
    WVR_Uuid                    uuid);

/**
 * @brief Function is used to stop tracking the trackable marker.
 *
 * Developers use this API to stop tracking the trackable marker with input uuid @ref WVR_Uuid.
 *
 * @param uuid the trackable marker with input uuid @ref WVR_Uuid that developers would like to stop tracking
 * @retval WVR_Success stop tracking the trackable marker successfully.
 * @retval others @ref WVR_Result mean failure.
 * @version API Level 13
*/
extern WVR_EXPORT WVR_Result WVR_StopTrackableMarkerTracking(
    WVR_Uuid                    uuid);

/**
 * @brief returned trackable marker state
 */
typedef struct WVR_TrackableMarkerState {
    WVR_MarkerObserverTarget    target;        /**< indicates the observer target */
    WVR_MarkerTrackingState     state;         /**< indicates the state */
    WVR_Pose_t                  pose;          /**< indicates the pose of the trackable marker */
    WVR_MarkerName              markerName;    /**< indicates the name */
} WVR_TrackableMarkerState;

/**
 * @brief Function is used to retrieve @ref WVR_TrackableMarkerState related to the trackable marker with input uuid @ref WVR_Uuid.
 *
 * Developers use this API to retrieve the trackable marker information.
 *
 * @param uuid the trackable marker with input uuid @ref WVR_Uuid.
 * @param originModel Only WVR_PoseOriginModel_OriginOnHead and WVR_PoseOriginModel_OriginOnGround are supported, refer to @ref WVR_PoseOriginModel.
 * @param state the returned data related to the trackable marker with input uuid @ref WVR_Uuid.
 * @retval WVR_Success Get the the state of the trackable marker successfully.
 * @retval others @ref WVR_Result mean failure.
 * @version API Level 13
*/
extern WVR_EXPORT WVR_Result WVR_GetTrackableMarkerState(
    WVR_Uuid                    uuid,
    WVR_PoseOriginModel         originModel,
    WVR_TrackableMarkerState*   state);

/**
 * @brief Function is used to clear all trackable markers.
 *
 * Developers use this API to clear all trackable markers on runtime.
 *
 * @retval WVR_Success Clear all cached anchors successfully.
 * @retval others @ref WVR_Result mean failure.
 * @version API Level 16
 */
extern WVR_EXPORT WVR_Result WVR_ClearTrackableMarkers();

#ifdef __cplusplus
}
#endif
#include "close_code.h"

#endif /* wvr_marker_h_ */
