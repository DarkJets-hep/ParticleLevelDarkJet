#pragma once

#include <Rivet/Particle.hh>
#include <iostream>
#include <vector>
#include <algorithm>
#include "ParticleName.hpp"

class Decay{
public:
    Decay(const std::vector<Rivet::PdgId> &parents, const std::vector<Rivet::PdgId> &children):
        _parents(parents),
        _children(children)
    {
        //So that the order doesn't matter
        std::sort(this->_parents.begin(), this->_parents.end());
        std::sort(this->_children.begin(), this->_children.end());
    }

    static Decay fromChild(Rivet::Particle child){
        while(child.parents().size() == 1 && child.parents()[0].children().size() == 1){
            child = child.parents()[0];
        }
        const Rivet::Particles parents = child.parents();
        if(parents.size() > 0){
            const Rivet::Particles children = parents[0].children();
            std::vector<Rivet::PdgId> parentPdgIds;
            std::transform(parents.begin(), parents.end(), std::back_inserter(parentPdgIds), [](const Rivet::Particle &p){return p.pid();});
            std::vector<Rivet::PdgId> childPdgIds;
            std::transform(children.begin(), children.end(), std::back_inserter(childPdgIds), [](const Rivet::Particle &p){return p.pid();});
            return Decay(parentPdgIds, childPdgIds);
        }
        return Decay(std::vector<Rivet::PdgId>(), {child.pid()});
    }

    static Decay fromParent(Rivet::Particle parent){
        while(parent.children().size() == 1 && parent.children()[0].parents().size() == 1){
            parent = parent.children()[0];
        }
        const Rivet::Particles children = parent.children();
        if(children.size() > 0){
            const Rivet::Particles parents = children[0].parents();
            std::vector<Rivet::PdgId> parentPdgIds;
            std::transform(parents.begin(), parents.end(), std::back_inserter(parentPdgIds), [](const Rivet::Particle &p){return p.pid();});
            std::vector<Rivet::PdgId> childPdgIds;
            std::transform(children.begin(), children.end(), std::back_inserter(childPdgIds), [](const Rivet::Particle &p){return p.pid();});
            return Decay(parentPdgIds, childPdgIds);
        }
        return Decay({parent.pid()}, std::vector<Rivet::PdgId>());
    }

    bool operator==(const Decay &other) const{
        return other._parents == this->_parents && other._children == this->_children;
    }
    bool operator!=(const Decay &other) const{
        return !(*this == other);
    }

    std::vector<Rivet::PdgId> parents() const{
        return this->_parents;
    }
    std::vector<Rivet::PdgId> children() const{
        return this->_children;
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
    std::vector<Rivet::PdgId> _parents;
    std::vector<Rivet::PdgId> _children;
};