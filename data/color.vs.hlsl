// Globals can be set from C++ in hlsl
cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

// -- Typedefs --
// Input type
struct VertexInputType
{
    float4 position : POSITION;
    float4 color : COLOR;
};

// Output type (sent to fragment shader)
struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

// -- Entry point --
PixelInputType ColorVertexShader(VertexInputType input)
{
    PixelInputType output;

    // Change the position vector to be 4 units for proper matrix calculations
    input.position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    // Store the input color fot the pixel shader to use
    output.color = input.color;

    return output;
}