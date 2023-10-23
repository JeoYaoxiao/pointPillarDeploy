/*!
 *  This file contains proprietary information that is the sole intellectual
 *  property of Black Sesame Technologies, Inc. and its affiliates. No portions
 *  of this material may be reproduced in any form without the written
 *  permission of:
 * 
 *  Black Sesame Technologies, Inc. and its affiliates
 *  2255 Martin Ave. Suite D
 *  Santa Clara, CA  95050
 *  Copyright \@2018: all right reserved.
 *
 *  Notice:
 *
 *  You are running an EVALUATION distribution of the neural network tools
 *  provided by Black Sesame Technologies, Inc. under NDA. This copy may NOT be
 *  used in production or distributed to any third party. For distribution or
 *  production, further Software License Agreement is required.
 *
 * @file
 *  lwnn_user_api.h
 *
 * @brief
 *  This file contains definitions for LWNN user API.
 *
 * @author
 *  Shichen Lin
 */

#ifndef _LWNN_USER_API_H
#define _LWNN_USER_API_H

#ifdef __cplusplus
extern "C" {
#endif

/* pre-allocated */
#define LWNN_MAX_IN_TS                    8     //!< the maximum input tensor number
#define LWNN_MAX_OUT_TS                   32    //!< the maximum output tensor number

#define LWNN_MAX_NAME_LEN                 256   //!< the maximum name length of the model

#define LWNN_MAX_IN_BUFFER_Q_SIZE         10    //!< the maximum input buffer queue size
#define LWNN_MAX_OUT_BUFFER_Q_SIZE        10    //!< the maximum output buffer queue size

/* tensor dimension constants */
#define BST_LWNN_PITCH_DIM                4     //!< the number of tensor pitch dimensions
#define BST_LWNN_SIZE_DIM                 4     //!< the number of tensor size dimensions

#define BST_LWNN_MAX_DSP_NUM              4     //!< the maximum number of CV DSPs

/*!
 * @enum The error code returned by the LWNN API.
 */
typedef enum {
  LWNN_SUCC                              = 0,   //!< execution success.
  LWNN_ERR_FAIL                          = 1,   //!< execution failure.
  LWNN_ERR_DEVICE_UNAVAILABLE            = 2,   //!< device unavailable.
  LWNN_ERR_FREE_FAIL                     = 3,   //!< dma buffer free failure.
  LWNN_ERR_MALLOC_FAIL                   = 4,   //!< dma buffer malloc failure.
  LWNN_ERR_SYNC_FAIL                     = 5,   //!< dma buffer sync failure.
  LWNN_ERR_PARAM_INVALID                 = 6,   //!< invalid input parameter.
  LWNN_ERR_MODEL_INVALID                 = 7,   //!< invalid model.
  LWNN_INPUT_QUEUE_EMPTY                 = 9,   //!< instance input queue empty.
  LWNN_OUTPUT_QUEUE_EMPTY                = 10,  //!< instance output queue empty.
  LWNN_INPUT_QUEUE_FULL                  = 11,  //!< instance input queue full.
  LWNN_OUTPUT_QUEUE_FULL                 = 12,  //!< instance output queue full.
  LWNN_ERR_DMA_BUF_IMPORT_FAIL           = 13,  //!< dma-buf import failure.
} lwnn_ret_t;


/*!
 * @enum The deprecated instance status enum.
 */
typedef enum {
  LWNN_INS_INACTIVE = 0,
  LWNN_INS_ACTIVE, 
  LWNN_INS_MAX
} lwnn_instance_status_t;


/*!
 * @enum The deprecated model status enum.
 */
typedef enum {
  LWNN_MODEL_INACTIVE = 0,
  LWNN_MODEL_ACTIVE,
  LWNN_MODEL_MAX
} lwnn_model_status_t;


/*!
 * @enum The CV DSP index enum. TODO.
 */
typedef enum {
  LWNN_CV_DSP_0 = 0,
  LWNN_CV_DSP_1,
  LWNN_CV_DSP_2,
  LWNN_CV_DSP_3
} lwnn_dsp_idx_t;


/*!
 * @enum The LWNN print level type.
 */
typedef enum {
  LWNN_NO_PRINT = 0,
  LWNN_LOG_PRINT,
  LWNN_DEBUG_PRINT
} lwnn_print_level_t;


/*!
 * @enum The tensor data type.
 */
typedef enum {
  LWNN_TENSOR_INT8          = 0,  /* data type is int8. */
  LWNN_TENSOR_INT16         = 1,  /* data type is int16. */
  LWNN_TENSOR_FP16          = 2,  /* data type is float16. */
  LWNN_TENSOR_UINT8         = 3,  /* data type is uint8. */
  LWNN_TENSOR_FP32          = 4,  /* data type is float32. */
  LWNN_TENSOR_UINT16        = 5,  /* data type is uint16. */
  LWNN_TENSOR_DTYPE_MAX
} lwnn_tensor_dtype_t;


/*!
 * @enum The tensor color type.
 */
typedef enum {
  LWNN_TENSOR_DEFAULT   = 0, // normal 4D-tensor
  LWNN_TENSOR_RGB888    = 1,
  LWNN_TENSOR_RGB888P   = 2,
  LWNN_TENSOR_RGBD      = 3,
  LWNN_TENSOR_RGBA      = 4,
  LWNN_TENSOR_YUYV      = 5,
  LWNN_TENSOR_UYVY      = 6,
  LWNN_TENSOR_NV12      = 7,
  LWNN_TENSOR_NV21      = 8,
  LWNN_TENSOR_YUV420P   = 9,
  LWNN_TENSOR_MULTI_PD  = 10,
  LWNN_TENSOR_COLORTYPE_MAX
} lwnn_tensor_ctype_t;


/*!
 * @enum The tensor data order.
 */
typedef enum {
  LWNN_TENSOR_WHD        = 0, //!< tensor data format is NCHW.
  LWNN_TENSOR_DWH        = 1, //!< tensor data format is NHWC.
  LWNN_TENSOR_DORDER_MAX
} lwnn_tensor_dorder_t;


/*! 
 * @enum The buffer queue type.
 * If nonblocking, 
 * function returns LWNN_INPUT_QUEUE_EMPTY immediately if there is no available input buffer, 
 * function returns LWNN_OUTPUT_QUEUE_EMPTY immediately if there is no available output buffer, 
 * user could re-try with/without sleeping for a period or take other actions. 
 *
 * If blocking, need to specify timeout limit
 * function returns LWNN_INPUT_QUEUE_EMPTY after timeout period if there is no available input buffer, 
 * function returns LWNN_OUTPUT_QUEUE_EMPTY after timeout period if there is no available output buffer, 
 * user could re-try with/without sleeping for a period or take other actions.
 */
typedef enum {
  LWNN_QUEUE_NONBLOCKING  = 0,
  LWNN_QUEUE_BLOCKING     = 1,
  LWNN_QUEUE_TYPE_MAX
} lwnn_queue_type_t;


/*!
 * @enum The input buffer type.
 */
typedef enum {
  LWNN_BUFFER_BUS_ADDR  = 0,
  LWNN_BUFFER_USER_ADDR = 1,
  LWNN_BUFFER_ADDR_TYPE_MAX
} lwnn_input_buffer_type_t;


/*!
 * @enum The instance configuration option type.
 */
typedef enum {
  LWNN_SET_DSP_AFFINITY,
} lwnn_ins_cfg_opt_t;


/*!
 * @enum The query command for lwnn_model_query and lwnn_query.
 */
typedef enum {
  LWNN_QUERY_IN_OUT_NUM = 0,  /* query the number of input & output tensor. */
  LWNN_QUERY_INPUT_ATTR,      /* query the attribute of input tensor. */
  LWNN_QUERY_OUTPUT_ATTR,     /* query the attribute of output tensor. */
  LWNN_QUERY_CMD_MAX
} lwnn_query_cmd_t;


/*!
 * @struct The tensor dimension.
 */
typedef struct {
  uint16_t pitch[BST_LWNN_PITCH_DIM];
  uint16_t size[BST_LWNN_SIZE_DIM];
} lwnn_tensor_dim_t;


/*!
 * @struct The tensor information exposed to users.
 */
typedef struct  {
  lwnn_tensor_dim_t     dim;        //!< tensor dimensions
  lwnn_tensor_dtype_t   dType;      //!< data type
  lwnn_tensor_dorder_t  dOrder;     //!< data order
  lwnn_tensor_ctype_t   cType;      //!< color type

  void *                user_addr;  //!< user address of the tensor buffer
} lwnn_user_tensor_t;


/*!
 * @struct The tensor meta information.
 */
typedef struct  {
  lwnn_tensor_dim_t       dim;        //!< tensor dimensions
  lwnn_tensor_dtype_t     dType;      //!< data type
  lwnn_tensor_dorder_t    dOrder;     //!< data order
  lwnn_tensor_ctype_t     cType;      //!< color type
  float                   dqnt_scale; //!< dequantization scale
  uint32_t                dsize;      //!< data unit size
  uint32_t                size;       //!< tensor buffer size
} lwnn_user_tensor_meta_t;


typedef uint32_t lwnn_model_id_t;
typedef uint32_t lwnn_instance_id_t; 


/*!
 * @struct The configuration of an instance to be created.
 */  
typedef struct {
  lwnn_queue_type_t         queue_type;         //!< the type of input/output buffer queues
  
  lwnn_input_buffer_type_t  in_type;            //!< the type of inputs that the instance accepts

  uint32_t                  input_bufferq_size;   //!< the input buffer queue size
  uint32_t                  output_bufferq_size;  //!< the output buffer queue size

  uint32_t                  in_out_timeout_period; //!< the timeout of input/output buffer queues in milisecond
} lwnn_user_instance_cfg_t;


/*!
 * @struct The input buffer information.
 */
typedef struct {
  uint32_t input_q_idx;                       //!< the index handle of the input
  void*    frame_ts_usr_addr[LWNN_MAX_IN_TS]; //!< 64-bit user addresses of input tensor buffers
  uint32_t frame_ts_bus_addr[LWNN_MAX_IN_TS]; //!< 32-bit bus addresses of output tensor buffers
  int	   dma_buf_fds[LWNN_MAX_IN_TS];		  //!< DMA-buf fd for input push use
} lwnn_user_input_t;


/*!
 * @struct The dma-buf input buffer information.
 */
typedef struct {
  int      fd;							//!< DMA buf fd of input tensor buffers
  uint32_t bus_addr;                    //!< The bus address of the buf
  uint32_t size;						//!< The size of the buf
} lwnn_user_dma_buf_t;

/*!
 * @struct The output buffer information given to users.
 */
typedef struct {
  uint32_t output_ts_num;                 //!< the output tensor number
  float    ts_qnt_scale[LWNN_MAX_OUT_TS]; //!< quantization scales of output tensors
  uint64_t ts_addr[LWNN_MAX_OUT_TS];      //!< user addresses of output tensor buffers
} lwnn_user_output_t;


/*!
 * @struct The model information given to users.
 */
typedef struct {
  lwnn_model_id_t     id;               //!< the model id

  char                model_name[LWNN_MAX_NAME_LEN];  //!< the model name defined in the meta file
  
  lwnn_model_status_t status;           //!< the deprecated model status field
  
  uint32_t            coeff_size;       //!< the coefficient buffer size
  uint32_t            fwlib_size;       //!< the rbf file size
  uint32_t            scratch_size;     //!< the scratch buffer size
  
  uint32_t            input_ts_num;     //!< the input tensor number
  uint32_t            output_ts_num;    //!< the output tensor number
} lwnn_user_model_t;


/*!
 * @struct The instance information given to users.
 */
typedef struct {
  lwnn_instance_id_t      ins_id;   //!< the instance id
  lwnn_model_id_t         model_id; //!< the model id
  
  lwnn_instance_status_t  status;   //!< the deprecated instance status field
  
  uint32_t                in_buf_queue_size;  //!< the input buffer queue size
  uint32_t                out_buf_queue_size; //!< the output buffer queue size

  uint32_t                in_ts_num;  //!< the input tensor number
  uint32_t                out_ts_num; //!< the output tensor number
} lwnn_user_instance_t;


/* query info structures (not used yet) */
/*!
 * @struct The unit version information structure.
 */
typedef struct {
  uint8_t  major;       //!< versino major number
  uint8_t  minor;       //!< versino minor number
  uint8_t  patch;       //!< versino patch number

  uint16_t year;        //!< released year
  uint8_t  month;       //!< released month
  uint8_t  day;         //!< released day
} _lwnn_version_t;


/*!
 * @struct The information of LWNN versions.
 */
typedef struct {
  /* library */
  _lwnn_version_t lib;
  
  /* driver */
  _lwnn_version_t drv;
  
  /* DSP runtime firmware */
  _lwnn_version_t fw;
} lwnn_ver_info_t;


/*!
 * @struct The information for LWNN DSPs.
 */
typedef struct {
  uint32_t  available_dsp_num;
  uint8_t   dsp_online[BST_LWNN_MAX_DSP_NUM];
} lwnn_dsp_info_t;


/*!
 * @struct The information for LWNN_QUERY_IN_OUT_NUM.
 */
typedef struct {
  uint32_t n_input;  //!< the number of input
  uint32_t n_output; //!< the number of output
} lwnn_input_output_num_t;


/*!
 * @struct The information for LWNN_QUERY_INPUT_ATTR and LWNN_QUERY_OUTPUT_ATTR.
 */
typedef struct {
  uint32_t index;                     //!< the tensor index set by caller
  uint16_t pitch[BST_LWNN_PITCH_DIM]; //!< the tensor dimension pitches
  uint16_t size[BST_LWNN_SIZE_DIM];   //!< the tensor dimension sizes
} lwnn_tensor_attr_t;


#ifdef __cplusplus
} //extern "C"
#endif


