// (c) 2023 Pururum LLC. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Common/FineBaseWidget.h"
#include "UObject/Object.h"
#include "FineStackContentWidget.generated.h"

class UFineStackContentWidget;
DECLARE_DELEGATE_TwoParams(FOnNavigationBarVisibleChanged, UFineStackContentWidget*, bool);
DECLARE_DELEGATE_OneParam(FOnNavigationPopRequested, UFineStackContentWidget*);

/**
 * The base widget class for the content widget to be inserted into UFineStackNavigationWidget.
 */
UCLASS(Blueprintable, BlueprintType)
class FINEWIDGET_API UFineStackContentWidget : public UFineBaseWidget
{
	GENERATED_BODY()

public:
	UFineStackContentWidget(const FObjectInitializer& ObjectInitializer);

	/// If true, the content widget is the root of the navigation stack.
	UPROPERTY(BlueprintReadOnly, Category = "StackNavigation")
	bool bIsRoot;

	/// A factory method to return a widget to show at the right side of navigation bar of UFineStackNavigationWidget.
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "StackNavigation")
	UWidget* GetRightBarWidget();

	/// A factory method to return a widget to show at the left side of navigation bar of UFineStackNavigationWidget.
	///
	/// If no widget is given, a back button is shown.
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "StackNavigation")
	UWidget* GetLeftBarWidget();

	/// A factory method to return a widget to show at the center of navigation bar of UFineStackNavigationWidget.
	///
	/// Set `NavigationTitle` to null to use this method.
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "StackNavigation")
	UWidget* GetTitleBarWidget();

	/// A callback to be invoked when navigation bar visibility changes.
	FOnNavigationBarVisibleChanged OnNavigationBarVisibleChanged;

	/// Returns navigation title. If given, GetNavigationTitleWidget() is ignored.
	FORCEINLINE FText GetTitleText() const { return NavigationTitle; }

	/// Determines navigation bar needs to be shown or not.
	FORCEINLINE bool IsTitleBarVisible() const { return bTitleBarVisible; };

	/// Sets navigation title visibility.
	void SetNavigationBarVisible(const bool& bVisible);

	/// The preferred bar height.
	FORCEINLINE float GetPreferredBarHeight() const { return PreferredBarHeight; }

	/**
	 * Events
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "StackNavigation")
	void OnPrePush();
	void OnPrePush_Implementation();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "StackNavigation")
	void OnPostPush();
	void OnPostPush_Implementation();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "StackNavigation")
	void OnPrePop();
	void OnPrePop_Implementation();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "StackNavigation")
	void OnPostPop();
	void OnPostPop_Implementation();

	/// Invoking this will cause navigation stack to pop this content.
	FOnNavigationPopRequested OnNavigationPopRequested;

	/// Pop this content widget from the navigation stack if this is the top content in the stack.
	UFUNCTION(BlueprintCallable, Category = "StackNavigation")
	void Pop();
private:
	/// Navigation bar title.
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "StackNavigation", meta = (AllowPrivateAccess = "true"))
	FText NavigationTitle;

	/// Determines if the navigation bar is visible or not.
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "StackNavigation", meta = (AllowPrivateAccess = "true"))
	bool bTitleBarVisible;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "StackNavigation", meta = (AllowPrivateAccess = "true"))
	float PreferredBarHeight = 50.f;
};
