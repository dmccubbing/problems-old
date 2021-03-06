/****************************************************************************
 * DATA_STRUCTURES
 *
 * Computer Science 50
 * Christopher Bartholomew
 * Data Structure Solutions Created by: Nate Hardison 
 * 
 * Choose one of the three methods to create various data structures using lights on the arduino board.
 *
 * Demonstrates use of stacks/queues/single linked lists
 * 
 * BONUS:
 * 1. Create one using a Hash Table / Hash Function
 ***************************************************************************/
 
#include <stdlib.h>
#include <avr-libc.h>
#include <arduino.h>
#define MAX_PINS 6
#define EMPTY 0

int PIN_COUNTER = 2;

void setup()
{
	// begin reading from serial
	Serial.begin(9600); 
    
        // init pins
        init_pinmode();
        
        digitalWrite(1, LOW);
        
	// give user options
	display_menu();      
}

/*********************************************************************************************************************************************
 * TODO: Queue
 **********************************************************************************************************************************************/
 
// data structure that is used for Queue 
typedef struct
{
    // the index of the first element in the queue
    int head;

    // storage for the elements in the queue
    int pins[MAX_PINS];

    // the size of the queue
    int size;
}
queue;

// queue pin config
int queue_pins[MAX_PINS] = { 2, 3, 4, 5, 6, 7 };
// declare a queue (as a global variable)
queue q;

bool enqueue(int num)
{
      // if we're at capacity, we can't add anything more to our queue
    if (q.size == MAX_PINS)
    {
        return false;
    }

    // calculate the index of the "last" slot in the queue
    int last = (q.head + q.size) % MAX_PINS;

    // add the new element to our queue
    q.pins[last] = num;
    
    // toggle the light
    toggle(num);
    
    // adjust the size appropriately
    q.size++;

    return true;
}

bool dequeue(void)
{
      // if nothing's in our queue, we can't retrieve anything!
    if (q.size == 0)
    {
        return NULL;
    }

    // grab the first element in our queue
    int first = q.pins[q.head];

    // adjust the head of our queue to point to the next element
    // wrap back to the beginning if necessary
    q.head = (q.head + 1) % MAX_PINS;
    
    // toggle the light
    toggle(first);
    
    // adjust the size appropriately
    q.size--;

    return first;
}

/*********************************************************************************************************************************************
 * END
 **********************************************************************************************************************************************/

/*********************************************************************************************************************************************
 * TODO: Stack
 **********************************************************************************************************************************************/
 
 // Data Structure that is used for Stack
typedef struct
{
    // storage for the elements in the stack
    int pins[MAX_PINS];

    // the number of elements currently in the stack
    int size;
}
stack;

// stack pin config
int stack_pins[MAX_PINS] = { 7, 6, 5, 4, 3, 2 };

// declare a stack (as a global variable)
stack s;

// global variable used for serial read
int incomingByte = 0;  

bool push(int num)
{
      // if we're at capacity, we can't add anything more to our stack
    if (s.size == MAX_PINS)
    {
        return false;
    }

    // add the new element to our stack
    s.pins[s.size] = num;
    
    // toggle the light
    toggle(num);
    
    // adjust the size appropriately
    s.size++;

    return true;
}

int pop()
{
  
    // if nothing's in our stack, we can't retrieve anything!
    if (s.size == 0)
    {
        return NULL;
    }

    // grab the last element in our stack
   int last = s.pins[s.size - 1];
    
    // toggle the light
    toggle(last);
    
    // adjust the size accordingly
    s.size--;

    return last;
}
/*********************************************************************************************************************************************
 * END
 **********************************************************************************************************************************************/

/*********************************************************************************************************************************************
 * TODO: Linked List
 **********************************************************************************************************************************************/
 
 // data strcuture that is used for Link List
typedef struct led_node
{
        // the pin 
	int pin; 

        // the next pin
 	struct led_node* next;
}
led_node;

//  single linked list pin config
int base_pins[5] = { 2, 3, 5, 6, 7 }; 

// for "insert after" set the previous pin
int previous_pin = 3;

// the pin you want to insert
int insert_pin   = 4;

// the counter that is used as the loader
int load_counter = 0;

// declare the very first "led_node" in our list
led_node* first = NULL;

/**
 * Helper function to build a new node. We'll use it in all of our
 * functions below that add nodes to the list.
 */
static struct led_node* build_node(int i)
{
    led_node* new_node = (led_node *)malloc(sizeof(led_node));
    if (new_node == NULL)
    {
        Serial.println("sll: error: Out of heap memory\n");
    }
    new_node->pin  = i;
    new_node->next = NULL;
    return new_node;

}

/**
 * Puts a new node containing i at the front (head) of the list.
 */
void prepend(int i)
{
    led_node* new_node = build_node(i);

    // the first node of our list will be the second node of our new one
    new_node->next = first;
    
    // toggle light
    toggle(i);
    
    // our new node is now going to be the first node
    first = new_node;
}

