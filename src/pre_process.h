#ifndef PRE_PROCESS_H
#define PRE_PROCESS_H

#include "base.h"
#include "voxel.h"
#include "pointpillarsnet.h"

#define MAYBE_UNUSED(...) __VA_ARGS__
#define TIME_STAMP(cyc_cnt)                                                    \
  { cyc_cnt = XT_RSR_CCOUNT(); }
MatrixXf read_matrix(const string &path, int rows, int cols, bool is_trans = false);

vector<float> pre_process(const string &points_path);

#endif
