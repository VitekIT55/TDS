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

	void TimerExplose(float DeltaTime);

	virtual void BulletCollisionSphereHit(class UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
	
	virtual void ImpactProjectile() override;

	void Explose();

	bool TimerEnabled = false;
	float TimerToExplose = 0.0f;
	float TimeToExplose = 5.0f;
};
