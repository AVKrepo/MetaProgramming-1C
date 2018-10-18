#include <cstdlib>
#include <iostream>
#include <set>
#include <vector>


enum class Mode {
    Proxy,
    Mediator,
    Observer
};

template<Mode>
class C {
};

struct Response {
    // contains some useful information (response from service to client)
};

// Interface for service
class InterfaceB {
public:
    virtual Response respond() = 0;
};

// Interface for observer
class InterfaceA {
public:
    virtual void beNotified() = 0;
};

// Interface for notifier
class InterfaceC {
public:
    virtual void addSubscriber(InterfaceA *subscriber) = 0;

    virtual void removeSubscriber(InterfaceA *subscriber) = 0;

    virtual void notifySubscribers() = 0;
};


// class B
// in case of patterns proxy and mediator B is a service
// in case of pattern observer B is an observable
class B : public InterfaceB {
public:
    B(InterfaceC *notifier_ptr) : notifier_ptr_(notifier_ptr) {}

    B() {
        B(nullptr);
    }

    virtual Response respond() {
        std::cout << "Service handles request" << std::endl;
        return Response();
    }

    void changeState() {
        std::cout << "Observable changed its state" << std::endl;
        notifier_ptr_->notifySubscribers();
    }

private:
    InterfaceC *notifier_ptr_;
};


// class A
// in case of patterns proxy and mediator A is a client
// in case of pattern observer B is an observer
class A : public InterfaceA {
public:

    void request(InterfaceB &service) {
        std::cout << "Client calls service" << std::endl;
        Response response = service.respond();
    }

    virtual void beNotified() {
        std::cout << "Received notification" << std::endl;
    }
};


template<>
class C<Mode::Proxy> : public InterfaceB {
public:
    C(InterfaceB &service) : real_service_(service) {
    }

    virtual Response respond() {
        // Here proxy may do some additional work
        std::cout << "Proxy redirects operation to real service" << std::endl;
        return real_service_.respond();
    }

private:
    InterfaceB &real_service_;
};


template<>
class C<Mode::Mediator> : public InterfaceB {
public:
    C(std::vector<InterfaceB *> services) : services_(services) {
    }

    virtual Response respond() {
        // Here mediator chooses appropriate service and redirects request to it
        srand(time(NULL));
        unsigned long index = rand() % services_.size();
        std::cout << "Mediator redirects operation to real service #" << index << std::endl;
        return services_[index]->respond();
    }

private:
    std::vector<InterfaceB *> services_;
};

template<>
class C<Mode::Observer> : public InterfaceC {
public:
    C() {}

    virtual void addSubscriber(InterfaceA *subscriber) {
        subscribers_.insert(subscriber);
    }

    virtual void removeSubscriber(InterfaceA *subscriber) {
        subscribers_.erase(subscriber);
    }

    virtual void notifySubscribers() {
        std::cout << "Notifier notifies all subscribers" << std::endl;
        for (auto subscriber : subscribers_) {
            subscriber->beNotified();
        }
    }

private:
    std::set<InterfaceA *> subscribers_;
};

void testProxy() {
    std::cout << "Test proxy:" << std::endl;
    B service = B();
    C<Mode::Proxy> proxy = C<Mode::Proxy>(service);
    A client = A();
    client.request(proxy);
    std::cout << std::endl;
}

void testMediator() {
    std::cout << "Test mediator:" << std::endl;
    B service1 = B();
    B service2 = B();
    std::vector<InterfaceB *> services = {&service1, &service2};
    C<Mode::Mediator> mediator = C<Mode::Mediator>(services);
    A client = A();
    client.request(mediator);
    std::cout << std::endl;
}

void testObserver() {
    std::cout << "Test observer:" << std::endl;
    A subscriber1 = A();
    A subscriber2 = A();
    C<Mode::Observer> notifier = C<Mode::Observer>();
    B publisher = B(&notifier);
    notifier.addSubscriber(&subscriber1);
    notifier.addSubscriber(&subscriber2);
    publisher.changeState();
    std::cout << std::endl;
}


int main() {
    testProxy();
    testMediator();
    testObserver();
    return 0;
}