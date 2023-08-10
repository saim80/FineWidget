// (c) 2023 Pururum LLC. All rights reserved.


#include "Common/FineTabButton.h"

void UFineTabButton::SetSelected(const bool& IsSelected)
{
	// If existing selected state is different from the new state. Update value and broadcast event.
	if (bIsSelected != IsSelected)
	{
		bIsSelected = IsSelected;
		OnTabButtonSelected.Broadcast(this, bIsSelected);
	}
}

void UFineTabButton::ToggleSelected()
{
	SetSelected(bTogglesOnClick ? !bIsSelected : true);
}

void UFineTabButton::NativeConstruct()
{
	Super::NativeConstruct();
	const auto Button = GetInnerButton();
	Button->OnClicked.AddUniqueDynamic(this, &UFineTabButton::ToggleSelected);
}

void UFineTabButton::NativeDestruct()
{
	const auto Button = GetInnerButton();
	Button->OnClicked.RemoveDynamic(this, &UFineTabButton::ToggleSelected);
	Super::NativeDestruct();
}
