// ----------------------------------- Per Frame --------------------------------------
cbuffer UpdatePerFrame : register(b0)
{
	float time : Time < string UIWidget = "None"; >;

	float4x4 viewI : ViewInverse < string UIWidget = "None"; >;

	float4x4 view : View < string UIWidget = "None"; >;

	float4x4 viewPrj : ViewProjection < string UIWidget = "None"; >;

};

// --------------------------------------- Per Object -----------------------------------------
cbuffer UpdatePerObject : register(b1)
{
	float SpeedU
	<
		float UIMin = 0.0;
		float UISoftMin = 0.0;
		float UIMax = 99.0;
		float UISoftMax = 99.0;
		float UIStep = 0.01;
		string UIName = "SpeedU";
		string UIWidget = "Slider";
	> = 0.0;

	float SpeedV
	<
		float UIMin = 0.0;
		float UISoftMin = 0.0;
		float UIMax = 99.0;
		float UISoftMax = 99.0;
		float UIStep = 0.01;
		string UIName = "SpeedV";
		string UIWidget = "Slider";
	> = 1.0;

	float4x4 world : World < string UIWidget = "None"; >;

	int ClampDynamicLights
	<
		float UIMin = 0;
		float UISoftMin = 0;
		float UIMax = 99;
		float UISoftMax = 99;
		float UIStep = 1;
		string UIName = "ClampDynamicLights";
		string UIWidget = "Slider";
	> = 99;

	float4x4 wvp : WorldViewProjection < string UIWidget = "None"; >;

};

// --------------------------------------- Attributes -----------------------------------------
cbuffer UpdateAttributes : register(b2)
{
	float Val
	<
		float UIMin = 0.0;
		float UISoftMin = 0.0;
		float UIMax = 99.0;
		float UISoftMax = 99.0;
		float UIStep = 0.01;
		string UIName = "Val";
		string UIWidget = "Slider";
	> = 0.2;

};

