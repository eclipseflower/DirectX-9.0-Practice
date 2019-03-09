matrix WorldMatrix;
matrix ViewMatrix;
matrix ProjMatrix;

texture Tex;

sampler S0 = sampler_state
{
    Texture = (Tex);
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
};

technique LightAndTexture
{
    pass P0
    {
        pixelshader = null;
        vertexshader = null;
        fvf = XYZ | Normal | Tex1;
        Lighting = true;
        NormalizeNormals = true;
        SpecularEnable = false;

        WorldTransform[0] = (WorldMatrix);
        ViewTransform = (ViewMatrix);
        ProjectionTransform = (ProjMatrix);

        LightType[0] = Directional;
        LightAmbient[0] = {0.2f,  0.2f, 0.2f, 1.0f};
        LightDiffuse[0]      = {1.0f,  1.0f, 1.0f, 1.0f};
        LightSpecular[0]     = {0.0f,  0.0f, 0.0f, 1.0f};
        LightDirection[0]    = {1.0f, -1.0f, 1.0f, 0.0f};
        LightPosition[0]     = {0.0f,  0.0f, 0.0f, 0.0f};
        LightFalloff[0]      = 0.0f;
        LightRange[0]        = 0.0f;
        LightTheta[0]        = 0.0f;
        LightPhi[0]          = 0.0f;
        LightAttenuation0[0] = 1.0f;
        LightAttenuation1[0] = 0.0f;
        LightAttenuation2[0] = 0.0f;

        LightEnable[0] = true;

        MaterialAmbient  = {1.0f, 1.0f, 1.0f, 1.0f};
        MaterialDiffuse  = {1.0f, 1.0f, 1.0f, 1.0f};
        MaterialEmissive = {0.0f, 0.0f, 0.0f, 0.0f};
        MaterialPower    = 1.0f;
        MaterialSpecular = {1.0f, 1.0f, 1.0f, 1.0f};

        Sampler[0] = (S0);

    }
}
