#pragma once

#include "Rivet/Particle.hh"
#include <iostream>
#include <vector>
#include <map>
#include "ParticleName.hpp"

class Decay{
public:
    static Decay fromChild(const Rivet::Particle &child){
        Decay decay;
        const Rivet::Particles parents = child.parents();
        if(parents.size() > 0){
            for(const Rivet::Particle &parent: parents){
                decay._parents.push_back(parent.pid());
            }
            std::sort(decay._parents.begin(), decay._parents.end());    //So that the order doesn't matter
            for(const Rivet::Particle &child: parents[0].children()){
                decay._children.push_back(child.pid());
            }
            std::sort(decay._children.begin(), decay._children.end());
        }
        else{
            decay._children.push_back(child.pid());
        }
        return decay;
    }

    static Decay fromParent(const Rivet::Particle &parent){
        Decay decay;
        const Rivet::Particles children = parent.children();
        if(children.size() > 0){
            for(const Rivet::Particle &child: children){
                decay._children.push_back(child.pid());
            }
            std::sort(decay._children.begin(), decay._children.end());    //So that the order doesn't matter
            for(const Rivet::Particle &parent: children[0].parents()){
                decay._parents.push_back(parent.pid());
            }
            std::sort(decay._parents.begin(), decay._parents.end());
        }
        else{
            decay._parents.push_back(parent.pid());
        }
        return decay;
    }

    bool operator==(const Decay &other) const{
        return other._parents == this->_parents && other._children == this->_children;
    }
    bool operator!=(const Decay &other) const{
        return !(*this == other);
    }

    friend std::ostream& operator<<(std::ostream &flux, const Decay &decay){
        if(decay._parents.size() == 0 || decay._children.size() == 0){
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
    Decay(){}

    std::vector<Rivet::PdgId> _parents;
    std::vector<Rivet::PdgId> _children;
};