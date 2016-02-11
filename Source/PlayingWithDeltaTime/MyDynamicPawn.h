// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "MyDynamicPawn.generated.h"

UCLASS()
class PLAYINGWITHDELTATIME_API AMyDynamicPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyDynamicPawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	//Create a UPROPERTY for ourVisible piece
	UPROPERTY(EditAnywhere)
	USceneComponent*OurVisibleComponent;

	UPROPERTY(EditAnywhere)
	UCameraComponent* OurCamera;

	UPROPERTY(EditAnywhere)
		bool bGrowing;

	UPROPERTY(EditAnywhere)
		bool bZooming;

	UPROPERTY(EditAnywhere)
		float zoomRate = 10.0f;
    
    UPROPERTY(EditAnywhere)
    FVector CurrentVelocity;
    
    UFUNCTION()
    void OnMouseOver();
    
    void ToggleZooming();
    
    void ChangeZoomLeft(float ZoomValue, float Distance);
    void ChangeZoomRight(float ZoomValue, float Distance);
    
    void Move_XAxis(float AxisValue);
    
    void Move_YAxis(float AxisValue);
    
    int32 CountdownTime;
    UTextRenderComponent* CountdownText;
    void UpdateTimerDisplay();
    
    void AdvanceTimer();
    void CountdownHasFinished();
    FTimerHandle CountdownTimerHandle;
    
    

	
};
