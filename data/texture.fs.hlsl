// -- Globals --
Texture2D shaderTexture : register(t0); // Texture resource
SamplerState SampleType : register(s0); // Sounds like it has to do with mipmapping(?)

// -- Typedefs --
struct PixelInputType
{
    float4 position : SV_POSITION;;
    float2 tex : TEXCOORD0;
};

float4 TexturePixelShader(PixelInputType input) : SV_TARGET
{
    float4 textureColor;

    // Sample the pixel color from the texture using SamplerType at tex uv coords
    textureColor = shaderTexture.Sample(SampleType, input.tex);

    return textureColor;
}