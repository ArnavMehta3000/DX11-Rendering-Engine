// Material struct

struct Intensity
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
};

struct Material
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
};


// Light strcts
struct DirectionalLight
{
	Intensity Intensity;
	Material Material;

	float3 Direction;
	float SpecularPower;
};

struct PointLight
{
	Intensity Intensity;
	Material Material;

	float3 Position;
	float Range;

	float3 Attenuation; 
	float padding;
};


// Constant buffer
cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
	
	DirectionalLight dirLight;
	PointLight pointLight;

	float3 EyePosW; // Eye/camera in world
	float padding;
	float3 LightVecW;
}


struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 NormalW : NORMAL;
	float3 PosW : POSITION; // Position in world
};



float4 ComputePointLights(float3 normal, float3 viewToEye, float3 position)
{
	float4 output = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Vector from surface to light
	float3 lightVec = pointLight.Position - position;

	// Distance from ligth to surface
	float dist = length(lightVec);
	if (dist > pointLight.Range)
		return output;  // OUtput will be zero since surface point is out of light range

	lightVec /= dist;

	// Ambient
	ambient = pointLight.Intensity.Ambient * pointLight.Material.Ambient;

	// Diffuse and specular if surface is in line of sight
	float diffuseFactor = dot(lightVec, normal);
	

	// Flatten to avoid dynamic branching
	[flatten]
	if (diffuseFactor > 0.0f)
	{
		float3 r = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(r, viewToEye), 0.0f), pointLight.Material.Specular);

		diffuse = diffuseFactor * pointLight.Material.Diffuse * pointLight.Intensity.Diffuse;
		specular = specFactor * pointLight.Material.Specular * pointLight.Intensity.Specular;
	}

	// Attenuate
	float att = 1.0f / dot(pointLight.Attenuation, float3(1.0, dist, dist * dist));

	diffuse *= att;
	specular *= att;

	output = diffuse + specular + ambient;

	return output;
}








// Vertex Shader
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


// Pixel Shader
float4 PS(VS_OUTPUT vsInput) : SV_Target
{
	float4 psOutput = (0, 0, 0, 0);
	
	// Compute the vector from the vertex to the eye position (normalize the difference)
	float3 viewToEye = EyePosW - vsInput.Pos.xyz;
	float3 toEyeNormalized = normalize(EyePosW - vsInput.Pos.xyz);
	
	// Convert normal from local space to world space
	float3 normalW = vsInput.NormalW;
	normalW = normalize(normalW);


	float4 pointLights = ComputePointLights(normalW, viewToEye, vsInput.Pos.xyz);

	
	// Calculate diffuse and ambient lighting
	float diffuseAmount = max(dot(LightVecW, normalW), 0.0f);
	float3 ambient = (dirLight.Intensity.Ambient * dirLight.Material.Ambient).rgb;
	
 
	float3 diffuse = diffuseAmount * (dirLight.Material.Diffuse * dirLight.Intensity.Diffuse).rgb;
	// Compute the reflection vector
	float3 r = reflect(-LightVecW, normalW);
	
	// Determine how much of the specular light makes it into your eye
	float specularAmount = pow(max(dot(r, toEyeNormalized), 0.0f), dirLight.SpecularPower);
	
	// Compute specular amount separately
	float3 specular = specularAmount * (dirLight.Material.Specular * dirLight.Intensity.Specular).rgb;
	
	// Sum all the terms together and copy over the diffuse alpha
	psOutput.rgb = diffuse + specular + ambient;
	psOutput.a = dirLight.Material.Diffuse.a;
	
	return psOutput;
}