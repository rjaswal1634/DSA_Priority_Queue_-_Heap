// Rahul Chaudhari
// Program that implements priorotiy queue with heap data structure and print country data
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <functional>
#include <stdexcept>

using namespace std;

// Heap Sturcture
template<class ItemType>
struct HeapType {
    /**
     * Reheap down function for the heap data structure
     *
     * @param root The index of the root node in the heap
     * @param bottom The index of the last element in the heap
     *
     * This function maintains the min-heap property by swapping elements
     * down the heap until the root node holds the minimum value.
     */
    void ReheapDown(int root, int bottom);

    /**
     * Reheap up function for the heap data structure
     *
     * @param root The index of the root node in the heap
     * @param bottom The index of the last element in the heap
     *
     * This function maintains the min-heap property by swapping elements
     * up the heap until the child node is greater than or equal to its parent.
     */
    void ReheapUp(int root, int bottom);

    ItemType *elements; // dynamic array
    int numElements;
    int arraySize; // for size of array to resize
};

// Part of Deliverable 1
template<class ItemType>
// not required just following the tempelete
void HeapType<ItemType>::ReheapDown(int root, int bottom) {
    int maxChild;
    int leftChild = 2 * root + 1;
    int rightChild = 2 * root + 2;
    if (leftChild <= bottom) {
        // left child is part of the heap
        if (leftChild == bottom) // only one child
            maxChild = leftChild;
        else {
            // two children
            if (elements[leftChild] <= elements[rightChild])
                maxChild = rightChild;
            else
                maxChild = leftChild;
        }
        // compare max child with parent
        if (elements[root] < elements[maxChild]) {
            swap(elements[root], elements[maxChild]);
            ReheapDown(maxChild, bottom);
        }
    }
}

template<class ItemType>
// not required just following the tempelete
void HeapType<ItemType>::ReheapUp(int root, int bottom) {
    int parent;
    if (bottom > root) {
        // tree is not empty
        parent = (bottom - 1) / 2;
        if (elements[parent] < elements[bottom]) {
            swap(elements[parent], elements[bottom]);
            ReheapUp(root, parent);
        }
    }
}

// Deliverable 1 Creating a Priority Queue data structure
template<typename T>
class PriorityQueue {
private:
    HeapType<T> heap;
    /**
    * Resizes the internal array of the priority queue when it reaches capacity.
    */
    void resize() {
        // we are using 1 based indexing so, there need to at least 2 array size so 0 and 1 index is there
        int newSize = (heap.arraySize == 0 || heap.arraySize == 1) ? 2 : heap.arraySize * 2; // doubling the array
        T *newArr = new T[newSize];
        for (int i = 0; i < heap.numElements; ++i) {
            newArr[i] = heap.elements[i];
        }
        for (int i = heap.numElements; i < newSize; ++i) {
            newArr[i] = T();
        }
        delete[] heap.elements;
        heap.elements = newArr;
        heap.arraySize = newSize;
    }

    /**
    * Helper function to percolate an element up the heap during insertion.
    *
    * @param hole The index of the current position in the heap
    * @param val The value to be inserted
    * @param comparator The comparison function to determine priority
    *
    * @return The final index of the inserted element in the heap
    */
    int percolateUp(int hole, T val, function<bool(const T &, const T &)> &comparator) {
        while (hole > 1 && comparator(val, heap.elements[hole / 2])) {
            heap.elements[hole] = heap.elements[hole / 2];
            hole = hole / 2;
        }
        return hole;
    }

    /**
    * Helper function to percolate an element down the heap during deletion (min extraction).
    *
    * @param hole The index of the current position in the heap
    * @param val The value of the element to be percolated down (the min element)
    * @param comparator The comparison function to determine priority
    *
    * @return The final index of the min element in the heap after percolation
    */
    int percolateDown(int hole, T val, function<bool(const T &, const T &)> &comparator) {
        while (2 * hole <= heap.numElements) {
            int left = 2 * hole;
            int right = left + 1;
            int target;
            if (right > heap.numElements || comparator(heap.elements[left], heap.elements[right]))
                target = left;
            else
                target = right;
            if (comparator(heap.elements[target], val)) {
                heap.elements[hole] = heap.elements[target];
                hole = target;
            } else
                break;
        }
        return hole;
    }

public:
    /**
     * Default constructor for PriorityQueue.
     * Constructs an empty priority queue with initial size 5 with empty elements of respective class.
     */
    PriorityQueue() {
        heap.arraySize = 5; // any random default value would work I choose 5. use a suitable value.
        heap.elements = new T[5];
        heap.numElements = 0;
    }

