// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_StateEffect.h"
#include "../Character/TPSHealthComponent.h"
#include "../Character/TPSCharacterHealthComponent.h"
#include "../Interface/TPS_IGameActor.h"
#include "Kismet/GameplayStatics.h"

bool UTPS_StateEffect::InitObject(AActor* Actor)
{

	myActor = Actor;

	ITPS_IGameActor* myInterface = Cast<ITPS_IGameActor>(myActor);
	if (myInterface)
	{
		myInterface->AddEffect(this);
	}

	return true;
}

void UTPS_StateEffect::DestroyObject()
{
	ITPS_IGameActor* myInterface = Cast<ITPS_IGameActor>(myActor);
	if (myInterface)
	{
		myInterface->RemoveEffect(this);
	}

	myActor = nullptr;
	if (this && this->IsValidLowLevel())
	{
		this->ConditionalBeginDestroy();
	}	
}

bool UTPS_StateEffect_ExecuteOnce::InitObject(AActor* Actor)
{
	Super::InitObject(Actor);
	ExecuteOnce();
	return true;
}

void UTPS_StateEffect_ExecuteOnce::DestroyObject()
{
	Super::DestroyObject();
}

void UTPS_StateEffect_ExecuteOnce::ExecuteOnce()
{
	if (myActor)
	{
		UTPSHealthComponent* myHealthComp = Cast<UTPSHealthComponent>(myActor->GetComponentByClass(UTPSHealthComponent::StaticClass()));
		if (myHealthComp)
		{
			myHealthComp->ChangeHealthValue(Power);
		}
	}	

	DestroyObject();
}

bool UTPS_StateEffect_ExecuteTimer::InitObject(AActor* Actor)
{
	Super::InitObject(Actor);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_EffectTimer, this, &UTPS_StateEffect_ExecuteTimer::DestroyObject, Timer, false);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_ExecuteTimer, this, &UTPS_StateEffect_ExecuteTimer::Execute, RateTime, true);
	
	if (ParticleEffect)
	{
		FName NameBoneToAttached;
		FVector Loc = FVector(0);
		
		ParticleEmitter = UGameplayStatics::SpawnEmitterAttached(ParticleEffect,myActor->GetRootComponent(), NameBoneToAttached, Loc, FRotator::ZeroRotator,EAttachLocation::SnapToTarget,false);
	}
	UTPSCharacterHealthComponent* myCharHealthComp = Cast<UTPSCharacterHealthComponent>(myActor->GetComponentByClass(UTPSCharacterHealthComponent::StaticClass()));
	if (myCharHealthComp)
	{
		myCharHealthComp->HealthChangeBlock = LocalHealthChangeBlock;
	}
	
	return true;
}

void UTPS_StateEffect_ExecuteTimer::DestroyObject()
{
	UTPSCharacterHealthComponent* myCharHealthComp = Cast<UTPSCharacterHealthComponent>(myActor->GetComponentByClass(UTPSCharacterHealthComponent::StaticClass()));
	if (myCharHealthComp)
	{
		myCharHealthComp->HealthChangeBlock = 0;
	}
	ParticleEmitter->DestroyComponent();
	ParticleEmitter = nullptr;
	Super::DestroyObject();
}

void UTPS_StateEffect_ExecuteTimer::Execute()
{
	if (myActor)
	{	
		//UGameplayStatics::ApplyDamage(myActor,Power,nullptr,nullptr,nullptr);	
		UTPSHealthComponent* myHealthComp = Cast<UTPSHealthComponent>(myActor->GetComponentByClass(UTPSHealthComponent::StaticClass()));
		if (myHealthComp)
		{
			myHealthComp->ChangeHealthValue(Power);
		}
	}
}
