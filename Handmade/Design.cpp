#include "Design.h"
#include "Input.h"
#include "Screen.h"
#include "Cuboid.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_sdl.h"

//======================================================================================================
bool Design::OnEnter()
{
	m_mainShader = std::make_unique<Shader>();

	if (!m_mainShader->Create("Shaders/Main.vert", "Shaders/Main.frag"))
	{
		return false;
	}

	m_mainShader->BindAttribute("vertexIn");
	m_mainShader->BindAttribute("colorIn");
	m_mainShader->BindAttribute("textureIn");

	m_mainShader->BindUniform("model");
	m_mainShader->BindUniform("view");
	m_mainShader->BindUniform("projection");
	m_mainShader->BindUniform("isTextured");

	//===================================================================

	m_textShader = std::make_unique<Shader>();

	if (!m_textShader->Create("Shaders/Text.vert", "Shaders/Text.frag"))
	{
		return false;
	}

	m_textShader->BindAttribute("vertexIn");
	m_textShader->BindAttribute("colorIn");
	m_textShader->BindAttribute("textureIn");

	m_textShader->BindUniform("model");
	m_textShader->BindUniform("view");
	m_textShader->BindUniform("projection");
	m_textShader->BindUniform("textureImage");

	//===================================================================

	m_lightShader = std::make_unique<Shader>();

	if (!m_lightShader->Create("Shaders/Light.vert", "Shaders/Light.frag"))
	{
		return false;
	}

	m_lightShader->BindAttribute("vertexIn");
	m_lightShader->BindAttribute("colorIn");
	m_lightShader->BindAttribute("textureIn");
	m_lightShader->BindAttribute("normalIn");

	m_lightShader->BindUniform("model");
	m_lightShader->BindUniform("view");
	m_lightShader->BindUniform("projection");
	m_lightShader->BindUniform("normal");

	m_lightShader->BindUniform("isTextured");
	m_lightShader->BindUniform("cameraPosition");

	m_lightShader->BindUniform("light.ambient");
	m_lightShader->BindUniform("light.diffuse");
	m_lightShader->BindUniform("light.specular");
	m_lightShader->BindUniform("light.position");

	m_lightShader->BindUniform("material.ambient");
	m_lightShader->BindUniform("material.diffuse");
	m_lightShader->BindUniform("material.specular");
	m_lightShader->BindUniform("material.shininess");

	//===================================================================

	Material::Load("Defaults", "Defaults.mtl");

	//===================================================================

	if (!(Text::Initialize()))
	{
		return false;
	}

	if (!(Audio::Initialize()))
	{
		return false;
	}

	//===================================================================

	m_grid = std::make_unique<Grid>();
	m_grid->GetTransform().SetRotation(45.0f, -30.0f, 0.0f);

	m_axes = std::make_unique<Axes>("Arrow.obj");

	/*m_topText = std::make_unique<Text>("Quikhand", "Quikhand.ttf", 30);
	m_topText->SetColor(1.0f, 0.0f, 0.196f, 1.0f);
	m_topText->SetString("Handmade Alpha");

	m_bottomText = std::make_unique<Text>("Quikhand");
	m_bottomText->SetColor(0.0f, 0.564f, 1.0f, 1.0f);
	m_bottomText->SetString("Click/Drag the mouse to rotate the grid. Use the mouse wheel to zoom in/out");

	m_axesLabelText = std::make_unique<Text>("Arial", "Arial.ttf", 66);
	m_axesLabelText->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_axesLabelText->SetString("X");*/

	//==========================================================================

	m_light = std::make_unique<Light>(0.0f, 7.5f, 0.0f);
	m_gecko = std::make_unique<Model>("Gecko", "Gecko.obj", true);
	m_gecko->GetTransform().SetScale(5.0f, 5.0f, 5.0f);

	//=========================================================================

	m_cuboid = std::make_unique<Cuboid>();
	m_cuboid->GetTransform().SetPosition(2.0f, 0.0f, 2.0f);

	m_bed = std::make_unique<Model>("Bed", "Bed.obj", true);
	m_bed->GetTransform().SetScale(5.0f, 5.0f, 5.0f);
	m_bed->GetTransform().SetPosition(-5.0f, 0.0f, 2.0f);

	m_mainCamera = std::make_unique<FreeCamera>();
	m_mainCamera->SetVelocity(0.0f);
	m_mainCamera->SetSensitivity(0.0f);
	m_mainCamera->GetTransform().SetPosition(0.0f, 0.0f, 50.0f);

	m_UICamera = std::make_unique<FreeCamera>();
	m_UICamera->SetVelocity(0.0f);
	m_UICamera->SetSensitivity(0.0f);
	
	//=========================================================================

	Screen::Instance()->SetColor(29U, 29U, 29U);
	
	return true;
}
//======================================================================================================
State* Design::Update(int deltaTime)
{
	Audio::Update();

	if (Input::Instance()->IsXClicked())
	{
		return nullptr;
	}

	//==============================================================================

	auto camPos = m_mainCamera->GetTransform().GetPosition();
	camPos.z -= (Input::Instance()->GetMouseWheel().y);
	m_mainCamera->GetTransform().SetPosition(camPos);

	auto mouseMotion = Input::Instance()->GetMouseMotion();
	static glm::vec3 eulerAngles = m_grid->GetTransform().GetEulerAngles();

	if (Input::Instance()->IsLeftButtonClicked())
	{
		eulerAngles.x += -mouseMotion.y;
		eulerAngles.y += mouseMotion.x;
	}

	m_grid->GetTransform().SetRotation(eulerAngles);

	//==============================================================================
	
	for (const auto& object : m_objects)
	{
		if (object->IsActive())
		{
			object->Update(static_cast<GLfloat>(deltaTime));
		}
	}

	//==============================================================================
	
	auto resolution = Screen::Instance()->GetResolution();
	Screen::Instance()->SetViewport(0, 0, resolution.x, resolution.y);

	return this;
}
//======================================================================================================
bool Design::Render()
{

	auto& mainShader = *m_mainShader.get();
	auto& textShader = *m_textShader.get();
	auto& lightShader = *m_lightShader.get();
	auto& testShader = *m_testShader.get();

	auto resolution = Screen::Instance()->GetResolution();

	mainShader.Use();

	m_mainCamera->CreatePerspView();
	m_mainCamera->Update(16.0f);
	m_mainCamera->SendToShader(mainShader);

	//==============================================================================

	m_grid->Render(mainShader);
	m_cuboid->Render(mainShader);

	lightShader.Use();
	lightShader.SendData("cameraPosition", m_mainCamera->GetTransform().GetPosition());

	m_light->SendToShader(lightShader);
	//m_light->Render(lightShader);
	m_mainCamera->SendToShader(lightShader);

	m_axes->GetTransform().SetRotation(m_grid->GetTransform().GetRotation());
	m_cuboid->GetTransform().SetRotation(m_grid->GetTransform().GetRotation());
	m_gecko->GetTransform().SetRotation(m_grid->GetTransform().GetRotation());
	m_bed->GetTransform().SetRotation(m_grid->GetTransform().GetRotation());

	m_axes->Render(lightShader);
	m_gecko->Render(lightShader);
	m_bed->Render(lightShader);
	
	//==============================================================================
	//Text rendering & UI
	//==============================================================================

	const auto PADDING = 25.0f;
	
	textShader.Use();

	m_UICamera->CreateOrthoView();
	m_UICamera->Update(16.0f);
	m_UICamera->SendToShader(textShader);

	/*m_topText->GetTransform().SetPosition(resolution.x - m_topText->GetTotalWidth() - PADDING,
		resolution.y - 50.0f, 0.0f);
	m_topText->SendToShader(textShader);
	m_topText->Render(textShader);*/

	/*m_bottomText->GetTransform().SetPosition(PADDING, PADDING, 0.0f);
	m_bottomText->SendToShader(textShader);
	m_bottomText->Render(textShader);*/

	/*auto labelPosition = m_mainCamera->ConvertWorldToScreen(m_axes->GetArrowTipPositionX());
	m_axesLabelText->IsFirstLetterCentered(true);
	m_axesLabelText->GetTransform().SetPosition(labelPosition.x, labelPosition.y, 0.0f);
	m_axesLabelText->SetString("X");
	m_axesLabelText->SendToShader(textShader);
	m_axesLabelText->Render(textShader);

	labelPosition = m_mainCamera->ConvertWorldToScreen(m_axes->GetArrowTipPositionY());
	m_axesLabelText->GetTransform().SetPosition(labelPosition.x, labelPosition.y, 0.0f);
	m_axesLabelText->SendToShader(textShader);
	m_axesLabelText->SetString("Y");
	m_axesLabelText->Render(textShader);

	labelPosition = m_mainCamera->ConvertWorldToScreen(m_axes->GetArrowTipPositionZ());
	m_axesLabelText->GetTransform().SetPosition(labelPosition.x, labelPosition.y, 0.0f);
	m_axesLabelText->SendToShader(textShader);
	m_axesLabelText->SetString("Z");
	m_axesLabelText->Render(textShader);*/

	for (const auto& object : m_objects)
	{
		if (object->IsVisible())
		{
			object->Render(lightShader);
		}
	}

	//==============================================================================
	//ImGUI UI (WIP)
	//==============================================================================

	// Start the Dear ImGui frame
	//ImGui_ImplOpenGL3_NewFrame();
	//ImGui_ImplSDL2_NewFrame();
	//ImGui::NewFrame();

	////Begin creates a new window and must have end
	//ImGui::Begin("A brand new window");
	//
	////uncomment to make it live in this window
	////ImGui::Text("Hello Handmade");  
	////ImGui::Button("Button");
	//
	//ImGui::End();
	//
	//ImGui::Button("Button");
	//
	//ImGui::Text("Hello Handmade");

	//static float f1 = 0.0f;
	//static float f2 = 0.0f;
	//static float f3 = 0.0f;
	//
	//ImGui::SliderFloat("Some random data", &f1, 0.0f, 1.0f);
	//ImGui::SliderFloat("Some more data", &f2, 0.0f, 1.0f);
	//ImGui::SliderFloat("Useful data", &f3, 0.0f, 1.0f);

	////bool showWindow = false;
	////ImGui::Checkbox("Check me", &showWindow);

	//ImVec4 color = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	//ImGui::ColorEdit3("clear color", (float*)&color);

	//ImGui::Render();
	//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return true;
}
//======================================================================================================
void Design::OnExit()
{
	m_objects.clear();
	Audio::Shutdown();
	Text::Shutdown();
}