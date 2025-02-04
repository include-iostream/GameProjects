// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"

#include "SiegeAttributeSet.h"
#include "SiegeCharacter.h"
#include "AS_BaseWeapon.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "Kismet/KismetSystemLibrary.h"



ABaseWeapon::ABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	SetReplicates(true);

	WeaponInfo.WeaponName = FString("Base Weapon");
	WeaponInfo.WeaponType = EWeaponType::WEAPONTYPE_None;

	WeaponAttributeSet = CreateDefaultSubobject<UAS_BaseWeapon>(TEXT("WeaponAttributeSet"));
}

void ABaseWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();


	if (StaticMesh)
	{
		StaticMesh->SetIsReplicated(true);
		StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	if (SkeletalMesh)
	{
		SkeletalMesh->SetIsReplicated(true);
		SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ABaseWeapon::Fire()
{
	
}

void ABaseWeapon::OnEquip()
{
	OwningSiegeAbilitySystemComponent->SpawnedAttributes.AddUnique(WeaponAttributeSet);
	OwningSiegeAbilitySystemComponent->ForceReplication();

	// grant fire ability
	if (HasAuthority())
	{
		FireAbilitySpecHandle = OwningSiegeAbilitySystemComponent->GiveAbility(OwningSiegeCharacter->FireAbility);
	}

	if (StaticMesh)
	{
		StaticMesh->SetVisibility(true);
	}
	if (SkeletalMesh)
	{
		SkeletalMesh->SetVisibility(true);
	}
}

void ABaseWeapon::OnDequip()
{
	OwningSiegeAbilitySystemComponent->SpawnedAttributes.Remove(WeaponAttributeSet);
	OwningSiegeAbilitySystemComponent->ForceReplication();

	// clear fire ability
	if (HasAuthority())
	{
		OwningSiegeAbilitySystemComponent->ClearAbility(FireAbilitySpecHandle);
	}

	if (StaticMesh)
	{
		StaticMesh->SetVisibility(false);
	}
	if (SkeletalMesh)
	{
		SkeletalMesh->SetVisibility(false);
	}
}

void ABaseWeapon::OnAddedToInventory(ASiegeCharacter* OwningCharacter)
{
	OwningSiegeCharacter = OwningCharacter;
	OwningSiegeAbilitySystemComponent = OwningSiegeCharacter->GetAbilitySystemComponent();
	SetInstigator(OwningCharacter);
	if (HasAuthority())
	{
		SetOwner(OwningCharacter);
	}

	if (StaticMesh)
	{
		StaticMesh->SetVisibility(false);
	}
	if (SkeletalMesh)
	{
		SkeletalMesh->SetVisibility(false);
	}

	if (OwningSiegeCharacter)
	{
		FAttachmentTransformRules WeaponAttachTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, false);
		
		if (GetOwner()->GetLocalRole() == ROLE_Authority)
		{
			AttachToComponent(OwningSiegeCharacter->GetMesh(), WeaponAttachTransformRules, TEXT("mixamorig_RightHand"));
		}
		else if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
		{
			AttachToComponent(OwningSiegeCharacter->GetSelfMesh(), WeaponAttachTransformRules, TEXT("mixamorig_RightHand"));
		}
	}	UKismetSystemLibrary::PrintString(this, "OnEnterInventory");
}

void ABaseWeapon::OnRemovedFromInventory()
{
	OwningSiegeCharacter = nullptr;
	SetInstigator(nullptr);
	if (HasAuthority())
	{
		SetOwner(nullptr);
	}
}
