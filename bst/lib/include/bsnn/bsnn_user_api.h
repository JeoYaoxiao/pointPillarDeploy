/*!
 *  This file contains proprietary information that is the sole intellectual property
 *  of Black Sesame Technologies, Inc. and its affiliates.  No portions of this material
 *  may be reproduced in any form without the written permission of:
 *
 *  Black Sesame Technologies, Inc. and its affiliates
 *  2255 Martin Ave. Suite D
 *  Santa Clara, CA  95050
 *  Copyright \@2018: all right reserved.
 *
 *  Notice:
 *
 *  You are running an EVALUATION distribution of the neural network tools provided by
 *  Black Sesame Technologies, Inc. under NDA.
 *  This copy may NOT be used in production or distributed to any third party.
 *  For distribution or production, further Software License Agreement is required.
 *
 * @file
 *  bsnn_user_api.h
 *
 * @brief
 *  This file contains definitions of BSNN user API.
 *
 * @author
 *  Zheng Li, Xian Wang, Qun Gu, Shichen Lin, Zhe Cui, Thomas Ning, Debin Chen
 *
 * @date
 *  02/16/2022
 *
 */
#include <vector>
#include <string>
#include <bits/stdc++.h>
#ifndef _BSNN_USER_API_H
#define _BSNN_USER_API_H

