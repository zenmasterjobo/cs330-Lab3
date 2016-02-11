// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayingWithDeltaTime.h"
#include "MyDynamicPawn.h"


// Sets default values
AMyDynamicPawn::AMyDynamicPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Lets pawn be controlled by default player
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	//Create a dummy root component we can attach things to.
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	//create a camera and a visible object
	OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("OurCamera"));
	OurVisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OurVisibleComponent"));

	// Attach our camera and visible object to our root component. Offset and rotate the camera.
	OurCamera->AttachTo(RootComponent);
	OurCamera->SetRelativeLocation(FVector(-250.0f, 0.0f, 250.0f));
	OurCamera->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
	OurVisibleComponent->AttachTo(RootComponent);
    
    
    //COUNTDOWN Values
    CountdownText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("CountdownNumber"));
    CountdownText->SetHorizontalAlignment(EHTA_Center);
    CountdownText->SetWorldSize(150.0f);
    CountdownText->AttachTo(RootComponent);
    CountdownText->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
    CountdownText->SetRelativeRotation(FRotator(0, -180, 0));
    
    CountdownTime = 3;
    
    OnBeginCursorOver.AddDynamic(this, & AMyDynamicPawn::OnMouseOver);
    

}

//UPDATE Timer Display
void AMyDynamicPawn::UpdateTimerDisplay()
{
    CountdownText->SetText(FString::FromInt(FMath::Max(CountdownTime, 0)));
}

//Advance Timer
void AMyDynamicPawn::AdvanceTimer()
{
    --CountdownTime;
    UpdateTimerDisplay();
    if (CountdownTime < 1)
    {
        //Were done counting down, so stop running the timer.
        GetWorldTimerManager().ClearTimer(CountdownTimerHandle);
        CountdownHasFinished();
    }
}

//Countdown has finished
void AMyDynamicPawn::CountdownHasFinished()
{
    //Change to a special readout
    CountdownText->SetText(TEXT("GO!"));
}



// Called when the game starts or when spawned
void AMyDynamicPawn::BeginPlay()
{
	Super::BeginPlay();
    UpdateTimerDisplay();
    GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &AMyDynamicPawn::AdvanceTimer, 1.0f, true);
    
    if(GetWorld() )
    {
        APlayerController *myPlayerController = GetWorld()->GetFirstPlayerController();
        
        if(myPlayerController != NULL)
        {
            myPlayerController->bShowMouseCursor = true;
            myPlayerController ->bEnableMouseOverEvents = true;
        }
    }
	
}

// Creating function for ToggleZooming

void AMyDynamicPawn::ToggleZooming()
{
    bZooming = !bZooming;
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, FString::Printf(TEXT("Reversing direction!")));
    }
}


//Function for ChangeZoomRateRight
void AMyDynamicPawn::ChangeZoomRight(float ZoomRate, float Distance)
{
    ZoomRate = FMath::Clamp(ZoomRate, 0.0f, 200.0f) * 5.0f *Distance;
}

//Function for ChangeZoomRateRight
void AMyDynamicPawn::ChangeZoomLeft(float ZoomRate, float Distance)
{
    //zoomRate = FMath::Clamp(ZoomRate, 0.0f, 200.0f) * 0.5f;
    
    //if (zoomRate == 0);
    //{/
        
//}
}

//Create function for Move_XAxis

void AMyDynamicPawn::Move_XAxis(float AxisValue)
{
    //Move at 100 units per second forward or backward
    CurrentVelocity.X = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 100.0f;

    
}

//Create function for Move_YAxis
void AMyDynamicPawn::Move_YAxis(float AxisValue)
{
    //Move at 100 units per second forward or backward
    CurrentVelocity.Y = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 100.0f;
    
    
}


//MOUSE OVER
void AMyDynamicPawn::OnMouseOver()
{
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, FString::Printf(TEXT("MOUSE OVER")));
    }
}



// Called every frame
void AMyDynamicPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	float CurrentScale = OurVisibleComponent->GetComponentScale().X;
	
	if (bGrowing)
	{
		// Grow to double size over the course of one second
		CurrentScale += DeltaTime;
	}
	else
	{
		// Shrink half as fast as we grow
		CurrentScale -= (DeltaTime * 0.5f);
	}
	// Make sure we never drop below our starting size, or increase past double size.
	CurrentScale = FMath::Clamp(CurrentScale, 1.0f, 2.0f);
	OurVisibleComponent->SetWorldScale3D(FVector(CurrentScale));
	
	
	//Store the location of the camera
	FVector cameraLoc = OurCamera->GetRelativeTransform().GetLocation();

	//Store the location of the Actor
	FVector objectLoc = OurVisibleComponent->GetRelativeTransform().GetLocation();

	//Get direction of the Zoom
	FVector direction = (cameraLoc - objectLoc).GetSafeNormal();

	//Distance in vector
	//FVector distancef = ( )

	if (bZooming)
	{
		cameraLoc += direction*zoomRate*DeltaTime;
	}
	else
	{
		cameraLoc -= direction*zoomRate*DeltaTime;
	}
    
   
    
    //Handle movement
    if(!CurrentVelocity.IsZero())
    {
        FVector NewLocation = objectLoc + (CurrentVelocity * DeltaTime);
        OurVisibleComponent ->SetRelativeLocation(NewLocation);
    }

	//The distance between the camera and the actor
	float distance = (cameraLoc - objectLoc).Size();
	distance = FMath::Clamp(distance, 300.0f, 1000.0f);
	OurCamera->SetWorldScale3D(FVector(distance));
	cameraLoc = objectLoc + distance *direction;
    
    /*
    //Pass the Zoomrate to the function
    ChangeZoomRight(zoomRate, distance);
	*/
    
    //Set new Location of the Camera
    OurCamera->SetRelativeLocation(cameraLoc);
    
    //Set new Rotation of the Camera
    OurCamera->SetRelativeRotation((-1*direction).Rotation());



	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("CurrentScale in Tick: %f"), CurrentScale));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("bGrowing in Tick: %d"), bGrowing));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("distance in Tick: %f"), distance));
	}

}

// Called to bind functionality to input
void AMyDynamicPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
    InputComponent->BindAction("Zoom", IE_Pressed, this, &AMyDynamicPawn::ToggleZooming);
    InputComponent->BindAxis("ChangeZoomRateDecrease", this, &AMyDynamicPawn::ChangeZoomLeft);
    InputComponent->BindAxis("ChangeZoomRateIncrease", this, &AMyDynamicPawn::ChangeZoomRight);
    InputComponent->BindAxis("MoveX", this, &AMyDynamicPawn::Move_XAxis);
    InputComponent->BindAxis("MoveY", this, &AMyDynamicPawn::Move_YAxis);

}

