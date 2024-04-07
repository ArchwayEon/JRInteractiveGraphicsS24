#pragma once
#include "IGenerator.h"
#include <memory>

class PCSphereGenerator :
    public IGenerator
{
protected:
    std::shared_ptr<PCSphereParams> params;

public:
    PCSphereGenerator(std::shared_ptr<GraphicsObject> graphicsObject);
    ~PCSphereGenerator() = default;

    inline void SetParameters(std::shared_ptr<IParams> params);
    void Generate();

    void SetUpDynamicBuffers();
};

