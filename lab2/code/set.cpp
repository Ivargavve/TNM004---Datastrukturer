#include "set.h"
#include "node.h"

int Set::Node::count_nodes = 0;

/*****************************************************
 * Implementation of the member functions             *
 ******************************************************/

 /*
  *  Default constructor :create an empty Set
  */
int Set::get_count_nodes() {
    return Set::Node::count_nodes;
}

/*
 *  Default constructor :create an empty Set
 */
Set::Set() : counter{ 0 } {
    // IMPLEMENT before Lab2 HA
    head = new Node();
    tail = new Node();

    head->next = tail;
    tail->prev = head;

    //Implement nodes inbetween head and tail.
}

/*
 *  Conversion constructor: convert val into a singleton {val}
 */
Set::Set(int val) : Set{} {  // create an empty list
    // IMPLEMENT before Lab2 HA
    insert_node(head, val);
}

/*
 * Constructor to create a Set from a sorted vector of ints
 * Create a Set with all ints in sorted vector list_of_values
 */
Set::Set(const std::vector<int>& list_of_values) : Set{} {  // create an empty list
    // IMPLEMENT before Lab2 HA

    for (long int i = list_of_values.size() - 1; i >= 0; i--) //returns reverse vector
    {
        insert_node(head, list_of_values[i]);
    }
}

/*
 * Copy constructor: create a new Set as a copy of Set S
 * \param S Set to copied
 * Function does not modify Set S in any way
 */
Set::Set(const Set& S) : Set{} {  // create an empty list
    // IMPLEMENT before Lab2 HA
    for (Node* temp = S.tail->prev; temp != S.head; temp = temp->prev) {
            insert_node(head, temp->value);
    }
}

/*
 * Transform the Set into an empty set
 * Remove all nodes from the list, except the dummy nodes
 */
void Set::make_empty() {
    // IMPLEMENT before Lab2 HA

    Node* temp = head;

    while (temp->next != tail) {
        temp = temp->next;
        remove_node(temp->prev);
    }
    head->next = tail;
    tail->prev = head;
}

/*
 * Destructor: deallocate all memory (Nodes) allocated for the list
 */
