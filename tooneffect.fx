extern matrix WorldViewMatrix;
extern matrix WorldViewProjMatrix;

extern vector Color;
extern vector LightDirection;
extern texture ShadeTex;

struct VS_INPUT
{
    vector position : POSITION;
    vector normal : NORMAL;
};

struct VS_OUTPUT
{
    vector position : POSITION;
    float2 uvCoords : TEXCOORD0;
    vector diffuse : COLOR;
};

VS_OUTPUT Main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.position = mul(input.position, WorldViewProjMatrix);
    LightDirection.w = 0.0f;
    input.normal.w = 0.0f;
    LightDirection = mul(LightDirection, WorldViewMatrix);
    input.normal = mul(input.normal, WorldViewMatrix);

    float u = dot(LightDirection, input.normal);
    if (u < 0.0f)
        u = 0.0f;

    float v = 0.5f;
    output.uvCoords.x = u;
    output.uvCoords.y = v;
	output.diffuse = Color;

    return output;
}

sampler ShaderSampler = sampler_state
{
    Texture = (ShadeTex);
    MinFilter = POINT;
    MagFilter = POINT;
    MipFilter = NONE;
};

technique Toon
{
    pass P0
    {
        vertexShader = compile vs_1_1 Main();
        Sampler[0] = (ShaderSampler);
    }
}