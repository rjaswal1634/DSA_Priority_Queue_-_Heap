Project Name: Priority Queue with Country Data

Description: This C++ code implements a priority queue data structure using a min-heap and demonstrates its application with country data. The program can sort countries by population and area using priority queues.
The Requirements & Deliverables are listed in the pdf. 

Key Features:
Implements a priority queue using a min-heap.
Defines a Country class to store country information (number, name, population, area).
Provides functions to read country data from a CSV file.
Sorts countries by population and area using priority queues with custom comparison functions.

How to Use:
1. Create a CSV file named "Country.txt" with the following format (one header row and data rows):
  Number, "Country Name", Population, Area
  1, "India", 1400000000, 3287263
  2, "China", 1444000000, 9596961

2. Clone or download the repository.
3. Compile and run the code. You will need a C++ compiler that supports C++11 features (e.g., g++).
   Bash
  g++ -o priority_queue priority_queue.cpp
  ./priority_queue


Output:
The program will print the countries sorted by population (highest to lowest) and then sorted by area (largest to smallest).
Example Output: Further Output are showcased in the pdf.
Sorting by Population:
Country Number: 2, Name: China, Population: 1444000000, Area: 9596961
Country Number: 1, Name: India, Population: 1400000000, Area: 3287263

Sorting by Area:
Country Number: 2, Name: China, Population: 1444000000, Area: 9596961
Country Number: 1, Name: India, Population: 1400000000, Area: 3287263

Further Notes:
This is a basic implementation of a priority queue and country data processing.
You can modify the code to suit your specific needs, such as adding more functionalities to the Country class or using different sorting criteria.

