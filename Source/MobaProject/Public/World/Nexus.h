// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AttributeSet.h"
#include "GameFramework/Actor.h"
#include "Interfaces/CanTakeDamage.h"
#include "Interfaces/TeamColours.h"
#include "MobaProject/MobaProject.h"
#include "Nexus.generated.h"
DECLARE_LOG_CATEGORY_EXTERN(LogCHNexus, Log, All);

struct FGameplayTagContainer;
class ACHBase;
class UCharacterNamePlate;
class UGameplayEffect;
class UCHAttributeSet;
class UCHAbilitySystemComponent;

UCLASS(Abstract)
class MOBAPROJECT_API ANexus : public AActor, public IAbilitySystemInterface, public ITeamColours, public ICanTakeDamage
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ANexus();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE ETeam GetTeam() const { return Team; }

	virtual FActorHelper GetActorInfo() override;

	/**
	* Team this unit belongs too
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	ETeam Team;

	virtual ETeam GetTeam() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;


	////////// Ability System //////////

	/**
	 * AbilitySystemComponent for this Character
	 */
	UPROPERTY
	(VisibleAnywhere, BlueprintReadOnly, Category="Ability", meta=(AllowPrivateAccess=true))
	UCHAbilitySystemComponent* AbilitySystemComponent;

	/**
	 * The Attribute set for this Character
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Ability", meta=(AllowPrivateAccess=true))
	UCHAttributeSet* Attributes;

	/**
	 * This Characters default attributes for when its spawned or levels up
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Attribute")
	TSubclassOf<UGameplayEffect> DefaultAttributeEffect;

	/**
	 * Name plate component to show character health / mana level and entity name
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCharacterNamePlate* NamePlateComponent;

	/**
	 * Used to set the NamePlateComponent Name. Ex PlayerName, or for Ai 'Minion' or 'Turret'
	 */
	virtual FName GetEntityName();

	/**
	 * Get this Characters level to know what attribute values to spawn it with
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual int32 GetCharacterLevel() const;

private:
	/**
	 * Initialize this characters attributes and owner
	 */
	virtual void InitializeAttributes();
	virtual void InitializeOwningActor();
public:
	////////// Server only  //////////
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnDamaged(float Damage, const FHitResult HitInfo, const FGameplayTagContainer& DamageTags,
	                  ACHBase* InstigatorCharacter, AActor* DamageCauser);
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);
	virtual void HandleDamage(float Damage, const FHitResult HitInfo, const FGameplayTagContainer& DamageTags,
	                          ACHBase* InstigatorCharacter, AActor* DamageCauser);
	virtual void HandleHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags) override;
	////////// Server only //////////

	/**
	* Client only Attribute On_Rep
	*/
	virtual void OnRep_Attribute(const FGameplayAttribute& Attribute, const FGameplayAttributeData& OldValue,
	                             const FGameplayAttributeData& NewValue);

	friend UCHAttributeSet;

	FORCEINLINE UCHAttributeSet* GetAttributeSet() const { return Attributes; }
	FORCEINLINE UCharacterNamePlate* GetNamePlateComponent() const { return NamePlateComponent; }
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
protected:
	virtual void SetupNamePlateWidget();

	////////// Ability System //////////

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Turret")
	UStaticMeshComponent* NexusMesh;
};
