#ifndef VOXEL_DSP_H
#define VOXEL_DSP_H
#include <cstddef>
#include <vector>
#include <memory>
#include "base.h"

struct PointsInVoxelsDSP
{
    vector<MatrixXf> voxels;
    MatrixXi coors;
    vector<int> num_points_per_voxel;

    PointsInVoxelsDSP() {};

    PointsInVoxelsDSP(vector<MatrixXf> &&voxelsi,
                   MatrixXi &&coorsi,
                   vector<int> &&num_points_per_voxeli)
        : voxels(std::move(voxelsi)), coors(std::move(coorsi)),
          num_points_per_voxel(std::move(num_points_per_voxeli)) {}

    PointsInVoxelsDSP(PointsInVoxelsDSP &&other) noexcept;
    PointsInVoxelsDSP &operator=(PointsInVoxelsDSP &&other) noexcept;
    void Init();
    void Reset();
};

class VoxelDSP
{
private:
    Array3i _voxel_range;
    int* idxs;
    bool* valids;
    MatrixXf* cloudIdxs;
    Array3i* array3is;

public:
    VoxelDSP();
    ~VoxelDSP();
    void points_to_voxels(const MatrixXf &point_cloud, std::shared_ptr<PointsInVoxelsDSP>& piv);
};

#endif //VOXEL_DSP_H
