// "WaveVR SDK
// © 2022 HTC Corporation. All Rights Reserved.
//
// Unless otherwise required by copyright law and practice,
// upon the execution of HTC SDK license agreement,
// HTC grants you access to and use of the WaveVR SDK(s).
// You shall fully comply with all of HTC’s SDK license agreement terms and
// conditions signed by you and all SDK and API requirements,
// specifications, and documentation provided by HTC to You."

#ifndef wvr_anchor_h_
#define wvr_anchor_h_

#include "wvr_stdinc.h"
#include "wvr_types.h"

#include "begin_code.h"
#ifdef __cplusplus
extern "C" {
#endif

#define WVR_MAX_SPATIAL_ANCHOR_NAME_SIZE 256                 /**< maximum size for anchor's name */
#define WVR_DEFINE_HANDLE(object) typedef uint64_t object;

/**
 * @brief an object that allows an application to communicate with runtime
 */
WVR_DEFINE_HANDLE(WVR_SpatialAnchor)   /**< an object that allows an application to communicate with runtime */

/**
 * @brief name is a null terminated character array of size WVR_MAX_SPATIAL_ANCHOR_NAME_SIZE
 */
typedef struct WVR_SpatialAnchorName {
    char    name[WVR_MAX_SPATIAL_ANCHOR_NAME_SIZE];   /**< The name of the anchor */
} WVR_SpatialAnchorName;

/**
 * @brief The parameters to create a new spatial anchor used in @ref WVR_CreateSpatialAnchor API.
 *
 * Developers provide information to create an anchor, including pose, originModel and anchorName.
 * Note that only WVR_PoseOriginModel_OriginOnHead and WVR_PoseOriginModel_OriginOnGround cases are supported in originModel.
 */
typedef struct WVR_SpatialAnchorCreateInfo {
    WVR_Pose_t                  pose;           /**< The pose is an @ref WVR_Pose defining the position and orientation of the anchor. */
    WVR_PoseOriginModel         originModel;    /**< The pose's origin model, refer to @ref WVR_PoseOriginModel */
    WVR_SpatialAnchorName       anchorName;     /**< The name of the anchor, refer to @ref WVR_SpatialAnchorName */
} WVR_SpatialAnchorCreateInfo;

/**
 * @brief Function is used to create a spatial anchor.
 *
 * Developers use this API to create a spatial anchor and get an anchor handle.
 * This spatial anchor will be cleared once AP session closed.
 *
 * Note: Since API Level 14, once the spatial anchor list has been changed, the corresponding event (@ref WVR_EventType) will be fired.
 * This API will cause WVR_EventType_SpatialAnchor_Changed event.
 *
 * @param createInfo creation information, refer to @ref WVR_SpatialAnchorCreateInfo
 * @param anchor returned handle object that holds the spatial anchor is related to createInfo
 * @retval WVR_Success Create a spatial anchor successfully.
 * @retval others @ref WVR_Result mean failure.
 * @version API Level 11
 */
extern WVR_EXPORT WVR_Result WVR_CreateSpatialAnchor(
    const WVR_SpatialAnchorCreateInfo*      createInfo,
    WVR_SpatialAnchor*                      anchor);

/**
 * @brief Function is used to destroy a spatial anchor.
 *
 * Developers use this API to destroy an anchor handle.
 *
 * Note: Since API Level 14, once the spatial anchor list has been changed, the corresponding event (@ref WVR_EventType) will be fired.
 * This API may cause WVR_EventType_SpatialAnchor_Changed event.
 *
 * @param anchor the spatial anchor that developers would like to destroy
 * @retval WVR_Success Destroy a spatial anchor successfully.
 * @retval others @ref WVR_Result mean failure.
 * @version API Level 11
 */
extern WVR_EXPORT WVR_Result WVR_DestroySpatialAnchor(
    WVR_SpatialAnchor           anchor);

/**
 * @brief Function is used to enumerate spatial anchors.
 *
 * This is two calls API. Developers should call this API with the value of anchorCapacityInput equals to 0 to retrieve
 * the size of anchors from anchorCountOutput. Then developers allocate the array of @ref WVR_SpatialAnchor handles and
 * assign the anchorCapacityInput and call the API in the second time. Then runtime will fill the handle anchors array.
 *
 * Note: Since there is a time gap between the first and the second calls, developers should verify the anchorCountOutput
 * when retrieving the data that filled by runtime during the second time.
 *
 * @param anchorCapacityInput the capacity of the anchor array, or 0 to indicate a request to retrieve the required capacity.
 * @param anchorCountOutput a pointer to the count of anchors written, or a pointer to the required capacity in the case that anchorCapacityInput is insufficient.
 * @param anchors a pointer to an array of @ref WVR_SpatialAnchor handles, but can be NULL if anchorCapacityInput is 0.
 * @retval WVR_Success Enumerate spatial anchors successfully.
 * @retval others @ref WVR_Result mean failure.
 * @version API Level 11
 */
extern WVR_EXPORT WVR_Result WVR_EnumerateSpatialAnchors(
    uint32_t                    anchorCapacityInput,
    uint32_t*                   anchorCountOutput,
    WVR_SpatialAnchor*          anchors);

/**
 * @brief enumerate different spatial anchor tracking state
 */
typedef enum {
    WVR_SpatialAnchorTrackingState_Tracking,       /**< Tracking state; The anchor is located and its pose is valid. */
    WVR_SpatialAnchorTrackingState_Paused,         /**< Paused state; The system is detecting/searching the anchor and its pose is not ready. */
    WVR_SpatialAnchorTrackingState_Stopped         /**< Stopped state; The anchor can not be tracked anymore, for instance it has been deleted. */
} WVR_SpatialAnchorTrackingState;

/**
 * @brief returned anchor state
 */
typedef struct WVR_SpatialAnchorState {
    WVR_SpatialAnchorTrackingState      trackingState; /**< tracking state of the input anchor, refer to @ref WVR_SpatialAnchorTrackingState. */
    WVR_Pose_t                          pose;          /**< the pose of anchor related to originModel, refer to @ref WVR_Pose. */
    WVR_SpatialAnchorName               anchorName;    /**< The name of the anchor, refer to @ref WVR_SpatialAnchorName. */
} WVR_SpatialAnchorState;

/**
 * @brief Function is used to retrieve @ref WVR_SpatialAnchorState related to the input anchor.
 *
 * @param anchor input anchor handle
 * @param originModel Only WVR_PoseOriginModel_OriginOnHead and WVR_PoseOriginModel_OriginOnGround are supported, refer to @ref WVR_PoseOriginModel.
 * @param anchorState the returned data related to input anchor
 * @retval WVR_Success Get the input spacial anchor state successfully.
 * @retval others @ref WVR_Result mean failure.
 * @version API Level 11
 */
extern WVR_EXPORT WVR_Result WVR_GetSpatialAnchorState(
    WVR_SpatialAnchor           anchor,
    WVR_PoseOriginModel         originModel,
    WVR_SpatialAnchorState*     anchorState);

/**
 * @brief The parameters to cache a spatial anchor used in @ref WVR_CacheSpatialAnchor API.
 *
 * Developers provide information to cache an anchor, including spatialAnchor and cachedSpatialAnchorName.
 * Note that cachedSpatialAnchorName should be unique on it's binding SLAM map, or @ref WVR_CacheSpatialAnchor will return WVR_Error_Anchor_NameDuplicated.
 * The spatialAnchor should not be cached before, or @ref WVR_CacheSpatialAnchor will return WVR_Error_Anchor_AnchorDuplicated.
 */
 typedef struct WVR_SpatialAnchorCacheInfo {
    WVR_SpatialAnchor                   spatialAnchor;             /**< The handle of the anchor. */
    WVR_SpatialAnchorName               cachedSpatialAnchorName;   /**< The name of the cached anchor. */
} WVR_SpatialAnchorCacheInfo;

/**
 * @brief Function is used to build and store a cache anchor from previous created spatial anchor.
 *
 * Developers use this API to build and store a cache anchor on runtime.
 * The cached anchors is SLAM tracking map dependent. They will be effected by runtime once the SLAM map is reset and re-create for this room.
 *
 * Note: Once the cached spatial anchor list has been changed, the corresponding event (@ref WVR_EventType) will be fired.
 * This API will cause WVR_EventType_CachedAnchor_Changed event.
 *
 * @param spatialAnchorCacheInfo creation information from previous created spatial anchor and give a cachedSpatialAnchorName, refer to @ref WVR_SpatialAnchorCacheInfo
 * @retval WVR_Success Build and store a cache anchor successfully.
 * @retval others @ref WVR_Result mean failure.
 * @version API Level 14
 */
extern WVR_EXPORT WVR_Result WVR_CacheSpatialAnchor(
    const WVR_SpatialAnchorCacheInfo*   spatialAnchorCacheInfo);

/**
 * @brief Function is used to remove a cached anchor.
 *
 * Developers use this API to remove a cached anchor on runtime.
 *
 * Note: Once the cached spatial anchor list has been changed, the corresponding event (@ref WVR_EventType) will be fired.
 * This API will cause WVR_EventType_CachedAnchor_Changed event.
 *
 * @param cachedSpatialAnchorName the name of this cached anchor, refer to @ref WVR_SpatialAnchorName
 * @retval WVR_Success Remove a cached anchor successfully.
 * @retval others @ref WVR_Result mean failure.
 * @version API Level 14
 */
extern WVR_EXPORT WVR_Result WVR_UncacheSpatialAnchor(
    const WVR_SpatialAnchorName*        cachedSpatialAnchorName);

/**
 * @brief Function is used to enumerate the name list of cached anchors.
 *
 * This is two calls API. Developers should call this API with the value of cachedSpatialAnchorNamesCapacityInput equals to 0 to retrieve
 * the size of data from cachedSpatialAnchorNamesCountOutput. Then developers allocate the size of data and
 * assign the cachedSpatialAnchorNamesCapacityInput and call the API in the second time. Then runtime will fill the data.
 *
 * Note: Since there is a time gap between the first and the second calls, developers should verify the cachedSpatialAnchorNamesCountOutput
 * when retrieving the data that filled by runtime during the second time.
 *
 * @param cachedSpatialAnchorNamesCapacityInput the capacity of the data, or 0 to indicate a request to retrieve the required capacity.
 * @param cachedSpatialAnchorNamesCountOutput a pointer to the count of data written, or a pointer to the required capacity in the case that cachedSpatialAnchorNamesCapacityInput is insufficient.
 * @param cachedSpatialAnchorNames a pointer to an array of @ref WVR_SpatialAnchorName names, but can be NULL if cachedSpatialAnchorNamesCapacityInput is 0.
 * @retval WVR_Success Enumerate the names of the cached spatial anchors successfully.
 * @retval others @ref WVR_Result mean failure.
 * @version API Level 14
 */
extern WVR_EXPORT WVR_Result WVR_EnumerateCachedSpatialAnchorNames(
    uint32_t                            cachedSpatialAnchorNamesCapacityInput,
    uint32_t*                           cachedSpatialAnchorNamesCountOutput,
    WVR_SpatialAnchorName*              cachedSpatialAnchorNames);

/**
 * @brief Function is used to clear all cached anchors.
 *
 * Developers use this API to clear all cached anchors on runtime.
 *
 * @retval WVR_Success Clear all cached anchors successfully.
 * @retval others @ref WVR_Result mean failure.
 * @version API Level 14
 */
extern WVR_EXPORT WVR_Result WVR_ClearCachedSpatialAnchors();

/**
 * @brief The parameters to create a spatial anchor from the enumerated cached names used
 * in @ref WVR_CreateSpatialAnchorFromCacheName API.
 *
 * Developers provide information to create a spatial anchor, including cachedSpatialAnchorName and spatialAnchorName.
 * Note: that cachedSpatialAnchorName should be retrieved from @ref WVR_EnumerateCachedSpatialAnchorNames API.
 */
typedef struct WVR_SpatialAnchorFromCacheNameCreateInfo {
    WVR_SpatialAnchorName   cachedSpatialAnchorName;
    WVR_SpatialAnchorName   spatialAnchorName;
} WVR_SpatialAnchorFromCacheNameCreateInfo;

/**
 * @brief Function is used to create a spatial anchor from previous built cached anchor.
 *
 * Developers use this API to create a spatial anchor by the cachedSpatialAnchorName of @ref WVR_SpatialAnchorFromCacheNameCreateInfo and get an anchor handle.
 * This spatial anchor which created from cached anchor will be cleared once AP session closed.
 * Each cachedSpatialAnchorName can only create one spatial anchor on it's binding SLAM map in an AP session otherwise it will get anchor duplicated error.
 *
 * Note: Once the spatial anchor list has been changed, the corresponding event (@ref WVR_EventType) will be fired.
 * This API will cause WVR_EventType_SpatialAnchor_Changed event.
 *
 * @param createInfo creation information, refer to @ref WVR_SpatialAnchorFromCacheNameCreateInfo
 * @param anchor returned handle object that holds the spatial anchor is related to createInfo
 * @retval WVR_Success Create a spatial anchor successfully.
 * @retval others @ref WVR_Result mean failure.
 * @version API Level 14
 */
extern WVR_EXPORT WVR_Result WVR_CreateSpatialAnchorFromCacheName(
    const WVR_SpatialAnchorFromCacheNameCreateInfo*     createInfo,
    WVR_SpatialAnchor*                                  anchor);

/**
 * @brief The parameters to persist a spatial anchor used in @ref WVR_PersistSpatialAnchor API.
 *
 * Developers provide information to persist an anchor, including spatialAnchor and persistedSpatialAnchorName.
 * Note that persistedSpatialAnchorName should be unique, or @ref WVR_PersistSpatialAnchor will return WVR_Error_Anchor_NameDuplicated.
 * The spatialAnchor should not be persisted before, or @ref WVR_PersistSpatialAnchor will return WVR_Error_Anchor_AnchorDuplicated.
 */
typedef struct WVR_SpatialAnchorPersistInfo {
    WVR_SpatialAnchor                   spatialAnchor;                 /**< The handle of the anchor, refer to @ref WVR_SpatialAnchor. */
    WVR_SpatialAnchorName               persistedSpatialAnchorName;    /**< The name of the cached anchor, refer to @ref WVR_SpatialAnchorName. */
} WVR_SpatialAnchorPersistInfo;

/**
 * @brief Function is used to build and store a persisted anchor from previous created spatial anchor.
 *
 * Developers use this API to build and store a persisted anchor on runtime.
 * The persisted anchor is saved with feature points info. around the object and environment, so it exists even the SLAM tracking map is reset and re-create.
 *
 * Note: Once the persisted spatial anchor list has been changed, the corresponding event (@ref WVR_EventType) will be fired.
 * This API will cause WVR_EventType_PersistedAnchor_Changed event.
 *
 * @param spatialAnchorPersistInfo creation information from previous created spatial anchor and give an unique persistedSpatialAnchorName, refer to @ref WVR_SpatialAnchorPersistInfo
 * @retval WVR_Success Build and store a persisted anchor successfully.
 * @retval others @ref WVR_Result mean failure.
 * @version API Level 14
 */
extern WVR_EXPORT WVR_Result WVR_PersistSpatialAnchor(
    const WVR_SpatialAnchorPersistInfo* spatialAnchorPersistInfo);

/**
 * @brief Function is used to remove a persisted anchor.
 *
 * Developers use this API to remove a persisted anchor on runtime.
 *
 * Note: Once the persisted spatial anchor list has been changed, the corresponding event (@ref WVR_EventType) will be fired.
 * This API will cause WVR_EventType_PersistedAnchor_Changed event.
 *
 * @param persistedSpatialAnchorName the unique name of this persisted anchor, refer to @ref WVR_SpatialAnchorName
 * @retval WVR_Success Remove a persisted anchor successfully.
 * @retval others @ref WVR_Result mean failure.
 * @version API Level 14
 */
extern WVR_EXPORT WVR_Result WVR_UnpersistSpatialAnchor(
    const WVR_SpatialAnchorName*        persistedSpatialAnchorName);

/**
 * @brief Function is used to enumerate the name list of persisted anchors.
 *
 * This is two calls API. Developers should call this API with the value of persistedSpatialAnchorNamesCapacityInput equals to 0 to retrieve
 * the size of data from persistedSpatialAnchorNamesCountOutput. Then developers allocate the size of data and
 * assign the persistedSpatialAnchorNamesCapacityInput and call the API in the second time. Then runtime will fill the data.
 *
 * Note: Since there is a time gap between the first and the second calls, developers should verify the persistedSpatialAnchorNamesCountOutput
 * when retrieving the data that filled by runtime during the second time.
 *
 * @param persistedSpatialAnchorNamesCapacityInput the capacity of the data, or 0 to indicate a request to retrieve the required capacity.
 * @param persistedSpatialAnchorNamesCountOutput a pointer to the count of data written, or a pointer to the required capacity in the case that persistedSpatialAnchorNamesCapacityInput is insufficient.
 * @param persistedSpatialAnchorNames a pointer to an array of @ref WVR_SpatialAnchorName names, but can be NULL if persistedSpatialAnchorNamesCapacityInput is 0.
 * @retval WVR_Success Enumerate the names of the persisted spatial anchors successfully.
 * @retval others @ref WVR_Result mean failure.
 * @version API Level 14
 */
extern WVR_EXPORT WVR_Result WVR_EnumeratePersistedSpatialAnchorNames(
    uint32_t                            persistedSpatialAnchorNamesCapacityInput,
    uint32_t*                           persistedSpatialAnchorNamesCountOutput,
    WVR_SpatialAnchorName*              persistedSpatialAnchorNames);

/**
 * @brief Function is used to clear all persisted anchors.
 *
 * Developers use this API to clear all persisted anchors on runtime.
 *
 * @retval WVR_Success Clear all persisted anchors successfully.
 * @retval others @ref WVR_Result mean failure.
 * @version API Level 14
 */
extern WVR_EXPORT WVR_Result WVR_ClearPersistedSpatialAnchors();

/**
 * @brief The parameters to retrieve the usage info of persisted anchors, including the max count that system supports and current usage number.
 *
 * Developers provide information to retrieve the usage info of persisted anchors, including maximumTrackingCount and currentTrackingCount.
 */
typedef struct WVR_PersistedSpatialAnchorCountGetInfo {
    uint32_t    maximumTrackingCount;
    uint32_t    currentTrackingCount;
} WVR_PersistedSpatialAnchorCountGetInfo;

/**
 * @brief Function is used to get the usage info of persisted anchors.
 *
 * Developers use this API to get the usage info of persisted anchor. The amount of persisted anchors is limited for each AP.
 * The AP will get error return value once allocated persisted anchors over maximumTrackingCount.
 *
 * @param getInfo count information of persisted anchors, refer to @ref WVR_PersistedSpatialAnchorCountGetInfo
 * @retval WVR_Success Get usage info of persisted anchors successfully.
 * @retval others @ref WVR_Result mean failure.
 * @version API Level 14
 */
extern WVR_EXPORT WVR_Result WVR_GetPersistedSpatialAnchorCount(
    WVR_PersistedSpatialAnchorCountGetInfo*     getInfo);

/**
 * @brief The parameters to create a spatial anchor from the enumerated persisted names used
 * in @ref WVR_CreateSpatialAnchorFromPersistenceName API.
 *
 * Developers provide information to create a spatial anchor, including persistedSpatialAnchorName and spatialAnchorName.
 * Note that persistedSpatialAnchorName should be retrieved from @ref WVR_EnumeratePersistedSpatialAnchorNames API.
 */
typedef struct WVR_SpatialAnchorFromPersistenceNameCreateInfo {
    WVR_SpatialAnchorName   persistedSpatialAnchorName;
    WVR_SpatialAnchorName   spatialAnchorName;
} WVR_SpatialAnchorFromPersistenceNameCreateInfo;

/**
 * @brief Function is used to create a spatial anchor from previous built persisted anchor.
 *
 * Developers use this API to create a spatial anchor by unique persistedSpatialAnchorName of @ref WVR_SpatialAnchorFromPersistenceNameCreateInfo and get an anchor handle.
 * This spatial anchor which created from persisted anchor will be cleared once AP session closed.
 * Each persistedSpatialAnchorName can only create one spatial anchor in an AP session otherwise it will get anchor duplicated error.
 *
 * Note: Once the spatial anchor list has been changed, the corresponding event (@ref WVR_EventType) will be fired.
 * This API will cause WVR_EventType_SpatialAnchor_Changed event.
 *
 * @param createInfo creation information, refer to @ref WVR_SpatialAnchorFromPersistenceNameCreateInfo
 * @param anchor returned handle object that holds the spatial anchor is related to createInfo
 * @retval WVR_Success Create a spatial anchor successfully.
 * @retval others @ref WVR_Result mean failure.
 * @version API Level 14
 */
extern WVR_EXPORT WVR_Result WVR_CreateSpatialAnchorFromPersistenceName(
    const WVR_SpatialAnchorFromPersistenceNameCreateInfo*   createInfo,
    WVR_SpatialAnchor*                                      anchor);

/**
 * @brief Function is used to export the persisted anchor related data. The exported data can be imported later by developers.
 *
 * This is two calls API. Developers should call this API with the value of dataCapacityInput equals to 0 to retrieve
 * the size of data from dataCountOutput. Then developers allocate the size of data and
 * assign the dataCapacityInput and call the API in the second time. Then runtime will fill the data.
 * Note that this API operation can be time-consuming, so it's recommended to avoid performing it on the main thread and render thread.
 *
 * @param persistedSpatialAnchorName persisted anchor name
 * @param dataCapacityInput the capacity of the data, or 0 to indicate a request to retrieve the required capacity.
 * @param dataCountOutput a pointer to the count of data written, or a pointer to the required capacity in the case that dataCapacityInput is insufficient.
 * @param data the buffer that will be filled by runtime, but can be NULL if dataCapacityInput is 0.
 * @retval WVR_Success Export the input spacial anchor data successfully.
 * @retval others @ref WVR_Result mean failure.
 * @version API Level 14
 */
extern WVR_EXPORT WVR_Result WVR_ExportPersistedSpatialAnchor(
    const WVR_SpatialAnchorName*    persistedSpatialAnchorName,
    uint32_t                        dataCapacityInput,
    uint32_t*                       dataCountOutput,
    char*                           data);

/**
 * @brief Function is used to import the persisted anchor related data.
 *
 * Note that this API operation can be time-consuming, so it's recommended to avoid performing it on the main thread and render thread.
 *
 * @param dataCount the data size that data allocated
 * @param data the data which is exported by @ref WVR_ExportPersistedSpatialAnchor API
 * @retval WVR_Success Import the data successfully.
 * @retval others @ref WVR_Result mean failure.
 * @version API Level 14
 */
extern WVR_EXPORT WVR_Result WVR_ImportPersistedSpatialAnchor(
    uint32_t                        dataCount,
    const char*                     data);

#ifdef __cplusplus
}
#endif
#include "close_code.h"

#endif /* wvr_anchor_h_ */
