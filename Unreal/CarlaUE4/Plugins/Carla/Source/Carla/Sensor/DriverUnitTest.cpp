// Copyright (c) 2022 Connected Motorcycle Consortium (CMC).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

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
  // Attribute 1: maximum of driver reaction time (float)
  // Attribute 2: minimum of driver reaction time (float)
  // Attribute 3: sensor_tick (float)
  // Attribute 4: if the reaction time should be randomly generated or not (bool)
  // Attribute 5: if reaction time is not generated randomly, a fixed reaction time should be set (float)
  // Attribute 6: if the braking ratio should be randomly generated or not (bool)
  // Attribute 7: if braking ratio is not generated randomly, a fixed braking ratio should be set (float)

  // Attribute 1
  FActorVariation ReactionTimeMax;
  ReactionTimeMax.Id = TEXT("reaction_time_max");
  ReactionTimeMax.Type = EActorAttributeType::Float;
  ReactionTimeMax.RecommendedValues = { TEXT("5.0") };
  ReactionTimeMax.bRestrictToRecommended = false;

  // Attribute 2
  FActorVariation ReactionTimeMin;
  ReactionTimeMin.Id = TEXT("reaction_time_min");
  ReactionTimeMin.Type = EActorAttributeType::Float;
  ReactionTimeMin.RecommendedValues = { TEXT("0.0") };
  ReactionTimeMin.bRestrictToRecommended = false;
  
  // Attribute 3
  FActorVariation Tick;
  Tick.Id = TEXT("sensor_tick");
  Tick.Type = EActorAttributeType::Float;
  Tick.RecommendedValues = { TEXT("0.0") };
  Tick.bRestrictToRecommended = false;

  // Attribute 4
  FActorVariation ReactionTimeIsRandom;
  ReactionTimeIsRandom.Id = TEXT("reaction_time_random");
  ReactionTimeIsRandom.Type = EActorAttributeType::Bool;
  ReactionTimeIsRandom.RecommendedValues = { TEXT("true") };
  ReactionTimeIsRandom.bRestrictToRecommended = false;

  //Attribute 5
  FActorVariation ReactionTimeFix;
  ReactionTimeFix.Id = TEXT("reaction_time_fix");
  ReactionTimeFix.Type = EActorAttributeType::Float;
  ReactionTimeFix.RecommendedValues = { TEXT("0.0") };
  ReactionTimeFix.bRestrictToRecommended = false;

  // Attribute 6
  FActorVariation BrakingRatioIsRandom;
  BrakingRatioIsRandom.Id = TEXT("braking_ratio_random");
  BrakingRatioIsRandom.Type = EActorAttributeType::Bool;
  BrakingRatioIsRandom.RecommendedValues = { TEXT("true") };
  BrakingRatioIsRandom.bRestrictToRecommended = false;

  //Attribute 7
  FActorVariation BrakingRatioFix;
  BrakingRatioFix.Id = TEXT("braking_ratio_fix");
  BrakingRatioFix.Type = EActorAttributeType::Float;
  BrakingRatioFix.RecommendedValues = { TEXT("0.0") };
  BrakingRatioFix.bRestrictToRecommended = false;


  Definition.Variations.Append({ReactionTimeMax, ReactionTimeMin, Tick, ReactionTimeIsRandom, ReactionTimeFix, BrakingRatioIsRandom, BrakingRatioFix});
  
  return Definition;
}

void ADriverUnitTest::Set(const FActorDescription &Description)
{
  Super::Set(Description);

  const bool ReactionRandom = UActorBlueprintFunctionLibrary::RetrieveActorAttributeToBool(
      "reaction_time_random",
      Description.Variations,
      true);

  const float ReactionMax = UActorBlueprintFunctionLibrary::RetrieveActorAttributeToFloat(
      "reaction_time_max",
      Description.Variations,
      5.0f);
  const float ReactionMin = UActorBlueprintFunctionLibrary::RetrieveActorAttributeToFloat(
      "reaction_time_min",
      Description.Variations,
      0.0f);
  const float ReactionFix = UActorBlueprintFunctionLibrary::RetrieveActorAttributeToFloat(
      "reaction_time_fix",
      Description.Variations,
      0.0f);

  const bool BrakingRandom = UActorBlueprintFunctionLibrary::RetrieveActorAttributeToBool(
      "braking_ratio_random",
      Description.Variations,
      true);

  const float BrakingFix = UActorBlueprintFunctionLibrary::RetrieveActorAttributeToFloat(
      "braking_ratio_fix",
      Description.Variations,
      0.0f);


  Rmax = ReactionMax ;
  Rmin = ReactionMin;
  reaction_random = ReactionRandom;
  reaction_fix = ReactionFix;
  braking_random = BrakingRandom;
  braking_fix = BrakingFix;
  
  
 
}

float ADriverUnitTest::ComputeReactionTime()
{
  float reaction_time;

  if (reaction_random == false)
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

  if (braking_random == false)
  {
    brake_ratio = braking_fix;
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