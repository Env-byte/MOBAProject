#include "Characters/AI/CHNonPlayable.h"

#include "Components/CharacterNamePlate.h"
#include "Widgets/Components/WNamePlate.h"

void ACHNonPlayable::SetupNamePlateWidget()
{
	if (NamePlateComponent)
	{
		UWNamePlate* NamePlate = NamePlateComponent->GetNamePlateWidget();
		if (NamePlate)
		{
			NamePlate->HideMana();
			NamePlate->HideLevel();
		}
	}
	Super::SetupNamePlateWidget();
}

FName ACHNonPlayable::GetEntityName()
{
	if (ActorName.GetStringLength() > 0)
	{
		return ActorName;
	}
	return Super::GetEntityName();
}

ETeam ACHNonPlayable::GetTeam()
{
	return Team;
}

void ACHNonPlayable::BeginPlay()
{
	Super::BeginPlay();
	Execute_SetColour(this, GetColour(GetClientTeam(GetWorld())));
}
