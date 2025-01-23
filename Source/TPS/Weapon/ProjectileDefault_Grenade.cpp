// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileDefault_Grenade.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

int32 DebugExplodeShow = 0;
FAutoConsoleVariableRef CVARExplodeShow{
	TEXT("TPS.DebugExplode"),
	DebugExplodeShow,
	TEXT("Draw Debug for Explode"),
	ECVF_Cheat
};

void AProjectileDefault_Grenade::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectileDefault_Grenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TimerExplode(DeltaTime);

}

void AProjectileDefault_Grenade::TimerExplode(float DeltaTime)
{
	if (TimerEnabled)
	{
		if (TimerToExplose > TimeToExplose)
		{
			//Explose
			Explode();
			
		}
		else
		{
			TimerToExplose += DeltaTime;
		}
	}
}

void AProjectileDefault_Grenade::BulletCollisionSphereHit(class UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::BulletCollisionSphereHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
}

void AProjectileDefault_Grenade::ImpactProjectile()
{
	//Init Grenade
	TimerEnabled = true;
}

void AProjectileDefault_Grenade::Explode()
{
	FHitResult Hit;
	if (DebugExplodeShow)
	{
		DrawDebugSphere(GetWorld(), GetActorLocation(), ProjectileSetting.ProjectileMinRadiusDamage, 12, FColor::Green, false, 12.0f);
		DrawDebugSphere(GetWorld(), GetActorLocation(), ProjectileSetting.ProjectileMaxRadiusDamage, 12, FColor::Red, false, 12.0f);
	}
	TimerEnabled = false;
	if (ProjectileSetting.ExploseFX)
	{
		//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ProjectileSetting.ExploseFX, GetActorLocation(), GetActorRotation(), FVector(1.0f));
		GrenadeHitFX_Multicast(ProjectileSetting.ExploseFX, GetActorLocation(), GetActorRotation());
		auto a = GetActorLocation().X;
		auto b = GetActorLocation().Y;
		OnScreenMessage_Multicast(a, b);
	}
	if (ProjectileSetting.ExploseSound)
	{
		//UGameplayStatics::PlaySoundAtLocation(GetWorld(), ProjectileSetting.ExploseSound, GetActorLocation());
		GrenadeHitSound_Multicast(ProjectileSetting.ExploseSound, GetActorLocation());
	}
	TArray<AActor*> IgnoredActor;
	UGameplayStatics::ApplyRadialDamageWithFalloff(GetWorld(),
		ProjectileSetting.ExplodeMaxDamage,
		ProjectileSetting.ExplodeMaxDamage * 0.2f,
		GetActorLocation(),
		ProjectileSetting.ProjectileMinRadiusDamage,
		ProjectileSetting.ProjectileMaxRadiusDamage,
		5,
		UDamageType::StaticClass(), IgnoredActor, this, nullptr);

	this->Destroy();
}

void AProjectileDefault_Grenade::OnScreenMessage_Multicast_Implementation(float a, float b)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("GrenadeLocation: %f %f"), a, b));
}

void AProjectileDefault_Grenade::GrenadeHitFX_Multicast_Implementation(UParticleSystem* FxTemplate, FVector Location, FRotator Rotation)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FxTemplate, Location, Rotation, FVector(1.0f));
}

void AProjectileDefault_Grenade::GrenadeHitSound_Multicast_Implementation(USoundBase* HitSound, FVector Location)
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, Location);
}
