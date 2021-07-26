
#include "CHUD.h"
#include "Global.h"
#include "Engine/Texture2D.h"
#include "Engine/Canvas.h"

ACHUD::ACHUD()
{
	CHelpers::GetAsset<UTexture2D>(&Texture, "Texture2D'/Game/Textures/T_Crosshair.T_Crosshair'");
}

void ACHUD::DrawHUD()
{
	Super::DrawHUD();

	CheckNull(Texture);
	CheckFalse(bDraw);

	// Cull : 점을 폐기, Clip : 픽셀을 폐기
	FVector2D center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);
	FVector2D size(Texture->GetSurfaceWidth() * 0.5f, Texture->GetSurfaceHeight() * 0.5f);
	FVector2D position = center - size;

	// 필요한 파라미터 : FTexture => SRV(Shader Resource View)에 대한 Description(각 픽셀에 대한 정보), 직렬화(1차원화) 된 데이터
	// 변수 Texture : UTexture2D => 에셋, 순수한 이미지 데이터
	FCanvasTileItem item(position, Texture->Resource, FLinearColor::White);
	item.BlendMode = SE_BLEND_Translucent;		// 알파를 사용하기 위한 모드
				// ESimpleElementBlendMode:: => EnumClass가 아니기에 지움

	Canvas->DrawItem(item);
}