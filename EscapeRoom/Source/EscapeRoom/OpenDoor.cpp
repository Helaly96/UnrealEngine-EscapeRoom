// Fill out your copyright notice in the Description page of Project Settings.

#define OUT
#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/Classes/Engine/World.h"



// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
    Owner = GetOwner();

	// ...
}

void UOpenDoor::Opening()
{
	OnOpen.Broadcast();
}

void UOpenDoor::Closing()
{
	OnClose.Broadcast();
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	if (PressurePlate==nullptr)
	{
	UE_LOG(LogTemp,Warning,TEXT("Error , No pressure plate"));

	}
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if ( GetTotalMassActorsOnWeight() > Weight_to_open)
	{
		Opening();
		last_time_opened = GetWorld()->GetTimeSeconds() ;
	}

	else
	{
		Closing();
	}


	
}

float UOpenDoor::GetTotalMassActorsOnWeight()
{
	float TotalMass=0;
	TArray<AActor*> ActorsThatOverlap;
	if (PressurePlate == nullptr){return 0 ;}
	PressurePlate->GetOverlappingActors(OUT ActorsThatOverlap);

	for (const auto& StandingOnit:ActorsThatOverlap)
	{
		TotalMass += (StandingOnit)->FindComponentByClass<UPrimitiveComponent>()->GetMass();

		UE_LOG(LogTemp,Warning,TEXT("Overlapping with %s"),*StandingOnit->GetName());
	}

	return TotalMass;

}

