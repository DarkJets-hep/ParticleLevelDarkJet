#pragma once

#include "Rivet/Particle.hh"
#include <iostream>
#include <vector>
#include <map>
#include "ParticleName.hpp"

class Decay{
public:
    Decay(const Rivet::Particle &child){
        const Rivet::Particles parents = child.parents();
        if(parents.size() > 0){
            for(const Rivet::Particle &parent: parents){
                this->_parents.push_back(parent.pid());
            }
            std::sort(this->_parents.begin(), this->_parents.end());    //So that the order doesn't matter
            for(const Rivet::Particle &child: parents[0].children()){
                this->_children.push_back(child.pid());
            }
            std::sort(this->_children.begin(), this->_children.end());
        }
        else{
            this->_children.push_back(child.pid());
        }
    }

    bool operator==(const Decay &other) const{
        return other._parents == this->_parents && other._children == this->_children;
    }
    bool operator!=(const Decay &other) const{
        return !(*this == other);
    }

    friend std::ostream& operator<<(std::ostream &flux, const Decay &decay){
        if(decay._parents.size() == 0){
            flux << "No decay";
        }
        else{
            for(unsigned int i = 0; i < decay._parents.size(); i++){
                if(i > 0){
                    flux << " + ";
                }
                flux << particleName(decay._parents[i]);
            }
            flux << " => ";
            for(unsigned int i = 0; i < decay._children.size(); i++){
                if(i > 0){
                    flux << " + ";
                }
                flux << particleName(decay._children[i]);
            }
        }
        return flux;
    }

    //Comparison operators are required for this class to be used as a key for std::map, the result of these isn't meaningful
    friend bool operator<(const Decay &a, const Decay &b){
        return a._parents < b._parents || (a._parents == b._parents && a._children < b._children);
    }
    friend bool operator<=(const Decay &a, const Decay &b){
        return a == b || a < b;
    }
    friend bool operator>(const Decay &a, const Decay &b){
        return !(a <= b);
    }
    friend bool operator>=(const Decay &a, const Decay &b){
        return !(a < b);
    }

private:
    std::vector<Rivet::PdgId> _parents;
    std::vector<Rivet::PdgId> _children;
};