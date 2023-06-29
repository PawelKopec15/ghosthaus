#pragma once
#include <memory>

#include "../utility/GlobalVars.hpp"
#include "../utility/ReturnControlValue.hpp"

class Master
{
public:
	explicit Master(std::shared_ptr<GlobalVars> gvPtr) : globalVarsPtr(gvPtr) {}
	virtual ~Master() = default;

	virtual ReturnControlValue handleInput(int input) { return ReturnControlValue::CONTINUE; };
	virtual ReturnControlValue handleTick() { return ReturnControlValue::CONTINUE; }
	virtual void terminalSizeChanged(){/* Empty for compiling reasons */};
	virtual void terminalSizeInvalid(){/* Empty for compiling reasons */};

protected:
	std::shared_ptr<GlobalVars> globalVarsPtr;
	uint32_t tickCounter = 0;

	// unnecessary: unsigned integer overflow
	// void increment_tickCounter()
	// {
	// 	++tickCounter;
	// 	tickCounter &= (UINT32_MAX / 2);
	// }
};