#ifdef __cplusplus
extern "C" {
#endif

/* pre-allocated */
#define BSNN_MAX_IN_TS                    8       /* maximum number of input tensor. */
#define BSNN_MAX_OUT_TS                   32      /* maximum number of output tensor. */

#define BSNN_MAX_BATCH_SIZE               4
#define BSNN_MAX_BATCH_SIZE_EXTENDED      4       //Added support for batch size 8

#define BSNN_MAX_NAME_LEN                 256     /* maximum name lenth of tensor. */
#define BSNN_ASIC_TYPE_LEN				  32	  /* maximum length of ASIC type, A0, B0, or N/A */

/*! tensor dimension constants */
#define BSNN_MAX_PITCH_DIM                4		  /* tensor pitch dimensions, padded size for alignment */
#define BSNN_MAX_SIZE_DIM                 4		  /* tensor size dimensions */

#define BSNN_MAX_DEQUANT_SCALE_TOTAL	  4096	  /* HW capacity of total dequant scale */

typedef uint32_t bsnn_model_id_t;
typedef uint32_t bsnn_instance_id_t;
using namespace std;

/*!
 * @enum The error code returned by the BSNN API
 */
typedef enum {
  BSNN_SUCC                               = 0,    /* execution success */
  BSNN_ERR_FAIL                           = 1,    /* execution failure */
  BSNN_ERR_TIMEOUT                        = 2,    /* execution timeout */
  BSNN_ERR_DEVICE_UNAVAILABLE             = 3,    /* device unavailable */
  BSNN_ERR_MALLOC_FAIL                    = 4,    /* dma buffer malloc failure. */
  BSNN_ERR_PARAM_INVALID                  = 5,    /* invalid input parameter */
  BSNN_ERR_MODEL_INVALID                  = 6,    /* invalid model */
  BSNN_ERR_CTX_INVALID                    = 7,    /* invalid context */
  BSNN_ERR_INPUT_INVALID                  = 8,    /* input is invalid. */
  BSNN_ERR_OUTPUT_INVALID                 = 9,    /* output is invalid. */
  BSNN_ERR_DEVICE_UNMATCH                 = 10,   /* the device is unmatch, please update bsnn sdk and npu driver/firmware. */
  BSNN_ERR_INCOMPATILE_PRE_COMPILE_MODEL  = 11,   /* This BSNN model use pre_compile mode, but not compatible with current driver. */
  BSNN_ERR_QUEUE_TYPE                     = 12,   /* blocking or nonblocking */
  BSNN_INPUT_QUEUE_EMPTY                  = 13,   /* run out of input buffer */
  BSNN_OUTPUT_QUEUE_EMPTY                 = 14,   /* run out of output buffer */
  BSNN_INPUT_QUEUE_TIMEOUT                = 15,   /* input buffer queue timeout */
  BSNN_OUTPUT_QUEUE_TIMEOUT               = 16,   /* output buffer queue timeout */
  BSNN_ERR_GLB_BUF_FAIL                   = 17,   /* global buffer failure (cannot restore from update failure) */
  BSNN_ERR_SYNC_FAIL                      = 18,   /* dma buffer sync failure. */
  BSNN_ERR_DMA_BUF_IMPORT_FAIL			  = 19,	  /* dma-buf import failure. */
  BSNN_ERR_BATCH                          = 20,   /* problem related to batch size */
  BSNN_BUF_QUEUE_ENQUEUE_FAIL			  = 21,	  /* enqueue input/output buffer queue failed */
  BSNN_ERR_FREE_FAIL					  = 22,	  /* free buffer failed */
  BSNN_INPUT_QUEUE_FULL					  = 23,   /* input buffer queue is full */
  BSNN_OUTPUT_QUEUE_FULL				  = 24,   /* output buffer queue is full */
} bsnn_ret_t;

/*!
 * @enum The model type
 */
typedef enum {
	BSNN_NET = 0,
	BSNN_LWNN = 1,
	BSNN_TFLITE = 2,
	BSNN_TVM = 3,
} bsnn_model_type_t;

/*!
 * @enum The Engine that is targeted to run
 */
typedef enum {
    BSNN_ENGINE_NET_2_MACA  = 0,	/* NET TOP engine, with 2 MAC arrays max */
    BSNN_ENGINE_NET_4_MACA  = 1,	/* NET LITE engine, with 4 MAC arrays max */
	  BSNN_ENGINE_MAX
} bsnn_target_engine_t;

/*!
 * @enum The instance status type
 */
typedef enum {
  BSNN_INS_ACTIVE = 0,		/* instance status when the instance is successfully created */
  BSNN_INS_DONE,			/* instance status when teh instance is successfully destroyed */
  BSNN_INS_MAX
} bsnn_instance_status_t;





/*!
 * @enum The model status type
 */
typedef enum {
  BSNN_MODEL_ACTIVE = 0,	/* model status when model is successfully loaded */
  BSNN_MODEL_DONE,			/* model status when model is unloaded */
  BSNN_MODEL_MAX
} bsnn_model_status_t;


/*!
 * @enum The tensor data type
 */
typedef enum {
  INT8          = 0,  BSNN_TENSOR_INT8      = 0,                               /* data type is int8. */
  INT16         = 1,  BSNN_TENSOR_INT16     = 1,                               /* data type is int16. */
  FP16          = 2,  BSNN_TENSOR_FP16   	= 2,                               /* data type is float16. */
  UINT8         = 3,  BSNN_TENSOR_UINT8     = 3,                               /* data type is uint8. */
  UINT16		= 4,  BSNN_TENSOR_UINT16    = 4,							   /* data type is uint16. */
  INT32			= 5,  BSNN_TENSOR_INT32     = 5,							   /* data type is int32. */
  UINT32		= 6,  BSNN_TENSOR_UINT32    = 6,							   /* data type is uint32. */
  FP32          = 7,  BSNN_TENSOR_FP32   	= 7,                               /* data type is float32. */
  bstdType_MAX,       BSNN_TENSOR_DTYPE_MAX = bstdType_MAX
} bsnn_tensor_dtype_t;

/*!
 * @enum The tensor color type
 */
typedef enum {
  DEFAULT        = 0,  BSNN_TENSOR_DEFAULT   = 0, 				/* normal 4D-tensor */
  RGB888         = 1,  BSNN_TENSOR_RGB888    = 1,               /* RGB888 tensor */
  RGB888P        = 2,  BSNN_TENSOR_RGB888P   = 2,               /* RGB888P tensor */
  RGBD           = 3,  BSNN_TENSOR_RGBD      = 3,               /* RGBD tensor */
  RGBA           = 4,  BSNN_TENSOR_RGBA      = 4,               /* RGBA tensor */
  YUYV           = 5,  BSNN_TENSOR_YUYV      = 5,               /* YUYV tensor */
  UYVY           = 6,  BSNN_TENSOR_UYVY      = 6,               /* UYVY tensor */
  NV12           = 7,  BSNN_TENSOR_NV12      = 7,               /* NV12 tensor */
  NV21           = 8,  BSNN_TENSOR_NV21      = 8,               /* NV21 tensor */
  YUV420P        = 9,  BSNN_TENSOR_YUV420P   = 9,               /* YUV420P tensor */
  MULTI_PD       = 10, BSNN_TENSOR_MULTI_PD  = 10,              /* multi-planar data */
  ColorType_MAX,       BSNN_TENSOR_CTYPE_MAX = ColorType_MAX
} bsnn_tensor_ctype_t;

/*!
 * @enum The tensor data order.
 */
typedef enum {
  WHD       = 0,  BSNN_TENSOR_WHD        = 0,                                /* tensor data format is NCHW. */
  DWH       = 1,  BSNN_TENSOR_DWH        = 1,                                /* tensor data format is NHWC. */
  Order_MAX,      BSNN_TENSOR_DORDER_MAX = Order_MAX
} bsnn_tensor_dorder_t;


/*! 
 * @enum The input/output buffer queue type
 * If nonblocking, 
 * function returns BSNN_INPUT_QUEUE_EMPTY immediately if there is no available input buffer, 
 * function returns BSNN_OUTPUT_QUEUE_EMPTY immediately if there is no available output buffer, 
 * user could re-try with/without sleeping for a period or take other actions. 
 *
 * If blocking, need to specify timeout limit
 * function returns BSNN_INPUT_QUEUE_TIMEOUT after timeout period if there is not available input buffer, 
 * function returns BSNN_OUTPUT_QUEUE_TIMEOUT after timeout period if there is not available output buffer, 
 * user could re-try with/without sleeping for a period or take other actions.
 */
typedef enum {
  GET_IN_OUT_NON_BLOCKING = 0,
  GET_IN_OUT_BLOCKING = 1
} bsnn_queue_type_t;    


/*!
 * @enum The input buffer type
 */
typedef enum {
  BUS_ADDR = 0,    	/* input tensor buffers are DMA buffers allocated by other sources and passed to BSNN */
  USER_ADDR = 1,   	/* input tensor buffers are DMA buffers allocated by BSNN, or userspace allocated buffers */
  ADDR_TYPE_MAX
} input_buffer_type_t;


/*!
 * @enum The query command for bsnn_query
 */
typedef enum {
  BSNN_QUERY_IN_OUT_NUM = 0, /* query the number of input & output tensor. */
  BSNN_QUERY_INPUT_ATTR,     /* query the tensor size and pitch size of input tensor. */
  BSNN_QUERY_OUTPUT_ATTR,    /* query the tensor size and pitch size of output tensor. */
  BSNN_QUERY_PERF_DETAIL,   /* query the detail performance, NOT supported in this version */
  BSNN_QUERY_PERF_RUNTIME,   /* query the cycle count of whole DAG */
  BSNN_QUERY_SDK_VERSION,    /* query the SDK & driver version */
  BSNN_QUERY_MODEL_ATTR,	 /* query model attributes */
  BSNN_QUERY_ASIC_TYPE,	   /* query ASIC type, A0 or B0 */
  BSNN_QUERY_CMD_MAX
} bsnn_query_cmd_t;


/*!
 * @enum The log level
 */
typedef enum {
  BSNN_NO_PRINT,		/* no printings from BSNN */
  BSNN_LOG_PRINT,		/* printings for execution status */
  BSNN_DEBUG_PRINT		/* printings for execution status and debugging info */
} bsnn_print_level_t;


/********************************  Structures  ********************************/
//compose the message transfer from the NPU to ARM
struct tflite_recv_msg{
	int output_ts_num;
	std::vector<std::string> tensor_name;	
	std::vector<int> osize0;
	std::vector<int> osize1;
	std::vector<int> osize2;
	std::vector<int> opitch0;
	std::vector<int> opitch1;
	std::vector<int> opitch2;
	std::vector<float> dequant_scale;
  std::vector<uint64_t> ts_addr;
};

/*!
 * @struct The configurtions of the instance to be created
 */
typedef struct {
  bsnn_queue_type_t queue_type;			/* blocking or non-blocking queue */
  input_buffer_type_t in_type;			/* buffer allocation type */
  uint32_t input_bufferq_size;			/* total number of input buffer queues */
  uint32_t output_bufferq_size;			/* total number of output buffer queues */
  uint32_t in_out_timeout_period;		/* timeout for blocking queue, in milisecond */
  uint32_t mac_arr_num;					/* MAC array number */
  uint32_t batch_size;                /* batch size for this model*/
  bsnn_target_engine_t target_engine;	/* Targeting Engine to run */
} bsnn_user_instance_cfg_t;


/*!
 * @struct The user input tensor buffer address.
*/
typedef struct {
  uint32_t input_q_idx;							/* input buffer queue index */
  void*    frame_ts_usr_addr[BSNN_MAX_IN_TS];   /* the addr passed is userspace addr, assign USER_ADDR to buffer_type */
  uint32_t frame_ts_bus_addr[BSNN_MAX_IN_TS];	/* the addr passed is physical addr, assign BUS_ADDR to buffer_type */
  int      dma_buf_fds[BSNN_MAX_IN_TS];         /* DMA-buf fd for input push use */
} bsnn_user_input_t;

typedef struct {
  uint32_t input_q_idx;							/* input buffer queue index */
  void*    frame_ts_usr_addr_batch[BSNN_MAX_BATCH_SIZE][BSNN_MAX_IN_TS];   /* the addr passed is userspace addr, assign USER_ADDR to buffer_type */
  uint32_t frame_ts_bus_addr_batch[BSNN_MAX_BATCH_SIZE][BSNN_MAX_IN_TS];	/* the addr passed is physical addr, assign BUS_ADDR to buffer_type */
  int      dma_buf_fds_batch[BSNN_MAX_BATCH_SIZE][BSNN_MAX_IN_TS];         /* DMA-buf fd for input push use */

  //extra part
  void*    frame_ts_usr_addr_batch_extra[BSNN_MAX_BATCH_SIZE_EXTENDED][BSNN_MAX_IN_TS];
  uint32_t frame_ts_bus_addr_batch_extra[BSNN_MAX_BATCH_SIZE_EXTENDED][BSNN_MAX_IN_TS];
  int      dma_buf_fds_batch_extra[BSNN_MAX_BATCH_SIZE_EXTENDED][BSNN_MAX_IN_TS];
} bsnn_user_input_batch_t;

/*!
 * @struct The dma-buf input buffer information.
 */
typedef struct {
  int      fd;                          //!< DMA buf fd of input tensor buffers
  uint32_t bus_addr;                    //!< The bus address of the buf
  uint32_t size;                        //!< The size of the buf
} bsnn_user_dma_buf_t;


/*!
 * @struct The user output tensor properties.
*/
typedef struct {
  uint32_t output_ts_num;														/* total number of output tensors */
  float    ts_qnt_scale[BSNN_MAX_OUT_TS];										/* dequant scale of output tensors, get from meta file */
  uint64_t ts_addr[BSNN_MAX_OUT_TS];											/* address of output tensor buffers */
  uint32_t dequant_scale_num[BSNN_MAX_OUT_TS];									/* per-channel dequant scale number */
  float    ts_deqnt_scale_per_channel[BSNN_MAX_OUT_TS][BSNN_MAX_DEQUANT_SCALE_TOTAL]; /* per-channel dequant scale of output tensors, get from meta file */
} bsnn_user_output_t;

/*!
 * @struct The user output tensor properties in batch.
*/
typedef struct {
  uint32_t output_ts_num_batch;														/* total number of output tensors, including tensors in all the batches */
  float    ts_qnt_scale_batch[BSNN_MAX_BATCH_SIZE][BSNN_MAX_OUT_TS];										/* dequant scale of output tensors, get from meta file */
  uint64_t ts_addr_batch[BSNN_MAX_BATCH_SIZE][BSNN_MAX_OUT_TS];											/* address of output tensor buffers */
  uint32_t dequant_scale_num_batch[BSNN_MAX_BATCH_SIZE][BSNN_MAX_OUT_TS];									/* per-channel dequant scale number */
  float    ts_deqnt_scale_per_channel_batch[BSNN_MAX_BATCH_SIZE][BSNN_MAX_OUT_TS][BSNN_MAX_DEQUANT_SCALE_TOTAL]; /* per-channel dequant scale of output tensors, get from meta file */

  //extra part
  float    ts_qnt_scale_batch_extra[BSNN_MAX_BATCH_SIZE_EXTENDED][BSNN_MAX_OUT_TS];
  uint64_t ts_addr_batch_extra[BSNN_MAX_BATCH_SIZE_EXTENDED][BSNN_MAX_OUT_TS];
  uint32_t dequant_scale_num_batch_extra[BSNN_MAX_BATCH_SIZE_EXTENDED][BSNN_MAX_OUT_TS];
  float    ts_deqnt_scale_per_channel_batch_extra[BSNN_MAX_BATCH_SIZE_EXTENDED][BSNN_MAX_OUT_TS][BSNN_MAX_DEQUANT_SCALE_TOTAL];
  
} bsnn_user_output_batch_t;

/*!
 * @struct The information for BSNN_QUERY_IN_OUT_NUM.
*/
typedef struct {
  uint32_t n_input;  /* the number of input tensors */
  uint32_t n_output; /* the number of output tensors */
} bsnn_input_output_num_t;


/*!
 * @struct The information for BSNN_QUERY_INPUT_ATTR.
*/
typedef struct {
  uint32_t index;						/* tensor index, set by caller */
  uint16_t pitch[BSNN_MAX_PITCH_DIM];	/* tensor pitch size */
  uint16_t size[BSNN_MAX_SIZE_DIM];		/* tensor size */
} bsnn_tensor_attr_t;


/*!
 * @struct The information for BSNN_QUERY_PERF_RUNTIME.
*/
typedef struct {
    uint32_t dag_cnt_top;       // DAG cycle count of NET TOP
    uint32_t dag_cnt_lite;      // DAG cycle count of NET LITE
} bstn_perf_info_t;


/*!
 * @struct The tensor dimension.
*/
typedef struct {
  uint16_t pitch[3];					/* tensor pitch size, for backward compatibility */
  uint16_t size[BSNN_MAX_SIZE_DIM];		/* tensor size */
} bsnn_tensor_dim_t;


/*!
 * @struct The user tensor properties.
*/
typedef struct  {
  bsnn_tensor_dim_t     dim;		/* tensor dimension */
  bsnn_tensor_dtype_t   dType;		/* data type */
  bsnn_tensor_dorder_t  dOrder;		/* date order */
  bsnn_tensor_ctype_t   cType;		/* color type */
  void                  *user_addr; /* tensor buffer user space address */
} bsnn_user_tensor_t;

/*!
 * @struct The user model properties.
*/
typedef struct {
  bsnn_model_id_t id;						/* ID for each model, incremental, starting from 0 */
  char model_name[BSNN_MAX_NAME_LEN];		/* model name, can be gotten from meta file */
  bsnn_model_status_t status;				/* model status */
  uint32_t            weight_size;			/* model weights.bin file size */
  uint32_t            fwlib_size;			/* model lib file size */
  uint32_t            activation_size;		/* model activation size, get from meta file */
  uint32_t            dag_size;				/* model total dag size, get from meta file */
  uint32_t            input_ts_num;			/* total number of input tensors */
  uint32_t            output_ts_num;		/* total number of output tensors */
  bsnn_model_type_t model_type;				/* model type, can be gotten from meta file */
  uint32_t            coeff_size;			/* lwnn model coeff buffer size */
  uint32_t            scratch_size;			/* lwnn model scratch buffer size */
} bsnn_user_model_t;


/*!
 * @struct The user instance properties.
*/
typedef struct {
  bsnn_instance_id_t      ins_id;				/* ID for each instance, incremental, starting from 0 */
  bsnn_model_id_t         model_id;				/* ID for the model this instance belongs to */
  bsnn_instance_status_t  status;				/* instance status, created or destroyed */
  uint32_t                in_buf_queue_size;	/* total number of input buffer queues */
  uint32_t                out_buf_queue_size;	/* total number of output buffer queues */
  uint32_t                in_ts_num;			/* number of input tensors per batch */
  uint32_t                out_ts_num;			/* number of output tensors per batch */
  bsnn_model_type_t		  model_type;			/* model type, passed from user model */
} bsnn_user_instance_t;


/*!
 * @struct The BSNN version and release information.
*/
typedef struct {
  int8_t bsnn_major;		/* BSNN lib/test case release versioning, identifier MAJOR in MAJOR.MINOR.PATCH notation */
  int8_t bsnn_minor;		/* BSNN lib/test case release versioning, identifier MINOR in MAJOR.MINOR.PATCH notation */
  int8_t bsnn_patch;		/* BSNN lib/test case release versioning, identifier PATCH in MAJOR.MINOR.PATCH notation */
  int8_t bsnn_magic;		/* reserved */

  int16_t bsnn_rel_year;	/* BSNN release year */
  int8_t bsnn_rel_day;		/* BSNN release day in the month */
  int8_t bsnn_rel_mon;		/* BSNN release month in the year */

  int8_t bstn_major;		/* kernel driver release versioning, identifier MAJOR in MAJOR.MINOR.PATCH notation */
  int8_t bstn_minor;		/* kernel driver release versioning, identifier MINOR in MAJOR.MINOR.PATCH notation */
  int8_t bstn_patch;		/* kernel driver release versioning, identifier PATCH in MAJOR.MINOR.PATCH notation */
  int8_t bstn_maigc;		/* reserved */

  int16_t bstn_rel_year;	/* kernel driver release year */
  int8_t bstn_rel_day;		/* kernel driver release day in the month */
  int8_t bstn_rel_mon;		/* kernel driver release month in the year */

  int8_t dsp_rt_major;		/* DSP RunTime release versioning, identifier MAJOR in MAJOR.MINOR.PATCH notation */
  int8_t dsp_rt_minor;		/* DSP RunTime release versioning, identifier MINOR in MAJOR.MINOR.PATCH notation */
  int8_t dsp_rt_patch;		/* DSP RunTime release versioning, identifier PATCH in MAJOR.MINOR.PATCH notation */
  int8_t dsp_rt_magic;		/* reserved */

  int16_t dsp_rt_rel_year;	/* DSP RunTime release year */
  int8_t dsp_rt_rel_day;	/* DSP RunTime release day in the month */
  int8_t dsp_rt_rel_mon;	/* DSP RunTime release month in the year */
} bsnn_version_t;

/*!
 * @struct The BSNN test configuration information.
*/
typedef struct {
	std::string model;                    /* Directory of [model].rbf, [model].meta, and weights.bin */
	std::string input;                    /* Diretory of input tensors */
	std::string ref_output;               /* Directory of reference output tensors, for comparison */                      
	std::string output;                   /* Directory of output tensors */                  
	uint32_t input_queue_num;             /* Input buffer queue number */                        
	uint32_t output_queue_num;            /* Output buffer queue number */                          
	bsnn_queue_type_t queue_type;         /* Input/output buffer queue type -- 1: blocking, 0: non-blocking */                                                  
	uint32_t batch_size;                  /* Batch size of the model */                                          
	uint32_t mac_arr_num;                 /* MAC array number -- 1: 1 MAC array, 2: 2 MAC arrays, 4: 4 MAC arrays */                                          
	int run_cnt;                          /* Total number of times inference runs -- 0: no run */                                  
	bsnn_target_engine_t target_engine;   /* The net engine to run the model -- 0: NET with 2 MAC arrays max, 1: NET with 4 MAC arrays max */                                                              
	uint32_t save_output;                 /* Whether to save output -- 0: no save, 1: save to the configured 'output' path */                                                
} bsnn_config_t;

/*!
 * @struct The BSNN model file from memory.
*/
typedef struct{
  int file_size;			/* model file size */
  char* file_in_memory_location;	/* model file physical start address */
  int file_category;			/* 1: .lib, 2: .meta, 3: weights.bin */
}bsnn_file_in_memory;

struct scheduler_config_t   
{
    bool scheduler_mode;
	bool time_sequence_mode;
	int segment_number;
	vector<string> execution_order;
	string operation_device;
	uint32_t instance_num;
	size_t time_sequence;
	uint32_t calibration_average;
	uint32_t calibration_scale;
	int target_dsp_device;     // 0 = LWNN_CV_DSP_0  1 = LWNN_CV_DSP_1 
	bsnn_config_t bsnn_config;
};

/*!
 *@struct output tensor information needed to pass between BSNN and LWNN
*/

// load static part of each seg information once in below structure
typedef struct {
	//following info is static to the model
	int output_ts_num;                                
	//target device 0 means BSNN | target device 1 means LWNN | target device 2 means ARM(TFLite) 
	int target_device;                                
  	uint32_t batch_size;                              
  	uint32_t calibration_average;                     
  	uint32_t calibration_scale;                       
  	size_t time_sequence;                               
	std::vector<std::string> tensor_name;	         
	std::vector<int> osize0;                         
	std::vector<int> osize1;                          
	std::vector<int> osize2;                          
	std::vector<int> opitch0;                         
	std::vector<int> opitch1;                         
	std::vector<int> opitch2;                         
	std::vector<float> dequant_scale;                 
} bsnn_lwnn_msg_t_static;




#ifdef __cplusplus
} //extern "C"
#endif




