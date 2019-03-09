extern matrix WorldViewMatrix;
extern matrix WorldViewProjMatrix;
extern vector Color;
extern vector LightDirection;

struct VS_INPUT
{
    vector position : POSITION;
    vector normal : NORMAL;
};

struct VS_OUTPUT
{
    vector position : POSITION;
    float2 uvCoords : TEXCOORD;
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

    float u = dot(input.normal, LightDirection);
    if (u < 0.0f)
        u = 0.0f;
    float v = 0.5f;
    output.uvCoords.x = u;
    output.uvCoords.y = v;
	output.diffuse = Color;

    return output;
}