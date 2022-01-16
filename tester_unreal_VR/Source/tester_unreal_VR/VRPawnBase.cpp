// Fill out your copyright notice in the Description page of Project Settings.

#include "VRPawnBase.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "vrLog.h"
#include "WidgetsVRFSA.h"
#include "Kismet/KismetSystemLibrary.h"

VRCaliFSA::VRCaliFSA()
	: m_actState(MapLoaded)
	, m_refPawn(NULL)
{
	m_stateStr = {
		TEXT("The map is loaded: \n\tPut a motion controller on the floor and press 'trigger' to caliberate the floor evaluation \n\tPress 'grip' to quit!"),
		TEXT("VR is aligned with the floor elevaution: \n\tPress 'trigger' to connect IK, \n\tPress 'grip' to quit!"),
		TEXT("VR and IK are connected, \n\tPress 'trigger' to disconnect IK for a start over,\n\tPress 'grip' to quit!"),
		TEXT("VR IK tester is quitted!")
	};
}

void VRCaliFSA::Initialize(AVRPawnBase& refPawn)
{
	m_transitions = {
		new Transition(refPawn, MapLoaded, Landed, LCTRL, TRIGGER_RELEASE, &AVRPawnBase::actFloorCali_L),
		new Transition(refPawn, MapLoaded, Landed, RCTRL, TRIGGER_RELEASE, &AVRPawnBase::actFloorCali_R),
		new Transition(refPawn, Landed, IKConnected, LCTRL, TRIGGER_RELEASE, &AVRPawnBase::actConnectIK),
		new Transition(refPawn, Landed, IKConnected, RCTRL, TRIGGER_RELEASE, &AVRPawnBase::actConnectIK),
		new Transition(refPawn, IKConnected, Landed, LCTRL, TRIGGER_RELEASE, &AVRPawnBase::actDisConnectIK),
		new Transition(refPawn, IKConnected, Landed, RCTRL, TRIGGER_RELEASE, &AVRPawnBase::actDisConnectIK),
		new Transition(refPawn, Any, Exit, LCTRL, GRIP_RELEASE, &AVRPawnBase::actQuit),
		new Transition(refPawn, Any, Exit, RCTRL, GRIP_RELEASE, &AVRPawnBase::actQuit),
	};
	m_refPawn = &refPawn;
	m_refPawn->m_winVRFSA->UpdateInstruction(m_stateStr[m_actState]);
}

VRCaliFSA::~VRCaliFSA()
{
	for (auto transi_i : m_transitions)
		delete transi_i;
	m_refPawn = NULL;
}

void VRCaliFSA::UpdateState(TRACKER_ID tracker_id, VR_EVT vrEvt)
{
	State actState_p = m_actState;
	for (auto transi_i : m_transitions)
	{
		if (transi_i->Execute(m_actState, tracker_id, vrEvt, actState_p))
		{
			m_actState = actState_p;
			m_refPawn->m_winVRFSA->UpdateInstruction(m_stateStr[m_actState]);
			break;
		}
	}
}

// Sets default values
AVRPawnBase::AVRPawnBase()
	: m_vrOrg(NULL)
	, m_actorIKDrivee(NULL)
	, m_winVRFSA(nullptr)
	, m_verifying(RH)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AVRPawnBase::BeginPlay()
{
	Super::BeginPlay();
	TSharedRef<SWindowVRFSA> SlateWin = SNew(SWindowVRFSA)
     								.AutoCenter(EAutoCenter::None)
     								.Title(FText::FromString(TEXT("VR Message Window")))
     								.IsInitiallyMaximized(false)
     								.ScreenPosition(FVector2D(0, 0))
     								.ClientSize(FVector2D(500, 800))
     								.CreateTitleBar(true)
     								.SizingRule(ESizingRule::UserSized)
     								.SupportsMaximize(false)
     								.SupportsMinimize(true)
     								.HasCloseButton(false);

    FSlateApplication & SlateApp = FSlateApplication::Get();
 
	SlateApp.AddWindow(SlateWin, true);

	m_winVRFSA = SlateWin;
	m_caliFSA.Initialize(*this);
}

void AVRPawnBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (m_winVRFSA)
	{
		m_winVRFSA->RequestDestroyWindow();
		m_winVRFSA = nullptr;
	}
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void AVRPawnBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
#if defined _DEBUG
	if (-1 < m_verifying && m_verifying < N_SPECTRAKS)
	{
		auto tracker_i = m_trackers[m_verifying];
		DBG_VisTransform(tracker_i->GetComponentTransform(), 10, 1);
	}