Set::~Set() {
    // IMPLEMENT before Lab2 HA
    while (head != nullptr) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

/*
 * Assignment operator: assign new contents to the *this Set, replacing its current content
 * \param S Set to be copied into Set *this
 * Call by valued is used
 */
Set& Set::operator=(Set S) {
    // IMPLEMENT before Lab2 HA
    counter = S.counter;
    std::swap(head, S.head);
    std::swap(tail, S.tail);
    return *this;
}

/*
 * Test whether val belongs to the Set
 * Return true if val belongs to the set, otherwise false
 * This function does not modify the Set in any way
 */
bool Set::is_member(int val) const {
    // IMPLEMENT before Lab2 HA
    Node* temp = head->next;
    
    while (temp != tail) {
        if (temp->value == val) {
            return true;
        }
        temp = temp->next;
    }
    
    return false;
}

/*
 * Test whether Set *this and S represent the same set
 * Return true, if *this has same elemnts as set S
 * Return false, otherwise
 */
bool Set::operator==(const Set& S) const {
    Node* temp = head->next;
    Node* temp2 = S.head->next;
    
    // If both sets are empty
    if (temp->next == temp2->next) {
        return true;
    }
    
    // Compares the values of the sets
    while (temp != tail && temp2 != S.tail) {
        if (temp->value == temp2->value) {
            return true;
        }
        temp = temp->next;
        temp2 = temp2->next;
    }
    return false;
}

/*
 * Three-way comparison operator: to test whether *this == S, *this < S, *this > S
 * Return std::partial_ordering::equivalent, if *this == S
 * Return std::partial_ordering::less, if *this < S
 * Return std::partial_ordering::greater, if *this > S
 * Return std::partial_ordering::unordered, otherwise
 */
std::partial_ordering Set::operator<=>(const Set& S) const {
    // Check for size difference
    // Creates 2 counters
    int counter1 = 0;
    int counter2 = 0;
    
    //Loops through both Sets
    for (Node* temp = tail->prev; temp != head; temp = temp->prev) {
        counter1++;
    }
    for (Node* temp = S.tail->prev; temp != S.head; temp = temp->prev) {
        counter2++;
    }
    
    // Compares the counters
    if (counter1 < counter2) {
        return std::partial_ordering::less;
    } else if (counter1 > counter2) {
        return std::partial_ordering::greater;
    } else if (counter1 == counter2) {
        return std::partial_ordering::equivalent;
    } else {
        return std::partial_ordering::unordered;
    }
    
    // Check for value difference
    // Loop to compare every value with itself
    Node* temp = head->next;
    Node* temp2 = S.head->next;
    
    while (temp != tail) {
        if (temp->value == temp2->value) {
            return std::partial_ordering::equivalent;
            temp = temp->next;
            temp2 = temp2->next;
        }
    }
}

/*
 * Modify Set *this such that it becomes the union of *this with Set S
 * Set *this is modified and then returned
 */
Set& Set::operator+=(const Set& S) {
    // Adds the union of the two sets.
    Node* temp = head->next;
    Node* temp2 = S.head->next;
    
    // { 1 3 5 8 }(this)+{ 2 3 7 }(Set) -> { 1, 2, 3, 5, 7, 8 } for example
    // Loops through the set and adds values to S1 if it does not already have it. Sorted.
    while (temp != tail && temp2 != S.tail) {
        if (temp->value < temp2->value){ // if S1 value is < than S2 value go ->next in S1.
            temp = temp->next;
        } else if (temp->value == temp2->value) { // if S1 value is == S2 value, both jump ->next.
            temp = temp->next;
            temp2 = temp2->next;
        } else if (temp->value > temp2->value){ // if S1 value is > than S2 value add S2 to S1.
            insert_node(temp->prev, temp2->value);
            temp2 = temp2->next;
        }
    }
    // Add whatever is left in Set S
    while (temp2->next != nullptr) {
        insert_node(temp->prev, temp2->value);
        temp = temp->next;
        temp2 = temp2->next;
    }
    return *this;
}

/*
 * Modify Set *this such that it becomes the intersection of *this with Set S
 * Set *this is modified and then returned
 */
Set& Set::operator*=(const Set& S) {
    // IMPLEMENT
    Node* temp = head->next;
    Node* temp2 = S.head->next;
    
    // { 1 3 5 8 }(this)*{ 2 3 7 }(Set) -> { 3 } for example
    // Loops through the sets and return the intersection between them.
    while (temp != tail && temp2 != S.tail) {
        if (temp->value == temp2->value) { // If values of this and Set are equal, move to ->next
            temp = temp->next;
            temp2 = temp2->next;
        } else if (temp->value > temp2->value) { // If this is > Set, Set -> next
            temp2 = temp2->next;
        } else if (temp->value < temp2->value) { // removes node if this < Set, this->next
            temp = temp->next;
            remove_node(temp->prev);
        }
    }
    
    // Remove rest of *this if there are more values
    while (temp->next != nullptr) {
        temp = temp->next;
        remove_node(temp->prev);
    }
    return *this;
}

/*
 * Modify Set *this such that it becomes the Set difference between Set *this and Set S
 * Set *this is modified and then returned
 */
Set& Set::operator-=(const Set& S) {
    // Set difference between S1 and S2
    Node* temp = head->next;
    Node* temp2 = S.head->next;
    
    // { 1 3 5 8 }(this)-{ 2 3 7 }(set) -> { 1 5 8 }
    // Loops through the sets
    while(temp != tail && temp2 != S.tail) {
        if (temp->value == temp2->value) { // if values are equal, remove and move on
            temp = temp->next;
            temp2 = temp2->next;
            remove_node(temp->prev);
        } else if (temp->value < temp2->value) { // S1 is smaller than S2, move S1 ->next
            temp = temp->next;
        } else if (temp->value > temp2->value) { // S1 is larger than S2, move S2 ->next
            temp2 = temp2->next;
        }
    }
    return *this;
}

/* ******************************************** *
 * Private Member Functions -- Implementation   *
 * ******************************************** */

 /*
  * Insert a new Node storing val after the Node pointed by p
  * \param p pointer to a Node
  * \param val value to be inserted  after position p
  */
void Set::insert_node(Node* p, int val) {
    // IMPLEMENT before Lab2 HA
    Node* newNode = new Node(val, p->next, p);
    if (p->next != nullptr) {
        p->next->prev = newNode;
        p->next = newNode;
    }

    counter++;
}

/*
 * Remove the Node pointed by p
 * \param p pointer to a Node
 */
void Set::remove_node(Node* p) {
    // IMPLEMENT before Lab2 HA
    if (nullptr) return;

    if (head == p) {
        head = p->next;
    }
    if (p->next != nullptr) {
        p->next->prev = p->prev;
    }
    if (p->prev != nullptr) {
        p->prev->next = p->next;
    }
    delete p;
    counter--;
}

/*
 * Write Set *this to stream os
 */
void Set::write_to_stream(std::ostream& os) const {
    if (is_empty()) {
        os << "Set is empty!";
    }
    else {
        Set::Node* ptr{ head->next };

        os << "{ ";
        while (ptr != tail) {
            os << ptr->value << " ";
            ptr = ptr->next;
        }
        os << "}";
    }
}
