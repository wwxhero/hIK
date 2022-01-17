// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/SceneCapture2D.h"
#include "SceneCapture2DActorVRSpec.generated.h"

/**
 * 
 */
UCLASS()
class TESTER_UNREAL_VR_API ASceneCapture2DActorVRSpec : public ASceneCapture2D
{
	GENERATED_BODY()
public:
	ASceneCapture2DActorVRSpec()
		: m_mirror(nullptr)
	{
	}
	UFUNCTION(BlueprintCallable, Category = Settings)
	void Initialize(USceneComponent* mirror);

	void AdjustMirrorOri(float ground_z);
private:
	USceneComponent* m_mirror;
};
