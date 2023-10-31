#include "MeshLoader.h"
#include "MACRO.h"

static constexpr unsigned int TRIANGLE = 3;

MeshLoader::MeshLoader()
{
}

MeshLoader::~MeshLoader()
{
}

bool MeshLoader::Load(const wchar_t* file, vector<MESH>& mesh, vector<MATERIAL>& mtr)
{
    if (file == nullptr) return false;
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
    mesh.resize(scene->mNumMeshes);
    for (size_t i = 0; i < mesh.size(); i++) {

        const auto pm = scene->mMeshes[i];
        ParseMesh(mesh[i], pm);
    }

    mtr.clear();
    mtr.resize(scene->mNumMaterials);
    for (size_t i = 0; i < mtr.size(); i++) {

        const auto pm = scene->mMaterials[i];
        ParseMaterial(mtr[i], pm);
    }

    scene = nullptr;

    return true;
}

void MeshLoader::ParseMesh(MESH& mesh, const aiMesh* src)
{
    mesh.ID_Material = src->mMaterialIndex;

    aiVector3D vecdef(0.0f, 0.0f, 0.0f);
    
    mesh.vtcs_.resize(src->mNumVertices);

    for (auto i = 0u; i < src->mNumVertices; i++) {

        auto pos = &(src->mVertices[i]);
        auto norm = &(src->mNormals[i]);
        auto uv = (src->HasTextureCoords(0)) ? &(src->mTextureCoords[0][i]) : &vecdef;
        auto tan =(src->HasTangentsAndBitangents())? & (src->mTangents[i]) : &vecdef;

        mesh.vtcs_[i] = VERTEX
        (
            XMFLOAT3(pos->x, pos->y, pos->z),
            XMFLOAT3(norm->x, norm->y, norm->z),
            XMFLOAT2(uv->x, uv->y),
            XMFLOAT3(tan->x, tan->y, tan->z)
        );
    }

    mesh.indexes_.resize(src->mNumFaces * TRIANGLE);

    for (auto i = 0;i < src->mNumFaces;i++) {

        const auto& face = src->mFaces[i];

        mesh.indexes_[i * TRIANGLE + 0] = face.mIndices[0];
        mesh.indexes_[i * TRIANGLE + 1] = face.mIndices[1];
        mesh.indexes_[i * TRIANGLE + 2] = face.mIndices[2];
    }
}

void MeshLoader::ParseMaterial(MATERIAL& mtl, const aiMaterial* src)
{
    {
        aiColor3D coltemp(0.5f, 0.5f, 0.5f);
        auto ResetColor = [&]()
            {
                coltemp.r = 0.5f;
                coltemp.g = 0.5f;
                coltemp.b = 0.5f;
            };

        __CREATE("DIFFUSE")
             if (src->Get(AI_MATKEY_COLOR_DIFFUSE, coltemp) == AI_SUCCESS)
             {
                 mtl.dif_.x = coltemp.r;
                 mtl.dif_.y = coltemp.g;
                 mtl.dif_.z = coltemp.b;
             }
             else
             {
                 mtl.dif_.x = coltemp.r;
                 mtl.dif_.y = coltemp.g;
                 mtl.dif_.z = coltemp.b;
             }

        __CREATE("SPECULAR")
            if (src->Get(AI_MATKEY_COLOR_SPECULAR, coltemp) == AI_SUCCESS)
            {
                mtl.spec_.x = coltemp.r;
                mtl.spec_.y = coltemp.g;
                mtl.spec_.z = coltemp.b;
            }
            else
            {
                ResetColor();
                mtl.spec_.x = coltemp.r;
                mtl.spec_.y = coltemp.g;
                mtl.spec_.z = coltemp.b;
            }

        __CREATE("SPECULAR")
            if (src->Get(AI_MATKEY_COLOR_SPECULAR, coltemp) == AI_SUCCESS)
            {
                mtl.spec_.x = coltemp.r;
                mtl.spec_.y = coltemp.g;
                mtl.spec_.z = coltemp.b;
            }
            else
            {
                ResetColor();
                mtl.spec_.x = coltemp.r;
                mtl.spec_.y = coltemp.g;
                mtl.spec_.z = coltemp.b;
            }

        __CREATE("SHININESS")   auto shine = 0.0f;
            if (src->Get(AI_MATKEY_COLOR_SPECULAR, shine) == AI_SUCCESS)
            {
                mtl.shin_ = shine;
            }
            else
            {
                ResetColor();
                mtl.shin_ = shine;
            }

        __CREATE("DIFFUSEMAP")  aiString path = {};
            if (src->Get(AI_MATKEY_COLOR_DIFFUSE(0), path) == AI_SUCCESS)
            {
                mtl.map_ = string(path.C_Str());
            }
            else
            {
                mtl.map_.clear();
            }
    }
}
//--------------------------------------------
static auto fmt3 = DXGI_FORMAT_R32G32B32_FLOAT;
static auto fmt2 = DXGI_FORMAT_R32G32_FLOAT;
static auto append = D3D12_APPEND_ALIGNED_ELEMENT;
static auto IL_V = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;

const D3D12_INPUT_ELEMENT_DESC VERTEX::element[] =
{
    {SEMANTICS_STR::POSITION,0,fmt3,0,append,IL_V,0},
    {SEMANTICS_STR::NORMAL,0,fmt3,0,append,IL_V,0},
    {SEMANTICS_STR::TEXCOORD,0,fmt2,0,append,IL_V,0},
    {SEMANTICS_STR::TANGENT,0,fmt3,0,append,IL_V,0}
};
const D3D12_INPUT_LAYOUT_DESC VERTEX::inp_Layout =
{
    VERTEX::element,
    VERTEX::AMMOUNT
};

bool LoadMesh(const wchar_t* file, vector<MESH>& mesh, vector<MATERIAL>& material)
{
    MeshLoader ml = {};

    //Flyweight
    return (ml.Load(file, mesh, material));
}