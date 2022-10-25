#include "SpawnArea.h"
#include "AI_Controller_Base.h"
#include "GameFramework/Character.h"

void ASpawnArea::BeginPlay()
{
	Super::BeginPlay();

	//TODO Set true via Players! SpawnArea.cpp
	//SetAreaStatus(true);
}

void ASpawnArea::SpawnEnemy(TSubclassOf<class ACharacter> EnemyToSpawn)
{
	if (!SpawnPoints.IsEmpty()) {
		uint8 RandomSpawnPoint = FMath::RandRange(0, (SpawnPoints.Num() - 1));
		ASpawnObject* SelectedSpawner = SpawnPoints[RandomSpawnPoint];

		if (SelectedSpawner) {
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.Name = MakeUniqueObjectName(this, EnemyToSpawn, FName("Zombie_AI"));
			GetWorld()->SpawnActor<ACharacter>(EnemyToSpawn, SelectedSpawner->GetSpawnTransform().GetLocation(), FRotator::ZeroRotator);
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("SPAWNAREA %s: No SpawnObjects Found!"), *this->GetName());
	}
}

bool ASpawnArea::GetAreaStatus()
{
	return bAreaActive;
}

bool ASpawnArea::SetAreaStatus(bool bActive)
{
	bAreaActive = bActive;
	return bAreaActive;
}

bool ASpawnArea::ContainsPlayers()
{
	return bContainsPlayers;
}

bool ASpawnArea::SetContainsPlayers(bool bActive)
{
	bContainsPlayers = bActive;
	return bContainsPlayers;
}

