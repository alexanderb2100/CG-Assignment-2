#pragma once
#include "Application/Layers/PostProcessingLayer.h"
#include "Graphics/ShaderProgram.h"
#include "Graphics/Textures/Texture3D.h"
#include "Utils/ImGuiHelper.h"

class Toonshading : public PostProcessingLayer::Effect {
public:
	MAKE_PTRS(Toonshading);

	Toonshading();
	virtual ~Toonshading();

	virtual void Apply(const Framebuffer::Sptr& gBuffer) override;


	virtual void RenderImGui() override;



	Toonshading::Sptr FromJson(const nlohmann::json& data);
	virtual nlohmann::json ToJson() const override;

protected:
	ShaderProgram::Sptr _shader;
	glm::vec4           _OutlineColor;
	glm::vec2			_pixelsize;
	float               _depthThreshold;
	float               _normalThreshold;
	float               _depthNormalThreshold;
	float               _depthNormalThresholdScale;
	

};
