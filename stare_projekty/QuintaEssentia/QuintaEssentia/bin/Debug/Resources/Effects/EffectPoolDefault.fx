shared cbuffer defaultVars
{
	float4x4 viewProjection;
	float3 lightDir;
	float ambient;
};

shared DepthStencilState DepthOnStencilOff
{
        DepthEnable = TRUE;
        DepthWriteMask = ALL;
        DepthFunc = Less;
    
        // Setup stencil states
        StencilEnable = false;
        StencilReadMask = 0xFF;
        StencilWriteMask = 0xFF;
    
        FrontFaceStencilFunc = Not_Equal;
        FrontFaceStencilPass = Keep;
        FrontFaceStencilFail = Zero;
    
        BackFaceStencilFunc = Not_Equal;
        BackFaceStencilPass = Keep;
        BackFaceStencilFail = Zero;
};

shared BlendState NoBlending
{
        BlendEnable[0] = FALSE;
};

shared RasterizerState BackSolid
{

	CullMode=Back;
	DepthClipEnable=true;
	FillMode=Solid;
};