// (c) 2023 Pururum LLC. All rights reserved.


#include "StackNavigation/FineStackTitleBar.h"

#include "Common/FineContent.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"

UFineStackTitleBar::UFineStackTitleBar(const FObjectInitializer& ObjectInitializer): Super(
	ObjectInitializer)
{
}

void UFineStackTitleBar::UpdateInternal_Implementation(UFineContent* ContentWidget)
{
	if (!IsValid(ContentWidget))
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
		const auto LeftSlot = LeftOverlay->AddChildToOverlay(NewLeftWidget);
		LeftSlot->SetVerticalAlignment(VAlign_Fill);
		LeftSlot->SetHorizontalAlignment(HAlign_Fill);
	}
	// Get right widget from content widget.
	UWidget* NewRightWidget = ContentWidget->GetRightBarWidget();
	if (IsValid(NewRightWidget))
	{
		RightWidget = NewRightWidget;
		const auto RightSlot = RightOverlay->AddChildToOverlay(NewRightWidget);
		RightSlot->SetVerticalAlignment(VAlign_Fill);
		RightSlot->SetHorizontalAlignment(HAlign_Fill);
	}
	// Get title widget from content widget.
	UWidget* NewTitleWidget = ContentWidget->GetTitleBarWidget();
	const FText NewTitleText = ContentWidget->GetTitleText();
	if (NewTitleText.IsEmpty())
	{
		if (IsValid(NewTitleWidget))
		{
			TitleWidget = NewTitleWidget;
			const auto TitleSlot = TitleOverlay->AddChildToOverlay(NewTitleWidget);
			TitleSlot->SetVerticalAlignment(VAlign_Fill);
			TitleSlot->SetHorizontalAlignment(HAlign_Fill);
		}
	}
	else
	{
		TitleTextBlock->SetText(NewTitleText);
		const auto TitleSlot = TitleOverlay->AddChildToOverlay(TitleTextBlock);
		TitleSlot->SetVerticalAlignment(VAlign_Center);
		TitleSlot->SetHorizontalAlignment(HAlign_Center);
	}

	BarSizeBox->SetHeightOverride(ContentWidget->GetPreferredBarHeight());
}
