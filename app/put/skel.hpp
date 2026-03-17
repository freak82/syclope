#pragma once

#include "mpl/types.hpp"
#include "put/throw.hpp"

namespace put // project utilities
{

template <typename Skel>
class skel
{
public:
    using impl_type = Skel;

private:
    struct destroyer
    {
        void operator()(impl_type* p) const { impl_type::destroy(p); }
    };
    std::unique_ptr<impl_type, destroyer> impl_;

public:
    skel() : impl_(impl_type::open_and_load())
    {
        if (!impl_) {
            put::throw_system_error(errno, "Failed to open_and_load skel: {}",
                                    mpl::type_name<impl_type>());
        }
        if (impl_type::attach(impl_.get()) != 0) {
            put::throw_system_error(errno, "Failed to attach skel: {}",
                                    mpl::type_name<impl_type>());
        }
    }
    ~skel() noexcept = default;

    skel(skel&&) noexcept            = default;
    skel& operator=(skel&&) noexcept = default;

    skel(const skel&)            = delete;
    skel& operator=(const skel&) = delete;

    void reset() noexcept { impl_.reset(); }

    impl_type* get() const noexcept { return impl_.get(); }
    impl_type* operator->() const noexcept { return impl_.get(); }
    explicit operator bool() const noexcept { return !!impl_; }
};

} // namespace put
