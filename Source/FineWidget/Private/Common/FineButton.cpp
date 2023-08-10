// (c) 2023 Pururum LLC. All rights reserved.


#include "Common/FineButton.h"

void UFineButton::NativeConstruct()
{
	Super::NativeConstruct();
	InnerButton->OnClicked.AddUniqueDynamic(this, &UFineButton::InternalOnClicked);
	InnerButton->OnPressed.AddUniqueDynamic(this, &UFineButton::InternalOnPressed);
	InnerButton->OnReleased.AddUniqueDynamic(this, &UFineButton::InternalOnReleased);
	InnerButton->OnHovered.AddUniqueDynamic(this, &UFineButton::InternalOnHovered);
	InnerButton->OnUnhovered.AddUniqueDynamic(this, &UFineButton::InternalOnUnhovered);
}

void UFineButton::NativeDestruct()
{
	InnerButton->OnClicked.RemoveDynamic(this, &UFineButton::InternalOnClicked);
	InnerButton->OnPressed.RemoveDynamic(this, &UFineButton::InternalOnPressed);
	InnerButton->OnReleased.RemoveDynamic(this, &UFineButton::InternalOnReleased);
	InnerButton->OnHovered.RemoveDynamic(this, &UFineButton::InternalOnHovered);
	InnerButton->OnUnhovered.RemoveDynamic(this, &UFineButton::InternalOnUnhovered);
	Super::NativeDestruct();
}

void UFineButton::InternalOnClicked()
{
	OnClicked.Broadcast();
}

void UFineButton::InternalOnPressed()
{
	OnPressed.Broadcast();
}

void UFineButton::InternalOnReleased()
{
	OnReleased.Broadcast();
}

void UFineButton::InternalOnHovered()
{
	OnReleased.Broadcast();
}

void UFineButton::InternalOnUnhovered()
{
	OnUnhovered.Broadcast();
}
