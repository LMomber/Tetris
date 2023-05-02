#pragma once

///Code from Jeremiah van Oosten, 03/01/2023
///https://github.com/jpvanoosten/SoftwareRasterizer/blob/main/graphics/src/Timer.cpp

#include <chrono>

namespace Graphics
{
    class Timer
    {
    public:
        Timer() noexcept;

        void tick() noexcept;
        void reset() noexcept;

        double elapsedSeconds() const noexcept;
        double elapsedMilliseconds() const noexcept;
        double elapsedMicroseconds() const noexcept;

        double totalSeconds() const noexcept;
        double totalMilliseconds() const noexcept;
        double totalMicroseconds() const noexcept;

        void limitFPS(int fps) const noexcept;
        void limitFPS(double seconds) const noexcept;
        void limitFPS(const std::chrono::high_resolution_clock::duration& duration) const noexcept;

        template<class Rep, class Period>
        constexpr void limitFPS(const std::chrono::duration<Rep, Period>& duration) const noexcept
        {
            limitFPS(std::chrono::duration_cast<std::chrono::high_resolution_clock::duration>(duration));
        }

    private:
        std::chrono::high_resolution_clock::time_point t0, t1;
        mutable std::chrono::high_resolution_clock::time_point beginFrame;

        double elapsedTime;
        double totalTime;
    };
}