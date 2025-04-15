#include <iostream>
#include <mutex>
#include <memory>
#include <cstdlib>

struct SingleThreaded {
    struct Lock {
        Lock() {}
    };
};

struct MultiThreaded {
    struct Lock {
        Lock() { mtx.lock(); }
        ~Lock() { mtx.unlock(); }
    private:
        static std::mutex mtx;
    };
};
std::mutex MultiThreaded::Lock::mtx;

template<typename T>
struct CreateUsingNew {
    static T* Create() { return new T(); }
    static void Destroy(T* p) { delete p; }
};

template<typename T>
struct CreateUsingMalloc {
    static T* Create() {
        void* buf = std::malloc(sizeof(T));
        return buf ? new(buf) T() : nullptr;
    }
    static void Destroy(T* p) {
        if (p) {
            p->~T();
            std::free(p);
        }
    }
};

template<typename T>
struct DefaultLifetime {
    static void ScheduleDestruction(T* p, void(*destroyer)(T*)) {
        std::atexit([=]() { destroyer(p); });
    }

    static void OnDeadReference() {
        throw std::runtime_error("Dead Reference Detected");
    }
};

template<typename T>
struct NoDestroyLifetime {
    static void ScheduleDestruction(T*, void(*)(T*)) {}
    static void OnDeadReference() {}
};


template <
    typename T,
    template <typename> class CreationPolicy = CreateUsingNew,
    template <typename> class LifetimePolicy = DefaultLifetime,
    class ThreadingPolicy = SingleThreaded
>
class Singleton {
public:
    static T& Instance() {
        if (!instance_) {
            typename ThreadingPolicy::Lock guard;
            if (!instance_) {
                if (destroyed_) {
                    LifetimePolicy<T>::OnDeadReference();
                    destroyed_ = false;
                }
                instance_ = CreationPolicy<T>::Create();
                LifetimePolicy<T>::ScheduleDestruction(instance_, DestroySingleton);
            }
        }
        return *instance_;
    }

private:
    static void DestroySingleton(T* p) {
        CreationPolicy<T>::Destroy(p);
        instance_ = nullptr;
        destroyed_ = true;
    }

    Singleton() = delete;
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    static inline T* instance_ = nullptr;
    static inline bool destroyed_ = false;
};

