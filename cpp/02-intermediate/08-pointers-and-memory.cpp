// This program covers dynamic memory management and pointers in C++.
#include <iostream>
#include <string>
#include <vector>
#include <memory> // Required for smart pointers (std::unique_ptr, std::shared_ptr, std::weak_ptr)

// --- Helper class for demonstrating smart pointers ---
class MyResource {
public:
    int id;
    MyResource(int i) : id(i) {
        std::cout << "MyResource " << id << " acquired (Constructor)." << std::endl;
    }
    ~MyResource() {
        std::cout << "MyResource " << id << " released (Destructor)." << std::endl;
    }
    void print() const {
        std::cout << "MyResource " << id << " is being used." << std::endl;
    }
};

// --- Function to illustrate a memory leak ---
void cause_memory_leak() {
    std::cout << "\n--- Illustrating Memory Leak ---" << std::endl;
    // Allocate memory but don't deallocate it
    int* leaky_ptr = new int(100);
    std::cout << "Allocated memory for an int with value: " << *leaky_ptr << std::endl;
    // delete leaky_ptr; // This line is missing, causing a leak!
    std::cout << "Function cause_memory_leak() finished. Memory for " << *leaky_ptr << " was not deallocated." << std::endl;
}

// --- Function to illustrate a dangling pointer ---
void cause_dangling_pointer() {
    std::cout << "\n--- Illustrating Dangling Pointer ---" << std::endl;
    int* ptr = new int(50);
    std::cout << "Original pointer value: " << *ptr << " at address: " << ptr << std::endl;

    int* dangling_ptr = ptr; // Both pointers point to the same memory

    delete ptr; // Deallocate memory using the original pointer
    ptr = nullptr; // Good practice: set original pointer to nullptr after delete

    std::cout << "Memory deallocated. ptr is now nullptr." << std::endl;
    // Now, dangling_ptr points to deallocated memory. Accessing it is undefined behavior.
    std::cout << "dangling_ptr still points to address: " << dangling_ptr << std::endl;
    // std::cout << "Value at dangling_ptr: " << *dangling_ptr << std::endl; // DANGER! Accessing deallocated memory.
    std::cout << "Accessing *dangling_ptr would be undefined behavior (could crash or show garbage)." << std::endl;
}


