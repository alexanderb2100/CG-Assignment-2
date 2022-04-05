#pragma once

#include "Application/Layers/PostProcessingLayer.h"
#include "Graphics/ShaderProgram.h"
#include "Graphics/Framebuffer.h"
#include "Graphics/Textures/Texture3D.h"
#include "Utils/ImGuiHelper.h"

class DepthOfField : public PostProcessingLayer::Effect {
public:
	MAKE_PTRS(DepthOfField);

	DepthOfField();
	virtual ~DepthOfField();

	virtual void Apply(const Framebuffer::Sptr& gBuffer) override;

	virtual void RenderImGui() override;


	DepthOfField::Sptr FromJson(const nlohmann::json& data);

	virtual nlohmann::json ToJson() const override;

protected:
	ShaderProgram::Sptr _shader;
};