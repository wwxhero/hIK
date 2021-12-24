// Fill out your copyright notice in the Description page of Project Settings.

#include "VRPawnBase.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "vrLog.h"

// Sets default values
AVRPawnBase::AVRPawnBase()
	: m_vrOrg(NULL)
	, m_ctrllerL(NULL)
	, m_ctrllerR(NULL)
	, m_actorIKDrivee(NULL)
	, m_verifying(RH)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AVRPawnBase::BeginPlay()
{
	Super::BeginPlay();
	int test = 0;
	int test_p = ik_test(test);
	check(test_p == test + 1);
}

// Called every frame
void AVRPawnBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
#if defined _DEBUG
	if (-1 < m_verifying && m_verifying < N_TRACKERS)
	{
		auto tracker_i = m_trackers[m_verifying];
		DBG_VisTransform(tracker_i->GetComponentTransform(), 10, 1);
	}	
#endif
	m_animIKDrivee->VRIK_Update();
}

bool AVRPawnBase::InitVRPawn(USceneComponent* org
							, USceneComponent* hmd
							, AActor* ctrller_l
							, AActor* ctrller_r
							, const TArray<USceneComponent*>& trackers
							, ASkeletalMeshActor* avatarIKDrivee)
{
	m_vrOrg = org;
	m_ctrllerL = ctrller_l;
	m_ctrllerR = ctrller_r;
	m_trackers = trackers;
	m_trackers.Add(hmd);
	m_actorIKDrivee = avatarIKDrivee;
	USkeletalMeshComponent* skm_comp = avatarIKDrivee->GetSkeletalMeshComponent();
	UAnimInstance* anim_inst = skm_comp->GetAnimInstance();
	m_animIKDrivee = Cast<UAnimInstance_HIKDrivee, UAnimInstance>(anim_inst);
	return NULL != m_animIKDrivee;
}

void AVRPawnBase::Proc_FloorCali(const FVector& p_v)
{
	FTransform tm_v(m_actorIKDrivee->GetActorLocation() - p_v);
	const FTransform& tm_r2v = m_vrOrg->GetComponentTransform();
	const FTransform& tm_c2v = GetActorTransform();
	FTransform tm_v2c = tm_c2v.Inverse();
	FTransform tm_r2c_prime = tm_r2v * tm_v * tm_v2c;
	m_vrOrg->SetRelativeTransform(tm_r2c_prime);
}