/**
 * Inserts a new node containing int i following node n.
 */
static void insert_after(led_node* n, int i)
{
    if (n != NULL)
    {
        led_node* new_node = build_node(i);
        new_node->next = n->next;
        n->next = new_node;
        
        // toggle 
        toggle(i);
    }
}

void append(int i)
{
    // track the previous node to the current one as well, since it's
    // otherwise impossible to insert *before* the current node
    led_node* prev = NULL;
    for (led_node* cur = first; cur != NULL; cur = cur->next)
    {
        // make sure to update the prev pointer on every iteration!
        prev = cur;
    }

    // if prev is NULL, that means that the element belongs first
    if (prev == NULL)
    {
        prepend(i);
    }
    else
    {
        // aren't you glad that you already wrote this one? ;-)
        insert_after(prev, i);
        
    }
   
}

/**
 * Returns true if a node in the list contains the value i and false
 * otherwise.
 */
struct led_node* contains(int i)
{
    for (led_node* n = first; n != NULL; n = n->next)
    {
        if (n->pin == i)
        {
            return n;
        }
    }
    return NULL;    
}

/**
 * Returns the length of the list.
 */
int length(void)
{
    int length = 0;
    for (led_node* n = first; n != NULL; n = n->next)
    {
        length++;
    }
    return length;
}

/*********************************************************************************************************************************************
 * END
 **********************************************************************************************************************************************/

void display_menu()
{
	int menu_digit = 0;
        int input = 0;
	Serial.println("=========MAIN MENU==========");
  	Serial.println("1. add pins 	(Linked List Only)");
	Serial.println("2. insert pins  (Linked List Only)");
	Serial.println("3. push 	(Stack Only)");
	Serial.println("4. pop  	(Stack Only)");
	Serial.println("5. enqueue 	(Queue Only)");
	Serial.println("6. dequeue 	(Queue Only)");
	Serial.println("9. bye");
	Serial.println("==========================");
	
       
	do
  	{
    	// send data only when you receive data:
     	if (Serial.available() > 0) 
     	{
      		// read the incoming byte:
      		incomingByte = Serial.read();
    		
			// assign actual digit value to variable
			menu_digit = incomingByte - '0';
			
			switch(menu_digit)
			{
				case 1:
					Serial.println("adding pins");
					// use this to create first node if length is zero
                                        
                                        if(length() == base_pins[load_counter])
                                        {
                                           prepend(base_pins[load_counter]);
                                           load_counter++;
                                        } 
                                        else
                                        {
                                           append(base_pins[load_counter]);
                                           load_counter++;
                                        }
                                        // use this to add additional nodes
                                        //
                                        display_menu();
				break;
				case 2:
					Serial.println("inserting pins");
                                        // contains will send back a led_node structure
                                        if(contains(previous_pin) != NULL)
                                        {
                                          // insert the the new pin after the previous pin
					  insert_after(contains(previous_pin), insert_pin);
                                        }
    
                                        display_menu();
				break;
				case 3:
					Serial.println("Pushing Pins On");
                                        if(push(PIN_COUNTER))
                                        {
                                          Serial.println("SUCCESS!");
                                          if(PIN_COUNTER != 7)
                                          {
                                            PIN_COUNTER++;
                                          }
                                        } 
                                        else
                                       {
                                          Serial.println("AT MAX!");                                      
                                       }
                                        Serial.println(PIN_COUNTER);
                                        display_menu();
				break;
				case 4:
					Serial.println("Popping Pins Off");
                                        pop();
                                        display_menu();
				break;
				case 5:
					Serial.println("Enqueue Pins");                                      
                                        if(enqueue(PIN_COUNTER))
                                        {
                                          Serial.println("SUCCESS!");
                                          if(PIN_COUNTER != 7)
                                          {
                                            PIN_COUNTER++;
                                          }
                                        } 
                                        else
                                       {
                                          Serial.println("AT MAX!");                                      
                                       }
                                        Serial.println(PIN_COUNTER);
                                        display_menu();
				break;
				case 6:
					// toggle pin
					Serial.println("Dequeue Pins");
                                        dequeue();
                                        display_menu();
				break;
				case 9:
					Serial.println("Bye!");
                                        // linked list only
                                        free_nodes();
				break;
				default:
				break;
			}
		
    	}
	// continue to prompt until max pins have been reached
  	} while(menu_digit != 9);

}

void init_pinmode()
{
    for(int i=1;i<=7;i++)
        pinMode(i,OUTPUT);
}

void toggle(int pin)
{
  (digitalRead(pin) == HIGH) ? digitalWrite(pin, LOW) : digitalWrite(pin, HIGH);
}

void free_nodes()
{
    // toggle the first pin before freeing it
    toggle(first->pin);
    
    while (first != NULL)
    {
        // get the node
        led_node* next = first->next;      
        // toggle the light 
        toggle(next->pin);
        // free the memory
        free(first);
        // make the "next" node the "first" node
        first = next;
    }
}

void loop()
{
		

}

