#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CActionComponent.generated.h"

UENUM(BlueprintType)
enum class EActionType : uint8
{
	Unarmed, Fist, OneHand, TwoHand, Warp, Tornado, MagicBall, Max
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FActionTypeChanged, EActionType, InPrevType, EActionType, InNewType);

// ���⿡ ���� �׼�(����, Ư��ȿ��)�� ����
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class U03_GAME_API UCActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCActionComponent();

private:
	UPROPERTY(EditDefaultsOnly)
		class UCActionData* Datas[(int32)EActionType::Max];

public:
	FORCEINLINE class UCActionData* GetCurrent() { return Datas[(int32)Type]; }


	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsUnarmedMode() { return Type == EActionType::Unarmed; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsFistMode() { return Type == EActionType::Fist; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsOneHandMode() { return Type == EActionType::OneHand; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsTwoHandMode() { return Type == EActionType::TwoHand; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsWarpMode() { return Type == EActionType::Warp; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsTornadoMode() { return Type == EActionType::Tornado; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsMagicBallMode() { return Type == EActionType::MagicBall; }

	void OffAllCollision();

public:
	void SetUnarmedMode();
	void SetFistMode();
	void SetOneHandMode();
	void SetTwoHandMode();
	void SetWarpMode();
	void SetTornadoMode();
	void SetMagicBallMode();

	void DoAction();

	void DoAim_Begin();
	void DoAim_End();

private:
	void SetMode(EActionType InType);
	void ChangeType(EActionType InNewType);

protected:
	virtual void BeginPlay() override;

public:
	// Purpose : ����(EActionType)�� ���� �׼�(����) ����
	// Call : Set������Mode ȣ���
	// Bind : ex) ACAnimInstance::OnTypeChanged
	//	Bind Setting : ACAnimInstance::BeginPlay
	//		=> ABP �ִ� �׷������� ���� Ÿ�Կ����� BlendSpace�� �ٲ�⿡ ���⿡ ������
	//		=> ����BP������Ʈ�� ABP������ Tick���� Player�� ���� Ÿ���� ��� Ȯ�� => �ƿ� ABP�� �����Ͽ� ���ʿ��� Ȯ���� ����
	UPROPERTY(BlueprintAssignable)
		FActionTypeChanged OnActionTypeChanged;

private:
	EActionType Type;
};