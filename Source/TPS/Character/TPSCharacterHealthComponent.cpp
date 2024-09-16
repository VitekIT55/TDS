// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSCharacterHealthComponent.h"



void UTPSCharacterHealthComponent::ChangeHealthValue(float ChangeValue)
{
	if (UTPSHealthComponent::CharIsDead != 1)
	{
		float CurrentDamage = ChangeValue * CoefDamage;

		if (Shield > 0.0f && ChangeValue < 0.0f)
		{
			ChangeShieldValue(ChangeValue);

			if (Shield < 0.0f)
			{
				//FX
				//UE_LOG(LogTemp, Warning, TEXT("UTPSCharacterHealthComponent::ChangeHealthValue - Sheild < 0"));
			}
		}
		else
		{
			if (HealthChangeBlock == 0)
			{
				Super::ChangeHealthValue(ChangeValue);
			}
		}
	}
}

float UTPSCharacterHealthComponent::GetCurrentShield()
{
	return Shield;
}

void UTPSCharacterHealthComponent::ChangeShieldValue(float ChangeValue)
{
	Shield += ChangeValue;

	if (Shield > 100.0f)
	{
		Shield = 100.0f;
	}
	else
	{
		if(Shield < 0.0f)
			Shield = 0.0f;
	}

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_CollDownShieldTimer, this, &UTPSCharacterHealthComponent::CoolDownShieldEnd, CoolDownShieldRecoverTime, false);

		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_ShieldRecoveryRateTimer);
	}	

	OnShieldChange.Broadcast(Shield, ChangeValue);
}

void UTPSCharacterHealthComponent::CoolDownShieldEnd()
{
	if (GetWorld() && UTPSHealthComponent::CharIsDead != 1)
	{		
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_ShieldRecoveryRateTimer, this, &UTPSCharacterHealthComponent::RecoveryShield, ShieldRecoverRate, true);
	}	
}

void UTPSCharacterHealthComponent::RecoveryShield()
{
	float tmp = Shield;
	tmp = tmp + ShieldRecoverValue;
	if (tmp > 100.0f)
	{
		Shield = 100.0f;
		if (GetWorld())
		{
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle_ShieldRecoveryRateTimer);
		}
	}
	else
	{
		Shield = tmp;
	}

	OnShieldChange.Broadcast(Shield, ShieldRecoverValue);
}