// ----------------------------------- Lights --------------------------------------
cbuffer UpdateLights : register(b3)
{
	int Light0Type : LIGHTTYPE
	<
		string UIName =  "Light 0 Type";
		float UIMin = 0;
		float UIMax = 5;
		float UIStep = 1;
		string UIWidget = "None";
		string Object =  "Light 0";
	> = {3};

	int Light1Type : LIGHTTYPE
	<
		string UIName =  "Light 1 Type";
		float UIMin = 0;
		float UIMax = 5;
		float UIStep = 1;
		string UIWidget = "None";
		string Object =  "Light 1";
	> = {3};

	int Light2Type : LIGHTTYPE
	<
		string UIName =  "Light 2 Type";
		float UIMin = 0;
		float UIMax = 5;
		float UIStep = 1;
		string UIWidget = "None";
		string Object =  "Light 2";
	> = {3};

	float3 Light0Color : LIGHTCOLOR
	<
		string UIName =  "Light 0 Color";
		string UIWidget = "ColorPicker";
		string Object =  "Light 0";
	> = { 1.0, 1.0, 1.0};

	float3 Light1Color : LIGHTCOLOR
	<
		string UIName =  "Light 1 Color";
		string UIWidget = "ColorPicker";
		string Object =  "Light 1";
	> = { 1.0, 1.0, 1.0};

	float3 Light2Color : LIGHTCOLOR
	<
		string UIName =  "Light 2 Color";
		string UIWidget = "ColorPicker";
		string Object =  "Light 2";
	> = { 1.0, 1.0, 1.0};

	float Light0Intensity : LIGHTINTENSITY
	<
		string UIName =  "Light 0 Intensity";
		float UIMin = 0;
		float UIStep = 0.1;
		string Object =  "Light 0";
	> = 1.0;

	float Light1Intensity : LIGHTINTENSITY
	<
		string UIName =  "Light 1 Intensity";
		float UIMin = 0;
		float UIStep = 0.1;
		string Object =  "Light 1";
	> = 1.0;

	float Light2Intensity : LIGHTINTENSITY
	<
		string UIName =  "Light 2 Intensity";
		float UIMin = 0;
		float UIStep = 0.1;
		string Object =  "Light 2";
	> = 1.0;

	float3 Light0Pos : POSITION
	<
		string UIName =  "Light 0 Position";
		string Space = "World";
		string Object =  "Light 0";
	> = {1.0, 1.0, 1.0};

	float3 Light1Pos : POSITION
	<
		string UIName =  "Light 1 Position";
		string Space = "World";
		string Object =  "Light 1";
	> = {1.0, 1.0, 1.0};

	float3 Light2Pos : POSITION
	<
		string UIName =  "Light 2 Position";
		string Space = "World";
		string Object =  "Light 2";
	> = {1.0, 1.0, 1.0};

	float3 Light0Dir : DIRECTION
	<
		string UIName =  "Light 0 Direction";
		string Space = "World";
		string Object =  "Light 0";
	> = {0.0, -1.0, 0.0};

	float3 Light1Dir : DIRECTION
	<
		string UIName =  "Light 1 Direction";
		string Space = "World";
		string Object =  "Light 1";
	> = {0.0, -1.0, 0.0};

	float3 Light2Dir : DIRECTION
	<
		string UIName =  "Light 2 Direction";
		string Space = "World";
		string Object =  "Light 2";
	> = {0.0, -1.0, 0.0};

	float Light0Attenuation : DECAYRATE
	<
		string UIName =  "Light 0 Decay";
		float UIMin = 0;
		float UIStep = 1;
		string Object =  "Light 0";
	> = 0.0;

	float Light1Attenuation : DECAYRATE
	<
		string UIName =  "Light 1 Decay";
		float UIMin = 0;
		float UIStep = 1;
		string Object =  "Light 1";
	> = 0.0;

	float Light2Attenuation : DECAYRATE
	<
		string UIName =  "Light 2 Decay";
		float UIMin = 0;
		float UIStep = 1;
		string Object =  "Light 2";
	> = 0.0;

	float Light0ConeAngle : HOTSPOT
	<
		string UIName =  "Light 0 ConeAngle";
		float UIMin = 0;
		float UIMax = 1.571;
		float UIStep = 0.05;
		string Space = "World";
		string Object =  "Light 0";
	> = 0.46;

	float Light1ConeAngle : HOTSPOT
	<
		string UIName =  "Light 1 ConeAngle";
		float UIMin = 0;
		float UIMax = 1.571;
		float UIStep = 0.05;
		string Space = "World";
		string Object =  "Light 1";
	> = 0.46;

	float Light2ConeAngle : HOTSPOT
	<
		string UIName =  "Light 2 ConeAngle";
		float UIMin = 0;
		float UIMax = 1.571;
		float UIStep = 0.05;
		string Space = "World";
		string Object =  "Light 2";
	> = 0.46;

	float Light0Falloff : FALLOFF
	<
		string UIName =  "Light 0 Penumbra Angle";
		float UIMin = 0;
		float UIMax = 1.571;
		float UIStep = 0.05;
		string Object =  "Light 0";
	> = 0.7;

	float Light1Falloff : FALLOFF
	<
		string UIName =  "Light 1 Penumbra Angle";
		float UIMin = 0;
		float UIMax = 1.571;
		float UIStep = 0.05;
		string Object =  "Light 1";
	> = 0.7;

	float Light2Falloff : FALLOFF
	<
		string UIName =  "Light 2 Penumbra Angle";
		float UIMin = 0;
		float UIMax = 1.571;
		float UIStep = 0.05;
		string Object =  "Light 2";
	> = 0.7;

	bool Light0ShadowOn : SHADOWFLAG
	<
		string UIName =  "Light 0 Shadow";
		string Object =  "Light 0";
	> = true;

	bool Light1ShadowOn : SHADOWFLAG
	<
		string UIName =  "Light 1 Shadow";
		string Object =  "Light 1";
	> = true;

	bool Light2ShadowOn : SHADOWFLAG
	<
		string UIName =  "Light 2 Shadow";
		string Object =  "Light 2";
	> = true;

	float4x4 Light0ViewPrj : SHADOWMAPMATRIX
	<
		string Object =  "Light 0";
		string UIName =  "Light 0 Matrix";
		string UIWidget = "None";
	>;

	float4x4 Light1ViewPrj : SHADOWMAPMATRIX
	<
		string Object =  "Light 1";
		string UIName =  "Light 1 Matrix";
		string UIWidget = "None";
	>;

	float4x4 Light2ViewPrj : SHADOWMAPMATRIX
	<
		string Object =  "Light 2";
		string UIName =  "Light 2 Matrix";
		string UIWidget = "None";
	>;

	float3 Light0ShadowColor : SHADOWCOLOR
	<
		string UIName =  "Light 0 Shadow Color";
		string Object =  "Light 0";
	> = {0, 0, 0};

	float3 Light1ShadowColor : SHADOWCOLOR
	<
		string UIName =  "Light 1 Shadow Color";
		string Object =  "Light 1";
	> = {0, 0, 0};

	float3 Light2ShadowColor : SHADOWCOLOR
	<
		string UIName =  "Light 2 Shadow Color";
		string Object =  "Light 2";
	> = {0, 0, 0};

};

// ---------------------------------------- Textures -----------------------------------------
Texture2D Light0ShadowMap : SHADOWMAP
<
	string ResourceName = "";
	string ResourceType = "2D";
	string UIWidget = "None";
	string Object =  "Light 0";
>;

Texture2D Light1ShadowMap : SHADOWMAP
<
	string ResourceName = "";
	string ResourceType = "2D";
	string UIWidget = "None";
	string Object =  "Light 1";
