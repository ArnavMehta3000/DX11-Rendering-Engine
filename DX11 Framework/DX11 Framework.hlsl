// Texturing
Texture2D skybox : TEXTURE : register(t0);
Texture2D diffuseTex : TEXTURE : register(t1);
SamplerState samLinear : SAMPLER : register(s0);

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
    float SpecularPower;
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
	float Time;

	float3 LightVecW;
    float Rate;

    float Amplitude;
    float Frequency;
    float Threshold;
    float padding;
}


struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 NormalW : NORMAL;
	float3 PosW : POSITION; // Position in world
    float2 Tex : TEXCOORD0;
};


// Compute all the point lights
float4 PointLights(float3 worldPos, float3 inputNormal, float3 toEyeNormalized)
{
	float4 output = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float4 ambient = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float4 specular = float4(0.0f, 0.0f, 0.0f, 1.0f);

	// Vector between light position and pixel position
    float3 lightToPixelVec = mul(pointLight.Position, World).xyz - worldPos.xyz;

	// Distance between light pos and pixel pos
	float d = length(lightToPixelVec);

	// Create the ambient light
	ambient = pointLight.Material.Ambient * pointLight.Intensity.Ambient;

	// If pixel is too far then return ambient light
	//if (d > pointLight.Range)
		//return float4(ambient.rgb, diffuse.a);

	lightToPixelVec = normalize(lightToPixelVec);

    float lightAmount = max(dot(lightToPixelVec, inputNormal.xyz),0.0f);
	
    lightAmount = 1.0f;
	
	// Reflection vector
    float3 r = reflect(-LightVecW, inputNormal.xyz);
    float specularAmount = pow(max(dot(r, toEyeNormalized), 0.0f), pointLight.SpecularPower);
    specular = specularAmount * (pointLight.Material.Specular * pointLight.Intensity.Specular);

    if (lightAmount > 0.0f)
    {
        output += lightAmount * pointLight.Material.Diffuse * pointLight.Intensity.Diffuse;

		// Calculate light falloff based on attenuation
        float att = pointLight.Attenuation + (pointLight.Attenuation * d) + (pointLight.Attenuation * d * d);
        output /= float4(att, att, att, 1.0f);
    }


    output += diffuse + specular + ambient;
	return output;
}

// Compute  all the directional lights
float4 DirectionalLights(float3 normalW, float3 toEyeNormalized)
{

    float4 output = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 diffuse = float4(0.0f, 0.0f, 1.0f, 0.0f);
    float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Calculate diffuse and ambient lighting
    float diffuseAmount = max(dot(LightVecW, normalW), 0.0f);
    diffuse = diffuseAmount * (dirLight.Material.Diffuse * dirLight.Intensity.Diffuse);
	
    ambient = (dirLight.Intensity.Ambient * dirLight.Material.Ambient);
 
	// Compute the reflection vector
    float3 r = reflect(-LightVecW, normalW);
	
	// Determine how much of the specular light makes it into your eye
    float specularAmount = pow(max(dot(r, toEyeNormalized), 0.0f), dirLight.SpecularPower);
	
	// Compute specular amount separately
    specular = specularAmount * (dirLight.Material.Specular * dirLight.Intensity.Specular);

    output = diffuse + specular + ambient;
    return output;
}


// Vertex Shader
VS_OUTPUT VS(float4 Pos : POSITION, float3 Normal : NORMAL, float2 Tex : TEXCOORD0)
{
	VS_OUTPUT output = (VS_OUTPUT) 0;
	
    output.Pos = mul(Pos, World);
    output.PosW = output.Pos;
	
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);

	output.NormalW = mul(Normal, World);
	
    output.Tex = Tex;	
	
	return output;
}


// Pixel Shader
float4 PS(VS_OUTPUT vsInput) : SV_Target
{
	float4 psOutput = (0.0f, 0.0f, 0.0f, 1.0f);
	
	// Compute the vector from the vertex to the eye position (normalize the difference)
	float3 viewToEye = EyePosW - vsInput.Pos.xyz;
	float3 toEyeNormalized = normalize(EyePosW - vsInput.Pos.xyz);
	
	// Convert normal from local space to world space
    float3 normalW = normalize(vsInput.NormalW);


	
	// Calculate lighting
    float4 pointLights = PointLights(vsInput.PosW,  (normalW, 0.0f), toEyeNormalized);
    float4 directionalLights = DirectionalLights(normalW, toEyeNormalized);

	// Texturing
    float4 textureColor = diffuseTex.Sample(samLinear, vsInput.Tex);

    //discard;
	
    psOutput.rgb = directionalLights.rgb * textureColor.rgb;
    psOutput.a = dirLight.Material.Diffuse.a;
	
    return psOutput;
}

// Pixel shader 
float4 PS_SKY(VS_OUTPUT vsInput) : SV_TARGET
{
    float4 output = skybox.Sample(samLinear, 1.0f - vsInput.Tex);
    return output ;
}

VS_OUTPUT VS_PLANE(float4 Pos : POSITION, float3 Normal : NORMAL, float2 Tex : TEXCOORD0)
{
    float dx = Pos.x;
    float dz = Pos.z;
    float freq = sqrt(dx * dx + dz * dz);
    float angle = -Time * Rate + freq * Frequency;
    Pos.y += sin(angle) * Amplitude;

	if (Pos.y <= Threshold)
        Pos.y = Threshold;

    VS_OUTPUT output = (VS_OUTPUT) 0;
	
    output.Pos = mul(Pos, World);
    output.PosW = output.Pos;
	
    output.Pos = mul(output.Pos, View);
    output.Pos = mul(output.Pos, Projection);
	
    output.NormalW = mul(Normal, World);
	
    output.Tex = Tex;
	
    return output;
}