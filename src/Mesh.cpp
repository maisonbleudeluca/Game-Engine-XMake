#include "Mesh.hpp"
#include <assimp/cimport.h>
#include <assimp/material.h>
#include <assimp/material.inl>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.inl>
#include <iostream>

Mesh::Mesh(Device &device, std::vector<Vertex> vertices,
           std::vector<uint32_t> indices, glm::mat4 matrix,
           std::shared_ptr<Texture> MeshTexture, bool hasTex)
    : device{device}, matrix{matrix} {
  numIndices = indices.size();
  stride = sizeof(Vertex);
  offset = 0;

  VertexBuffer = std::make_unique<Buffer>(
      device, D3D11_BIND_VERTEX_BUFFER, D3D11_CPU_ACCESS_WRITE,
      D3D11_USAGE_DYNAMIC, sizeof(Vertex) * vertices.size(), vertices.data());

  IndexBuffer = std::make_unique<Buffer>(
      device, D3D11_BIND_INDEX_BUFFER, (D3D11_CPU_ACCESS_FLAG)0,
      D3D11_USAGE_DEFAULT, sizeof(uint32_t) * indices.size(), indices.data());

  MatrixConstantBuffer = std::make_unique<Buffer>(
      device, D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_FLAG(0),
      D3D11_USAGE_DEFAULT, sizeof(glm::mat4), nullptr);

  this->hasText = hasTex;

  if (hasTex) {
    this->MeshTexture = MeshTexture;
  } else {
    hasTex = true;
  }

  vertices.clear();
  vertices.shrink_to_fit();

  indices.clear();
  indices.shrink_to_fit();
}

void Mesh::Draw() {
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));

  this->matrix = model;

  device.context->UpdateSubresource(MatrixConstantBuffer.get()->buffer, 0, 0,
                                    &this->matrix, 0, 0);
  device.context->VSSetConstantBuffers(0, 1,
                                       &MatrixConstantBuffer.get()->buffer);
  device.context->IASetVertexBuffers(0, 1, &VertexBuffer.get()->buffer, &stride,
                                     &offset);
  device.context->IASetIndexBuffer(IndexBuffer.get()->buffer,
                                   DXGI_FORMAT_R32_UINT, 0);

  device.context->PSSetShaderResources(0, 1, &MeshTexture.get()->resourceView);
  device.context->PSSetSamplers(0, 1, &MeshTexture.get()->sampleState);

  device.context->DrawIndexed(numIndices, 0, 0);
}