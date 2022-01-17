// Fill out your copyright notice in the Description page of Project Settings.


#include "SceneCapture2DActorVRSpec.h"

void ASceneCapture2DActorVRSpec::Initialize(USceneComponent* mirror)
{
	m_mirror = mirror;
	// AdjustMirrorOri();
}

void ASceneCapture2DActorVRSpec::AdjustMirrorOri(float ground_z)
{
	if (nullptr == m_mirror)
		return;
	const FTransform& tm_0_l2w = m_mirror->GetComponentToWorld();
	FVector x_0 = tm_0_l2w.TransformVector(FVector::XAxisVector);
	FVector y_0 = tm_0_l2w.TransformVector(FVector::YAxisVector);
	FVector z_0 = tm_0_l2w.TransformVector(FVector::ZAxisVector);
	FVector s_0 = tm_0_l2w.GetScale3D();
	FVector x = x_0;
	FVector y = -FVector::ZAxisVector*s_0.Y;
	FVector z = FVector::CrossProduct(y, x).GetUnsafeNormal()*s_0.Z; // make sure it is a left handed cross
	FVector t = tm_0_l2w.GetTranslation(); t.Z = s_0.Z * 50.0f + ground_z;
	FTransform tm_l2w(x, y, z, t);
	FTransform tm_dl = tm_l2w * tm_0_l2w.Inverse();	//tm_dl * tm_0_l2w = tm_l2w => tm_dl = tm_l2w*(tm_0_l2w)^-1
	const FTransform& tm_0_l2p = m_mirror->GetRelativeTransform();
	m_mirror->SetRelativeTransform(tm_dl*tm_0_l2p);
}