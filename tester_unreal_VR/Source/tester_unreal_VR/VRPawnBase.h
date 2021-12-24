// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "tester_unreal_VR.h"
#include "Animation/SkeletalMeshActor.h"
#include "AnimInstance_HIKDrivee.h"
#include "VRPawnBase.generated.h"

UCLASS()
class TESTER_UNREAL_VR_API AVRPawnBase : public APawn
{
	GENERATED_BODY()
	
protected:
	UFUNCTION(BlueprintCallable, Category = Settings)
	bool InitVRPawn(USceneComponent* org
					, USceneComponent* hmd
					, AActor* ctrller_l
					, AActor* ctrller_r
					, const TArray<USceneComponent*>& trackers
					, ASkeletalMeshActor* avatarIKDrivee);

	UFUNCTION(BlueprintCallable, Category = Settings)
	void Proc_FloorCali(const FVector& p_v);

	UFUNCTION(BlueprintCallable, Category = Settings)
	bool Proc_SortTrackers();

	UFUNCTION(BlueprintCallable, Category = Settings)
	void Proc_ConnectIKTaget();

	UFUNCTION(BlueprintCallable, Category = Settings)
	void VerifyTracker();

public:
	// Sets default values for this pawn's properties
	AVRPawnBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void DBG_VisTransform(const FTransform& tm_l2w, float axis_len, float thickness) const;

protected:
	USceneComponent* m_vrOrg;
	AActor* m_ctrllerL;
	AActor* m_ctrllerR;
	TArray<USceneComponent*> m_trackers;
	ASkeletalMeshActor* m_actorIKDrivee;
	UAnimInstance_HIKDrivee* m_animIKDrivee;
private:
	TRACKER_ID m_verifying;

};
