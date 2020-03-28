#ifndef BASE_MEMORY_SINGLETON_H_
#define BASE_MEMORY_SINGLETON_H_

#include <memory>
#include <mutex>
#include "../util/at_exit.h"
namespace nbase
{
//饿汉模式
#define SINGLETON_DEFINE(TypeName)				\
static TypeName* GetInstance()					\
{												\
	static TypeName type_instance;				\
	return &type_instance;						\
}												\
												\
TypeName(const TypeName&) = delete;				\
TypeName& operator=(const TypeName&) = delete

// 懒汉模式
//release_atexitmanager 是否由atexitmanager来释放
template <typename TSingleton, bool release_atexitmanager = true>
class Singleton
{
public:
    using TSingletonPtr = std::unique_ptr<TSingleton>;
    static const TSingletonPtr& GetInstance()
    {
        static std::once_flag oc;
        static TSingletonPtr instance;

        std::call_once(oc, [&]() {
            instance.reset(new TSingleton);
            if (release_atexitmanager) {
                StdClosure cb = [&]() {
                    instance.reset(nullptr);
#if _MSC_VER > 1900
                    oc._Opaque = 0;
#else
                    oc._Flag = 0;
#endif
                };
                AtExitManager::RegisterTask(cb);
            }
        });
        return instance;
    }

private:
    Singleton(void) = default;
    Singleton(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator = (const Singleton&) = delete;
    virtual ~Singleton() = default;
};

}

#endif // BASE_MEMORY_SINGLETON_H_
