#include "Generate.h"

std::shared_ptr<VertexBuffer> Generate::Cuboid(
    float width, float height, float depth, glm::vec3 color, glm::vec2 tex)
{
    float hw = width / 2;
    float hh = height / 2;
    float hd = depth / 2;
    std::shared_ptr<VertexBuffer> buffer = std::make_shared<VertexBuffer>(8);
    // Front
    buffer->AddVertexData(8,-hw, hh, hd, color.r, color.g, color.b, 0.0f, tex.t ); // A
    buffer->AddVertexData(8,-hw,-hh, hd, color.r, color.g, color.b, 0.0f, 0.0f  );// B 
    buffer->AddVertexData(8, hw,-hh, hd, color.r, color.g, color.b, tex.s, 0.0f ); // C
    buffer->AddVertexData(8,-hw, hh, hd, color.r, color.g, color.b, 0.0f, tex.t ); // A
    buffer->AddVertexData(8, hw,-hh, hd, color.r, color.g, color.b, tex.s, 0.0f ); // C
    buffer->AddVertexData(8, hw, hh, hd, color.r, color.g, color.b, tex.s, tex.t);  // D
    // Right                                                       
    buffer->AddVertexData(8, hw, hh, hd, color.r, color.g, color.b, 0.0f, tex.t ); // E
    buffer->AddVertexData(8, hw,-hh, hd, color.r, color.g, color.b, 0.0f, 0.0f  ); // F
    buffer->AddVertexData(8, hw,-hh,-hd, color.r, color.g, color.b, tex.s, 0.0f ); // G
    buffer->AddVertexData(8, hw, hh, hd, color.r, color.g, color.b, 0.0f, tex.t ); // E
    buffer->AddVertexData(8, hw,-hh,-hd, color.r, color.g, color.b, tex.s, 0.0f ); // G
    buffer->AddVertexData(8, hw, hh,-hd, color.r, color.g, color.b, tex.s, tex.t); // H
    // Back                                                       
    buffer->AddVertexData(8, hw, hh,-hd, color.r, color.g, color.b, 0.0f, tex.t ); // I
    buffer->AddVertexData(8, hw,-hh,-hd, color.r, color.g, color.b, 0.0f, 0.0f  ); // J
    buffer->AddVertexData(8,-hw,-hh,-hd, color.r, color.g, color.b, tex.s, 0.0f ); // K
    buffer->AddVertexData(8, hw, hh,-hd, color.r, color.g, color.b, 0.0f, tex.t ); // I
    buffer->AddVertexData(8,-hw,-hh,-hd, color.r, color.g, color.b, tex.s, 0.0f ); // 
    buffer->AddVertexData(8,-hw, hh,-hd, color.r, color.g, color.b, tex.s, tex.t); // L
    // Left                                                       
    buffer->AddVertexData(8,-hw, hh,-hd, color.r, color.g, color.b, 0.0f, tex.t ); // M
    buffer->AddVertexData(8,-hw,-hh,-hd, color.r, color.g, color.b, 0.0f, 0.0f  ); // N
    buffer->AddVertexData(8,-hw,-hh, hd, color.r, color.g, color.b, tex.s, 0.0f ); // O
    buffer->AddVertexData(8,-hw, hh,-hd, color.r, color.g, color.b, 0.0f, tex.t ); // M
    buffer->AddVertexData(8,-hw,-hh, hd, color.r, color.g, color.b, tex.s, 0.0f ); // O
    buffer->AddVertexData(8,-hw, hh, hd, color.r, color.g, color.b, tex.s, tex.t); // P
    // Top                                                       
    buffer->AddVertexData(8,-hw, hh,-hd, color.r, color.g, color.b, 0.0f, tex.t ); // P
    buffer->AddVertexData(8,-hw, hh, hd, color.r, color.g, color.b, 0.0f, 0.0f  ); // Q
    buffer->AddVertexData(8, hw, hh, hd, color.r, color.g, color.b, tex.s, 0.0f ); // R
    buffer->AddVertexData(8,-hw, hh,-hd, color.r, color.g, color.b, 0.0f, tex.t ); // P
    buffer->AddVertexData(8, hw, hh, hd, color.r, color.g, color.b, tex.s, 0.0f ); // R
    buffer->AddVertexData(8, hw, hh,-hd, color.r, color.g, color.b, tex.s, tex.t); // S
    // Bottom                                                       
    buffer->AddVertexData(8, hw,-hh,-hd, color.r, color.g, color.b, 0.0f, tex.t ); // U
    buffer->AddVertexData(8, hw,-hh, hd, color.r, color.g, color.b, 0.0f, 0.0f  ); // V
    buffer->AddVertexData(8,-hw,-hh, hd, color.r, color.g, color.b, tex.s, 0.0f ); // W
    buffer->AddVertexData(8, hw,-hh,-hd, color.r, color.g, color.b, 0.0f, tex.t ); // U
    buffer->AddVertexData(8,-hw,-hh, hd, color.r, color.g, color.b, tex.s, 0.0f ); // W
    buffer->AddVertexData(8,-hw,-hh,-hd, color.r, color.g, color.b, tex.s, tex.t); // X
    return buffer;
}

