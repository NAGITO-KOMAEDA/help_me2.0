#include "ObjLoader.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <tuple>
#include <Windows.h>

bool ObjLoader::LoadObj(const std::wstring& filename,
                       std::vector<ObjVertex>& outVertices,
                       std::vector<uint32_t>& outIndices)
{
    // Convert wide string to narrow string for file operations using Windows API
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, filename.c_str(), -1, NULL, 0, NULL, NULL);
    if (size_needed <= 0)
    {
        return false;
    }
    
    std::string narrowFilename(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, filename.c_str(), -1, &narrowFilename[0], size_needed, NULL, NULL);
    narrowFilename.resize(size_needed - 1); // Remove null terminator
    
    std::ifstream file(narrowFilename, std::ios::binary);
    if (!file.is_open())
    {
        return false;
    }

    std::vector<XMFLOAT3> positions;
    std::vector<XMFLOAT3> normals;
    std::vector<XMFLOAT2> texCoords;
    
    std::vector<uint32_t> posIndices, normalIndices, texIndices;

    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#')
            continue;

        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "v")
        {
            XMFLOAT3 pos;
            iss >> pos.x >> pos.y >> pos.z;
            positions.push_back(pos);
        }
        else if (prefix == "vn")
        {
            XMFLOAT3 normal;
            iss >> normal.x >> normal.y >> normal.z;
            normals.push_back(normal);
        }
        else if (prefix == "vt")
        {
            XMFLOAT2 tex;
            iss >> tex.x >> tex.y;
            texCoords.push_back(tex);
        }
        else if (prefix == "f")
        {
            std::string vertex1, vertex2, vertex3;
            iss >> vertex1 >> vertex2 >> vertex3;

            auto parseFace = [&](const std::string& vertexStr) {
                std::stringstream ss(vertexStr);
                std::string item;
                uint32_t posIdx = 0, texIdx = 0, normalIdx = 0;

                // Parse position index
                if (std::getline(ss, item, '/'))
                {
                    if (!item.empty())
                        posIdx = std::stoul(item) - 1; // OBJ uses 1-based indexing
                }

                // Parse texture coordinate index
                if (std::getline(ss, item, '/'))
                {
                    if (!item.empty())
                        texIdx = std::stoul(item) - 1;
                }

                // Parse normal index
                if (std::getline(ss, item, '/'))
                {
                    if (!item.empty())
                        normalIdx = std::stoul(item) - 1;
                }

                return std::make_tuple(posIdx, texIdx, normalIdx);
            };

            auto [p1, t1, n1] = parseFace(vertex1);
            auto [p2, t2, n2] = parseFace(vertex2);
            auto [p3, t3, n3] = parseFace(vertex3);

            // Create vertices
            ObjVertex v1, v2, v3;

            if (p1 < positions.size())
                v1.Position = positions[p1];
            if (t1 < texCoords.size())
                v1.TexCoord = texCoords[t1];
            if (n1 < normals.size())
                v1.Normal = normals[n1];
            else if (p1 < positions.size() && p2 < positions.size() && p3 < positions.size())
            {
                // Calculate normal if not provided
                XMVECTOR edge1 = XMVectorSubtract(XMLoadFloat3(&positions[p2]), XMLoadFloat3(&positions[p1]));
                XMVECTOR edge2 = XMVectorSubtract(XMLoadFloat3(&positions[p3]), XMLoadFloat3(&positions[p1]));
                XMVECTOR normal = XMVector3Normalize(XMVector3Cross(edge1, edge2));
                XMStoreFloat3(&v1.Normal, normal);
            }

            if (p2 < positions.size())
                v2.Position = positions[p2];
            if (t2 < texCoords.size())
                v2.TexCoord = texCoords[t2];
            if (n2 < normals.size())
                v2.Normal = normals[n2];
            else if (p1 < positions.size() && p2 < positions.size() && p3 < positions.size())
            {
                XMVECTOR edge1 = XMVectorSubtract(XMLoadFloat3(&positions[p2]), XMLoadFloat3(&positions[p1]));
                XMVECTOR edge2 = XMVectorSubtract(XMLoadFloat3(&positions[p3]), XMLoadFloat3(&positions[p1]));
                XMVECTOR normal = XMVector3Normalize(XMVector3Cross(edge1, edge2));
                XMStoreFloat3(&v2.Normal, normal);
            }

            if (p3 < positions.size())
                v3.Position = positions[p3];
            if (t3 < texCoords.size())
                v3.TexCoord = texCoords[t3];
            if (n3 < normals.size())
                v3.Normal = normals[n3];
            else if (p1 < positions.size() && p2 < positions.size() && p3 < positions.size())
            {
                XMVECTOR edge1 = XMVectorSubtract(XMLoadFloat3(&positions[p2]), XMLoadFloat3(&positions[p1]));
                XMVECTOR edge2 = XMVectorSubtract(XMLoadFloat3(&positions[p3]), XMLoadFloat3(&positions[p1]));
                XMVECTOR normal = XMVector3Normalize(XMVector3Cross(edge1, edge2));
                XMStoreFloat3(&v3.Normal, normal);
            }

            // Add vertices and indices
            uint32_t baseIndex = (uint32_t)outVertices.size();
            outVertices.push_back(v1);
            outVertices.push_back(v2);
            outVertices.push_back(v3);

            outIndices.push_back(baseIndex);
            outIndices.push_back(baseIndex + 1);
            outIndices.push_back(baseIndex + 2);
        }
    }

    file.close();
    return !outVertices.empty();
}

