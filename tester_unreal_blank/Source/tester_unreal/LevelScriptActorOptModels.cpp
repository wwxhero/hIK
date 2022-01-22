// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelScriptActorOptModels.h"

ModelType ALevelScriptActorOptModels::GetModelType()
{
	FString strMDType = FWindowsPlatformMisc::GetEnvironmentVariable(TEXT("IKMODEL"));
	if (TEXT("ManMakeHuman163") == strMDType)
		return ManMakeHuman163;
	else if (TEXT("WomanMixamo65") == strMDType)
		return WomanMixamo65;
	else
		return ManMakeHuman163;
}