>;

Texture2D Light2ShadowMap : SHADOWMAP
<
	string ResourceName = "";
	string ResourceType = "2D";
	string UIWidget = "None";
	string Object =  "Light 2";
>;

SamplerState Light0ShadowMapSampler : SHADOWMAPSAMPLER
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = BORDER;
	AddressV = BORDER;
	AddressW = BORDER;
	BorderColor = float4(1.0,1.0,1.0,1.0);
};

Texture2D TransDepthTexture : transpdepthtexture
<
	string ResourceName = "";
	string UIName = "TransDepthTexture";
	string ResourceType = "2D";
	string UIWidget = "None";
>;

Texture2D OpaqueDepthTexture : opaquedepthtexture
<
	string ResourceName = "";
	string UIName = "OpaqueDepthTexture";
	string ResourceType = "2D";
	string UIWidget = "None";
>;

SamplerState MMMPBBW_255_255_255_255_Sampler
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = BORDER;
	AddressV = BORDER;
	AddressW = WRAP;
	BorderColor = float4(1.0,1.0,1.0,1.0);
};


// ---------------------------------------- Functions -----------------------------------------
int GetLightType(int ActiveLightIndex) 
{ 
	if (ActiveLightIndex == 0) 
		return Light0Type; 
	else if (ActiveLightIndex == 1) 
		return Light1Type; 
	else 
		return Light2Type; 
}

float3 GetLightColor(int ActiveLightIndex) 
{ 
	if (ActiveLightIndex == 0) 
		return Light0Color; 
	else if (ActiveLightIndex == 1) 
		return Light1Color; 
	else 
		return Light2Color; 
}

float GetLightIntensity(int ActiveLightIndex) 
{ 
	if (ActiveLightIndex == 0) 
		return Light0Intensity; 
	else if (ActiveLightIndex == 1) 
		return Light1Intensity; 
	else 
		return Light2Intensity; 
}

float3 GetLightPos(int ActiveLightIndex) 
{ 
	if (ActiveLightIndex == 0) 
		return Light0Pos; 
	else if (ActiveLightIndex == 1) 
		return Light1Pos; 
	else 
		return Light2Pos; 
}

float3 GetLightDir(int ActiveLightIndex) 
{ 
	if (ActiveLightIndex == 0) 
		return Light0Dir; 
	else if (ActiveLightIndex == 1) 
		return Light1Dir; 
	else 
		return Light2Dir; 
}

float GetLightAttenuation(int ActiveLightIndex) 
{ 
	if (ActiveLightIndex == 0) 
		return Light0Attenuation; 
	else if (ActiveLightIndex == 1) 
		return Light1Attenuation; 
	else 
		return Light2Attenuation; 
}

float GetLightConeAngle(int ActiveLightIndex) 
{ 
	if (ActiveLightIndex == 0) 
		return Light0ConeAngle; 
	else if (ActiveLightIndex == 1) 
		return Light1ConeAngle; 
	else 
		return Light2ConeAngle; 
}

float GetLightFalloff(int ActiveLightIndex) 
{ 
	if (ActiveLightIndex == 0) 
		return Light0Falloff; 
	else if (ActiveLightIndex == 1) 
		return Light1Falloff; 
	else 
		return Light2Falloff; 
}

bool GetLightShadowOn(int ActiveLightIndex) 
{ 
	if (ActiveLightIndex == 0) 
		return Light0ShadowOn; 
	else if (ActiveLightIndex == 1) 
		return Light1ShadowOn; 
	else 
		return Light2ShadowOn; 
}

float4x4 GetLightViewPrj(int ActiveLightIndex) 
{ 
	if (ActiveLightIndex == 0) 
		return Light0ViewPrj; 
	else if (ActiveLightIndex == 1) 
		return Light1ViewPrj; 
	else 
		return Light2ViewPrj; 
}

Texture2D GetLightShadowMap(int ActiveLightIndex) 
{ 
	if (ActiveLightIndex == 0) 
		return Light0ShadowMap; 
	else if (ActiveLightIndex == 1) 
		return Light1ShadowMap; 
	else 
		return Light2ShadowMap; 
}

float4 SampleFromShadowMap( int ActiveLightIndex, float2 UVs) 
{ 
	if (ActiveLightIndex == 0) 
		return Light0ShadowMap.SampleLevel(Light0ShadowMapSampler, UVs, 0); 
	else if (ActiveLightIndex == 1) 
		return Light1ShadowMap.SampleLevel(Light0ShadowMapSampler, UVs, 0); 
	else 
		return Light2ShadowMap.SampleLevel(Light0ShadowMapSampler, UVs, 0); 
}

