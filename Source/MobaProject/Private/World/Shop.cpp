// Fill out your copyright notice in the Description page of Project Settings.


#include "World/Shop.h"

#include "Blueprint/UserWidget.h"
#include "Components/SphereComponent.h"
#include "Framework/AllMid/PCAllMid.h"

// Sets default values
AShop::AShop()
{
	ShopMesh = CreateDefaultSubobject<UStaticMeshComponent>("ShopMesh");
	SetRootComponent(ShopMesh);

	ShopRadius = CreateDefaultSubobject<USphereComponent>("ShopRadius");
	ShopRadius->SetupAttachment(ShopMesh);
	ShopRadius->AreaClass = nullptr;
	ShopRadius->SetSphereRadius(300.5);
}

// Called when the game starts or when spawned
void AShop::BeginPlay()
{
	Super::BeginPlay();
}
