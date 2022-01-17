// Fill out your copyright notice in the Description page of Project Settings.


#include "SceneCapture2DActorVRSpec.h"

void ASceneCapture2DActorVRSpec::AdjustMirrorOri(USceneComponent* mirror)
{
	const FTransform& tm_0_l2w = mirror->GetComponentToWorld();
	FVector x_0 = tm_0_l2w.TransformVector(FVector::XAxisVector);
	FVector y_0 = tm_0_l2w.TransformVector(FVector::YAxisVector);
	FVector z_0 = tm_0_l2w.TransformVector(FVector::ZAxisVector);
	FVector s_0 = tm_0_l2w.GetScale3D();
	FVector x = x_0;
	FVector y = -FVector::ZAxisVector*s_0.Y;
	FVector z = FVector::CrossProduct(y, x).GetUnsafeNormal()*s_0.Z; // make sure it is a left handed cross
	FTransform tm_l2w(x, y, z, tm_0_l2w.GetTranslation());
	FTransform tm_dl = tm_l2w * tm_0_l2w.Inverse();	//tm_dl * tm_0_l2w = tm_l2w => tm_dl = tm_l2w*(tm_0_l2w)^-1
	const FTransform& tm_0_l2p = mirror->GetRelativeTransform();
	mirror->SetRelativeTransform(tm_dl*tm_0_l2p);
}