float3 GetLightShadowColor(int ActiveLightIndex) 
{ 
	if (ActiveLightIndex == 0) 
		return Light0ShadowColor; 
	else if (ActiveLightIndex == 1) 
		return Light1ShadowColor; 
	else 
		return Light2ShadowColor; 
}

#ifndef SFX_HLSL_5
	#define SFX_HLSL_5
#endif 
#ifndef _MAYA_
	#define _MAYA_
#endif 

float4 sampleTransDepthTex(float2 UV)
{
	float4 col = float4(0,0,0,0);

	#if defined(SFX_CGFX_3) || defined(SFX_HLSL_3)
		col = tex2D( MMMPBBW_255_255_255_255_Sampler, UV );
	#endif
	#ifdef SFX_HLSL_5
		#if defined(SFX_SWATCH) || defined(_3DSMAX_)
			col = TransDepthTexture.Sample( MMMPBBW_255_255_255_255_Sampler, UV );
		#else
			col = TransDepthTexture.Sample( MMMPBBW_255_255_255_255_Sampler, UV );
		#endif
	#endif
	#ifdef SFX_GLSL_4
		col = texture( TransDepthTexture, UV );
	#endif
	#ifdef SFX_GLSL_1_2
		col = texture2D( TransDepthTexture, UV );
	#endif

	return col;
}

float4 sampleOpaqueDepthTex(float2 UV)
{
	float4 col = float4(0,0,0,0);

	#if defined(SFX_CGFX_3) || defined(SFX_HLSL_3)
		col = tex2D( MMMPBBW_255_255_255_255_Sampler, UV );
	#endif
	#ifdef SFX_HLSL_5
		#if defined(SFX_SWATCH) || defined(_3DSMAX_)
			col = OpaqueDepthTexture.Sample( MMMPBBW_255_255_255_255_Sampler, UV );
		#else
			col = OpaqueDepthTexture.Sample( MMMPBBW_255_255_255_255_Sampler, UV );
		#endif
	#endif
	#ifdef SFX_GLSL_4
		col = texture( OpaqueDepthTexture, UV );
	#endif
	#ifdef SFX_GLSL_1_2
		col = texture2D( OpaqueDepthTexture, UV );
	#endif

	return col;
}

struct DepthPeelOutput 
{ 
	float4 LinearDepth; 
	float Peel;
}; 

DepthPeelOutput DepthPeelFunc( float3 worldPos, float4x4 view, float4x4 viewPrj ) 
{ 
	DepthPeelOutput OUT; 

	// todo: not sure what future glsl4 expects here, same as CGFX or same as HLSL
	#ifdef SFX_CGFX_3
		float currZ = abs( mul( view, float4(worldPos, 1.0f) ).z );
		float4 Pndc  = mul( viewPrj, float4(worldPos, 1.0f) );
		float2 UV = Pndc.xy / Pndc.w * float2(0.5f, 0.5f) + 0.5f;

		float prevZ = sampleTransDepthTex(UV).r;
		float opaqZ = sampleOpaqueDepthTex(UV).r;
		float bias = 0.00002f;
		if (currZ < prevZ * (1.0f + bias) || currZ > opaqZ * (1.0f - bias))
		{
			discard;
		}

		float ld = abs( mul( view, float4(worldPos, 1.0f) ).z );
		OUT.LinearDepth = float4(ld, ld, ld, ld);
	#else
		float currZ = abs( mul( float4(worldPos, 1.0f), view ).z );
		float4 Pndc  = mul( float4(worldPos, 1.0f), viewPrj );
		float2 UV = Pndc.xy / Pndc.w * float2(0.5f, -0.5f) + 0.5f;

		float prevZ = sampleTransDepthTex(UV).r;
		float opaqZ = sampleOpaqueDepthTex(UV).r;
		float bias = 0.00002f;
		if (currZ < prevZ * (1.0f + bias) || currZ > opaqZ * (1.0f - bias))
		{
			discard;
		}

		float ld = abs( mul( float4(worldPos, 1.0f), view ).z );
		OUT.LinearDepth = float4(ld, ld, ld, ld);
	#endif

	OUT.Peel = 1.0f;

	return OUT; 
} 



// -------------------------------------- APP and DATA  --------------------------------------
struct APPDATA
{
	float3 Position : POSITION;
	float2 map1 : TEXCOORD0;
	float3 Normal : NORMAL;
};

struct SHADERDATA
{
	float4 Position : SV_Position;
	float4 map1 : TEXCOORD0;
	float4 Normal : NORMAL;
	float4 WorldPosition : TEXCOORD1;
};

// -------------------------------------- UVPannerFunction --------------------------------------
struct UVPannerOutput
{
	float2 PannedUV;
};