/*******************************************************************************
 ******************************************************************************/
#ifdef AARCH64
void *bsnn_memcpy_neon(uint8_t* dst, const uint8_t* src, uint64_t size);
#endif

/*!
  * Return version number and release date
  * @param[out] bsnn_ver     The pointer to a preallocted BSNN version structure
  * @return                  BSNN_SUCC - success
  *                          BSNN_ERR_DEVICE_UNAVAILABLE - invalid driver status
  *                          BSNN_ERR_PARAM_INVALID - invalid input parameter
  */
bsnn_ret_t bsnn_get_version(bsnn_version_t* bsnn_ver);


/*!
 * Set log level to BSNN
 * @param[in] print_level   log level to be set
 * @return                  BSNN_SUCC - success
 *                          BSNN_ERR_PARAM_INVALID - invalid input parameter
 */
bsnn_ret_t bsnn_set_print_level(bsnn_print_level_t print_level);


/*!
 * Initializes a model by loading model related files.
 * @param[out]  p_u_model   Pointer to model structure with essential information to caller.
 * @param[in]   model_path  Path of model related files: .meta, .lib, .bin
 * @return                  BSNN_SUCC - success
 *                          BSNN_ERR_PARAM_INVALID - invalid input parameter
 *                          BSNN_ERR_DEVICE_UNAVAILABLE - failed to open driver, or invalid driver status
 *                          BSNN_ERR_FAIL - failed to get system info
 *                          BSNN_ERR_GLB_BUF_FAIL - failed to alloc shared buffer, e.g. activation buf, dag buf
 *                          BSNN_ERR_MALLOC_FAIL - failed to alloc DMA buffer
 *                          BSNN_ERR_MODEL_INVALID - invalid model file
 */
