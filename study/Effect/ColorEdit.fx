
float4x4 matWVP : WorldViewProjection;

struct vertexInput
{
    float3 Position : POSITION;
};

struct vertexOutput
{
    float4 HPosition : POSITION;
    float4 Diffuse : COLOR0;
};

vertexOutput VS_TransformDiffuse(vertexInput IN)
{
    vertexOutput OUT;
    OUT.HPosition = mul(float4(IN.Position.xyz, 1.0), matWVP);
    OUT.Diffuse = float4(1.0f, 0.0f, 0.0f, 1.0f);
    return OUT;
}

technique textured
{
    pass p0
    {
        VertexShader = compile vs_1_1 VS_TransformDiffuse();
    }
}