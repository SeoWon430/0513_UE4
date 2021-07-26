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
	// �θ� UFUNCTION()�̸� �ڽĵ� ��������
	virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter) override;
	virtual void OnAttachmentEndOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter) override;


private:
	// PlayerSetupInput�� �ٴ� �Լ� ���� ��������Ʈ���� ��� UFUNCTION()�� �پ�� ��
	UFUNCTION()
		void ResetGlobalDilation();

private:
	int32 Index;	// �޺� ī��Ʈ
	bool bEnable;	// ���� �޺� ���� ����	 => ��Ƽ���̿��� ����
	bool bExist;	// ���� ���� �޺� ����
	bool bLast;		// ��Ÿ ����

	TArray<class ACharacter*> HittedCharacters;

};
