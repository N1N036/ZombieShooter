#include "SpawnArea.h"

void ASpawnArea::SpawnEnemies(TSubclassOf<class ACharacter> EnemyToSpawn)
{
	if (SpawnPoints.Num() > 0) {
		for (uint8 i = 0; i < SpawnPoints.Num(); i++)
		{
			FTransform SpawnTransform = SpawnPoints[i]->GetSpawnTransform();
			SpawnPoints[i]->SpawnEnemy(EnemyToSpawn, &SpawnTransform);
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("SPAWNAREA %s: No SpawnObjects Found!"), *this->GetName());
	}
}