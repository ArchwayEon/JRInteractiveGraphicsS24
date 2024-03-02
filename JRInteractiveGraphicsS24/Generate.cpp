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
