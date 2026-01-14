#pragma once

#include "../math/MathUtils.h"
#include "../graphics/GpuUploadBuffer.h"
#include "AppBase.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace DirectX::PackedVector;

struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
	XMFLOAT4 Color; // albedo
};

struct ObjectConstants
{
	XMFLOAT4X4 World             = MathUtils::Identity4x4();
	XMFLOAT4X4 WorldInvTranspose = MathUtils::Identity4x4();
	XMFLOAT4X4 WorldViewProj     = MathUtils::Identity4x4();

	XMFLOAT3   EyePosW   = {0.0f, 0.0f, 0.0f};
	float      SpecPower = 32.0f;

	XMFLOAT3   LightDirW = {0.577f, -0.577f, 0.577f}; // диагональный directional light
	float      AmbientK  = 0.15f;

	XMFLOAT3   LightColor = {1.0f, 1.0f, 1.0f};
	float      _pad0      = 0.0f;
};

class CubeApp : public AppBase
{
public:
	CubeApp(HINSTANCE hInstance);
    CubeApp(const CubeApp& rhs) = delete;
    CubeApp& operator=(const CubeApp& rhs) = delete;
	~CubeApp();

	virtual bool Initialize()override;

private:
    virtual void OnResize()override;
    virtual void Update(const FrameTimer& gt)override;
    virtual void Draw(const FrameTimer& gt)override;

    virtual void OnMouseDown(WPARAM btnState, int x, int y)override;
    virtual void OnMouseUp(WPARAM btnState, int x, int y)override;
    virtual void OnMouseMove(WPARAM btnState, int x, int y)override;

    void BuildDescriptorHeaps();
	void BuildConstantBuffers();
    void BuildRootSignature();
    void BuildShadersAndInputLayout();
    void BuildBoxGeometry();
    void BuildPSO();

private:
    
    ComPtr<ID3D12RootSignature> mRootSignature = nullptr;
    ComPtr<ID3D12DescriptorHeap> mCbvHeap = nullptr;

    std::unique_ptr<GpuUploadBuffer<ObjectConstants>> mObjectCB = nullptr;

	std::unique_ptr<MeshGeometry> mBoxGeo = nullptr;

    ComPtr<ID3DBlob> mvsByteCode = nullptr;
    ComPtr<ID3DBlob> mpsByteCode = nullptr;

    std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;

    ComPtr<ID3D12PipelineState> mPSO = nullptr;

    XMFLOAT4X4 mWorld = MathUtils::Identity4x4();
    XMFLOAT4X4 mView = MathUtils::Identity4x4();
    XMFLOAT4X4 mProj = MathUtils::Identity4x4();

    float mTheta = 1.5f*XM_PI;
    float mPhi = XM_PIDIV4;
    float mRadius = 12.0f;

    POINT mLastMousePos;
};