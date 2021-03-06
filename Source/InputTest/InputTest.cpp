#include "Input.h"
#include "ConfigManager.h"
#include <cassert>
#include <iostream>
#include <map>

std::string GetNameForState(ButtonsState bState) {

	switch (bState)
	{
	case ButtonsState::JustPressed:
		return "JustPressed";
	case ButtonsState::Pressed:
		return "Pressed";
	case ButtonsState::JustReleased:
		return "JustReleased";
	case ButtonsState::Released:
		return "Released";
	default:
		return "Undefined";
		break;
	}
}

int main()
{
	std::multimap<int, ButtonKey_t> actions;
	ConfigManager* cm1 = ConfigManager::Create("InputConfig.INI");
	LogCategory category = cm1->GetCategory("Input");
	std::multimap<const std::string, const std::string> inputCategory = category.GetParams();

	for (auto i : inputCategory)
	{
		int a = atoi(i.first.c_str());
		int b = atoi(i.second.c_str());
		actions.insert(std::pair<int, int>(a,b));
	}

	if (actions.empty())
	{
		return 1;
	}

	InputManager input(actions);

	// ~~~~~~~~~~~~ Only for test mode! ~~~~~~~~~~~~~~
	// We don't want to see "released" or "pressed" all the time - so let's print to the console only when state changes
	ButtonsState previousStateMoveUp = ButtonsState::Released;
	ButtonsState previousStateMoveDown = ButtonsState::Released;
	ButtonsState previousStateMoveLeft = ButtonsState::Released;
	ButtonsState previousStateMoveRight = ButtonsState::Released;
	ButtonsState previousStateExit = ButtonsState::Released;
	ButtonsState previousStateShoot = ButtonsState::Released;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	ButtonsState stateMoveUp;
	ButtonsState stateMoveDown;
	ButtonsState stateMoveLeft;
	ButtonsState stateMoveRight;
	ButtonsState stateExit;
	ButtonsState stateShoot;

	while (true)
	{
		input.Update();

		if (input.GetState(static_cast<int>(GameActions::MoveUp), stateMoveUp) && previousStateMoveUp != stateMoveUp)
		{
			previousStateMoveUp = stateMoveUp;
		}
		if (input.GetState(static_cast<int>(GameActions::MoveDown), stateMoveDown) && previousStateMoveDown != stateMoveDown)
		{
			previousStateMoveDown = stateMoveDown;
		}
		if (input.GetState(static_cast<int>(GameActions::MoveLeft), stateMoveLeft) && previousStateMoveLeft != stateMoveLeft)
		{
			previousStateMoveLeft = stateMoveLeft;
		}
		if (input.GetState(static_cast<int>(GameActions::MoveRight), stateMoveRight) && previousStateMoveRight != stateMoveRight)
		{
			previousStateMoveRight = stateMoveRight;
		}
		if (input.GetState(static_cast<int>(GameActions::Exit), stateExit) && previousStateExit != stateExit)
		{
			previousStateExit = stateExit;
		}
		if (input.GetState(static_cast<int>(GameActions::Shoot), stateShoot) && previousStateShoot != stateShoot)
		{
			previousStateShoot = stateShoot;
		}

	}
	return 0;
}

