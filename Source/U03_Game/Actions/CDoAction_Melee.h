// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/CDoAction.h"
#include "CDoAction_Melee.generated.h"

/**
 * 
 */
UCLASS()
class U03_GAME_API ACDoAction_Melee : public ACDoAction
{
	GENERATED_BODY()

public:
	FORCEINLINE void EnableCombo() { bEnable = true; }
	FORCEINLINE void DisableCombo() { bEnable = false; }


public:
	void DoAction() override;
	void Begin_DoAction() override;
	void End_DoAction() override;

public:
	// 부모가 UFUNCTION()이면 자식도 마찬가지
	virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter) override;
	virtual void OnAttachmentEndOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter) override;


private:
	// PlayerSetupInput에 붙는 함수 외의 델리게이트용은 모두 UFUNCTION()이 붙어야 함
	UFUNCTION()
		void ResetGlobalDilation();

private:
	int32 Index;	// 콤보 카운트
	bool bEnable;	// 다음 콤보 진행 여부	 => 노티파이에서 접근
	bool bExist;	// 다음 존재 콤보 여부
	bool bLast;		// 막타 여부

	TArray<class ACharacter*> HittedCharacters;

};
