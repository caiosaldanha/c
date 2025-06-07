// This program demonstrates concepts of exception safety in C++.
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept> // For std::runtime_error
#include <algorithm> // For std::copy, std::swap
#include <utility>   // For std::move (used in copy-and-swap)

// --- Helper: A simple resource class that can throw on copy ---
class Resource {
public:
    int id;
    std::string data;
    static int copy_throw_countdown; // Simulate copy failure after N copies

    Resource(int i, const std::string& d = "default") : id(i), data(d) {
        std::cout << "Resource " << id << " created." << std::endl;
    }

    Resource(const Resource& other) : id(other.id), data(other.data) {
        if (copy_throw_countdown > 0) {
            --copy_throw_countdown;
            if (copy_throw_countdown == 0) {
                std::cout << "Resource " << id << " copy constructor: Simulating exception!" << std::endl;
                throw std::runtime_error("Simulated copy failure for Resource " + std::to_string(id));
            }
        }
        std::cout << "Resource " << id << " copied." << std::endl;
    }

    Resource& operator=(const Resource& other) {
        if (this == &other) return *this;
        if (copy_throw_countdown > 0) {
            --copy_throw_countdown;
            if (copy_throw_countdown == 0) {
                std::cout << "Resource " << id << " copy assignment: Simulating exception!" << std::endl;
                throw std::runtime_error("Simulated copy assignment failure for Resource " + std::to_string(id));
            }
        }
        id = other.id;
        data = other.data;
        std::cout << "Resource " << id << " copy assigned." << std::endl;
        return *this;
    }

    ~Resource() {
        // std::cout << "Resource " << id << " destroyed." << std::endl; // Can be noisy
    }

    void print() const {
        std::cout << "Resource ID: " << id << ", Data: " << data << std::endl;
    }
};
int Resource::copy_throw_countdown = 0; // Static member initialization


// --- Class demonstrating different levels of exception safety ---
class DataManager {
private:
    Resource* main_res;
    std::vector<Resource*> backup_res_list;
    size_t current_size; // To track valid elements in a raw array example

public:
    DataManager() : main_res(nullptr), current_size(0) {
        std::cout << "DataManager created." << std::endl;
    }

    // --- 1. No Exception Safety ---
    // If Resource copy throws, main_res might be partially updated,
    // or backup_res_list might be left in an inconsistent state. Leaks if not careful.
    void update_no_safety(int id, const std::string& data, const std::vector<Resource>& backups) {
        std::cout << "\n--- Updating (No Safety) ---" << std::endl;
        // Clean up old main resource
        delete main_res;
        main_res = nullptr; // Avoid dangling pointer if new Resource throws

        main_res = new Resource(id, data); // Potential throw here

        // Clear old backups (potential memory leak if new Resource throws before this)
        for (Resource* r : backup_res_list) delete r;
        backup_res_list.clear();

        try {
            for (const auto& b_src : backups) {
                backup_res_list.push_back(new Resource(b_src)); // Potential throw here
            }
        } catch (...) {
            std::cout << "Exception during backup creation in update_no_safety." << std::endl;
            // State is now inconsistent: main_res might be new, backups partially new or empty.
            // Memory leak for successfully allocated backups if not cleaned up.
            // If main_res was allocated and this part throws, main_res itself is a leak if not handled.
            throw; // Rethrow
        }
        std::cout << "Update (No Safety) successful." << std::endl;
    }

    // --- 2. Basic Exception Guarantee ---
    // Invariants are preserved (e.g., class remains in a valid state).
    // No resource leaks.
    // Achieved by careful ordering and using RAII for resources.
    void update_basic_safety(int id, const std::string& data, const std::vector<Resource>& backups) {
        std::cout << "\n--- Updating (Basic Safety) ---" << std::endl;
        Resource* temp_main_res = nullptr;
        std::vector<Resource*> temp_backup_res_list;
        // Use RAII for the temporary backup list to ensure cleanup if an exception occurs
        // For simplicity, manual cleanup in catch block is shown here. A vector of unique_ptrs would be better.

        try {
            temp_main_res = new Resource(id, data); // Stage 1: Create new main resource

            // Stage 2: Create new backup resources
            for (const auto& b_src : backups) {
                temp_backup_res_list.push_back(new Resource(b_src)); // Can throw
            }

            // Stage 3: Commit (no-throw operations)
            // Delete old main_res and assign new one
            delete main_res;
            main_res = temp_main_res;
            temp_main_res = nullptr; // Ownership transferred

            // Delete old backups and swap with new ones
            for (Resource* r : backup_res_list) delete r;
            backup_res_list.clear();
            backup_res_list.swap(temp_backup_res_list); // vector::swap is no-throw

            std::cout << "Update (Basic Safety) successful." << std::endl;

        } catch (...) {
            std::cout << "Exception during update_basic_safety. Cleaning up temporaries." << std::endl;
            delete temp_main_res; // Clean up if allocated
            for (Resource* r : temp_backup_res_list) delete r; // Clean up if any allocated
            // The original state of DataManager (main_res, backup_res_list) is preserved.
            // No leaks from temporaries. This is the basic guarantee.
            throw; // Rethrow
        }
    }


    // --- 3. Strong Exception Guarantee (Commit-or-Rollback) ---
    // If an operation fails (throws), the state of the object is as if the operation was never called.
    // Often achieved using the copy-and-swap idiom.

    // For this, DataManager itself would need a proper copy constructor, assignment operator, and swap function.
    // Let's make a simplified version focusing on the update logic.
    // We'll create a temporary DataManager state and then swap.