UVPannerOutput UVPannerFunction(float2 UV, float Time, float PanAmountInU, float PanAmountInV)
{
	UVPannerOutput OUT;

	float MulOp = (Time * PanAmountInU);
	float MulOp1266 = (Time * PanAmountInV);
	float2 VectorConstruct = float2(MulOp, MulOp1266);
	float2 AddOp = (UV + VectorConstruct.xy);
	OUT.PannedUV = AddOp;

	return OUT;
}

// -------------------------------------- AmbientLightFunction --------------------------------------
struct AmbientLightOutput
{
	float3 LightColor;
};

AmbientLightOutput AmbientLightFunction(int ActiveLightIndex, float3 AlbedoColor, float3 LightColor, float LightIntensity)
{
	AmbientLightOutput OUT;

	float3 MulOp = (LightIntensity * (AlbedoColor * LightColor));
	OUT.LightColor = MulOp;

	return OUT;
}

// -------------------------------------- GetLightVectorFunction --------------------------------------
struct GetLightVectorOutput
{
	float3 Result;
};

GetLightVectorOutput GetLightVectorFunction(int ActiveLightIndex, float3 LightPosition, float3 VertexWorldPosition, int LightType, float3 LightDirection)
{
	GetLightVectorOutput OUT;

	bool IsDirectionalLight = (LightType == 4);
	float3 LerpOp = lerp((LightPosition - VertexWorldPosition), -(LightDirection), IsDirectionalLight);
	OUT.Result = LerpOp;

	return OUT;
}

// -------------------------------------- LambertDiffuseFunction --------------------------------------
struct LambertDiffuseOutput
{
	float3 Color;
};

LambertDiffuseOutput LambertDiffuseFunction(int ActiveLightIndex, float3 AlbedoColor, float3 Normal, float3 LightVector)
{
	LambertDiffuseOutput OUT;

	float SatOp = saturate(dot(Normal, LightVector));
	float3 Diffuse = (AlbedoColor * SatOp);
	OUT.Color = Diffuse;

	return OUT;
}

// -------------------------------------- LightDecayFunction --------------------------------------
struct LightDecayOutput
{
	float Attenuation;
};

LightDecayOutput LightDecayFunction(int ActiveLightIndex, float3 LightVectorUN, float Attenuation)
{
	LightDecayOutput OUT;

	bool IsAttenuationUsed = (Attenuation > 0.001);
	float DecayContribution457 = 0.0;
	if (IsAttenuationUsed)
	{
		float PowOp = pow(length(LightVectorUN), Attenuation);
		float DivOp = (1.0 / PowOp);
		DecayContribution457 = DivOp;
	}
	else
	{
		DecayContribution457 = 1.0;
	}
	OUT.Attenuation = DecayContribution457;

	return OUT;
}

// -------------------------------------- LightConeAngleFunction --------------------------------------
struct LightConeAngleOutput
{
	float ConeAngle;
};

LightConeAngleOutput LightConeAngleFunction(int ActiveLightIndex, float3 LightVector, float3 LightDirection, float ConeAngle, float ConeFalloff)
{
	LightConeAngleOutput OUT;

	float CosOp = cos(max(ConeFalloff, ConeAngle));
	float DotOp = dot(LightVector, -(LightDirection));
	float SmoothStepOp = smoothstep(CosOp, cos(ConeAngle), DotOp);
	OUT.ConeAngle = SmoothStepOp;

	return OUT;
}

// -------------------------------------- ShadowMapFunction --------------------------------------
struct ShadowMapOutput
{
	float LightGain;
};

ShadowMapOutput ShadowMapFunction(int ActiveLightIndex, float4x4 LightViewPrj, float ShadowMapBias, float3 VertexWorldPosition)
{
	ShadowMapOutput OUT;

	float IfElseOp540 = 0.0;
	float4 VectorConstruct = float4(VertexWorldPosition.x, VertexWorldPosition.y, VertexWorldPosition.z, 1.0);
	float4 MulOp = mul(VectorConstruct, LightViewPrj);
	float3 DivOp = (MulOp.xyz / MulOp.w);
	if (DivOp.x > -1.0 && DivOp.x < 1.0 && DivOp.y > -1.0 && DivOp.y < 1.0 && DivOp.z > 0.0 && DivOp.z < 1.0)
	{
		float Val528 = 0.5;
		float2 AddOp = ((DivOp.xy * Val528) + Val528);
		float SubOp = (DivOp.z - (ShadowMapBias / MulOp.w));
		float ShadowTotal = 0.0;
		for(int i=0; i<10; i+=1)
		{
			Texture2D _LightShadowMap = GetLightShadowMap(ActiveLightIndex);
			float2 MulOp580 = (float2(1.0, 1.0) * 0.00195313);
			float4 Sampler = SampleFromShadowMap(ActiveLightIndex, (float2(AddOp.x, 1.0-AddOp.y) + MulOp580));
			float IfElseOp534 = ((SubOp - Sampler.x) >= 0.0) ? (0.0) : (0.1);
			ShadowTotal += IfElseOp534;
		}
		IfElseOp540 = ShadowTotal;
	}
	else
	{
		IfElseOp540 = 1.0;
	}
	OUT.LightGain = IfElseOp540;

	return OUT;
}

