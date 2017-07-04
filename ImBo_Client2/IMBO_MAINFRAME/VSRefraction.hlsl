


struct VertexInputType
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 tex : TEXCOORD0;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float clip : SV_ClipDistance0;
};

PixelInputType main(VertexInputType input)
{
	PixelInputType output = (PixelInputType)0;
	// Change the position vector to be 4 units for proper matrix calculations.
	//input.position.w = 1.0f;

	//// Calculate the position of the vertex against the world, view, and projection matrices.
	//output.position = mul(input.position, worldMatrix);
	//output.position = mul(output.position, viewMatrix);
	//output.position = mul(output.position, projectionMatrix);

	//// Store the texture coordinates for the pixel shader.
	//output.tex = input.tex;

	//// Calculate the normal vector against the world matrix only.
	//output.normal = mul(input.normal, (float3x3)worldMatrix);

	//// Normalize the normal vector.
	//output.normal = normalize(output.normal);

	//// Set the clipping plane.
	//output.clip = dot(mul(input.position, worldMatrix), clipPlane);

	return output;
}