#include "MeshLoader.h"
#include "MACRO.h"

MeshLoader::MeshLoader()
{
}

MeshLoader::~MeshLoader()
{
}

bool MeshLoader::Load(const wchar_t* file, vector<MESH>& mesh, vector<MATERIAL>& mtr)
{
    /*if (file == nullptr) return false;
    auto path = wtoc(file);

    Assimp::Importer imp;
    //---------
    auto flag = 0;
    flag |= aiProcess_Triangulate;
    flag |= aiProcess_PreTransformVertices;
    flag |= aiProcess_CalcTangentSpace;
    flag |= aiProcess_GenSmoothNormals;
    flag |= aiProcess_GenUVCoords;
    flag |= aiProcess_RemoveRedundantMaterials;
    flag |= aiProcess_OptimizeMeshes;
    //----------------
    auto scene = imp.ReadFile(path, flag);
    if (scene == nullptr)    return false;

    mesh.clear();
    mesh.reserve(scene->mNumMeshes);
    for (size_t i = 0; i < mesh.size(); i++) {

        const auto pm = scene->mMeshes[i];
        ParseMesh(mesh[i], pm);
    }

    mtr.clear();
    mtr.reserve(scene->mNumMaterials);
    for (size_t i = 0; i < mtr.size(); i++) {

        const auto pm = scene->mMaterials[i];
        ParseMaterial(mtr[i], pm);
    }

    scene = nullptr;*/

    return true;
}

void MeshLoader::ParseMesh(MESH& mesh, const aiMesh* src)
{
    mesh.ID_Material = src->mMaterialIndex;

    aiVector3D vec(0.0f, 0.0f, 0.0f);
    mesh.vtcs_.resize(src->mNumVertices);

    for (auto i = 0u; i < src->mNumVertices; i++) {

        auto pos = &(src->mVertices[i]);
        auto norm = &(src->mNormals[i]);
        auto uv = &(src->mTextureCoords[i]);
        auto tan = &(src->mTangents[i]);
    }
}

void MeshLoader::ParseMaterial(MATERIAL& mtl, const aiMaterial* src)
{
}
