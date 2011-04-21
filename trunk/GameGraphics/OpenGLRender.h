#pragma once

#define TEXTURE_PARTICLE_BUFFER_SIZE 1024
#define LINE_BUFFER_SIZE			 1024
#define POINT_BUFFER_SIZE			 1024
#define SELECTION_BUFFER_SIZE		 1024

struct SRenderState
{
	bool bActiveTextures;
	bool bActiveLighting;
	bool bActiveSolid;
	bool bActiveShadowEmission;
	bool bActiveWater;
	bool bActiveShadowReception;

	bool bActiveDepth;
	unsigned long nDepthFunction;

	bool bActiveBlending;
	unsigned int nBlendOperator1;
	unsigned int nBlendOperator2;
	unsigned int nBlendingLayer;

	bool bActiveHeightFog;
	double dHeightFogStart;
	double dHeightFogEnd;
	CVector vHeightFogColor;

	bool operator <(const SRenderState &otherState) const
	{
		return Compare(otherState)<0;
	}

	int Compare(const SRenderState &otherState) const
	{
		if(bActiveLighting<otherState.bActiveLighting){return -1;}
		if(bActiveLighting>otherState.bActiveLighting){return 1;}

		if(bActiveDepth<otherState.bActiveDepth){return -1;}
		if(bActiveDepth>otherState.bActiveDepth){return 1;}
		if(nDepthFunction<otherState.nDepthFunction){return -1;}
		if(nDepthFunction>otherState.nDepthFunction){return 1;}

		if(bActiveHeightFog<otherState.bActiveHeightFog){return -1;}
		if(bActiveHeightFog>otherState.bActiveHeightFog){return 1;}

		if(bActiveBlending<otherState.bActiveBlending){return -1;}
		if(bActiveBlending>otherState.bActiveBlending){return 1;}
		if(nBlendingLayer<otherState.nBlendingLayer){return -1;}
		if(nBlendingLayer>otherState.nBlendingLayer){return 1;}
		if(nBlendOperator1<otherState.nBlendOperator1){return -1;}
		if(nBlendOperator1>otherState.nBlendOperator1){return 1;}
		if(nBlendOperator2<otherState.nBlendOperator2){return -1;}
		if(nBlendOperator2>otherState.nBlendOperator2){return 1;}

		if(bActiveTextures<otherState.bActiveTextures){return -1;}
		if(bActiveTextures>otherState.bActiveTextures){return 1;}

		if(bActiveSolid<otherState.bActiveSolid){return -1;}
		if(bActiveSolid>otherState.bActiveSolid){return 1;}

		if(bActiveShadowEmission<otherState.bActiveShadowEmission){return -1;}
		if(bActiveShadowEmission>otherState.bActiveShadowEmission){return 1;}
		if(bActiveShadowReception<otherState.bActiveShadowReception){return -1;}
		if(bActiveShadowReception>otherState.bActiveShadowReception){return 1;}
		if(bActiveWater<otherState.bActiveWater){return -1;}
		if(bActiveWater>otherState.bActiveWater){return 1;}
		return 0;
	}

	SRenderState()
	{
		bActiveTextures=true;
		bActiveLighting=false;
		bActiveSolid=true;
		bActiveShadowEmission=true;
		bActiveShadowReception=true;

		bActiveDepth=true;
		nDepthFunction=GL_LESS;

		bActiveBlending=false;
		nBlendOperator1=GL_SRC_ALPHA;
		nBlendOperator2=GL_ONE_MINUS_SRC_ALPHA;
		nBlendingLayer=0;
		
		bActiveHeightFog=false;
		dHeightFogStart=0;
		dHeightFogEnd=0;
		bActiveWater=false;
	}
};


struct SRenderOptions
{
	bool bEnableTextures;
	bool bEnableLighting;
	bool bEnableSolid;
	bool bEnableShadows;
	bool bEnableBlending;
	bool bEnableHeightFog;
	bool bEnableShader;

	SRenderOptions()
	{
		bEnableShader=true;
		bEnableHeightFog=false;
		bEnableTextures=true;
		bEnableLighting=false;
		bEnableShadows=true;
		bEnableSolid=true;
		bEnableBlending=true;
	}
};

struct SHardwareSupport
{
	bool bShaders;
	int  nMaxLights;
	int  nMaxTextureUnits;

