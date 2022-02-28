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

ETeam ACHNonPlayable::GetTeam()
{
	return Team;
}

void ACHNonPlayable::BeginPlay()
{
	Super::BeginPlay();
	Execute_SetColour(this, GetColour(GetClientTeam(GetWorld())));
}
