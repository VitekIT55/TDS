// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileDefault.h"
#include "ProjectileDefault_Grenade.generated.h"

/**
 * 
 */
UCLASS()
class TPS_API AProjectileDefault_Grenade : public AProjectileDefault
{
	GENERATED_BODY()
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//UFUNCTION(Server, Unreliable)
	void TimerExplode(float DeltaTime);

	virtual void BulletCollisionSphereHit(class UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
	
	virtual void ImpactProjectile() override;

	void Explode();

	//UPROPERTY(Replicated)
	float TimerToExplose = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade")
	bool TimerEnabled = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade")
	float TimeToExplose = 5.0f;
	UFUNCTION(NetMulticast, Unreliable)
	void GrenadeHitFX_Multicast(UParticleSystem* FxTemplate, FVector Location, FRotator Rotation);
	UFUNCTION(NetMulticast, Unreliable)
	void GrenadeHitSound_Multicast(USoundBase* HitSound, FVector Location);
	UFUNCTION(NetMulticast, Unreliable)
	void OnScreenMessage_Multicast(const TArray<float> &a, float len, const FString &ShowText);
};
