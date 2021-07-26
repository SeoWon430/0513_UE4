
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/TimelineComponent.h"
#include "CAim.generated.h"

UCLASS()
class U03_GAME_API UCAim : public UObject
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)	// Ÿ�Ӷ��ο��� ���� On/Off�� �ε巴�� ���� ���� Ŀ��
		class UCurveFloat* Curve;


public:
	FORCEINLINE bool IsAvaliable() { return SpringArm != nullptr && Camera != nullptr; }
	FORCEINLINE bool IsInZoom() { return bInZoom; }
	

public:
	UCAim();

	void BeginPlay(class ACharacter* InCharacter);	// Actor���� �ֱ⿡, ���� ���� �Լ�
	void Tick(float DeltaTime);

	void On();
	void Off();


private:
	UFUNCTION()
		void ZoomIn(float Output);

private:
	class ACharacter* OwnerCharacter;
	class UCStateComponent* State;
	class USpringArmComponent* SpringArm;	// �÷��̾ ���� (��x)
	class UCameraComponent* Camera;
	bool bInZoom;

	FTimeline Timeline;
	FOnTimelineFloat TimelineFloat;

	class ACHUD* Hud;
};
