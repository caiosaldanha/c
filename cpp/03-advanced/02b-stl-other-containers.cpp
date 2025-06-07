// This program demonstrates other useful STL containers: std::list, std::deque, and std::priority_queue.
#include <iostream>
#include <list>          // For std::list
#include <deque>         // For std::deque
#include <queue>         // For std::priority_queue
#include <vector>        // For initializing priority_queue example
#include <string>
#include <algorithm>     // For std::for_each

// Helper function to print elements of a container that supports range-based for loops
template<typename Container>
void print_container(const std::string& name, const Container& c) {
    std::cout << name << ": ";
    for (const auto& val : c) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
}

int main() {
    // --- 1. std::list ---
    // - Doubly-linked list.
    // - Efficient insertion and deletion anywhere (O(1) if iterator is available).
    // - Not so efficient random access (O(n)).
    // - Preferred over std::vector when frequent insertions/deletions in the middle are needed.
    // - Does not support operator[] for random access.
    std::cout << "--- std::list ---" << std::endl;
    std::list<int> l;

    // Basic operations
    l.push_back(10);    // Add to end
    l.push_front(5);   // Add to front
    l.push_back(20);
    print_container("List l after push_back/front", l); // Expected: 5 10 20

    // Insert
    auto it_list = l.begin(); // Iterator to the first element (5)
    std::advance(it_list, 1); // Move iterator to point to 10
    l.insert(it_list, 15);    // Insert 15 before 10
    print_container("List l after insert(15)", l); // Expected: 5 15 10 20

    // Erase
    it_list = l.begin();
    std::advance(it_list, 2); // Iterator to 10
    l.erase(it_list);         // Erase element 10
    print_container("List l after erase(10)", l); // Expected: 5 15 20

    // Iteration (already shown in print_container)
    std::cout << "Iterating through list l using std::for_each and lambda: ";
    std::for_each(l.begin(), l.end(), [](int x){ std::cout << x << " "; });
    std::cout << std::endl;

    l.remove(15); // Remove all occurrences of value 15
    print_container("List l after remove(15)", l);


    // --- 2. std::deque ---
    // - Double-ended queue (pronounced "deck").
    // - Supports efficient insertion and deletion at both beginning and end (amortized O(1)).
    // - Supports random access (operator[]) like std::vector (O(1)).
    // - Internally, often implemented as a sequence of individually allocated fixed-size arrays.
    // - Good when you need vector-like random access but also frequent push/pop at front.
    std::cout << "\n--- std::deque ---" << std::endl;
    std::deque<std::string> dq;

    // Basic operations
    dq.push_back("Bravo");
    dq.push_front("Alpha");
    dq.push_back("Charlie");
    print_container("Deque dq after push_back/front", dq); // Expected: Alpha Bravo Charlie

    dq.pop_front();
    print_container("Deque dq after pop_front", dq); // Expected: Bravo Charlie

    dq.push_front("Xray");
    dq.pop_back();
    print_container("Deque dq after push_front(Xray), pop_back", dq); // Expected: Xray Bravo

    // Random access
    if (dq.size() >= 2) {
        std::cout << "Element at index 0: " << dq[0] << std::endl;
        std::cout << "Element at index 1 using at(): " << dq.at(1) << std::endl;
    }
    dq.clear();
    print_container("Deque dq after clear", dq);


    // --- 3. std::priority_queue ---
    // - Adapts a container (default std::vector) to provide priority queue functionality.
    // - Elements are ordered according to a comparison function (default std::less, so largest element has highest priority).
    // - Provides constant time lookup of the largest (by default) element.
    // - Insertion and extraction are logarithmic in time.
    // - Not iterable directly; access is only via top(), push(), pop().
    std::cout << "\n--- std::priority_queue ---" << std::endl;

    // Default priority_queue (max-heap: largest element at top)
    std::priority_queue<int> pq_max;
    pq_max.push(30);
    pq_max.push(100);
    pq_max.push(20);
    pq_max.push(50);

    std::cout << "Max-heap priority_queue (pq_max):" << std::endl;
    std::cout << "Top element: " << pq_max.top() << std::endl; // Expected: 100
    pq_max.pop(); // Remove 100
    std::cout << "Top element after pop: " << pq_max.top() << std::endl; // Expected: 50
    std::cout << "Size: " << pq_max.size() << std::endl;

    std::cout << "Popping all elements from pq_max: ";
    while(!pq_max.empty()) {
        std::cout << pq_max.top() << " ";
        pq_max.pop();
    }
    std::cout << std::endl;

    // Min-heap priority_queue (smallest element at top)
    // Requires specifying the underlying container (std::vector<int>) and comparator (std::greater<int>)
    std::priority_queue<int, std::vector<int>, std::greater<int>> pq_min;
    pq_min.push(30);
    pq_min.push(100);
    pq_min.push(20);
    pq_min.push(50);

    std::cout << "\nMin-heap priority_queue (pq_min):" << std::endl;
    std::cout << "Top element: " << pq_min.top() << std::endl; // Expected: 20
    pq_min.pop(); // Remove 20
    std::cout << "Top element after pop: " << pq_min.top() << std::endl; // Expected: 30

    std::cout << "Popping all elements from pq_min: ";
    while(!pq_min.empty()) {
        std::cout << pq_min.top() << " ";
        pq_min.pop();
    }
    std::cout << std::endl;

    std::cout << "\nOther STL containers demonstration complete." << std::endl;
    return 0;
}
