#ifndef POST_PROCESS_DSP_H
#define POST_PROCESS_DSP_H

#include "base.h"
#include <chrono>
struct PostRetDSP
{
    MatrixXf boxes;
    MatrixXf scores;
    MatrixXi labels;

    PostRetDSP(MatrixXf &&boxesi, MatrixXf &&scoresi, MatrixXi &&labelsi)
        : boxes(std::move(boxesi)), scores(std::move(scoresi)), labels(std::move(labelsi)) {}
    PostRetDSP(PostRetDSP &&other) noexcept;
};

PostRetDSP
post_process_dsp(const vector<MatrixXf> &batch_boxes,
             const vector<MatrixXf> &batch_cls,
             const vector<MatrixXf> &batch_dir_cls,
             const MatrixXf &anchors);

PostRetDSP post_process_1_dsp(const vector<vector<float>> &boxes,
                       const vector<float> &scores,
                       const vector<int> &cls,
                       const vector<int> &dir_cls,
                       const vector<int> &keep_ids,
                       const MatrixXf &anchors);

#endif
