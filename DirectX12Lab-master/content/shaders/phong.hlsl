cbuffer ObjectCB : register(b0)
{
	float4x4 gWorld;
	float4x4 gWorldInvTranspose;
	float4x4 gWorldViewProj;

	float3   gEyePosW;
	float    gSpecPower;

	float3   gLightDirW;   // РЅР°РїСЂР°РІР»РµРЅРёРµ СЃРІРµС‚Р° (РєСѓРґР° СЃРІРµС‚РёС‚)
	float    gAmbientK;

	float3   gLightColor;
	float    _pad0;
};

struct VSInput
{
	float3 PosL    : POSITION;
	float3 NormalL : NORMAL;
	float4 Color   : COLOR;
};

struct PSInput
{
	float4 PosH    : SV_POSITION;
	float3 PosW    : TEXCOORD0;
	float3 NormalW : TEXCOORD1;
	float4 Color   : COLOR;
};

PSInput VS(VSInput vin)
{
	PSInput vout;

	float4 posW = mul(float4(vin.PosL, 1.0f), gWorld);
	vout.PosW = posW.xyz;

	vout.NormalW = mul(vin.NormalL, (float3x3)gWorldInvTranspose);

	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);
	vout.Color = vin.Color;

	return vout;
}

float4 PS(PSInput pin) : SV_TARGET
{
	float3 N = normalize(pin.NormalW);
	float3 L = normalize(-gLightDirW);
	float3 V = normalize(gEyePosW - pin.PosW);
	float3 R = reflect(-L, N);

	float NdotL = max(dot(N, L), 0.0f);

	float3 ambient  = gAmbientK * pin.Color.rgb;
	float3 diffuse  = NdotL * pin.Color.rgb * gLightColor;

	float spec = 0.0f;
	if (NdotL > 0.0f)
		spec = pow(max(dot(R, V), 0.0f), gSpecPower);

	float3 specular = spec * gLightColor;

	return float4(ambient + diffuse + specular, pin.Color.a);
}
