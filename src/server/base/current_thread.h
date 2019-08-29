#ifndef BASE_CURRENT_THREAD_H_
#define BASE_CURRENT_THREAD_H_
namespace dachun {
//当前线程：利用syscall保证线程id的唯一性
namespace CurrentThread {
extern __thread int t_cached_tid;
extern __thread char t_tid_string[32];
extern __thread int t_tid_length;
extern __thread const char* t_thread_name;
void CacheTid();

inline int tid() {
  if (t_cached_tid == 0) {
      CacheTid();
  }
  return t_cached_tid;
}

inline const char* tidString() {
    return t_tid_string;
}

inline const char* name() {
    return t_thread_name;
}

bool isMainThread();
} // namespace CurrentThread
} // namespace dachun
#endif // BASE_CURRENT_THREAD_H_