bsnn_ret_t bsnn_model_load(bsnn_user_model_t* p_u_model, const char* model_path);


/*!
 * Initializes a model by loading model related files and assign model type from
 * configuration file.
 * @param[out]  p_u_model   Pointer to model structure with essential information to caller.
 * @param[in]   model_path  Path of model related files: .meta, .lib, .bin
 * @param[in]   type		Type of the model, e.g. NET, DSP, ...
 * @return                  BSNN_SUCC - success
 *                          BSNN_ERR_PARAM_INVALID - invalid input parameter
 *                          BSNN_ERR_DEVICE_UNAVAILABLE - failed to open driver, or invalid driver status
 *                          BSNN_ERR_FAIL - failed to get system info
 *                          BSNN_ERR_GLB_BUF_FAIL - failed to alloc shared buffer, e.g. activation buf, dag buf
 *                          BSNN_ERR_MALLOC_FAIL - failed to alloc DMA buffer
 *                          BSNN_ERR_MODEL_INVALID - invalid model file
 */
bsnn_ret_t bsnn_model_load_with_type(bsnn_user_model_t *p_u_model, const char *model_path, const char *type);


/*!
 * Generates an instance based on an existed model.
 * User could also call this funciton multiple times to generate multiple instances, these instances
 * will share the same model, but each instance has its own output buffer queue(each buffer has a
 * complete set of output tensors)
 * @param[in]  p_u_model    The model that the generated instance is based on.
 * @param[out] p_u_ins      Pointer to instance structure with essential information to user.
 * @param[in]  p_u_ins_cfg  Configuration of the instance to be created.
 * @return                  BSNN_SUCC - success
 *                          BSNN_ERR_DEVICE_UNAVAILABLE - invalid driver status
 *                          BSNN_ERR_PARAM_INVALID - invalid input parameter
 *                          BSNN_ERR_MALLOC_FAIL - failed to alloc DMA buffer
 */