	SHardwareSupport(){bShaders=false;nMaxLights=8;nMaxTextureUnits=2;}
};

struct STextureParticleBuffer
{
	int nUsedElements;
	float pVertexBuffer[3*4*TEXTURE_PARTICLE_BUFFER_SIZE];
	float pColorBuffer[4*4*TEXTURE_PARTICLE_BUFFER_SIZE];
	float pTexBuffers[2*4*TEXTURE_PARTICLE_BUFFER_SIZE];

	STextureParticleBuffer(){nUsedElements=0;}
};

struct STextureParticleStage
{
	std::vector<STextureParticleBuffer*> vBuffers;
};

struct STextureParticleStageKey
{
	SRenderState  sRenderState;
	IGenericTexture *piTexture;

	bool operator <(const STextureParticleStageKey &otherStage) const
	{
		int nStateComp=sRenderState.Compare(otherStage.sRenderState);
		if(nStateComp<0){return true;}
		if(nStateComp>0){return false;}
		if(piTexture<otherStage.piTexture){return true;}
		if(piTexture>otherStage.piTexture){return false;}
		return false;
	}

	STextureParticleStageKey(){piTexture=NULL;}
	STextureParticleStageKey(IGenericTexture *texture,SRenderState renderState){piTexture=texture;sRenderState=renderState;}
};


struct SPointBuffer
{
	int nUsedElements;
	float pVertexBuffer[3*2*POINT_BUFFER_SIZE];
	float pColorBuffer[4*2*POINT_BUFFER_SIZE];

	SPointBuffer(){nUsedElements=0;}
};

struct SPointStage
{
	std::vector<SPointBuffer*> vBuffers;
};

struct SPointStageKey
{
	SRenderState	sRenderState;
	double			dSize;

	bool operator <(const SPointStageKey &otherStage) const
	{
		int nStateComp=sRenderState.Compare(otherStage.sRenderState);
		if(nStateComp<0){return true;}
		if(nStateComp>0){return false;}
		if(dSize<otherStage.dSize){return true;}
		if(dSize>otherStage.dSize){return false;}
		return false;
	}

	SPointStageKey(){dSize=0;}
	SPointStageKey(SRenderState renderState,unsigned long size){sRenderState=renderState;dSize=size;}
};


struct SLineBuffer
{
	int nUsedElements;
	float pVertexBuffer[3*2*LINE_BUFFER_SIZE];
	float pColorBuffer[4*2*LINE_BUFFER_SIZE];

	SLineBuffer(){nUsedElements=0;}
};

struct SLineStage
{
	std::vector<SLineBuffer*> vBuffers;
};

struct SLineStageKey
{
	SRenderState  sRenderState;
	unsigned long nStipple;

	bool operator <(const SLineStageKey &otherStage) const
	{
		int nStateComp=sRenderState.Compare(otherStage.sRenderState);
		if(nStateComp<0){return true;}
		if(nStateComp>0){return false;}
		if(nStipple<otherStage.nStipple){return true;}
		if(nStipple>otherStage.nStipple){return false;}
		return false;
	}

	SLineStageKey(){nStipple=0;}
	SLineStageKey(SRenderState renderState,unsigned long stipple){sRenderState=renderState;nStipple=stipple;}
};


struct SModelInstance
{
	CVector vPos;
	CVector vAngles;
	bool bSkipRender;
	SModelInstance(){bSkipRender=false;}
};

struct SModelStageKey
{
	SRenderState  sRenderState;
	IGenericModel *piModel;
	unsigned int  nAnimation;
	unsigned int  nFrame;
	
	bool operator <(const SModelStageKey &otherStage) const
	{
		int nStateComp=sRenderState.Compare(otherStage.sRenderState);
		if(nStateComp<0){return true;}
		if(nStateComp>0){return false;}
		if(piModel<otherStage.piModel){return true;}
		if(piModel>otherStage.piModel){return false;}
		if(nAnimation<otherStage.nAnimation){return true;}
		if(nAnimation>otherStage.nAnimation){return false;}
		if(nFrame<otherStage.nFrame){return true;}
		if(nFrame>otherStage.nFrame){return false;}
		return false;
	}

