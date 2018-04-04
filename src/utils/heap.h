#ifndef HEAP_H
#define HEAP_H

#include <vector>
#include <functional>
#include <utility>
#include <algorithm>
#include <stdexcept>
#include <unordered_map>
template <
         typename T,
         typename KComparator = std::equal_to<T>,
         typename PComparator = std::less<T>,
         typename Hasher = std::hash<T> >
class Heap
{
public:
    /// Constructs an m-ary heap. M should be >= 2
    Heap(int m = 2,
         const PComparator& c = PComparator(),
         const Hasher& hash = Hasher(),
         const KComparator& kcomp = KComparator()  );

    /// Destructor as needed
    ~Heap();

    /// Adds an item
    void push(const T& item);

    /// returns the element at the top of the heap
    ///  max (if max-heap) or min (if min-heap)
    T const & top() const;

    /// Removes the top element
    void pop();

    /// returns true if the heap is empty
    bool empty() const;

    /// decreaseKey - Finds key matching old object &
    /// updates its location in the heap based on the new value
    void decreaseKey(const T& old, const T& newVal);

 private:
    /// Add whatever helper functions and data members you need below
    void trickleUp(int index);
    void trickleDown(int index);
    int getNthChildIndex(int index, int n);
    int getParentIndex(int index);


 private:
   /// Data members - These should be sufficient
    std::vector< T > store_;
    int m_;
    PComparator c_;
    std::unordered_map<T, size_t, Hasher, KComparator> keyToLocation_;

};


// Find index helpers
template <typename T, typename KComparator, typename PComparator, typename Hasher >
int Heap<T,KComparator,PComparator,Hasher>::getNthChildIndex(int index, int n){
	return m_*index + n;
}


template <typename T, typename KComparator, typename PComparator, typename Hasher >
int Heap<T,KComparator,PComparator,Hasher>::getParentIndex(int index){
	return (index - 1)/m_;
}



template <typename T, typename KComparator, typename PComparator, typename Hasher >
Heap<T,KComparator,PComparator,Hasher>::Heap(
    int m,
    const PComparator& c,
    const Hasher& hash,
    const KComparator& kcomp ) :

    store_(),
    m_(m),
    c_(c),
    keyToLocation_(100, hash, kcomp)

{

}


template <typename T, typename KComparator, typename PComparator, typename Hasher >
Heap<T,KComparator,PComparator,Hasher>::~Heap()
{

}


template <typename T, typename KComparator, typename PComparator, typename Hasher >
void Heap<T,KComparator,PComparator,Hasher>::push( const T& item)
{
	store_.push_back(item);
	keyToLocation_[item] = store_.size() - 1;
	trickleUp(store_.size() - 1);
}

template <typename T, typename KComparator, typename PComparator, typename Hasher >
void Heap<T,KComparator,PComparator,Hasher>::trickleUp(int index){

	int parentIndex = getParentIndex(index);
	while(  c_(store_[index], store_[parentIndex])  ){

		// Swap
		T temp = store_[index];
		store_[index] = store_[parentIndex];
		store_[parentIndex] = temp;

		// Update key_to_location map
		keyToLocation_[store_[index]] = index;
		keyToLocation_[store_[parentIndex]] = parentIndex;

		index = parentIndex;
		parentIndex = getParentIndex(index);

		if(parentIndex < 0){
			break;
		}
	}

}




template <typename T, typename KComparator, typename PComparator, typename Hasher >
void Heap<T,KComparator,PComparator,Hasher>::decreaseKey(const T& old, const T& newVal)
{
	if(c_(old,newVal)){
		return;
	}

	typename std::unordered_map<T, size_t, Hasher, KComparator>::iterator it;
	it = keyToLocation_.find(old);
	if(it == keyToLocation_.end()){
		return;
	}

	int index = it->second;
	store_[index] = newVal;
	int parentIndex = getParentIndex(index);
	if(!c_(store_[parentIndex], newVal)){
		trickleUp(index);
	}
	else{
		trickleDown(index);
	}

}

template <typename T, typename KComparator, typename PComparator, typename Hasher >
T const & Heap<T,KComparator,PComparator,Hasher>::top() const
{
    // Here we use exceptions to handle the case of trying
    // to access the top element of an empty heap
    if(empty()) {
        throw std::logic_error("can't top an empty heap");
    }
    // If we get here we know the heap has at least 1 item
    // Add code to return the top element
    return store_[0];
}


template <typename T, typename KComparator, typename PComparator, typename Hasher >
void Heap<T,KComparator,PComparator,Hasher>::pop()
{
    if(empty()) {
        throw std::logic_error("can't pop an empty heap");
    }

    keyToLocation_.erase(store_[0]);
    store_[0] = store_.back();
    //std::cout << "back: " << store_.back() << std::endl;
    store_.pop_back();
    //std::cout << "back: " << store_.back() << std::endl;

    trickleDown(0);
}


template <typename T, typename KComparator, typename PComparator, typename Hasher >
void Heap<T,KComparator,PComparator,Hasher>::trickleDown(int index){

	if( getNthChildIndex(index, 1) >= store_.size() ){
		return;
	}
	
	int smallestChildIndex = getNthChildIndex(index, 1);

	for(int i = 2; i <= m_; i++){
		int nthChildIndex = getNthChildIndex(index, i);

		if( nthChildIndex >= store_.size()){
			break;
		}

		if(c_(store_[nthChildIndex],store_[smallestChildIndex])){
			smallestChildIndex = nthChildIndex;
		}
	}



	if(!c_(store_[index],store_[smallestChildIndex])){
		// Swap
		T temp = store_[index];
		store_[index] = store_[smallestChildIndex];
		store_[smallestChildIndex] = temp;

		//Update key_to_location map
		keyToLocation_[store_[index]] = index;
		keyToLocation_[store_[smallestChildIndex]] = smallestChildIndex;

		trickleDown(smallestChildIndex);
	}

}


template <typename T, typename KComparator, typename PComparator, typename Hasher >
bool Heap<T,KComparator,PComparator,Hasher>::empty() const
{
    return store_.empty();
}


#endif

