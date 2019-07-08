// Fill out your copyright notice in the Description page of Project Settings.

#define OUT  

#include "Grabber.h"
#include "GameFramework/Actor.h"
#include "Engine/Classes/Engine/World.h"
#include "Classes/PhysicsEngine/PhysicsHandleComponent.h"
#include  "Engine/Classes/Components/PrimitiveComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	//Find Componeny by class basically searches the owner for Objects with that class name
	PhyHandler = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	inHandler = GetOwner()->FindComponentByClass<UInputComponent>();
	FindPhyHandleComponent();
	SetupInputComponent();
	
}

//find the physics handler
void  UGrabber::FindPhyHandleComponent()
{
	if (PhyHandler == nullptr)
	{
		UE_LOG(LogTemp,Warning,TEXT("Error No Phy handler for %s"),*GetOwner()->GetName());

	}
}

//inits the input component and binds the action after looking for inputhandler
void UGrabber::SetupInputComponent()
{
	if (inHandler)
	{
		// it bind the action in the Editor when the key is pressed to the fun grab
		// just like the signal and slot in Qt.
		inHandler->BindAction("Grab",IE_Pressed,this,&UGrabber::Grab);
		inHandler->BindAction("Grab",IE_Released,this,&UGrabber::release);
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("Error No Input handler for %s"),*GetOwner()->GetName());

	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp,Warning,TEXT("We are grabbing!"));
	auto Hit = GetFirstItemInReach();
	auto Component_to_grab= Hit.GetComponent();
	auto ActorHit=Hit.GetActor();

	if (ActorHit)
	{
	//we grab from here
	PhyHandler->GrabComponentAtLocation(Component_to_grab,NAME_None,Component_to_grab->GetOwner()->GetActorLocation());
	}
}

void UGrabber::release()
{
	UE_LOG(LogTemp,Warning,TEXT("we released"));

	//realease the phy handler
	PhyHandler->ReleaseComponent();

}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(PhyHandler==nullptr)
	{
		return;
	}
	if(PhyHandler->GrabbedComponent)
	{
		//where to put the phy handler in the grabbed component
		GetTraceEnd();
		PhyHandler->SetTargetLocation(TraceEnd);
	}

}

void UGrabber::GetPlayerViewPointRotationandLocation()
{
	//this is an out funcution, which modifies its parameters
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		 OUT PlayerViewPointLocation,
		 OUT PlayerViewPointRotation);
}

void UGrabber::GetTraceEnd()
{
   GetPlayerViewPointRotationandLocation();
   TraceEnd = PlayerViewPointLocation + Reach * PlayerViewPointRotation.Vector();

}

FHitResult UGrabber::GetFirstItemInReach()
{
	GetTraceEnd();
	DrawDebugLine( GetWorld(),PlayerViewPointLocation,TraceEnd,FColor(255,0,0),false,0,0,10);

	//b2olo el params of the query, false de bt3abar 3an howa complex wala simple , GetOwner() de for what actors we will ignore
	FCollisionQueryParams TraceParams(FName(TEXT("")),false,GetOwner());
	//LineTrace
	FHitResult LineTraceHit;
	///gets the first object our line collides with
	GetWorld()->LineTraceSingleByObjectType(
		OUT LineTraceHit,PlayerViewPointLocation,TraceEnd , FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
	TraceParams) ; 
	//logging what we hit
	AActor * ActorHitted = LineTraceHit.GetActor();
	if (ActorHitted)
	{
		FString Name = ActorHitted->GetName();
		UE_LOG(LogTemp,Warning,TEXT("Collided with %s"),*Name);

	}
	return LineTraceHit ; 
}

