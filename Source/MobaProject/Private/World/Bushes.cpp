// Fill out your copyright notice in the Description page of Project Settings.


#include "World/Bushes.h"

// Sets default values
ABushes::ABushes()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABushes::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABushes::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