bsnn_ret_t bsnn_instance_create(bsnn_user_model_t* p_u_model,
				bsnn_user_instance_t* p_u_ins, 
				bsnn_user_instance_cfg_t* p_u_ins_cfg);


/*!
 * Pushes an input to input buffer queue, the buffer is from external and copied into BSNN. Now only support batch size 1
 * @param[in]  p_u_ins      Pointer to an existing instance.
 * @param[in]  p_input      Input to this current frame run, the address could be physical
 *                          addr or user space address.
 *                          If userspace addr, assign as below:
 *                            u_input.frame_ts_user_addr[0] = input_buf;
 *                          If physical addr, assign as below:
 *                            u_input.frame_ts_user_addr[0] = uint_32 physical_addr
 * @return                  BSNN_SUCC - success
 *                          BSNN_ERR_BATCH - function can not support model with batch size greater than 1
 *                          BSNN_ERR_DEVICE_UNAVAILABLE - invalid driver status
 *                          BSNN_ERR_SYNC_FAIL - failed to sync DMA buffer
 *                          BSNN_ERR_PARAM_INVALID - invalid input parameter
 *                          BSNN_INPUT_QUEUE_EMPTY - failed to dequeue from input queue
 *                          BSNN_INPUT_QUEUE_TIMEOUT - timeout waiting for input queue member
 */
bsnn_ret_t bsnn_input_push(bsnn_user_instance_t *p_u_ins, bsnn_user_input_t *p_input);


/*!
 * Pushes an input as batch to input buffer queue, the buffer is from external and copied into BSNN
 * @param[in]  p_u_ins        Pointer to an existing instance.
 * @param[in]  p_input_batch  Input batch to this current frame run, the address could be physical
 *                            addr or user space address.
 *                            If userspace addr, assign as below(higher dimension as batch index, lower dimension as tensor index):
 *                              u_input.frame_ts_user_addr[0][0] = input_buf;
 *                            If physical addr, assign as below(higher dimension as batch index, lower dimension as tensor index):
 *                              u_input.frame_ts_user_addr[0][0] = uint_32 physical_addr
 * @return                    BSNN_SUCC - success
 *                            BSNN_ERR_DEVICE_UNAVAILABLE - invalid driver status
 *                            BSNN_ERR_SYNC_FAIL - failed to sync DMA buffer
 *                            BSNN_ERR_PARAM_INVALID - invalid input parameter
 *                            BSNN_INPUT_QUEUE_EMPTY - failed to dequeue from input queue
 *                            BSNN_INPUT_QUEUE_TIMEOUT - timeout waiting for input queue member
 */
bsnn_ret_t bsnn_input_push_batch(bsnn_user_instance_t *p_u_ins, bsnn_user_input_batch_t *p_input_batch);


/*!
 * @brief                 Get input tensor bus address from shared DMA-buf
 * @param[in]  buf        The pointer to an user buffer includes DMA-buf fd that
 *                        needs.
 * @param[in]  u_ins      The pointer to user instance
 * @return                BSNN_SUCC - get bus address success
 *                        BSNN_ERR_DMA_BUF_IMPORT_FAIL - get bus address failure
 */
bsnn_ret_t bsnn_input_ts_bus_addr_get(bsnn_user_dma_buf_t *buf, bsnn_user_instance_t *u_ins);


/*!
 * This is the first step of a two-step operation of "push", so caller can use BSNN input buffer. Now only support batch size 1
 * The "get" operation assume the ownership of input buffer from BSNN. No
 * out-of-order supported.
 * @param[in]  p_u_ins      Pointer to an existing instance.
 * @param[out] p_input      Return current input buffer from BSNN
 * @return                  BSNN_SUCC - success
 *                          BSNN_ERR_BATCH - function can not support model with batch size greater than 1
 *                          BSNN_ERR_DEVICE_UNAVAILABLE - invalid driver status
 *                          BSNN_ERR_PARAM_INVALID - invalid input parameter
 *                          BSNN_INPUT_QUEUE_EMPTY - failed to dequeue from input queue
 *                          BSNN_INPUT_QUEUE_TIMEOUT - timeout waiting for input queue member
 */
bsnn_ret_t bsnn_input_get(bsnn_user_instance_t *p_u_ins, bsnn_user_input_t *p_input);


/*!
 * This is the first step of a two-step operation of "push", so caller can use BSNN input buffer.
 * The "get" operation assume the ownership of input buffer from BSNN. No
 * out-of-order supported.
 * @param[in]  p_u_ins        Pointer to an existing instance.
 * @param[out] p_input_batch  Return current input batch buffer from BSNN
 * @return                    BSNN_SUCC - success
 *                            BSNN_ERR_DEVICE_UNAVAILABLE - invalid driver status
 *                            BSNN_ERR_PARAM_INVALID - invalid input parameter
 *                            BSNN_INPUT_QUEUE_EMPTY - failed to dequeue from input queue
 *                            BSNN_INPUT_QUEUE_TIMEOUT - timeout waiting for input queue member
 */
bsnn_ret_t bsnn_input_get_batch(bsnn_user_instance_t *p_u_ins, bsnn_user_input_batch_t *p_input_batch);


/*!
 * This is the second step of a two-step operation of "push", so caller can use BSNN input buffer. Now only support batch size 1
 * The "set" operatoin will return the ownship of the buffer to BSNN. No
 * out-of-order supported.
 * @param[in]  p_u_ins      Pointer to an existing instance.
 * @param[in]  p_input      current input buffer from BSNN to push
 * @return                  BSNN_SUCC - success
 *                          BSNN_ERR_BATCH - function can not support model with batch size greater than 1
 *                          BSNN_ERR_DEVICE_UNAVAILABLE - invalid driver status
 *                          BSNN_ERR_SYNC_FAIL - failed to sync DMA buffer
 *                          BSNN_ERR_PARAM_INVALID - invalid input parameter
 */
bsnn_ret_t bsnn_input_set(bsnn_user_instance_t *p_u_ins, bsnn_user_input_t *p_input);


/*!
 * This is the second step of a two-step operation of "push", so caller can use BSNN input buffer.
 * The "set" operatoin will return the ownship of the buffer to BSNN. No
 * out-of-order supported.
 * @param[in]  p_u_ins        Pointer to an existing instance.
 * @param[in]  p_input_batch  current input batch buffer from BSNN to push
 * @return                    BSNN_SUCC - success
 *                            BSNN_ERR_DEVICE_UNAVAILABLE - invalid driver status
 *                            BSNN_ERR_SYNC_FAIL - failed to sync DMA buffer
 *                            BSNN_ERR_PARAM_INVALID - invalid input parameter
 */
