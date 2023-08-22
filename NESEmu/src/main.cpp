#include "Core/RP2A03.h"
#include "Core/WRAM.h"
#include "Core/VRAM.h"

int main(int argc, char** argv) {
	NESEmu::Bus cpuBus("CPU");
	NESEmu::Bus ppuBus("PPU");

	NESEmu::VRAM vram;
	NESEmu::WRAM wram;
	NESEmu::RP2A03 cpu;

	cpuBus.connectDevice(&cpu);
	cpuBus.connectDevice(&wram);
	ppuBus.connectDevice(&vram);

	cpu.reset();

	while (true) {
		cpu.clock();
	}

	return 0;
}