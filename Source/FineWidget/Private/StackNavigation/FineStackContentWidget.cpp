// (c) 2023 Pururum LLC. All rights reserved.


#include "StackNavigation/FineStackContentWidget.h"

#include "FineWidgetLog.h"

UFineStackContentWidget::UFineStackContentWidget(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	bTitleBarVisible = true;
	bIsRoot = false;
}

void UFineStackContentWidget::SetNavigationBarVisible(const bool& bVisible)
{
	if (bTitleBarVisible == bVisible) return;
	bTitleBarVisible = bVisible;
	OnNavigationBarVisibleChanged.ExecuteIfBound(this, bVisible);
}

void UFineStackContentWidget::OnPrePush_Implementation()
{
	FW_VERBOSE("A stack content will be pushed.");
}

void UFineStackContentWidget::OnPostPush_Implementation()
{
	FW_VERBOSE("A stack content has been pushed.");
}

void UFineStackContentWidget::OnPrePop_Implementation()
{
	FW_VERBOSE("A stack content will be popped.");
}

void UFineStackContentWidget::OnPostPop_Implementation()
{
	FW_VERBOSE("A stack content has been popped.");
}

void UFineStackContentWidget::Pop()
{
	OnNavigationPopRequested.ExecuteIfBound(this);
}

UFineStackNavigationWidget* UFineStackContentWidget::GetStackNavigation()
{
	// Return strong ptr if weak stack navigation ptr is valid.
	if (StackNavigationPtr.IsValid())
	{
		return StackNavigationPtr.Get();
	}

	return nullptr;
}