/*******************************************************************************
 ******************************************************************************/
#ifdef AARCH64
void *lwnn_memcpy_neon(uint8_t *dst, const uint8_t *src, uint64_t size);
#endif

/******************************** Core APIs ***********************************/
/*!
 * @brief                   Initialize a model by loading offlline model related
 *                          files.
 * @param[out]  p_u_model   The pointer to a preallocated model structure with
 *                          essential information for the caller.
 * @param[in]   model_path  The path of model related files: *.meta, *.rbf, *.bin
 * @return                  LWNN_SUCC - model loading success
 *                          LWNN_ERR_PARAM_INVALID - invalid input parameter(s)
 *                          LWNN_ERR_MODEL_INVALID - invalid model file(s)
 *                          LWNN_ERR_DEVICE_UNAVAILABLE - driver device open failure
 *                          LWNN_ERR_MALLOC_FAIL - DMA buffer allocation failure
 *                          LWNN_ERR_SYNC_FAIL - DMA buffer flushing failure
 *                          LWNN_ERR_FAIL - LWNN failure
 */
lwnn_ret_t lwnn_model_load(lwnn_user_model_t *p_u_model, const char *model_path);


/*!
 * @brief                   Generate an instance of an existing model. Multiple
 * @note                    instances can be created with the same model, but
 *                          each instance has its own buffer queues(each buffer
 *                          is a complete set of tensors).
 * @param[in]  p_u_model    The model that the generated instance is based on.
 * @param[out] p_u_ins      The pointer to a preallocated instance structure
 *                          with essential information for user.
 * @param[in]  p_u_ins_cfg  Configurations of the instance to be created.
 * @return                  LWNN_SUCC - instance creation success
 *                          LWNN_ERR_PARAM_INVALID - invalid input parameter(s)
 *                          LWNN_ERR_MALLOC_FAIL - DMA buffer allocation failure
 *                          LWNN_ERR_FAIL - LWNN failure 
 */
