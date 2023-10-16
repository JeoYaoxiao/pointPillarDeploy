#ifndef PRE_PROCESS_H
#define PRE_PROCESS_H

#include "base.h"
#include "voxel_dsp.h"
#include "pointpillarsnet.h"

#define MAYBE_UNUSED(...) __VA_ARGS__
#define TIME_STAMP(cyc_cnt)                                                    \
  { cyc_cnt = XT_RSR_CCOUNT(); }
MatrixXf read_matrix_dsp(const string &path, int rows, int cols, bool is_trans = false);

vector<float> pre_process_dsp(const string &points_path);

#endif
