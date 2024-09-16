// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../FuncLibrary/Types.h"
#include "../Weapon/WeaponDefault.h"
#include "../Character/TPSInventoryComponent.h"
#include "../Character/TPSCharacterHealthComponent.h"
#include "../Interface/TPS_IGameActor.h"
#include "../StateEffects/TPS_StateEffect.h"

#include "TPSCharacter.generated.h"

UCLASS(Blueprintable)
class ATPSCharacter : public ACharacter, public ITPS_IGameActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	//Inputs
	void InputAxisY(float Value);
	void InputAxisX(float Value);

	void InputAttackPressed();
	void InputAttackReleased();

	void InputWalkPressed();
	void InputWalkReleased();

	void InputSprintPressed();
	void InputSprintReleased();

	void InputAimPressed();
	void InputAimReleased();

	//Inventory Inputs
	void TrySwitchNextWeapon();
	void TrySwitchPreviosWeapon();
	//Ability Inputs
	void TryAbilityEnabled();

	template<int32 Id>
	void TKeyPressed()
	{
		TrySwitchWeaponToIndexByKeyInput(Id);
	}
	//Inputs End

	//Input Flags
	float AxisX = 0.0f;
	float AxisY = 0.0f;

	//bool SprintEnabled = false;
	//bool WalkEnabled = false;
	//bool AimEnabled = false;

	//bool bIsAlive = true;

	//EMovementState MovementState = EMovementState::Run_State;

	AWeaponDefault* CurrentWeapon = nullptr;

	UDecalComponent* CurrentCursor = nullptr;

	TArray<UTPS_StateEffect*> Effects;

	int32 CurrentIndexWeapon = 0;

	UFUNCTION()
	void CharDead();
	void EnableRagdoll();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

public:
	ATPSCharacter();

	FTimerHandle TimerHandle_RagDollTimer;

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UTPSInventoryComponent* InventoryComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = "true"))
	class UTPSCharacterHealthComponent* CharHealthComponent;

	//Cursor material on decal
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	UMaterialInterface* CursorMaterial = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	FVector CursorSize = FVector(20.0f, 40.0f, 40.0f);
	//Default move rule and state character
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FChatacterSpeed MovementSpeedInfo;
	//FChatacterSpeed

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
	TArray<UAnimMontage*> DeadsAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	TSubclassOf<UTPS_StateEffect> AbilityEffect;

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

public:

	//Movement
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	EMovementState MovementState = EMovementState::Run_State;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FChatacterSpeed MovementInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool SprintEnabled = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool WalkEnabled = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool AimEnabled = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float Stamina = 1.00f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool SprintBlock = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool SprintAllow = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float CharacterSpeed = 0;
	
	// Tick Func
	UFUNCTION()
	void MovementTick(float DeltaTime);
	// Tick Func End

	//Func
	void CharacterUpdate();
	void ChangeMovementState();

	void AttackCharEvent(bool bIsFiring);

	UFUNCTION()
	void InitWeapon(FName IdWeaponName, FAdditionalWeaponInfo WeaponAdditionalInfo, int32 NewCurrentIndexWeapon);
	void TryReloadWeapon();
	UFUNCTION()
	void WeaponFireStart(UAnimMontage* Anim);
	UFUNCTION()
	void WeaponReloadStart(UAnimMontage* Anim);
	UFUNCTION()
	void WeaponReloadEnd(bool bIsSuccess, int32 AmmoSafe);
	//
	bool TrySwitchWeaponToIndexByKeyInput(int32 ToIndex);
	void DropCurrentWeapon();
	UFUNCTION(BlueprintNativeEvent)
	void WeaponReloadStart_BP(UAnimMontage* Anim);
	UFUNCTION(BlueprintNativeEvent)
	void WeaponReloadEnd_BP(bool bIsSuccess);
	UFUNCTION(BlueprintNativeEvent)
	void WeaponFireStart_BP(UAnimMontage* Anim);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	AWeaponDefault* GetCurrentWeapon();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UDecalComponent* GetCursorToWorld();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	EMovementState GetMovementState();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<UTPS_StateEffect*> GetCurrentEffectsOnChar();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetCurrentWeaponIndex();
	//Func End

	//Interface
	EPhysicalSurface GetSurfuceType() override;
	TArray<UTPS_StateEffect*> GetAllCurrentEffects() override;
	void RemoveEffect(UTPS_StateEffect* RemoveEffect)override;
	void AddEffect(UTPS_StateEffect* newEffect)override;
	//End Interface
};

