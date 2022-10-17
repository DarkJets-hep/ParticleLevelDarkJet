#pragma once

#include <Rivet/Particle.hh>
#include <vector>
#include <map>

template<typename T1, typename T2> std::vector<std::pair<T1, T2>> sortMap(const std::map<T1, T2> &map){
    std::vector<std::pair<T1, T2>> sortedMap;
    for(const std::pair<T1, T2> &pair: map){
        sortedMap.push_back(pair);
    }
    std::sort(sortedMap.begin(), sortedMap.end(), [](const std::pair<T1, T2> &a, const std::pair<T1, T2> &b){
        return a.second > b.second;
    });
    return sortedMap;
}

static Rivet::Particles particlesByEnergy(Rivet::Particles particles){
    std::sort(particles.begin(), particles.end(), [](const Rivet::Particle &a, const Rivet::Particle &b){
        return a.energy() > b.energy();    //Reverse the sorting so that the highest energy comes first
    });
    return particles;
}