lwnn_ret_t lwnn_instance_create(lwnn_user_model_t *p_u_model,
	                              lwnn_user_instance_t *p_u_ins, 
	                              lwnn_user_instance_cfg_t *p_u_ins_cfg);


/*!
 * Configure a created instance.
 * @param[in]   p_u_ins     The pointer to an existing instance.
 * @param[in]   opt         The configuration option.
 * @param[in]   cfg         The instance configuration info.
 * @return                  LWNN_SUCC - instance configuration success
 *                          LWNN_ERR_PARAM_INVALID - invalid input parameter(s)
 *                          LWNN_ERR_FAIL - LWNN failure
 */
lwnn_ret_t lwnn_instance_configure(lwnn_user_instance_t *p_u_ins,
                                   lwnn_ins_cfg_opt_t opt, void *cfg);


/*!
 * @brief                 Push a set of input into the input buffer queue of an
 *                        instance.
 * @param[in]  p_u_ins    The pointer to an existing instance.
 * @param[in]  p_input    The input buffer information.
 *                        For non-DMA buffers, assign as below:
 *                          u_input.frame_ts_user_addr[i] = void_star_user_addr;
 *                        For DMA buffers, assign as below:
 *                          u_input.frame_ts_user_addr[i] = void_star_user_addr;
 *                          u_input.frame_ts_user_addr[i] = uint32_phys_addr;
 * @return                LWNN_SUCC - input pushing success
 *                        LWNN_ERR_PARAM_INVALID - invalid input parameter(s)
 *                        LWNN_INPUT_QUEUE_FULL - input queue full
 *                        LWNN_ERR_SYNC_FAIL - DMA buffer flushing failure
 *                        LWNN_ERR_FAIL - LWNN failure
 */
