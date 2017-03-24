/*
 * A simple priority queue class which can deal with the
 * multithreaded aplication. The class is simple because
 * in the kronos scheduler I do not expect too much
 * contetion for the lock, and as such dont feel the need
 * to over-engineer it.
 */
#include <queue>
#include <mutex>

#pragma once

namespace kronos {
template <class T,
         class Container = std::vector<T>,
         class Compare = std::less<typename Container::value_type>
         >
class locked_priority_queue {
    public:
        bool empty() {
            std::lock_guard<std::mutex> g(lock);
            return q.empty();
        }
        T top() {
            std::lock_guard<std::mutex> g(lock);
            return q.top();
        }
        void push(T& item) {
            std::lock_guard<std::mutex> g(lock);
            return q.push(item);
        }
        void pop() {
            std::lock_guard<std::mutex> g(lock);
            return q.pop();
        }
    private:
        std::mutex lock;
        std::priority_queue<T, Container, Compare> q;
};
} /* namespace kronos */
