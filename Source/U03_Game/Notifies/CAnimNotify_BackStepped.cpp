#include "CAnimNotify_BackStepped.h"
#include "Global.h"
#include "Characters/CPlayer.h"

FString UCAnimNotify_BackStepped::GetNotifyName_Implementation() const
{
	return "BackStepped";
}

void UCAnimNotify_BackStepped::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());


	// 게임 플레이가 아니여도 몽타주는 재생 되기에 체크
	ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner());
	CheckNull(player);

	player->End_BackStep();
}