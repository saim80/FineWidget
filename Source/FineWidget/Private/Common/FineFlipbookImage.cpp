// (c) 2023 Pururum LLC. All rights reserved.


#include "Common/FineFlipbookImage.h"

#include "PaperFlipbook.h"
#include "PaperSprite.h"
#include "PaperSpriteBlueprintLibrary.h"

void UFineFlipbookImage::SetFlipbook(UPaperFlipbook* NewFlipbook)
{
	// Set new flipbook and reset animation timer.
	Flipbook = NewFlipbook;
	Reset();
}

void UFineFlipbookImage::Reset()
{
	// Reset animation timer.
	CurrentFrameIndex = 0;
	UpdateAnimation();
	// Start animation using timer manager according to flipbook's frame rate.
	GetWorld()->GetTimerManager().SetTimer(AnimTimerHandle, this, &UFineFlipbookImage::UpdateAnimation,
	                                       1.0f / Flipbook->GetFramesPerSecond(), true);
}

TSharedRef<SWidget> UFineFlipbookImage::RebuildWidget()
{
	auto OutWidget = Super::RebuildWidget();
	// If flipbook is valid, start animating without resetting current frame index.
	if (IsValid(Flipbook))
	{
		GetWorld()->GetTimerManager().SetTimer(AnimTimerHandle, this, &UFineFlipbookImage::UpdateAnimation,
		                                       1.0f / Flipbook->GetFramesPerSecond(), true);
	}
	return OutWidget;
}

void UFineFlipbookImage::UpdateAnimation()
{
	if (!IsValid(Flipbook)) return;
	// Get the sprite from flipbook and turn it to brush.
	const auto Sprite = Flipbook->GetSpriteAtFrame(CurrentFrameIndex);
	const auto SourceSize = Sprite->GetSourceSize();

	FSlateBrush SpriteBrush;
	SpriteBrush.SetResourceObject(Sprite);
	const auto Width = SourceSize.X;
	const auto Height = SourceSize.Y;
	SpriteBrush.ImageSize = FVector2D(Width, Height);

	// Set the brush to image.
	SetBrush(SpriteBrush);
	// Increase frame index and reset it if it exceeds the number of frames.
	CurrentFrameIndex++;
	if (CurrentFrameIndex >= Flipbook->GetNumKeyFrames())
	{
		if (bFlipbookLoop)
		{
			CurrentFrameIndex = 0;
		}
		else
		{
			GetWorld()->GetTimerManager().ClearTimer(AnimTimerHandle);
		}
	}
}
