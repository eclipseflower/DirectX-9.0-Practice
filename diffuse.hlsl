matrix ViewMatrix;
matrix ViewProjMatrix;

vector AmbientMtrl;
vector DiffuseMtrl;
vector LightDirection;

vector DiffuseLightIntensity = {0.0f, 0.0f, 1.0f, 1.0f};
vector AmibentLightIntensity = {0.0f, 0.0f, 0.2f, 1.0f};

struct VS_INPUT {
    vector position : POSITION;
    vector normal : NORMAL;
};

struct VS_OUTPUT {
    vector position : POSITION;
    vector diffuse : COLOR;
};

VS_OUTPUT Main(VS_INPUT input) {
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.position = mul(input.position, ViewProjMatrix);
    LightDirection.w = 0.0f;
    input.normal.w = 0.0f;
    LightDirection = mul(LightDirection, ViewMatrix);
    input.normal = mul(input.normal, ViewMatrix);

    float s = dot(LightDirection, input.normal);
    if(s < 0.0f)
        s = 0.0f;
    output.diffuse = AmbientMtrl * AmibentLightIntensity + s * DiffuseMtrl * DiffuseLightIntensity;
    return output;
}