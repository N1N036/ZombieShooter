#include "GameInstance_Main.h"

void UGameInstance_Main::SetGameState(EZombieGameState newState)
{
	CurrentGameState = newState;
	HandleGameState();
}

EZombieGameState UGameInstance_Main::GetGameState() const
{
	return CurrentGameState;
}

void UGameInstance_Main::HandleGameState()
{
	switch (CurrentGameState)
	{
	case EZombieGameState::DEFAULT:
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "DEFAULT STATE");
		break;
	case EZombieGameState::LOBBY:
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "LOBBY STATE");
		break;
	case EZombieGameState::PRE_GAME:
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "PRE GAME STATE");
		break;
	case EZombieGameState::INGAME:
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "INGAME STATE");
		break;
	case EZombieGameState::POSTGAME:
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "POSTGAME STATE");
		break;
	default:
		break;
	}
}