
#include "PlayerWeaponComponent.h"

#include "PlayerPawn.h"


UPlayerWeaponComponent::UPlayerWeaponComponent()
{
	SetIsReplicatedByDefault(true);
}

void UPlayerWeaponComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPlayerWeaponComponent, ActiveWeapon);
}

void UPlayerWeaponComponent::BeginPlay()
{
	//FString ObjectName = GetOwner()->GetName();
	//UE_LOG(LogTemp, Warning, TEXT("Object Owner: %s"), *ObjectName);
	//if (!UKismetSystemLibrary::IsServer(GetWorld())) return;
	SpawnStartWeapons();
}

void UPlayerWeaponComponent::SpawnStartWeapons_Implementation()
{
	UWorld* World = GetWorld();
	if (World) {
		for (size_t i = 0; i < StartingWeapons.Num(); i++)
		{
			//AWeaponObject* NewWeaponObject = (AWeaponObject*)GetWorld()->SpawnActor(AWeaponObject::StaticClass(), Params);

			FTransform SpawnTransform = FTransform(FRotator::ZeroRotator, FVector::ZeroVector, FVector::OneVector);
			AWeaponObject* NewWeaponObject = (AWeaponObject*)World->SpawnActorDeferred<AWeaponObject>(AWeaponObject::StaticClass(),
				SpawnTransform, GetOwner(), GetOwner()->GetInstigator());

			if (NewWeaponObject) {
				NewWeaponObject->WeaponData = StartingWeapons[i];

				UGameplayStatics::FinishSpawningActor(NewWeaponObject, NewWeaponObject->GetTransform());

				NewWeaponObject->GetRootComponent()->ToggleVisibility(true);
				EquippedWeapons.Add(NewWeaponObject);
				SetEquippedWeapon(0);
			}
		}
	}
}

void UPlayerWeaponComponent::SetEquippedWeapon_Implementation(uint8 Index)
{
	//TODO MEMORY LEAK!!
	if (!EquippedWeapons[Index]) return;

	ActiveWeapon = EquippedWeapons[Index];

	APlayerPawn* ParentPawn = Cast<APlayerPawn>(GetOwner());
	if (!ParentPawn) return;

	if (ParentPawn->GetWeaponModel()) {
		ParentPawn->GetWeaponModel()->SetStaticMesh(EquippedWeapons[Index]->WeaponData->WeaponModel);
		ParentPawn->GetWeaponModel()->SetRelativeTransform(EquippedWeapons[Index]->WeaponData->FP_Model_Transform);
	}

	if (ActiveWeapon->WeaponData->ShotAudio) {
		ParentPawn->GetWeaponAudioComponent()->SetSound(EquippedWeapons[Index]->WeaponData->ShotAudio);
	}
}

void UPlayerWeaponComponent::EquipPrimaryWeapon()
{
	SetEquippedWeapon(0);
}

void UPlayerWeaponComponent::EquipSecondaryWeapon()
{
	SetEquippedWeapon(1);
}

void UPlayerWeaponComponent::OnFire()
{
	FireWeapon_Request();

	APlayerPawn* ParentPawn = Cast<APlayerPawn>(GetOwner());
	if (!ParentPawn) return;
	ParentPawn->GetWeaponAudioComponent()->Play();

	OnFireEvent.Broadcast();
}

void UPlayerWeaponComponent::FireWeapon_Request_Implementation()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Purple, FString::Printf(TEXT("Fire Request!")));
	Server_FireWeapon();
}

void UPlayerWeaponComponent::Server_FireWeapon()
{
	if (!UKismetSystemLibrary::IsServer(GetWorld()) || !ActiveWeapon) return;

	if (ActiveWeapon->CurrentAmmo <= 0) return;

	ActiveWeapon->CurrentAmmo--;

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Purple, FString::Printf(TEXT("Current Ammo: %d"), ActiveWeapon->CurrentAmmo));

	APlayerPawn* ParentPawn = Cast<APlayerPawn>(GetOwner());
	FHitResult HitResult(ForceInit);

	if (ParentPawn != nullptr) {
		//TODO FIX STARTLOC w/ Actual Camera loc!
		FVector StartLoc = ParentPawn->GetActorLocation() + FVector(0, 0, 65.0f);

		//TODO Add Gun Range
		FVector ControlRot = ParentPawn->GetControlRotation().Vector() * 10000.0f;
		FVector EndLoc = StartLoc + ControlRot;

		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(GetOwner());

		DrawDebugLine(GetWorld(), StartLoc, EndLoc, FColor(255, 0, 0), false, 2.0f, 0, 3.f);

		GetWorld()->LineTraceSingleByChannel(HitResult, StartLoc, EndLoc, ECC_Visibility, CollisionParams);

		if (HitResult.GetActor()) {
			FString hitRes = HitResult.GetActor()->GetName();
			UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *hitRes);

			//TODO Add Gun Damage
			UGameplayStatics::ApplyDamage(HitResult.GetActor(), 10, ParentPawn->GetController(), GetOwner(), UDamageType::StaticClass());
		}
	}
}


void UPlayerWeaponComponent::ReloadWeapon()
{
	if (GetOwner()->HasAuthority()) {
		OnReloadEvent.Broadcast();
	}
}

