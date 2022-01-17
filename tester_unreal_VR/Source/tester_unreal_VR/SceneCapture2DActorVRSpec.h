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
		, m_installed(false)
	{
	}
	UFUNCTION(BlueprintCallable, Category = Settings)
	void Initialize(USceneComponent* mirror);

	void AdjustMirrorOri(float ground_z);

	void CameraInstall(const FVector& from, const FVector& to, const FVector& up);
	void CameraCloser();
	void CameraFarther();
private:
	USceneComponent* m_mirror;
	bool m_installed;
	FVector m_dir;
};
