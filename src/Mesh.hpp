#pragma once
#include "Buffer.hpp"
#include "Texture.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>

struct Vertex {
  glm::vec3 position;
  glm::vec2 uv;
  glm::vec3 normal;
};

class Mesh {
public:
  Mesh(Device &device, std::vector<Vertex> vertices,
       std::vector<uint32_t> indices, glm::mat4 matrix,
       std::shared_ptr<Texture> MeshTexture, bool hasTex);

  void Draw();

private:
  std::unique_ptr<Buffer> VertexBuffer;
  std::unique_ptr<Buffer> IndexBuffer;
  std::unique_ptr<Buffer> MatrixConstantBuffer;
  std::shared_ptr<Texture> MeshTexture;

  std::vector<Vertex> vertices;
  std::vector<uint32_t> indices;

  UINT stride = -1;
  UINT offset = -1;
  float numIndices = -1;

  bool hasText = false;

  glm::mat4 matrix;

  const Device &device;
};
