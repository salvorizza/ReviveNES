#include "Bus.h"

namespace NESEmu {

	

	Bus::Bus(const std::string& name)
		: mName(name)
	{
	}

	Bus::~Bus()
	{
	}

	void Bus::writeLine(const std::string& lineName, bool value)
	{
		for (auto& [name, device] : mDevices) {
			device->writeLine(mName, lineName, value);
		}
	}

	void Bus::write(uint16_t address, uint8_t value)
	{
		for (auto& [name,device] : mDevices) {
			auto range = device->getRange(mName, address);
			if (range) {
				device->write(mName,address & range->Mask, value);
			}
		}
	}

	uint8_t Bus::read(uint16_t address)
	{
		uint8_t result = 0;
		for (auto& [name, device] : mDevices) {
			auto range = device->getRange(mName,address);
			if (range) {
				result = device->read(mName,address & range->Mask);
				break;
			}
		}
		return result;
	}

	void Bus::connectDevice(BusDevice* device) {
		mDevices[device->getName()] = device;
		device->connectToBus(this);
	}


	std::optional<BusRange> BusDevice::getRange(const std::string& busName, uint16_t address)
	{
		auto it = std::find_if(mRanges[busName].begin(), mRanges[busName].end(), [address](const BusRange& t) -> bool {
			return address >= t.Start  && address < t.End;
		});

		if (it == mRanges[busName].end())
			return {};

		return *it;
	}

	void BusDevice::connectToBus(Bus* pBus)
	{
		mBusses[pBus->getName()] = pBus;
	}

	Bus* BusDevice::getBus(const std::string& busName)
	{
		return mBusses[busName];
	}



}