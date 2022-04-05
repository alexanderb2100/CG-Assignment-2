#include "Toonshading.h"
#include "Utils/ResourceManager/ResourceManager.h"
#include "Utils/JsonGlmHelpers.h"
#include "Utils/ImGuiHelper.h"

Toonshading::Toonshading() :
	PostProcessingLayer::Effect(),

	_shader(nullptr),
	_OutlineColor(glm::vec4(1.0, 0.5, 0.5, 1.0)),
	_pixelsize(1.0, 1.0),
	_depthThreshold(1.0f),
	_normalThreshold(1.5f),
	_depthNormalThreshold(0.5f),
	_depthNormalThresholdScale(5)
{
	Name = "Toon Shading";
	_format = RenderTargetType::ColorRgb8;

	_shader = ResourceManager::CreateAsset<ShaderProgram>(std::unordered_map<ShaderPartType, std::string>{
		{ ShaderPartType::Vertex, "shaders/vertex_shaders/fullscreen_quad.glsl" },
		{ ShaderPartType::Fragment, "shaders/fragment_shaders/post_effects/toon_shading.glsl" }
	});
}

Toonshading::~Toonshading() = default;

void Toonshading::Apply(const Framebuffer::Sptr & gBuffer)
{
	_shader->Bind();
	_shader->SetUniform("u_OutlineColor", _OutlineColor);
	_shader->SetUniform("u_PixelSize", glm::vec2(1.0f) / (glm::vec2)gBuffer->GetSize());
	_shader->SetUniform("u_DepthThreshold", _depthThreshold);
	_shader->SetUniform("u_NormalThreshold", _normalThreshold);
	_shader->SetUniform("u_DepthNormThreshold", _depthNormalThreshold);
	_shader->SetUniform("u_DepthNormThresholdScale", _depthNormalThresholdScale);
	gBuffer->BindAttachment(RenderTargetAttachment::Depth, 1);
	gBuffer->BindAttachment(RenderTargetAttachment::Color1, 2); 
}

void Toonshading::RenderImGui()
{
}

Toonshading::Sptr Toonshading::FromJson(const nlohmann::json & data)
{
	Toonshading::Sptr result = std::make_shared<Toonshading>();
	result->Enabled = JsonGet(data, "enabled", false);
	return result;
}

nlohmann::json Toonshading::ToJson() const
{
	return
	{
		{ "enabled", 0 },
	};
}