std::shared_ptr<VertexBuffer> Generate::CuboidWithNormals(
    float width, float height, float depth, glm::vec4 color, glm::vec2 tex)
{
    float hw = width / 2;
    float hh = height / 2;
    float hd = depth / 2;
    std::shared_ptr<VertexBuffer> buffer = std::make_shared<VertexBuffer>(12);
    // Front
    buffer->AddVertexData(12, -hw, hh, hd, color.r, color.g, color.b, color.a, 0.0, 0.0, 1.0, 0.0f, tex.t); // A
    buffer->AddVertexData(12, -hw, -hh, hd, color.r, color.g, color.b,color.a, 0.0, 0.0, 1.0, 0.0f, 0.0f);// B 
    buffer->AddVertexData(12, hw, -hh, hd, color.r, color.g, color.b, color.a, 0.0, 0.0, 1.0, tex.s, 0.0f); // C
    buffer->AddVertexData(12, -hw, hh, hd, color.r, color.g, color.b, color.a, 0.0, 0.0, 1.0, 0.0f, tex.t); // A
    buffer->AddVertexData(12, hw, -hh, hd, color.r, color.g, color.b, color.a, 0.0, 0.0, 1.0, tex.s, 0.0f); // C
    buffer->AddVertexData(12, hw, hh, hd, color.r, color.g, color.b,  color.a, 0.0, 0.0, 1.0, tex.s, tex.t);  // D
    // Right                                                       
    buffer->AddVertexData(12, hw, hh, hd, color.r, color.g, color.b,  color.a, 1.0, 0.0, 0.0, 0.0f, tex.t); // E
    buffer->AddVertexData(12, hw, -hh, hd, color.r, color.g, color.b, color.a, 1.0, 0.0, 0.0, 0.0f, 0.0f); // F
    buffer->AddVertexData(12, hw, -hh, -hd, color.r, color.g, color.b,color.a, 1.0, 0.0, 0.0, tex.s, 0.0f); // G
    buffer->AddVertexData(12, hw, hh, hd, color.r, color.g, color.b,  color.a, 1.0, 0.0, 0.0, 0.0f, tex.t); // E
    buffer->AddVertexData(12, hw, -hh, -hd, color.r, color.g, color.b,color.a, 1.0, 0.0, 0.0, tex.s, 0.0f); // G
    buffer->AddVertexData(12, hw, hh, -hd, color.r, color.g, color.b, color.a, 1.0, 0.0, 0.0, tex.s, tex.t); // H
    // Back                                                       
    buffer->AddVertexData(12, hw, hh, -hd, color.r, color.g, color.b,   color.a, 0.0, 0.0, -1.0, 0.0f, tex.t); // I
    buffer->AddVertexData(12, hw, -hh, -hd, color.r, color.g, color.b,  color.a, 0.0, 0.0, -1.0, 0.0f, 0.0f); // J
    buffer->AddVertexData(12, -hw, -hh, -hd, color.r, color.g, color.b, color.a, 0.0, 0.0, -1.0, tex.s, 0.0f); // K
    buffer->AddVertexData(12, hw, hh, -hd, color.r, color.g, color.b,   color.a, 0.0, 0.0, -1.0, 0.0f, tex.t); // I
    buffer->AddVertexData(12, -hw, -hh, -hd, color.r, color.g, color.b, color.a, 0.0, 0.0, -1.0, tex.s, 0.0f); // 
    buffer->AddVertexData(12, -hw, hh, -hd, color.r, color.g, color.b,  color.a, 0.0, 0.0, -1.0, tex.s, tex.t); // L
    // Left                                                       
    buffer->AddVertexData(12, -hw, hh, -hd, color.r, color.g, color.b,  color.a, -1.0, 0.0, 0.0, 0.0f, tex.t); // M
    buffer->AddVertexData(12, -hw, -hh, -hd, color.r, color.g, color.b, color.a, -1.0, 0.0, 0.0, 0.0f, 0.0f); // N
    buffer->AddVertexData(12, -hw, -hh, hd, color.r, color.g, color.b,  color.a, -1.0, 0.0, 0.0, tex.s, 0.0f); // O
    buffer->AddVertexData(12, -hw, hh, -hd, color.r, color.g, color.b,  color.a, -1.0, 0.0, 0.0, 0.0f, tex.t); // M
    buffer->AddVertexData(12, -hw, -hh, hd, color.r, color.g, color.b,  color.a, -1.0, 0.0, 0.0, tex.s, 0.0f); // O
    buffer->AddVertexData(12, -hw, hh, hd, color.r, color.g, color.b,   color.a, -1.0, 0.0, 0.0, tex.s, tex.t); // P
    // Top                                                       
    buffer->AddVertexData(12, -hw, hh, -hd, color.r, color.g, color.b, color.a, 0.0, 1.0, 0.0, 0.0f, tex.t); // P
    buffer->AddVertexData(12, -hw, hh, hd, color.r, color.g, color.b,  color.a, 0.0, 1.0, 0.0, 0.0f, 0.0f); // Q
    buffer->AddVertexData(12, hw, hh, hd, color.r, color.g, color.b,   color.a, 0.0, 1.0, 0.0, tex.s, 0.0f); // R
    buffer->AddVertexData(12, -hw, hh, -hd, color.r, color.g, color.b, color.a, 0.0, 1.0, 0.0, 0.0f, tex.t); // P
    buffer->AddVertexData(12, hw, hh, hd, color.r, color.g, color.b,   color.a, 0.0, 1.0, 0.0, tex.s, 0.0f); // R
    buffer->AddVertexData(12, hw, hh, -hd, color.r, color.g, color.b,  color.a, 0.0, 1.0, 0.0, tex.s, tex.t); // S
    // Bottom                                                       
    buffer->AddVertexData(12, hw, -hh, -hd, color.r, color.g, color.b, color.a, 0.0, -1.0, 0.0, 0.0f, tex.t); // U
    buffer->AddVertexData(12, hw, -hh, hd, color.r, color.g, color.b,  color.a, 0.0, -1.0, 0.0, 0.0f, 0.0f); // V
    buffer->AddVertexData(12, -hw, -hh, hd, color.r, color.g, color.b, color.a, 0.0, -1.0, 0.0, tex.s, 0.0f); // W
    buffer->AddVertexData(12, hw, -hh, -hd, color.r, color.g, color.b, color.a, 0.0, -1.0, 0.0, 0.0f, tex.t); // U
    buffer->AddVertexData(12, -hw, -hh, hd, color.r, color.g, color.b, color.a, 0.0, -1.0, 0.0, tex.s, 0.0f); // W
    buffer->AddVertexData(12, -hw, -hh, -hd, color.r, color.g, color.b,color.a, 0.0, -1.0, 0.0, tex.s, tex.t); // X
    return buffer;
}

