// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CHBase.h"
#include "CHPlayable.generated.h"
DECLARE_LOG_CATEGORY_EXTERN(LogCHPlayable, Log, All);

class UBaseItem;
class UCHGameplayAbility;
/**
 * 
 */
UCLASS(Abstract, NotBlueprintable)
class MOBAPROJECT_API ACHPlayable : public ACHBase
{
	GENERATED_BODY()
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

private:
	/**
	 * Update the cursor decal
	 */
	void UpdateCursorDecal();

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
	virtual void BeginPlay() override;
	////////// Ability System //////////

	/**
	 *
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Gold")
	TSubclassOf<UGameplayEffect> PassiveGold;

	/**
	 *
	 * Character Abilities
	 */
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
public:
	UFUNCTION(BlueprintPure, Category="Attack")
	FORCEINLINE int32 GetNextAttackIndex() const { return NextPrimaryAttackIndex; };

	UFUNCTION(BlueprintPure, Category="Attack")
	FORCEINLINE UAnimMontage* GetPrimaryAttackMontage(const int32 AtIndex) const { return PrimaryAttackMontages[AtIndex]; };

	virtual void OnRep_Attribute(const FGameplayAttribute& Attribute, const FGameplayAttributeData& OldValue, const FGameplayAttributeData& NewValue) override;

	virtual void HandleHealthChanged(float DeltaValue, const FGameplayTagContainer& EventTags) override;
	UFUNCTION(Client, Unreliable)
	virtual void Client_HandleHealthChanged(float DeltaValue, const FGameplayTagContainer& EventTags);
	////////// Ability System //////////

	////////// Items //////////
	UFUNCTION(BlueprintCallable, Category="Items")
	void BuyItem(TSubclassOf<UBaseItem> Item);

	UFUNCTION(Server, Reliable)
	void Server_BuyItem(TSubclassOf<UBaseItem> Item);
};