bsnn_ret_t bsnn_input_set_batch(bsnn_user_instance_t *p_u_ins, bsnn_user_input_batch_t *p_input_batch);


/*!
 * Runs a model inference
 * @param[in]  p_u_ins      Pointer to an existing instance.
 * @return                  BSNN_SUCC - success
 *                          BSNN_ERR_FAIL - failed to lock tensor prep mutex
 *                          BSNN_ERR_DEVICE_UNAVAILABLE - invalid driver status
 *                          BSNN_ERR_PARAM_INVALID - invalid input parameter
 *                          BSNN_INPUT_QUEUE_EMPTY - failed to dequeue from input queue
 *                          BSNN_INPUT_QUEUE_TIMEOUT - timeout waiting for input queue member
 *                          BSNN_OUTPUT_QUEUE_EMPTY - failed to dequeue from output queue
 *                          BSNN_OUTPUT_QUEUE_TIMEOUT - timeout waiting for output queue member
 *                          BSNN_ERR_GLB_BUF_FAIL - invalid shared buffer, e.g. activation buf, dag buf
 *                          BSNN_ERR_SYNC_FAIL - failed to sync DMA buffer
 */
bsnn_ret_t bsnn_instance_run(bsnn_user_instance_t *p_u_ins);


/*!
 * This API call returns the output tensors in parameter bsnn_user_output_t* p_u_out. Now only support batch size 1
 * It could be called in another thread than API bsnn_instance_run() for the same instance.
 * @param[in]   p_u_ins     Pointer to an existing instance.
 * @param[out]  p_u_out     pointer to output tensors.
 * @param[out]  p_post_idx  pointer to index of output tensors in output buffer queue.
 *                          The ownership of this buffer will be handle over to calller.
 *                          and the caller is expect to release with this parameter.
 * @return                  BSNN_SUCC - success
 *                          BSNN_ERR_BATCH - function can not support model with batch size greater than 1
 *                          BSNN_ERR_DEVICE_UNAVAILABLE - invalid driver status
 *                          BSNN_ERR_PARAM_INVALID - invalid input parameter
 *                          BSNN_OUTPUT_QUEUE_EMPTY - failed to dequeue from output queue
 *                          BSNN_OUTPUT_QUEUE_TIMEOUT - timeout waiting for output queue member
 */
bsnn_ret_t bsnn_output_get(bsnn_user_instance_t *p_u_ins, bsnn_user_output_t* p_u_out,
			   int* p_post_idx);


/*!
 * This API call returns the output tensors in parameter bsnn_user_output_t* p_u_out.
 * It could be called in another thread than API bsnn_instance_run() for the same instance.
 * Compatible with all batch size.
 * @param[in]   p_u_ins         Pointer to an existing instance.
 * @param[out]  p_u_out_batch   pointer to output tensors batch.
 * @param[out]  p_post_idx      pointer to index of output tensors in output buffer queue.
 *                              The ownership of this buffer will be handle over to calller.
 *                              and the caller is expect to release with this parameter.
 * @return                      BSNN_SUCC - success
 *                              BSNN_ERR_DEVICE_UNAVAILABLE - invalid driver status
 *                              BSNN_ERR_PARAM_INVALID - invalid input parameter
 *                              BSNN_OUTPUT_QUEUE_EMPTY - failed to dequeue from output queue
 *                              BSNN_OUTPUT_QUEUE_TIMEOUT - timeout waiting for output queue member
 */
bsnn_ret_t bsnn_output_get_batch(bsnn_user_instance_t *p_u_ins, bsnn_user_output_batch_t *p_u_out_batch, int *p_post_idx);


/*!
 * Releases the buffer that are acquired in API bsnn_output_get().
 * @param[in]   p_u_ins     Pointer to an existing instance.
 * @param[in]  p_post_idx   Index of output tensors to be released
 * @return                  BSNN_SUCC - success
 *                          BSNN_ERR_DEVICE_UNAVAILABLE - invalid driver status
 *                          BSNN_ERR_PARAM_INVALID - invalid input parameter
 *                          BSNN_ERR_SYNC_FAIL - failed to sync DMA buffer
 */
bsnn_ret_t bsnn_output_release(bsnn_user_instance_t *p_u_ins, int post_idx);


/*!
 * Destroys the speicified instance and frees its resources.
 * @param[in]   p_u_ins     Pointer to an existing instance to be destroyed.
 * @return                  BSNN_SUCC - success
 *                          BSNN_ERR_DEVICE_UNAVAILABLE - invalid driver status
 *                          BSNN_ERR_PARAM_INVALID - invalid input parameter
 */
bsnn_ret_t bsnn_instance_destroy(bsnn_user_instance_t *p_u_ins);


/*!
 *  Unloads a model.
 *  If some instances are generated based on this model, these instances will also be destroyed.
 * @param[in]   p_u_model   pointer to an existed model structure.
 * @return                  BSNN_SUCC - success
 *                          BSNN_ERR_DEVICE_UNAVAILABLE - invalid driver status
 *                          BSNN_ERR_PARAM_INVALID - invalid input parameter
 */
bsnn_ret_t bsnn_model_unload(bsnn_user_model_t* p_u_model);


/*!
 *  Releases all resources in BSNN. If some models are not unloaded,
 *  It will unload all models including all instances that are generated based these models.
 * @return                  BSNN_SUCC - success
 *                          BSNN_ERR_DEVICE_UNAVAILABLE - invalid driver status
 */
bsnn_ret_t bsnn_all_destroy();


/*!
 * Query the information about model or others. see bsnn_query_cmd.
 * @param[in]   p_u_model   The pointer to an existing model
 * @param[in]   cmd         The command of query
 * @param[out]  info        The pointer to an information buffer
 * @param[in]   size        The size of the information buffer
 * @return                  BSNN_SUCC - success
 *                          BSNN_ERR_DEVICE_UNAVAILABLE - invalid driver status
 *                          BSNN_ERR_PARAM_INVALID - invalid input parameter
*/
bsnn_ret_t bsnn_model_query(bsnn_user_model_t *p_u_model, bsnn_query_cmd_t cmd,
		     void* info, uint32_t size);


/*!
 * Obtain the information to a specified input tensor of an instance.  Now only support batch size 1
 * @param[in]   p_u_ins     The pointer to an existing instance.
 * @param[in]   queue_idx   The index of the input buffer in the queue.
 * @param[in]   ts_idx      The index of the input tensor in the buffer.
 * @param[out]  ts          The pointer to a preallcoated tensor information
 *                          structure.
 * @return                  BSNN_SUCC - success
 *                          BSNN_ERR_BATCH - function can not support model with batch size greater than 1
 *                          BSNN_ERR_DEVICE_UNAVAILABLE - invalid driver status
 *                          BSNN_ERR_PARAM_INVALID - invalid input parameter
 */
bsnn_ret_t bsnn_get_input_ts(bsnn_user_instance_t *p_u_ins, int queue_idx, int ts_idx, bsnn_user_tensor_t *ts);


