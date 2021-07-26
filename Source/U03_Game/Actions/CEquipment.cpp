#include "CEquipment.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Interfaces/ICharacter.h"

ACEquipment::ACEquipment()
{

}

void ACEquipment::BeginPlay()
{
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	Status = CHelpers::GetComponent<UCStatusComponent>(OwnerCharacter);

	Super::BeginPlay();
}

//PlayEquipMontage, PawnControl, ChangeBodyMaterial
void ACEquipment::Equip_Implementation()
{
	State->SetEquipMode();

	if (!!Data.AnimMontage)
		OwnerCharacter->PlayAnimMontage(Data.AnimMontage, Data.PlayRate, Data.StartSection);

	// 몽타주가 없으면 노티파이가 없기 때문에 아래 함수 호출이 안되기에 따로 호출 함
	else
	{
		Begin_Equip();	// 호출 할때는 _Implementation가 없는 원본 함수를 호출
		End_Equip();
	}

	// 캐릭터 회전 설정 bPawnControl => true면 항상 전방을 바라봄
	if (Data.bPawnControl)
	{
		OwnerCharacter->bUseControllerRotationYaw = true;
		OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
	}

	IICharacter* character = Cast<IICharacter>(OwnerCharacter);
	CheckNull(character);
	character->ChangeColor(Color);
}

//Socket? Particle? Decal? -> Delegate Execute
void ACEquipment::Begin_Equip_Implementation()
{
	if (OnEquipmentDelegate.IsBound())
		OnEquipmentDelegate.Broadcast();
}

//State -> Idle
void ACEquipment::End_Equip_Implementation()
{
	State->SetIdleMode();
	bEquipped = true;
}

//OrientRotation -> true
void ACEquipment::Unequip_Implementation()
{
	// 캐릭터 회전 설정 초기화
	OwnerCharacter->bUseControllerRotationYaw = false;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;

	bEquipped = false;

	if (OnUnequipmentDelegate.IsBound())
		OnUnequipmentDelegate.Broadcast();
}
