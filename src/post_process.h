#ifndef POST_PROCESS_H
#define POST_PROCESS_H

#include "base.h"
#include <chrono>
struct PostRet
{
    MatrixXf boxes;
    MatrixXf scores;
    MatrixXi labels;

    PostRet(MatrixXf &&boxesi, MatrixXf &&scoresi, MatrixXi &&labelsi)
        : boxes(std::move(boxesi)), scores(std::move(scoresi)), labels(std::move(labelsi)) {}
    PostRet(PostRet &&other) noexcept;
};

PostRet
post_process(const vector<MatrixXf> &batch_boxes,
             const vector<MatrixXf> &batch_cls,
             const vector<MatrixXf> &batch_dir_cls,
             const MatrixXf &anchors);

PostRet post_process_1(const vector<vector<float>> &boxes,
                       const vector<float> &scores,
                       const vector<int> &cls,
                       const vector<int> &dir_cls,
                       const vector<int> &keep_ids,
                       const MatrixXf &anchors);

#endif