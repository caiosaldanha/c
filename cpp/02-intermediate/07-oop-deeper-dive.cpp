// This program demonstrates more advanced Object-Oriented Programming concepts in C++.
#include <iostream>
#include <string>
#include <utility> // For std::move (if move constructor is added)

// --- Base Class ---
class Shape {
protected: // Accessible by this class and derived classes
    std::string color;

public:
    // 1. Constructors
    // Default Constructor
    Shape() : color("transparent") {
        std::cout << "Shape Default Constructor called. Color: " << this->color << std::endl;
    }

    // Parameterized Constructor
    Shape(const std::string& c) : color(c) {
        // 'this' pointer is used to access the member variable 'color'
        // when there's a local variable or parameter with the same name.
        // Here, it's not strictly necessary as the names are different (c vs color)
        // but good for demonstration.
        std::cout << "Shape Parameterized Constructor called. Color: " << this->color << std::endl;
    }

    // Copy Constructor
    Shape(const Shape& other) : color(other.color) {
        std::cout << "Shape Copy Constructor called. Copied color: " << this->color << std::endl;
    }

    // (Optional) Move Constructor C++11 - efficient for transferring resources
    // Shape(Shape&& other) noexcept : color(std::move(other.color)) {
    //     std::cout << "Shape Move Constructor called. Moved color: " << this->color << std::endl;
    //     other.color = "empty_after_move"; // Leave the source object in a valid but unspecified state
    // }


    // 2. Destructor
    // Called when an object of the class goes out of scope or is explicitly deleted.
    // Virtual destructor is important for base classes to ensure derived class destructors are called correctly.
    virtual ~Shape() {
        std::cout << "Shape Destructor called for color: " << this->color << std::endl;
    }

    // 3. 'this' pointer
    // The 'this' pointer is an implicit pointer in member functions that points to the object itself.
    // It's used to differentiate between member variables and local variables/parameters with the same name,
    // or to return a reference/pointer to the current object.
    Shape& setColor(const std::string& c) {
        this->color = c;
        return *this; // Return a reference to the current object for chaining
    }

    std::string getColor() const { // const member function: doesn't modify object state
        return this->color;
    }

    // 6. Polymorphism: virtual function
    // A virtual function is a member function that you expect to be redefined in derived classes.
    // When you refer to a derived class object using a pointer or a reference to the base class,
    // you can call the virtual function for that object and execute the derived class's version of the function.
    virtual void draw() const { // Made const as it doesn't modify the object
        std::cout << "Drawing a generic shape with color: " << this->color << std::endl;
    }

    // Pure virtual function (makes Shape an abstract class, cannot be instantiated)
    // virtual double area() const = 0;
};

// --- Derived Class ---
// 5. Inheritance: Circle inherits from Shape
// 'public' inheritance means public members of Shape become public members of Circle,
// and protected members of Shape become protected members of Circle.
class Circle : public Shape {
private: // 4. Access Specifiers & Encapsulation
    // 'radius' is encapsulated within the Circle class, accessible only through its public methods.
    double radius;

public:
    // Constructor for Circle - calls Shape's parameterized constructor
    Circle(const std::string& c, double r) : Shape(c), radius(r) {
        std::cout << "Circle Parameterized Constructor called. Color: " << this->color << ", Radius: " << this->radius << std::endl;
    }

    // Copy constructor for Circle
    Circle(const Circle& other) : Shape(other), radius(other.radius) { // Call base class copy constructor
        std::cout << "Circle Copy Constructor called. Color: " << this->color << ", Radius: " << this->radius << std::endl;
    }

    // Destructor for Circle
    ~Circle() override { // C++11 'override' keyword ensures we are overriding a virtual function
        std::cout << "Circle Destructor called for color: " << this->color << ", Radius: " << this->radius << std::endl;
    }

    double getRadius() const {
        return this->radius;
    }

    // Polymorphism: Overriding the virtual function 'draw'
    void draw() const override { // 'override' is optional but good practice
        std::cout << "Drawing a circle with color: " << this->color << " and radius: " << this->radius << std::endl;
    }

    // double area() const override { // Implementation for pure virtual function if Shape was abstract
    //     return 3.14159 * radius * radius;
    // }
};

// Another derived class
class Rectangle : public Shape {
private:
    double width;
    double height;
public:
    Rectangle(const std::string& c, double w, double h) : Shape(c), width(w), height(h) {
        std::cout << "Rectangle Parameterized Constructor called. Color: " << this->color
                  << ", Width: " << this->width << ", Height: " << this->height << std::endl;
    }

    ~Rectangle() override {
        std::cout << "Rectangle Destructor called for color: " << this->color << std::endl;
    }

    void draw() const override {
        std::cout << "Drawing a rectangle with color: " << this->color
                  << ", Width: " << this->width << ", Height: " << this->height << std::endl;
    }
};


int main() {
    std::cout << "--- Constructors and Destructor ---" << std::endl;
    Shape s1; // Default constructor
    Shape s2("Red"); // Parameterized constructor
    Shape s3 = s2; // Copy constructor (alternative syntax: Shape s3(s2);)
    // Shape s4(std::move(Shape("Blue"))); // Move constructor example if enabled

    std::cout << "\n--- 'this' pointer and Encapsulation ---" << std::endl;
    s2.setColor("Green").setColor("Dark Green"); // Chaining calls using 'this'
    std::cout << "s2 color after setColor: " << s2.getColor() << std::endl;
    // s2.color = "Blue"; // This would be a compile error if 'color' were private in Shape

    std::cout << "\n--- Inheritance ---" << std::endl;
    Circle c1("Blue", 5.0);
    std::cout << "Circle c1 color: " << c1.getColor() << ", radius: " << c1.getRadius() << std::endl;

    std::cout << "\n--- Polymorphism ---" << std::endl;
    Shape* shape_ptr1 = &s2;
    Shape* shape_ptr2 = &c1; // Base class pointer to derived class object
    Rectangle rect1("Yellow", 4.0, 6.0);
    Shape* shape_ptr3 = &rect1;


    std::cout << "Calling draw() via base class pointer (s2 - Shape object):" << std::endl;
    shape_ptr1->draw(); // Calls Shape::draw()

    std::cout << "Calling draw() via base class pointer (c1 - Circle object):" << std::endl;
    shape_ptr2->draw(); // Calls Circle::draw() due to virtual function

    std::cout << "Calling draw() via base class pointer (rect1 - Rectangle object):" << std::endl;
    shape_ptr3->draw(); // Calls Rectangle::draw() due to virtual function

    // Demonstrate dynamic polymorphism with an array of pointers
    std::cout << "\n--- Polymorphism with an array of Shape pointers ---" << std::endl;
    Shape* shapes[] = {&s1, &s2, &c1, &rect1};
    for (Shape* s_ptr : shapes) {
        s_ptr->draw(); // Calls the appropriate draw() method for each object
    }

    std::cout << "\n--- Destructor calls when objects go out of scope (in reverse order of creation for stack objects) ---" << std::endl;
    // When main ends, destructors for s1, s2, s3, c1, rect1 will be called.
    // For shape_ptr1, shape_ptr2, shape_ptr3, no delete is called as they point to stack objects.
    // If they pointed to heap objects, delete would be needed.

    std::cout << "\n--- Polymorphic deletion (if using heap allocation) ---" << std::endl;
    Shape* heap_circle = new Circle("Purple", 3.3);
    heap_circle->draw();
    delete heap_circle; // This will correctly call Circle destructor then Shape destructor because Shape's destructor is virtual.
                        // If Shape's destructor was not virtual, only Shape's destructor would be called - a common bug!

    return 0;
}
