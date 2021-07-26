
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

	// Cull : ���� ���, Clip : �ȼ��� ���
	FVector2D center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);
	FVector2D size(Texture->GetSurfaceWidth() * 0.5f, Texture->GetSurfaceHeight() * 0.5f);
	FVector2D position = center - size;

	// �ʿ��� �Ķ���� : FTexture => SRV(Shader Resource View)�� ���� Description(�� �ȼ��� ���� ����), ����ȭ(1����ȭ) �� ������
	// ���� Texture : UTexture2D => ����, ������ �̹��� ������
	FCanvasTileItem item(position, Texture->Resource, FLinearColor::White);
	item.BlendMode = SE_BLEND_Translucent;		// ���ĸ� ����ϱ� ���� ���
				// ESimpleElementBlendMode:: => EnumClass�� �ƴϱ⿡ ����

	Canvas->DrawItem(item);
}