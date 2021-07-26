#include "CDoAction_Warp.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/DecalComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"

void ACDoAction_Warp::BeginPlay()
{
	Super::BeginPlay();


	// ���� Owner�� ���� ��� ��ü(Actor)
	//	SpawnActor �� ��, Owner�� ������ �� 
	//		=> �̶�, ���� Owner�� ������ Actor���� ����� �̵��� Children�� ��
	for (AActor* actor : OwnerCharacter->Children)
	{
		// IsA : Cast �Ǵ� ��� ��� ����
		if (actor->IsA<ACAttachment>() && actor->GetActorLabel().Contains("Warp"))	// Warp�� Ȯ��
		{
			Decal = CHelpers::GetComponent<UDecalComponent>(actor);
			break;
		}
	}

}

void ACDoAction_Warp::DoAction()
{
	CheckFalse(*bEquipped);
	CheckFalse(State->IsIdleMode());

	FRotator rotation;
	CheckFalse(GetCursorLocationAndRotation(Location, rotation));

	State->SetActionMode();
	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRate, Datas[0].StartSection);
	Datas[0].bCanMove ? Status->SetMove() : Status->SetStop();

}

void ACDoAction_Warp::Begin_DoAction()
{
	FTransform transform = Datas[0].EffectTransform;

	// ""���� �� �̸� �������� root�� ����
	UGameplayStatics::SpawnEmitterAttached
		(Datas[0].Effect
		, OwnerCharacter->GetMesh()
		, ""
		, transform.GetLocation()
		, FRotator(transform.GetRotation())
		, transform.GetScale3D());

	OwnerCharacter->SetActorLocation(Location);
	Location = OwnerCharacter->GetActorLocation();

}

void ACDoAction_Warp::End_DoAction()
{

	State->SetIdleMode();
	Status->SetMove();
}

void ACDoAction_Warp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckFalse(*bEquipped);

	FVector location; 
	FRotator rotation;
	if (GetCursorLocationAndRotation(location, rotation))
	{
		Decal->SetVisibility(true);
		Decal->SetWorldLocation(location);
		Decal->SetWorldRotation(rotation);
	}
	else
		Decal->SetVisibility(false);
}

bool ACDoAction_Warp::GetCursorLocationAndRotation(FVector& OutLocation, FRotator& OutRotation)
{
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	TArray<TEnumAsByte<EObjectTypeQuery>> queries;	// enum class�� �ƴϱ⿡ �̷��� ����
	queries.Add(EObjectTypeQuery::ObjectTypeQuery1);	// world static : ������Ʈ ���ÿ����� ����

	FHitResult hitResult;
	if (controller->GetHitResultUnderCursorForObjects(queries, false, hitResult))	// ���콺 ����ĳ��Ʈ
	{
		//hitResult.Location	=> �浹 �޽��� �������� ������ ����
		//hitResult.ImpactPoint	 => �浹 ������ ��� (�� ��Ȯ)
		OutLocation = hitResult.Location;
		OutRotation = hitResult.ImpactNormal.Rotation();

		return true;
	}


	return false;
}