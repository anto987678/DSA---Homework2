# Big Homework 2 – Data Structures and Algorithms  
### **Graphs & Binary Trees**

This repository contains the second major project for the **Data Structures and Algorithms** course.  
It focuses on implementing **Graphs** and **Binary Trees** to solve complex, real-world problems using custom C++ data structures (no STL).

---

## Exercises Overview

### **Exercise 1 – Digital Combo (Graph)**
Design a program that generates the shortest possible sequence of digits that will eventually open a digital combination lock, regardless of the correct password.

#### Description
- The lock checks the last `n` digits after each new digit is entered.  
- The goal is to create a sequence that contains every possible combination of length `n` for digits `[0, k−1]`.  
- This problem is equivalent to generating a **De Bruijn sequence** of order `n` over an alphabet of size `k`.
  
### **Exercise 2 – Castle Defence (Binary Tree)**
Simulate Daenerys Targaryen’s empire and determine the **minimum number of dragons** needed to protect all cities in a hierarchical network.

#### Description
- Each city is represented as a node in a **binary tree**.  
- A dragon placed in a city protects:
  - The city itself  
  - Its ruler (parent)  
  - Its subordinate cities (children)  
- The goal is to deploy as few dragons as possible while ensuring every city is protected.

### **Exercise 3 – Core Scheduling: Minimizing Energy with Deadlines & Dependencies (Graph)**
Create a **task scheduler** that minimizes total energy consumption on a multi-core system while respecting task dependencies and deadlines.

#### Description
Each task has:
- A workload and a deadline  
- Possible dependencies on other tasks  

Each core has:
- A speed  
- A power consumption rate  

#### Features
- Detect and handle dependency cycles:  
  - 2-node cycles are handled by parallel scheduling on different cores.  
  - Larger cycles make scheduling impossible.  
- Assign tasks to cores efficiently to reduce total energy:  
  - `Energy = Execution Time × Power Rate`  
- Generate a detailed schedule showing task order, core allocation, and total energy usage.