/*!
 * Obtain the information to a specified input tensor of an instance. Compatible with all batch size.
 * @param[in]   p_u_ins     The pointer to an existing instance.
 * @param[in]   queue_idx   The index of the input buffer in the queue.
 * @param[in]   batch_idx   The index of the input buffer in the batch.
 * @param[in]   ts_idx      The index of the input tensor in the buffer.
 * @param[out]  ts          The pointer to a preallcoated tensor information
 *                          structure.
 * @return                  BSNN_SUCC - success
 *                          BSNN_ERR_DEVICE_UNAVAILABLE - invalid driver status
 *                          BSNN_ERR_PARAM_INVALID - invalid input parameter
 */
bsnn_ret_t bsnn_get_input_ts_batch(bsnn_user_instance_t *p_u_ins, int queue_idx, int batch_idx, int ts_idx, bsnn_user_tensor_t *ts);


/*!
 * Obtain the information of a specified output tensor of an instance. Now only support batch size 1
 * @param[in]   p_u_ins     The pointer to an existing instance.
 * @param[in]   queue_idx   The index of the input buffer in the queue.
 * @param[in]   ts_idx      The index of the input tensor in the buffer.
 * @param[out]  ts          The pointer to a preallcoated tensor information
 *                          structure.
 * @return                  BSNN_SUCC - success
 *                          BSNN_ERR_BATCH - function can not support model with batch size greater than 1
 *                          BSNN_ERR_DEVICE_UNAVAILABLE - invalid driver status
 *                          BSNN_ERR_PARAM_INVALID - invalid input parameter
 */
bsnn_ret_t bsnn_get_output_ts(bsnn_user_instance_t *p_u_ins, int queue_idx, int ts_idx, bsnn_user_tensor_t *ts);


/*!
 * Obtain the information of a specified output tensor of an instance. Compatible with all batch size.
 * @param[in]   p_u_ins     The pointer to an existing instance.
 * @param[in]   queue_idx   The index of the input buffer in the queue.
 * @param[in]   batch_idx   The index of the input buffer in the batch.
 * @param[in]   ts_idx      The index of the input tensor in the buffer.
 * @param[out]  ts          The pointer to a preallcoated tensor information
 *                          structure.
 * @return                  BSNN_SUCC - success
 *                          BSNN_ERR_DEVICE_UNAVAILABLE - invalid driver status
 *                          BSNN_ERR_PARAM_INVALID - invalid input parameter
 */
bsnn_ret_t bsnn_get_output_ts_batch(bsnn_user_instance_t *p_u_ins, int queue_idx, int batch_idx, int ts_idx, bsnn_user_tensor_t *ts);


/*!
 * Obtain the buffer size of an input tensor of the model.
 * @param[in]   p_u_model   The pointer to an existing model.
 * @param[in]   ts_idx      The index of the input tensor.
 * @return                  The size of the input tensor, 0 for error.
 */
uint32_t bsnn_get_input_buf_sz(bsnn_user_model_t *p_u_model, int ts_idx);


/*!
 * Obtain the buffer size of an input tensor of the model.
 * @param[in]   p_u_model   The pointer to an existing model.
 * @param[in]   ts_idx      The index of the input tensor.
 * @return                  The size of the output tensor, 0 for error.
 */
uint32_t bsnn_get_output_buf_sz(bsnn_user_model_t *p_u_model, int ts_idx);


/*!
 * Obtain the quantization scale of an input tensor of the model.
 * @param[in]   p_u_model   The pointer to an existing model.
 * @param[in]   ts_idx      The index of the input tensor.
 * @return                  The dequantization scale of the tensor, 0 for error.
 */
float bsnn_get_input_qnt_scale(bsnn_user_model_t *p_u_model, int ts_idx);


/*!
 * Obtain the quantization scale of an output tensor of the model.
 * @param[in]   p_u_model   The pointer to an existing model.
 * @param[in]   ts_idx      The index of the output tensor.
 * @return                  The dequantization scale of the tensor, 0 for error.
 */
float bsnn_get_output_qnt_scale(bsnn_user_model_t *p_u_model, int ts_idx);


/*!
 * Obtain the total amount of the per-channel dequantization scales of an input tensor of the model.
 * @param[in]   p_u_model   The pointer to an existing model.
 * @param[in]   ts_idx      The index of the input tensor.
 * @param[out]  num			The total amount of the dequant scales.
 * @return                  BSNN_SUCC - success
 *							            BSNN_ERR_FAIL - invalid API used
 *                          BSNN_ERR_DEVICE_UNAVAILABLE - invalid driver status
 *                          BSNN_ERR_PARAM_INVALID - invalid input parameter
 */
bsnn_ret_t bsnn_get_input_deqnt_scale_num(bsnn_user_model_t *p_u_model, int ts_idx, uint32_t *num);


/*!
 * Obtain the per-channel dequantization scales of an input tensor of the model.
 * @param[in]   p_u_model   The pointer to an existing model.
 * @param[in]   ts_idx      The index of the input tensor.
 * @param[out]  scales      The pointer to the dequant scales.
 * @return                  BSNN_SUCC - success
 *							            BSNN_ERR_FAIL - invalid API used
 *                          BSNN_ERR_DEVICE_UNAVAILABLE - invalid driver status
 *                          BSNN_ERR_PARAM_INVALID - invalid input parameter
 */
bsnn_ret_t bsnn_get_input_deqnt_scales(bsnn_user_model_t *p_u_model, int ts_idx, float *scales);


/*!
 * Obtain the total amount of the per-channel dequantization scales of an output tensor of the model.
 * @param[in]   p_u_model   The pointer to an existing model.
 * @param[in]   ts_idx      The index of the output tensor.
 * @param[out]  num			    The total amount of the dequant scales.
 * @return                  BSNN_SUCC - success
 *							            BSNN_ERR_FAIL - invalid API used
 *                          BSNN_ERR_DEVICE_UNAVAILABLE - invalid driver status
 *                          BSNN_ERR_PARAM_INVALID - invalid input parameter
 */
bsnn_ret_t bsnn_get_output_deqnt_scale_num(bsnn_user_model_t *p_u_model, int ts_idx, uint32_t *num);


/*!
 * Obtain the per-channel dequantization scales of an output tensor of the model.
 * @param[in]   p_u_model   The pointer to an existing model.
 * @param[in]   ts_idx      The index of the output tensor.
 * @param[out]  scales      The pointer to the dequant scales.
 * @return                  BSNN_SUCC - success
 *							BSNN_ERR_FAIL - invalid API used
 *                          BSNN_ERR_DEVICE_UNAVAILABLE - invalid driver status
 *                          BSNN_ERR_PARAM_INVALID - invalid input parameter
 */
bsnn_ret_t bsnn_get_output_deqnt_scales(bsnn_user_model_t *p_u_model, int ts_idx, float *scales);


/*!
 * Check if the asic type is A0.
 * @return                  true - Current board's asic type is A0
 *							            false - Current board's asic type is not A0
 */
bool bsnn_asic_type_is_a0();


/*!
 * Check if the asic type is B0.
 * @return                  true - Current board's asic type is B0
 *							            false - Current board's asic type is not B0
 */
bool bsnn_asic_type_is_b0();


