// Fill out your copyright notice in the Description page of Project Settings.


#include "World/Shop.h"

#include "Blueprint/UserWidget.h"
#include "Components/SphereComponent.h"

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

void AShop::ShowShop()
{
	ShopWidget->SetVisibility(ESlateVisibility::Visible);
}

// Called when the game starts or when spawned
void AShop::BeginPlay()
{
	Super::BeginPlay();

	UUserWidget* CreatedWidget = CreateWidget(GetWorld(), ShopWidgetClass,TEXT("ShopWidget"));
	if (CreatedWidget)
	{
		ShopWidget = Cast<UWItemsShop>(CreatedWidget);
		ShopWidget->SetVisibility(ESlateVisibility::Hidden);
		ShopWidget->AddToViewport(11);
		ShopWidget->InitShop(ItemsInShop);
	}
}
