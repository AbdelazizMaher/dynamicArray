#pragma once
#include <vector>
#include <functional>
#include <iostream>
#include <mutex>

namespace DynamicStrategy {

class lockPolicy {
public:
	virtual void lock() = 0;
	virtual void unlock() = 0;
	virtual ~lockPolicy() = default;
};

class mutexLock : public lockPolicy {
private:
	std::mutex m_mtx;
public:
	void lock() override {
		m_mtx.lock();
	}
	void unlock() override {
		m_mtx.unlock();
	}
};

template<typename T>
class dynamicArray {
private:
	std::vector<T> m_array{};
	std::unique_ptr<lockPolicy> m_pLockPolicy;
	class nullLock : public lockPolicy {
	public:
		void lock() override {
		}
		void unlock() override {
		}
		static nullLock* getInstance() {
			static nullLock instance;
			return &instance;
		}
	};
public:
	dynamicArray(std::unique_ptr<lockPolicy> policy = nullptr) : m_pLockPolicy{ std::move(policy) } {
		if (!m_pLockPolicy) {
			m_pLockPolicy.reset(nullLock::getInstance());
		}
	}
	void add(const T& element) {
		m_pLockPolicy->lock();
		m_array.push_back(element);
		m_pLockPolicy->unlock();
	}
	void insert(size_t pos, const T& element) {
		m_pLockPolicy->lock();
		m_array.insert(m_array.begin() + pos, element);
		m_pLockPolicy->unlock();
	}
	void erase(size_t pos) {
		m_pLockPolicy->lock();
		m_array.erase(m_array.begin() + pos);
		m_pLockPolicy->unlock();
	}
	void erase(size_t pos1 = 0, size_t pos2 = m_array.size()) {
		m_pLockPolicy->lock();
		m_array.erase(m_array.begin() + pos1, m_array.begin() + pos2);
		m_pLockPolicy->unlock();
	}
	T& operator[](size_t index) {
		m_pLockPolicy->lock();
		T& element = m_array[index];
		m_pLockPolicy->unlock();
		return element;
	}
	const T& operator[](size_t index)const {
		m_pLockPolicy->lock();
		const T& element = m_array[index];
		m_pLockPolicy->unlock();
		return element;
	}
	size_t size()const {
		m_pLockPolicy->lock();
		size_t size = m_array.size();
		m_pLockPolicy->unlock();
		return size;
	}
	void print() {
		m_pLockPolicy->lock();
		for (const auto& element : m_array) {
			std::cout << element << ' ';
		}
		std::cout << std::endl;
		m_pLockPolicy->unlock();
	}
};
}


class mutexLock {
private:
	mutable std::mutex m_mtx;
public:
	void lock()const {
		m_mtx.lock();
	}
	void unlock()const {
		m_mtx.unlock();
	}
};

class nullLock {
public:
	void lock() {
	}
	void unlock() {
	}
};

template<typename T>
using printerFunction = std::function<void (const std::vector<T>&)>;

template<typename T, typename lockPolicy = nullLock>
class dynamicArray {
private:
	std::vector<T> m_array{};
	lockPolicy m_LockPolicy{};
	printerFunction<T> m_printer{};
public:
	void setPrinter(printerFunction<T> pfunc) {
		m_LockPolicy.lock();
		m_printer = pfunc;
		m_LockPolicy.unlock();
	}
	void add(const T& element) {
		m_LockPolicy.lock();
		m_array.push_back(element);
		m_LockPolicy.unlock();
	}
	void insert(size_t pos, const T& element) {
		m_LockPolicy.lock();
		m_array.insert(m_array.begin() + pos, element);
		m_LockPolicy.unlock();
	}
	void erase(size_t pos) {
		m_LockPolicy.lock();
		m_array.erase(m_array.begin() + pos);
		m_LockPolicy.unlock();
	}
	void erase(size_t pos1 = 0, size_t pos2 = m_array.size()) {
		m_LockPolicy.lock();
		m_array.erase(m_array.begin() + pos1, m_array.begin() + pos2);
		m_LockPolicy.unlock();
	}
	T& operator[](size_t index) {
		m_LockPolicy.lock();
		T& element = m_array[index];
		m_LockPolicy.unlock();
		return element;
	}
	const T& operator[](size_t index)const {
		m_LockPolicy.lock();
		const T& element = m_array[index];
		m_LockPolicy.unlock();
		return element;
	}
	size_t size()const {
		m_LockPolicy.lock();
		size_t size = m_array.size();
		m_LockPolicy.unlock();
		return size;
	}
	void print() {
		m_LockPolicy.lock();
		if (m_printer)
			m_printer(m_array);
		//for (const auto& element : m_array) {
		//	std::cout << element << ' ';
		//}
		//std::cout << std::endl;
		m_LockPolicy.unlock();
	}
};

