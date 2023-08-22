#pragma once

#include <cstdint>
#include <unordered_map>
#include <memory>
#include <string>
#include <optional>

namespace NESEmu {

	struct BusRange {
		uint16_t Start;
		uint16_t End;
		uint16_t Mask;

		BusRange(uint16_t start,uint16_t end,uint16_t mask)
			:	Start(start),
				End(end),
				Mask(mask)
		{}
	};

	typedef class Bus;

	class BusDevice {
	public:
		BusDevice(const std::string& name) : mName(name) {}
		virtual ~BusDevice() = default;

		virtual void writeLine(const std::string& busName, const std::string& lineName, bool value) {}
		virtual void write(const std::string& busName, uint16_t address, uint8_t value) = 0;
		virtual uint8_t read(const std::string& busName, uint16_t address) = 0;

		std::optional<BusRange> getRange(const std::string& busName, uint16_t address);

		const std::string& getName() const { return mName; }

		void connectToBus(Bus* pBus);
		Bus* getBus(const std::string& busName);

	protected:
		template<typename... T>
		void addRange(const std::string& busName, T&&... args) {
			mRanges[busName].emplace_back(std::forward<T>(args)...);
		}

	protected:
		std::unordered_map<std::string, Bus*> mBusses;
		std::unordered_map<std::string, std::vector<BusRange>> mRanges;

		std::string mName;
	};


	class Bus {
	public:
		Bus(const std::string& name);
		~Bus();

		void writeLine(const std::string& lineName, bool value);
		void write(uint16_t address, uint8_t value);
		uint8_t read(uint16_t address);

		void connectDevice(BusDevice* device);

		const std::string& getName() const { return mName; }

	private:
		std::string mName;
		std::unordered_map<std::string,BusDevice*> mDevices;
	};


}