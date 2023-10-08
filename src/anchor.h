#ifndef ANCHOR_H
#define ANCHOR_H

#include <string>

#include "base.h"

class AnchorGenerator
{
public:
    AnchorGenerator(const std::string &class_name,
                    const Array3f &stride,
                    const Array3f &size,
                    const Array3f &offset);

    MatrixXf get_anchors();

private:
    string _class_name;
    MatrixXf _anchors;
};

MatrixXf create_anchor();

#endif