	SModelStageKey(){piModel=NULL;nAnimation=0;nFrame=0;}
	SModelStageKey(IGenericModel *model,unsigned int animation,unsigned int frame,SRenderState &renderState){piModel=model;nAnimation=animation;nFrame=frame;sRenderState=renderState;}
};

struct SModelStage
{
	IGenericModel *piModel;
	IOpenGLModel *piGLModel;

	std::vector<SModelInstance> vInstances;

	SModelStage(){piModel=NULL;piGLModel=NULL;}
};


struct SShaderKey
{
	bool bHeightFog;
	bool bShadows;
	int  nTextureUnits;
	int  nActiveLighs;
	int  bWater;
	
	bool operator <(const SShaderKey &otherKey) const
	{
		if(bHeightFog<otherKey.bHeightFog){return true;}
		if(bHeightFog>otherKey.bHeightFog){return false;}
		if(bShadows<otherKey.bShadows){return true;}
		if(bShadows>otherKey.bShadows){return false;}
		if(nTextureUnits<otherKey.nTextureUnits){return true;}
		if(nTextureUnits>otherKey.nTextureUnits){return false;}
		if(nActiveLighs<otherKey.nActiveLighs){return true;}
		if(nActiveLighs>otherKey.nActiveLighs){return false;}
		if(bWater<otherKey.bWater){return true;}
		if(bWater>otherKey.bWater){return false;}
		return false;
	}

	SShaderKey(){bHeightFog=false;bShadows=false;nTextureUnits=0;nActiveLighs=0;bWater=false;}
	SShaderKey(bool heightFog,bool shadows,int textureUnits,int activeLighs,int water){bHeightFog=heightFog;bShadows=shadows;nTextureUnits=textureUnits;nActiveLighs=activeLighs;bWater=water;}
};

class COpenGLRender: virtual public CSystemObjectBase,virtual public IGenericRender
{
	bool		m_bPerspectiveProjection;
	double		m_dPerspectiveViewAngle;
	double		m_dPerspectiveNearPlane;
	double		m_dPerspectiveFarPlane;

	double m_dProyectionWidth;
	double m_dProyectionHeight;

	CVector m_vCameraForward;
	CVector m_vCameraRight;
	CVector m_vCameraUp;
	CVector m_vCameraPos;
	CVector m_vCameraAngles;

	int			m_nViewportX;
	int			m_nViewportY;
	int			m_nViewportW;
	int			m_nViewportH;

	bool		 m_bStagedRendering;
	GLuint		 m_pSelectionBuffer[SELECTION_BUFFER_SIZE];

	SRenderOptions m_sRenderOptions;

	SRenderState m_sRenderState;
	SRenderState m_sStagedRenderingState;
	SRenderState m_sPreStagedRenderingState;
	double		 m_dStagedRenderingMinZ;
	double		 m_dStagedRenderingMaxZ;
	double		 m_dMinDistanceToLight;

	bool		 m_bActiveHeightFog;
	double		 m_dHeightFogStart;
	double		 m_dHeightFogEnd;
	CVector		 m_vHeightFogColor;
	
	CVector      m_vLastShadowCameraTarget;
	CVector      m_vLastShadowCameraPosition;
	CVector      m_pvLastShadowVolume[8];
	double 		 m_dShadowAntiFlickeringMargin;
	
	bool		 m_bPrecompileShaders;
	
	unsigned int m_nFirstTimeStamp;
	
	unsigned long m_nActiveLights;
	bool m_bLightingPrepared;

	CVector m_vColor;
	double  m_dAlpha;
	IGenericFont     *m_piSelectedFont;
	IGenericViewport *m_piCurrentViewport;

	CGenericShaderWrapper m_ShadowShader;
	CGenericShaderWrapper *m_pCurrentShader;
	
	CGenericTextureWrapper m_ShadowTexture;
	CGenericLightWrapper  m_SunLight;

	CPlane				m_CameraFustrumPlanes[4];
	CPlane				m_CameraForwardPlane;
	CPlane				m_CameraRightPlane;
	CPlane				m_CameraUpPlane;

	bool				m_bRestoreTextureMatrix;
	bool				m_bRenderingWithShader;
	bool				m_bShadowVolumeFirstVertex;
	CVector				m_vShadowVolumeMins;
	CVector				m_vShadowVolumeMaxs;
	bool				m_bHardwareSupportRead;
	SHardwareSupport	m_sHardwareSupport;
	bool				m_bIgnoreShaderSupport;
	
