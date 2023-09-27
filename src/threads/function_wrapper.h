#ifndef PARALLELS_FUNCTION_WRAPPER_H
#define PARALLELS_FUNCTION_WRAPPER_H

#include <memory>

namespace Parallels {
    class FunctionWrapper {
    public:
    template<typename T>
    FunctionWrapper(T&& func): 
    impl_(new ImplType<T>(std::move(func))){}
    void operator()() {
        impl_->call();
    }

    FunctionWrapper() = default;

    ~FunctionWrapper() = default;

    FunctionWrapper(FunctionWrapper&& other): impl_(std::move(other.impl_)){}

    FunctionWrapper& operator=(FunctionWrapper&& other){
        impl_ = std::move(other.impl_);
        return *this;
    }

    FunctionWrapper(const FunctionWrapper& other)=delete;
    FunctionWrapper(FunctionWrapper& other)=delete;
    FunctionWrapper& operator=(const FunctionWrapper& other)=delete;

       
    private:
    struct BaseImpl{
        virtual void call() = 0;
        virtual ~BaseImpl(){}
    };

    std::unique_ptr<BaseImpl> impl_;

    template<typename T>
    struct ImplType: BaseImpl {
        T func_;
        ImplType(T&& func): func_(std::move(func)){}
        void call() { func_(); }
    };

    };
};  // namespace Parallels

#endif  // PARALLELS_FUNCTION_WRAPPER_H
