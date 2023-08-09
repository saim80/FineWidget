// Fill out your copyright notice in the Description page of Project Settings.


#include "FineBaseWidget.h"

#include "FineWidgetLog.h"
#include "Animation/WidgetAnimation.h"

UFineBaseWidget::UFineBaseWidget(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	bPlayShowAnimationOnConstruct = true;
	bRemoveFromParentOnHide = true;
	bUpdateLayoutOnViewportResized = false;

	PreShowAnimationVisibility = ESlateVisibility::HitTestInvisible;
	PostShowAnimationVisibility = ESlateVisibility::SelfHitTestInvisible;
	PreHideAnimationVisibility = ESlateVisibility::HitTestInvisible;
	PostHideAnimationVisibility = ESlateVisibility::Collapsed;
}

void UFineBaseWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (bUpdateLayoutOnViewportResized)
	{
		RegisterViewportResizeEvent();
	}
	if (bPlayShowAnimationOnConstruct)
	{
		Show();
	}
}

void UFineBaseWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UFineBaseWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	Super::OnAnimationFinished_Implementation(Animation);
	if (Animation == ShowAnimation)
	{
		SetVisibility(PostShowAnimationVisibility);
	}
	else if (Animation == HideAnimation)
	{
		SetVisibility(PostHideAnimationVisibility);
		if (bRemoveFromParentOnHide)
		{
			RemoveFromParent();
		}
	}
}

void UFineBaseWidget::Show()
{
	if (IsValid(ShowAnimation))
	{
		SetVisibility(PreShowAnimationVisibility);
		PlayAnimation(ShowAnimation);
	}
	else
	{
		SetVisibility(PostShowAnimationVisibility);
	}
}

void UFineBaseWidget::Hide()
{
	if (IsValid(HideAnimation))
	{
		SetVisibility(PreHideAnimationVisibility);
		PlayAnimation(HideAnimation);
	}
	else
	{
		SetVisibility(PostHideAnimationVisibility);
	}
}

void UFineBaseWidget::RegisterViewportResizeEvent()
{
	// create weak ref to this.
	TWeakObjectPtr<UFineBaseWidget> WeakThis(this);
	ViewportResizeHandle = GEngine->GameViewport->Viewport->ViewportResizedEvent.AddLambda(
		[=](auto Viewport, auto Index)
		{
			// check if weak ref is valid.
			if (WeakThis.IsValid())
			{
				// call UpdateLayout() if valid.
				WeakThis->UpdateLayout();
			}
		});
	// Initial update.
	UpdateLayout();
}

void UFineBaseWidget::UnregisterViewportResizeEvent()
{
	// remove the delegate.
	GEngine->GameViewport->Viewport->ViewportResizedEvent.Remove(ViewportResizeHandle);
}

void UFineBaseWidget::UpdateLayout_Implementation()
{
	FW_VERBOSE("UFineBaseWidget::UpdateLayout_Implementation() is not implemented."
		"If you want to use this function, please override it in your widget class."
		" Otherwise, turn off by setting `bUpdateLayoutOnViewportResized` to false.");
}

void UFineBaseWidget::SetUpdateLayoutOnViewportResized(const bool& UpdateLayoutOnViewportResized)
{
	if (bUpdateLayoutOnViewportResized == UpdateLayoutOnViewportResized) return;
	bUpdateLayoutOnViewportResized = UpdateLayoutOnViewportResized;
	if (bUpdateLayoutOnViewportResized)
	{
		RegisterViewportResizeEvent();
	}
	else
	{
		UnregisterViewportResizeEvent();
	}
}
