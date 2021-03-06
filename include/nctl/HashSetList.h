#ifndef CLASS_NCTL_HASHSETLIST
#define CLASS_NCTL_HASHSETLIST

#include "Array.h"
#include "List.h"
#include "HashFunctions.h"

namespace nctl {

template <class K, class HashFunc> class HashSetListIterator;
template <class K, class HashFunc> struct HashSetListHelperTraits;
class String;

/// A template based hashset implementation with separate chaining and list head cell
template <class K, class HashFunc = FNV1aHashFunc<K>>
class HashSetList
{
  public:
	/// Iterator type
	using Iterator = HashSetListIterator<K, HashFunc>;
	/// Constant iterator type
	using ConstIterator = HashSetListIterator<K, HashFunc>;
	/// Reverse iterator type
	using ReverseIterator = nctl::ReverseIterator<Iterator>;
	/// Reverse constant iterator type
	using ConstReverseIterator = nctl::ReverseIterator<ConstIterator>;

	explicit HashSetList(unsigned int capacity);
	~HashSetList() { clear(); }

	/// Copy constructor
	HashSetList(const HashSetList &other);
	/// Move constructor
	HashSetList(HashSetList &&other);
	/// Copy-and-swap assignment operator
	HashSetList &operator=(HashSetList other);

	/// Swaps two hashSets without copying their data
	inline void swap(HashSetList &first, HashSetList &second)
	{
		nctl::swap(first.buckets_, second.buckets_);
	}

	/// Returns an iterator to the first element
	Iterator begin();
	/// Returns a reverse iterator to the last element
	ReverseIterator rBegin();
	/// Returns an iterator to past the last element
	Iterator end();
	/// Returns a reverse iterator to prior the first element
	ReverseIterator rEnd();

	/// Returns a constant iterator to the first element
	ConstIterator begin() const;
	/// Returns a constant reverse iterator to the last element
	ConstReverseIterator rBegin() const;
	/// Returns a constant iterator to past the last lement
	ConstIterator end() const;
	/// Returns a constant reverse iterator to prior the first element
	ConstReverseIterator rEnd() const;

	/// Returns a constant iterator to the first element
	inline ConstIterator cBegin() const { return begin(); }
	/// Returns a constant reverse iterator to the last element
	inline ConstReverseIterator crBegin() const { return rBegin(); }
	/// Returns a constant iterator to past the last lement
	inline ConstIterator cEnd() const { return end(); }
	/// Returns a constant reverse iterator to prior the first element
	inline ConstReverseIterator crEnd() const { return rEnd(); }

	/// Inserts an element if no other has the same key
	bool insert(const K &key);
	/// Moves an element if no other has the same key
	bool insert(K &&key);

	/// Returns true if the hashSet is empty
	inline bool isEmpty() const { return size() == 0; }
	/// Returns the number of elements in the hashSet
	unsigned int size() const;
	/// Returns the average number of elements per bucket,
	inline float loadFactor() const { return size() / static_cast<float>(buckets_.size()); }
	/// Returns the number of buckets
	inline unsigned int bucketAmount() const { return buckets_.size(); }
	/// Returns the size of the bucket for the hash generated by the specified key
	inline unsigned int bucketSize(const K &key) const { return retrieveBucket(hashFunc_(key)).size(); }
	/// Returns the index of the bucket for the hash generated by the specified key
	inline unsigned int bucket(const K &key) const { return hashFunc_(key) % buckets_.size(); }
	/// Returns the hash of a given key
	inline hash_t hash(const K &key) const { return hashFunc_(key); }

	/// Clears the hashSet
	void clear();
	/// Checks whether an element is in the hashSet or not
	bool contains(const K &key) const;
	/// Checks whether an element is in the hashSet or not
	K *find(const K &key);
	/// Checks whether an element is in the hashSet or not (read-only)
	const K *find(const K &key) const;
	/// Removes a key from the hashSet, if it exists
	bool remove(const K &key);

	/// Sets the number of buckets to the new specified size and rehashes the container
	void rehash(unsigned int count);

  private:
	/// The template class for the node stored inside the hashSet
	class Node
	{
	  public:
		hash_t hash;
		K key;

		Node()
		    : hash(NullHash) {}
		Node(hash_t hh, const K &kk)
		    : hash(hh), key(kk) {}
		Node(hash_t hh, K &&kk)
		    : hash(hh), key(nctl::move(kk)) {}
	};

