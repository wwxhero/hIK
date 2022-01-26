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