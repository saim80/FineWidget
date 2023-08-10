// (c) 2023 Pururum LLC. All rights reserved.


#include "StackNavigation/FineStackTitleBarWidget.h"

#include "StackNavigation/FineStackContentWidget.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"

UFineStackTitleBarWidget::UFineStackTitleBarWidget(const FObjectInitializer& ObjectInitializer): Super(
	ObjectInitializer)
{
}

void UFineStackTitleBarWidget::UpdateInternal_Implementation(UFineStackContentWidget* ContentWidget)
{
	if (IsValid(ContentWidget))
	{
		return;
	}
	// clear left widget after removing it from parent.
	if (IsValid(LeftWidget))
	{
		LeftOverlay->RemoveChild(LeftWidget);
		LeftWidget = nullptr;
	}
	// clear right widget after removing it from parent.
	if (IsValid(RightWidget))
	{
		RightOverlay->RemoveChild(RightWidget);
		RightWidget = nullptr;
	}
	// clear title widget after removing it from parent.
	if (IsValid(TitleWidget))
	{
		TitleOverlay->RemoveChild(TitleWidget);
		TitleWidget = nullptr;
	}

	// Get left widget from content widget.
	UWidget* NewLeftWidget = ContentWidget->GetLeftBarWidget();
	if (IsValid(NewLeftWidget))
	{
		LeftWidget = NewLeftWidget;
		const auto Slot = LeftOverlay->AddChildToOverlay(NewLeftWidget);
		Slot->SetVerticalAlignment(VAlign_Fill);
		Slot->SetHorizontalAlignment(HAlign_Fill);
	}
	// Get right widget from content widget.
	UWidget* NewRightWidget = ContentWidget->GetRightBarWidget();
	if (IsValid(NewRightWidget))
	{
		RightWidget = NewRightWidget;
		const auto Slot = RightOverlay->AddChildToOverlay(NewRightWidget);
		Slot->SetVerticalAlignment(VAlign_Fill);
		Slot->SetHorizontalAlignment(HAlign_Fill);
	}
	// Get title widget from content widget.
	UWidget* NewTitleWidget = ContentWidget->GetTitleBarWidget();
	const FText NewTitleText = ContentWidget->GetTitleText();
	if (NewTitleText.IsEmpty())
	{
		if (IsValid(NewTitleWidget))
		{
			TitleWidget = NewTitleWidget;
			const auto Slot = TitleOverlay->AddChildToOverlay(NewTitleWidget);
			Slot->SetVerticalAlignment(VAlign_Fill);
			Slot->SetHorizontalAlignment(HAlign_Fill);
		}
	}
	else
	{
		TitleTextBlock->SetText(NewTitleText);
		const auto Slot = TitleOverlay->AddChildToOverlay(TitleTextBlock);
		Slot->SetVerticalAlignment(VAlign_Center);
		Slot->SetHorizontalAlignment(HAlign_Center);
	}

	BarSizeBox->SetHeightOverride(ContentWidget->GetPreferredBarHeight());
}
