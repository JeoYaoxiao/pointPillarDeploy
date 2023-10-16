#include "pointpillarsnet.h"
#include <chrono>
// void matrix_to_file(const vector<MatrixXf> &input, const string &path);

PointPillarsNet::PointPillarsNet()
{
    for (int i = 0; i < 3; ++i)
    {
        _offset[i] = pc_range_low[i] + voxel_size[i] / 2.0;
        _voxel_range[i] = floor((pc_range_high[i] - pc_range_low[i]) / voxel_size[i]);
    }
}

vector<float> PointPillarsNet::extract(std::shared_ptr<PointsInVoxels>& piv)
{
    // vector<MatrixXf> voxels(std::move(piv.voxels));
    int num_non_empty_voxels = piv->voxels.size();
    MatrixXf points_mean(num_non_empty_voxels, 4);

    #pragma omp parallel for
    for (int i = 0; i < num_non_empty_voxels; ++i)
    {
        points_mean.row(i) = piv->voxels[i].colwise().sum() / piv->num_points_per_voxel[i];
        for (int j = 0; j < piv->num_points_per_voxel[i]; ++j)
        {
            piv->voxels[i].row(j) -= points_mean.row(i);
        }
    }

    MatrixXf res_max_pool = max_pool(piv->voxels);
    int num_voxels = _voxel_range[1] * _voxel_range[0];
    vector<float> image1(num_out_features * num_voxels);
    int num_out_features_nn = res_max_pool.cols();
    int shift_4 = num_out_features_nn + 4;

    #pragma omp parallel for
    for (int i = 0; i < num_non_empty_voxels; ++i)
    {
        int cur_coors_idx = piv->coors(i, 1) * _voxel_range[0] + piv->coors(i, 0);
        for (int j = 0; j < num_out_features; ++j)
        {
            int cur_image1_idx = j * num_voxels + cur_coors_idx;
            if (j - num_out_features_nn < 0)
            {
                image1[cur_image1_idx] = res_max_pool(i, j);
                continue;
            }

            int k = j - shift_4;
            
            if (k < 0)
            {
                image1[cur_image1_idx] = points_mean(i, j - num_out_features_nn);
                continue;
            }

            image1[cur_image1_idx] = piv->coors(i, k) * voxel_size[k] + _offset[k];
        }
    }

    return image1;
}


vector<float> PointPillarsNet::extract(std::shared_ptr<PointsInVoxelsDSP>& piv)
{
    // vector<MatrixXf> voxels(std::move(piv.voxels));
    int num_non_empty_voxels = piv->voxels.size();
    MatrixXf points_mean(num_non_empty_voxels, 4);

    #pragma omp parallel for
    for (int i = 0; i < num_non_empty_voxels; ++i)
    {
        points_mean.row(i) = piv->voxels[i].colwise().sum() / piv->num_points_per_voxel[i];
        for (int j = 0; j < piv->num_points_per_voxel[i]; ++j)
        {
            piv->voxels[i].row(j) -= points_mean.row(i);
        }
    }

    MatrixXf res_max_pool = max_pool(piv->voxels);
    int num_voxels = _voxel_range[1] * _voxel_range[0];
    vector<float> image1(num_out_features * num_voxels);
    int num_out_features_nn = res_max_pool.cols();
    int shift_4 = num_out_features_nn + 4;

    #pragma omp parallel for
    for (int i = 0; i < num_non_empty_voxels; ++i)
    {
        int cur_coors_idx = piv->coors(i, 1) * _voxel_range[0] + piv->coors(i, 0);
        for (int j = 0; j < num_out_features; ++j)
        {
            int cur_image1_idx = j * num_voxels + cur_coors_idx;
            if (j - num_out_features_nn < 0)
            {
                image1[cur_image1_idx] = res_max_pool(i, j);
                continue;
            }

            int k = j - shift_4;

            if (k < 0)
            {
                image1[cur_image1_idx] = points_mean(i, j - num_out_features_nn);
                continue;
            }

            image1[cur_image1_idx] = piv->coors(i, k) * voxel_size[k] + _offset[k];
        }
    }

    return image1;
}

// torch.max(x, dim=1, keepdim=True)[0]
// 在vector<MatrixX4f>的每个MatrixX4f的列维度上取最大
// 返回 (num_non_empty_voxels, num_out_features)
MatrixXf PointPillarsNet::max_pool(const vector<MatrixXf> &input)
{
    int num_non_empty_voxels = input.size();
    MatrixXf res_max_pooled = MatrixXf::Zero(num_non_empty_voxels, num_out_features - 6);
    #pragma omp parallel for
    for (int i = 0; i < num_non_empty_voxels; ++i)
    {
        res_max_pooled.block<1, num_out_features - 6>(i, 0) = input[i].colwise().maxCoeff();
    }
    return res_max_pooled;
}
