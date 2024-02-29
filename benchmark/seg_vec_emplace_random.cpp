#include "common.hpp"

#define ANKERL_NANOBENCH_IMPLEMENT
#include "nanobench.h"

#include "sfl/segmented_devector.hpp"
#include "sfl/segmented_vector.hpp"

#include <deque>
#include <vector>

template <typename Vector>
void emplace_random(ankerl::nanobench::Bench& bench, const int num_elements)
{
    const std::string title(name_of_type<Vector>());

    ankerl::nanobench::Rng rng;

    bench.warmup(10).batch(num_elements).unit("emplace").run
    (
        title,
        [&]
        {
            Vector vec;

            for (int i = 0; i < num_elements; ++i)
            {
                vec.emplace(vec.begin() + rng.bounded(i), int(rng()));
            }

            ankerl::nanobench::doNotOptimizeAway(vec.size());
        }
    );
}

int main()
{
    constexpr int num_elements = 10'000;

    ankerl::nanobench::Bench bench;
    bench.title("emplace random (" + std::to_string(num_elements) + " elements)");
    bench.performanceCounters(false);
    bench.epochs(100);

    emplace_random<std::deque<int>>(bench, num_elements);
    emplace_random<sfl::segmented_devector<int, 1024>>(bench, num_elements);
    emplace_random<sfl::segmented_vector<int, 1024>>(bench, num_elements);
    emplace_random<std::vector<int>>(bench, num_elements);
}
