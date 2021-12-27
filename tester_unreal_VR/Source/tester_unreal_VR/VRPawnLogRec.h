// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VRPawnBase.h"
#include "tester_unreal_VR.h"
#include "vrLog.h"
#include "VRPawnLogRec.generated.h"

UCLASS()
class TESTER_UNREAL_VR_API AVRPawnLogRec : public AVRPawnBase
{
	GENERATED_BODY()
public:
public:
	// Sets default values for this pawn's properties
	AVRPawnLogRec();
	~AVRPawnLogRec();
protected:
	virtual void OnVRMsg(TRACKER_ID tracker_id, VR_EVT evt) override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	HLOG m_hLog;
	uint32 c_frameID0;
};