// -------------------------------------- LightContributionFunction --------------------------------------
struct LightContributionOutput
{
	float3 Light;
};

LightContributionOutput LightContributionFunction(int ActiveLightIndex, float3 VertexWorldPosition, float3 LightVectorUN)
{
	LightContributionOutput OUT;

	float _LightIntensity = GetLightIntensity(ActiveLightIndex);
	int _LightType = GetLightType(ActiveLightIndex);
	bool IsDirectionalLight = (_LightType == 4);
	float DecayMul503 = 0.0;
	if (IsDirectionalLight)
	{
		DecayMul503 = 1.0;
	}
	else
	{
		float LightAttenuation = GetLightAttenuation(ActiveLightIndex);
		LightDecayOutput LightDecay451 = LightDecayFunction(ActiveLightIndex, LightVectorUN, LightAttenuation);
		DecayMul503 = LightDecay451.Attenuation;
	}
	bool IsSpotLight = (_LightType == 2);
	float ConeMul505 = 1.0;
	if (IsSpotLight)
	{
		float3 NormOp = normalize(LightVectorUN);
		float3 _LightDir = GetLightDir(ActiveLightIndex);
		float _LightConeAngle = GetLightConeAngle(ActiveLightIndex);
		float _LightFalloff = GetLightFalloff(ActiveLightIndex);
		LightConeAngleOutput LightConeAngle393 = LightConeAngleFunction(ActiveLightIndex, NormOp, _LightDir, _LightConeAngle, _LightFalloff);
		ConeMul505 = LightConeAngle393.ConeAngle;
	}
	bool _LightShadowOn = GetLightShadowOn(ActiveLightIndex);
	float ShadowMul506 = 1.0;
	if (_LightShadowOn)
	{
		float4x4 _LightViewPrj = GetLightViewPrj(ActiveLightIndex);
		ShadowMapOutput ShadowMap519 = ShadowMapFunction(ActiveLightIndex, _LightViewPrj, 0.01, VertexWorldPosition);
		float3 _LightShadowColor = GetLightShadowColor(ActiveLightIndex);
		float ShadowColorMix = lerp(ShadowMap519.LightGain, 1.0, _LightShadowColor.x);
		ShadowMul506 = ShadowColorMix;
	}
	float DecayShadowConeMul = (DecayMul503 * (ConeMul505 * ShadowMul506));
	float3 LightColor = GetLightColor(ActiveLightIndex);
	float3 MulItensity = (_LightIntensity * (DecayShadowConeMul * LightColor));
	OUT.Light = MulItensity;

	return OUT;
}

// -------------------------------------- BlinnSpecularFunction --------------------------------------
struct BlinnSpecularOutput
{
	float3 SpecularColor;
};

BlinnSpecularOutput BlinnSpecularFunction(int ActiveLightIndex, float3 LightVector, float3 Normal, float3 CameraVector, float SpecularPower, float3 SpecularColor)
{
	BlinnSpecularOutput OUT;

	float3 NormOp = normalize((LightVector + CameraVector));
	float SatOp = saturate(dot(Normal, NormOp));
	float3 BlinnSpec = (pow(SatOp, SpecularPower) * SpecularColor);
	float SatOp968 = saturate(dot(Normal, LightVector));
	float3 MulOp = (BlinnSpec * SatOp968);
	OUT.SpecularColor = MulOp;

	return OUT;
}

// -------------------------------------- DesaturateColorFunction --------------------------------------
struct DesaturateColorOutput
{
	float DesaturateColor;
};

DesaturateColorOutput DesaturateColorFunction(int ActiveLightIndex, float3 Color)
{
	DesaturateColorOutput OUT;

	float3 Col = float3(0.300008,0.6,0.100008);
	float DotOp = dot(saturate(Color), Col.xyz);
	OUT.DesaturateColor = DotOp;

	return OUT;
}

// -------------------------------------- DesaturateColorFunction --------------------------------------
DesaturateColorOutput DesaturateColorFunction(float3 Color)
{
	DesaturateColorOutput OUT;

	float3 Col = float3(0.300008,0.6,0.100008);
	float DotOp = dot(saturate(Color), Col.xyz);
	OUT.DesaturateColor = DotOp;

	return OUT;
}

