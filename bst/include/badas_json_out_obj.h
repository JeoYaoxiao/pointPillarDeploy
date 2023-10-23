#include <stdlib.h>
#include <string>
#include "badas_base.h"
#include "nlohmann/json.hpp"

#define IOUTHRES 0.9f

#define MAX_FLOAT 10000000.

class Singleton {
   private:
    Singleton()
    {
        g_frame_num = 0;
        g_input_image_name = "";
        g_Json = false;
    }
    ~Singleton() {}

   public:
    static Singleton *New()
    {
        static Singleton *pSingleton = NULL;

        if (!pSingleton) {
            pSingleton = new Singleton();
        }

        return pSingleton;
    }
    int g_frame_num;
    std::string g_input_image_name;
    int g_Json;
};

typedef struct AdasCompareResult {
    int difftype;  // 0 diff 1 same 2 different compare frame

    // objects
    int numObjsIn0ButNotIn1;  // including as same position but different class;
    int numObjsIn1ButNotIn0;  // see above
    int numObjsInBoth;
    float PSNR4IOU;

    // lanes
    int numLanesIn0ButNotIn1;  //
    int numLanesIn1ButNotIn0;  // see above
    int numLanesInBoth;
    float Diff4Lanes;  // distance, length, can be estimated by the area
                       // enclosed by the two lanes;

    // curb
    int numCurbsIn0ButNotIn1;  //
    int numCurbsIn1ButNotIn0;  // see above
    int numCurbsInBoth;
    float Diff4Curbs;  // distance, length, can be estimated by the area
                       // enclosed by the two lanes;

    // freespace
    float Diff4Freespace;  // distance, length, can be estimated by the area
                           // enclosed by the two freespace;

    // stopline
    float diffBegin;
    float diffEnd;
    int numPolyGonsIn0ButNotIn1;  // including as same position but different
                                  // class;
    int numPolyGonsIn1ButNotIn0;  // see above
    int numPolyGonsInBoth;
    float PSNR4IOUCW;
} AdasCompareResult;

int BadasOutputResult(Badas::AdasInfo *adas_detect_results,
                      std::string foldername, int frameNo,
                      std::string input_filename);
int BadasInputResult(Badas::AdasInfo *adas_detect_results, FILE *fpResult);
void BadasCompareResult(Badas::AdasInfo *adas_detect_results0,
                        std::string filenname, std::string cur_frame_name,
                        int frame_no, AdasCompareResult &cresult, bool if_hd);
void ShowAdasCompareResult(AdasCompareResult *cres, std::string input_frame);