    // Assume DataManager has a swap method (member function)
    void swap(DataManager& other) noexcept {
        std::swap(main_res, other.main_res);
        backup_res_list.swap(other.backup_res_list); // std::vector::swap is noexcept
        std::swap(current_size, other.current_size);
    }

    // update_strong_safety using a temporary object and swap
    void update_strong_safety(int id, const std::string& data, const std::vector<Resource>& backups) {
        std::cout << "\n--- Updating (Strong Safety with temporary object) ---" << std::endl;

        // Create a temporary DataManager that will hold the new state.
        // This is a simplified version. A full copy-and-swap would involve copy constructing 'this'
        // into a temporary, modifying the temporary, and then swapping.
        // Here, we construct a new state in separate temporary variables.

        Resource* new_main_res = nullptr;
        std::vector<Resource*> new_backup_list;

        try {
            // Phase 1: Perform all potentially throwing operations on temporary data.
            new_main_res = new Resource(id, data);
            for (const auto& b_src : backups) {
                new_backup_list.push_back(new Resource(b_src)); // Can throw
            }

            // Phase 2: If all successful, commit by swapping current state with new state.
            // These operations must be no-throw.
            std::swap(this->main_res, new_main_res);
            this->backup_res_list.swap(new_backup_list);

            // Phase 3: Clean up the old state (which is now in new_main_res, new_backup_list)
            delete new_main_res; // old main_res
            for (Resource* r : new_backup_list) delete r; // old backup_res_list

            std::cout << "Update (Strong Safety) successful." << std::endl;

        } catch (...) {
            std::cout << "Exception during update_strong_safety. Original state preserved." << std::endl;
            // Clean up any resources allocated for the new state if an exception occurred.
            delete new_main_res;
            for (Resource* r : new_backup_list) delete r;
            // The original state of 'this' DataManager remains untouched due to working on temporaries.
            throw; // Rethrow
        }
    }


    ~DataManager() {
        std::cout << "DataManager destroyed. Cleaning up resources." << std::endl;
        delete main_res;
        for (Resource* r : backup_res_list) delete r;
    }

    void print() const {
        if (main_res) main_res->print();
        else std::cout << "Main resource is null." << std::endl;
        std::cout << "Backup resources (" << backup_res_list.size() << "):" << std::endl;
        for (const Resource* r : backup_res_list) {
            std::cout << "  ";
            r->print();
        }
    }
};


int main() {
    DataManager dm;
    std::vector<Resource> initial_backups;
    initial_backups.emplace_back(101, "backup1_data");
    initial_backups.emplace_back(102, "backup2_data");

    // --- Test No Safety ---
    std::cout << "\n--- Testing No Safety ---" << std::endl;
    Resource::copy_throw_countdown = 3; // Fails on 3rd copy (1 for main, 1 for first backup, fails on second backup)
    try {
        dm.update_no_safety(1, "main_data_v1", initial_backups);
    } catch (const std::exception& e) {
        std::cerr << "Caught exception (No Safety test): " << e.what() << std::endl;
        std::cout << "State of DataManager after No Safety failure:" << std::endl;
        dm.print(); // Likely inconsistent state, potential leaks
    }
    // Reset dm for next test (manual cleanup or re-create)
    // For simplicity, we'll just continue with the possibly modified dm to see effects.

    // --- Test Basic Safety ---
    // Re-initialize dm for a cleaner test, or ensure previous state is fully cleaned.
    // Let's create a new DataManager for clarity in testing each guarantee.
    DataManager dm_basic;
    std::cout << "\n\n--- Testing Basic Safety ---" << std::endl;
    Resource::copy_throw_countdown = 2; // Fails on 2nd copy (1 for main, fails on first backup)
    try {
        dm_basic.update_basic_safety(2, "main_data_v2", initial_backups);
    } catch (const std::exception& e) {
        std::cerr << "Caught exception (Basic Safety test): " << e.what() << std::endl;
        std::cout << "State of DataManager after Basic Safety failure:" << std::endl;
        dm_basic.print(); // Should be in original (empty) state, no leaks from temporaries.
    }
    // Test basic safety success
    Resource::copy_throw_countdown = 0; // No throw
    try {
         dm_basic.update_basic_safety(3, "main_data_v3", initial_backups);
         std::cout << "State of DataManager after Basic Safety success:" << std::endl;
         dm_basic.print();
    } catch (const std::exception& e) {
        std::cerr << "Unexpected exception during Basic Safety success test: " << e.what() << std::endl;
    }


    // --- Test Strong Safety ---
    DataManager dm_strong;
    std::cout << "\n\n--- Testing Strong Safety ---" << std::endl;
    Resource::copy_throw_countdown = 2; // Fails on 2nd copy (1 for new_main_res, fails on first new_backup)
    try {
        dm_strong.update_strong_safety(4, "main_data_v4", initial_backups);
    } catch (const std::exception& e) {
        std::cerr << "Caught exception (Strong Safety test): " << e.what() << std::endl;
        std::cout << "State of DataManager after Strong Safety failure:" << std::endl;
        dm_strong.print(); // Should be in original (empty) state.
    }
    // Test strong safety success
    Resource::copy_throw_countdown = 0; // No throw
    try {
         dm_strong.update_strong_safety(5, "main_data_v5", initial_backups);
         std::cout << "State of DataManager after Strong Safety success:" << std::endl;
         dm_strong.print();
    } catch (const std::exception& e) {
        std::cerr << "Unexpected exception during Strong Safety success test: " << e.what() << std::endl;
    }

    std::cout << "\nException safety demonstration complete." << std::endl;
    return 0;
}
