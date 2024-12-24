#include "ProjectileDefault.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/GameEngine.h"
#include "Perception/AISense_Damage.h"

// Sets default values
AProjectileDefault::AProjectileDefault()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetReplicates(true);

	BulletCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Sphere"));

	BulletCollisionSphere->SetSphereRadius(16.f);

	BulletCollisionSphere->OnComponentHit.AddDynamic(this, &AProjectileDefault::BulletCollisionSphereHit);
	BulletCollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AProjectileDefault::BulletCollisionSphereBeginOverlap);
	BulletCollisionSphere->OnComponentEndOverlap.AddDynamic(this, &AProjectileDefault::BulletCollisionSphereEndOverlap);

	BulletCollisionSphere->bReturnMaterialOnMove = true;//hit event return physMaterial

	BulletCollisionSphere->SetCanEverAffectNavigation(false);//collision not affect navigation (P keybord on editor)

	RootComponent = BulletCollisionSphere;

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bullet Projectile Mesh"));
	BulletMesh->SetupAttachment(RootComponent);
	BulletMesh->SetCanEverAffectNavigation(false);

	BulletFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Bullet FX"));
	BulletFX->SetupAttachment(RootComponent);

	//BulletSound = CreateDefaultSubobject<UAudioComponent>(TEXT("Bullet Audio"));
	//BulletSound->SetupAttachment(RootComponent);

	BulletProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Bullet ProjectileMovement"));
	BulletProjectileMovement->UpdatedComponent = RootComponent;
	BulletProjectileMovement->InitialSpeed = 1.f;
	BulletProjectileMovement->MaxSpeed = 0.f;

	BulletProjectileMovement->bRotationFollowsVelocity = true;
	BulletProjectileMovement->bShouldBounce = true;
}

// Called when the game starts or when spawned
void AProjectileDefault::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AProjectileDefault::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectileDefault::BulletCollisionSphereHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && Hit.PhysMaterial.IsValid())
	{
		EPhysicalSurface mySurfacetype = UGameplayStatics::GetSurfaceType(Hit);

		if (ProjectileSetting.HitDecals.Contains(mySurfacetype))
		{
			UMaterialInterface* myMaterial = ProjectileSetting.HitDecals[mySurfacetype];

			if (myMaterial && OtherComp)
			{
				SpawnHitDecal_Multicast(myMaterial, OtherComp, Hit);
			}
		}
		if (ProjectileSetting.HitFXs.Contains(mySurfacetype))
		{
			UParticleSystem* myParticle = ProjectileSetting.HitFXs[mySurfacetype];
			if (myParticle)
			{
				SpawnHitFX_Multicast(myParticle, Hit);
			}
		}

		if (ProjectileSetting.HitSound)
		{
			SpawnHitSound_Multicast(ProjectileSetting.HitSound, Hit);
		}
		UTypes::AddEffectBySurfaceType(Hit.GetActor(), Hit.BoneName, ProjectileSetting.Effect, mySurfacetype);
	}
	UGameplayStatics::ApplyPointDamage(OtherActor, ProjectileSetting.ProjectileDamage, Hit.TraceStart, Hit, GetInstigatorController(), this, NULL);
	UAISense_Damage::ReportDamageEvent(GetWorld(), Hit.GetActor(), GetInstigator(), ProjectileSetting.ProjectileDamage, Hit.Location, Hit.Location);

	ImpactProjectile();
}

void AProjectileDefault::BulletCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AProjectileDefault::BulletCollisionSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AProjectileDefault::InitProjectile(FProjectileInfo InitParam)
{
	ProjectileSetting = InitParam;
	BulletProjectileMovement->InitialSpeed = ProjectileSetting.ProjectileInitSpeed;
	BulletProjectileMovement->MaxSpeed = ProjectileSetting.ProjectileMaxSpeed;
	this->SetLifeSpan(ProjectileSetting.ProjectileLifeTime);
	if (ProjectileSetting.ProjectileStaticMesh)
	{
		InitVirtualMeshProjectile_Multicast(ProjectileSetting.ProjectileStaticMesh, ProjectileSetting.ProjectileStaticMeshOffset);
	}
	else
	{
		BulletMesh->DestroyComponent();
	}
	if (ProjectileSetting.ProjectileTrailFx)
	{
		InitVirtualTrailProjectile_Multicast(ProjectileSetting.ProjectileTrailFx, ProjectileSetting.ProjectileTrailFxOffset);
	}
	else
	{
		BulletFX->DestroyComponent();
	}
}

void AProjectileDefault::ImpactProjectile()
{
	this->Destroy();
}

void AProjectileDefault::SpawnHitDecal_Multicast_Implementation(UMaterialInterface* DecalMaterial, UPrimitiveComponent* OtherComp, FHitResult HitResult)
{
	UGameplayStatics::SpawnDecalAttached(DecalMaterial, FVector(20.0f), OtherComp, NAME_None, HitResult.ImpactPoint, HitResult.ImpactNormal.Rotation(), EAttachLocation::KeepWorldPosition, 10.0f);
}

void AProjectileDefault::SpawnHitFX_Multicast_Implementation(UParticleSystem* FxTemplate, FHitResult HitResult)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FxTemplate, FTransform(HitResult.ImpactNormal.Rotation(), HitResult.ImpactPoint, FVector(1.0f)));
}

void AProjectileDefault::SpawnHitSound_Multicast_Implementation(USoundBase* HitSound, FHitResult HitResult)
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, HitResult.ImpactPoint);
}

void AProjectileDefault::InitVirtualMeshProjectile_Multicast_Implementation(UStaticMesh* newMesh, FTransform MeshRelative)
{
	BulletMesh->SetStaticMesh(newMesh);
}

void AProjectileDefault::InitVirtualTrailProjectile_Multicast_Implementation(UParticleSystem* newTemplate, FTransform TemplateRelative)
{
	BulletFX->SetTemplate(newTemplate);
}

