#pragma once
#include "IGenerator.h"
#include <memory>

class PCCuboidGenerator : public IGenerator
{
protected:
    std::shared_ptr<PCCuboidParams> params;

public:
    PCCuboidGenerator(std::shared_ptr<GraphicsObject> graphicsObject);
    ~PCCuboidGenerator() = default;

    inline void SetParameters(std::shared_ptr<IParams> params);
    void Generate();

    void SetUpDynamicBuffers();
};

