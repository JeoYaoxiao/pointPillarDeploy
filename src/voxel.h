#ifndef VOXEL_H
#define VOXEL_H
#include <cstddef>
#include <vector>
#include <memory>
#include "base.h"

struct PointsInVoxels
{
    vector<MatrixXf> voxels;
    MatrixXi coors;
    vector<int> num_points_per_voxel;

    PointsInVoxels() {};

    PointsInVoxels(vector<MatrixXf> &&voxelsi,
                   MatrixXi &&coorsi,
                   vector<int> &&num_points_per_voxeli)
        : voxels(std::move(voxelsi)), coors(std::move(coorsi)),
          num_points_per_voxel(std::move(num_points_per_voxeli)) {}

    PointsInVoxels(PointsInVoxels &&other) noexcept;
    PointsInVoxels &operator=(PointsInVoxels &&other) noexcept;
    void Init();
    void Reset();
};

class Voxel
{
private:
    Array3i _voxel_range;
    int* idxs;
    bool* valids;
    MatrixXf* cloudIdxs;
    Array3i* array3is;

public:
    Voxel();
    ~Voxel();
    void points_to_voxels(const MatrixXf &point_cloud, std::shared_ptr<PointsInVoxels>& piv);
};

#endif