// Fill out your copyright notice in the Description page of Project Settings.
#include "Characters/CHGameplayAbility.h"

#include "Framework/AllMid/HUDAllMid.h"
#include "Framework/AllMid/PCAllMid.h"
#include "Kismet/DataTableFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Widgets/AllMid/WAbility.h"
#include "Widgets/AllMid/WPlayerHud.h"

UCHGameplayAbility::UCHGameplayAbility()
{
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateYes;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ClientOrServer;
	bServerRespectsRemoteAbilityCancellation = true;
	bRetriggerInstancedAbility = false;
	bReplicateInputDirectly = false;
}

void UCHGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo,
                                         const FGameplayAbilityActivationInfo ActivationInfo,
                                         const FGameplayEventData* TriggerEventData)
{
	UE_LOG(LogTemp, Display, TEXT("ActorInfo->OwnerActor %p"), ActorInfo->OwnerActor.Get());
	UE_LOG(LogTemp, Display, TEXT("ActorInfo->OwnerActor %s"), *ActorInfo->OwnerActor.Get()->GetClass()->GetName());
	UE_LOG(LogTemp, Display, TEXT("ActorInfo->AvatarActor %p"), ActorInfo->AvatarActor.Get());
	if (ActorInfo->OwnerActor.IsValid())
	{
		AActor* OwningActor = ActorInfo->OwnerActor.Get();
		CharacterRef = Cast<ACHBase>(OwningActor);
		UE_LOG(LogTemp, Display, TEXT("CharacterRef %p"), CharacterRef);

		PlayerControllerRef = CharacterRef->GetInstigatorController<APCAllMid>();
	}
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

UWPlayerHud* UCHGameplayAbility::GetPlayerHudWidget()
{
	if (IsValid(CharacterRef))
	{
		if (!IsValid(PlayerControllerRef))
		{
			return nullptr;
		}

		const AHUDAllMid* HUD = PlayerControllerRef->GetHUD<AHUDAllMid>();
		if (!IsValid(HUD))
		{
			return nullptr;
		}

		return HUD->GetPlayerHudWidget();
	}
	return nullptr;
}

void UCHGameplayAbility::StartCooldown()
{
	if (!IsValid(CharacterRef)) { return; }
	UWPlayerHud* PlayerHud = GetPlayerHudWidget();
	if (!IsValid(PlayerHud)) { return; }
	UWAbility* AbilityWidget = PlayerHud->BP_GetAbility(AbilityInputID);
	TEnumAsByte<EEvaluateCurveTableResult::Type> OutResult;
	float OutXY = 0;
	UDataTableFunctionLibrary::EvaluateCurveTableRow(AbilityDataTable, FName("Ability.Cooldown"), 1, OutResult, OutXY,
	                                                 FString());
	if (OutXY != 0)
	{
		AbilityWidget->StartCountdown(OutXY - 1, OutXY);
	}
}

FRotator UCHGameplayAbility::GetCharacterToMouseRotation()
{
	if (IsValid(CharacterRef) && !CharacterRef->HasAuthority())
	{
		if (IsValid(PlayerControllerRef))
		{
			FHitResult HitResult;
			const bool Hit = PlayerControllerRef->GetHitResultUnderCursorByChannel(TraceTypeQuery1, true, HitResult);
			if (Hit)
			{
				const FVector Start = CharacterRef->GetActorLocation();
				return UKismetMathLibrary::FindLookAtRotation(Start, HitResult.Location);
			}
		}
		//if pc not valid or nothing hit then return actor current rotation
		return CharacterRef->GetActorRotation();
	}
	return FRotator(0.f, 0.f, 0.f);
}

void UCHGameplayAbility::RotateCharacter(const float Z)
{
	if (IsValid(CharacterRef))
	{
		CharacterRef->SetActorRotation(FRotator(0.f, Z, 0.f));
	}
}
