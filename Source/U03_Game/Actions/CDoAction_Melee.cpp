
#include "CDoAction_Melee.h"
#include "Global.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "GameFramework/Character.h"

void ACDoAction_Melee::DoAction()
{
	Super::DoAction();

	CheckFalse(Datas.Num() > 0);

	if (bEnable == true)
	{
		bEnable = false;
		bExist = true;
		return;
	}


	CheckFalse(State->IsIdleMode());
	State->SetActionMode();

	HittedCharacters.Empty();
	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRate, Datas[0].StartSection);
	Datas[0].bCanMove ? Status->SetMove() : Status->SetStop();


}
void ACDoAction_Melee::Begin_DoAction()
{
	Super::Begin_DoAction();

	CheckFalse(bExist);
	bExist = false;

	OwnerCharacter->StopAnimMontage();

	Index++;
	FMath::Clamp(Index, 0, Datas.Num() - 1);

	HittedCharacters.Empty();
	OwnerCharacter->PlayAnimMontage(Datas[Index].AnimMontage, Datas[Index].PlayRate, Datas[Index].StartSection);
	Datas[Index].bCanMove ? Status->SetMove() : Status->SetStop();
}

void ACDoAction_Melee::End_DoAction()
{
	Super::End_DoAction();

	Index = 0;
	OwnerCharacter->StopAnimMontage(Datas[Index].AnimMontage);

	State->SetIdleMode();
	Status->SetMove();

}

void ACDoAction_Melee::OnAttachmentBeginOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InAttackCauser, InOtherCharacter);

	// �ߺ� ���� ����
	for (const ACharacter* other : HittedCharacters)
	{
		if (InOtherCharacter == other)
			return;
	}
	HittedCharacters.Add(InOtherCharacter);


	// Hit Stop : Ÿ�ݽ� Ÿ�� ���ο�
	float hitStop = Datas[Index].HitStop;
	if (FMath::IsNearlyZero(hitStop) == false)	// 0�� �ƴ϶��
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 2e-2f);
		// CustomTimeDilation = 0.2f	//=> �� �ش� ���͸� �ӵ��� ������
		
		// Ÿ�̸Ӵ� �� TimeDilation�� ���� ������ �ð��� ���� ����
		UKismetSystemLibrary::K2_SetTimer(this, "ResetGlobalDilation", hitStop * 2e-2f, false);
		
	}


	
	// Hit Effect (ParticleSystem)
	UParticleSystem* hitEffect = Datas[Index].Effect;
	if (!!hitEffect)
	{
		FTransform transform = Datas[Index].EffectTransform;
		transform.AddToTranslation(InOtherCharacter->GetActorLocation());	// ��� ��ǥ�� �̵�
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitEffect, transform);
	}


	// Camera Shake
	TSubclassOf<class UCameraShake> cameraShake = Datas[Index].ShakeClass;
	if (!!cameraShake)
	{
		APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if(!!controller)
			controller->PlayerCameraManager->PlayCameraShake(cameraShake);
	}


	// Send(Apply) Damage
	FDamageEvent e;
	InOtherCharacter->TakeDamage(Datas[Index].power, e, InAttacker->GetController(), InAttackCauser);
}

void ACDoAction_Melee::OnAttachmentEndOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter)
{
	Super::OnAttachmentEndOverlap(InAttacker, InAttackCauser, InOtherCharacter);

	//HittedCharacters.Empty();
}


void ACDoAction_Melee::ResetGlobalDilation()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
}