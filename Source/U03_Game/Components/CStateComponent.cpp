#include "CStateComponent.h"
#include "Global.h"

UCStateComponent::UCStateComponent()
{

}


void UCStateComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UCStateComponent::SetIdleMode()
{
	ChangeType(EStateType::Idle);
}

void UCStateComponent::SetRollMode()
{
	ChangeType(EStateType::Roll);
}

void UCStateComponent::SetBackStepMode()
{
	ChangeType(EStateType::BackStep);
}

void UCStateComponent::SetEquipMode()
{
	ChangeType(EStateType::Equip);
}

void UCStateComponent::SetActionMode()
{
	ChangeType(EStateType::Action);
}
void UCStateComponent::SetHittedMode()
{
	ChangeType(EStateType::Hitted);
}
void UCStateComponent::SetDeadMode()
{
	ChangeType(EStateType::Dead);
}

void UCStateComponent::ChangeType(EStateType InNewType)
{
	EStateType prev = Type;
	Type = InNewType;


	// State 변경시 등록된 델리게이트 함수 호출
	//	( Player->BeginPlay()에서 등록 )
	if (OnStateTypeChanged.IsBound())
		OnStateTypeChanged.Broadcast(prev, InNewType);
}