/*!
 *  Check model has edp/gemm
 * @param[in]   p_u_model   The pointer to an existing model.
 * @return                  true - Input model has edp/gemm
 *                          false - Either input model does not exist or does not have edp/gemm 
 */
bool bsnn_model_has_edp_gemm(bsnn_user_model_t *p_u_model);


/*!
 * This function updates the DMA address of input buffer and pitch1 when user
 * cropping image from the input buffer, both Y and UV channels are
 * cropped hence the DMA address and the channel height are updated in
 * accordance
 *
 * @param[in]   p_u_ins		The pointer to an existing instance.
 *				x_offset	position of the cropping starting point on x-axis,
 *							must be 16-byte aligned
 *				y_offset	position of the cropping starting point on y-axis,
 *							must be multiple of 2
 * @return                  BSNN_SUCC - success
 *                          BSNN_ERR_PARAM_INVALID - invalid input parameter
 */
bsnn_ret_t bsnn_crop_yuv_input(bsnn_user_instance_t *p_u_ins, uint32_t x_offset, uint32_t y_offset);


/*!
 * This function gets the offset of the cropped input tensors
 *
 * @param[in]   p_u_ins		The pointer to an existing instance.
 * @param[out]	x_offset	pointer to the position of the cropping starting point on x-axis, 
 *							could be an array when number of input tensors of
 *							this model is bigger than one
 *				y_offset	pointer to the position of the cropping starting point on y-axis,
 *							could be an array when number of input tensors of
 *							this model is bigger than one
 * @return                  BSNN_SUCC - success
 *                          BSNN_ERR_DEVICE_UNAVAILABLE - invalid driver status
 *                          BSNN_ERR_PARAM_INVALID - invalid input parameter
 */
bsnn_ret_t bsnn_get_input_crop(bsnn_user_instance_t *p_u_ins, uint32_t *x_offset, uint32_t *y_offset);


/*!
 * This function checks if the empty output buffer queue is empty
 * this queue is dequeued in bsnn_instance_run, and enqueued in
 * bsnn_output_release
 * user can use this API to monitor output buffer queue when post-process is on
 * a separate thread
 *
 * @param[in]   p_u_ins		The pointer to an existing instance.
 * @param[out]  is_empty    The pointer to pass empty status.
 * @return                  BSNN_SUCC - success
 *                          BSNN_ERR_DEVICE_UNAVAILABLE - invalid driver status
 *                          BSNN_ERR_PARAM_INVALID - invalid input parameter
 */
bsnn_ret_t bsnn_empty_output_queue_check(bsnn_user_instance_t *p_u_ins, bool *is_empty);


/*!
 * This function passes physical address of model files(.lib, .meta. weights.bin) directly to model processing
 *
 * @param[in]   file_stat_list_in	The pointer to the wrapped data structure that contains the location of the file 
 *                          	in memory and the size of the file.
 * @param[in]   u_model   	The pointer to an existing model.
 * @param[in]   nums_of_files number of files
 * @return                  	BSNN_SUCC - success
 *                          	BSNN_ERR_FAIL - fail
 */
bsnn_ret_t bsnn_memory_load(bsnn_file_in_memory *file_stat_list_in, bsnn_user_model_t *u_model, int nums_of_files);


/*!
 * This function reads the model information that contains the tensor input & output number with each of its name. 
 * User/Developer could get a string array contains the string name of every tensor
 *
 * @param[in]   u_model	      The pointer to an existing model.
 *                          	
 * @param[out]  tensor_num    	The pointer that will output the input tensor number.
 *
 * @param[out]  namelist       The pointer that will output the input tensor name list.
 *
 * @return                  	BSNN_SUCC - success
 *                          	BSNN_ERR_FAIL - fail
 */
bsnn_ret_t bsnn_get_input_tensor_names( bsnn_user_model_t *u_model, uint32_t *tensor_num,  std::vector<std::string>& namelist);



/*!
 * This function reads the model information that contains the tensor input & output number with each of its name. 
 * User/Developer could get a string array contains the string name of every tensor
 *
 * @param[in]   u_model	      The pointer to an existing model.
 *                          	
 * @param[out]  tensor_num    	The pointer that will output the output tensor number.
 *
 * @param[out]  namelist       The pointer that will output the output tensor name list.
 *
 * @return                  	BSNN_SUCC - success
 *                          	BSNN_ERR_FAIL - fail
 */ 
bsnn_ret_t bsnn_get_output_tensor_names( bsnn_user_model_t *u_model, uint32_t *tensor_num,  std::vector<std::string>& namelist);



/*
* This function reads the config and model information into the static message between bsnn and lwnn.
*
* @param[in] p_u_model  The pointer to an existing model.
*
* @param[out]  p_config The input of the hyper parameters we give to the model before it starts running
*
* @param[out] p_msg_to_next_model The output message
*
*/
bsnn_ret_t bsnn_set_bsnn_lwnn_msg_static(bsnn_user_model_t *p_u_model, scheduler_config_t *p_config, bsnn_lwnn_msg_t_static *p_msg_to_next_model);


/*
* This function will match the two name list of the input tensor name list and output tensor name list 
* it will generate out a 2D vector that represent the vector
* @param[in] prev_namelist     The addres of the name list of output tensor of the previous model segement 
* 
* @param[in] cur_namelist      The address of the name list of input tensor of the current model segment
*
* @param[out] output_map       The output string map
*
*/
bsnn_ret_t bsnn_lwnn_tensor_match(vector<string> &prev_namelist, vector<string> &cur_namelist, vector<pair<uint32_t, uint32_t>> &output_map);


 /*!
 * This function reads the model information and fetch all the output tensor sizes and pitches out
 * The input of the function is the model and output will be vector of each dimension sizes
 *
 * @param[in]   u_model	      The pointer to an existing model.
 *                          	
 * @param[out]  osize0, osize1, osize2    	
 *              opitch0, opitch1, opitch2    Those vectors will store all the output tensors size & pitch information
 *
 * @return                  	BSNN_SUCC - success
 *                          	BSNN_ERR_FAIL - fail
 */ 
 bsnn_ret_t bsnn_get_tflite_msg(bsnn_user_model_t *u_model, struct tflite_recv_msg *tflite_msg);

/*!
 * This function read the Original function name and then return a name that allow dlopen to call.
 *
 * @param[in]   O_func_name	      The pointer to an Original function name.
 *
 * @return                       	a pointer to the c++ mangled name.
 *                          	
 */ 
extern "C" {
const char* get_mangle_name(const char* O_func_name);
}

#ifdef BSNN_BIST
/*!
 * Runs a software BIST test to see if NET engines work well
 * @param[out]				The pointer to test status
 * @return                  BSNN_SUCC - success
 *                          BSNN_ERR_FAIL - bsnn software BIST test failed
 *                          BSNN_ERR_PARAM_INVALID - NULL bist_status pointer
 *                          BSNN_ERR_DEVICE_UNAVAILABLE - invalid driver status
 *                          BSNN_ERR_TIMEOUT - SW BIST test hangs at FW
 */
bsnn_ret_t bsnn_sw_bist_run(uint32_t *bist_status);
#endif
#endif  //_BSNN_USER_API_H
