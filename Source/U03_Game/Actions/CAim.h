
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
	UPROPERTY(EditAnywhere)	// 타임라인에서 조준 On/Off시 부드럽게 가기 위한 커브
		class UCurveFloat* Curve;


public:
	FORCEINLINE bool IsAvaliable() { return SpringArm != nullptr && Camera != nullptr; }
	FORCEINLINE bool IsInZoom() { return bInZoom; }
	

public:
	UCAim();

	void BeginPlay(class ACharacter* InCharacter);	// Actor부터 있기에, 따로 만든 함수
	void Tick(float DeltaTime);

	void On();
	void Off();


private:
	UFUNCTION()
		void ZoomIn(float Output);

private:
	class ACharacter* OwnerCharacter;
	class UCStateComponent* State;
	class USpringArmComponent* SpringArm;	// 플레이어만 가짐 (적x)
	class UCameraComponent* Camera;
	bool bInZoom;

	FTimeline Timeline;
	FOnTimelineFloat TimelineFloat;

	class ACHUD* Hud;
};