lwnn_ret_t lwnn_input_push(lwnn_user_instance_t *p_u_ins, lwnn_user_input_t *p_input);


/*!
 * @brief                 Get input tensor bus address from shared DMA-buf
 * @param[in]  buf	      The pointer to an user buffer includes DMA-buf fd that
 *						  needs.
 * @return                LWNN_SUCC - get bus address success
 *                        LWNN_ERR_DMA_BUF_IMPORT_FAIL - get bus address failure
 */
lwnn_ret_t lwnn_input_ts_bus_addr_get(lwnn_user_dma_buf_t *buf);


/*!
 * @brief                 This is a two step of "push" operation so that the
 *                        caller can directly use LWNN preallocated input
 *                        buffers. The "get" step acquires an input buffer to be
 *                        filled with tensor data.
 * @param[in]  p_u_ins    The pointer to an existing instance.
 * @param[out] p_input    The LWNN allocated DMA input buffer.
 * @return                LWNN_SUCC - input getting success
 *                        LWNN_ERR_PARAM_INVALID - invalid input parameter(s)
 *                        LWNN_INPUT_QUEUE_FULL - input queue full
 *                        LWNN_ERR_FAIL - LWNN failure
 */
lwnn_ret_t lwnn_input_get(lwnn_user_instance_t *p_u_ins, lwnn_user_input_t *p_input);