std::shared_ptr<VertexBuffer> Generate::XZPlane(float width, float depth, glm::vec3 color, glm::vec2 tex)
{
    float hw = width / 2;
    float hh = 0.0f;
    float hd = depth / 2;
    std::shared_ptr<VertexBuffer> buffer = std::make_shared<VertexBuffer>(8);
    // Top                                                       
    buffer->AddVertexData(8, -hw, hh, -hd, color.r, color.g, color.b, 0.0f, tex.t); // P
    buffer->AddVertexData(8, -hw, hh, hd, color.r, color.g, color.b, 0.0f, 0.0f); // Q
    buffer->AddVertexData(8, hw, hh, hd, color.r, color.g, color.b, tex.s, 0.0f); // R
    buffer->AddVertexData(8, -hw, hh, -hd, color.r, color.g, color.b, 0.0f, tex.t); // P
    buffer->AddVertexData(8, hw, hh, hd, color.r, color.g, color.b, tex.s, 0.0f); // R
    buffer->AddVertexData(8, hw, hh, -hd, color.r, color.g, color.b, tex.s, tex.t); // S
    return buffer;
}

std::shared_ptr<VertexBuffer> Generate::XYPlane(float width, float height, glm::vec3 color, glm::vec2 tex)
{
    float hw = width / 2;
    float hh = height / 2;
    float hd = 0.0f;
    std::shared_ptr<VertexBuffer> buffer = std::make_shared<VertexBuffer>(8);
    // Top                                                       
    buffer->AddVertexData(8, -hw, hh, hd, color.r, color.g, color.b, 0.0f, tex.t); // P
    buffer->AddVertexData(8, -hw,-hh, hd, color.r, color.g, color.b, 0.0f, 0.0f); // Q
    buffer->AddVertexData(8, hw, -hh, hd, color.r, color.g, color.b, tex.s, 0.0f); // R
    buffer->AddVertexData(8, -hw, hh, hd, color.r, color.g, color.b, 0.0f, tex.t); // P
    buffer->AddVertexData(8, hw, -hh, hd, color.r, color.g, color.b, tex.s, 0.0f); // R
    buffer->AddVertexData(8, hw, hh, hd, color.r, color.g, color.b, tex.s, tex.t); // S
    return buffer;
}

