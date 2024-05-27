#include <iostream>
using namespace std;

class MyClass {  // The class
  public:        // Access specifier
    int myNum;   // Attribute (int variable)
    string myString;  // Attribute (string variable)
    
    // Method
    void myMethod() {
        cout << "Hello World" << endl;
    }
};

int main() {
    MyClass myObj;  // Create an object of MyClass
    
    // Access attributes and set values
    myObj.myNum = 15;
    myObj.myString = "Some text";
    
    // Call the method
    myObj.myMethod();
    
    // Print attribute values
    cout << "myNum: " << myObj.myNum << endl;
    cout << "myString: " << myObj.myString << endl;
    
    return 0;
}