/*!
 * @brief                 This is a two step of "push" operation so that the
 *                        caller can directly use LWNN preallocated input
 *                        buffers. The "set" step flushes the buffer caches and
 *                        enqueue the buffer for the next inference step,
 *                        lwnn_instance_run().
 * @param[in]  p_u_ins    The pointer to an existing instance.
 * @param[in]  p_input    The pointer to an lwnn input structure set by
 *                        bsnn_input_get().
 * @return                LWNN_SUCC - input setting success
 *                        LWNN_ERR_PARAM_INVALID - invalid input parameter(s)
 *                        LWNN_ERR_SYNC_FAIL - DMA buffer flushing failure
 *                        LWNN_ERR_FAIL - LWNN failure
 */
lwnn_ret_t lwnn_input_set(lwnn_user_instance_t *p_u_ins, lwnn_user_input_t *p_input);


/*!
 * @brief                 Run inference for a model instance.
 * @param[in]  p_u_ins    The pointer to an existing instance.
 * @return                LWNN_SUCC - instance execution success
 *                        LWNN_ERR_PARAM_INVALID - invalid input parameter(s)
 *                        LWNN_INPUT_QUEUE_EMPTY - input queue empty
 *                        LWNN_OUTPUT_QUEUE_FULL - output queue full
 *                        LWNN_ERR_SYNC_FAIL - DMA buffer flushing failure
 *                        LWNN_ERR_FAIL - LWNN failure
 */
lwnn_ret_t lwnn_instance_run(lwnn_user_instance_t *p_u_ins);


/*!
 * @brief                   Get an output buffer with inference result.
 * @note                    It could be called before input preparation or
 *                          inference running.
 * @param[in]   p_u_ins     The pointer to an existing instance.
 * @param[out]  p_u_out     The pointer to the output buffer with inference
 *                          completed.
 * @param[out]  p_post_idx  The pointer to a preallocated variable to store
 *                          the index handle of the output buffer in the output
 *                          buffer queue. The ownership of this buffer will be
 *                          handed over to the calller and the caller should
 *                          later release the buffer with this index.
 * @return                  LWNN_SUCC - output getting success
 *                          LWNN_ERR_PARAM_INVALID - invalid input parameter(s)
 *                          LWNN_OUTPUT_QUEUE_EMPTY - output queue empty
 *                          LWNN_ERR_FAIL - LWNN failure
 */
lwnn_ret_t lwnn_output_get(lwnn_user_instance_t *p_u_ins, lwnn_user_output_t* p_u_out,
			                     int* p_post_idx);