std::shared_ptr<VertexBuffer> Generate::XZPlaneWithNormals(float width, float depth, glm::vec4 color, glm::vec2 tex)
{
    float hw = width / 2;
    float hh = 0.0f;
    float hd = depth / 2;
    std::shared_ptr<VertexBuffer> buffer = std::make_shared<VertexBuffer>(12);
    // Top                                                       
    buffer->AddVertexData(12, -hw, hh, -hd, color.r, color.g, color.b, color.a, 0.0, 1.0, 0.0, 0.0f, tex.t); // P
    buffer->AddVertexData(12, -hw, hh,  hd, color.r, color.g, color.b, color.a, 0.0, 1.0, 0.0, 0.0f, 0.0f); // Q
    buffer->AddVertexData(12,  hw, hh,  hd, color.r, color.g, color.b, color.a, 0.0, 1.0, 0.0, tex.s, 0.0f); // R
    buffer->AddVertexData(12, -hw, hh, -hd, color.r, color.g, color.b, color.a, 0.0, 1.0, 0.0, 0.0f, tex.t); // P
    buffer->AddVertexData(12,  hw, hh,  hd, color.r, color.g, color.b, color.a, 0.0, 1.0, 0.0, tex.s, 0.0f); // R
    buffer->AddVertexData(12,  hw, hh, -hd, color.r, color.g, color.b, color.a, 0.0, 1.0, 0.0, tex.s, tex.t); // S
    return buffer;
}

void Generate::XZLineCircle(
    std::shared_ptr<VertexBuffer>& bufferToFill, 
    float radius, glm::vec3 color, float offset, int steps)
{
    float thetaRadians, x, z;
    for (float theta = 0; theta < 360.0f; theta += steps) {
        thetaRadians = glm::radians(theta);
        x = radius * cosf(thetaRadians);
        z = radius * sinf(thetaRadians);
        bufferToFill->AddVertexData(6, x, offset, z, color.r, color.g, color.b);
    }
}

