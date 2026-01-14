#pragma once

#include "../graphics/Dx12Core.h"
#include <string>
#include <vector>
#include <DirectXMath.h>

using namespace DirectX;

struct ObjVertex
{
    XMFLOAT3 Position;
    XMFLOAT3 Normal;
    XMFLOAT2 TexCoord;
};

class ObjLoader
{
public:
    static bool LoadObj(const std::wstring& filename, 
                       std::vector<ObjVertex>& outVertices,
                       std::vector<uint32_t>& outIndices);
};

