#include "Carla.h"

#include "Carla/Sensor/DriverUnitTest.h"

#include "Carla/Actor/ActorBlueprintFunctionLibrary.h"

#include <cstdlib>
#include <cstdio>

#include <iostream>
#include <time.h>

ADriverUnitTest::ADriverUnitTest(const FObjectInitializer &ObjectInitializer)
  : Super(ObjectInitializer)
{

  PrimaryActorTick.bCanEverTick = true;
  Rmax = 0.0f;
  Rmin = 0.0f;
 


}

FActorDefinition ADriverUnitTest::GetSensorDefinition()
{
  auto Definition = UActorBlueprintFunctionLibrary::MakeGenericSensorDefinition(
      TEXT("other"),
      TEXT("driver"));

  // create sensor attributes which can be configured by the user at Python API
  // Attribute 1: maximal driver reaction time
  // Attribute 2: minimal driver reaction time

  FActorVariation Reaction_max;
  Reaction_max.Id = TEXT("reaction_time_max");
  Reaction_max.Type = EActorAttributeType::Float;
  Reaction_max.RecommendedValues = { TEXT("5.0") };
  Reaction_max.bRestrictToRecommended = false;

  FActorVariation Reaction_min;
  Reaction_min.Id = TEXT("reaction_time_min");
  Reaction_min.Type = EActorAttributeType::Float;
  Reaction_min.RecommendedValues = { TEXT("0.0") };
  Reaction_min.bRestrictToRecommended = false;

  Definition.Variations.Append({Reaction_max, Reaction_min});
  
  return Definition;
}

void ADriverUnitTest::Set(const FActorDescription &Description)
{
  Super::Set(Description);

  const float Reaction_max = UActorBlueprintFunctionLibrary::RetrieveActorAttributeToFloat(
      "reaction_time_max",
      Description.Variations,
      5.0f);
  const float Reaction_min = UActorBlueprintFunctionLibrary::RetrieveActorAttributeToFloat(
      "reaction_time_min",
      Description.Variations,
      0.0f);

  Rmax = Reaction_max;
  Rmin = Reaction_min;

  std::cout<<"eingestellte maximale Reaktionszeit: "+std::to_string(Rmax)+"\n";
  std::cout<<"eingestellte minimale Reaktionszeit: "+std::to_string(Rmin)+"\n";
  
  
 
}

float ADriverUnitTest::ComputeReactionTime()
{
  
  const float reaction_range = Rmax - Rmin;
  const float reaction_time = Rmin + static_cast<float>(rand()) * static_cast<float>(reaction_range) / RAND_MAX;
  return reaction_time;
  
}

float ADriverUnitTest::ComputeBreaking()
{
  
  const float break_ratio = rand() / static_cast<float>(RAND_MAX);
  return break_ratio;

}




void ADriverUnitTest::SetOwner(AActor *Owner)
{
  Super::SetOwner(Owner);
}



void ADriverUnitTest::PostPhysTick(UWorld *World, ELevelTick TickType, float DeltaTime)
{
  TRACE_CPUPROFILER_EVENT_SCOPE(ADriverUnitTest::PostPhysTick);
  //ComputeBreaking();
  //ComputeReactionTime();
    

    auto Stream = GetDataStream(*this);
    Stream.Send(*this, ComputeReactionTime(), ComputeBreaking());
    
  
}

void ADriverUnitTest::BeginPlay()
{
  Super::BeginPlay();
  srand(time(NULL));
}