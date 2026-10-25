#include <iostream>

int main() {
    unsigned int number = 4;
    unsigned int evil_number = number - 1;

    if (number!=0 &
        // (number | evil_number) == number+evil_number) // First attempt
        (number & evil_number) == 0){
        std::cout << "Number is a power of 2" << std::endl;
    } else {
        std::cout << "Number is not a power of 2" << std::endl;
    }
    return 0;
}