#pragma once

#include <stdexcept>
#include <string_view>

namespace overlay {

    class Win32Error final : public std::runtime_error {
    public:
        explicit Win32Error(std::string_view message)
            : std::runtime_error(message.data()) {}
    };

} // namespace overlay