// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VRPawnBase.h"
#include "tester_unreal_VR.h"
#include "vrLog.h"
#include "VRPawnLogPB.generated.h"

UCLASS()
class TESTER_UNREAL_VR_API AVRPawnLogPB : public AVRPawnBase
{
	GENERATED_BODY()
	
public:
	// Sets default values for this pawn's properties
	AVRPawnLogPB();
	~AVRPawnLogPB();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	HLOG m_hLog;
	uint32 m_frameid0;
};
