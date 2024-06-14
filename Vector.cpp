#include <iostream>
#include <stdexcept>
#include <cstring>
#include <iterator>

template<typename T>
class Node {
public:
    T Data;
    int Index;

    Node() : Data(T()), Index(-1) {}
    Node(T data, int index) : Data(data), Index(index) {}
};

template<typename T>
class Vector {
private:
    Node<T>* _elements;
    int _count;
    int _capacity;

public:
    Vector(int numberOfElements = 1) : _count(0), _capacity(numberOfElements) {
        _elements = new Node<T>[_capacity];
    }

    Vector(const Node<T>* copy, int size) : _count(size), _capacity(size) {
        if (copy != nullptr) {
            _elements = new Node<T>[_capacity];
            std::memcpy(_elements, copy, size * sizeof(Node<T>));
        }
        else {
            throw std::invalid_argument("Signed list is empty or null");
        }
    }

    ~Vector() {
        delete[] _elements;
    }

    Vector<T> Union(const Vector<T>& vector) const {
        Vector<T> result;

        for (int i = 0; i < _count; ++i) {
            if (!result.Contains(_elements[i].Data)) {
                result.Add(_elements[i].Data);
            }
        }

        for (int i = 0; i < vector._count; ++i) {
            if (!result.Contains(vector._elements[i].Data)) {
                result.Add(vector._elements[i].Data);
            }
        }

        return result;
    }

    Vector<T> Intersect(const Vector<T>& vector) const {
        Vector<T> result;

        for (int i = 0; i < _count; ++i) {
            if (vector.Contains(_elements[i].Data)) {
                result.Add(_elements[i].Data);
            }
        }

        return result;
    }

    Vector<T> Difference(const Vector<T>& vector) const {
        Vector<T> result;

        for (int i = 0; i < _count; ++i) {
            if (!vector.Contains(_elements[i].Data)) {
                result.Add(_elements[i].Data);
            }
        }

        return result;
    }

    bool Contains(const T& data) const {
        for (int i = 0; i < _count; ++i) {
            if (_elements[i].Data == data) {
                return true;
            }
        }
        return false;
    }

    void Add(const T& data) {
        if (data != T()) {
            if (_count == _capacity) {
                RearrangeArray();
            }
            _elements[_count] = Node<T>(data, _count);
            _count++;
        }
    }

    int Capacity() const {
        return _capacity - _count;
    }

    bool Capacity(int requiredElements) const {
        if (requiredElements > 0) {
            return (_capacity - (_count + requiredElements)) > 0;
        }
        else {
            throw std::invalid_argument("The value is out of bounds. It must be greater than 0");
        }
    }

    int MaxSize() const {
        return _capacity;
    }

    void SwapElements(int firstId, int secondId) {
        T temp = _elements[firstId].Data;
        _elements[firstId].Data = _elements[secondId].Data;
        _elements[secondId].Data = temp;
    }

    void Insert(const T& data, int index) {
        if (data != T() && index >= 0 && index <= _count) {
            if (_count == _capacity) {
                RearrangeArray();
            }
            for (int i = _count; i > index; i--) {
                _elements[i] = _elements[i - 1];
            }
            _elements[index] = Node<T>(data, index);
            _count++;
            for (int i = index + 1; i < _count; i++) {
                _elements[i].Index = i;
            }
        }
    }

    void EraseAt(int index) {
        if (index < 0 || index >= _count) {
            std::cout << "Index is out of bounds" << std::endl;
        }
        else {
            for (int i = index; i < _count - 1; i++) {
                _elements[i] = _elements[i + 1];
                _elements[i].Index = i;
            }
            _elements[_count - 1] = Node<T>();
            _count--;
        }
    }

    void EraseFrom(int target) {
        if (target < 0 || target > _count) {
            std::cout << "Index is out of bounds" << std::endl;
        }
        else {
            for (int i = _count; i > target; i--) {
                _elements[i] = Node<T>();
                _count--;
            }
        }
    }

    T* FindElementById(int target) const {
        int left = 0;
        int right = _count - 1;

        while (left <= right) {
            int middle = left + (right - left) / 2;

            if (_elements[middle].Index == target)
                return &_elements[middle].Data;
            if (_elements[middle].Index < target)
                left = middle + 1;
            else
                right = middle - 1;
        }
        std::cout << "Index is out of bounds" << std::endl;
        return nullptr;
    }

    T* GetFirstElement() const {
        if (_capacity > 0 && _elements[0].Data != T()) {
            return &_elements[0].Data;
        }
        else {
            std::cout << "Vector is empty" << std::endl;
            return nullptr;
        }
    }

    T* GetLastElement() const {
        if (_capacity > 0 && _elements[_count - 1].Data != T()) {
            return &_elements[_count - 1].Data;
        }
        else {
            std::cout << "Vector is empty" << std::endl;
            return nullptr;
        }
    }

    void RemoveLast() {
        if (_count > 0) {
            _count--;
            _elements[_count] = Node<T>();
        }
    }

    void ShowData() const {
        for (int i = 0; i < _count; i++) {
            std::cout << _elements[i].Data << std::endl;
        }
    }

    void Clear() {
        delete[] _elements;
        _elements = new Node<T>[_capacity];
        _count = 0;
    }

    void ClearAll() {
        delete[] _elements;
        _capacity = 1;
        _elements = new Node<T>[_capacity];
        _count = 0;
    }

    void Reverse() {
        int leftIndex = 0;
        int rightIndex = _count - 1;

        while (leftIndex < rightIndex) {
            Node<T> temp = _elements[leftIndex];
            _elements[leftIndex] = _elements[rightIndex];
            _elements[rightIndex] = temp;

            leftIndex++;
            rightIndex--;
        }
    }

    T& operator[](int index) {
        if (index < 0 || index >= _count) {
            throw std::out_of_range("Index is out of bounds");
        }
        return _elements[index].Data;
    }

    const T& operator[](int index) const {
        if (index < 0 || index >= _count) {
            throw std::out_of_range("Index is out of bounds");
        }
        return _elements[index].Data;
    }

    bool IsEmpty() const {
        return _count == 0;
    }

private:
    void RearrangeArray() {
        _capacity *= 2;
        Node<T>* newElements = new Node<T>[_capacity];
        for (int i = 0; i < _count; i++) {
            newElements[i] = _elements[i];
        }
        delete[] _elements;
        _elements = newElements;
    }
};

int main() {
    Vector<int> vec(10);

    for (int i = 0; i < 10; ++i) {
        vec.Add(i);
    }
    vec.Reverse();
    vec.ShowData();

    return 0;
}