    // if the user specify a value of the index the respective size array will be created.
    /**
     * Constructor that allows specifying the initial size of the priority queue.
     * @param maxSize The initial size of the priority queue
     */
    PriorityQueue(int maxSize) {
        heap.arraySize = maxSize;
        heap.elements = new T[maxSize];
        heap.numElements = 0;
        // initializing all the values to null
        for (int i = 0; i < maxSize; ++i) {
            heap.elements[i] = T(); // This assumes T has a default constructor
        }
    }

    /**
    * Destructor for PriorityQueue.
    */
    ~PriorityQueue() {
        delete[] heap.elements;
    }

    /**
    * Insert an element into the priority queue.
    * @param val The value to be inserted.
    * @param comparator Comparator function for comparing elements.
    */
    void insert(T val, function<bool(const T &, const T &)> &comparator) {
        // we are using 1 based indexing so, there need to at least 2 array size so 0 and 1 index is there
        if (heap.numElements == heap.arraySize - 1 || (heap.arraySize == 0 || heap.arraySize == 1)) {
            resize();
        }
        ++heap.numElements;
        int i = percolateUp(heap.numElements, val, comparator);
        heap.elements[i] = val;
    }

    /**
    * Delete the minimum element from the priority queue.
    * @param comparator Comparator function for comparing elements.
    * @return The minimum element in the priority queue.
    */
    T deleteMin(function<bool(const T &, const T &)> &comparator) {
        if (isEmpty()) {
            throw runtime_error("Priority queue is empty");
        }
        T minItem = heap.elements[1];
        int hole = percolateDown(1, heap.elements[heap.numElements], comparator);
        heap.elements[hole] = heap.elements[heap.numElements];
        --heap.numElements;
        return minItem;
    }

    /**
    * Check if the priority queue is empty.
    * @return True if the priority queue is empty, otherwise false.
    */
    bool isEmpty() const {
        return heap.numElements == 0;
    }

    /**
    * Print the internal array of the priority queue.
    * @param pq The priority queue to be printed.
    */
    // This is extra code for debugging proposes, no need in the main code. ******Ignore it****
    void printArray(const PriorityQueue<T> &pq) {
        if (pq.isEmpty()) {
            cout << "Priority queue is empty." << endl;
            return;
        }

        cout << "Priority queue array: ";
        for (int i = 1; i <= pq.heap.numElements; ++i) {
            cout << pq.heap.elements[i] << " ";
        }
        cout << endl;
    }
};

// Country Class
class Country {
private:
    string name;
    int number;
    int area;
    int population;

public:
    /**
     * Default constructor for Country class.
     * Initializes all attributes to zero or empty string.
     */
    Country() {
        number = 0; // nullvalues for the array
        name = ""; // nullvalues for the array
        population = 0;
        area = 0;
    }

    /**
    * Parameterized constructor for Country class.
    * @param number The country number.
    * @param country_name The name of the country.
    * @param population The population of the country.
    * @param area The area of the country.
    */
    Country(int number, const string &country_name, int population, int area) {
        this->name = country_name;
        this->number = number;
        this->area = area;
        this->population = population;
    }

    /**
     * Getter for the name of the country.
     * @return The name of the country.
     */
    string getName() const {
        return name;
    }

    /**
    * Getter for the country number.
    * @return The number of the country.
    */
    int getNumber() const {
        return number;
    }

    /**
    * Getter for the area of the country.
    * @return The area of the country.
    */
    int getArea() const {
        return area;
    }

    /**
    * Getter for the population of the country.
    * @return The population of the country.
    */
    int getPopulation() const {
        return population;
    }

    /**
    * Setter for the name of the country.
    * @param user_name The name to be set.
    */
    void setName(const string &user_name) {
        this->name = user_name;
    }

    /**
    * Setter for the country number.
    * @param num The number to be set.
    */
    void setNumber(int num) {
        this->number = num;
    }

    /**
    * Setter for the area of the country.
    * @param user_area The area to be set.
    */
    void setArea(int user_area) {
        this->area = user_area;
    }

    /**
    * Setter for the population of the country.
    * @param user_pop The population to be set.
    */
    void setPopulation(int user_pop) {
        this->population = user_pop;
    }

