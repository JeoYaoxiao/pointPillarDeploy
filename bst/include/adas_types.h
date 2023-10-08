#ifndef ADAS_TYPES_H_
#define ADAS_TYPES_H_

namespace Badas {

#define ENV_BST_ADAS_ROOT "BST_ADAS_ROOT"
#define ENV_BST_ADAS_DATA "BST_ADAS_DATA"

#define DEFAULT_ROOT_DIR "."
#define DEFAULT_DATA_DIR "./models"
#define MODEL_PREFIX "model"
#define MODEL_BASE_NAME "bsnn-model-1280x640-v"
#define PARAMETER_PREFIX "parameter"

#define MAX_DISPLAY_WIDTH 1920
#define MAX_DISPLAY_HEIGHT 1080

// pic size needed by net
#define NET_PICTURE_WIDTH 1280
#define NET_PICTURE_HEIGHT 640

#define NET_FOV100PICTURE_WIDTH 1280
#define NET_FOV100PICTURE_HEIGHT 512

// a suitable size scale to net pic size
#define CROP_WIDTH 1920
#define CROP_HEIGHT 960

#define SECOND_TO_USECOND 1000000

//#define TIMECOUNT

static const int MAJOR_VERSION = 2;
static const int MINOR_VERSION = 7;
static const int PATCH_VERSION = 0;

static const int SAVE_PIC_START_FRAME = 100;  // save pic from 100th frame
static const int SAVE_PIC_END_FRAME = 300;    // save pic end at 300th frame
static const int SAVE_PIC_INTERVAL = 50;      // every 50 frame save one pic

static const int WINDOWN_RECT_1920_1080 = 1080;
static const int WINDOWN_RECT_1920_960 = 960;
static const int WINDOWN_RECT_1280_640 = 640;

typedef enum OMX_COLOR_FORMATTYPE {
    OMX_COLOR_FormatUnused,
    OMX_COLOR_FormatMonochrome,
    OMX_COLOR_Format8bitRGB332,
    OMX_COLOR_Format12bitRGB444,
    OMX_COLOR_Format16bitARGB4444,
    OMX_COLOR_Format16bitARGB1555,
    OMX_COLOR_Format16bitRGB565,
    OMX_COLOR_Format16bitBGR565,
    OMX_COLOR_Format18bitRGB666,
    OMX_COLOR_Format18bitARGB1665,
    OMX_COLOR_Format19bitARGB1666,
    OMX_COLOR_Format24bitRGB888,
    OMX_COLOR_Format24bitBGR888,
    OMX_COLOR_Format24bitARGB1887,
    OMX_COLOR_Format25bitARGB1888,
    OMX_COLOR_Format32bitBGRA8888,
    OMX_COLOR_Format32bitARGB8888,
    OMX_COLOR_FormatYUV411Planar,
    OMX_COLOR_FormatYUV411PackedPlanar,
    OMX_COLOR_FormatYUV420Planar,
    OMX_COLOR_FormatYUV420PackedPlanar,
    OMX_COLOR_FormatYUV420SemiPlanar,
    OMX_COLOR_FormatYUV422Planar,
    OMX_COLOR_FormatYUV422PackedPlanar,
    OMX_COLOR_FormatYUV422SemiPlanar,
    OMX_COLOR_FormatYCbYCr,
    OMX_COLOR_FormatYCrYCb,
    OMX_COLOR_FormatCbYCrY,
    OMX_COLOR_FormatCrYCbY,
    OMX_COLOR_FormatYUV444Interleaved,
    OMX_COLOR_FormatRawBayer8bit,
    OMX_COLOR_FormatRawBayer10bit,
    OMX_COLOR_FormatRawBayer8bitcompressed,
    OMX_COLOR_FormatL2,
    OMX_COLOR_FormatL4,
    OMX_COLOR_FormatL8,
    OMX_COLOR_FormatL16,
    OMX_COLOR_FormatL24,
    OMX_COLOR_FormatL32,
    OMX_COLOR_FormatYUV420PackedSemiPlanar,
    OMX_COLOR_FormatYUV422PackedSemiPlanar,
    OMX_COLOR_Format18BitBGR666,
    OMX_COLOR_Format24BitARGB6666,
    OMX_COLOR_Format24BitABGR6666,
    OMX_COLOR_FormatKhronosExtensions =
        0x6F000000, /**< Reserved region for introducing Khronos Standard
                       Extensions */
    OMX_COLOR_FormatVendorStartUnused =
        0x7F000000, /**< Reserved region for introducing Vendor Extensions */
    /**<Reserved android opaque colorformat. Tells the encoder that
     * the actual colorformat will be  relayed by the
     * Gralloc Buffers.
     * FIXME: In the process of reserving some enum values for
     * Android-specific OMX IL colorformats. Change this enum to
     * an acceptable range once that is done.
     * */
    OMX_COLOR_FormatAndroidOpaque = 0x7F000789,
    OMX_COLOR_Format32BitRGBA8888 = 0x7F00A000,
    /** Flexible 8-bit YUV format.  Codec should report this format
     *  as being supported if it supports any YUV420 packed planar
     *  or semiplanar formats.  When port is set to use this format,
     *  codec can substitute any YUV420 packed planar or semiplanar
     *  format for it. */
    OMX_COLOR_FormatYUV420Flexible = 0x7F420888,

    OMX_TI_COLOR_FormatYUV420PackedSemiPlanar = 0x7F000100,
    OMX_QCOM_COLOR_FormatYVU420SemiPlanar = 0x7FA30C00,
    OMX_QCOM_COLOR_FormatYUV420PackedSemiPlanar64x32Tile2m8ka = 0x7FA30C03,
    OMX_SEC_COLOR_FormatNV12Tiled = 0x7FC00002,
    OMX_QCOM_COLOR_FormatYUV420PackedSemiPlanar32m = 0x7FA30C04,
    OMX_COLOR_FormatMax = 0x7FFFFFFF

} OMX_COLOR_FORMATTYPE;

#ifndef MAX_PATH_LEN
#define MAX_PATH_LEN 256
#endif

enum BufferFlags {
    BUFFER_FLAG_SYNCFRAME = 1,
    BUFFER_FLAG_CODECCONFIG = 2,
    BUFFER_FLAG_EOS = 4,
    BUFFER_FLAG_EXTRADATA = 0x1000,
    BUFFER_FLAG_DATACORRUPT = 0x2000,
};

enum {
    INPUT_TYPE_VIDEO = 1,
    INPUT_TYPE_CAMERA = 2,
};

enum {
    CAMERA_FOV_INVALID = -1,
    CAMERA_FOV_1080 = 0,  // 1920 x 1080
    CAMERA_FOV_800 = 1,   // 1280 x 800
    CAMERA_FOV_720 = 2,   // 1280 x 720
};

typedef struct {
    float f_x;
    float f_y;
    float u_x;
    float u_y;
    float camera_height;
    float pitch;
    float yaw;
    float vanishing_y;  // y coordinate of vanishing point in net's input image
                        // (after preprocessing, 1280x640)
} CameraInfoHolder;

typedef struct {
    const char *camera_param_file;
    const char *video_filename;
    int model_version;
    int input_type;
    int window_size;
    int camera_fov;  // camera fov of camera input and video file
    bool lane_fit;
    bool loop_run;
} AdasConfig_t;

}  // namespace Badas

#endif  // ADAS_TYPES_H_