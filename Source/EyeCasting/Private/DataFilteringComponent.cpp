#include "DataFilteringComponent.h"

DataFilteringComponent::UDataFilteringComponent()
{
    setFilteringMode("Average");
}

DataFilteringComponent::UDataFilteringComponent(const FObjectInitializer &ObjectInitializer)
    : Super(ObjectInitializer)
{
    setFilteringMode("Average");
}

DataFilteringComponent::~UDataFilteringComponent()
{
    // Empty destructor
}

void DataFilteringComponent::setGazeDataArray(FEyeTrackerGazeData[] gazeDataArray)
{
    this->gazeDataArray = gazeDataArray;
    switch (this->filteringMode)
    {
    case "Average":
        this->filteredGazeVector = averageLastXVectors(this->gazeDataArray);
        break;
    case "Median":
        this->filteredGazeVector = medianLastXVectors(this->gazeDataArray);
        break;
    case "Exclude Outlier":
        this->filteredGazeVector = excludeOutliers(this->gazeDataArray);
        break;
    case "Exclude Most Recent":
        this->filteredGazeVector = excludeMostRecent(this->gazeDataArray);
        break;
    default:
        this->filteredGazeVector = averageLastXVectors(this->gazeDataArray);
        break;
    }

    clearGazeDataArray();
}

bool DataFilteringComponent::setFilteringMode(FString filteringMode)
{
    switch (filteringMode)
    {
    case "Average":
        this->filteringMode = filteringMode;
        return true;
        break;
    case "Median":
        this->filteringMode = filteringMode;
        return true;
        break;
    case "Exclude Outlier":
        this->filteringMode = filteringMode;
        return true;
        break;
    case "Exclude Most Recent":
        this->filteringMode = filteringMode;
        return true;
        break;
    default:
        this->filteringMode = "Average";
        return false;
        break;
    }

    return false;
}

FVector DataFilteringComponent::getFilteredGazeVector()
{
    return this->filteredGazeVector;
}

FVector DataFilteringComponent::averageLastXVectors(const FVector[] vector)
{
    FVector averageVector = FVector(0);
    auto arraySize = std::size(vector);

    for (int i = 0; i < arraySize; i++)
    {
        averageVector += vector[i];
    }

    averageVector /= arraySize;
    return averageVector;
}

FVector DataFilteringComponent::medianLastXVectors(const FVector[] vector)
{
    FVector medianVector = FVector(0);
    auto arraySize = std::size(vector);

    if (arraySize % 2 == 0)
    {
        medianVector = (vector[arraySize / 2 - 1] + vector[arraySize / 2]) / 2;
    }
    else
    {
        medianVector = vector[arraySize / 2];
    }

    return medianVector;
}

FVector DataFilteringComponent::excludeOutlier(const FVector[] vector)
{
    FVector excludedOutlierVector = FVector(0);
    auto arraySize = std::size(vector);

    std::sort(vector, vector + arraySize);

    if (arraySize < 3)
    {
        return averageLastXVectors(vector);
    }

    for (int i = 1; i < arraySize - 1; i++)
    {
        excludedOutlierVector += vector[i];
    }

    excludedOutlierVector /= arraySize - 2;
    return excludedOutlierVector;
}

FVector DataFilteringComponent::excludeMostRecent(const FVector[] vector)
{
    FVector excludedMostRecentVector = FVector(0);
    auto arraySize = std::size(vector);

    if (arraySize < 2)
    {
        return averageLastXVectors(vector);
    }

    for (int i = 0; i < arraySize - 1; i++)
    {
        excludedMostRecentVector += vector[i];
    }

    excludedMostRecentVector /= arraySize - 1;
    return excludedMostRecentVector;
}

void DataFilteringComponent::clearGazeDataArray()
{
    this->gazeDataArray = nullptr;
}