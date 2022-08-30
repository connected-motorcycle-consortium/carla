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
  // Attribute 1: maximal driver reaction time (float)
  // Attribute 2: minimal driver reaction time (float)
  // Attribute 3: sensor_tick (float)
  // Attribute 4: if the reaction time should be randomized or not (bool)

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

  FActorVariation Tick;
  Tick.Id = TEXT("sensor_tick");
  Tick.Type = EActorAttributeType::Float;
  Tick.RecommendedValues = { TEXT("0.0") };
  Tick.bRestrictToRecommended = false;

  FActorVariation IsRandom;
  IsRandom.Id = TEXT("reaction_time_randomize");
  IsRandom.Type = EActorAttributeType::Bool;
  IsRandom.RecommendedValues = { TEXT("true") };
  IsRandom.bRestrictToRecommended = false;

  FActorVariation Reaction_Fix;
  Reaction_Fix.Id = TEXT("reaction_time_Fix");
  Reaction_Fix.Type = EActorAttributeType::Float;
  Reaction_Fix.RecommendedValues = { TEXT("0.0") };
  Reaction_Fix.bRestrictToRecommended = false;


  Definition.Variations.Append({Reaction_max, Reaction_min,Tick,IsRandom,Reaction_Fix});
  
  return Definition;
}

void ADriverUnitTest::Set(const FActorDescription &Description)
{
  Super::Set(Description);

  const bool reaction_random = UActorBlueprintFunctionLibrary::RetrieveActorAttributeToBool(
      "reaction_time_randomize",
      Description.Variations,
      true);

  const float Reaction_max = UActorBlueprintFunctionLibrary::RetrieveActorAttributeToFloat(
      "reaction_time_max",
      Description.Variations,
      5.0f);
  const float Reaction_min = UActorBlueprintFunctionLibrary::RetrieveActorAttributeToFloat(
      "reaction_time_min",
      Description.Variations,
      0.0f);
  const float Reaction_Fix = UActorBlueprintFunctionLibrary::RetrieveActorAttributeToFloat(
      "reaction_time_Fix",
      Description.Variations,
      0.0f);


  Rmax = Reaction_max;
  Rmin = Reaction_min;
  reaction_rand = reaction_random;
  reaction_fix = Reaction_Fix;
  
  
 
}

float ADriverUnitTest::ComputeReactionTime()
{
    float reaction_time;

    if (reaction_rand == false)
    {
        reaction_time = reaction_fix;
    }
    else
    {
        const float reaction_range = Rmax - Rmin;
        reaction_time = Rmin + static_cast<float>(rand()) * static_cast<float>(reaction_range) / RAND_MAX;
    }
   
  return reaction_time;
  
}

float ADriverUnitTest::ComputeBraking()
{
  float brake_ratio;
  if (reaction_rand == false && reaction_fix == 0.0f)
  {
      brake_ratio = 0.0f;
  }
  else
  {
      brake_ratio = rand() / static_cast<float>(RAND_MAX);
  }
  
  return brake_ratio;

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
    Stream.Send(*this, ComputeReactionTime(), ComputeBraking());
    
  
}

void ADriverUnitTest::BeginPlay()
{
  Super::BeginPlay();
  srand(time(NULL));
}