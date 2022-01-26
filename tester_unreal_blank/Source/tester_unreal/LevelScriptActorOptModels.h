// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "LevelScriptActorOptModels.generated.h"

UENUM()
enum ModelType
{
	ManMakeHuman163 = 0	UMETA(DisplayName = "MakeHuman163"),
	WomanMixamo65		UMETA(DisplayName = "Mixamo65"),
	ChildJasper67 			UMETA(DisplayName = "Jasper67"),
	Child
};

/**
 * 
 */
UCLASS()
class TESTER_UNREAL_API ALevelScriptActorOptModels : public ALevelScriptActor
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = Settings, meta=(PinShownByDefault))
	ModelType GetModelType();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Settings, meta=(PinShownByDefault))
	void GetModelTM0(ModelType type, FTransform& tm_driver, FTransform& tm_drivee);
};
