// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SiegeCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class SIEGE_API USiegeCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_UCLASS_BODY()
	
	class FSavedMove_Siege : public FSavedMove_Character
	{
	public:

		typedef FSavedMove_Character Super;

		virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* Character, float MaxDelta) const override;
		virtual void Clear() override;
		virtual uint8 GetCompressedFlags() const override;
		virtual void SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character& ClientData) override;
		virtual void PrepMoveFor(class ACharacter* Character) override;

		//Walk Speed Update
		uint8 bSavedRequestMaxWalkSpeedChange : 1;

		//Dodge
		FVector SavedMoveDirection;
		uint8 bSavedWantsToDodge : 1;
	};

	class FNetworkPredictionData_Client_Siege : public FNetworkPredictionData_Client_Character
	{
	public:
		FNetworkPredictionData_Client_Siege(const UCharacterMovementComponent& ClientMovement);

		typedef FNetworkPredictionData_Client_Character Super;

		virtual FSavedMovePtr AllocateNewMove() override;
	};
};