    /**
    * Convert Country object to a string representation.
    * @return The string representation of the Country object.
    */
    string toString() const {
        if (getNumber() > 0) {
            stringstream output;
            output << "Country Number: " << getNumber() << ", Name: " << getName() << ", Population: " <<
                    getPopulation() << ", Area: " << getArea();
            return output.str();
        } else {
            return ""; // empty string for checking nullvalues
        }
    }

    /**
    * Overloaded stream insertion operator for Country class.
    * @param os The output stream.
    * @param country The Country object to be printed.
    * @return The output stream.
    */
    // This is extra part for degugging purposes that I used
    friend ostream &operator<<(std::ostream &os, const Country &country) {
        os << country.toString() << endl;
        return os;
    }
};

// Comparator lambda expression for population
function<bool(const Country &, const Country &)> compareByPopulation = [](const Country &a, const Country &b) {
    return a.getPopulation() > b.getPopulation();
};

function<bool(const Country &, const Country &)> compareByArea = [](const Country &a, const Country &b) {
    return a.getArea() > b.getArea();
};
/**
 * Remove commas and quotes from a string.
 * @param str The input string.
 * @return The modified string without commas and quotes.
 */
string removeCommasQuote(const string &str) {
    string result;
    bool insideQuotes = false;
    for (char c: str) {
        if (c == '"') {
            // Toggle insideQuotes flag
            insideQuotes = !insideQuotes;
        } else if (c == ',' && insideQuotes) {
            // Skip comma if inside quotes
            continue;
        } else {
            // Append character if it's not a comma inside quotes
            result += c;
        }
    }
    return result;
}

/**
 * Read country data from a file and insert into priority queues.
 * @param filename The name of the file containing country data.
 * @param populationPriorityQueue The priority queue for population.
 * @param areaPriorityQueue The priority queue for area.
 */
void readCountryData(const string &filename, PriorityQueue<Country> &populationPriorityQueue,
                     PriorityQueue<Country> &areaPriorityQueue) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Error opening file");
    }
    string line;
    if (getline(file, line)) {
        cout << line; // header skipping and printng
    } else {
        throw runtime_error("File is empty");
    }
    while (!file.eof()) {
        getline(file, line);
        if (line.find_first_not_of(" \t\v\r\n") == string::npos) {
            // Line contains only whitespace, skip it // if file has empty spaces
            // if there is a missing row, in the file, it will skip through it to the next country
            continue;
        }
        istringstream iss(line);
        string token;
        vector<string> tokens;
        bool inQuotes = false;
        string partialToken;
        while (getline(iss, token, ',')) {
            // Check if the token starts with a quote
            if (!inQuotes && token.front() == '"') {
                inQuotes = true;
                partialToken = token;
            } else if (inQuotes) {
                // Append to the current token until a closing quote is found
                partialToken += "," + token;
                if (token.back() == '"') {
                    inQuotes = false;
                    tokens.push_back(partialToken);
                }
            } else {
                tokens.push_back(token);
            }
        }

        if (tokens.size() != 4) {
            throw runtime_error("Invalid format in line: " + line);
        }

        int number = stoi(tokens[0]);
        string name = tokens[1];
        // Remove commas & quote from numbers before converting to integers
        int population = stoi(removeCommasQuote(tokens[2]));
        int area = stoi(removeCommasQuote(tokens[3]));
        Country country(number, name, population, area);
        populationPriorityQueue.insert(country, compareByPopulation);
        areaPriorityQueue.insert(country, compareByArea);
    }
}


int main() {
    PriorityQueue<Country> populationPriorityQueue; // Adjust size as needed
    PriorityQueue<Country> areaPriorityQueue; // Adjust size as needed
    // Can also declare the array size in the following format, if not it will create array of size 5 by default
    // PriorityQueue<Country> areaPriorityQueue(100); this will create user specified array size

    // Read country data and insert into priority queues
    readCountryData("Country.txt", populationPriorityQueue, areaPriorityQueue);
    cout << "\nSorting by Population:\n";
    while (!populationPriorityQueue.isEmpty()) {
        Country country = populationPriorityQueue.deleteMin(compareByPopulation);
        string printable_string = country.toString();
        if (!printable_string.empty()) {
            cout << printable_string << endl;
        }
    }
    cout << "\nSorting by Area:\n";
    while (!areaPriorityQueue.isEmpty()) {
        Country country = areaPriorityQueue.deleteMin(compareByArea);
        string printable_string = country.toString();
        if (!printable_string.empty()) {
            cout << printable_string << endl;
        }
    }
    return 0;
}
