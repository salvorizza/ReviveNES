/*

int main(int argc, char** argv) {
	std::optional<NESEmu::Palette> palette = NESEmu::Utils::ReadPaletteFile("commons/palettes/2C02G.pal");

	NESEmu::Bus cpuBus("CPU");
	NESEmu::Bus ppuBus("PPU");

	NESEmu::VRAM vram;
	NESEmu::WRAM wram;
	NESEmu::RP2A03 cpu;
	NESEmu::RP2C02 ppu(palette.value());


	while (true) {
		
	}

	return 0;
}*/


#include "UI/Application/ApplicationManager.h"

#include "UI/Panels/ViewportPanel.h"

#include "UI/Graphics/BatchRenderer.h"
#include "UI/Window/FontAwesome5.h"

#include "Core/RP2A03.h"
#include "Core/RP2C02.h"

#include "Core/WRAM.h"
#include "Core/VRAM.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <imgui_internal.h>

#include <iostream>

using namespace NESEmu;


class ReviveNESApp : public Application {
public:
	ReviveNESApp()
		:	Application("ReviveNES"),
			mProjectionMatrix(glm::identity<glm::mat4>()),
			mCPUBus("CPU"),
			mPPUBus("PPU"),
			mPPU(Utils::ReadPaletteFile("commons/palettes/2C02G.pal").value())
	{

		mCPUBus.connectDevice(&mCPU);
		mCPUBus.connectDevice(&mWRAM);
		mCPUBus.connectDevice(&mPPU);

		mPPUBus.connectDevice(&mVRAM);
		mPPUBus.connectDevice(&mPPU);

		mCPU.reset();
	}

	~ReviveNESApp() {
	}

	virtual void onSetup() override {
		mBatchRenderer = std::make_shared<BatchRenderer>();
		mViewportPanel = std::make_shared<ViewportPanel>();
	}

	virtual void onUpdate() override {
		mCPU.clock();
		for (int i = 0; i < 3; i++) {
			mPPU.clock();
		}
	}

	virtual void onRender() override {
		mViewportPanel->startFrame();
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		mProjectionMatrix = glm::ortho(0.0f, (float)mViewportPanel->width(), (float)mViewportPanel->height(), 0.0f);

		float cellWidth = (float)mViewportPanel->width() / 256.0f;
		float cellHeight = (float)mViewportPanel->height() / 240.0f;
		glm::vec4 color;

		mBatchRenderer->begin(mProjectionMatrix, 0.1f, 1);

		for (size_t y = 0; y < 240; y++) {
			for (size_t x = 0; x < 256; x++) {
				PaletteColor col = mPPU.mScreen[y * 256 + x];

				glm::vec4 glColor = { col.R / 255.0f, col.G / 255.0f, col.B / 255.0f, 1.0f };

				mBatchRenderer->drawQuad({ x * cellWidth, y * cellHeight }, { cellWidth,cellHeight }, glColor);
			}
		}

		mBatchRenderer->end();
		mViewportPanel->endFrame();
	}

	virtual void onImGuiRender(const std::shared_ptr<ImGuiManager>& pManager, const std::shared_ptr<Window>& pWindow) override {
		static bool p_open = true;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_MenuBar;
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		window_flags |= ImGuiWindowFlags_NoTitleBar;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		ImGui::Begin("ReviveNES", &p_open, window_flags);
		ImGui::PopStyleVar();

		if (ImGui::BeginMenuBar())
		{

			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open", "CTRL+M")) {
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Tools"))
			{
				if (ImGui::MenuItem("Viewport", "CTRL+O")) mViewportPanel->open();

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Emulation"))
			{
				ImGui::MenuItem("Play");
				ImGui::MenuItem("Stop");
				ImGui::MenuItem("Pause");

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}


		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

		ImGui::End();

		mViewportPanel->render(pManager);
	}


private:
	std::shared_ptr<ViewportPanel> mViewportPanel;
	glm::mat4 mProjectionMatrix;
	std::shared_ptr<BatchRenderer> mBatchRenderer;


	Bus mCPUBus;
	Bus mPPUBus;
	VRAM mVRAM;
	WRAM mWRAM;
	RP2A03 mCPU;
	RP2C02 mPPU;
};

int main(int argc, char** argv) {
	ApplicationManager appManager;
	appManager.run(std::make_shared<ReviveNESApp>());
	return 0;
}