#ifndef POINTPILLARSNET_H
#define POINTPILLARSNET_H

//#include <omp.h>
#include <fstream>
#include "voxel.h"
#include "voxel_dsp.h"
class PointPillarsNet
{
private:
    MatrixXf max_pool(const vector<MatrixXf> &input);

    Array3f _offset;
    Array3i _voxel_range;

public:
    PointPillarsNet();
    vector<float> extract(std::shared_ptr<PointsInVoxels>& piv);
    vector<float> extract(std::shared_ptr<PointsInVoxelsDSP>& piv);
};

#endif
