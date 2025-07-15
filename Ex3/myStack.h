#include <iostream>

using namespace std;

#define NMAX 50
template<typename T>
class Stack {
    private:
        T stackArray[NMAX]; // an array of NMAX dimension
        int topLevel; // the top of the stack, representing the INDEX of last element of the
                    //stackArray:0, 1, 3,....

    public:
        void push(T x) // puts an element in the stack array
        {
            if (topLevel >= NMAX-1) //check if the stack array has the maximum dimension
			{
                cout<<"The stack is full: we have already NMAX elements!\n";
                //exit the function without making anything
                return;
            }
        	/* add an element=> the index of the last element of the stack Array
        	increases and put the value of the new element in the stack array */
            stackArray[++topLevel] = x;
        }

        int isEmpty()
        {
            //returns 1, if topLevel>=0, meaning the stack array has elements
            // returns 0, otherwise
            return (topLevel < 0);
        }

        T pop() // extracts and element from the stack array and returns the new top
        {
            if (isEmpty())
            {
            // the extraction is made only if the array is not empty
                cout<<"The stack is empty! \n";
                T x;
                return x;
            }
            return stackArray[topLevel--]; 	// the topLevel decreases and the new top is changed
            //difference return stackArray[--topLevel] ?
        }

        T peek()
        {
        // returns the top of the stack
            if (isEmpty())
            {
            // the extraction is made only if the array is not empty
                cout<<"The stack is empty! \n";
                T x;
                return x;
        	}
            return stackArray[topLevel];
        }

        int getTopLevel()
        {
            return topLevel;
        }
        Stack()
        { // constructor
            topLevel = -1; //the stack is empty in the beginning

        }

        ~Stack()
        { // destructor
        }
};