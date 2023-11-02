#pragma once
#include "BUFFER.h"
#include<codecvt>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>

//#pragma comment(lib,"assimp-vc143-mt.lib")


class MeshLoader
{
public:

	MeshLoader();
	~MeshLoader();

	bool Load(const wchar_t* file,vector<MESH>& mesh,vector<MATERIAL>& mtr);

private:

	void ParseMesh(MESH& mesh, const aiMesh* src);
	void ParseMaterial(MATERIAL& mtl, const aiMaterial* src);

private:
};

