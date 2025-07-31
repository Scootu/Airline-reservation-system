# ✈️ Airline Reservation System – C++

A fully-featured airline booking platform built in C++ using Object-Oriented Programming, MySQL integration, and real-world design patterns.

---

## 🚀 Features
✅ **Object-Oriented Architecture**  
- Abstraction, inheritance, polymorphism, Factory & Interface-based patterns.

✅ **User Management**  
- Admin & Customer roles with login/signup system.

✅ **Flight Management**  
- Search flights from multiple airlines (Turkish Airlines, Air Canada).  
- Handles passengers (adults, children, infants).

✅ **Ticketing System**  
- Generates unique ticket IDs.  
- Supports Economic & VIP ticket classes.

✅ **Payment Processing**  
- Integrates PayPal, Stripe, and Square APIs.  
- Unified `IPayment` interface for easy extensions.

✅ **Database Integration**  
- MySQL backend for users, flights, and reservations.

✅ **Itinerary Management**  
- Group multiple reservations under one itinerary.

---

## 🛠️ Technologies Used
- **C++17** (STL, Polymorphism, Dynamic Casting)  
- **MySQL Connector/C++**  
- **Design Patterns:** Factory, Clone, Interface-based architecture  
- **Console-based UI**

---

## 📸 Screenshots
*(Add screenshots here)*

Example:
```plaintext
✅ Database Connected Successfully!  
Welcome to Airline Reservation System!  
1. Login  
2. Customer Sign Up

## Installation & setup 
`
git clone https://github.com/your-username/AirlineReservationSystem.git
cd AirlineReservationSystem

`
## Setup database 
`
CREATE DATABASE cpp_airlinereservation_db;


CREATE TABLE flight_reservations (
    username VARCHAR(50),
    ticket_id VARCHAR(50),
    airline_name VARCHAR(50),
    from_location VARCHAR(50),
    to_location VARCHAR(50),
    date_time_from VARCHAR(50),
    date_time_to VARCHAR(50),
    total_cost DOUBLE
);
`

