// (c) 2023 Pururum LLC. All rights reserved.


#include "Common/FineContent.h"

#include "FineWidgetLog.h"
#include "Common/FineButton.h"
#include "Common/FineTabButton.h"

UFineContent::UFineContent(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	bTitleBarVisible = true;
	bIsRoot = false;
	BackButtonClass = UFineButton::StaticClass();
	TabButtonClass = UFineTabButton::StaticClass();
}

UWidget* UFineContent::GetRightBarWidget_Implementation()
{
	if (bIsRoot)
	{
		return nullptr;
	}

	// if backbutton is already valid, return it.
	if (IsValid(BackButton))
	{
		return BackButton;
	}

	// create back button widget with the backbutton class.
	BackButton = CreateWidget<UFineButton>(this, BackButtonClass.LoadSynchronous());
	if (IsValid(BackButton))
	{
		BackButton->OnClicked.AddDynamic(this, &UFineContent::Pop);
	}
	
	return BackButton;
}

UFineTabButton* UFineContent::GetTabButton_Implementation()
{
	// if tab button is already valid, return it.
	if (IsValid(TabButton))
	{
		return TabButton;
	}

	// create tab button widget with the tabbutton class.
	TabButton = CreateWidget<UFineTabButton>(this, TabButtonClass.LoadSynchronous());
	return TabButton;
}

void UFineContent::SetNavigationBarVisible(const bool& bVisible)
{
	if (bTitleBarVisible == bVisible) return;
	bTitleBarVisible = bVisible;
	OnNavigationBarVisibleChanged.ExecuteIfBound(this, bVisible);
}

void UFineContent::OnPrePush_Implementation()
{
	FW_VERBOSE("A stack content will be pushed.");
}

void UFineContent::OnPostPush_Implementation()
{
	FW_VERBOSE("A stack content has been pushed.");
}

void UFineContent::OnPrePop_Implementation()
{
	FW_VERBOSE("A stack content will be popped.");
}

void UFineContent::OnPostPop_Implementation()
{
	FW_VERBOSE("A stack content has been popped.");
}

void UFineContent::OnTabSelected_Implementation()
{
	FW_VERBOSE("A tab content has been selected.");
}

void UFineContent::Pop()
{
	OnNavigationPopRequested.ExecuteIfBound(this);
}

UFineStackNavigation* UFineContent::GetStackNavigation()
{
	// Return strong ptr if weak stack navigation ptr is valid.
	if (StackNavigationPtr.IsValid())
	{
		return StackNavigationPtr.Get();
	}

	return nullptr;
}

void UFineContent::NativeDestruct()
{
	// unbind back button event and clear.
	if (IsValid(BackButton))
	{
		BackButton->OnClicked.RemoveDynamic(this, &UFineContent::Pop);
		BackButton = nullptr;
	}
	TabButton = nullptr;
	Super::NativeDestruct();
}
