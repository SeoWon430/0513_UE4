
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
	// UObject�� ���� �� ��, new�� �ȵ� => delete�� ������ �ϱ� ���� => �𸮾� ������ �÷��� ���� ó��

	Aim = NewObject<UCAim>();
	// ������ �÷��� ù �ֱ�(61��)�� ���� �� NewObject<> ������� Aim�� �ڴ�� ��������
	//	���� Aim�� ���ų� ���� �ֱⰡ ���� ������ ����
	//	=> ����ȭ�� �Ǹ�, ������ �÷��Ͱ� �Ժη� ������ ���� => UPROPERTY()
	//	=> �������


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
	// ���ư� ��ġ, ���� ����
	FVector location = OwnerCharacter->GetMesh()->GetSocketLocation("Hand_ThrowItem");
	FRotator rotator = OwnerCharacter->GetController()->GetControlRotation();
	FTransform transform = Datas[0].EffectTransform;
	transform.AddToTranslation(location);
	transform.SetRotation(FQuat(rotator));

	ACThrow* throwObject = GetWorld()->SpawnActorDeferred<ACThrow>
			(Datas[0].ThrowClass, transform, OwnerCharacter, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
			// ESpawnActorCollisionHandlingMethod ���� �Ҷ�, ������Ʈ�� ���� ���� ��쿡 ���� ó��

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
