#include "UI/Application/ApplicationManager.h"

#include "UI/Panels/ViewportPanel.h"

#include "UI/Graphics/BatchRenderer.h"
#include "UI/Window/FontAwesome5.h"

#include "Core/RP2A03.h"
#include "Core/RP2C02.h"
#include "Core/WRAM.h"
#include "Core/VRAM.h"
#include "Core/CartridgeSlot.h"

#include "Utils/Cartridge.h"

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
		mCartridge = Cartridge::loadFromFile("commons/roms/nestest.nes");

		mCartridgeSlot.insert(mCartridge);

		mCPUBus.connectDevice(&mCPU);
		mCPUBus.connectDevice(&mWRAM);
		mCPUBus.connectDevice(&mPPU);
		mCPUBus.connectDevice(&mCartridgeSlot);

		mPPUBus.connectDevice(&mVRAM);
		mPPUBus.connectDevice(&mPPU);


		mCPU.powerUp();
		mCPU.reset();

		mPPU.powerUp();
	}

	~ReviveNESApp() {
	}

	virtual void onSetup() override {
		mBatchRenderer = std::make_shared<BatchRenderer>();
		mViewportPanel = std::make_shared<ViewportPanel>();
	}

	virtual void onUpdate() override {
		for (int j = 0; j < 1000; j++) {
			size_t last = mCPU.mCyclesPassed;
			mCPU.clock();

			if (last != mCPU.mCyclesPassed) {
				printf("%04X  %02X ", mCPU.mCurrentInstruction.address, mCPU.mCurrentInstruction.opcode);
				if (mCPU.mCurrentInstruction.lo != -1) {
					printf("%02X ", mCPU.mCurrentInstruction.lo);
				} else {
					printf("   ");
				}
				if (mCPU.mCurrentInstruction.hi != -1) {
					printf("%02X ", mCPU.mCurrentInstruction.hi);
				}
				else {
					printf("   ");
				}
				printf("%s ", mCPU.mCurrentInstruction.Mnemonic.c_str());
				printf("CYC: %d", mCPU.mCyclesPassed);

				printf("\n");
				
				//C000  4C F5 C5  JMP $C5F5                       A:00 X:00 Y:00 P:24 SP:FD PPU:  0, 21 CYC:7
			}

			for (int i = 0; i < 3; i++) {
				mPPU.clock();
			}
		}
	}

	virtual void onRender() override {
		mViewportPanel->startFrame();
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		mProjectionMatrix = glm::ortho(0.0f, (float)mViewportPanel->width(), (float)mViewportPanel->height(), 0.0f);

		float cellWidth = (float)mViewportPanel->width() / 256.0f;
		float cellHeight = (float)mViewportPanel->height() / 240.0f;

		mBatchRenderer->begin(mProjectionMatrix, 0.1f, cellWidth / cellHeight);

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
	std::shared_ptr<Cartridge> mCartridge;

	Bus mCPUBus;
	Bus mPPUBus;
	VRAM mVRAM;
	WRAM mWRAM;
	CartridgeSlot mCartridgeSlot;
	RP2A03 mCPU;
	RP2C02 mPPU;
};

int main(int argc, char** argv) {
	ApplicationManager appManager;
	appManager.run(std::make_shared<ReviveNESApp>());
	return 0;
}