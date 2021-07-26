#include "CMontagesComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"

UCMontagesComponent::UCMontagesComponent()
{

}




void UCMontagesComponent::BeginPlay()
{
	Super::BeginPlay();

	//GetRows from DataTable (GetAllRows 는 가변길이 배열만 가져옴)
	TArray<FMontageData*> datas;
	DataTable->GetAllRows<FMontageData>("", datas);

	//Write Datas(Member) from TArray
	// 고정길이에 담기
	//	EStateType과 인덱스를 맞추기 위해 고정길이로 h에 따로 변수 선언 했음
	for (int32 i = 0; i < (int32)EStateType::Max; i++)
	{
		for (FMontageData* data : datas)
		{
			if ((EStateType)i == data->Type)
			{
				Datas[i] = data;
				continue;
			}
		}
	}
	
}

void UCMontagesComponent::PlayRoll()
{
	PlayAnimMontage(EStateType::Roll);
}

void UCMontagesComponent::PlayBackStep()
{
	PlayAnimMontage(EStateType::BackStep);
}

void UCMontagesComponent::PlayHitted()
{
	PlayAnimMontage(EStateType::Hitted);
}

void UCMontagesComponent::PlayDead()
{
	PlayAnimMontage(EStateType::Dead);
}

void UCMontagesComponent::PlayAnimMontage(EStateType InStateType)
{
	ACharacter* character = Cast<ACharacter>(GetOwner());
	//CheckNull(character);

	FMontageData* data = Datas[(int32)InStateType];
	if (!!data)
	{
		if (!!data->AnimMontage)
			character->PlayAnimMontage(data->AnimMontage, data->PlayRatio, data->StartSection);
	}

}


