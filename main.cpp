/*
 * Shared toy ptr like shared ptr
 */
#include <iostream>
#include <string>


class Toy {
public:
    Toy () : name("Some toy") {
        std::cout << "New toy" << std::endl;
    };
    Toy (std::string_view newName) : name(newName){
        std::cout << "New toy" << std::endl;
    };
    ~Toy() {
       std::cout << "Destroy toy" << std::endl;
    };

    std::string_view getName() {
        return name;
    }
private:
    std::string name;
};

class shared_ptr_toy {
private:

    bool blocked = false;
    int uses = 0;
    Toy *t = nullptr;
    shared_ptr_toy *main = nullptr;

public:
    shared_ptr_toy(std::string_view ToyName) {
        t = new Toy(ToyName);
    }

    shared_ptr_toy() {
        t = nullptr;
    }

    shared_ptr_toy(Toy *newToy) {
        t = newToy;
    }

    shared_ptr_toy(shared_ptr_toy& other) {
        if (other.blocked) {
            std::cout << "shared ptr has been blocked!" << std::endl;
            return;
        }

        main = &other;
        main->uses += 1;
        t = main->t;
    }

    shared_ptr_toy& operator=(shared_ptr_toy& other) {
        if (other.blocked) {
            std::cout << "shared ptr has been blocked!" << std::endl;
            return *this;
        }

        if (this == &other)
            return *this;

        main = &other;
        main->uses += 1;
        if (t != nullptr)
            delete t;

        t = main->t;
        return *this;
    };

    ~shared_ptr_toy() {
        //std::cout <<  "Destruct shared_ptr_toy " << this << " uses " << uses << std::endl;
        //std::cout <<  "Toy: " << t << std::endl;

        if (main == nullptr) delete t;
        else t = nullptr;
    };

    void reset() {
        blocked = true;
    }
};

class Dog {
private:
    shared_ptr_toy lvlToy;
public:
    Dog() {};
    Dog(std::string_view name): lvlToy(name) {};
    Dog(shared_ptr_toy &t): lvlToy(t) {};

    void addToy(shared_ptr_toy &newToy) {
        lvlToy = newToy;
    }

    ~Dog() {std::cout << "Dog destroy" << std::endl;};
};

shared_ptr_toy make_shared_toy(Toy *newToy) {
    return shared_ptr_toy(newToy);
};


shared_ptr_toy make_shared_toy(const std::string_view &name) {
    return shared_ptr_toy(name);
};


int main() {

    auto toy_ptr = shared_ptr_toy("Ball");
    Dog *d = new Dog(toy_ptr);
    Dog *d1 = new Dog(toy_ptr);
    delete d;
    delete d1;


    /*
    Toy *toy = new Toy;
    auto t = make_shared_toy(toy);
    auto t2 = make_shared_toy(toy);
    delete toy;
    */

    /*
    auto t = make_shared_toy("Ball");
    Dog *d = new Dog(t);
    delete d;
     */

    return 0;
}
