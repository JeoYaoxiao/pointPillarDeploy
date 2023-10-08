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
#ifndef BADAS_JSON_
#define BADAS_JSON_

#include <sys/stat.h>
#include <iomanip>
#include "badas.h"
#include "nlohmann/json.hpp"
using namespace std;

namespace Badas {

class json_obj {
   public:
    int disp_x_off;
    int disp_y_off;
    float disp_x_ratio;
    float disp_y_ratio;
    void init(int frame_height, int frame_width)
    {
        nlohmann::json json;
        json["metadata"]["date"] = "";
        json["metadata"]["time"] = "";
        json["metadata"]["GPS_Latitude"] = "";
        json["metadata"]["GPS_Longitude"] = "";
        json["metadata"]["weather"] = "";
        json["metadata"]["highway/local"] = "";
        json["metadata"]["province"] = "";
        json["metadata"]["city"] = "";
        json["metadata"]["road_name"] = "";
        json["metadata"]["image_height"] = frame_height;
        json["metadata"]["image_width"] = frame_width;

        annotations_O.clear();
        annotations_F.clear();
        annotations_L.clear();
        folder_json_O.push_back(json);
        folder_json_F.push_back(json);
        folder_json_L.push_back(json);
    };

    void set_scale(int xoff, int yoff, float xr, float yr)
    {
        disp_x_off = xoff;
        disp_y_off = yoff;
        disp_x_ratio = xr;
        disp_y_ratio = yr;
    }

    void set_path(string adas_input_path, string adas_output_path,
                  bool if_output, bool if_single_file, string &json_fc)
    {
        string json_folder_name;

        if (!if_single_file) {
            auto pos2 = adas_input_path.find_last_of('/');
            if (pos2 != std::string::npos) {
                auto pos3 = adas_input_path.substr(0, pos2).find_last_of('/');
                json_folder_name =
                    adas_input_path.substr(pos3 + 1, pos2 - pos3 - 1);
            } else
                json_folder_name = adas_input_path;
        }

        else {
            auto pos = adas_input_path.find_last_of('.');
            auto pos1 = adas_input_path.find_last_of('/');
            json_folder_name = adas_input_path.substr(pos1 + 1, pos - pos1 - 1);
        }
        json_O_file = json_fc + "/" + json_folder_name + "_O.json";
        json_F_file = json_fc + "/" + json_folder_name + "_F.json";
        json_L_file = json_fc + "/" + json_folder_name + "_L.json";
        cout << "json output dir ::  " << json_fc << endl;

        struct stat st = {0};
        if (stat(json_fc.c_str(), &st) == -1) {
            mkdir(json_fc.c_str(), 0777);
        }

        json_stream_O.open(json_O_file);
        json_stream_F.open(json_F_file);
        json_stream_L.open(json_L_file);
    };
    void json_push(string img_fn_cur)
    {
        img_json_O["annotations"] = annotations_O;
        img_json_O["class"] = "image";
        img_json_O["filename"] = img_fn_cur;

        img_json_F["annotations"] = annotations_F;
        img_json_F["class"] = "image";
        img_json_F["filename"] = img_fn_cur;

        img_json_L["annotations"] = annotations_L;
        img_json_L["class"] = "image";
        img_json_L["filename"] = img_fn_cur;

        folder_json_O.push_back(img_json_O);
        folder_json_F.push_back(img_json_F);
        folder_json_L.push_back(img_json_L);
    };
    void L_push(nlohmann::json &anno_L) { annotations_L.push_back(anno_L); };
    void F_push(nlohmann::json &anno_L) { annotations_F.push_back(anno_L); };
    void O_push(nlohmann::json &anno_L) { annotations_O.push_back(anno_L); };

    void save_json()
    {
        json_stream_O << std::setw(4) << folder_json_O << std::endl;
        json_stream_F << std::setw(4) << folder_json_F << std::endl;
        json_stream_L << std::setw(4) << folder_json_L << std::endl;
    };

    void json_point_record(const cv::Mat &pts, std::vector<int> &xn_v,
                           std::vector<int> &yn_v)
    {
        float x_rat = disp_x_ratio;
        float y_rat = disp_y_ratio;
        // cout << "**** debug :: " << disp_x_ratio << endl;
        int x_off = disp_x_off;
        int y_off = disp_y_off;

        for (int i = 0; i < pts.rows; i++) {
            int x = pts.at<unsigned short>(i, 0);
            int y = pts.at<unsigned short>(i, 1);
            x = x * x_rat + x_off;
            y = y * y_rat + y_off;
            xn_v.push_back(x);
            yn_v.push_back(y);
        }
    };

   private:
    string json_O_file;
    string json_F_file;
    string json_L_file;
    std::ofstream json_stream_O;
    std::ofstream json_stream_F;
    std::ofstream json_stream_L;

    nlohmann::json folder_json_O;
    nlohmann::json folder_json_F;
    nlohmann::json folder_json_L;
    nlohmann::json annotations_O;
    nlohmann::json annotations_F;
    nlohmann::json annotations_L;
    nlohmann::json img_json_O;
    nlohmann::json img_json_F;
    nlohmann::json img_json_L;
};

}  // namespace Badas
#endif  // BADAS_JSON_
