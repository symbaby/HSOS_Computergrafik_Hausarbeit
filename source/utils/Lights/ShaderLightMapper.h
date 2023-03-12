#ifndef LIGHTMANAGER_H
#define LIGHTMANAGER_H

#include <vector>
#ifdef WIN32
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#else
#define GLFW_INCLUDE_GLCOREARB
#define GLFW_INCLUDE_GLEXT
#include <glfw/glfw3.h>
#endif
#include "utils/Lights/Lights.h"

#define MaxLightCount 14

class ShaderLightMapper
{
public:
	typedef std::vector<BaseLight*> LightList;

	void addLight(BaseLight* Light);
	const LightList& lights() const { return Lights; }
	void clear();

	void activate();
	void deactivate();
	static ShaderLightMapper& instance();
	GLuint uniformBlockID() { return UBO;  }
protected:
	struct ShaderLight
	{
		int Type; //Vector xVecPadding;
        Vector padding5; //float xPaddingFloat;
		Color Color; float padding0;
		Vector Position; float padding1;
		Vector Direction; float padding2;
		Vector Attenuation; float padding3;
		Vector SpotRadius;
		int ShadowIndex;

        // Perly Noise für game
	};

	struct ShaderLightBlock
	{
		int LightCount; Vector padding0;
		ShaderLight lights[MaxLightCount];

	};
protected:
	ShaderLightMapper();
	ShaderLightMapper(const ShaderLightMapper& m) {}
	~ShaderLightMapper();
	LightList Lights;
	ShaderLightBlock ShaderLights;
	GLuint UBO;
	static ShaderLightMapper* pMapper;
};

#endif

