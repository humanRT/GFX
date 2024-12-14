#ifndef MESH_HPP
#define MESH_HPP

#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // For transformations like perspective, lookAt, etc.

#include <assimp/Importer.hpp>  // C++ importer interface
#include <assimp/scene.h>       // Output data structure
#include <assimp/postprocess.h> // Post processing flags

#include "camera.hpp"
#include "math3d.hpp"
#include "material.hpp"
#include "utils.hpp"

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))
#define ASSIMP_LOAD_FLAGS (aiProcess_JoinIdenticalVertices |    \
                           aiProcess_Triangulate |              \
                           aiProcess_GenSmoothNormals |         \
                           aiProcess_LimitBoneWeights |         \
                           aiProcess_SplitLargeMeshes |         \
                           aiProcess_ImproveCacheLocality |     \
                           aiProcess_RemoveRedundantMaterials | \
                           aiProcess_FindDegenerates |          \
                           aiProcess_FindInvalidData |          \
                           aiProcess_GenUVCoords |              \
                           aiProcess_CalcTangentSpace)

#define GL_CHECK_ERROR() (glGetError() == GL_NO_ERROR)
#define INVALID_MATERIAL 0xFFFFFFFF


class Mesh
{
public:
   
    Mesh();
    ~Mesh();
    
    void drawNormals(float normalLength);
    void drawTriangles(GLuint wireframeProgram, const glm::mat4& mvp);
    bool loadMesh(const std::string& filename);
    void render();

protected:
    enum BUFFER_TYPE {
        INDEX_BUFFER = 0,
        VERTEX_BUFFER = 1,
        WVP_MAT_BUFFER = 2,  // required only for instancing
        WORLD_MAT_BUFFER = 3,  // required only for instancing
        NUM_BUFFERS = 4
    };

    struct Triangle {
        glm::vec3 v0, v1, v2;
    };

    struct MeshEntry {
        MeshEntry()
        {
            NumIndices = 0;
            BaseVertex = 0;
            BaseIndex = 0;
            MaterialIndex = INVALID_MATERIAL;
        }

        uint NumIndices;
        uint BaseVertex;
        uint BaseIndex;
        uint MaterialIndex;
    };

    Camera *m_camera;
    const aiScene* m_pScene;
    Assimp::Importer m_importer;
    std::vector<MeshEntry> m_meshes;
    Matrix4f m_globalInverseTransform;

    GLuint m_VAO = 0;
    GLuint m_buffers[NUM_BUFFERS] = { 0 };

    void clear();
    void extractTrianglesFromScene();
    virtual void reserveSpace(uint NumVertices, uint NumIndices);
    virtual void initSingleMesh(uint MeshIndex, const aiMesh* paiMesh);
    virtual void populateBuffers();

private:
    struct Vertex {
        Vector3f position;
        Vector2f texCoords;
        Vector3f normal;
    };

    std::vector<uint> m_indices;
    std::vector<Vertex> m_vertices;
    std::vector<Material> m_materials;
    std::vector<Triangle> m_triangles;

    void initAllMeshes(const aiScene* pScene);
    bool initFromScene(const aiScene* pScene, const std::string& filename);
    bool initMaterials(const aiScene* pScene, const std::string& filename);
    void countVerticesAndIndices(const aiScene* pScene, uint& numVertices, uint& numIndices);
    void loadColors(const aiMaterial* pMaterial, int index);
    void loadTextures(const std::string& Dir, const aiMaterial* pMaterial, int index);

    void loadDiffuseTexture(const std::string& Dir, const aiMaterial* pMaterial, int index);
    void loadDiffuseTextureEmbedded(const aiTexture* paiTexture, int MaterialIndex);
    void loadDiffuseTextureFromFile(const std::string& dir, const aiString& Path, int MaterialIndex);

    void loadSpecularTexture(const std::string& Dir, const aiMaterial* pMaterial, int index);
    void loadSpecularTextureEmbedded(const aiTexture* paiTexture, int MaterialIndex);
    void loadSpecularTextureFromFile(const std::string& dir, const aiString& Path, int MaterialIndex);

    void loadAlbedoTexture(const std::string& Dir, const aiMaterial* pMaterial, int index);
    void loadAlbedoTextureEmbedded(const aiTexture* paiTexture, int MaterialIndex);
    void loadAlbedoTextureFromFile(const std::string& dir, const aiString& Path, int MaterialIndex);

    void loadMetalnessTexture(const std::string& Dir, const aiMaterial* pMaterial, int index);
    void loadMetalnessTextureEmbedded(const aiTexture* paiTexture, int MaterialIndex);
    void loadMetalnessTextureFromFile(const std::string& dir, const aiString& Path, int MaterialIndex);

    void loadRoughnessTexture(const std::string& Dir, const aiMaterial* pMaterial, int index);
    void loadRoughnessTextureEmbedded(const aiTexture* paiTexture, int MaterialIndex);
    void loadRoughnessTextureFromFile(const std::string& dir, const aiString& Path, int MaterialIndex);
};

#endif // MESH_HPP