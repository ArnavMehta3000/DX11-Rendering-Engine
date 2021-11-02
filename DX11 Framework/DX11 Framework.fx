//--------------------------------------------------------------------------------------
// File: DX11 Framework.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer ConstantBuffer : register( b0 )
{
	matrix World;
	matrix View;
	matrix Projection;
	float4 DiffuseMtrl;
	float4 DiffuseLight;
	float3 LightVecW;
}

//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR0;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS( float4 Pos : POSITION, float4 Normal : NORMAL )
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	
	output.Pos = mul( Pos, World );
	output.Pos = mul( output.Pos, View );
	output.Pos = mul( output.Pos, Projection );
	
	// Convert from local space to world space (W component is 0)
	float3 normalW = mul(float4(Normal.xyz, 0.0f), World).xyz;
	normalW = normalize(normalW);
	
	// Compute color using diffuse lighting only
	float diffuseAmount = max(dot(LightVecW, normalW), 0.0f);
	
	
	output.Color.rgb = diffuseAmount * (DiffuseMtrl * DiffuseLight).rgb;
	output.Color.a = DiffuseMtrl.a;
	
	return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( VS_OUTPUT input ) : SV_Target
{
	return input.Color;
}
