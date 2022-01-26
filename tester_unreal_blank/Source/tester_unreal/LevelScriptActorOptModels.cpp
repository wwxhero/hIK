// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelScriptActorOptModels.h"

ModelType ALevelScriptActorOptModels::GetModelType()
{
	FString strMDType = FWindowsPlatformMisc::GetEnvironmentVariable(TEXT("IKMODEL"));
	if (TEXT("ManMakeHuman163") == strMDType)
		return ManMakeHuman163;
	else if (TEXT("WomanMixamo65") == strMDType)
		return WomanMixamo65;
	else if (TEXT("ChildJasper67") == strMDType)
		return ChildJasper67;
	else
		return ManMakeHuman163;
}

void ALevelScriptActorOptModels::GetModelTM0(ModelType type, FTransform& tm_driver, FTransform& tm_drivee)
{
	switch(type)
	{
		case ManMakeHuman163:
			tm_drivee = FTransform(FQuat::Identity, FVector(-91, -70, 0));
			tm_driver = FTransform(FQuat::Identity, FVector(-91, -50, -106));
			break;
		case WomanMixamo65:
			tm_drivee = FTransform(FQuat::Identity, FVector(-91, -70, 0));
			tm_driver = FTransform(FQuat::Identity, FVector(-91, -50, -106));
			break;
		case ChildJasper67:
			tm_drivee = FTransform(FQuat::Identity, FVector(-91, -70, 0));
			tm_driver = FTransform(FQuat::Identity, FVector(160, -70, -90));
			break;
		default:
			tm_drivee = FTransform(FQuat::Identity, FVector(-91, -70, 0));
			tm_driver = FTransform(FQuat::Identity, FVector(-91, -50, -106));
			break;
	}


}

void ALevelScriptActorOptModels::SpawnPairs(const TArray<UClass*>& templates_drivee, const TArray<UClass*>& templates_driver)
{
	ModelType type = GetModelType();
	FTransform tm_driver, tm_drivee;
	GetModelTM0(type, tm_driver, tm_drivee);
	UWorld* world = GetWorld();
	FActorSpawnParameters Params;
	Params.bNoFail = true;
	Params.bDeferConstruction = true; // We defer construction so that we set ParentComponentActor prior to component registration so they appear selected
	Params.bAllowDuringConstructionScript = true;
	Params.OverrideLevel = this->GetLevel();
	Params.Name = "drivee";
	if (!HasAllFlags(RF_Transactional))
	{
		Params.ObjectFlags &= ~RF_Transactional;
	}
	AActor* actor_drivee = world->SpawnActor(templates_drivee[type], &tm_drivee, Params);
	check(NULL != actor_drivee);
	
	Params.Name = "driver";
	AActor* actor_driver = world->SpawnActor(templates_driver[type], &tm_driver, Params);
	check(NULL != actor_driver);
}