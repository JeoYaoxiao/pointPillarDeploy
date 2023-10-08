#ifndef BADAS_BSNN_H_
#define BADAS_BSNN_H_

#include <memory>
#include <string>

#include "badas_base.h"
#include "badas_lane_proc.h"
#include "bsnn/bsnn_user_api.h"

#define BSNN_VERSION_CHECK(ver, major, minor, patch)                \
    (((ver).bsnn_major > major) ||                                  \
     (((ver).bsnn_major == major) && ((ver).bsnn_minor > minor)) || \
     (((ver).bsnn_major == major) && ((ver).bsnn_minor == minor) && \
      ((ver).bsnn_patch >= patch)))

namespace Badas {

class Badas_BSNN : public Badas_Base {
   public:
    Badas_BSNN(int net_ver = 6);
    ~Badas_BSNN();

    bsnn_version_t* GetBsnnVer();

    void SetChip(bool if_b0);
    bool GetChip();

    void SetDim(int w, int h);

    static bsnn_user_model_t u_model_;
    static int LoadModel(std::string model_path);
    static int UnloadModel();

    int LoadInstance(std::string nn_path, bsnn_user_instance_cfg_t* cfg = NULL);
    int UnloadInstance();

    int QueryModel();

    int SetInput(uint8_t* in_buffer);  // copy to cachable

    int InputGet(bsnn_user_input_t& bsnn_in);  // Get BSNN buffer
    int InputSet(bsnn_user_input_t& bsnn_in);  // flush to DDR, no copy need it

    int Detect(uint8_t* in_buffer = NULL);

    //
    int GetOutput(bsnn_user_output_t& bsnn_out, int& bsnn_out_idx);
    int RelOutput(int bsnn_out_idx);

    //
    int ProcObject(bsnn_user_output_t& bsnn_out,
                   std::vector<BBoxInfo>& bbox_info, bool track_en);

    void CheckCSC4TLRTSR(const cv::Mat& cur_frame);

    int ProcTrafficLight(std::vector<BBoxInfo>& bbox_info,
                         const cv::Mat& net_frm, const cv::Mat& cur_frame,
                         const int disp_x_off, const int disp_y_off,
                         const float disp_x_ratio, const float disp_y_ratio);
    int ProcTrafficLight(BBoxInfo& s_bbox, const cv::Mat& net_frm,
                         const cv::Mat& cur_frame, const int disp_x_off,
                         const int disp_y_off, const float disp_x_ratio,
                         const float disp_y_ratio);

    int ProcTrafficSign(std::vector<BBoxInfo>& bbox_info,
                        const cv::Mat& net_frm, const cv::Mat& cur_frame,
                        const int disp_x_off, const int disp_y_off,
                        const float disp_x_ratio, const float disp_y_ratio);

    int ProcTrafficSign(BBoxInfo& b_bbox, const cv::Mat& net_frm,
                        const cv::Mat& cur_frame, const int disp_x_off,
                        const int disp_y_off, const float disp_x_ratio,
                        const float disp_y_ratio);

    int SetObjNmsThreshold(std::vector<float> nms_thres);

    // until NetV6
    int ProcLane(bsnn_user_output_t& bsnn_out,
                 std::vector<std::pair<float, float>>& lane_out);

    // since NetV7
    int ProcLane(bsnn_user_output_t& bsnn_out, LaneDetectionResult& results,
                 const bool track_enable = true, const int& track_num = 1,
                 const int& output_num_pts_per_lane = 100,
                 const bool if_inner_lane = false);

    // for NetV7 fast version
    int ProcLane(bsnn_user_output_t& bsnn_out,
                 std::vector<std::pair<float, float>>& lane_out,
                 std::vector<std::pair<float, float>>& curbside_out);

    int SetCameraInfo(CameraInfoHolder camera_info, int img_top_offset);

    int SetLanenessThreshold(float lane_thres);

    int SetCurbnessThreshold(float curb_thres);

    int ProcFreeSpace(bsnn_user_output_t& bsnn_out,
                      std::vector<std::pair<float, float>>& out_free_space,
                      bool if_reduce_pts);

    int ProcNet(bsnn_user_output_t& bsnn_out);
    int ProcStopLine(bsnn_user_output_t& bsnn_out, StoplineInfo& out_stop_line);
    int ProcCrossWalk(bsnn_user_output_t& bsnn_out,
                      CrosswalkInfo& out_crosswalk);
    int ProcPole(bsnn_user_output_t& bsnn_out,
                 std::vector<std::vector<cv::Point>>& out_pole);
    int ProcArrow(bsnn_user_output_t& bsnn_out, std::vector<ArrowInfo>& out_aw);

    void SetTLR(bool if_tlr, std::string TLR_meta_path, int debug_level,
                bool if_output);
    void SetTSR(const int& if_tsr, std::string TSR_meta_path, int debug_level,
                bool if_output);

    void SetDebugLevel(int debug_level);

    int SetNetVersion(int net_ver);
    int GetNetVersion() { return net_version_; };

    int StoreTensor(bsnn_user_tensor_t* tensor_st, char* fn_st);

    static void CheckVersion();

    //
    void SetDump(bool if_dump) { if_dump_ = if_dump; }
    void SetOutputPath(std::string the_output_path)
    {
        output_path_ = the_output_path;
    }

   protected:
    int AllocOutputBuf();  // for overlapping postprocessing, a tentative
                           // solution, to be deprecated once bsnn ready
    int FreeOutputBuf();

   private:
    class Badas_BSNN_Impl;
    std::shared_ptr<Badas_BSNN_Impl> pimpl_;

    // bsnn_user_model_t ;
    bsnn_user_instance_t u_instance_;

    bsnn_version_t bsnn_ver;

    std::vector<float> qnt_scales_;

    int dag_index_map_[18];

    //
    bool if_dump_;
    bool if_b0_;
    std::string output_path_;

    bool initialized_;

    int input_proc_stage_;  // 0: init stage, the Badas_BSNN is just created;
                            // 1: ProcObject
                            // 2: after first time
                            // ProcTrafficLight/ProcTrafficSign called;
    cv::Mat cur_frame_rgb;

    //
    std::vector<std::vector<float>> shared_fs_pts_;

    //
    std::vector<std::vector<float>> shared_sl_pts_;
    float stopline_score_;  // reference by lane
    float crosswalk_score_;
};

}  // namespace Badas

#endif  // BADAS_BSNN_H_