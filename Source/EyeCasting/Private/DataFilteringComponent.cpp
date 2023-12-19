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

// Set the gaze data array, filter it, expose the filtered gaze vector, and clear the gaze data array
bool DataFilteringComponent::setGazeDataArray(FEyeTrackerGazeData[] gazeDataArray)
{
    // Check if the gaze data array is empty
    if (std::size(gazeDataArray) == 0)
    {
        return false;
    }

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
    case "None":
        this->filteredGazeVector = returnMostRecent(this->gazeDataArray);
        break;
    default:
        this->filteredGazeVector = averageLastXVectors(this->gazeDataArray);
        break;
    }

    clearGazeDataArray();

    return true;
}

// Set the filtering mode. Returns true if the filtering mode is valid, false otherwise.
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
    case "None":
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

// Return the filtered gaze vector
FVector DataFilteringComponent::getFilteredGazeVector()
{
    return this->filteredGazeVector;
}

// Return the filtering mode
FString DataFilteringComponent::getFilteringMode()
{
    return this->filteringMode;
}

// Average the all the vectors in the array
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

// Take the median of the vectors in the array
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

// Exclude 2 outliers from the array. If the array size is less than 3, average the vectors in the array.
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

// Exclude the most recent vector from the array. If the array size is less than 2, average the vectors in the array.
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

// Return the most recent vector in the array
FVector DataFilteringComponent::returnMostRecent(const FVector[] vector)
{
    return vector[std::size(vector) - 1];
}

// Clear the gaze data array
void DataFilteringComponent::clearGazeDataArray()
{
    this->gazeDataArray = nullptr;
}