	std::stack<SRenderState> m_sStagedRenderingStateStack;
	std::stack<SRenderState> m_sRenderStateStack;
	std::stack<SRenderOptions> m_sRenderOptionsStack;
	std::map<SShaderKey,CGenericShaderWrapper> m_mShaders;

	CVector m_vAmbientColor;

	std::vector<IGenericLight *> m_vLights;
	std::map<unsigned long,IGenericTexture *>					m_mTextureLevels;
	std::map<SPointStageKey,SPointStage>						m_mPointStages;
	std::map<SLineStageKey,SLineStage>							m_mLineStages;
	std::map<STextureParticleStageKey,STextureParticleStage>	m_mTextureParticleStages;
	std::map<SModelStageKey,SModelStage>						m_mModelStages;
	bool m_bRenderingShadowReception;
	void UpdateProjectionMatrix();

	void SetRenderState(const SRenderState &sNewState,bool bForce);

	void PrepareLighting();
	void UnPrepareLighting();

	void ProcessCameraVertex( const CVector &vVertex );

	void RenderAllStages(bool bRenderingShadow,bool bShadowReceptionState);
	void RenderPointStages(bool bRenderingShadow,bool bShadowReceptionState);
	void RenderLineStages(bool bRenderingShadow,bool bShadowReceptionState);
	void RenderParticleStages(bool bRenderingShadow,bool bShadowReceptionState);
	void RenderModelStages(bool bRenderingShadow,bool bShadowReceptionState);

	void AddShader(const SShaderKey &key);

public:

	bool Init(std::string sClass,std::string sName,ISystem *piSystem);
	void Destroy();

	// IGenericRender

	void StartFrame(IGenericViewport *piViewport);
	void EndFrame();

	IGenericViewport *GetViewPort(); // solo valido entre StartFrame y EndFrame.

	void SetOrthographicProjection(double cx,double cy);
	void SetPerspectiveProjection(double dViewAngle,double dNearPlane,double dFarPlane);
	void SetViewport(double x,double y,double cx, double cy);
	void SetCamera(const CVector &vPosition,double dYaw, double dPitch, double dRoll);

	void ActivateClipping(bool bActivate);
	void SetClipRect(double x,double y,double cx, double cy);

	void Clear(const CVector &vColor,double dAlpha);
	void SetColor(const CVector &vColor,double dAlpha);

	void SelectFont(const char *pFamilyName,double dSize);
	void SelectTexture(IGenericTexture *pTexture,int nTextureLevel);
	void SetTextureMatrix(CMatrix *pMatrix,int nTextureLevel);	
	void UnselectTexture(int nTextureLevel);

	void CalcTextSize(const char *pText,double *pdWidth,double *pdHeight);
	virtual void RenderPoint(const CVector &vPosition,double dSize,const CVector &vColor,double dAlpha);
	void RenderText(double x,double y,const char *pText);
	void RenderTextEx(double x,double y,double w,double h,const char *pText,eTextAlignment dwHorzAlign,eTextAlignment dwVertAlign);
	void RenderTexture(const CVector &vOrigin,double s1,double s2);
	void RenderParticle(IGenericTexture *piTexture,const CVector &vOrigin,double dAngle,double s1,double s2,const CVector &vColor,double dAlpha);
	void RenderModel(const CVector &vOrigin,const CVector &vOrientation,IGenericModel *piModel,unsigned int nAnimation,unsigned int nFrame);
	void RenderTextureRect(double dPosx,double dPosy,double dWidth,double dHeight,double dTexturex,double dTexturey,double dTextureWidth,double dTextureHeight);

