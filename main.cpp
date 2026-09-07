#include <iostream>
#include <array>
#include <random>
#include <algorithm>
#include <numeric>
#include <chrono>
#include <thread>
#include <stop_token>
#include <cmath>

struct Particle
{
    double m_position,
           m_bestPosition,
           m_velocity;
};

int main()
{
    // essentials:
    constexpr int rows = 40,
                  columns = 170;
    char grid[rows][columns];

    auto func = [](double xValue) {
        return std::sin(1 / xValue);
    };

    constexpr double minX = 0.1,
                     maxX = 0.6,
                     xIncrement = (maxX - minX) / (columns - 1),
                     minY = -1,
                     maxY = 1,
                     yIncrement = (maxY - minY) / (rows - 1);

    constexpr double inertiaWeight = 0.7,
                     personalWeight = 1.5,
                     globalWeight = 1.5;

    std::array<Particle, 50> particles;

    std::random_device rd;
    std::uniform_real_distribution<double> xDist(minX, maxX),
                                           chanceDist(0, 1);
    for (Particle& particleRef : particles)
    {
        particleRef.m_position = particleRef.m_bestPosition = xDist(rd);
        particleRef.m_velocity = 0.5;
    }

    // visualization:
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "\033[?25l"; // cursor off
    std::cout << "\033[2J"; // clear screen

    {
        std::jthread thr{[&](std::stop_token st) {
            double globalBestPosition = minX,
                   globalLowestHeight = std::numeric_limits<double>::max();
            int frame = 1;

            while (!st.stop_requested())
            {
                // display:
                for (int row = 0; row < rows; ++row)
                    for (int column = 0; column < columns; ++column)
                        grid[row][column] = ' ';

                for (int column = 0; column < columns; ++column)
                {
                    const double xValue = minX + column * xIncrement,
                                 yValue = func(xValue);

                    const int row = rows - 1 - static_cast<int>((yValue - minY) / yIncrement);

                    grid[row][column] = '-';
                }

                for (const Particle& particle : particles)
                {
                    const double height = func(particle.m_position);

                    const int column = static_cast<int>((particle.m_position - minX) / xIncrement),
                              row = rows - 1 - static_cast<int>((height - minY) / yIncrement);

                    grid[row][column] = 'o';
                }

                std::cout << "\033[0H";
                for (int row = 0; row < rows; ++row)
                {
                    for (int column = 0; column < columns; ++column)
                    {
                        if (grid[row][column] == '-')
                            std::cout << "\033[38;2;0;255;255m";
                        else if (grid[row][column] == 'o')
                            std::cout << "\033[38;2;255;0;0m";

                        std::cout << grid[row][column];
                    }

                    std::cout << '\n';
                }
                std::cout << std::flush;

                // move particles:
                for (Particle& particleRef : particles)
                {
                    const double height = func(particleRef.m_position);

                    if (height < globalLowestHeight)
                    {
                        globalBestPosition = particleRef.m_position;
                        globalLowestHeight = height;
                    }

                    if (height < func(particleRef.m_bestPosition))
                        particleRef.m_bestPosition = particleRef.m_position;
                }

                for (Particle& particleRef : particles)
                {
                    particleRef.m_velocity
                        = inertiaWeight * particleRef.m_velocity
                        + personalWeight * chanceDist(rd) * (particleRef.m_bestPosition - particleRef.m_position)
                        + globalWeight * chanceDist(rd) * (globalBestPosition - particleRef.m_position);

                    particleRef.m_position = std::clamp(
                        particleRef.m_position + particleRef.m_velocity, minX, maxX
                        );
                }

                std::this_thread::sleep_for(
                    std::chrono::milliseconds(std::max(static_cast<int>(75 / (frame * 0.1)), 50))
                    );
                ++frame;
            }
        }};
        std::cin.get();
    }

    std::cout << "\033[2J"; // clear screen
    std::cout << "\033[0H"; // move cursor to top left
    std::cout << "\033[?25h"; // restore cursor
    std::cout << "\033[0m"; // restore color
}
