// (c) 2023 Pururum LLC. All rights reserved.


#include "..\..\Public\TabNavigation\FineTabNavigation.h"

#include "Common/FineContent.h"
#include "Common/FineTabButton.h"
#include "Components/WidgetSwitcher.h"
#include "TabNavigation/FineTabBar.h"

UFineTabNavigation::UFineTabNavigation(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
}

void UFineTabNavigation::AddContent(UFineContent* ContentWidget)
{
	check(IsValid(ContentWidget));
	ContentWidget->TabNavigationPtr = this;
	ContentSwitcher->AddChild(ContentWidget);
	TabBar->AddButton(ContentWidget->GetTabButton());
	if (ContentSwitcher->GetChildrenCount() == 1)
	{
		TabBar->SetSelectedIndex(0);
	}
}

void UFineTabNavigation::InsertContent(const int32& Index, UFineContent* ContentWidget)
{
	check(IsValid(ContentWidget));
	ContentWidget->TabNavigationPtr = this;
	ContentSwitcher->InsertChildAt(Index, ContentWidget);
	TabBar->InsertButtonAt(Index, ContentWidget->GetTabButton());
	if (ContentSwitcher->GetChildrenCount() == 1)
	{
		TabBar->SetSelectedIndex(0);
	}
}

void UFineTabNavigation::RemoveContent(UFineContent* ContentWidget)
{
	check(IsValid(ContentWidget));
	const auto Index = ContentSwitcher->GetChildIndex(ContentWidget);
	RemoveContentAt(Index);
}

void UFineTabNavigation::RemoveContentAt(int32 Index)
{
	if (Index >= 0 && Index < ContentSwitcher->GetChildrenCount())
	{
		const auto Content = Cast<UFineContent>(ContentSwitcher->GetChildAt(Index));
		Content->TabNavigationPtr = nullptr;
		ContentSwitcher->RemoveChildAt(Index);
		TabBar->RemoveButtonAt(Index);
		if (ContentSwitcher->GetChildrenCount() == 0)
		{
			TabBar->SetSelectedIndex(-1);
		}
	}
}

void UFineTabNavigation::NativeConstruct()
{
	Super::NativeConstruct();
	// Get array of fine content widgets from widget switcher.
	TArray<UWidget*> Widgets = ContentSwitcher->GetAllChildren();
	// For each fine content widget, get tab button from it and add it to tab bar.
	TArray<UFineTabButton*> Buttons;
	for (const auto Widget : Widgets)
	{
		const auto Content = Cast<UFineContent>(Widget);
		Content->TabNavigationPtr = this;
		if (ensure(IsValid(Content)))
		{
			auto Button = Content->GetTabButton();
			if (ensure(IsValid(Button)))
			{
				Buttons.Add(Button);
			}
		}
	}
	if (Buttons.Num() > 0)
	{
		TabBar->RefreshButtons(Buttons);
		const int32 SelectedIndex = ContentSwitcher->GetActiveWidgetIndex();
		TabBar->SetSelectedIndex(SelectedIndex);
	}

	TabBar->OnSelectionChanged.AddDynamic(this, &UFineTabNavigation::OnTabSelectionChanged);

	// for each content classes, create a widget and call AddContent with it.
	for (const auto ContentClass : ContentClasses)
	{
		const auto Content = CreateWidget<UFineContent>(this, ContentClass);
		if (ensure(IsValid(Content)))
		{
			AddContent(Content);
		}
	}
}

void UFineTabNavigation::NativeDestruct()
{
	// for each content widget, clear tab navigation ptr.
	TArray<UWidget*> Widgets = ContentSwitcher->GetAllChildren();
	for (const auto Widget : Widgets)
	{
		const auto Content = Cast<UFineContent>(Widget);
		if (ensure(IsValid(Content)))
		{
			Content->TabNavigationPtr = nullptr;
		}
	}
	TabBar->OnSelectionChanged.RemoveDynamic(this, &UFineTabNavigation::OnTabSelectionChanged);
	TabBar->ClearButtons();
	Super::NativeDestruct();
}

void UFineTabNavigation::OnTabSelectionChanged(int32 NewIndex)
{
	TabBar->SetSelectedIndex(NewIndex);
	ContentSwitcher->SetActiveWidgetIndex(NewIndex);
	const auto Widget = ContentSwitcher->GetActiveWidget();
	if (ensure(IsValid(Widget)))
	{
		const auto Content = Cast<UFineContent>(Widget);
		if (ensure(IsValid(Content)))
		{
			Content->OnTabSelected();
		}
	}
}