int main() {
    // --- Dynamic Memory Allocation for Single Variables ---
    std::cout << "--- Dynamic Memory for Single Variables ---" << std::endl;
    // Using `new` to allocate memory for an integer on the heap
    int* p_int = new int; // Allocates memory for one int, uninitialized
    *p_int = 10;          // Assign a value
    std::cout << "Value of dynamically allocated int: " << *p_int << " at address: " << p_int << std::endl;

    // Allocate and initialize
    double* p_double = new double(3.14159);
    std::cout << "Value of dynamically allocated double: " << *p_double << " at address: " << p_double << std::endl;

    // Using `delete` to deallocate the memory
    delete p_int;
    p_int = nullptr; // Good practice: set pointer to nullptr after delete to avoid dangling pointer issues

    delete p_double;
    p_double = nullptr;

    std::cout << "Memory for p_int and p_double deallocated and pointers nullified." << std::endl;

    // --- Dynamic Memory Allocation for Arrays ---
    std::cout << "\n--- Dynamic Memory for Arrays ---" << std::endl;
    int size = 5;
    int* p_array = new int[size]; // Allocate memory for an array of 5 integers

    // Initialize array elements
    for (int i = 0; i < size; ++i) {
        p_array[i] = (i + 1) * 100;
    }

    std::cout << "Dynamically allocated array elements: ";
    for (int i = 0; i < size; ++i) {
        std::cout << p_array[i] << " ";
    }
    std::cout << std::endl;

    // Using `delete[]` to deallocate memory for an array
    delete[] p_array;
    p_array = nullptr; // Good practice
    std::cout << "Memory for p_array deallocated and pointer nullified." << std::endl;
    // IMPORTANT: Using `delete` on memory allocated with `new[]` or `delete[]` on memory
    // allocated with `new` is undefined behavior. Always match them correctly.

    // --- Memory Leaks ---
    cause_memory_leak();
    std::cout << "Note: The memory allocated in cause_memory_leak() is now lost (leaked) "
              << "as there's no pointer to it outside that function's scope." << std::endl;

    // --- Dangling Pointers ---
    cause_dangling_pointer();
    std::cout << "Note: Dangling pointers can lead to crashes or unpredictable behavior if dereferenced." << std::endl;


    // --- Smart Pointers (C++11 and later) ---
    // Smart pointers automate memory management and help prevent leaks and dangling pointers.

    // 1. std::unique_ptr
    // - Owns the pointed-to object exclusively.
    // - Cannot be copied, but can be moved.
    // - Automatically deallocates memory when the unique_ptr goes out of scope.
    std::cout << "\n--- std::unique_ptr ---" << std::endl;
    { // Inner scope to demonstrate automatic cleanup
        std::unique_ptr<MyResource> u_ptr1(new MyResource(1)); // Direct initialization
        // Or, using std::make_unique (preferred, C++14 onwards, exception-safe)
        std::unique_ptr<MyResource> u_ptr2 = std::make_unique<MyResource>(2);

        u_ptr1->print();
        u_ptr2->print();

        // MyResource* raw_ptr = u_ptr1.get(); // Get raw pointer (use with caution)

        // std::unique_ptr<MyResource> u_ptr3 = u_ptr1; // Compile Error: Cannot copy unique_ptr
        std::unique_ptr<MyResource> u_ptr3 = std::move(u_ptr1); // OK: Transfer ownership via move

        if (u_ptr1) { // Check if unique_ptr still owns a resource
            std::cout << "u_ptr1 still owns a resource (this shouldn't happen after move)." << std::endl;
        } else {
            std::cout << "u_ptr1 is now null (ownership transferred)." << std::endl;
        }
        u_ptr3->print();
        // u_ptr1, u_ptr2, u_ptr3 will automatically release their MyResource objects
        // when this scope ends. u_ptr1 is null, so it does nothing.
    }
    std::cout << "unique_ptrs went out of scope. Resources should be released." << std::endl;

    // unique_ptr for arrays
    std::unique_ptr<int[]> u_array_ptr = std::make_unique<int[]>(3);
    u_array_ptr[0] = 1; u_array_ptr[1] = 2; u_array_ptr[2] = 3;
    std::cout << "unique_ptr for array: " << u_array_ptr[0] << ", " << u_array_ptr[1] << ", " << u_array_ptr[2] << std::endl;
    // Automatically calls delete[] when u_array_ptr goes out of scope.


    // 2. std::shared_ptr
    // - Allows multiple shared_ptrs to own the same object.
    // - Keeps a reference count; the object is deallocated when the last shared_ptr owning it is destroyed or reset.
    std::cout << "\n--- std::shared_ptr ---" << std::endl;
    std::shared_ptr<MyResource> sh_ptr1; // Initially empty
    { // Inner scope
        std::shared_ptr<MyResource> sh_ptr_original = std::make_shared<MyResource>(3); // Preferred way
        // std::shared_ptr<MyResource> sh_ptr_original(new MyResource(3)); // Also possible

        std::cout << "Original shared_ptr use count: " << sh_ptr_original.use_count() << std::endl; // Count is 1
        sh_ptr_original->print();

        sh_ptr1 = sh_ptr_original; // Copying shared_ptr, increases reference count
        std::cout << "sh_ptr1 (after assignment) use count: " << sh_ptr1.use_count() << std::endl; // Count is 2
        std::cout << "sh_ptr_original use count: " << sh_ptr_original.use_count() << std::endl; // Count is 2

        {
            std::shared_ptr<MyResource> sh_ptr_another = sh_ptr1; // Another copy
            std::cout << "sh_ptr_another use count: " << sh_ptr_another.use_count() << std::endl; // Count is 3
            sh_ptr_another->print();
        } // sh_ptr_another goes out of scope, reference count decreases to 2
        std::cout << "After sh_ptr_another scope, sh_ptr1 use count: " << sh_ptr1.use_count() << std::endl;

    } // sh_ptr_original goes out of scope, reference count decreases to 1 (sh_ptr1 still owns it)
    std::cout << "After sh_ptr_original scope, sh_ptr1 use count: " << sh_ptr1.use_count() << std::endl;
    if(sh_ptr1) {
        sh_ptr1->print(); // MyResource 3 is still alive
    }
    // MyResource 3 will be released when sh_ptr1 goes out of scope at the end of main.

    // 3. std::weak_ptr (Brief Mention)
    // - A non-owning ("weak") reference to an object managed by a std::shared_ptr.
    // - Used to break circular dependencies between std::shared_ptrs.
    // - To access the object, a weak_ptr must be converted to a shared_ptr (using lock()).
    //   This conversion fails if the object has already been deleted.
    std::cout << "\n--- std::weak_ptr (Brief Mention) ---" << std::endl;
    std::weak_ptr<MyResource> w_ptr;
    {
        std::shared_ptr<MyResource> sp_for_wp = std::make_shared<MyResource>(4);
        w_ptr = sp_for_wp; // weak_ptr points to resource owned by sp_for_wp

        if (auto locked_sp = w_ptr.lock()) { // Try to get a shared_ptr from weak_ptr
            std::cout << "Resource " << locked_sp->id << " accessed via weak_ptr. Use count: " << locked_sp.use_count() << std::endl;
        } else {
            std::cout << "Resource no longer exists (weak_ptr expired)." << std::endl;
        }
        // sp_for_wp goes out of scope here, MyResource(4) is deleted if no other shared_ptr owns it.
    }
    // Now, the object MyResource(4) is deleted.
    if (w_ptr.expired()) { // Check if the pointed-to object has been deleted
         std::cout << "weak_ptr is expired (points to a deleted object)." << std::endl;
    }
    if (auto locked_sp = w_ptr.lock()) { // This will fail
        std::cout << "Resource " << locked_sp->id << " accessed via weak_ptr (this shouldn't happen)." << std::endl;
    } else {
        std::cout << "Could not lock weak_ptr; resource is gone." << std::endl;
    }


    std::cout << "\nPointers and memory management demonstration complete." << std::endl;
    // sh_ptr1 goes out of scope here, releasing MyResource 3.
    return 0;
}
