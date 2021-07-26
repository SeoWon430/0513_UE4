
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actions/CActionData.h"
#include "CDoAction.generated.h"

UCLASS()
class U03_GAME_API ACDoAction : public AActor
{
	GENERATED_BODY()
	
public:
	FORCEINLINE void SetDatas(TArray<FDoActionData> InDatas) { Datas = InDatas; }
	FORCEINLINE void SetEquipped(const bool* InEquipped) { bEquipped = InEquipped; }

public:	
	ACDoAction();

public:
	// 좌 클릭시 실행할 기능
	//	자식에서 재정의
	virtual void DoAction() {};

	// 노티파이 용
	//	자식에서 재정의
	virtual void Begin_DoAction() {};	
	virtual void End_DoAction() {};

	
	virtual void OnAim() {};	// Melee에서는 안쓰고, Throw에서 쓸것
	virtual void OffAim() {};


public:
	UFUNCTION()
		virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter) {};
	UFUNCTION()
		virtual void OnAttachmentEndOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter) {};



protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;


protected:
	UPROPERTY(BlueprintReadOnly)
		class ACharacter* OwnerCharacter;

	UPROPERTY(BlueprintReadOnly)
		class UCStateComponent* State;

	UPROPERTY(BlueprintReadOnly)
		class UCStatusComponent* Status;

protected:
	TArray<FDoActionData> Datas;
	const bool* bEquipped;		// CEquipment.cpp에서 가져올것
};