bool AVRPawnBase::Proc_SortTrackers()
{
	auto hmd = m_trackers[HMD];
	check(NULL != hmd);
	const FTransform& hmd2world = hmd->GetComponentTransform();
	struct Tagger
	{
		USceneComponent* t;
		int id_y;
		int id_z;
		float y;
		float z;
		TRACKER_ID tid;
	};
	
	FVector axis_y = hmd2world.TransformVector(FVector(0, 1, 0));
	FVector axis_z = hmd2world.TransformVector(FVector(0, 0, 1));
	FVector org = hmd2world.GetLocation();

	TArray<Tagger> taggers;
	taggers.SetNum(N_TRACKERS);
	for (int i_tracker = 0; i_tracker < N_TRACKERS; i_tracker ++)
	{
		auto& tagger_i = taggers[i_tracker];
		tagger_i.t = m_trackers[i_tracker];
		tagger_i.id_y = -1;
		tagger_i.id_z = -1;
		FVector org_t_i = tagger_i.t->GetComponentTransform().GetLocation();
		FVector hmd2t_i = org_t_i - org;
		tagger_i.y = FVector::DotProduct(hmd2t_i, axis_y);
		tagger_i.z = FVector::DotProduct(hmd2t_i, axis_z);
		tagger_i.tid = Unknown;
	}

	auto & taggers_y = taggers;
	struct FCompare_y
	{
		FORCEINLINE bool operator()(const Tagger& A, const Tagger& B) const
		{
			return A.y < B.y;
		}
	};
	taggers_y.Sort(FCompare_y());
	for (int i_tagger = 0; i_tagger < N_TRACKERS; i_tagger ++)
		taggers_y[i_tagger].id_y = i_tagger;

	auto & taggers_z = taggers;
	struct FCompare_z
	{
		FORCEINLINE bool operator()(const Tagger& A, const Tagger& B) const
		{
			return A.z < B.z;
		}
	};
	taggers_z.Sort(FCompare_z());
	for (int i_tagger = 0; i_tagger < N_TRACKERS; i_tagger ++)
		taggers_z[i_tagger].id_z = i_tagger;

	struct Identifier
	{
		int id_y;
		int id_z;
	};

	TArray<Identifier> rh = {{4, 3}, {4, 4}};
	TArray<Identifier> lh = {{0, 3}, {0, 4}};
	TArray<Identifier> rf = {{2, 0}, {2, 1}, {3, 0}, {3, 1}};
	TArray<Identifier> lf = {{1, 0}, {1, 1}, {2, 0}, {2, 1}};
	TArray<Identifier> pw = {{1, 2}, {2, 2}, {3, 2}};
	auto Is_a = [](const TArray<Identifier>& identifier, const Tagger& tagger) -> bool
		{
			bool positive = false;
			for (auto id : identifier)
			{
				positive = (id.id_y == tagger.id_y
							&& id.id_z == tagger.id_z);
				if (positive)
					break;
			}
			return positive;
		};

	TArray<Identifier>* identifiers[] = {&rh, &lh, &rf, &lf, &pw};
	// one_on_one: (forall identifiers exists_a_tagger: tagger is not tagged and tagger is identfied)
	bool one_on_one = true;
	for (int i_predicate = RH; i_predicate < N_TRACKERS && one_on_one; i_predicate ++)
	{
		TArray<Identifier>* identfier_i = identifiers[i_predicate];
		for (auto& tagger_i : taggers)
		{
			one_on_one = (Unknown == tagger_i.tid
						&& Is_a(*identfier_i, tagger_i));
			if (one_on_one)
			{
				tagger_i.tid = (TRACKER_ID)i_predicate;
				break;
			}
		}
	}

	if (one_on_one)
	{
		for (auto tagger_i : taggers)
			m_trackers[tagger_i.tid] = tagger_i.t;

		UE_LOG(TESTER_UNREAL_VR, Display, TEXT("AVRPawnBase::Proc_SortTrackers: SUCCESSFUL"));
		m_verifying = RH;
	}
	else
	{
		UE_LOG(TESTER_UNREAL_VR, Display, TEXT("AVRPawnBase::Proc_SortTrackers: FAILED"));
	}

	return one_on_one;	
}

void AVRPawnBase::Proc_ConnectIKTaget()
{
	m_animIKDrivee->VRIK_Connect(m_trackers);
}

void AVRPawnBase::VerifyTracker()
{
	int verifying = m_verifying;
	verifying ++;
	m_verifying = (TRACKER_ID)(verifying % N_TRACKERS);
}

// Called to bind functionality to input
void AVRPawnBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AVRPawnBase::DBG_VisTransform(const FTransform& tm_l2w, float axis_len, float thickness) const
{
	const FVector4 ori(0, 0, 0, 1);

	const FVector4 axis_ends[] = {
		FVector4(axis_len,	0,	0,	1),
		FVector4(0,	axis_len,	0,	1),
		FVector4(0,	0,	axis_len,	1),
	};
	const FColor axis_color[] = {
		FColor::Red,
		FColor::Green,
		FColor::Blue
	};
	FVector4 ori_w = tm_l2w.TransformFVector4(ori);
	const int n_axis = sizeof(axis_ends) / sizeof(FVector4);
	for (int i_end = 0; i_end < n_axis; i_end ++)
	{
		FVector4 end_w = tm_l2w.TransformFVector4(axis_ends[i_end]);
		UWorld *world = NULL;
		DrawDebugLine(GetWorld()
					, ori_w
		 			, end_w
		 			, axis_color[i_end]
		 			, false // bPersistentLines =
		 			, -1.f  // LifeTime =
		 			, 0
		 			, thickness);
	}
}
