// Fill out your copyright notice in the Description page of Project Settings.


#include "World/MinionSpawner.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AMinionSpawner::AMinionSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called every frame
void AMinionSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMinionSpawner::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer<AMinionSpawner>(SpawnTimerHandle, this, &AMinionSpawner::Spawn,
	                                                       SpawnFrequency, true,
	                                                       SpawnFrequency);
}

void AMinionSpawner::Spawn()
{
	if (!HasAuthority()) { return; }
	if (!IsValid(GetWorld())) { return; }

	for (int32 i = 0; i < UnitsPerWave; i ++)
	{
		FRotator Rotator = GetActorRotation();
		Rotator.Pitch = 0;
		const FTransform Transform{Rotator, this->GetActorLocation()};
		ACHBase* SpawnedActor = GetWorld()->SpawnActorDeferred<ACHBase>(
			MinionClass,
			Transform,
			this,
			nullptr,
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		);
		SpawnedActor->Team = this->Team;
		UGameplayStatics::FinishSpawningActor(SpawnedActor, Transform);
	}
}