/*!
 * @brief                   Release the buffer acquired in lwnn_output_get().
 * @param[in]   p_u_ins     The pointer to an existing instance.
 * @param[out]  post_idx    The index handle of the output buffer to be released
 * @return                  LWNN_SUCC - output releasing success
 *                          LWNN_ERR_PARAM_INVALID - invalid input parameter(s)
 *                          LWNN_ERR_SYNC_FAIL - DMA buffer flushing failure
 *                          LWNN_ERR_FAIL - LWNN failure
 */
lwnn_ret_t lwnn_output_release(lwnn_user_instance_t *p_u_ins, int post_idx);


/*!
 * @brief                   Destroy an instance.
 * @param[in]   p_u_ins     The pinter to an existing instance to be destroyed.
 * @return                  LWNN_SUCC - instance destruction success
 *                          LWNN_ERR_PARAM_INVALID - invalid input parameter(s)
 *                          LWNN_ERR_FREE_FAIL - DMA buffer deallocation failure
 *                          LWNN_ERR_FAIL - LWNN failure
 */
lwnn_ret_t lwnn_instance_destroy(lwnn_user_instance_t *p_u_ins);


/*!
 * @brief                   Unload a model. All instances of the model will also
 *                          be destroyed.  
 * @param[in]   p_u_model   The pointer to an existing model structure.
 * @return                  LWNN_SUCC - model unloading success
 *                          LWNN_ERR_PARAM_INVALID - invalid input parameter(s)
 *                          LWNN_ERR_FREE_FAIL - DMA buffer deallocation failure
 *                          LWNN_ERR_FAIL - LWNN failure
 */
lwnn_ret_t lwnn_model_unload(lwnn_user_model_t* p_u_model);

/*!
 * @brief                   Release all resources in LWNN. It unloads all
 *                          models and all instances. 
 * @return                  LWNN_SUCC - destruction success
 *                          WNN_ERR_PARAM_INVALID - invalid input parameter(s)
 *                          LWNN_ERR_FREE_FAIL - DMA buffer deallocation failure
 *                          LWNN_ERR_FAIL - LWNN failure
 */
lwnn_ret_t lwnn_all_destroy();


/***************************** Accessory APIs *********************************/
/*!
 * @brief                 Obtain the information of a specified input tensor of
 *                        an instance.
 * @param[in]   p_u_ins   The pointer to an existing instance.
 * @param[in]   queue_idx The index handle of the output in the queue.
 * @param[in]   ts_idx    The index of the input tensor.
 * @param[out]  ts        The pointer to a preallcoated tensor information
 *                        structure.
 * @return                LWNN_SUCC - input tensor getting success
 *                        LWNN_ERR_PARAM_INVALID - invalid input parameter(s)
 *                        LWNN_ERR_FAIL - LWNN failure
 */ 
lwnn_ret_t lwnn_get_input_ts(lwnn_user_instance_t *p_u_ins, int queue_idx,
                             int ts_idx, lwnn_user_tensor_t *ts);


/*!
 * @brief                 Obtain the information of a specified output tensor of
 *                        an instance.
 * @param[in]   p_u_ins   The pointer to an existing instance.
 * @param[in]   queue_idx The index handle of the output in the queue.
 * @param[in]   ts_idx    The index of the output tensor.
 * @param[out]  ts        The pointer to a preallcoated tensor information
 *                        structure.
 * @return                LWNN_SUCC - output tensor getting success
 *                        LWNN_ERR_PARAM_INVALID - invalid input parameter(s)
 *                        LWNN_ERR_FAIL - LWNN failure
 */ 
lwnn_ret_t lwnn_get_output_ts(lwnn_user_instance_t *p_u_ins, int queue_idx,
                              int ts_idx, lwnn_user_tensor_t *ts);


/*!
 * @brief                 Obtain the buffer size of an input tensor of the model.
 * @param[in]   p_u_model The pointer to an existing model.
 * @param[in]   ts_idx    The index of the input tensor.
 * @return                The size of the input tensor, 0 for errors.
 */
uint32_t lwnn_get_input_buf_sz(lwnn_user_model_t *p_u_model, int ts_idx);


/*!
 * @brief                 Obtain the buffer size of an output tensor of the
 *                        model.
 * @param[in]   p_u_model The pointer to an existing model.
 * @param[in]   ts_idx    The index of the output tensor.
 * @return                The size of the output tensor, 0 for errors.
 */