	/// The bucket class for the hashSet, with separate chaining and list head cell
	class HashBucket
	{
	  public:
		HashBucket()
		    : size_(0) {}
		unsigned int size() const { return size_; }
		void clear();
		bool contains(hash_t hash, const K &key) const;
		K *find(hash_t hash, const K &key);
		const K *find(hash_t hash, const K &key) const;
		K &findOrInsert(hash_t hash, const K &key);
		bool insert(hash_t hash, const K &key);
		bool insert(hash_t hash, K &&key);
		bool remove(hash_t hash, const K &key);

	  private:
		/// Number of nodes in this bucket
		unsigned int size_;
		/// Separate chaining with head cell
		Node firstNode_;
		/// Separate chaining with a linked list
		List<Node> collisionList_;

		Node *findNode(hash_t hash, const K &key);
		const Node *findNode(hash_t hash, const K &key) const;

		friend class HashSetListIterator<K, HashFunc>;
		friend class HashSetList<K, HashFunc>;
	};

	Array<HashBucket> buckets_;
	HashFunc hashFunc_;

	HashBucket &retrieveBucket(hash_t hash);
	const HashBucket &retrieveBucket(hash_t hash) const;

	friend class HashSetListIterator<K, HashFunc>;
	friend struct HashSetListHelperTraits<K, HashFunc>;
};

template <class K, class HashFunc>
typename HashSetList<K, HashFunc>::Iterator HashSetList<K, HashFunc>::begin()
{
	Iterator iterator(this, Iterator::SentinelTagInit::BEGINNING);
	return ++iterator;
}

template <class K, class HashFunc>
typename HashSetList<K, HashFunc>::ReverseIterator HashSetList<K, HashFunc>::rBegin()
{
	Iterator iterator(this, Iterator::SentinelTagInit::END);
	return ReverseIterator(--iterator);
}

template <class K, class HashFunc>
typename HashSetList<K, HashFunc>::Iterator HashSetList<K, HashFunc>::end()
{
	return Iterator(this, Iterator::SentinelTagInit::END);
}

template <class K, class HashFunc>
typename HashSetList<K, HashFunc>::ReverseIterator HashSetList<K, HashFunc>::rEnd()
{
	Iterator iterator(this, Iterator::SentinelTagInit::BEGINNING);
	return ReverseIterator(iterator);
}

template <class K, class HashFunc>
typename HashSetList<K, HashFunc>::ConstIterator HashSetList<K, HashFunc>::begin() const
{
	ConstIterator iterator(this, ConstIterator::SentinelTagInit::BEGINNING);
	return ++iterator;
}

template <class K, class HashFunc>
typename HashSetList<K, HashFunc>::ConstReverseIterator HashSetList<K, HashFunc>::rBegin() const
{
	ConstIterator iterator(this, ConstIterator::SentinelTagInit::END);
	return ConstReverseIterator(--iterator);
}

template <class K, class HashFunc>
typename HashSetList<K, HashFunc>::ConstIterator HashSetList<K, HashFunc>::end() const
{
	return ConstIterator(this, ConstIterator::SentinelTagInit::END);
}

template <class K, class HashFunc>
typename HashSetList<K, HashFunc>::ConstReverseIterator HashSetList<K, HashFunc>::rEnd() const
{
	ConstIterator iterator(this, ConstIterator::SentinelTagInit::BEGINNING);
	return ConstReverseIterator(iterator);
}

template <class K, class HashFunc>
void HashSetList<K, HashFunc>::HashBucket::clear()
{
	collisionList_.clear();
	size_ = 0;
}

template <class K, class HashFunc>
bool HashSetList<K, HashFunc>::HashBucket::contains(hash_t hash, const K &key) const
{
	const Node *node = findNode(hash, key);
	return (node != nullptr);
}

template <class K, class HashFunc>
K *HashSetList<K, HashFunc>::HashBucket::find(hash_t hash, const K &key)
{
	Node *node = findNode(hash, key);

	if (node)
		return &(node->key);

	return nullptr;
}

template <class K, class HashFunc>
const K *HashSetList<K, HashFunc>::HashBucket::find(hash_t hash, const K &key) const
{
	const Node *node = findNode(hash, key);

	if (node)
		return &(node->key);

	return nullptr;
}

template <class K, class HashFunc>
K &HashSetList<K, HashFunc>::HashBucket::findOrInsert(hash_t hash, const K &key)
{
	if (size_ == 0)
	{
		// Early-out if the bucket is empty
		firstNode_.hash = hash;
		firstNode_.key = key;
		size_++;
		return firstNode_.key;
	}

	Node *node = findNode(hash, key);
	if (node)
		return node->key;

	// The item has not been found, a new entry is created at the end of the list
	size_++;
	collisionList_.pushBack(Node(hash, key));
	return collisionList_.back().key;
}

template <class K, class HashFunc>
bool HashSetList<K, HashFunc>::HashBucket::insert(hash_t hash, const K &key)
{
	if (size_ == 0)
	{
		// Early-out if the bucket is empty
		firstNode_.hash = hash;
		firstNode_.key = key;
		size_++;
		return true;
	}

	Node *node = findNode(hash, key);
	if (node)
		return false;

	// The item has not been found, a new entry is created at the end of the list
	size_++;
	collisionList_.pushBack(Node(hash, key));
	return true;
}

template <class K, class HashFunc>
bool HashSetList<K, HashFunc>::HashBucket::insert(hash_t hash, K &&key)
{
	if (size_ == 0)
	{
		// Early-out if the bucket is empty
		firstNode_.hash = hash;
		firstNode_.key = nctl::move(key);
		size_++;
		return true;
	}

	Node *node = findNode(hash, key);
	if (node)
		return false;

	// The item has not been found, a new entry is created at the end of the list
	size_++;
	collisionList_.pushBack(Node(hash, nctl::move(key)));
	return true;
}

/*! \return True if the element has been found and removed */
template <class K, class HashFunc>
bool HashSetList<K, HashFunc>::HashBucket::remove(hash_t hash, const K &key)
{
	// Early-out if the bucket is empty
	if (size_ == 0)
		return false;

	bool found = false;
	if (firstNode_.hash == hash && equalTo(firstNode_.key, key))
	{
		// The item has been found in the direct access node
		found = true;

		// Bring the first element of the list, if any, as direct access node
		if (collisionList_.isEmpty() == false)
		{
			firstNode_ = collisionList_.front();
			collisionList_.popFront();
		}
		size_--;
	}
	else
	{
		for (typename List<Node>::ConstIterator i = collisionList_.begin(); i != collisionList_.end(); ++i)
		{
			if ((*i).hash == hash && equalTo((*i).key, key))
			{
				// The item has been found in the list
				found = true;
				i = collisionList_.erase(i);
				size_--;

				// The item has been removed and the iterator is no longer valid
				break;
			}
		}
	}

	return found;
}

template <class K, class HashFunc>
typename HashSetList<K, HashFunc>::Node *HashSetList<K, HashFunc>::HashBucket::findNode(hash_t hash, const K &key)
{
	if (size_ == 0)
		return nullptr;

	if (firstNode_.hash == hash && equalTo(firstNode_.key, key))
		// The item has been found in the direct access node
		return &firstNode_;
	else
	{
		for (typename List<Node>::Iterator i = collisionList_.begin(); i != collisionList_.end(); ++i)
		{
			// The item has been found in the list
			if ((*i).hash == hash && equalTo((*i).key, key))
				return &(*i);
		}
	}

	return nullptr;
}

template <class K, class HashFunc>
const typename HashSetList<K, HashFunc>::Node *HashSetList<K, HashFunc>::HashBucket::findNode(hash_t hash, const K &key) const
{
	if (size_ == 0)
		return nullptr;

	if (firstNode_.hash == hash && equalTo(firstNode_.key, key))
		// The item has been found in the direct access node
		return &firstNode_;
	else
	{
		for (typename List<Node>::ConstIterator i = collisionList_.begin(); i != collisionList_.end(); ++i)
		{
			// The item has been found in the list
			if ((*i).hash == hash && equalTo((*i).key, key))
				return &(*i);
		}
	}

	return nullptr;
}

template <class K, class HashFunc>
HashSetList<K, HashFunc>::HashSetList(unsigned int capacity)
    : buckets_(capacity, ArrayMode::FIXED_CAPACITY)
{
	FATAL_ASSERT_MSG(capacity > 0, "Zero is not a valid capacity");

	for (unsigned int i = 0; i < capacity; i++)
		buckets_[i] = HashBucket();
}

template <class K, class HashFunc>
HashSetList<K, HashFunc>::HashSetList(const HashSetList<K, HashFunc> &other)
    : buckets_(other.buckets_)
{
}

template <class K, class HashFunc>
HashSetList<K, HashFunc>::HashSetList(HashSetList<K, HashFunc> &&other)
    : buckets_(nctl::move(other.buckets_))
{
}

/*! \note The parameter should be passed by value for the idiom to work. */
template <class K, class HashFunc>
HashSetList<K, HashFunc> &HashSetList<K, HashFunc>::operator=(HashSetList<K, HashFunc> other)
{
	swap(*this, other);
	return *this;
}

/*! \return True if the element has been inserted */
template <class K, class HashFunc>
bool HashSetList<K, HashFunc>::insert(const K &key)
{
	const hash_t hash = hashFunc_(key);
	return retrieveBucket(hash).insert(hash, key);
}

/*! \return True if the element has been inserted */
template <class K, class HashFunc>
bool HashSetList<K, HashFunc>::insert(K &&key)
{
	const hash_t hash = hashFunc_(key);
	return retrieveBucket(hash).insert(hash, nctl::move(key));
}

template <class K, class HashFunc>
unsigned int HashSetList<K, HashFunc>::size() const
{
	unsigned int totalSize = 0;
	for (unsigned int i = 0; i < buckets_.size(); i++)
		totalSize += buckets_[i].size();

	return totalSize;
}

template <class K, class HashFunc>
void HashSetList<K, HashFunc>::clear()
{
	for (unsigned int i = 0; i < buckets_.size(); i++)
		buckets_[i].clear();
}

template <class K, class HashFunc>
bool HashSetList<K, HashFunc>::contains(const K &key) const
{
	const hash_t hash = hashFunc_(key);
	return retrieveBucket(hash).contains(hash, key);
}

/*! \note Prefer this method if copying `T` is expensive, but always check the validity of returned pointer. */
template <class K, class HashFunc>
K *HashSetList<K, HashFunc>::find(const K &key)
{
	const hash_t hash = hashFunc_(key);
	return retrieveBucket(hash).find(hash, key);
}

/*! \note Prefer this method if copying `T` is expensive, but always check the validity of returned pointer. */
template <class K, class HashFunc>
const K *HashSetList<K, HashFunc>::find(const K &key) const
{
	const hash_t hash = hashFunc_(key);
	return retrieveBucket(hash).find(hash, key);
}

template <class K, class HashFunc>
bool HashSetList<K, HashFunc>::remove(const K &key)
{
	const hash_t hash = hashFunc_(key);
	return retrieveBucket(hash).remove(hash, key);
}

template <class K, class HashFunc>
void HashSetList<K, HashFunc>::rehash(unsigned int count)
{
	const unsigned int totalSize = size();
	if (totalSize == 0 || count < totalSize)
		return;

	HashSetList<K, HashFunc> hashSet(count);

	unsigned int bucketIndex = 0;
	HashBucket &bucket = buckets_[bucketIndex];
	while (bucketIndex < buckets_.size() - 1)
	{
		while (bucketIndex < buckets_.size() - 1 && bucket.size() == 0)
			bucket = buckets_[++bucketIndex];

		if (bucket.size() > 0)
		{
			hashSet.insert(bucket.firstNode_.key);
			for (typename List<Node>::ConstIterator i = bucket.collisionList_.begin(); i != bucket.collisionList_.end(); ++i)
				hashSet.insert((*i).key);

			++bucketIndex;
		}
	}

	*this = nctl::move(hashSet);
}

template <class K, class HashFunc>
typename HashSetList<K, HashFunc>::HashBucket &HashSetList<K, HashFunc>::retrieveBucket(hash_t hash)
{
	const unsigned int index = hash % buckets_.size();
	return buckets_[index];
}

template <class K, class HashFunc>
const typename HashSetList<K, HashFunc>::HashBucket &HashSetList<K, HashFunc>::retrieveBucket(hash_t hash) const
{
	const unsigned int index = hash % buckets_.size();
	return buckets_[index];
}

using StringHashSetList = HashSetList<String, FNV1aHashFuncContainer<String>>;
using CStringHashSetList = HashSetList<const char *, FNV1aHashFunc<const char *>>;

}

#endif