// -------------------------------------- ShaderVertex --------------------------------------
SHADERDATA ShaderVertex(APPDATA IN)
{
	SHADERDATA OUT;

	OUT.Position = float4(IN.Position, 1);
	float4 OutUVs = float4(IN.map1.x, IN.map1.y, 0.0, 0.0);
	OUT.map1 = OutUVs;
	float3 MulOp = mul(IN.Normal, ((float3x3)world));
	float3 NormalN = normalize(MulOp);
	float4 WorldNormal = float4(NormalN.x, NormalN.y, NormalN.z, 1.0);
	OUT.Normal = WorldNormal;
	float4 WorldPos = mul(OUT.Position, world);
	OUT.WorldPosition = WorldPos;
	float4 WVSpace = mul(OUT.Position, wvp);
	OUT.Position = WVSpace;

	return OUT;
}

// -------------------------------------- ShaderPixel --------------------------------------
struct PIXELDATA
{
	float4 Color : SV_Target;
};

PIXELDATA ShaderPixel(SHADERDATA IN, bool FrontFace : SV_IsFrontFace)
{
	PIXELDATA OUT;

	float InvertSatMask = (1.0 - saturate(0.0));
	UVPannerOutput UVPanner1262 = UVPannerFunction(IN.map1.xy, time, SpeedU, SpeedV);
	float NoiseOp =  frac(sin(dot(UVPanner1262.PannedUV , float2(12.9898,78.233))) * 43758.5453); ;
	float3 VectorConstruct = float3(NoiseOp, NoiseOp, 1.0);
	float3 ReplaceDiffuseWithReflection = (InvertSatMask * VectorConstruct.xyz);
	float3 NormOp = normalize(IN.Normal.xyz);
	float3 FlippedNormals = lerp(-(NormOp), NormOp, FrontFace);
	float AddOp = (NoiseOp + Val);
	float ClampOpacity = saturate(AddOp);
	float3 CameraPosition = viewI[3].xyz;
	float3 CamVec = (CameraPosition - IN.WorldPosition.xyz);
	float3 CamVecNorm = normalize(CamVec);
	float4 LightLoopTotal11 = float4(0,0,0,0);
	for (int ActiveLightIndex = 0; ActiveLightIndex < 3; ++ActiveLightIndex)
	{
		if (ActiveLightIndex >= ClampDynamicLights) {continue;}
		int _LightType = GetLightType(ActiveLightIndex);
		bool IsAmbientLight = (_LightType == 5);
		float4 IfAmbientLight378 = float4(0, 0, 0, 0);
		if (IsAmbientLight)
		{
			float3 LightColor = GetLightColor(ActiveLightIndex);
			float _LightIntensity = GetLightIntensity(ActiveLightIndex);
			AmbientLightOutput AmbientLight383 = AmbientLightFunction(ActiveLightIndex, ReplaceDiffuseWithReflection, LightColor, _LightIntensity);
			float3 AmbientLightAndAO = (AddOp * AmbientLight383.LightColor);
			float4 VectorConstruct663 = float4(AmbientLightAndAO.x, AmbientLightAndAO.y, AmbientLightAndAO.z, 0.0);
			IfAmbientLight378 = VectorConstruct663;
		}
		else
		{
			float3 NoTranslucency = float3(0.0,0.0,0.0);
			float3 _LightPos = GetLightPos(ActiveLightIndex);
			float3 _LightDir = GetLightDir(ActiveLightIndex);
			GetLightVectorOutput GetLightVector838 = GetLightVectorFunction(ActiveLightIndex, _LightPos, IN.WorldPosition.xyz, _LightType, _LightDir);
			float3 LightVecNorm = normalize(GetLightVector838.Result);
			LambertDiffuseOutput LambertDiffuse813 = LambertDiffuseFunction(ActiveLightIndex, ReplaceDiffuseWithReflection, FlippedNormals, LightVecNorm);
			LightContributionOutput LightContribution391 = LightContributionFunction(ActiveLightIndex, IN.WorldPosition.xyz, GetLightVector838.Result);
			float3 AddTranslucency = (NoTranslucency.xyz + (LambertDiffuse813.Color * LightContribution391.Light));
			float3 Col = float3(1.0,1.0,1.0);
			BlinnSpecularOutput BlinnSpecular956 = BlinnSpecularFunction(ActiveLightIndex, LightVecNorm, FlippedNormals, CamVecNorm, 20.0, Col.xyz);
			float3 SpecLightIntensity = (LightContribution391.Light * BlinnSpecular956.SpecularColor);
			float3 Diffuse_Spec = ((AddTranslucency * ClampOpacity) + SpecLightIntensity);
			DesaturateColorOutput DesaturateColor654 = DesaturateColorFunction(ActiveLightIndex, SpecLightIntensity);
			float4 Color_Alpha = float4(Diffuse_Spec.x, Diffuse_Spec.y, Diffuse_Spec.z, DesaturateColor654.DesaturateColor);
			IfAmbientLight378 = Color_Alpha;
		}
		float4 ApplyWeight389 = IfAmbientLight378;
		LightLoopTotal11 += ApplyWeight389;
	}
	float3 NoReflection = float3(0.0,0.0,0.0);
	float3 ReflectXmask = (0.0 * NoReflection.xyz);
	float3 DefaultEmissiveColor = float3(0.0,0.0,0.0);
	float3 DefaultIBLColor = float3(0.0,0.0,0.0);
	float3 PreMultipliedAlpha = ((DefaultEmissiveColor.xyz + DefaultIBLColor.xyz) * ClampOpacity);
	float3 AddReflection = (ReflectXmask + PreMultipliedAlpha);
	DesaturateColorOutput DesaturateColor357 = DesaturateColorFunction(ReflectXmask);
	float OpacityAndReflection = (ClampOpacity + DesaturateColor357.DesaturateColor);
	float4 TotalAmbientAndOpacity = float4(AddReflection.x, AddReflection.y, AddReflection.z, OpacityAndReflection);
	float4 LightLoopAndAfterLoop = (LightLoopTotal11 + TotalAmbientAndOpacity);
	float SatOp = saturate(LightLoopAndAfterLoop.w);
	float4 VectorConstruct31 = float4(LightLoopAndAfterLoop.xyz.x, LightLoopAndAfterLoop.xyz.y, LightLoopAndAfterLoop.xyz.z, SatOp);
	OUT.Color = VectorConstruct31;

	return OUT;
}

