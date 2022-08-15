#include "Model.hpp"
#include <iostream>

Model::Model(Device &device, std::string path) : device{device}
{
    Assimp::Importer importer;
    const aiScene *scene =
        importer.ReadFile(path, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_GenNormals |
                                    aiProcess_FlipUVs | aiProcess_GenUVCoords |

                                    aiProcess_ConvertToLeftHanded);
    // check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    };

    processNode(scene->mRootNode, scene, glm::mat4(1.0f));
}
void Model::processNode(aiNode *node, const aiScene *scene, glm::mat4 parentTransform)
{
    // process each mesh located at the current node
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene, parentTransform * mat4_cast(node->mTransformation)));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene,
                    parentTransform * mat4_cast(node->mChildren[i]->mParent->mTransformation));
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene, glm::mat4 parentTransform)
{
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<TextureStruct> textures;

    textures.reserve(1);

    bool tex = false;

    for (std::uint32_t vertIdx = 0; vertIdx < mesh->mNumVertices; vertIdx++)
    {
        aiVector3D vert = mesh->mVertices[vertIdx];
        aiVector3D norm = mesh->mNormals[vertIdx];

        aiVector3D uv = aiVector3D(0.0f, 0.0f, 0.0f);

        if (mesh->mTextureCoords[0])
        {
            uv = mesh->mTextureCoords[0][vertIdx];
        }

        vertices.push_back(
            Vertex{glm::vec3(vert.x, vert.y, vert.z), glm::vec2(uv.x, uv.y), glm::vec3(norm.x, norm.y, norm.z)

            });

        textures.resize(0);
    }

    indices.reserve(mesh->mNumFaces * 3u);
    for (std::uint32_t faceIdx = 0u; faceIdx < mesh->mNumFaces; faceIdx++)
    {
        indices.push_back(mesh->mFaces[faceIdx].mIndices[0u]);
        indices.push_back(mesh->mFaces[faceIdx].mIndices[1u]);
        indices.push_back(mesh->mFaces[faceIdx].mIndices[2u]);
    }

    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

        // std::cout << material->GetName().C_Str() << " has textures: " <<
        // material->GetTextureCount(aiTextureType_UNKNOWN) << std::endl;
        std::vector<TextureStruct> diffuseMaps =
            loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", scene);
        if (diffuseMaps.size() > 0)
        {
            textures.push_back(diffuseMaps[0]);

            tex = true;
        }
        else
        {

            std::cout << "Couldn't find" << mesh->mName.C_Str() << " with "
                      << scene->mMaterials[mesh->mMaterialIndex]->GetTextureCount(aiTextureType_UNKNOWN) << "materials"
                      << "\n";
            tex = false;
        }
    }
    if (tex)
    {
        return Mesh(device, vertices, indices, parentTransform, textures[0].texture, true);
    }
    return Mesh(device, vertices, indices, parentTransform, nullptr, false);
}

std::vector<TextureStruct> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName,
                                                       const aiScene *scene)
{
    std::vector<TextureStruct> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);

        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if (!skip)
        { // if texture hasn't been loaded already, load it
            TextureStruct texture;
            texture.texture = std::make_shared<Texture>(device, str.C_Str());
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture); // add to loaded textures
        }
    }
    return textures;
}

void Model::Draw()
{
    for (int i = 0; i < meshes.size(); i++)
    {
        meshes[i].Draw();
    }
}