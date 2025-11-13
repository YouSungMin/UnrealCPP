// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ResourceBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALCPP_API UResourceBarWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void RefreshWidget(float InCurrent, float InMax);
protected:
	virtual void NativePreConstruct() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resource", meta = (BindWidget))
	class UProgressBar* Bar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resource", meta = (BindWidget))
	TObjectPtr<class UTextBlock> Current;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resource", meta = (BindWidget))
	TObjectPtr<class UTextBlock> Max;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
	FLinearColor BarFillColor = FLinearColor::Red;
	
};
