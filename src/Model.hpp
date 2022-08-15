#include "Mesh.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <string>
#include <vector>

struct TextureStruct
{
    std::shared_ptr<Texture> texture;
    std::string type;
    std::string path;
};

class Model
{
  public:
    Model(Device &device, std::string path);
    void Draw();

  private:
    Device &device;

    std::vector<Mesh> meshes;

    void processNode(aiNode *node, const aiScene *scene, glm::mat4 parentTransform);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene, glm::mat4 parentTransform);

    std::vector<TextureStruct> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName,
                                                    const aiScene *scene);

    std::vector<TextureStruct> textures_loaded;

    static inline glm::vec3 vec3_cast(const aiVector3D &v)
    {
        return glm::vec3(v.x, v.y, v.z);
    }
    static inline glm::vec2 vec2_cast(const aiVector3D &v)
    {
        return glm::vec2(v.x, v.y);
    } // it's aiVector3D because assimp's texture coordinates use that
    static inline glm::quat quat_cast(const aiQuaternion &q)
    {
        return glm::quat(q.w, q.x, q.y, q.z);
    }
    static inline glm::mat4 mat4_cast(const aiMatrix4x4 &m)
    {
        return glm::transpose(glm::make_mat4(&m.a1));
    }
};