	void RenderBBox(const CVector &vMins,const CVector &vMaxs,const CVector &vColor,unsigned long nStipple);
	void RenderBBox(const CVector &vOrigin,const CVector &vOrientation,const CVector &vMins,const CVector &vMaxs,const CVector &vColor,unsigned long nStipple=0x8888);
	void RenderLine(const CVector &v1,const CVector &v2,const CVector &vColor,unsigned long nStipple);
	void RenderLines(unsigned int nLines,const CVector *pPoints,const CVector &vColor,unsigned long nStipple=0x8888);
	void RenderLineStrip(unsigned int nLines,const CVector *pPoints,const CVector &vColor,unsigned long nStipple=0x8888);
	void RenderRect(const CVector &vCenter,const CVector &vAxisW,const CVector &vAxisH,double w,double h);
	void RenderRect(double x, double y,double w,double h);
	void RenderPolygon(unsigned int nVertexes,const CVector *pVertexes,const CVector *pColors);
	void RenderPyramid(const CVector &vTopVertex,const CVector &vSizes,bool bSolid);
	void RenderPyramid(const CVector &vTopVertex,double dUpperSizeX,double dUpperSizeZ,double dLowerSizeX,double dLowerSizeZ,double dHeight,bool bSolid);
	void RenderArrowHead(const CVector &vPosition,const CVector &vDirection,CVector &vUp,double dForward,double dUp,double dRight);
	
	void ActivateHeightFog(double dStart,double dEnd,const CVector &vColor);
	void DeactivateHeightFog();
	bool IsHeightFogActive();

	void ActivateTextures();
	void DeactivateTextures();
	bool AreTexturesActive();

	void ActivateLighting();
	void DeactivateLighting();
	bool IsLightingActive();
	
	void ActivateWater();
	void SetWaterMappingSize(double dMaxU,double dMaxV);
	void DeactivateWater();
	
	void SetAmbientLight(const CVector &vColor);
	void GetAmbientLight(CVector *pvColor);

	void SetSunLight(const CVector &vPosition,const CVector &vDirection,const CVector &vAmbientColor,const CVector &vDiffuseColor,const CVector &vSpecularColor);
	void GetSunLight(CVector *pvPosition,CVector *pvDirection,CVector *pvAmbientColor,CVector *pvDiffuseColor,CVector *pvSpecularColor);

	void AddLight(IGenericLight *);
	void RemoveLight(IGenericLight *);

	void ActivateSolid();
	void DeactivateSolid();
	bool IsSolidActive();

	void ActivateBlending();
	void DeactivateBlending();
	bool IsBlendingActive();
	void SetBlendingFunction(unsigned int nOperator1,unsigned int nOperator2);
	void SetBlendingLayer(unsigned int nLayer);
	
	void ActivateDepth();
	void DeactivateDepth();
	void SetDepthFunction(unsigned int nDepthFunc);

	void ActivateShadowEmission();
	void DeactivateShadowEmission();
	bool IsShadowEmissionActive();

	void ActivateShadowReception();
	void DeactivateShadowReception();
	bool IsShadowReceptionActive();

	void PushState();
	void PopState();

	// Render Options

	void EnableShaders();
	void DisableShaders();
	bool AreShadersEnabled();

	void EnableTextures();
	void DisableTextures();
	bool AreTexturesEnabled();

	void EnableHeightFog();
	void DisableHeightFog();
	bool IsHeightFogEnabled();

	void EnableLighting();
	void DisableLighting();
	bool IsLightingEnabled();

	void EnableSolid();
	void DisableSolid();
	bool IsSolidEnabled();

	void EnableBlending();
	void DisableBlending();
	bool IsBlendingEnabled();

	void EnableShadows();
	void DisableShadows();
	bool AreShadowsEnabled();

	void PushOptions();
	void PopOptions();

	// Staged Rendering

	void StartStagedRendering();
	void EndStagedRendering();
	bool IsRenderingWithShader();

	// Selection

	void StartSelection(SGameRect rWindowRect,IGenericCamera *piCamera,double dx,double dy,double dPrecision);
	void SetSelectionId(unsigned int nId);
	int EndSelection();

	BEGIN_PROP_MAP(COpenGLRender)
	PROP_VALUE_FLAGS(m_bIgnoreShaderSupport,"IgnoreShaderSupport",false,MRPF_NORMAL|MRPF_OPTIONAL)
	PROP_VALUE_FLAGS(m_dShadowAntiFlickeringMargin,"ShadowAntiFlickeringMargin",10,MRPF_NORMAL|MRPF_OPTIONAL)
	PROP_VALUE_FLAGS(m_bPrecompileShaders,"PrecompileShaders",true,MRPF_NORMAL|MRPF_OPTIONAL)
	END_PROP_MAP();
	
	COpenGLRender(void);
	~COpenGLRender(void);
};
