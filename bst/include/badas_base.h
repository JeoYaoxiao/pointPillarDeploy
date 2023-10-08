/*******************************************************************************
 * Copyright(c) Black Sesame Technologies Inc., All right reserved.
 *
 * No part of this file may be distributed, duplicated or transmitted in any
 *form or by any means, without prior consent of Black Sesame Technologies Inc.
 *
 * This file is  property. It contains BST's trade secret, proprietary
 * and confidential information.
 *
 *********************************************************************************/

#ifndef BADAS_BASE_H_
#define BADAS_BASE_H_

#include <mutex>

#include "badas_json.h"
#include "opencv2/opencv.hpp"

typedef struct {
    bool wheeline_valid;
    bool anglePoint_valid;
    float front_point_x;
    float front_point_y;
    float rear_point_x;
    float rear_point_y;
    float angle_point_x;
} BBox3dInternal;

namespace Badas {

extern std::mutex g_prn_mtx;

////////////////////////////////////////////////////////////////////////////////
class Badas_Base {
   public:
    Badas_Base();

    ~Badas_Base(){};

   public:
    void DbgPrintN(int lvl, const char* fmt, ...);

    void DbgPrint(int lvl, const char* fmt, ...);
    void ErrPrint(const char* fmt, ...);

    virtual void SetDebugLevel(int debug_level) { dbg_lvl_ = debug_level; };

    int GetDebugLevel() { return dbg_lvl_; }

    void SetChip(bool if_b0);
    bool GetChip();

    void SetDim(int w, int h);
    //
    inline static float sigmoid(float x) { return (1.0 / (1 + exp(-x))); }

    //
    inline static double fast_exp(double y)
    {
        double d = (double)0LL;
        *(reinterpret_cast<int*>(&d) + 1) =
            static_cast<int>(1512775 * y + 1072632447);
        return d;
    }

    inline static float fast_sigmoid(float x)
    {
        return (1.0 / (1 + fast_exp(-x)));
    }

    inline static float logit(float x) { return (log(x / (1 - x))); }
    //
    inline static bool compare_box_score(const BBoxInfo& a, const BBoxInfo& b)
    {
        return a.score > b.score;
    }

    static float badas_clampf(float value, float min, float max);
    static int badas_clamp(int value, int min, int max);

    static std::vector<float> badas_linspace(float a, float b, int k);

    static std::vector<float> badas_polyval(const float coefficients[3],
                                            const std::vector<float>& xValues);

    //
    std::string get_rgb_by_class(int classid, int& r, int& g, int& b);

    //
    std::string get_rgb_by_tlr_class(int tf_id, int& r, int& g, int& b);

    //
    std::string get_tsr_class(int tf_id);

    //
    std::string get_tsr2_class(int tsr_type);

    //
    std::string get_rgb_by_tf_class(int tf_id, int& r, int& g, int& b);

    //
    static void rgb2nv12_bt709(uint8_t* destination, uint8_t* rgb, size_t width,
                               size_t height);

    static void nv122rgb_bt709(uint8_t* destination, uint8_t* nv12,
                               size_t width, size_t height,
                               bool do_normalize = true);

    //
    void set_lane_draw_option(int lane_draw_option);

    //
    void DrawAdas(cv::Mat& display_image, const AdasInfo& adas_info,
                  int x_offset, int y_offset, float x_ratio, float y_ratio,
                  bool if_tlr, int if_tsr, bool if_inner_lane);

    void generatejson(json_obj* jobj, const AdasInfo& adas_info,
                      const std::vector<BBox3dInternal>& bbox3d_internal);

    int LoadBinaryNV12(const char* fn, uint8_t* nv_buf);

    int StoreBinary(const char* fn, uint8_t* data_buf, size_t data_len);

    int LoadWeight(const char* fn, float* weight_buf, const int w, const int h,
                   const int byte_size);

    void CvMatInfo(cv::Mat& cv_mat, std::string mat_name = "my_mat ");

    /*
      Draw points on image

      @param image Image
      @param pts Mat of the points
      @param if_curb If plotting curbside
      @param shape Shape/type of the lane
      @param color_map Color of the lane
      @param color Visualization color ID of the lane
      @param kernel Thickness of lane, kernel*2 = lane width when drawing
    */
    void DrawPointsMat(cv::Mat& image, const cv::Mat& pts, bool if_curb,
                       const int& shape, const int& color_map,
                       const cv::Vec3b& color, const int& kernel, int x_off,
                       int y_off, float x_rat, float y_rat, int img_width,
                       int img_height);

    /*
      Draw lane's shape and color on image

      @param image Image
      @param pts Mat of the points
      @param shape shape/type of the lane
      @param color color of the lane
      @param lane_prob confidence level of the lane
    */
    void DrawShapeColorProb(cv::Mat& image, const cv::Mat& pts,
                            const int& shape, const int& color,
                            const float& lane_prob, int x_offs, int y_off,
                            float x_ratio, float y_ratio);

    void DrawLaneWidth(cv::Mat& image, const cv::Mat& pts,
                       const cv::Mat& widths, const cv::Vec3b& color,
                       const int& kernel, int x_off, int y_off, float x_rat,
                       float y_rat, int img_width, int img_height);

    std::vector<std::string> GetImageList(const std::string& file_path);

   protected:
    /*
      Draw dashline

      @param image Image
      @param thickness the thickness of the dash line
      @param color color of the lane
    */
    void draw_dashline(cv::Mat& image, std::pair<float, float> startpoint,
                       std::pair<float, float> endpoint, int thickness,
                       const cv::Scalar& color);

    /*
      Draw 3D bounding box

      @param image Image
      @param valid_vertices_num valid vertices number
      @param bbox3d 3D bounding box in BBox3d type
      @param ort_ind oritentation index
      @param class_ind class index
    */
    void draw_3dbbox(cv::Mat& display_image, int valid_vertices_num,
                     BBox3d& bbox3d, uint8_t ort_ind, uint8_t class_ind,
                     int x_offset, int y_offset, float x_ratio, float y_ratio);

   protected:
    // general
    int ww_;  // image width, depends on the net's traning input size, no need
              // to change
    int hh_;  // image height, depends on net's traning input size, no need to
              // change
    int postp_ww_;
    int postp_hh_;

    std::map<int, std::string> lane_shape_map_;
    std::map<int, std::string> lane_color_map_;

    std::map<int, cv::Vec3b> color_map_;
    std::map<int, cv::Vec3b> curb_color_map_;

    int dbg_lvl_;
    int dbg_ln_cnt_;

    //
    int net_version_;
    bool if_fov100_;
    bool if_b0_;
    int lane_draw_option_;

   public:
    std::mutex& get_prn_mtx() { return g_prn_mtx; }
};

}  // namespace Badas
#endif  // BADAS_BASE_H_
