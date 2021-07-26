
#include "CAim.h"
#include "Global.h"
#include "CHUD.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CStateComponent.h"


UCAim::UCAim()
{
	CHelpers::GetAsset<UCurveFloat>(&Curve, "CurveFloat'/Game/Actions/Curve_Aim.Curve_Aim'");
}

void UCAim::BeginPlay(ACharacter* InCharacter)
{
	OwnerCharacter = InCharacter;
	SpringArm = CHelpers::GetComponent<USpringArmComponent>(OwnerCharacter);
	Camera = CHelpers::GetComponent<UCameraComponent>(OwnerCharacter);
	State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);

	TimelineFloat.BindUFunction(this, "ZoomIn");
	Timeline.AddInterpFloat(Curve, TimelineFloat);
	Timeline.SetPlayRate(200);	// 진행 속도

	//Hud = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD<ACHUD>();	// Cast하는것 처럼 가져옴
	Hud = OwnerCharacter->GetWorld()->GetFirstPlayerController()->GetHUD<ACHUD>();	// 위처럼 하면 GetWorld()에서 Null나옴 => 이 객체가 UObject로 만들었기 때문

	Hud->DisableDraw();
}

void UCAim::Tick(float DeltaTime)
{
	Timeline.TickTimeline(DeltaTime);
}

void UCAim::On()
{
	CheckFalse(IsAvaliable());
	CheckTrue(bInZoom);
	bInZoom = true;

	SpringArm->TargetArmLength = 100.0f;
	SpringArm->SocketOffset = FVector(0, 30, 10);
	SpringArm->bEnableCameraLag = false;

	//Camera->FieldOfView = 45;
	Timeline.PlayFromStart();	// 시작시 무조건 처음부터 (  Play() => 중단됏던 지점부터  )

	Hud->EnableDraw();
}

void UCAim::Off()
{
	CheckFalse(IsAvaliable());
	CheckFalse(bInZoom);
	bInZoom = false;

	SpringArm->TargetArmLength = 200.0f;
	SpringArm->SocketOffset = FVector(0, 0, 0);
	SpringArm->bEnableCameraLag = true;

	//Camera->FieldOfView = 90;
	Timeline.ReverseFromEnd();

	Hud->DisableDraw();
}


// 타임라인 시간에 따른 결과 값(y)이 Output으로 나옴
void UCAim::ZoomIn(float Output)
{
	Camera->FieldOfView = Output;
}