// (c) 2023 Pururum LLC. All rights reserved.


#include "TabNavigation/FineTabBar.h"

#include "FineWidgetLog.h"
#include "Common/FineTabButton.h"
#include "Components/HorizontalBox.h"

UFineTabBar::UFineTabBar(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	SelectedIndex = -1;
}

void UFineTabBar::SetSelectedIndex(const int32& NewIndex)
{
	if (NewIndex < 0)
	{
		SelectedIndex = NewIndex;
		return;
	}
	// if selected index is different from the new index, update value and broadcast event.
	if (SelectedIndex != NewIndex)
	{
		SelectedIndex = NewIndex;
		// for each button in button container, if index matches, set selected to true, otherwise false.
		auto Buttons = ButtonContainer->GetAllChildren().FilterByPredicate([=](UWidget* Widget)
		{
			return Widget->IsA<UFineTabButton>();
		});
		if (ensure(Buttons.Num() == ButtonContainer->GetChildrenCount()))
		{
			for (int32 i = 0; i < Buttons.Num(); ++i)
			{
				const auto Button = Cast<UFineTabButton>(Buttons[i]);
				if (ensure(IsValid(Button)))
				{
					Button->SetSelected(i == SelectedIndex);
				}
			}
		}
		else
		{
			FW_FATAL("Tab bar contains non-tab button widget.");
		}
		OnSelectionChanged.Broadcast(SelectedIndex);
	}
}

void UFineTabBar::AddButton(UFineTabButton* Button)
{
	check(IsValid(Button));
	ButtonContainer->AddChild(Button);
	PrepareButton(Button);
}

void UFineTabBar::InsertButtonAt(const int32& Index, UFineTabButton* Button)
{
	check(IsValid(Button));
	ButtonContainer->InsertChildAt(Index, Button);
	PrepareButton(Button);
}

void UFineTabBar::RemoveButtonAt(const int32& Index)
{
	auto Buttons = ButtonContainer->GetAllChildren().FilterByPredicate([=](UWidget* Widget)
	{
		return Widget->IsA<UFineTabButton>();
	});
	if (ensure(Buttons.Num() == ButtonContainer->GetChildrenCount()))
	{
		if (Buttons.IsValidIndex(Index))
		{
			const auto OldButton = Cast<UFineTabButton>(Buttons[Index]);
			CleanUpButton(OldButton);
			ButtonContainer->RemoveChildAt(Index);
		}
	}
	else
	{
		FW_FATAL("Tab bar contains non-tab button widget.");
	}
}

void UFineTabBar::RemoveButton(UFineTabButton* Button)
{
	check(IsValid(Button));
	if (const auto FoundIndex = ButtonContainer->GetChildIndex(Button))
	{
		if (FoundIndex >= 0 && FoundIndex < ButtonContainer->GetChildrenCount())
		{
			RemoveButtonAt(FoundIndex);
		}
	}
}

void UFineTabBar::ClearButtons()
{
	// Call clean up button for each button in button container.
	auto Buttons = ButtonContainer->GetAllChildren().FilterByPredicate([=](UWidget* Widget)
	{
		return Widget->IsA<UFineTabButton>();
	});
	if (ensure(Buttons.Num() == ButtonContainer->GetChildrenCount()))
	{
		for (auto Button : Buttons)
		{
			CleanUpButton(Cast<UFineTabButton>(Button));
		}
	}
	else
	{
		FW_FATAL("Tab bar contains non-tab button widget.");
	}
	ButtonContainer->ClearChildren();
}

void UFineTabBar::RefreshButtons(TArray<UFineTabButton*> Buttons)
{
	ClearButtons();
	for (auto Button : Buttons)
	{
		AddButton(Button);
	}
}

void UFineTabBar::NativeConstruct()
{
	Super::NativeConstruct();
	// Call prepare button for each button in button container.
	auto Buttons = ButtonContainer->GetAllChildren().FilterByPredicate([=](UWidget* Widget)
	{
		return Widget->IsA<UFineTabButton>();
	});
	if (ensure(Buttons.Num() == ButtonContainer->GetChildrenCount()))
	{
		for (auto Button : Buttons)
		{
			PrepareButton(Cast<UFineTabButton>(Button));
		}
	}
	else
	{
		FW_FATAL("Tab bar contains non-tab button widget.");
	}
}

void UFineTabBar::NativeDestruct()
{
	// Call clean up button for each button in button container.
	auto Buttons = ButtonContainer->GetAllChildren().FilterByPredicate([=](UWidget* Widget)
	{
		return Widget->IsA<UFineTabButton>();
	});
	if (ensure(Buttons.Num() == ButtonContainer->GetChildrenCount()))
	{
		for (auto Button : Buttons)
		{
			CleanUpButton(Cast<UFineTabButton>(Button));
		}
	}
	else
	{
		FW_FATAL("Tab bar contains non-tab button widget.");
	}
	Super::NativeDestruct();
}

void UFineTabBar::PrepareButton(UFineTabButton* Button)
{
	Button->OnTabButtonSelected.AddUniqueDynamic(this, &UFineTabBar::UpdateSelectedTab);
}

void UFineTabBar::CleanUpButton(UFineTabButton* Button)
{
	Button->OnTabButtonSelected.RemoveDynamic(this, &UFineTabBar::UpdateSelectedTab);
}

void UFineTabBar::UpdateSelectedTab(UFineTabButton* FineTabButton, bool IsSelected)
{
	if (!IsSelected) return;
	const auto ButtonIndex = ButtonContainer->GetChildIndex(FineTabButton);
	if (ButtonIndex >= 0 && ButtonIndex < ButtonContainer->GetChildrenCount())
	{
		SetSelectedIndex(ButtonIndex);
	}
}
