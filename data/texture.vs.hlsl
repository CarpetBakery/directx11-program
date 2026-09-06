// -- Globals --
cbuffer MatrixBuffer
{
    matrix mat_world;
    matrix mat_view;
    matrix mat_proj;
};

// -- Typedefs --
struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

PixelInputType TextureVertexShader(VertexInputType input)
{
    PixelInputType output;

    // For proper matrix math
    input.position.w = 1.0f;

    output.position = mul(input.position, mat_world);
    output.position = mul(input.position, mat_view);
    output.position = mul(input.position, mat_proj);

    // Pass texture coords to fragment shader
    output.tex = input.tex;

    return output;
}