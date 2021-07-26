#include "CDoAction_Warp.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/DecalComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"

void ACDoAction_Warp::BeginPlay()
{
	Super::BeginPlay();


	// 같은 Owner를 가진 모든 객체(Actor)
	//	SpawnActor 할 때, Owner를 세팅해 줌 
	//		=> 이때, 같은 Owner를 가지는 Actor들이 생기며 이들이 Children이 됨
	for (AActor* actor : OwnerCharacter->Children)
	{
		// IsA : Cast 판단 대신 사용 가능
		if (actor->IsA<ACAttachment>() && actor->GetActorLabel().Contains("Warp"))	// Warp를 확인
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

	// ""으로 본 이름 안적으면 root에 붙음
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

	TArray<TEnumAsByte<EObjectTypeQuery>> queries;	// enum class가 아니기에 이렇게 선언
	queries.Add(EObjectTypeQuery::ObjectTypeQuery1);	// world static : 프로젝트 세팅에서의 순서

	FHitResult hitResult;
	if (controller->GetHitResultUnderCursorForObjects(queries, false, hitResult))	// 마우스 레이캐스트
	{
		//hitResult.Location	=> 충돌 메쉬의 폴리곤의 중점을 리턴
		//hitResult.ImpactPoint	 => 충돌 지점을 계산 (더 정확)
		OutLocation = hitResult.Location;
		OutRotation = hitResult.ImpactNormal.Rotation();

		return true;
	}


	return false;
}