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
	TimerExplode_OnServer(DeltaTime);
}

void AProjectileDefault_Grenade::TimerExplode_OnServer_Implementation(float DeltaTime)
{
	if (TimerEnabled)
	{
		if (TimerToExplose > TimeToExplose)
		{
			//Explode
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
		GrenadeHitFX_Multicast(ProjectileSetting.ExploseFX, GetActorLocation(), GetActorRotation());
		float a = GetActorLocation().X;
		float b = GetActorLocation().Y;
		float c = GetActorLocation().Z;
		TArray<float> arr = {a, b, c};
		FString TEXT = "GrenadeLocation";
		OnScreenMessage_Multicast(arr, 3, TEXT);
	}
	if (ProjectileSetting.ExploseSound)
	{
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

void AProjectileDefault_Grenade::OnScreenMessage_Multicast_Implementation(const TArray<float> &a, float len, const FString &ShowText)
{
	//For Testing Multicast
	//GEngine->AddOnScreenDebugMessage(1, 4.0f, FColor::Red, FString::Printf(TEXT("Array lenght: %i"), sizeof(a)));
	for (int i = 0; i < len; i++)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("%s: %f"), *ShowText, a[i]));
	}
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("//////")));
}

void AProjectileDefault_Grenade::GrenadeHitFX_Multicast_Implementation(UParticleSystem* FxTemplate, FVector Location, FRotator Rotation)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FxTemplate, Location, Rotation, FVector(1.0f));
}

void AProjectileDefault_Grenade::GrenadeHitSound_Multicast_Implementation(USoundBase* HitSound, FVector Location)
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, Location);
}
