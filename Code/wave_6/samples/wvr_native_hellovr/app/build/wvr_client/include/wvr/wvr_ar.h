// "WaveVR SDK
// © 2019 HTC Corporation. All Rights Reserved.
//
// Unless otherwise required by copyright law and practice,
// upon the execution of HTC SDK license agreement,
// HTC grants you access to and use of the WaveVR SDK(s).
// You shall fully comply with all of HTC’s SDK license agreement terms and
// conditions signed by you and all SDK and API requirements,
// specifications, and documentation provided by HTC to You."


#ifndef wvr_ar_h_
#define wvr_ar_h_

#include "wvr_types.h"
#include "wvr_stdinc.h"
#include "wvr_render.h"

// TODO: move some struct of camera into wvr_types.h
#include "wvr_camera.h"

#include "begin_code.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    WVR_ARPlaneType_Invalid                  = 0,  // The plane type is invalid.
    WVR_ARPlaneType_HorizontalDownwardFacing = 1,  // A horizontal plane facing downward (for example a ceiling).
    WVR_ARPlaneType_HorizontalUpwardFacing   = 2,  // A horizontal plane facing upward (for example a floor or tabletop).
    WVR_ARPlaneType_Vertical                 = 3   // A vertical plane (for example a wall).
} WVR_ARPlaneType;

typedef enum {
    WVR_ARTrackingState_Invalid  = 0,  // The tracking state is invalid.
    WVR_ARTrackingState_Tracking = 1,  // The object is currently tracked and its pose is current.
    WVR_ARTrackingState_Paused   = 2,  // This can happen if device tracking is lost, if the user enters a new space, or if the Session is currently paused.
    WVR_ARTrackingState_Stopped  = 3   // AR device has stopped tracking this Trackable and will never resume tracking it.
} WVR_ARTrackingState;

typedef struct WVR_ARPlane {
    int64_t             identity;
    WVR_ARTrackingState trackingState;
    WVR_ARPlaneType     type;
    WVR_Pose_t          center;
    float               polygon[40];   // Returns the 2D vertices of a convex polygon approximating the detected plane, in the form [x1, z1, x2, z2, ...]
    uint32_t            polygonCount;
    float               extentX;
    float               extentZ;
} WVR_ARPlane_t;

typedef struct WVR_ARAnchor {
    int64_t             identity;       // Use to identify each anchor
    WVR_ARTrackingState trackingState;
    WVR_Pose_t          pose;
} WVR_ARAnchor_t;

typedef struct WVR_ARHitResult {
    WVR_Pose_t     hitPose;
    WVR_ARPlane_t  hitPlane;
} WVR_ARHitResult_t;

typedef enum {
    WVR_ARCoordinates2dType_ImageNormalized                   = 0,
    WVR_ARCoordinates2dType_ImagePixels                       = 1,
    WVR_ARCoordinates2dType_OpenGlNormalizedDeviceCoordinates = 2,
    WVR_ARCoordinates2dType_TextureNormalized                 = 3,
    WVR_ARCoordinates2dType_TexturePixels                     = 4,
    WVR_ARCoordinates2dType_View                              = 5,
    WVR_ARCoordinates2dType_ViewNormalized                    = 6
} WVR_ARCoordinates2dType;


extern WVR_EXPORT WVR_Result WVR_StartAR();
extern WVR_EXPORT WVR_Result WVR_StopAR();

extern WVR_EXPORT void WVR_SetARCamTextureId(WVR_Eye eye, WVR_TextureTarget target, uint32_t textureId);



extern WVR_EXPORT WVR_Matrix4f WVR_GetARCamProjectionMatrix(WVR_Eye eye, float near, float far);

extern WVR_EXPORT WVR_Matrix4f_t WVR_GetARCamProjectionMatrixTest(float fx, float fy, float cx, float cy, float near, float far, float width, float height, int rotation, bool forOneEye);

extern WVR_EXPORT WVR_Result WVR_UpdateARFrame();

extern WVR_EXPORT void WVR_GetARFrameTimestamp(int64_t *outTimestamp);

extern WVR_EXPORT void WVR_SetDisplayGeometry(int32_t rotation, int32_t width, int32_t height);

extern WVR_EXPORT void WVR_TransformARCoordinate2D(WVR_ARCoordinates2dType inputTypeCoordinates, int32_t numberOfVertices, const float *vertices2d,
                                                                WVR_ARCoordinates2dType outputTypeCoordinates, float *outVertices2d);

extern WVR_EXPORT uint32_t WVR_GetAllARPlanes(WVR_ARPlane_t *outPlanes, uint32_t size);

extern WVR_EXPORT uint32_t WVR_GetAllARAnchors(WVR_ARAnchor_t *outAnchors, uint32_t size);

/**
 * @brief Function to do hit test and return result array.
 *
 * We use this function to get which planes were hit by this ray.
 * If there is no plane be hit, (*outHitResults) will be set nullptr and (*hitResultSize) be set 0. 
 *
 * @param rayOrigin3 Origin of this ray. It's position.
 * @param rayDirection3 Direction of this ray. It's vector.
 * @param outHitResults The pointer for storing allocated hit result array. Please note that (*outHitResults) should be nullptr before use it.
 * @param hitResultSize The pointer for storing number of allocated hit result array.
 *
 * @retval WVR_Success All input parameters are legal. (Even if we hit no plane by this ray.)
 * @retval WVR_Error_InvalidArgument Any parameter is nullptr. Or (*outHitResults) isn't nullptr.
*/
extern WVR_EXPORT WVR_Result WVR_ARHitTestRay(const WVR_Vector3f_t *rayOrigin3,
    const WVR_Vector3f_t* rayDirection3, WVR_ARHitResult_t **outHitResults, uint32_t *hitResultSize);


/**
 * @brief Function to release hit result array created by API WVR_ARHitTestRay.
 *
 * @param iTargetHitResultArray The pointer point hit result array.
 * @param iHitResultSize The number about this hit result array.
 *
 */
extern WVR_EXPORT void WVR_DestroyARHitResultArray(WVR_ARHitResult_t **iTargetHitResultArray, uint32_t iHitResultSize);

extern WVR_EXPORT WVR_Result WVR_GetARAnchorByARHitResult(const WVR_ARHitResult_t *hitResult,  WVR_ARAnchor_t *outAnchor);
extern WVR_EXPORT WVR_Result WVR_DetachARAnchor(const WVR_ARAnchor_t *anchor);

extern WVR_EXPORT bool WVR_SupportARCameraImage();

#ifdef __cplusplus
} /* extern "C" */
#endif
#include "close_code.h"

#endif /* wvr_ar_h_ */
