# Travel Agency Management System (C++ OOP)

A robust, object-oriented console application developed in Modern C++ simulating a full-fledged Travel Agency ecosystem. The system features role-based access control (Visitor vs. Organizer), dynamic booking management, transport and accommodation selection, and integrated statistics tracking.

---

## Key Features

### Role-Based Access Control
Upon launch, users authenticate and navigate through tailored interactive menus based on their assigned role:

#### **1. Visitor Interface**
* **Browse & Filter Offers:** View all packages or filter dynamically by destination, price range, dates, or specific categories (*Mountain City Breaks, Beach Getaways, Cruises*).
* **Smart Booking System:** Reserve vacation packages with real-time seat availability limits.
* **Transport & Accommodation Selection:** Interactively select preferred transport (bus, flight seats) and view detailed hotel amenities/star ratings.
* **Ticket Management:** View booked tickets and cancel specific transport reservations with seat release logic.
* **Price Sorting:** Sort available packages in ascending order for budget optimization.
* **Gamified Loyalty System:** Earn 50 reward points per booking; automatically unlocks surprise rewards upon reaching 100 points.
* **Interactive Mini-Game:** *"Flight Agency Race"* — an interactive mini-simulation comparing flight speeds across various airlines.

#### **2. Organizer / Admin Interface**
* **Package Management:** Add new vacation packages with custom parameters (destination, date, price, available tickets, type).
* **Inventory Overview:** Track and list all active, fully booked, or archived packages.

#### **3. Analytics & Statistics Module**
* Total tickets sold across all categories.
* Most popular destinations and most booked vacation types.
* Total loyalty points accumulated across all user profiles.

---

## Technical Architecture & C++ Concepts

This project demonstrates advanced Object-Oriented Programming (OOP) paradigms and Modern C++ best practices:

| Concept | Implementation Details |
| :--- | :--- |
| **Polymorphism & Virtual Functions** | Abstract base classes driving dynamic dispatch for varied vacation package types (*Cruises, City Breaks*). |
| **Inheritance & Class Hierarchies** | Clean structural relationships, leveraging `upcasting` and `downcasting` for domain-specific behavior. |
| **Memory Management** | Usage of **Smart Pointers** (`std::unique_ptr`, `std::shared_ptr`) ensuring strict ownership semantics and zero memory leaks. |
| **Standard Template Library (STL)** | Heavy utilization of `std::vector`, `std::map`, and custom templates for generic data structure handling and sorting algorithms. |
| **Operator Overloading** | Overloaded stream operators (`<<`, `>>`) and comparison operators for seamless console I/O and custom sorting. |

---

### Prerequisites
* A C++ compiler supporting **C++17** or higher (`g++`, `clang++`, or MSVC).
* CMake (optional, for build systems) or any modern IDE (VS Code, CLion, Visual Studio).

### Build & Execution

1. **Clone the repository:**
   ```bash
   git clone https://github.com/maracaramidaru/proiect.poo.git
   cd proiect.poo
