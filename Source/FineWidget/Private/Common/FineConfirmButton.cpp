// (c) 2023 Pururum LLC. All rights reserved.


#include "Common/FineConfirmButton.h"

#include "Components/Button.h"

UFineConfirmButton::UFineConfirmButton(
	const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
}

void UFineConfirmButton::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (IsValid(ConfirmButton))
	{
		ConfirmButton->SetClickMethod(EButtonClickMethod::PreciseClick);
	}
}

void UFineConfirmButton::OnConfirmButtonClicked()
{
	if (ConfirmMethod == EPerilousConfirmMethod::Click)
	{
		OnInteractionConfirmed.Broadcast();
	}
}

void UFineConfirmButton::OnConfirmUpdate()
{
	if (bHeld)
	{
		ElapsedTime = FMath::Clamp(ElapsedTime + UpdateInterval, 0, UpdateInterval);
		float Alpha = ElapsedTime / HoldingTime;
		if (FMath::IsNearlyEqual(ElapsedTime, HoldingTime, UE_KINDA_SMALL_NUMBER))
		{
			OnInteractionConfirmed.Broadcast();
		}
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(HoldTimerHandle);
	}
}

void UFineConfirmButton::OnConfirmButtonPressed()
{
	if (ConfirmMethod == EPerilousConfirmMethod::Hold)
	{
		bHeld = true;
		// Start the timer to confirm the interaction.
		GetWorld()->GetTimerManager().SetTimer(HoldTimerHandle, this,
		                                       &UFineConfirmButton::OnConfirmUpdate,
		                                       UpdateInterval,
		                                       true);
	}
}

void UFineConfirmButton::OnConfirmButtonReleased()
{
	if (ConfirmMethod == EPerilousConfirmMethod::Hold)
	{
		bHeld = false;
	}
}

void UFineConfirmButton::NativeConstruct()
{
	Super::NativeConstruct();
	if (IsValid(ConfirmButton))
	{
		ConfirmButton->OnClicked.AddDynamic(this, &UFineConfirmButton::OnConfirmButtonClicked);
		ConfirmButton->OnPressed.AddDynamic(this, &UFineConfirmButton::OnConfirmButtonPressed);
		ConfirmButton->OnReleased.AddDynamic(this, &UFineConfirmButton::OnConfirmButtonReleased);
	}
}

void UFineConfirmButton::NativeDestruct()
{
	// Clear timer just in case.
	GetWorld()->GetTimerManager().ClearTimer(HoldTimerHandle);
	if (IsValid(ConfirmButton))
	{
		ConfirmButton->OnClicked.RemoveDynamic(this, &UFineConfirmButton::OnConfirmButtonClicked);
		ConfirmButton->OnPressed.RemoveDynamic(this, &UFineConfirmButton::OnConfirmButtonPressed);
		ConfirmButton->OnReleased.RemoveDynamic(this, &UFineConfirmButton::OnConfirmButtonReleased);
	}
	Super::NativeDestruct();
}

FReply UFineConfirmButton::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	// if key is space bar, then invoke the interaction.
	if (InKeyEvent.GetKey() == ConfirmPadKey || InKeyEvent.GetKey() == ConfirmKey)
	{
		OnConfirmButtonPressed();
		LastKeyPressed = FDateTime::UtcNow();
		return FReply::Handled();
	}
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

FReply UFineConfirmButton::NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == ConfirmPadKey || InKeyEvent.GetKey() == ConfirmKey)
	{
		if (LastKeyPressed + FTimespan::FromSeconds(ClickTimeout) > FDateTime::UtcNow())
		{
			OnConfirmButtonClicked();
		}
		OnConfirmButtonReleased();
		return FReply::Handled();
	}
	return Super::NativeOnKeyUp(InGeometry, InKeyEvent);
}