#endif
	if (VRCaliFSA::IKConnected == m_caliFSA.actState())
		m_animIKDrivee->VRIK_PushUpdateTargets();
}

bool AVRPawnBase::InitVRPawn(USceneComponent* org
							, USceneComponent* hmd
							, USceneComponent* ctrller_l
							, USceneComponent* ctrller_r
							, const TArray<USceneComponent*>& trackers
							, ASkeletalMeshActor* avatarIKDrivee)
{
	m_vrOrg = org;
	m_trackers = trackers;
	m_trackers.SetNumUninitialized(N_TRACKERS);
	m_trackers[HMD] = hmd;
	m_trackers[RCTRL] = ctrller_r;
	m_trackers[LCTRL] = ctrller_l;
	m_actorIKDrivee = avatarIKDrivee;
	USkeletalMeshComponent* skm_comp = avatarIKDrivee->GetSkeletalMeshComponent();
	UAnimInstance* anim_inst = skm_comp->GetAnimInstance();
	m_animIKDrivee = Cast<UAnimInstance_HIKDrivee, UAnimInstance>(anim_inst);
	return NULL != m_animIKDrivee;
}

bool AVRPawnBase::actFloorCali_L()
{
	FVector p_v = m_trackers[LCTRL]->GetComponentLocation();
	FloorCali_x(p_v);
	return true;
}

bool AVRPawnBase::actFloorCali_R()
{
	FVector p_v = m_trackers[RCTRL]->GetComponentLocation();
	FloorCali_x(p_v);
	return true;
}

bool AVRPawnBase::actDisConnectIK()
{
	m_animIKDrivee->VRIK_Disconnect();
	return true;
}

bool AVRPawnBase::actQuit()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), NULL, EQuitPreference::Quit, true);
	return true;
}

void AVRPawnBase::FloorCali_x(const FVector& p_v)
{
	FTransform tm_v(m_actorIKDrivee->GetActorLocation() - p_v);
	const FTransform& tm_r2v = m_vrOrg->GetComponentTransform();
	const FTransform& tm_c2v = GetActorTransform();
	FTransform tm_v2c = tm_c2v.Inverse();
	FTransform tm_r2c_prime = tm_r2v * tm_v * tm_v2c;
	m_vrOrg->SetRelativeTransform(tm_r2c_prime);
}

bool AVRPawnBase::actConnectIK()
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
	taggers.SetNum(N_SPECTRAKS);
	for (int i_tracker = 0; i_tracker < N_SPECTRAKS; i_tracker ++)
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
	for (int i_tagger = 0; i_tagger < N_SPECTRAKS; i_tagger ++)
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
	for (int i_tagger = 0; i_tagger < N_SPECTRAKS; i_tagger ++)
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
	for (int i_predicate = RH; i_predicate < N_SPECTRAKS && one_on_one; i_predicate ++)
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
		m_animIKDrivee->VRIK_Connect(m_trackers);
	}
	else
	{
		UE_LOG(TESTER_UNREAL_VR, Display, TEXT("AVRPawnBase::Proc_SortTrackers: FAILED"));
	}

	return one_on_one;	
}

void AVRPawnBase::OnVRMsg(TRACKER_ID tracker_id, VR_EVT vrEvt)
{
	// static FString tracker_id_str[] = {
	// 	TEXT("RH"),
	// 	TEXT("LH"),
	// 	TEXT("RF"),
	// 	TEXT("LF"),
	// 	TEXT("PW"),
	// 	TEXT("HMD"),
	// 	TEXT("RCTRL"),
	// 	TEXT("LCTRL")
	// };

	// static FString vrEvt_str[] = {
	// 	TEXT("NONE"),
	// 	TEXT("GRIP_PRESS"),
	// 	TEXT("GRIP_RELEASE"),
	// 	TEXT("TRIGGER_PRESS"),
	// 	TEXT("TRIGGER_RELEASE")
	// };

	// FTransform tm_l2p = m_trackers[tracker_id]->GetRelativeTransform();
	// FString tm_l2p_str = tm_l2p.ToString();

	// UE_LOG(TESTER_UNREAL_VR, Display, TEXT("(%s, %s): %s"), *tracker_id_str[tracker_id], *vrEvt_str[vrEvt], *tm_l2p_str);
}

void AVRPawnBase::Proc_VRMsg(TRACKER_ID tracker_id, VR_EVT vrEvt)
{
	m_caliFSA.UpdateState(tracker_id, vrEvt);
	OnVRMsg(tracker_id, vrEvt);
}

void AVRPawnBase::VerifyTracker()
{
	int verifying = m_verifying;
	verifying ++;
	m_verifying = (TRACKER_ID)(verifying % N_SPECTRAKS);
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
