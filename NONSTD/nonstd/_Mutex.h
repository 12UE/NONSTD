#pragma once
namespace nonstd {
	class mutex {
	public:
		mutex() {
			KeInitializeMutex(&mtx_, 0);
		}

		void lock() {
			KeWaitForSingleObject(&mtx_, Executive, KernelMode, FALSE, NULL);
		}

		bool try_lock() {
			return KeWaitForSingleObject(&mtx_, Executive, KernelMode, FALSE, NULL) == STATUS_SUCCESS;
		}

		void unlock() {
			KeReleaseMutex(&mtx_, FALSE);
		}

	private:
		KMUTEX mtx_;
	};
	template<class Mutex>
	class unique_lock {
	public:
		unique_lock(Mutex& mtx) : mtx_(mtx), locked_(true) {
			mtx_.lock();
		}

		~unique_lock() {
			if (locked_) mtx_.unlock();
		}

		void unlock() {
			if (locked_) {
				mtx_.unlock();
				locked_ = false;
			}
		}

		void lock() {
			if (!locked_) {
				mtx_.lock();
				locked_ = true;
			}
		}

		bool try_lock() {
			if (locked_) return false;
			locked_ = mtx_.try_lock();
			return locked_;
		}

	private:
		Mutex& mtx_;
		bool locked_;
	};
	template<class Mutex>
	class shared_lock : public unique_lock<Mutex> {
	public:
		shared_lock(Mutex& mtx) : unique_lock<Mutex>(mtx) {}
		void lock() = delete;
		bool try_lock() = delete;
	};
}