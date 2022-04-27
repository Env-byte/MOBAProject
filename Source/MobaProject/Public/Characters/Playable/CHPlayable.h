// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CHBase.h"
#include "CHPlayable.generated.h"
DECLARE_LOG_CATEGORY_EXTERN(LogCHPlayable, Log, All);

class UCHGameplayAbility;
/**
 * 
 */
UCLASS(Abstract, NotBlueprintable)
class MOBAPROJECT_API ACHPlayable : public ACHBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category=Camera)
	float CameraPitch = -60.f;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	/**
	 * Use Internally to handle team changing, such as changing camera angle etc
	 */
	void SetTeam(ETeam ThisTeam);

	virtual FName GetEntityName() override;

public:
	ACHPlayable();
	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns CursorToWorld subobject **/
	FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }

	virtual void OnRep_PlayerState() override;


private:
	/**
	 * Update the cursor decal
	 */
	void UpdateCursorDecal() const;

	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* CursorToWorld;


protected:
	void PlayerSpawned();
	virtual void BeginPlay() override;
	////////// Ability System //////////
	///
	/**
	 *
	 * Character Abilities
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Health")
	float HealthTickFrequency = 1.f;
	float LastHealthTick = 0.f;

	/**
	 * Apply Passive Health and Mana Regen
	 */
	void ApplyPassiveRegen();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Attack")
	TSubclassOf<UCHGameplayAbility> PrimaryAttack;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Attack")
	TSubclassOf<UCHGameplayAbility> Ability1;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Attack")
	TSubclassOf<UCHGameplayAbility> Ability2;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Attack")
	TSubclassOf<UCHGameplayAbility> Ability3;

	/**
	 * Characters basic attacks
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attack")
	TArray<UAnimMontage*> PrimaryAttackMontages;

	/**
	 * The next primary attack animation to play
	 */
	UPROPERTY(BlueprintReadOnly)
	int32 NextPrimaryAttackIndex;

	/**
	 * Is Attacking
	 */
	UPROPERTY(BlueprintReadWrite, Category="Attack")
	bool bIsAttacking;

	bool bAbilitiesInitialized;

	/**
	 * Give the characters abilities to themselves on game start
	 */
	virtual void GiveAbilities();

	/**
	 * Called only on the server
	 */
	virtual void PossessedBy(AController* NewController) override;

	/**
	 * Tell server to CastPrimaryAttack
	*/
	UFUNCTION(Server, Reliable)
	void Server_CastPrimaryAttack(AActor* Target);
public:
	UFUNCTION(BlueprintPure, Category="Attack")
	FORCEINLINE int32 GetNextAttackIndex() const { return NextPrimaryAttackIndex; };

	UFUNCTION(BlueprintPure, Category="Attack")
	FORCEINLINE UAnimMontage* GetPrimaryAttackMontage(const int32 AtIndex) const
	{
		return PrimaryAttackMontages[AtIndex];
	}


	void CastPrimaryAttack(AActor* Target);

	/**
		 * The target of the primary attack
		 */
	UPROPERTY(BlueprintReadWrite, Replicated=OnRep_PrimaryAttackTarget)
	AActor* PrimaryAttackTarget;

	UFUNCTION()
	void OnRep_PrimaryAttackTarget();

	/**
	 * For simplicity just going to minus armour from damage
	 */
	UFUNCTION(BlueprintCallable)
	float GetPrimaryAttackDamage();

	/**
	 *
	 */
	UFUNCTION(BlueprintCallable)
	float GetPrimaryAttackSpeedCooldown();

	/**
	 *
	 */
	UFUNCTION(BlueprintPure)
	FORCEINLINE AActor* GetPrimaryAttackTarget() const { return PrimaryAttackTarget; };

	virtual void OnRep_Attribute(const FGameplayAttribute& Attribute, const FGameplayAttributeData& OldValue,
	                             const FGameplayAttributeData& NewValue) override;

	virtual void HandleHealthChanged(float DeltaValue, const FGameplayTagContainer& EventTags,ACHPlayable* SourcePlayer) override;

	virtual int32 GetCharacterLevel() const override;
	////////// Ability System //////////
};
