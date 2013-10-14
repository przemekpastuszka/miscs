#include "EffectPoolDefault.fx"


float2 mousePick;
float4 pickOpt;

cbuffer cInstance
{
	float4 orientations[256];
}
Texture2D normalMap;
//Texture2DArray textures;
//float texCoordMul[12];
cbuffer loadedOnce
{
	float mapSize;
	float heightMul;
};

SamplerState pointSampler
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Clamp;
    AddressV = Clamp;
};

SamplerState linearSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

SamplerState anisotropicSampler
{
    Filter = ANISOTROPIC;
    AddressU = Wrap;
    AddressV = Wrap;
};

struct VS_IN
{
	float2 UV: TEXCOORD0;
	uint orientation : SV_InstanceID;
};

//////////////
//render
/////////////
struct PS_IN
{
	float4 position : SV_POSITION;
	float3 normal: NORMAL;
	float2 UV : TEXCOORD0;
};

PS_IN VS( VS_IN vin )
{
	PS_IN vout = (PS_IN)0;
	

	vout.UV=vin.UV=vin.UV+orientations[vin.orientation].xy;
	vout.position.x=vin.UV.x;
	vout.position.z=vin.UV.y;
	vout.position.w=1;

	vin.UV=vin.UV/mapSize;

	float4 data=normalMap.SampleLevel(pointSampler,vin.UV,0);
	vout.position.y=data.a*255*heightMul;
	

	vout.normal=data.rgb*2.008-1; //approximation of (255*value-127)/127
		
	vout.position=mul(vout.position, viewProjection);
	
	return vout;
}

float4 PS( PS_IN psin) : SV_Target
{
	float4 finalColor=float4(1,1,1,1);
	//finalColor=textures.Sample(linearSampler,float3(psin.UV*texCoordMul[0],0));
	finalColor.rgb=finalColor.rgb*saturate(saturate(dot(-lightDir,psin.normal))+ambient);
	return finalColor;	
}

technique10 Render
{
	pass P0
	{
		SetGeometryShader( 0 );
		SetVertexShader( CompileShader( vs_4_0, VS() ) );
		SetPixelShader( CompileShader( ps_4_0, PS() ) );
		SetDepthStencilState(DepthOnStencilOff,0);
		SetBlendState( NoBlending, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
		SetRasterizerState(BackSolid);
	}
}


///////////////
//editor
/////////////
struct PS_INEditor
{
	float4 position : SV_POSITION;
	float3 normal: NORMAL;
	float2 UV : TEXCOORD0;
	float value : COLOR;
};

PS_INEditor PStoEditor(PS_IN input)
{
	PS_INEditor output=(PS_INEditor)0;
	output.position=input.position;
	output.normal=input.normal;
	output.UV=input.UV;
	return output;
}

PS_INEditor VSEditor( VS_IN vin )
{
	PS_INEditor vout = PStoEditor(VS(vin));	
	vout.value=0;
	if(pickOpt.w!=0)
	{
		float dist=distance(float2(vin.UV.x+orientations[vin.orientation].x,vin.UV.y+orientations[vin.orientation].y),mousePick);
		if(dist<pickOpt.x)
			if(dist>pickOpt.y&&pickOpt.z!=0)
				vout.value=(pickOpt.x-dist)/pickOpt.z;
			else
				vout.value=1;
	}
	return vout;
}

float4 PSEditor( PS_IN pixelShaderIn, float value: COLOR) : SV_Target
{
	float4 final=PS(pixelShaderIn);	
	return lerp(final, float4(1,0,0,1), pickOpt.w*value);
}

technique10 Edit
{
	pass P0
	{
		SetGeometryShader( 0 );
		SetVertexShader( CompileShader( vs_4_0, VSEditor() ) );
		SetPixelShader( CompileShader( ps_4_0, PSEditor() ) );
		SetDepthStencilState(DepthOnStencilOff,0);
		SetBlendState( NoBlending, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
		SetRasterizerState(BackSolid);
	}
}