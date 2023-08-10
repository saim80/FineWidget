// (c) 2023 Pururum LLC. All rights reserved.


#include "..\..\Public\StackNavigation\FineStackNavigation.h"

#include "FineWidgetLog.h"
#include "Common/FineContent.h"
#include "StackNavigation/FineStackTitleBar.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"

UFineStackNavigation::UFineStackNavigation(const FObjectInitializer& ObjectInitializer): Super(
	ObjectInitializer)
{
}

void UFineStackNavigation::Push(UFineContent* ContentWidget)
{
	if (!ensure(IsValid(ContentWidget))) return;
	if (IsPlayingAnimation() && ContentWidgets.Num() > 0)
	{
		FW_WARNING("Pushing a content widget is not safe while playing animation.");
	}

	// Get the current top content widget
	UFineContent* TopContentWidget = nullptr;
	if (ContentWidgets.Num() > 0)
	{
		TopContentWidget = ContentWidgets.Last();
		ContentWidget->bIsRoot = false;
	}
	else
	{
		ContentWidget->bIsRoot = true;
	}
	
	// If the current top content widget is valid, add it to BackOverlay.
	if (IsValid(TopContentWidget))
	{
		const auto OverlaySlot = BackOverlay->AddChildToOverlay(TopContentWidget);
		OverlaySlot->SetVerticalAlignment(VAlign_Fill);
		OverlaySlot->SetHorizontalAlignment(HAlign_Fill);
	}

	// Add the new content widget to FrontOverlay.
	PushQueue.Add(ContentWidget);
	ContentWidgets.Add(ContentWidget);
	const auto OverlaySlot = FrontOverlay->AddChildToOverlay(ContentWidget);
	OverlaySlot->SetVerticalAlignment(VAlign_Fill);
	OverlaySlot->SetHorizontalAlignment(HAlign_Fill);
	UpdateTitleBar(ContentWidget);

	// For the pushed widget, register callback for navigation title visibility change.
	const auto WeakThis = TWeakObjectPtr<UFineStackNavigation>(this);
	ContentWidget->OnNavigationBarVisibleChanged.BindLambda([=] (auto Widget, auto bVisible)
	{
		if (!WeakThis.IsValid()) return;
		// if the top content widget's nav bar changed, update the bar.
		if (Widget == WeakThis->ContentWidgets.Last())
		{
			WeakThis->UpdateTitleBar(ContentWidget);
		}
	});
	ContentWidget->OnNavigationPopRequested.BindLambda([=] (auto Widget)
	{
		if (!WeakThis.IsValid()) return;
		// if the top content widget is the given widget, pop it.
		if (Widget == WeakThis->ContentWidgets.Last())
		{
			WeakThis->Pop();
		}
	});

	PlayAnimation(PushAnimation);
}

void UFineStackNavigation::Pop()
{
	if (ContentWidgets.Num() <= 1) return;
	if (IsPlayingAnimation()) return;
	const auto ContentWidget = ContentWidgets.Pop();
	if (!ensure(IsValid(ContentWidget))) return;

	// Get the current top content widget
	UFineContent* TopContentWidget = nullptr;
	if (ContentWidgets.Num() > 0)
	{
		TopContentWidget = ContentWidgets.Last();
	}
	
	// If the current top content widget is valid, add it to BackOverlay.
	if (IsValid(TopContentWidget))
	{
		const auto BackSlot = BackOverlay->AddChildToOverlay(TopContentWidget);
		BackSlot->SetVerticalAlignment(VAlign_Fill);
		BackSlot->SetHorizontalAlignment(HAlign_Fill);
		UpdateTitleBar(TopContentWidget);
	}

	// Add the new content widget to FrontOverlay.
	PopQueue.Add(ContentWidget);
	const auto FrontSlot = FrontOverlay->AddChildToOverlay(ContentWidget);
	FrontSlot->SetVerticalAlignment(VAlign_Fill);
	FrontSlot->SetHorizontalAlignment(HAlign_Fill);

	PlayAnimation(PopAnimation);
}

