// (c) 2023 Pururum LLC. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Common/FineBaseWidget.h"
#include "UObject/Object.h"
#include "FineContent.generated.h"

class UFineButton;
class UFineTabButton;
class UFineStackNavigation;
class UFineContent;
DECLARE_DELEGATE_TwoParams(FOnNavigationBarVisibleChanged, UFineContent*, bool);
DECLARE_DELEGATE_OneParam(FOnNavigationPopRequested, UFineContent*);

/**
 * The base widget class for the content widget to be inserted into UFineStackNavigationWidget.
 */
UCLASS(Blueprintable, BlueprintType)
class FINEWIDGET_API UFineContent : public UFineBaseWidget
{
	GENERATED_BODY()

public:
	UFineContent(const FObjectInitializer& ObjectInitializer);

	/// If true, the content widget is the root of the navigation stack.
	UPROPERTY(BlueprintReadOnly, Category = "StackNavigation")
	bool bIsRoot;

	/// A factory method to return a widget to show at the right side of navigation bar of UFineStackNavigationWidget.
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "StackNavigation")
	UWidget* GetRightBarWidget();

	/// A factory method to return a widget to show at the left side of navigation bar of UFineStackNavigationWidget.
	///
	/// If no widget is given, a back button is shown.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "StackNavigation")
	UWidget* GetLeftBarWidget();
	UWidget* GetLeftBarWidget_Implementation();

	/// A factory method to return a widget to show at the center of navigation bar of UFineStackNavigationWidget.
	///
	/// Set `NavigationTitle` to null to use this method.
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "StackNavigation")
	UWidget* GetTitleBarWidget();

	/// A factory method to return a tab button widget to show in tab tab for tab navigation.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "StackNavigation")
	UFineTabButton* GetTabButton();
	UFineTabButton* GetTabButton_Implementation();

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
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "StackNavigation")
	void OnTabSelected();
	void OnTabSelected_Implementation();

	/// Invoking this will cause navigation stack to pop this content.
	FOnNavigationPopRequested OnNavigationPopRequested;

	/// Pop this content widget from the navigation stack if this is the top content in the stack.
	UFUNCTION(BlueprintCallable, Category = "StackNavigation")
	void Pop();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "StackNavigation")
	UFineStackNavigation* GetStackNavigation();

protected:
	virtual void NativeDestruct() override;
private:
	/// Navigation bar title.
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "StackNavigation", meta = (AllowPrivateAccess = "true"))
	FText NavigationTitle;

	/// Determines if the navigation bar is visible or not.
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "StackNavigation", meta = (AllowPrivateAccess = "true"))
	bool bTitleBarVisible;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "StackNavigation", meta = (AllowPrivateAccess = "true"))
	float PreferredBarHeight = 50.f;

	/// The class of the back button to be shown in the navigation bar.
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "StackNavigation", meta = (AllowPrivateAccess = "true"))
	TSoftClassPtr<UFineButton> BackButtonClass;
	/// The class of the tab button to be shown in the tab navigation.
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "StackNavigation", meta = (AllowPrivateAccess = "true"))
	TSoftClassPtr<UFineTabButton> TabButtonClass;

	UPROPERTY(BlueprintReadOnly, Category="StackNavigation", meta = (AllowPrivateAccess = "true"))
	UFineButton* BackButton;
	UPROPERTY(BlueprintReadOnly, Category="TabNavigation", meta = (AllowPrivateAccess = "true"))
	UFineTabButton* TabButton;
private:
	friend class UFineStackNavigation;
	TWeakObjectPtr<UFineStackNavigation> StackNavigationPtr;

	friend class UFineTabNavigation;
	TWeakObjectPtr<UFineTabNavigation> TabNavigationPtr;
};
