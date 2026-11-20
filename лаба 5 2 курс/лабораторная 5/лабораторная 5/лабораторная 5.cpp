#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <cstdlib>
#include <ctime>

using namespace std;

// 1. СТЕК (Stack) - на основе динамического массива

template <typename T>
class Stack {
private:
    T* data;        // Указатель на массив данных
    int topIndex;   // Индекс вершины (последнего элемента)
    int capacity;   // Вместимость стека

    // Вспомогательная функция для расширения массива
    void resize() {
        int newCapacity = capacity * 2;
        T* newData = new T[newCapacity];
        for (int i = 0; i <= topIndex; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

public:
    // Конструктор
    Stack(int initialCapacity = 10) : capacity(initialCapacity), topIndex(-1) {
        data = new T[capacity];
    }

    // Деструктор (освобождаем память)
    ~Stack() {
        delete[] data;
    }

    // Добавить элемент на вершину
    void push(const T& value) {
        if (topIndex >= capacity - 1) {
            resize();
        }
        data[++topIndex] = value;
    }

    // Удалить и вернуть верхний элемент
    T pop() {
        if (isEmpty()) throw underflow_error("Стек пуст!");
        return data[topIndex--];
    }

    // Посмотреть верхний элемент
    T top() const {
        if (isEmpty()) throw underflow_error("Стек пуст!");
        return data[topIndex];
    }

    // Проверка на пустоту
    bool isEmpty() const {
        return topIndex == -1;
    }
};

// Функция проверки правильности расстановки скобок
bool checkBrackets(const string& expr) {
    Stack<char> stack;
    for (char ch : expr) {
        // Если открывающая скобка - кладем в стек
        if (ch == '(' || ch == '[' || ch == '{') {
            stack.push(ch);
        }
        // Если закрывающая - проверяем
        else if (ch == ')' || ch == ']' || ch == '}') {
            if (stack.isEmpty()) return false; // Стек пуст, а скобка закрывается - ошибка
            char top = stack.pop();
            // Проверяем соответствие
            if ((ch == ')' && top != '(') ||
                (ch == ']' && top != '[') ||
                (ch == '}' && top != '{')) {
                return false;
            }
        }
    }
    // Если в конце стек пуст - все скобки закрыты корректно
    return stack.isEmpty();
}

// 2 Очередь (Queue) - на основе кольцевого буфера

template <typename T>
class Queue {
private:
    T* data;
    int capacity;
    int frontIndex; // Индекс начала
    int rearIndex;  // Индекс конца
    int count;      // Текущее количество элементов

    void resize() {
        int newCapacity = capacity * 2;
        T* newData = new T[newCapacity];
        for (int i = 0; i < count; i++) {
            newData[i] = data[(frontIndex + i) % capacity];
        }
        delete[] data;
        data = newData;
        frontIndex = 0;
        rearIndex = count;
        capacity = newCapacity;
    }

public:
    Queue(int initialCapacity = 10)
        : capacity(initialCapacity), frontIndex(0), rearIndex(0), count(0) {
        data = new T[capacity];
    }

    ~Queue() {
        delete[] data;
    }

    // Добавить в конец
    void enqueue(const T& value) {
        if (count >= capacity) resize();
        data[rearIndex] = value;
        rearIndex = (rearIndex + 1) % capacity; // Зацикливание
        count++;
    }

    // Удалить из начала
    T dequeue() {
        if (isEmpty()) throw underflow_error("Очередь пуста!");
        T value = data[frontIndex];
        frontIndex = (frontIndex + 1) % capacity; // Зацикливание
        count--;
        return value;
    }

    T front() const {
        if (isEmpty()) throw underflow_error("Очередь пуста!");
        return data[frontIndex];
    }

    bool isEmpty() const { return count == 0; }
    int size() const { return count; }
};

// Симуляция очереди в банке
void simulateBank(int numClients) {
    cout << "\n Симуляция банка " << endl;
    Queue<int> bankQueue;
    int totalWaitTime = 0;
    int currentTime = 0;

    // Клиенты приходят и встают в очередь (время обслуживания случайно от 1 до 5)
    for (int i = 1; i <= numClients; i++) {
        int serviceTime = rand() % 5 + 1;
        bankQueue.enqueue(serviceTime);
        cout << "Клиент " << i << " встал в очередь. Время обслуживания: " << serviceTime << " мин." << endl;
    }

    cout << "\nНачинаем обслуживание " << endl;
    int clientNum = 1;
    while (!bankQueue.isEmpty()) {
        int serviceTime = bankQueue.dequeue();
        totalWaitTime += currentTime;
        currentTime += serviceTime;
        cout << "Клиент " << clientNum << " обслужен за " << serviceTime << " мин. (Текущее время: " << currentTime << ")" << endl;
        clientNum++;
    }
    cout << "Общее время работы банка: " << currentTime << " минут." << endl;
}



// 3 Дек (Deque) - на основе двусвязного списка

template <typename T>
class Deque {
private:
    struct Node {
        T data;
        Node* prev;
        Node* next;
        Node(const T& value) : data(value), prev(nullptr), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    int count;

public:
    Deque() : head(nullptr), tail(nullptr), count(0) {}

    ~Deque() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void push_front(const T& value) {
        Node* newNode = new Node(value);
        if (head == nullptr) {
            head = tail = newNode;
        }
        else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        count++;
    }

    void push_back(const T& value) {
        Node* newNode = new Node(value);
        if (tail == nullptr) {
            head = tail = newNode;
        }
        else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        count++;
    }

    T pop_front() {
        if (isEmpty()) throw underflow_error("Дек пуст!");
        Node* temp = head;
        T value = temp->data;
        head = head->next;
        if (head != nullptr) head->prev = nullptr;
        else tail = nullptr;
        delete temp;
        count--;
        return value;
    }

    T pop_back() {
        if (isEmpty()) throw underflow_error("Дек пуст!");
        Node* temp = tail;
        T value = temp->data;
        tail = tail->prev;
        if (tail != nullptr) tail->next = nullptr;
        else head = nullptr;
        delete temp;
        count--;
        return value;
    }

    bool isEmpty() const { return count == 0; }
    int size() const { return count; }
};

// Проверка на палиндром с помощью дека
bool isPalindrome(const string& str) {
    Deque<char> dq;
    // Заполняем дек символами
    for (char ch : str) {
        dq.push_back(ch);
    }
    // Сравниваем символы с концов
    while (dq.size() > 1) {
        if (dq.pop_front() != dq.pop_back()) {
            return false;
        }
    }
    return true;
}



// 4 Приоритетная очередь (PriorityQueue) - на основе кучи (max-heap)

template <typename T>
class PriorityQueue {
private:
    T* heap;
    int count;
    int capacity;

    void resize() {
        int newCapacity = capacity * 2;
        T* newHeap = new T[newCapacity];
        for (int i = 0; i < count; i++) newHeap[i] = heap[i];
        delete[] heap;
        heap = newHeap;
        capacity = newCapacity;
    }

    // Просеивание вверх (для Max-Heap)
    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (heap[index] > heap[parent]) {
                swap(heap[index], heap[parent]);
                index = parent;
            }
            else break;
        }
    }

    // Просеивание вниз (для Max-Heap)
    void heapifyDown(int index) {
        while (true) {
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            int largest = index;

            if (left < count && heap[left] > heap[largest]) largest = left;
            if (right < count && heap[right] > heap[largest]) largest = right;
            if (largest == index) break;

            swap(heap[index], heap[largest]);
            index = largest;
        }
    }

public:
    PriorityQueue(int initialCapacity = 10)
        : capacity(initialCapacity), count(0) {
        heap = new T[capacity];
    }

    ~PriorityQueue() { delete[] heap; }

    void push(const T& value) {
        if (count >= capacity) resize();
        heap[count] = value;
        heapifyUp(count);
        count++;
    }

    T pop() {
        if (isEmpty()) throw underflow_error("Приоритетная очередь пуста!");
        T maxValue = heap[0];
        heap[0] = heap[count - 1];
        count--;
        heapifyDown(0);
        return maxValue;
    }

    bool isEmpty() const { return count == 0; }
    int size() const { return count; }
};

// Поиск K наибольших элементов
vector<int> findKLargest(const vector<int>& arr, int k) {
    PriorityQueue<int> pq; // Используем Max-Heap
    for (int x : arr) {
        pq.push(x);
    }

    vector<int> result;
    // Извлекаем K максимальных элементов
    for (int i = 0; i < k && !pq.isEmpty(); i++) {
        result.push_back(pq.pop());
    }
    return result;
}


int main() {
    setlocale(LC_ALL, "Russian"); 
    srand(time(0)); // Инициализация генератора случайных чисел

    // Задание 1: стек
    cout << " Задание 1: Проверка скобок" << endl;
    string expr1 = "[{}]";
    string expr2 = "([{}])";
    string expr3 = "[{]}";
    string expr4 = "((()))";
    string expr5 = "([)]";

    cout << expr1 << " -> " << (checkBrackets(expr1) ? "ВЕРНО" : "НЕВЕРНО") << endl;
    cout << expr2 << " -> " << (checkBrackets(expr2) ? "ВЕРНО" : "НЕВЕРНО") << endl;
    cout << expr3 << " -> " << (checkBrackets(expr3) ? "ВЕРНО" : "НЕВЕРНО") << endl;
    cout << expr4 << " -> " << (checkBrackets(expr4) ? "ВЕРНО" : "НЕВЕРНО") << endl;
    cout << expr5 << " -> " << (checkBrackets(expr5) ? "ВЕРНО" : "НЕВЕРНО") << endl;

    // Задание 2: Очередь
    simulateBank(5); // Симулируем 5 клиентов

    // Задание 3: дек
    cout << "\n=== Задание 3: палидром" << endl;
    string str1 = "radar";
    string str2 = "hello";
    string str3 = "level";

    cout << str1 << " -> " << (isPalindrome(str1) ? "ПАЛИНДРОМ" : "НЕ ПАЛИНДРОМ") << endl;
    cout << str2 << " -> " << (isPalindrome(str2) ? "ПАЛИНДРОМ" : "НЕ ПАЛИНДРОМ") << endl;
    cout << str3 << " -> " << (isPalindrome(str3) ? "ПАЛИНДРОМ" : "НЕ ПАЛИНДРОМ") << endl;

    // Задание 4: приоритетная очередь
    cout << "\n Задание 4: K наибольших элементов " << endl;
    vector<int> arr = { 3, 1, 5, 12, 2, 11, 8, 7 };
    int k = 3;

    cout << "Исходный массив: ";
    for (int x : arr) cout << x << " ";
    cout << endl;

    vector<int> topK = findKLargest(arr, k);
    cout << "Топ " << k << " наибольших элементов: ";
    for (int x : topK) cout << x << " ";
    cout << endl;

    return 0;
}