// -------------------------------------- ShaderPixelP1 --------------------------------------
struct PIXELDATAP1
{
	float4 Color0 : SV_Target0;
	float4 Color1 : SV_Target1;
};

PIXELDATAP1 ShaderPixelP1(SHADERDATA IN, bool FrontFace : SV_IsFrontFace)
{
	PIXELDATAP1 OUT;

	DepthPeelOutput DepthPeel = DepthPeelFunc(IN.WorldPosition.xyz, view, viewPrj);
	OUT.Color0 = ShaderPixel(IN, FrontFace).Color;
	OUT.Color1 = DepthPeel.LinearDepth;

	return OUT;
}

// -------------------------------------- ShaderPixelP2 --------------------------------------
struct PIXELDATAP2
{
	float4 Color0 : SV_Target0;
	float4 Color1 : SV_Target1;
};

PIXELDATAP2 ShaderPixelP2(SHADERDATA IN, bool FrontFace : SV_IsFrontFace)
{
	PIXELDATAP2 OUT;

	DepthPeelOutput DepthPeel = DepthPeelFunc(IN.WorldPosition.xyz, view, viewPrj);
	OUT.Color0 = ShaderPixel(IN, FrontFace).Color;
	OUT.Color1 = (DepthPeel.Peel * (OUT.Color0.w > 0.001f ? 1.0f : 0.0f));

	return OUT;
}

// -------------------------------------- ShaderPixelP3 --------------------------------------
struct PIXELDATAP3
{
	float4 Color0 : SV_Target0;
	float4 Color1 : SV_Target1;
};

PIXELDATAP3 ShaderPixelP3(SHADERDATA IN, bool FrontFace : SV_IsFrontFace)
{
	PIXELDATAP3 OUT;

	OUT.Color0 = ShaderPixel(IN, FrontFace).Color;
	OUT.Color1 = (OUT.Color0.w > 0.001f ? 1.0f : 0.0f);

	return OUT;
}

// -------------------------------------- technique T0 ---------------------------------------
technique11 T0
<
	bool overridesDrawState = false;
	int isTransparent = 1;
>
{
	pass P0
	<
		string drawContext = "colorPass";
	>
	{
		SetVertexShader(CompileShader(vs_5_0, ShaderVertex()));
		SetPixelShader(CompileShader(ps_5_0, ShaderPixel()));
		SetHullShader(NULL);
		SetDomainShader(NULL);
		SetGeometryShader(NULL);
	}

	pass P1
	<
		string drawContext = "transparentPeel";
	>
	{
		SetVertexShader(CompileShader(vs_5_0, ShaderVertex()));
		SetPixelShader(CompileShader(ps_5_0, ShaderPixelP1()));
		SetHullShader(NULL);
		SetDomainShader(NULL);
		SetGeometryShader(NULL);
	}

	pass P2
	<
		string drawContext = "transparentPeelAndAvg";
	>
	{
		SetVertexShader(CompileShader(vs_5_0, ShaderVertex()));
		SetPixelShader(CompileShader(ps_5_0, ShaderPixelP2()));
		SetHullShader(NULL);
		SetDomainShader(NULL);
		SetGeometryShader(NULL);
	}

	pass P3
	<
		string drawContext = "transparentWeightedAvg";
	>
	{
		SetVertexShader(CompileShader(vs_5_0, ShaderVertex()));
		SetPixelShader(CompileShader(ps_5_0, ShaderPixelP3()));
		SetHullShader(NULL);
		SetDomainShader(NULL);
		SetGeometryShader(NULL);
	}

}

