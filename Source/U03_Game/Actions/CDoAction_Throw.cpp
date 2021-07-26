
#include "CDoAction_Throw.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "CAim.h"
#include "CThrow.h"

void ACDoAction_Throw::BeginPlay()
{
	Super::BeginPlay();

	//Aim = new UCAim();	
	// UObject를 생성 할 때, new가 안됨 => delete를 못쓰게 하기 위해 => 언리얼 가비지 컬렉터 에서 처리

	Aim = NewObject<UCAim>();
	// 가비지 컬렉터 첫 주기(61초)가 오면 위 NewObject<> 만들어진 Aim을 멋대로 지워버림
	//	이후 Aim을 쓰거나 다음 주기가 오면 게임이 터짐
	//	=> 직렬화가 되면, 가비지 컬렉터가 함부로 지우지 않음 => UPROPERTY()
	//	=> 지울려면


	Aim->BeginPlay(OwnerCharacter);

	Action = CHelpers::GetComponent<UCActionComponent>(OwnerCharacter);
	Action->OnActionTypeChanged.AddDynamic(this, &ACDoAction_Throw::AbortByTypeChanged);
}

void ACDoAction_Throw::DoAction()
{
	if (Aim->IsAvaliable())
		CheckFalse(Aim->IsInZoom());

	CheckFalse(State->IsIdleMode());
	State->SetActionMode();

	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRate, Datas[0].StartSection);
	Datas[0].bCanMove ? Status->SetMove() : Status->SetStop();
}

void ACDoAction_Throw::Begin_DoAction()
{
	// 날아갈 위치, 방향 세팅
	FVector location = OwnerCharacter->GetMesh()->GetSocketLocation("Hand_ThrowItem");
	FRotator rotator = OwnerCharacter->GetController()->GetControlRotation();
	FTransform transform = Datas[0].EffectTransform;
	transform.AddToTranslation(location);
	transform.SetRotation(FQuat(rotator));

	ACThrow* throwObject = GetWorld()->SpawnActorDeferred<ACThrow>
			(Datas[0].ThrowClass, transform, OwnerCharacter, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
			// ESpawnActorCollisionHandlingMethod 스폰 할때, 오브젝트간 겹쳐 있을 경우에 대한 처리

	throwObject->OnThrowBeginOverlap.AddDynamic(this, &ACDoAction_Throw::OnThrowBeginOverlap);

	UGameplayStatics::FinishSpawningActor(throwObject, transform);
}

void ACDoAction_Throw::End_DoAction()
{
	State->SetIdleMode();
	Status->SetMove();

}

void ACDoAction_Throw::OnAim()
{
	Aim->On();
}

void ACDoAction_Throw::OffAim()
{
	Aim->Off();
}


void ACDoAction_Throw::OnThrowBeginOverlap(FHitResult InHitResult)
{
	FDamageEvent e;
	InHitResult.GetActor()->TakeDamage(Datas[0].power, e, OwnerCharacter->GetController(), this);
	
}

void ACDoAction_Throw::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Aim->Tick(DeltaTime);
}


void ACDoAction_Throw::AbortByTypeChanged(EActionType InPrevType, EActionType InNewType)
{
	if (Aim->IsAvaliable())
		Aim->Off();
}
