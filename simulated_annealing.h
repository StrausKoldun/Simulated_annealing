//
// Created by Starus on 01.07.2022.
//

#ifndef SIMULATED_ANNEALING_SIMULATED_ANNEALING_H
#define SIMULATED_ANNEALING_SIMULATED_ANNEALING_H


#include <functional>
#include <utility>
#include <random>
#include <chrono>

template<class T>
class simulated_annealing{
public:
    simulated_annealing(std::function<long double(long double)> _upd_temp_func,
                        std::function<T(const T &)> _find_neighbor_func,
                        std::function<long double(const T &)> _calc_value_from_T_func,
                        std::function<long double(long double, long double, long double)> _calc_chance_to_change_func);
    T calc_ans(T seed, int count_of_iterations);

private:
    long double t=1;
    std::function<long double(long double)> upd_temp;
    std::function<T(const T&)> find_neighbor;
    std::function<long double(const T&)> calc_value_from_T;
    std::function<long double(long double, long double, long double)> calc_chance_to_change;
    std::mt19937 rnd;
    std::uniform_real_distribution<> dis;
};

template<class T>
simulated_annealing<T>::simulated_annealing(std::function<long double(long double)> _upd_temp_func,
                                            std::function<T(const T &)> _find_neighbor_func,
                                            std::function<long double(const T &)> _calc_value_from_T,
                                            std::function<long double(long double, long double,
                                                                      long double)> _calc_chance_to_change)
        :upd_temp(std::move(_upd_temp_func)),
         find_neighbor(std::move(_find_neighbor_func)),
         calc_value_from_T(std::move(_calc_value_from_T)),
         calc_chance_to_change(std::move(_calc_chance_to_change)),
         rnd(std::chrono::steady_clock::now().time_since_epoch().count()), dis(0.0, 1.0){

}

template<class T>
T simulated_annealing<T>::calc_ans(T seed, int count_of_iterations) {
    T ans, now;
    ans=now=seed;
    long double ans_value, now_value;
    ans_value=now_value=calc_value_from_T(seed);
    while(count_of_iterations--){
        t=upd_temp(t);
        auto neighbor=find_neighbor(now);
        auto neighbor_value=calc_value_from_T(neighbor);
        if(calc_chance_to_change(now_value, neighbor_value, t) >= dis(rnd)){
            std::swap(now, neighbor);
            std::swap(now_value, neighbor_value);
            if(now_value >= ans_value){
                ans=now;
                ans_value=now_value;
            }
        }
    }
    return ans;
}



#endif //SIMULATED_ANNEALING_SIMULATED_ANNEALING_H
