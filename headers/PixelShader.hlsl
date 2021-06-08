Texture2D tex: register(t0);
SamplerState sam: register(s0);

struct PixelShaderInput
{
	float4 pos: SV_POSITION; // SCREEN COORDINATE( VSH-> RASTERIZER-> PSH )
	float2 randN: SOMEFLOAT2_NAME2; // IT DOENST CARE ABOUT SEMANTIC NAME, ONLY VSH INPUT MATTERS IF YOU WANT YOUR UVs
	uint kchau: AAA;
	
};

float4 main(in PixelShaderInput psInput) : sV_TaRGEt /* SV_TARGET - THE SAME THING AS COLOR, BUT AFTER DX10 THERE IS SV_ SEMANTICS THAT CHANGE
													SOME OF THE OLD VARIANTS */
{
	if (true)
		return tex.Sample(sam, psInput.randN);
	else
		return float4(1,1,1,1);
}