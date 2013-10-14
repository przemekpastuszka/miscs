#include "EffectPoolDefault.fx"

cbuffer eachFrame
{
	float3 translation;
	float3 sunPos;
};

cbuffer loadedOnce
{
	float3 mieTemps=float3(0.010016433,1.9801,-0.99);
	float2 K=float2(0.0025,0.001);
	float sunIntensity=30;
	float outerRadius=6456.75244;
	float innerRadius=6356.75244;
	float atmScale=0.01;
	float3 waveLengthRay=float3(0.178506225,0.105560005,0.05090664);
	//float3 waveLengthMie=float3(1.43599451,1.60348582,1.86886263);
	float3 waveLengthMie=float3(1,1,1);
	float3 cameraPoint=float3(0,6356.75244,0);
	float2 scaleDepth=float2(0.25,0.1);
	float PI=3.14159265;
	int nIn=50;
	int nOut=20;
	float2 invertedSkySize=float2(0.015625,0.0078125);
};

Texture2D mieLookupTex;
Texture2D rayLookupTex;

//functions

float GetScaledHeight(float3 pos)
{
	return (length(pos)-innerRadius)*atmScale;
}

float2 OpticalDepth(float3 a, float3 b)
{
	float3 addRay=b-a;
	float sectorLen=length(addRay)/nOut;
	addRay=normalize(addRay);
	addRay*=sectorLen;
	float3 start=a+addRay*0.5;
	float2 integral=float2(0,0);
	for(int i=0;i<nOut;i++)
	{
		integral+=exp(-GetScaledHeight(start)/scaleDepth);
		start+=addRay;
	}
	integral*=sectorLen*atmScale;
	return integral;
}

float3 HitOuterSphere(float3 o, float3 dir)
{
	dir=normalize(dir);
	float B=dot(o,dir);
	float C=dot(o,o)-outerRadius*outerRadius;
	float D=sqrt(B*B-C);
	return o+dir*(D-B);
}



//structs
struct VSIN
{
	float3 pos : POSITION;
	float2 texCoord : TEXCOORD;
};
struct PSIN
{
	float4 pos: SV_POSITION;
	float3 c1 : COLOR0;
	float3 c2 : COLOR1;
	float3 realPos: TEXCOORD;
};

struct PSOutputUpdate
{
	float4 rayleigh : SV_TARGET0;
	float4 mie : SV_TARGET1;
};


//shaders
float4 VSUpdate(float3 pos: POSITION) : SV_POSITION
{
	return float4(pos,1);
}

PSOutputUpdate PSUpdate(float4 psin : SV_POSITION)
{
	PSOutputUpdate psout=(PSOutputUpdate)0;
	float2 offset=psin.xy*invertedSkySize;
	offset.x*=PI/2;
	offset.y*=2*PI;
	float3 dir=float3(sin(offset.x)*cos(offset.y),cos(offset.x),sin(offset.x)*sin(offset.y));
	float3 target=HitOuterSphere(cameraPoint,dir);
	float sectorLen=length(target-cameraPoint)/nIn;
	dir*=sectorLen;
	float3 start=cameraPoint+dir*0.5;
	for(int i=0;i<nIn;i++)
	{
		float3 tempSunPos=HitOuterSphere(start,sunPos);
		float2 opticalDepth=exp(-GetScaledHeight(start)/scaleDepth);
		float2 temp=-4*PI*K*(OpticalDepth(start,tempSunPos)+OpticalDepth(cameraPoint,start));
		psout.rayleigh.rgb+=exp(temp.x/waveLengthRay)*opticalDepth.x;
		psout.mie.rgb+=exp(temp.y/waveLengthMie)*opticalDepth.y;
		start+=dir;
		
	}
	psout.rayleigh.rgb*=sunIntensity*sectorLen*atmScale*K.x/waveLengthRay;
	psout.mie.rgb*=sunIntensity*sectorLen*atmScale*K.y/waveLengthMie;
	psout.mie.a=psout.rayleigh.a=1;
	return psout;
}


PSIN VS(VSIN vsin)
{
	PSIN vout=(PSIN)0;
	vout.realPos=vsin.pos;
	vout.c1=rayLookupTex.Load(int3(vsin.texCoord,0)).rgb;
	vout.c2=mieLookupTex.Load(int3(vsin.texCoord,0)).rgb;
	vsin.pos+=translation;
	vout.pos=mul(float4(vsin.pos,1),viewProjection);
	return vout;
}

float4 PS(PSIN psin) : SV_Target
{
	float cos=dot(psin.realPos,sunPos)/length(psin.realPos);
	float cos2=cos*cos;
	float rayleighPhase=0.75+0.75*cos2;
	float miePhase=mieTemps.x*(1+cos2)/sqrt(pow(mieTemps.y-2*mieTemps.z*cos,3));
	return float4(psin.c1*rayleighPhase+miePhase*psin.c2,1);
}

DepthStencilState DepthOffStencilOff
{
	DepthEnable=false;
	StencilEnable=false;
};
RasterizerState NoneOffSolid
{
	CullMode=none;
	DepthClipEnable=false;
	FillMode=solid;
};
RasterizerState NoneOnSolid
{
	CullMode=none;
	DepthClipEnable=true;
	FillMode=solid;
};

technique10 Render
{
	pass P0
	{
		SetGeometryShader( 0 );
		SetVertexShader( CompileShader( vs_4_0, VS() ) );
		SetPixelShader( CompileShader( ps_4_0, PS() ) );
		SetDepthStencilState(DepthOnStencilOff,0);
		SetBlendState( NoBlending, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
		SetRasterizerState(NoneOnSolid);
	}
}

technique10 Update
{

	pass P0
	{
		SetGeometryShader( 0 );
		SetVertexShader( CompileShader( vs_4_0, VSUpdate() ) );
		SetPixelShader( CompileShader( ps_4_0, PSUpdate() ) );
		SetDepthStencilState(DepthOffStencilOff,0);
		SetBlendState( NoBlending, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
		SetRasterizerState(NoneOffSolid);
	}
}