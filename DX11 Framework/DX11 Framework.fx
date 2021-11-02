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
	float4 AmbientMtrl;
	float4 AmbientLight;
	float4 SpecularMtrl;
	float4 SpecularLight;
	float  SpecularPower;
	float3 EyePosW; // Camera position in world space
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
	
	// Compute the vector from the vertex to the eye position (normalize the difference)
    float3 viewToEye = (EyePosW.xyz - output.Pos.xyz) / normalize(EyePosW - output.Pos.xyz);
	
	// Apply view and projection transformations
    output.Pos = mul(output.Pos, View);
    output.Pos = mul(output.Pos, Projection);
	
	// Convert normal from local space to world space
    float3 normalW = mul(float4(Normal.xyz, 0.0f), World).xyz;
    normalW = normalize(normalW);
	
	// Calculate diffuse and ambient lighting
    float diffuseAmount = max(dot(LightVecW, normalW), 0.0f);
    float ambientAmount = AmbientLight * AmbientMtrl;
	
	// Compute the reflection vector
    float3 r = reflect(-LightVecW, normalW);
	
	// Determine how much of the specular light makes it into your eye
    float specularAmount = pow(max(dot(r, viewToEye), 0.0f), SpecularPower);
	
	// Compute specular amount separately
    float3 specular = specularAmount * (SpecularMtrl * SpecularLight).rgb;
	
	// Sum all the terms together and copy over the diffuse alpha
	output.Color.rgb = (diffuseAmount * (DiffuseMtrl * DiffuseLight).rgb) + ambientAmount + specular;
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
