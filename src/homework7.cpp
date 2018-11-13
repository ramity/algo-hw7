// -----------------------------------------------------------------------------
// 16.1-5 text:
// -----------------------------------------------------------------------------
// Consider a modification to the activity-selection problem in which each
// activity a of i has, in addition to a start and finish time, a value v of i.
// The objective is no longer to maximize the number of activities scheduled,
// but instead to maximize the total value of the activities scheduled. That is,
// we wish to choose a set A of compatible activities such that the sumlimation
// of a of k is an element of A to the power of V of k is maximized. Give a
// polynomial-time algorithm for this problem.

// -----------------------------------------------------------------------------
// Blackboard assignment information:
// -----------------------------------------------------------------------------
// Exercise 16.1-5
// submit code and documentation and output files.
// Each input file contains only one test case. It contains:
// - the first line ccontains two integers separated by space
// - an int n (total number of activities)  an int I for the [0 I] interval
//   where activities to fit\n
// - n lines of each containing four ints designating activity ID (an int
//   number), start time, finish time, and value where each of the ID, the
//   times, and the value are integers <= 1000
// Things to note:
// - Do not expect the activities ID to be ordered or consecutive.
// - An output file contains the maximum value followed by a possible list (in
//   different lines) of activity IDs. List the activities where IDs are
//   increasing.
// - All numbers are separated by space.

#include <fstream>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <string.h>

using namespace std;

struct activity
{
  int ID;
  int startTime;
  int finishTime;
  int value;
};

// define a method for the integrated sort function (via <algorithm>) to utilize
bool finishTimeComparison(activity a, activity b)
{
  return (a.finishTime < b.finishTime);
}

// gets the next possible "fitting" activity key given the starting key
int getNextPossibleActivityKey(activity activities[], int startingKey)
{
  // iterate from startingKey - 1 downto 0
  for(int key = startingKey - 1; key >= 0; key--)
  {
    // check if current activity finishes before startTime of previous activity
    if(activities[key].finishTime <= activities[startingKey].startTime)
    {
      return key;
    }
  }

  // no valid activity exists
  return -1;
}

// converts an int to string - allows for the usage of this file without c++11
string intToString(int integer)
{
    char characters[1024];
    sprintf(characters, "%d", integer);
    return (string) characters;
}

string getMaxValueCombinationOutput(activity activities[], int totalActivities)
{
  // while both inputs are pre sorted by finish time, assume they are not
  // (a brief not on std::sort - https://stackoverflow.com/a/5039387)
  sort(activities, activities + totalActivities, finishTimeComparison);

  // stores value sum of value combinations (dynamic programming)
  int * valueSums;
  valueSums = new int[totalActivities];

  // stores IDs of value combinations for output (dynamic programming)
  string * valueIDs;
  valueIDs = new string[totalActivities];

  // init base cases
  valueSums[0] = activities[0].value;
  valueIDs[0] = intToString(activities[0].ID) + " ";

  for(int key = 1;key < totalActivities;key++)
  {
    string currentGreedyActivityCombinationIDs = intToString(activities[key].ID) + " ";
    int currentGreedyActivityCombinationSum = activities[key].value;

    int nextPossibleActivityKey = getNextPossibleActivityKey(activities, key);

    // make sure nextPossibleActivityKey is valid
    if(nextPossibleActivityKey != -1)
    {
      // note that how currentGreedyActivityCombinationIDs is currently being
      // set ensures for the IDs to be listed in increasing order as requested.
      currentGreedyActivityCombinationIDs = valueIDs[nextPossibleActivityKey] + currentGreedyActivityCombinationIDs;
      currentGreedyActivityCombinationSum += valueSums[nextPossibleActivityKey];
    }

    if(currentGreedyActivityCombinationSum > valueSums[key - 1])
    {
      valueIDs[key] = currentGreedyActivityCombinationIDs;
      valueSums[key] = currentGreedyActivityCombinationSum;
    }
    else
    {
      valueIDs[key] = valueIDs[key - 1];
      valueSums[key] = valueSums[key - 1];
    }
  }

  int maxValueCombination = valueSums[totalActivities - 1];
  string maxValueIDs = valueIDs[totalActivities - 1];

  string output;
  output += intToString(maxValueCombination);
  output += "\n";
  output += maxValueIDs;

  // free memory
  delete[] valueSums;
  delete[] valueIDs;

  // return result
  return output;
}

int main()
{
  ifstream inputFile;
  ofstream outputFile;

  inputFile.open("./input1.txt");

  if(!inputFile)
  {
    cout << "Unable to open file" << endl;
    exit(1);
  }

  // read totalActivities from file
  int totalActivities;
  inputFile >> totalActivities;

  // read interval from file
  int interval;
  inputFile >> interval;

  // init activities array
  activity * activities;
  activities = new activity[totalActivities];

  // populate activities from file
  for(int z = 0;z < totalActivities;z++)
  {
    activity localActivity;

    inputFile >> localActivity.ID;
    inputFile >> localActivity.startTime;
    inputFile >> localActivity.finishTime;
    inputFile >> localActivity.value;

    activities[z] = localActivity;
  }

  // output answer
  outputFile.open("output1.txt", fstream::out);
  string result = getMaxValueCombinationOutput(activities, totalActivities);
  char * cresult;
  cresult = new char[result.size() + 1];
  strcpy(cresult, result.c_str());
  outputFile << cresult;
  outputFile.close();
  inputFile.close();

  inputFile.open("./input2.txt");

  if(!inputFile)
  {
    cout << "Unable to open file" << endl;
    exit(1);
  }

  // read totalActivities from file
  inputFile >> totalActivities;

  // read interval from file
  inputFile >> interval;

  // init activities array
  activities = new activity[totalActivities];

  // populate activities from file
  for(int z = 0;z < totalActivities;z++)
  {
    activity localActivity;

    inputFile >> localActivity.ID;
    inputFile >> localActivity.startTime;
    inputFile >> localActivity.finishTime;
    inputFile >> localActivity.value;

    activities[z] = localActivity;
  }

  // output answer
  outputFile.open("output2.txt", fstream::out);
  result = getMaxValueCombinationOutput(activities, totalActivities);
  cresult = new char[result.size() + 1];
  strcpy(cresult, result.c_str());
  outputFile << cresult;
  outputFile.close();
  inputFile.close();

  // free memory
  delete[] activities;

  return 0;
};
