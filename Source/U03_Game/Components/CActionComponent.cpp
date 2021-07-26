#include "CActionComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Actions/CActionData.h"
#include "Actions/CEquipment.h"
#include "Actions/CDoAction.h"
#include "Actions/CAttachment.h"
#include "Actions/CDoAction_Throw.h"
#include "Actions/CAim.h"

UCActionComponent::UCActionComponent()
{

}


void UCActionComponent::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* character = Cast<ACharacter>(GetOwner());
	for (int32 i = 0; i < (int32)EActionType::Max; i++)
	{
		if (!!Datas[i])
			Datas[i]->BeginPlay(character);
	}
	
}

void UCActionComponent::SetUnarmedMode()
{
	// 현재 든 무기가 있을 때만
	if (!!Datas[(int32)Type])
		Datas[(int32)Type]->GetEquipment()->Unequip();

	Datas[(int32)EActionType::Unarmed]->GetEquipment()->Equip();

	ChangeType(EActionType::Unarmed);
}

void UCActionComponent::SetFistMode()
{
	SetMode(EActionType::Fist);
}

void UCActionComponent::SetOneHandMode()
{
	SetMode(EActionType::OneHand);
}

void UCActionComponent::SetTwoHandMode()
{
	SetMode(EActionType::TwoHand);
}

void UCActionComponent::SetWarpMode()
{
	SetMode(EActionType::Warp);
}

void UCActionComponent::SetTornadoMode() 
{
	SetMode(EActionType::Tornado);
}

void UCActionComponent::SetMagicBallMode()
{
	SetMode(EActionType::MagicBall);
}

void UCActionComponent::SetMode(EActionType InType)
{
	//CheckTrue()
	if (Type == InType)
	{
		SetUnarmedMode();
		return;
	}

	// 워프는 무기(장착)가 없음
	else if (IsUnarmedMode() == false)
	{
		if (!!Datas[(int32)Type])
			Datas[(int32)Type]->GetEquipment()->Unequip();
	}

	// 현재 장착 무기 변경
	if (!!Datas[(int32)InType])
		Datas[(int32)InType]->GetEquipment()->Equip();

	ChangeType(InType);
}

void UCActionComponent::ChangeType(EActionType InNewType)
{
	EActionType prevType = Type;
	Type = InNewType;


	if (OnActionTypeChanged.IsBound())
		OnActionTypeChanged.Broadcast(prevType,InNewType);
}

void UCActionComponent::DoAction()
{
	CheckTrue(IsUnarmedMode());

	if (!!Datas[(int32)Type])
	{
		ACDoAction* doAction = Datas[(int32)Type]->GetDoAction();

		if (!!doAction)
			doAction->DoAction();
	}
}

void UCActionComponent::DoAim_Begin()
{
	CheckTrue(IsUnarmedMode());

	if (!!Datas[(int32)Type])
	{
		ACDoAction* doAction = Datas[(int32)Type]->GetDoAction();

		if (!!doAction)
			doAction->OnAim();
	}
}

void UCActionComponent::DoAim_End()
{
	CheckTrue(IsUnarmedMode());

	if (!!Datas[(int32)Type])
	{
		ACDoAction* doAction = Datas[(int32)Type]->GetDoAction();

		if (!!doAction)
			doAction->OffAim();
	}
}


void UCActionComponent::OffAllCollision()
{
	for (UCActionData* data : Datas)
	{
		if (!!data == false)
			continue;
		
		if (!!data->GetAttachment() == false)
			continue;

		data->GetAttachment()->OffCollision();
	}
}