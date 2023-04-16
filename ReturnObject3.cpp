#include <concepts>
#include <coroutine>
#include <exception>
#include <iostream>
// g++ -fcoroutines -std=c++20 ReturnObject3.cpp
template <typename PromiseType> struct GetPromise {
  PromiseType *p_;
  bool await_ready() { return false; } // says yes call await_suspend
  bool await_suspend(std::coroutine_handle<PromiseType> h) {
    p_ = &h.promise();
    return false; // says no don't suspend coroutine after all
  }
  PromiseType *await_resume() { return p_; }
};

struct ReturnObject3 {
  struct promise_type {
    unsigned value_;

    ReturnObject3 get_return_object() {
      return ReturnObject3{
          .h_ = std::coroutine_handle<promise_type>::from_promise(*this)};
    }
    std::suspend_never initial_suspend() { return {}; }
    std::suspend_never final_suspend() noexcept { return {}; }
    void unhandled_exception() {}
  };

  std::coroutine_handle<promise_type> h_;
  operator std::coroutine_handle<promise_type>() const {
    std::cout << "operator std::coroutine_handle<promise_type>() const"
              << std::endl;
    return h_;
  }
};

ReturnObject3 counter3() {
  auto pp = co_await GetPromise<ReturnObject3::promise_type>{};

  std::cout << __FUNCTION__ << ":" << __LINE__ << std::endl;

  for (unsigned i = 0;; ++i) {
    pp->value_ = i;
    co_await std::suspend_always{};
  }
}

int main() {
  std::coroutine_handle<ReturnObject3::promise_type> h = counter3();
  // 上面这行，获取句柄,这里不挂起，因为 GetPromise 的 await_suspend 
  // 函数返回 false

  std::cout << "after coroutine_handle^^^^^" << std::endl;

  ReturnObject3::promise_type &promise = h.promise();
  // 上面这行，获取用户自定义值，不能直接获取，可以通过先获取promise，再获取用户定义在promise的值
  for (int i = 0; i < 3; ++i) {
    std::cout << "counter3: " << promise.value_ << std::endl;
    h();
    // 上面这行，其实就是跳转到counter3()里面的 co_await std::suspend_always{};
    // 继续执行，因为 co_await std::suspend_always{}; 在 for 循环里面，pp->value_ 又加了1
  }
  h.destroy();
}