void UFineStackNavigation::PopToRoot()
{
	if (ContentWidgets.Num() <= 1) return;
	if (IsPlayingAnimation()) return;
	const auto ContentWidget = ContentWidgets.Pop();
	if (!ensure(IsValid(ContentWidget))) return;

	// Get the current top content widget
	UFineContent* RootContent = nullptr;
	if (ContentWidgets.Num() > 0)
	{
		RootContent = ContentWidgets[0];
	}
	// If the current top content widget is valid, add it to BackOverlay.
	if (IsValid(RootContent))
	{
		const auto BackSlot = BackOverlay->AddChildToOverlay(RootContent);
		BackSlot->SetVerticalAlignment(VAlign_Fill);
		BackSlot->SetHorizontalAlignment(HAlign_Fill);
		UpdateTitleBar(RootContent);
	}

	// Add the new content widget to FrontOverlay.
	for (auto Widget : ContentWidgets)
	{
		if (Widget != RootContent)
			PopQueue.Add(Widget);
	}
	const auto FrontSlot = FrontOverlay->AddChildToOverlay(ContentWidget);
	FrontSlot->SetVerticalAlignment(VAlign_Fill);
	FrontSlot->SetHorizontalAlignment(HAlign_Fill);

	PlayAnimation(PopAnimation);
}

void UFineStackNavigation::OnAnimationStarted_Implementation(const UWidgetAnimation* Animation)
{
	Super::OnAnimationStarted_Implementation(Animation);
	if (Animation == PushAnimation)
	{
		for (const auto ContentWidget : PushQueue)
		{
			ContentWidget->StackNavigationPtr = this;
			ContentWidget->OnPrePush();
		}
	}
	else if (Animation == PopAnimation)
	{
		for (const auto ContentWidget : PopQueue)
		{
			ContentWidget->OnNavigationPopRequested.Unbind();
			ContentWidget->OnNavigationBarVisibleChanged.Unbind();
			ContentWidget->OnPrePop();
		}
	}
}

void UFineStackNavigation::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	Super::OnAnimationFinished_Implementation(Animation);
	if (Animation == PushAnimation)
	{
		BackOverlay->ClearChildren();
		for (const auto ContentWidget : PushQueue)
		{
			ContentWidget->OnPostPush();
		}
		PushQueue.Empty();
	}
	else if (Animation == PopAnimation)
	{
		FrontOverlay->ClearChildren();
		for (const auto ContentWidget : PopQueue)
		{
			ContentWidget->OnPostPop();
			ContentWidget->StackNavigationPtr.Reset();
		}
		PopQueue.Empty();
	}
}

void UFineStackNavigation::UpdateTitleBar(UFineContent* ContentWidget)
{
	for (UWidget* Child : TitleOverlay->GetAllChildren())
	{
		// Cast child to UFineStackTitleBarWidget
		UFineStackTitleBar* TitleBarWidget = Cast<UFineStackTitleBar>(Child);
		if (IsValid(TitleBarWidget))
		{
			TitleBarWidget->Hide();
			FreeTitleBarWidgets.Add(TitleBarWidget);
		}
	}
	if (!ContentWidget->IsTitleBarVisible()) return;
	UFineStackTitleBar* TitleBarWidget = nullptr;
	// If free title bars are two or more, use the oldest one.
	// Otherwise, create a new one. Note that we don't want to use the only one in the queue in case it's being removed
	// with animation.
	if (FreeTitleBarWidgets.Num() > 1)
	{
		TitleBarWidget = FreeTitleBarWidgets[0];
		FreeTitleBarWidgets.RemoveAt(0);
	}
	else
	{
		TitleBarWidget = CreateWidget<UFineStackTitleBar>(this, TitleBarClass);
	}
	// Set up the title bar widget.
	if (IsValid(TitleBarWidget))
	{
		TitleBarWidget->UpdateInternal(ContentWidget);
		const auto TitleSlot = TitleOverlay->AddChildToOverlay(TitleBarWidget);
		TitleSlot->SetVerticalAlignment(VAlign_Fill);
		TitleSlot->SetHorizontalAlignment(HAlign_Fill);
		TitleBarWidget->Show();
	}
}
