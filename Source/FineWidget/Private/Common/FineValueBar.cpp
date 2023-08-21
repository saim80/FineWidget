// (c) 2023 Pururum LLC. All rights reserved.


#include "Common/FineValueBar.h"

#include "Components/Image.h"

UFineValueBar::UFineValueBar(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	AnimationDuration = 0.5f;
	AnimationInterval = 1.f / 25.f;
}

void UFineValueBar::SetPercent(float InPercent, bool bAnimate)
{
	// if in percent is different from current percent
	if (FMath::IsNearlyEqual(Percent, InPercent, UE_KINDA_SMALL_NUMBER) == false)
	{
		// if animation is not ended
		if (!IsAnimationEnded())
		{
			// end animation
			EndAnimation();
		}
		// set percent
		Percent = InPercent;
		// if animation is needed
		if (bAnimate)
		{
			// begin animation
			BeginAnimation(Percent, InPercent);
		}
		else
		{
			// update bar image
			UpdateBarImage();
		}
	}
}

void UFineValueBar::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (BarImage)
	{
		BarImage->SetRenderTransformPivot({0.0f, 0.5f});
	}
}

void UFineValueBar::NativeConstruct()
{
	Super::NativeConstruct();
	if (BarImage)
	{
		BarImage->SetRenderTransformPivot({0.0f, 0.5f});
	}
}

void UFineValueBar::BeginAnimation(const float& OldPercent, const float& NewPercent)
{
	// set source and target percent
	SourcePercent = OldPercent;
	TargetPercent = NewPercent;
	// reset elapsed time
	AnimationElapsed = 0.0f;
	// set timer
	GetWorld()->GetTimerManager().SetTimer(AnimationTimerHandle, this, &UFineValueBar::UpdateBarImage,
	                                       AnimationInterval, true);
}

void UFineValueBar::EndAnimation()
{
	// invalidate the timer.
	GetWorld()->GetTimerManager().ClearTimer(AnimationTimerHandle);
}

bool UFineValueBar::IsAnimationEnded() const
{
	// if the elapsed time is greater than the duration, the animation is ended.
	return AnimationElapsed >= AnimationDuration;
}

void UFineValueBar::UpdateBarImage()
{
	if (!BarImage)
	{
		return;
	}
	float NewScale = 0.0f;
	// if animation timer is valid, calculate percent from elapsed time
	if (GetWorld()->GetTimerManager().IsTimerActive(AnimationTimerHandle))
	{
		AnimationElapsed += AnimationInterval;
		const auto ClampedAlpha = FMath::Clamp(AnimationElapsed / AnimationDuration, 0.0f, 1.0f);
		NewScale = FMath::Lerp(SourcePercent, TargetPercent, ClampedAlpha);
	}
	else
	{
		// not animating, set percent directly
		NewScale = Percent;
	}
	// set scale
	BarImage->SetRenderScale({NewScale, 1.0f});
	// if animation timer valid and animation is ended, call end animation
	if (IsAnimationEnded())
	{
		EndAnimation();
	}
}