uint32_t lwnn_get_output_buf_sz(lwnn_user_model_t *p_u_model, int ts_idx);


/*!
 * @brief                 Obtain the quantization scale of an output tensor of
 *                        the model.
 * @param[in]   p_u_model The pointer to an existing model.
 * @param[in]   ts_idx    The index of the output tensor.
 * @return                The quantization scale of the output tensor, 0 for
 *                        errors.
 */
float lwnn_get_output_qnt_scale(lwnn_user_model_t *p_u_model, int ts_idx);


/*!
 * @brief                 Obtain the meta information of a specified input
 *                        tensor of a model.
 * @param[in]   p_u_model The pointer to an existing model.
 * @param[in]   ts_idx    The index of the input tensor.
 * @param[out]  ts        The pointer to a preallcoated tensor meta
 *                        information structure.
 * @return                LWNN_SUCC - input tensor meta getting success
 *                        LWNN_ERR_PARAM_INVALID - invalid input parameter(s)
 *                        LWNN_ERR_FAIL - LWNN failure
 */ 
lwnn_ret_t lwnn_get_input_ts_meta(lwnn_user_model_t *p_u_model, int ts_idx,
                                  lwnn_user_tensor_meta_t *ts);


/*!
 * @brief                 Obtain the meta information of a specified output
 *                        tensor of a model.
 * @param[in]   p_u_model The pointer to an existing model.
 * @param[in]   ts_idx    The index of the output tensor.
 * @param[out]  ts        The pointer to a preallcoated tensor meta
 *                        information structure.
 * @return                LWNN_SUCC - output tensor meta getting success
 *                        LWNN_ERR_PARAM_INVALID - invalid input parameter(s)
 *                        LWNN_ERR_FAIL - LWNN failure
 */ 
lwnn_ret_t lwnn_get_output_ts_meta(lwnn_user_model_t *p_u_model, int ts_idx,
                                   lwnn_user_tensor_meta_t *ts);


/*!
 * @brief                     Obtain the low-level DSP status of LWNN.
 * @param[in]   lwnn_dsp_info The pointer to the preallocated DSP information
 *                            structure.
 * @return                    LWNN_SUCC - DSP status getting success
 *                            LWNN_ERR_PARAM_INVALID - invalid input parameter(s)
 *                            LWNN_FAIL - LWNN failure
 */ 
lwnn_ret_t lwnn_get_dsp_status(lwnn_dsp_info_t *lwnn_dsp_info);


/*!
 * @brief                     Obtain the version information of LWNN.
 * @param[in]   lwnn_ver_info The pointer to the preallocated version
 *                            information structure.
 * @return                    LWNN_SUCC - output tensor meta getting success
 *                            LWNN_ERR_PARAM_INVALID - invalid input parameter(s)
 *                            LWNN_FAIL - LWNN failure
 */ 
lwnn_ret_t lwnn_get_version(lwnn_ver_info_t *lwnn_ver_info);


/*!
 * @brief                   Set the print level of LWNN. See lwnn_print_level_t.
 * @param[out]  print_level The print level to be set.
 * @return                  LWNN_SUCC - print level setting success
 *                          LWNN_ERR_PARAM_INVALID - invalid input parameter(s)
 */
lwnn_ret_t lwnn_set_print_level(lwnn_print_level_t print_level);


/*!
 * @brief                 Query the information about a model. See lwnn_query_cmd_t.
 * @param[in]   p_u_model The pointer to an existing model.
 * @param[in]   cmd       The command of query.
 * @param[out]  info      The pointer to an information buffer.
 * @param[in]   size      The size of the information buffer.
 * @return                Return code, 0 for success, others for errors.
 */
lwnn_ret_t lwnn_model_query(lwnn_user_model_t *p_u_model, lwnn_query_cmd_t cmd,
		                        void* info, uint32_t size);


/*!
 * @brief                     Assign DSP for instance, call between instance
 * create and instance run
 * @param[in]   ins_id		  Instance ID.
 * @param[in]   dsp_id		  DSP ID, can be LWNN_CV_DSP_0 or LWNN_CV_DSP_1.
 * @return                    LWNN_SUCC - DSP setting success
 *                            LWNN_FAIL - LWNN failure
 */ 
lwnn_ret_t lwnn_set_target_dsp(lwnn_instance_id_t ins_id, lwnn_dsp_idx_t dsp_id);
#endif  //_LWNN_USER_API_H