int Generate::LineCircleIndexes(
    std::shared_ptr<IndexBuffer>& bufferToFill, int numberOfLineSegments, 
    bool isClosed, int startIndex)
{
    unsigned short index, nextIndex = 0, highestIndex = 0;
    unsigned short stopIndex = numberOfLineSegments;
    if (isClosed == false) stopIndex = numberOfLineSegments - 1;
    for (index = 0; index < stopIndex; ++index) {
        nextIndex = index + 1;
        if (isClosed) {
            nextIndex = 
                nextIndex % static_cast<unsigned short>(numberOfLineSegments);
        }
        if (nextIndex > highestIndex) highestIndex = nextIndex;
        bufferToFill->AddIndexData(2, index + startIndex, nextIndex + startIndex);
    }
    return highestIndex + startIndex;
}

void Generate::LineCylinder(std::shared_ptr<VertexBuffer>& bufferToFill, 
    float height, float radius, glm::vec3 color, int steps)
{
    float hh = height / 2;
    Generate::XZLineCircle(bufferToFill, radius, color, hh, steps);
    Generate::XZLineCircle(bufferToFill, radius, color, -hh, steps);
}

void Generate::LineCylinderIndexes(std::shared_ptr<IndexBuffer>& bufferToFill, 
    int numberOfCircleLineSegments)
{
    int highestIndexC1 = Generate::LineCircleIndexes(bufferToFill, 
        numberOfCircleLineSegments, true, 0);
    Generate::LineCircleIndexes(bufferToFill, numberOfCircleLineSegments, true, 
        highestIndexC1 + 1);
    unsigned short index, nextIndex;
    for (index = 0; index < numberOfCircleLineSegments; ++index) {
        nextIndex = index + highestIndexC1 + 1;
        bufferToFill->AddIndexData(2, index, nextIndex);
    }
}

void Generate::LineSphere(
    std::shared_ptr<VertexBuffer>& bufferToFill, 
    float radius, int slices, int stacks, glm::vec3 color)
{
    float sliceStep = 360.0f / slices;
    float stackStep = 180.0f / stacks;
    float phi;
    float rho;
    float x = 0.0f, y = 0.0f, z = 0.0f;
    float theta, thetaRad, sinTheta, cosTheta, phiRad, sinPhi, cosPhi;

    // North pole
    bufferToFill->AddVertexData(6, x, radius, z, color.r, color.g, color.b);

    // Middle circles
    phi = stackStep;
    rho = radius;
    int middleStackCount = stacks - 1;
    for (int stack = 0; stack < middleStackCount; stack++) {
        for (theta = 0.0f; theta < 360.0f; theta += sliceStep) {
            thetaRad = glm::radians(theta);
            sinTheta = sinf(thetaRad);
            cosTheta = cosf(thetaRad);
            phiRad = glm::radians(phi);
            sinPhi = sinf(phiRad);
            cosPhi = cosf(phiRad);
            x = rho * sinTheta * sinPhi;
            y = rho * cosPhi;
            z = rho * cosTheta * sinPhi;
            bufferToFill->AddVertexData(6, x, y, z, color.r, color.g, color.b);
        }
        phi += stackStep;
    }

    // South pole
    bufferToFill->AddVertexData(6, 0.0f, -radius, 0.0f, color.r, color.g, color.b);

}

void Generate::LineSphereIndexes(
    std::shared_ptr<IndexBuffer>& bufferToFill, int slices, int stacks, int numberOfVertices)
{
    int index, nextIndex, si = 1;
    int middleStackCount = stacks - 1;
    for (int stack = 0; stack < middleStackCount; stack++) {
        for (index = si; index < si + slices; index++) {
            nextIndex = index + 1;
            if (nextIndex >= si + slices) nextIndex = si;
            bufferToFill->AddIndexData(2, index, nextIndex);
        }
        si = index;
    }
    int lastIndex = numberOfVertices - 1;
    for (int slice = 0; slice < slices; slice++) {
        for (int stack = 0; stack < stacks; stack++) {
            if (stack == 0) {
                index = 0; // North pole
                nextIndex = slice + 1;
            }
            else {
                index = si;
                if (stack == stacks - 1) {
                    nextIndex = lastIndex; // South pole
                }
                else {
                    nextIndex = index + slices;
                }
            }
            si = nextIndex;
            bufferToFill->AddIndexData(2, index, nextIndex);
        }
    }
}
