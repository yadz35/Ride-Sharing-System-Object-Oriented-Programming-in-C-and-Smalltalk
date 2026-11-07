#include <iomanip>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

class Ride {
private:
    std::string rideID_, pickupLocation_, dropoffLocation_;
    double distance_;

public:
    Ride(std::string id, std::string pickup, std::string dropoff, double miles)
        : rideID_(std::move(id)), pickupLocation_(std::move(pickup)),
          dropoffLocation_(std::move(dropoff)), distance_(miles) {
        if (miles < 0) throw std::invalid_argument("Distance cannot be negative");
    }
    virtual ~Ride() = default;
    double distance() const { return distance_; }
    virtual double fare() const = 0;
    virtual std::string type() const = 0;
    virtual void rideDetails() const {
        std::cout << rideID_ << " | " << type() << " | " << pickupLocation_
                  << " -> " << dropoffLocation_ << " | " << distance_
                  << " miles | $" << std::fixed << std::setprecision(2)
                  << fare() << '\n';
    }
};

class StandardRide final : public Ride {
public:
    using Ride::Ride;
    double fare() const override { return 3.00 + 1.50 * distance(); }
    std::string type() const override { return "Standard"; }
};

class PremiumRide final : public Ride {
public:
    using Ride::Ride;
    double fare() const override { return 6.00 + 2.75 * distance(); }
    std::string type() const override { return "Premium"; }
};

class Driver {
private:
    std::string driverID_, name_;
    double rating_;
    std::vector<std::shared_ptr<Ride>> assignedRides_;
public:
    Driver(std::string id, std::string name, double rating)
        : driverID_(std::move(id)), name_(std::move(name)), rating_(rating) {
        if (rating < 0 || rating > 5) throw std::invalid_argument("Rating must be 0-5");
    }
    void addRide(const std::shared_ptr<Ride>& ride) { assignedRides_.push_back(ride); }
    void getDriverInfo() const {
        std::cout << "Driver " << driverID_ << ": " << name_ << " | Rating: "
                  << rating_ << "/5 | Completed rides: " << assignedRides_.size() << '\n';
    }
};

class Rider {
private:
    std::string riderID_, name_;
    std::vector<std::shared_ptr<Ride>> requestedRides_;
public:
    Rider(std::string id, std::string name)
        : riderID_(std::move(id)), name_(std::move(name)) {}
    void requestRide(const std::shared_ptr<Ride>& ride) { requestedRides_.push_back(ride); }
    void viewRides() const {
        std::cout << "Ride history for " << name_ << " (" << riderID_ << "):\n";
        for (const auto& ride : requestedRides_) ride->rideDetails();
    }
};

int main() {
    auto standard = std::make_shared<StandardRide>("R101", "Campus", "Library", 5.0);
    auto premium = std::make_shared<PremiumRide>("R102", "Airport", "Hotel", 8.0);
    std::vector<std::shared_ptr<Ride>> rides{standard, premium};
    Driver driver("D01", "Alex", 4.9);
    Rider rider("U01", "Jordan");

    std::cout << "=== Ride Sharing System (C++) ===\n";
    for (const auto& ride : rides) {
        rider.requestRide(ride);
        driver.addRide(ride);
        ride->rideDetails();  // Dynamic dispatch calls the correct fare().
    }
    driver.getDriverInfo();
    rider.viewRides();
}
