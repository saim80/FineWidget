// (c) 2023 Pururum LLC. All rights reserved.


#include "FineWidgetScreen.h"

#include "Common/FineBaseWidget.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"


class UFineBaseWidget;

UFineWidgetScreen::UFineWidgetScreen(): Super()
{
	PrimaryComponentTick.bCanEverTick = false;
	LayerPriority = 0;
	UserWidget = nullptr;
	bAutoActivate = true;
}

// Called when the game starts
void UFineWidgetScreen::BeginPlay()
{
	Super::BeginPlay();
	HideAnimationStartedEvent.BindDynamic(this, &UFineWidgetScreen::OnHideAnimationStarted);
	if (IsActive())
	{
		AddWidget();
	}
}

void UFineWidgetScreen::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Deactivate();
	HideAnimationStartedEvent.Clear();
	Super::EndPlay(EndPlayReason);
}

void UFineWidgetScreen::Activate(bool bReset)
{
	Super::Activate(bReset);
	AddWidget();
}

void UFineWidgetScreen::Deactivate()
{
	Super::Deactivate();
	RemoveWidget();
}

void UFineWidgetScreen::OnHideAnimationStarted()
{
	// When this function is called and UserWidget is valid, it means that the widget is being removed from the
	// viewport from elsewhere. Clean up the reference for integrity.

	// Check if UserWidget is valid.
	if (!IsValid(UserWidget)) return;
	// Remove hide animation started event binding.
	if (UserWidget->IsA<UFineBaseWidget>())
	{
		UFineBaseWidget* FineBaseWidget = Cast<UFineBaseWidget>(UserWidget);
		if (IsValid(FineBaseWidget))
		{
			FineBaseWidget->UnbindFromAnimationStarted(FineBaseWidget->GetHideAnimation(), HideAnimationStartedEvent);
		}
	}
	// Clean up the reference.
	UserWidget = nullptr;
}

void UFineWidgetScreen::ToggleWidget()
{
	ToggleActive();
}

void UFineWidgetScreen::AddWidget()
{
	if (!IsActive()) return;
	if (IsValid(UserWidget)) return;
	// Get current local player's controller.
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, PlayerIndex);
	// Create widget with the widget class we have now.
	UserWidget = CreateWidget<UUserWidget>(PlayerController, WidgetClass.LoadSynchronous());
	if (!IsValid(UserWidget)) return;
	if (UserWidget->IsA<UFineBaseWidget>())
	{
		// Bind to hide animation started event for UserWidget
		UFineBaseWidget* FineBaseWidget = Cast<UFineBaseWidget>(UserWidget);
		if (IsValid(FineBaseWidget))
		{
			FineBaseWidget->BindToAnimationStarted(FineBaseWidget->GetHideAnimation(), HideAnimationStartedEvent);
		}
	}
	if (!IsValid(UserWidget)) return;
	if (bAddToPlayerScreen)
	{
		UserWidget->AddToPlayerScreen(LayerPriority);
	}
	else
	{
		// Add widget to the viewport.
		UserWidget->AddToViewport(LayerPriority);
	}
}

void UFineWidgetScreen::RemoveWidget()
{
	if (!IsValid(UserWidget)) return;
	auto OldWidget = UserWidget;
	// Clean up the reference.
	UserWidget = nullptr;
	// Remove widget from the viewport if the widget is not a subclass of UFineBaseWidget.
	if (!OldWidget->IsA<UFineBaseWidget>())
	{
		OldWidget->RemoveFromParent();
		return;
	}
	// Cast the widget to FineBaseWidget.
	UFineBaseWidget* FineBaseWidget = Cast<UFineBaseWidget>(OldWidget);
	if (!IsValid(FineBaseWidget)) return;
	// Invoke hide animation if CanRemoveFromParentOnHide is true.
	if (FineBaseWidget->CanRemoveFromParentOnHide())
	{
		FineBaseWidget->Hide();
	}
	else
	{
		FineBaseWidget->RemoveFromParent();
	}
}
