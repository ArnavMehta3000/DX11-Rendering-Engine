// Material struct
struct Material
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
};


// Light strcts
struct DirectionalLight
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    Material Material;

    float3 Direction;
    float SpecularPower;
};


cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
	
    DirectionalLight dirLight;

	float3 EyePosW; // Eye/camera in world
	float3 LightVecW;
}

//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 NormalW : NORMAL;
	float3 PosW : POSITION; // Position in world
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(float4 Pos : POSITION, float4 Normal : NORMAL)
{
	VS_OUTPUT output = (VS_OUTPUT) 0;
	
	output.Pos = mul(Pos, World);
	output.PosW = output.Pos;
	
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	
	output.NormalW = mul(Normal, World);
	return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT vsInput) : SV_Target
{
	float4 psOutput = (0, 0, 0, 0);
	
	// Compute the vector from the vertex to the eye position (normalize the difference)
	float3 viewToEye = normalize(EyePosW - vsInput.Pos.xyz);
	//float3 viewToEye = (EyePosW.xyz - vsInput.Pos.xyz) / normalize(EyePosW - vsInput.Pos.xyz);
	
	// Convert normal from local space to world space
	float3 normalW = vsInput.NormalW; /* = mul(float4(vsInput.NormalW.xyz, 0.0f), World).xyz;*/
	normalW = normalize(normalW);
	
	// Calculate diffuse and ambient lighting
	float diffuseAmount = max(dot(LightVecW, normalW), 0.0f);
	float3 ambient = (dirLight.Ambient * dirLight.Material.Ambient).rgb;
	
 
	float3 diffuse = diffuseAmount * (dirLight.Material.Diffuse * dirLight.Diffuse).rgb;
	// Compute the reflection vector
	float3 r = reflect(-LightVecW, normalW);
	
	// Determine how much of the specular light makes it into your eye
	float specularAmount = pow(max(dot(r, viewToEye), 0.0f), dirLight.SpecularPower);
	
	// Compute specular amount separately
	float3 specular = specularAmount * (dirLight.Material.Specular * dirLight.Specular).rgb;
	
	// Sum all the terms together and copy over the diffuse alpha
	psOutput.rgb = diffuse + specular + ambient;
	psOutput.a = dirLight.Material.Diffuse.a;
	
	return psOutput;
}