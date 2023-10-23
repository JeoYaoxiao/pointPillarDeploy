/*
 * Copyright (c) 2017 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.
 *
 * These coded instructions, statements, and computer programs are the
 * copyrighted works and confidential proprietary information of Cadence Design
 * Systems Inc. They may be adapted and modified by bona fide purchasers for
 * internal use, but neither the original nor any adapted or modified version
 * may be disclosed or distributed to third parties in any manner, medium, or
 * form, in whole or in part, without the prior written consent of Cadence
 * Design Systems Inc or in accordance with the terms of your Design Technology
 * License Agreement with Cadence Design Systems. This software and its
 * derivatives are to be executed solely on products incorporating a Cadence
 * Design Systems processor.
 */

#ifndef VCK_VX_H
#define VCK_VX_H

/* This is the interface for vendor custom kernel code on the DSP side */

#include <VX/vx.h>
#include <VX/vx_ext_cadence.h>

#ifdef  __cplusplus
extern "C" {
#endif

/* ================================= */
/*    Kernel library registration    */
/* ================================= */

/*! \brief Prototype of pointer to DSP kernel callbacks.
 * All DSP kernel callback functions (processing, start and end) must comply to this interface.
 * \ingroup group_user_kernels
 * returns the address to the callback function.
 * \param [in] kernel_callback_id ID of the requested callback.
 * \return The address of the requested callback function.
 * \retval NULL in case the ID is invalid.
 * \ingroup group_user_kernels
 */
typedef vx_status (* vck_vx_kernel_f) (void **args);

/*! \brief Prototype of pointer to DSP kernel library registration callback functions.
 * A kernel library registration function takes as input the ID of the requested callback
 * function belonging to a kernel of this library (processing, start and end) and
 * returns the address to the callback function.
 * Callback parameter types are:
 * - vck_vxImage for VX_TYPE_IMAGE parameters,
 * - vck_vxTile for TENVX_TYPE_TILABLE_IMAGE parameters,
 * - vck_vxArray for VX_TYPE_ARRAY parameters.
 * - For scalar parameters (VX_TYPE_SCALAR) a C scalar corresponding to the scalar type is
 *   passed as parameter to the DSP callback (vx_uint8, vx_uint16, vx_int16, etc.).
 * \param [in] args Array of pointers to the callback parameters. Parameters are provided
 * in the same order they are declared at host kernel registration. Parameter indexes
 * must therefore be the same on OpenVX host side and on the DSP callback side.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \retval VX_SUCCESS No errors; any other value indicates failure.
 * \ingroup group_user_kernels
 */
typedef vck_vx_kernel_f (* vck_vx_dispatch_kernels_f) (vx_enum kernel_callback_id);

/*! \brief Kernel library registration function.
 * It registers a kernel library dispatch function for a given library ID. A single
 * dispatch function can be registered per library ID.
 * Note: A dispatch function was preferred to a table in order to save on-chip
 * DSP memory.
 * \param [in] library_id ID of the kernel library. The library ID must be consistent
 * with the ID used for host kernel registration.
 * \param [in] dispatch_func address of the dispatch function for the kernel library.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \retval VX_SUCCESS No errors; any other value indicates failure.
 * \ingroup group_user_kernels
 */
vx_status vck_vxRegisterDispatchCallback (vx_enum library_id,
                                          vck_vx_dispatch_kernels_f dispatch_func);


/* ================================= */
/*         Node local buffers        */
/* ================================= */

/*! \brief Get the address of the first node local buffer.
 * This buffer relates to VX_KERNEL_LOCAL_DATA_SIZE and VX_NODE_LOCAL_DATA_SIZE
 * attributes, and is always allocated in the on-chip DSP local memory.
 * Notes:
 * - This function can be called in all kernel callbacks: process, start, end.
 * - This local buffer is persistent across the entire node execution and all kernel
 *   calls related to a single node execution will receive the same buffer.
 * - This buffer is not shared between multiple node instances of the same VCK kernel.
 * \param [in] args the parameter array that the callback received
 * \param [in] num_kernel_params the number of parameters for the kernel
 * \param [out] size pointer to vx_size integer where the size (in bytes) of the
 * local buffer is returned.
 * \return The address of the first node local buffer.
 * \ingroup group_user_kernels
 */
void *vck_vxGetLocalDataPtr(void **args, vx_size num_kernel_params, vx_size *size);

/*! \brief Get the address of the second node local buffer.
 * This buffer relates to TENVX_KERNEL_LOCAL_DATA_SIZE_2 and
 * TENVX_NODE_LOCAL_DATA_SIZE_2 attributes, and is always allocated in the on-chip
 * DSP local memory.
 * Notes:
 * - This function can be called in all kernel callbacks: process, start, end.
 * - This local buffer is persistent across the entire node execution and all
 *   kernel calls related to a single node execution will receive the same buffer.
 * - This buffer is not shared between multiple node instances of the same VCK kernel.
 * \param [in] args the parameter array that the callback received
 * \param [in] num_kernel_params the number of parameters for the kernel
 * \param [out] size pointer to vx_size integer where the size (in bytes) of the
 * second local buffer is returned.
 * \return The address of the second node local buffer.
 * \ingroup group_user_kernels
 */
void *vck_vxGetLocalDataPtr2(void **args, vx_size num_kernel_params, vx_size *size);


/* ================================= */
/*            Image Object           */
/* ================================= */

/*! \brief An opaque reference to the descriptor of a ‘full image’ parameter.
 * The Image data resides in system memory.
 * \ingroup group_user_kernels
 */
typedef struct _vck_vxImage_t * vck_vxImage;


/* General getters */

/*! \brief Get the OpenVX format of the image.
 * \param [in] image the image reference.
 * \return The image format.
 * \ingroup group_user_kernels
 */
vx_enum vck_vxGetImageFormat(vck_vxImage image);

/*! \brief Get the number of planes in the image.
 * \param [in] image the image reference.
 * \return The image number of planes in the image.
 * \ingroup group_user_kernels
 */
vx_size vck_vxGetImageNumPlanes(vck_vxImage image);


/* Per plane getters */

/*! \brief Get the image plane width.
 * \param [in] image the image reference.
 * \return The width of the image plane, in pixels.
 * \ingroup group_user_kernels
 */
vx_uint32 vck_vxGetImageWidth(vck_vxImage image, vx_uint32 plane);

/*! \brief Get the image plane height.
 * \param [in] image the image reference.
 * \return The height of the image plane, in pixels.
 * \ingroup group_user_kernels
 */
vx_uint32 vck_vxGetImageHeight(vck_vxImage image, vx_uint32 plane);

/*! \brief Get the pitch of an image plane.
 * The pitch is the number of bytes separating the address of the first
 * pixel of a row and the address of the first pixel of the next row.
 * \param [in] image the image reference.
 * \param [in] plane the plane index.
 * \return The pitch of the image plane, in bytes.
 * \ingroup group_user_kernels
 */
vx_size vck_vxGetImagePitch(vck_vxImage image, vx_uint32 plane);

/*! \brief Get the address of the first pixel (i.e., the top-left) of an image plane.
 * The memory layout follows a row major ordering.
 * Notes:
 * - This address is independent from the valid region of the image, as it points
 *   to the entire image buffer.
 * - In case a border mode is requested for the node, no memory is allocated or
 *   initialized by the OpenVX framework. The only memory area that can be accessed
 *   is within the image bounds, excluding the potential gap between rows.
 * \param [in] image the image reference.
 * \param [in] plane the plane index.
 * \return The address of the top-left pixel of the image plane.
 * \ingroup group_user_kernels
 */
void *vck_vxGetImagePtr(vck_vxImage image, vx_uint32 plane);


/* Per plane valid region getters */

/*! \brief Get the X coordinate of the first pixel (i.e., the top-left) of
 * the valid region.
 * \param [in] image the image reference.
 * \param [in] plane the plane index.
 * \return The X coordinate of the first valid region pixels, in pixels.
 * \ingroup group_user_kernels
 */
vx_uint32 vck_vxGetImageValidX(vck_vxImage image, vx_uint32 plane);

/*! \brief Get the Y coordinate of the first pixel (i.e., the top-left) of
 * the valid region.
 * \param [in] image the image reference.
 * \param [in] plane the plane index.
 * \return The Y coordinate of the first valid region pixels, in pixels.
 * \ingroup group_user_kernels
 */
vx_uint32 vck_vxGetImageValidY(vck_vxImage image, vx_uint32 plane);

/*! \brief Get the width of the valid region.
 * \param [in] image the image reference.
 * \param [in] plane the plane index.
 * \return Width of the valid region, in pixels.
 * \ingroup group_user_kernels
 */
vx_uint32 vck_vxGetImageValidWidth(vck_vxImage image, vx_uint32 plane);

/*! \brief Get the height of the valid region.
 * \param [in] image the image reference.
 * \param [in] plane the plane index.
 * \return Height of the valid region, in pixels.
 * \ingroup group_user_kernels
 */
vx_uint32 vck_vxGetImageValidHeight(vck_vxImage image, vx_uint32 plane);

/*! \brief Get the address of the first pixel (i.e., the top-left) of the
 *  image plane’s valid region.
 * The memory layout follows a row major ordering.
 * Note:
 * - The valid region is part of the image buffer. If the valid region is
 *   smaller than the image, the pitch of the valid region will differ from
 *   the pitch of the image. It must be computed by the kernel code.
 * \param [in] image the image reference.
 * \param [in] plane the plane index.
 * \return The address of the top-left pixel of the image plane valid region
 * \ingroup group_user_kernels
 */
void *vck_vxGetImageValidPtr(vck_vxImage image, vx_uint32 plane);


/* ================================= */
/*            Tile Object            */
/* ================================= */

/*! \brief An opaque reference to the descriptor of an image tile parameter.
 * The tile data resides in on-chip DSP memory.
 * \ingroup group_user_kernels
 */
typedef struct _vck_vxTile_t * vck_vxTile;


/* General getters */

/*! \brief Get the OpenVX format of the image tile.
 * \param [in] tile the tile reference.
 * \return The image tile format.
 * \ingroup group_user_kernels
 */
vx_enum vck_vxGetTileFormat(vck_vxTile tile);

/*! \brief Get the number of planes in the image tile.
 * Note: planes are different from channels. See the OpenVX specification
 * for more details.
 * \param [in] tile the tile reference.
 * \return The image number of planes in the image tile.
 * \ingroup group_user_kernels
 */
vx_size vck_vxGetTileNumPlanes(vck_vxTile tile);


/* Per plane getters */

/*! \brief Get the tile plane width, excluding any neighborhood region.
 * \param [in] tile the tile reference.
 * \param [in] plane the plane index.
 * \return The width of the tile plane, in pixels.
 * \ingroup group_user_kernels
 */
vx_uint32 vck_vxGetTileWidth(vck_vxTile tile, vx_uint32 plane);

/*! \brief Get the tile plane height, excluding any neighborhood region.
 * \param [in] tile the tile reference.
 * \param [in] plane the plane index.
 * \return The height of the tile plane, in pixels.
 * \ingroup group_user_kernels
 */
vx_uint32 vck_vxGetTileHeight(vck_vxTile tile, vx_uint32 plane);

/*! \brief Get the address of the first pixel (i.e., the top-left) of a tile plane,
 * excluding any neighborhood region.
 * The memory layout follows a row major ordering.
 * Note: if a neighborhood is requested for the parameter
 * (TENVX_KERNEL_PARAM_TILE_NEIGHBORHOOD), then this neighborhood area is always
 * allocated around the tile area and can be accessed by the user code.
 * \param [in] tile the tile reference.
 * \param [in] plane the plane index.
 * \return The address of the top-left pixel of the tile plane.
 * \ingroup group_user_kernels
 */
void *vck_vxGetTilePtr(vck_vxTile tile, vx_uint32 plane);

/*! \brief Get the pitch of an image plane, excluding any neighborhood region.
 * The pitch is the number of bytes separating the address of the first pixel
 * of a row and the address of the first pixel of the next row.
 * \param [in] tile the tile reference.
 * \param [in] plane the plane index.
 * \return The pitch of the image plane, in bytes.
 * \ingroup group_user_kernels
 */
vx_size vck_vxGetTilePitch(vck_vxTile tile, vx_uint32 plane);


/* Image related information */

/*! \brief Get the X coordinate of the first pixel (i.e., the top-left) of
 * the tile plane in the image plane.
 * \param [in] tile the tile reference.
 * \param [in] plane the plane index.
 * \return X coordinate (in pixels) of the top-left tile plane corner, in the image plane.
 * \ingroup group_user_kernels
 */
vx_uint32 vck_vxGetTileImageX(vck_vxTile tile, vx_uint32 plane);

/*! \brief Get the Y coordinate of the first pixel (i.e., the top-left) of
 * the tile plane in the image plane.
 * \param [in] tile the tile reference.
 * \param [in] plane the plane index.
 * \return Y coordinate (in pixels) of the top-left tile plane corner, in the image plane.
 * \ingroup group_user_kernels
 */
vx_uint32 vck_vxGetTileImageY(vck_vxTile tile, vx_uint32 plane);

/*! \brief Get the width of image plane to which belongs the tile plane.
 * \param [in] tile the tile reference.
 * \param [in] plane the plane index.
 * \return The width of the image plane, in pixels.
 * \ingroup group_user_kernels
 */
vx_uint32 vck_vxGetTileImageWidth(vck_vxTile tile, vx_uint32 plane);

/*! \brief Get the height of image plane to which belongs the tile plane.
 * \param [in] tile the tile reference.
 * \param [in] plane the plane index.
 * \return The height of the image plane, in pixels.
 * \ingroup group_user_kernels
 */
vx_uint32 vck_vxGetTileImageHeight(vck_vxTile tile, vx_uint32 plane);


/* ================================= */
/*            Array Object           */
/* ================================= */

/*! \brief An opaque reference to the descriptor of an array parameter.
 * The array item data resides in system memory or on-chip DSP memory, depending on
 * the user requests at host kernel registration time.
 * \ingroup group_user_kernels
 */
typedef struct _vck_vxArray_t * vck_vxArray;


/* Getters */

/*! \brief Get the OpenVX type of the array items.
 * \param [in] array the array reference.
 * \return The type of the array items.
 * \ingroup group_user_kernels
 */
vx_enum vck_vxGetArrayItemType(vck_vxArray array);

/*! \brief Get the size, in bytes, of the array items.
 * \param [in] array the array reference.
 * \return The size of array items, in bytes.
 * \ingroup group_user_kernels
 */
vx_size vck_vxGetArrayItemSize(vck_vxArray array);

/*! \brief Get the capacity of the array.
 * The capacity is the maximum number of items that the array can contain.
 * \param [in] array the array reference.
 * \return The capacity of the array, in items.
 * \ingroup group_user_kernels
 */
vx_size vck_vxGetArrayCapacity(vck_vxArray array);

/*! \brief Get the number of items in the array.
 * \param [in] array the array reference.
 * \return The number of items in the array.
 * \ingroup group_user_kernels
 */
vx_size vck_vxGetArrayNumItems(vck_vxArray array);

/*! \brief Get the address of the first item of the array.
 * The memory layout of array is the following:
 * - Array items are placed one after the other in memory following their
 *   index in the array
 * - There is no gap between array items
 * - The array is by default in system memory. It is placed in on-chip local
 *   memory by the OpenVX implementation only when TENVX_KERNEL_PARAM_REQ_LOCAL
 *   was set for the parameter
 * \param [in] array the array reference.
 * \return The address of the first item of the array.
 * \ingroup group_user_kernels
 */
void *vck_vxGetArrayPtr(vck_vxArray array);


/* Setters */

/*! \brief Set the new number of items in the array.
 * The number of items must be lower or equal to the array capacity.
 * \param [in] array the array reference.
 * \param [in] num_items The new number of items in the array.
 * \return A <tt>\ref vx_status_e</tt> enumeration.
 * \retval VX_SUCCESS No errors; any other value indicates failure.
 * \ingroup group_user_kernels
 */
vx_status vck_vxSetArrayNumItems(vck_vxArray array, vx_size num_items);


/* ================================= */
/*         XI library interop        */
/* ================================= */

/* It is required to additionally include xi_api.h for exploiting the VCK-XI interop */

/* XI library abstract structs for compiling the header without including xi_api.h */
struct xiFrameStruct; /* typedef struct xiFrameStruct xi_frame */
struct xiTileStruct;  /* typedef struct xiTileStruct xi_tile */
struct xiArrayStruct; /* typedef struct xiArrayStruct xi_array */

/* XI interop API */

/*! \brief Get the address of an XI library tile structure that describes the VCK image plane.
 * This structure belongs to the OpenVX implementation and the caller must not attempt to
 * delete the structure.
 * Note: xi_tile is returned instead of xi_frame, because xi_tile can carry in addition the
 * information about the image valid region. Full image XI library functions then use xi_tile
 * for full images, not xi_frame.
 * \param [in] image the VCK image reference.
 * \param [in] plane the plane index.
 * \return A pointer to the XI library tile structure.
 * \retval NULL an error occurred.
 * \ingroup group_user_kernels
 */
struct xiTileStruct *vck_xiGetFrame(vck_vxImage image, vx_uint32 plane);

/*! \brief Get the address of a XI library tile structure that describes the VCK tile plane.
 * This structure belongs to the OpenVX implementation and the caller must not attempt to
 *  delete the structure.
 * \param [in] tile the VCK tile reference.
 * \param [in] plane the plane index.
 * \return A pointer to the XI library tile structure.
 * \retval NULL an error occurred.
 * \ingroup group_user_kernels
 */
struct xiTileStruct *vck_xiGetTile(vck_vxTile tile, vx_uint32 plane);

/*! \brief Get the address of a XI library array structure that describes the VCK tile plane.
 * This structure belongs to the OpenVX implementation and the caller must not attempt to
 * delete the structure.
 * Notes:
 * - In the XI library, xiArray can describe a 2D object
 * - xi_array is smaller than the xi_tile. Many XI library functions can work with either
 *   xi_array or xi_tile, as xi_tile is binary compatible with xi_array.
 * - In addition to the information provided by xi_array, xi_tile provides information about
 *   the tile position in the image as well as a pointer to the image buffer.
 * \param [in] tile the VCK tile reference.
 * \param [in] plane the plane index.
 * \return A pointer to the XI library array structure.
 * \retval NULL an error occurred.
 * \ingroup group_user_kernels
 */
struct xiArrayStruct *vck_xiGetTileAsArray(vck_vxTile tile, vx_uint32 plane);


/*! \brief Get the address of a XI library array structure that describes the VCK array.
 * This structure belongs to the OpenVX implementation and the caller must not attempt to
 * delete the structure.
 * \param [in] array the VCK array reference.
 * \return A pointer to the XI library array structure.
 * \retval NULL an error occurred.
 * \ingroup group_user_kernels
 */
struct xiArrayStruct *vck_xiGetArray(vck_vxArray array);


#ifdef  __cplusplus
}
#endif

#endif // VCK_VX_H
