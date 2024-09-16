// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Interface/TPS_IGameActor.h"
#include "../StateEffects/TPS_StateEffect.h"
#include "TPS_EnvironmentStructure.generated.h"

UCLASS()
class TPS_API ATPS_EnvironmentStructure : public AActor, public ITPS_IGameActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATPS_EnvironmentStructure();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	EPhysicalSurface GetSurfuceType() override;	
	
	TArray<UTPS_StateEffect*> GetAllCurrentEffects() override;
	void RemoveEffect(UTPS_StateEffect* RemoveEffect)override;
	void AddEffect(UTPS_StateEffect* newEffect)override;

	//Effect
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	TArray<UTPS_StateEffect*> Effects;
};
