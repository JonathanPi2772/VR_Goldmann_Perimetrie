// "WaveVR SDK
// © 2022 HTC Corporation. All Rights Reserved.
//
// Unless otherwise required by copyright law and practice,
// upon the execution of HTC SDK license agreement,
// HTC grants you access to and use of the WaveVR SDK(s).
// You shall fully comply with all of HTC’s SDK license agreement terms and
// conditions signed by you and all SDK and API requirements,
// specifications, and documentation provided by HTC to You."


#ifndef wvr_scene_h_
#define wvr_scene_h_

#include "wvr_stdinc.h"
#include "wvr_types.h"

#include "begin_code.h"
#ifdef __cplusplus
extern "C" {
#endif

#define WVR_MAX_SEMANTIC_LABEL_NAME_SIZE 256    /**< maximum size for semantic label name */

typedef struct WVR_SemanticLabelName {
    char    name[WVR_MAX_SEMANTIC_LABEL_NAME_SIZE];   /**< The semantic name of the scene component */
} WVR_SemanticLabelName;

/**
 * @brief Function is used to start the scene and anchor features.
 *
 * This API will start scene device service to manage the system resource of the scene perception and anchors.
 *
 * @retval WVR_Success Start scene successfully.
 * @retval others @ref WVR_Result mean failure.
 * @version API Level 11
*/
extern WVR_EXPORT WVR_Result WVR_StartScene();

/**
 * @brief Function is used to stop scene and anchor features.
 *
 * This API will stop operations related to scene perception and anchors.
 *
 * @version API Level 11
*/
extern WVR_EXPORT void WVR_StopScene();

/**
 * @brief enumerate different scene perception types.
 */
typedef enum {
    WVR_ScenePerceptionTarget_2dPlane       = 0,           /**< Specifies to get 2d plane data. */
    WVR_ScenePerceptionTarget_3dObject      = 1,           /**< Specifies to get 3d object data. */
    WVR_ScenePerceptionTarget_SceneMesh     = 2,           /**< Specifies to get scene meshes of surrounding environment */
    WVR_ScenePerceptionTarget_Max           = 0x7FFFFFFF
} WVR_ScenePerceptionTarget;

/**
 * @brief enumerate different scene mesh types.
 */
typedef enum {
    WVR_SceneMeshType_VisualMesh             = 0,           /**< Specifies to get visualization meshes. */
    WVR_SceneMeshType_ColliderMesh           = 1,           /**< Specifies to get collider meshes. */
    WVR_SceneMeshType_Max                    = 0x7FFFFFFF
} WVR_SceneMeshType;
/**
 * @brief Function is used to start scene perception with the target parameter.
 *
 * This API will start scene perception related to the input perception target. Developers can use corresponding APIs to
 * get related data. For example, when developers start scene perception with WVR_ScenePerceptionTarget_2dPlane, they can get 2dPlane
 * data by calling @ref WVR_GetScenePlanes API. Otherwise, if the WVR_ScenePerceptionTarget_2dPlane is stopped, the underlying
 * related resources will be released.
 *
 * @param target the perception target that developers would like to use. (refer to @ref WVR_ScenePerceptionTarget)
 * @retval WVR_Success Start scene perception successfully.
 * @retval others @ref WVR_Result mean failure.
 * @version API Level 11
*/
extern WVR_EXPORT WVR_Result WVR_StartScenePerception(WVR_ScenePerceptionTarget target);

/**
 * @brief Function is used to stop scene perception.
 *
 * This API will stop perception related to input perception target and release the underlying related resources.
 *
 * @param target the perception target that developers don't use. (refer to @ref WVR_ScenePerceptionTarget)
 * @retval WVR_Success Stop scene perception successfully.
 * @retval others @ref WVR_Result mean failure.
 * @version API Level 11
*/
extern WVR_EXPORT WVR_Result WVR_StopScenePerception(WVR_ScenePerceptionTarget target);

/**
 * @brief enumerate different scene perception state
 */
typedef enum {
    WVR_ScenePerceptionState_Empty     = 0,          /**< indicates that a perception target is not perceived */
    WVR_ScenePerceptionState_Observing = 1,          /**< indicates that a perception target is observing */
    WVR_ScenePerceptionState_Paused    = 2,          /**< indicates that a perception target is paused */
    WVR_ScenePerceptionState_Completed = 3,          /**< indicates that a perception target is completed */
    WVR_ScenePerceptionState_Max       = 0x7FFFFFFF
} WVR_ScenePerceptionState;

/**
 * @brief Function is used to get the corresponding perception target state.
 *
 * Developers can inspect the completion of the scene perception by polling @ref WVR_GetScenePerceptionState.
 * Check state (refer to @ref WVR_ScenePerceptionState) is WVR_ScenePerceptionState_Completed to know the result is ready.
 *
 * @param target input perception target , refer to @ref WVR_ScenePerceptionTarget
*  @param state the returned @ref WVR_ScenePerceptionState value
 * @retval WVR_Success Get scene perception state successfully.
 * @retval others @ref WVR_Result mean failure.
 * @version API Level 11
*/
extern WVR_EXPORT WVR_Result WVR_GetScenePerceptionState(
    WVR_ScenePerceptionTarget      target,
    WVR_ScenePerceptionState*      state);

/**
 * @brief The scene plane label
 */
typedef enum {
    WVR_ScenePlaneLabel_Unknown    = 0,            /**< This scene plane has yet to be classified and assigned a type */
    WVR_ScenePlaneLabel_Floor      = 1,            /**< This scene plane has classified and assigned as floor */
    WVR_ScenePlaneLabel_Ceiling    = 2,            /**< This scene plane has classified and assigned as ceiling */
    WVR_ScenePlaneLabel_Wall       = 3,            /**< This scene plane has classified and assigned as wall */
    WVR_ScenePlaneLabel_Desk       = 4,            /**< This scene plane has classified and assigned as desk */
    WVR_ScenePlaneLabel_Couch      = 5,            /**< This scene plane has classified and assigned as couch */
    WVR_ScenePlaneLabel_Door       = 6,            /**< This scene plane has classified and assigned as door */
    WVR_ScenePlaneLabel_Window     = 7,            /**< This scene plane has classified and assigned as window */
    WVR_ScenePlaneLabel_Stage      = 8,            /**< This scene plane has classified and assigned as stage */
    WVR_ScenePlaneLabel_Max        = 0x7FFFFFFF    /**< meaning no filer and all planes are returned */
} WVR_ScenePlaneLabel;

/**
 * @brief The scene plane type
 */
typedef enum {
    WVR_ScenePlaneType_Unknown                  = 0,          /**< This planes have yet to be classified and assigned to unknown plane type */
    WVR_ScenePlaneType_HorizontalUpwardFacing   = 1,          /**< horizontal planes that point upward facing will be detected  */
    WVR_ScenePlaneType_HorizontalDownwardFacing = 2,          /**< horizontal planes that point downward facing will be detected  */
    WVR_ScenePlaneType_Vertical                 = 3,          /**< vertical planes will be detected  */
    WVR_ScenePlaneType_Max                      = 0x7FFFFFFF  /**< meaning no filer and all planes are returned */
} WVR_ScenePlaneType;

/**
 * @brief scene mesh buffer identifier
 *
 * Specifies the scene mesh buffer to read.
 */
typedef struct WVR_SceneMesh {
    uint64_t    meshBufferId;
} WVR_SceneMesh;

/**
 * @brief the 2D size of the plane’s extent
 *
 * The 2D size of the plane’s extent, where width is the width of the plane along the X axis and height
 * is the height of the plane along the Y axis.
 */
typedef struct WVR_Extent2Df {
    float    width;                         /**< the width of the plane along the X axis */
    float    height;                        /**< the height of the plane along the Y axis */
} WVR_Extent2Df;

/**
 * @brief the scene plane information from @ref WVR_GetScenePlanes API
 *
 * The @ref WVR_ScenePlane structure represents these information related to a scene plane.
 *
 * Note: Starting from API level 16, developers can retrieve @ref WVR_SemanticLabelName of WVR_ScenePlanes
 * from WVR_GetScenePlanes API.
 */
typedef struct WVR_ScenePlane {
    WVR_Uuid                        uuid;         /**< the plane's uuid */
    WVR_Uuid                        parentUuid;   /**< the plane's parentUuid */
    uint64_t                        meshBufferId; /**< the uint64_t identifier that specifies the mesh buffer of this plane */
    WVR_Pose_t                      pose;         /**< the plane's pose */
    WVR_Extent2Df                   extent;       /**< the 2D size of the plane’s extent */
    WVR_ScenePlaneType              planeType;    /**< the plane's type, refer to @ref WVR_ScenePlaneType */
    WVR_ScenePlaneLabel             planeLabel;   /**< the plane's label, refer to @ref WVR_ScenePlaneLabel */
    WVR_SemanticLabelName           semanticName; /**< the semantic name of the plane, refer to @ref WVR_SemanticLabelName*/
} WVR_ScenePlane;

/**
 * @brief the scene plane filter comprised @ref WVR_ScenePlaneType and @ref WVR_ScenePlaneLabel.
 *
 * Developers use the @ref WVR_ScenePlaneFilter structure to filter the scene planes.
 */
typedef struct WVR_ScenePlaneFilter {
    WVR_ScenePlaneType      planeType;        /**< the plane's type, refer to @ref WVR_ScenePlaneType */
    WVR_ScenePlaneLabel     planeLabel;       /**< the plane's label, refer to @ref WVR_ScenePlaneLabel */
} WVR_ScenePlaneFilter;

/**
 * @brief Function is used to retrieve scene planes.
 *
 * This is two calls API. Developers should call this API with the value of planeCapacityInput equals to 0 to retrieve
 * the size of planes from planeCountOutput. Then developers allocate the array of @ref WVR_ScenePlane data and
 * assign the planeCapacityInput and call the API in the second time. Then runtime will fill the @ref WVR_ScenePlane array.
 * Note: that only WVR_PoseOriginModel_OriginOnHead and WVR_PoseOriginModel_OriginOnGround cases are supported in originModel.
 *
 * @param planeFilter Filter the scene planes which meeting the filter criteria. Developers set nullptr if no need filter, refer to @ref WVR_ScenePlaneFilter.
 * @param planeCapacityInput the capacity of the planes array, or 0 to indicate a request to retrieve the required capacity.
 * @param planeCountOutput a pointer to the count of planes written, or a pointer to the required capacity in the case that planeCapacityInput is insufficient.
 * @param originModel Only WVR_PoseOriginModel_OriginOnHead and WVR_PoseOriginModel_OriginOnGround are supported, refer to @ref WVR_PoseOriginModel.
 * @param planes An array of @ref WVR_ScenePlane will be filled by the runtime.
 * @retval WVR_Success Get scene planes successfully.
 * @retval others @ref WVR_Result mean failure.
 * @version API Level 11
 */
extern WVR_EXPORT WVR_Result WVR_GetScenePlanes(
    const WVR_ScenePlaneFilter*             planeFilter,
    uint32_t                                planeCapacityInput,
    uint32_t*                               planeCountOutput,
    WVR_PoseOriginModel                     originModel,
    WVR_ScenePlane*                         planes);

/**
 * @brief Function is used to retrieve scene meshes after perception process is finished.
 *
 * This is two calls API. Developers should call this API with the value of meshCapacityInput equals to 0 to retrieve
 * the size of meshes from meshCountOutput. Then developers allocate the array of @ref WVR_SceneMesh and assign the
 * meshCapacityInput and call the API in the second time. Then runtime will fill the meshes array.
 *
 * @param meshType input scene mesh type, refer to @ref WVR_SceneMeshType
 * @param meshCapacityInput the capacity of meshes array, or 0 to indicate a request to retrieve the required capacity
 * @param meshCountOutput a pointer to the count of meshes written, or a pointer to the required capacity in the case that meshCapacityInput
 * @param meshes An array of @ref WVR_SceneMesh will be filled by the runtime.
 * @retval WVR_Success Get meshes successfully.
 * @retval others @ref WVR_Result mean failure.
 * @version API Level 11
 */
extern WVR_EXPORT WVR_Result WVR_GetSceneMeshes(
    WVR_SceneMeshType                   meshType,
    uint32_t                            meshCapacityInput,
    uint32_t*                           meshCountOutput,
    WVR_SceneMesh*                      meshes);

/**
 * @brief the scene mesh information from @ref WVR_GetSceneMeshes API
 *
 * The @ref WVR_SceneMeshBuffer structure represents the related information related to a scene mesh.
 * The indexBuffer is an array of triangle indices filled in by the runtime. The triangle indices must
 * be returned in counter-clockwise order and three indices denote one triangle.
 */
typedef struct WVR_SceneMeshBuffer {
    uint32_t                    vertexCapacityInput;   /**< the capacity of the vertexBuffer array, or 0 to indicate a request to retrieve the required capacity. */
    uint32_t                    vertexCountOutput;     /**< a pointer to the count of vertices */
    WVR_Vector3f*               vertexBuffer;          /**< an array of @ref WVR_Vector3f filled in by the runtime returns the position of vertices in the mesh component’s space. */
    uint32_t                    indexCapacityInput;    /**< the capacity of the indexBuffer array, or 0 to indicate a request to retrieve the required capacity. */
    uint32_t                    indexCountOutput;      /**< a pointer to the count of index */
    uint32_t*                   indexBuffer;           /**< an array of triangle indices filled in by the runtime, specifying the indices of the scene mesh buffer in the vertices array. */
} WVR_SceneMeshBuffer;

/**
 * @brief Function is used to retrieves the scene mesh vertex buffer and index buffer for the given scene mesh buffer identifier.
 *
 * This is two calls API. Developers should call this API with the value of vertexCapacityInput/indexCapacityInput
 * (@ref WVR_SceneMeshBuffer) equals to 0 to retrieve the size of vertexBuffer/indexBuffer from vertexCountOutput/indexCountOutput.
 * Then developers allocate the array of vertexBuffer/indexBuffer and call the API in the second time.
 * Then runtime will fill these two arrays.
 *
 * Note: This API may cause lots of memory copy, if you call the first time, please set the vertexCapacityInput/indexCapacityInput equals to 0,
 * and let vertexBuffer/indexBuffer equals to nullptr, then runtime will just fill the vertexCountOutput/indexCountOutput size.
 *
 * @param meshBufferId the scene mesh buffer identifier
 * @param originModel Only WVR_PoseOriginModel_OriginOnHead and WVR_PoseOriginModel_OriginOnGround are supported, refer to @ref WVR_PoseOriginModel.
 * @param sceneMeshBuffer the scene mesh vertex buffer and index buffer, refer to @ref WVR_SceneMeshBuffer
 * @retval WVR_Success Get scene mesh buffer successfully.
 * @retval others @ref WVR_Result mean failure.
 * @version API Level 11
 */
extern WVR_EXPORT WVR_Result WVR_GetSceneMeshBuffer(
    uint64_t                    meshBufferId,
    WVR_PoseOriginModel         originModel,
    WVR_SceneMeshBuffer*        sceneMeshBuffer);

/**
 * @brief the 3D size of the object’s extent
 *
 * The 3D size of the object’s extent, where width is the width of the object along the X axis, height
 * is the height of the object along the Y axis and depth is the depth of the object along the Z axis.
 */
typedef struct WVR_Extent3Df {
    float    width;          /**< the width of the object along the X axis */
    float    height;         /**< the height of the object along the Y axis */
    float    depth;          /**< the depth of the object along the Z axis */
} WVR_Extent3Df;

/**
 * @brief the scene object information from @ref WVR_GetSceneObjects API
 *
 * The @ref WVR_SceneObject structure represents these information related to a scene object.
 */
typedef struct WVR_SceneObject {
    WVR_Uuid                        uuid;         /**< the object's uuid */
    WVR_Uuid                        parentUuid;   /**< the object's parentUuid */
    uint64_t                        meshBufferId; /**< the uint64_t identifier that specifies the mesh buffer of this object */
    WVR_Pose_t                      pose;         /**< the object's center pose */
    WVR_Extent3Df                   extent;       /**< the 3D size of the object’s extent */
    WVR_SemanticLabelName           semanticName; /**< the semantic name of the object, refer to @ref WVR_SemanticLabelName*/
} WVR_SceneObject;

/**
 * @brief Function is used to retrieve scene objects.
 *
 * This is two calls API. Developers should call this API with the value of objectCapacityInput equals to 0 to retrieve
 * the size of objects from objectCountOutput. Then developers allocate the array of @ref WVR_SceneObject data and
 * assign the objectCapacityInput and call the API in the second time. Then runtime will fill the @ref WVR_SceneObject array.
 * Note that only WVR_PoseOriginModel_OriginOnHead and WVR_PoseOriginModel_OriginOnGround cases are supported in originModel.
 *
 * @param objectCapacityInput the capacity of the objects array, or 0 to indicate a request to retrieve the required capacity.
 * @param objectCountOutput a pointer to the count of objects written, or a pointer to the required capacity in the case that objectCapacityInput is insufficient.
 * @param originModel Only WVR_PoseOriginModel_OriginOnHead and WVR_PoseOriginModel_OriginOnGround are supported, refer to @ref WVR_PoseOriginModel.
 * @param objects An array of @ref WVR_SceneObject will be filled by the runtime.
 * @retval WVR_Success Get scene objects successfully.
 * @retval others @ref WVR_Result mean failure.
 * @version API Level 16
 */
extern WVR_EXPORT WVR_Result WVR_GetSceneObjects(
    uint32_t                                objectCapacityInput,
    uint32_t*                               objectCountOutput,
    WVR_PoseOriginModel                     originModel,
    WVR_SceneObject*                        objects);


#ifdef __cplusplus
}
#endif
#include "close_code.h"

#endif /* wvr_scene_h_ */
