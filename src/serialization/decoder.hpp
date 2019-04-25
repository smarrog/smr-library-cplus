#pragma once

#include <string>
#include <stdexcept>
#include <istream>

namespace smr {
    class Decoder {
    public:
        Decoder(const Decoder&) = delete;
        Decoder& operator = (Decoder const&) = delete;
        Decoder(Decoder&&) noexcept = delete;
        Decoder& operator = (Decoder&&) noexcept = delete;

    protected:
        Decoder(std::istream& is, size_t flags)
            : _is(is)
            , _flags(flags) {}

        std::istream& _is;
        size_t _flags;

        bool hasFlag(uint32_t flag) {
            return (_flags & flag) > 0;
        }
    };
}