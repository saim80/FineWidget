// (c) 2023 Pururum LLC. All rights reserved.


#include "Common/FineMessageBanner.h"

#include "Components/Button.h"
#include "Components/RichTextBlock.h"

void UFineMessageBanner::SetMessage(const FText NewText, const float NewDuration)
{
	MessageBlock->SetText(NewText);
	Show();
	GetWorld()->GetTimerManager().SetTimer(DismissTimerHandle, this, &UFineMessageBanner::Dismiss, NewDuration,
	                                       false);
}

FText UFineMessageBanner::GetMessage() const
{
	return MessageBlock->GetText();
}

void UFineMessageBanner::Dismiss()
{
	GetWorld()->GetTimerManager().ClearTimer(DismissTimerHandle);
	Hide();
}

void UFineMessageBanner::ClearMessage()
{
	MessageBlock->SetText(FText());
}

void UFineMessageBanner::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UFineMessageBanner::NativeConstruct()
{
	Super::NativeConstruct();
	// bind dismiss button click
	DismissButton->OnClicked.AddDynamic(this, &UFineMessageBanner::Dismiss);
}

void UFineMessageBanner::NativeDestruct()
{
	// unbind dismiss button click
	DismissButton->OnClicked.RemoveDynamic(this, &UFineMessageBanner::Dismiss);
	Super::NativeDestruct();
}

void UFineMessageBanner::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	Super::OnAnimationFinished_Implementation(Animation);
	if (Animation == GetHideAnimation())
	{
		OnBannerDismissed.Broadcast();
	}
}
