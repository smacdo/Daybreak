#pragma once

// CRTP template for singleton pattern.
template<typename T>
class Singleton
{
public:
    // Constructor.
    Singleton() = default;

    // Get singleton instance.
    static T& instance()
    {
        static T theInstance;
        return theInstance;
    }
};