# Headers

These C++ headers contain  functions that are meant to be reusable in other projects. These functions are header-only, so you can use them simply by including the correct header.

## [Darkness.hpp](https://raw.githubusercontent.com/DarkJets-hep/ParticleLevelDarkJet/main/Headers/Darkness.hpp)

This file contains functions to check the darkness, invisibility and lepton fraction of a jet.

Dependencies: Rivet, [ParticleSort.hpp](https://raw.githubusercontent.com/DarkJets-hep/ParticleLevelDarkJet/main/Headers/ParticleSort.hpp), [GetEnvVars.hpp](https://raw.githubusercontent.com/DarkJets-hep/ParticleLevelDarkJet/main/Headers/GetEnvVars.hpp)

Functions:

- **`bool particleIsDark(const Rivet::Particle &particle)`**: Checks if `particle` itself is a dark particle by comparing the absolute value of its PDG ID to a regular expression. The default regular expression is `^490[0-9][1-9][0-9]{2}$`, and can be overridden by setting the environment variable `DARK_REGEX`.
- **`bool hasDarkAncestor(Rivet::Particle particle)`**: Checks if `particle` has an ancestor that is a dark particle according to the `particleIsDark` function.
- **`double pTDarkness(const Rivet::Jet &jet)`**: Returns how much of the $p_\text{T}$ of `jet` originates from dark particles (0 if none of it does, 1 if all of it does).
- **`double multiplicityDarkness(const Rivet::Jet &jet)`**: Returns what fraction of particles in `jet` originate from dark particles (0 if none do, 1 if all do).
- **`double pTInvisibility(const Rivet::Jet &jet)`**: Returns how much of the $p_\text{T}$ of `jet` is invisible (0 if none of it is, 1 if all of it is).
- **`double multiplicityInvisibility(const Rivet::Jet &jet)`**: Returns what fraction of particles in `jet` are invisible (0 if none are, 1 if all are).
- **`double pTLeptonFraction(const Rivet::Jet &jet)`**: Returns how much of the $p_\text{T}$ of `jet` is carried by leptons (0 if none of it is, 1 if all of it is).
- **`double multiplicityLeptonFraction(const Rivet::Jet &jet)`**: Returns what fraction of particles in `jet` are leptons (0 if none are, 1 if all are).
- **`bool jetIsDark(const Rivet::Jet &jet, double darknessCut = 0.8)`**: Checks if `jet` is dark according to the definition proposed in my thesis. Note that jet should be built including invisible particles.

## [Decay.hpp](https://raw.githubusercontent.com/DarkJets-hep/ParticleLevelDarkJet/main/Headers/Decay.hpp)

This file contains a `Decay` class, which represents a specific type of decay (for example, $\pi_D \to c\bar{c}$ is one object, $\pi_D \to s\bar{s}$ is a different object).

Dependencies: Rivet, [ParticleName.hpp](https://raw.githubusercontent.com/DarkJets-hep/ParticleLevelDarkJet/main/Headers/ParticleName.hpp)

Constructor of the `Decay` class:

- **`Decay(const std::vector<Rivet::PdgId> &parents, const std::vector<Rivet::PdgId> &children)`**: Constructs a `Decay` object where the PDG IDs of the parent particles are given by `parents` and the PDG IDs of the child particles are given by `children`. For example, `Decay({111}, {22, 22})` constructs the $\pi^0 \to \gamma \gamma$ decay.

Static methods of the `Decay` class:

- **`Decay Decay::fromChild(Rivet::Particle child)`**: Returns a `Decay` object corresponding to the decay that resulted in `child` being produced. If `child` has no parents (most likely because it's a beam proton), the resulting `Decay` object will have a children vector of length 1 with the PDG ID of `child`, and an empty parents vector.
- **`Decay Decay::fromParent(Rivet::Particle parent)`**: Returns a `Decay` object corresponding to the decay channel of `parent`. If `parent` is stable, the resulting `Decay` object will have a parents vector of length 1 with the PDG ID of `parent`, and an emtpy children vector.

Methods of the `Decay` class:

- **`std::vector<Rivet::PdgId> parents() const`**: Returns a vector with the PDG IDs of each parent of the decay. The length of this vector can be greater than 1 in the case of scattering (although in that case the word "decay" might not be the most appropriate).
- **`std::vector<Rivet::PdgId> children() const`**: Returns a vector with the PDG IDs of each child of the decay.

Overloaded operators of the `Decay` class:

- **`bool Decay::operator==(const Decay &other) const`**: Returns true if the two objects represent the same decay, and false if they represent different decays.
- **`bool Decay::operator!=(const Decay &other) const`**: Returns false if the two objects represent the same decay, and true if they represent different decays.
- **`std::ostream& operator<<(std::ostream &flux, const Decay &decay)`**: Prints the Decay object `decay` to the stream `flux`. For example, to print a decay to the standard output, do `std::cout << decay`.
- **`bool operator<(const Decay &a, const Decay &b)`, `bool operator<=(const Decay &a, const Decay &b)`, `bool operator>(const Decay &a, const Decay &b)`, `bool operator>=(const Decay &a, const Decay &b)`**: Comparison operators are implemented in order to be able to use the `Decay` class as a key for an `std::map`. They do not produce any meaningful result and should never be used directly.

## [ParticleName.hpp](https://raw.githubusercontent.com/DarkJets-hep/ParticleLevelDarkJet/main/Headers/ParticleName.hpp)

This file contains functions to get particle names from PDG IDs.

Dependencies: Rivet

Functions:

- **`std::string particleName(int pdgid)`**: Returns the name of the Rivet constant corresponding to `pdgid`. For example, `particleName(11)` returns `ELECTRON`, since `Rivet::PID::ELECTRON` has the value 11. Note that all particles supported by this function do not have a Rivet constant, for example, `particleName(2224)` returns `DELTAPLUSPLUS`, but `Rivet::PID::DELTAPLUSPLUS` does not exist (there is no Rivet constant corresponding to $\Delta^{++}$). This is the case for $\Delta$, $\Sigma^\*$ and $\Xi^\*$ baryons, $K^\*$, $D^\*$ and $B^\*$ mesons, diquarks and dark particles. The behavior of this function is similar to Rivet's built-in `Rivet::PID::ParticleNames::particleName()` function, but the built-in one is incomplete.
- **`std::string particleNameAsTLatex(int pdgid)`**: Returns the symbol of the particle correspoinding to `pdgid` as a [TLatex](https://root.cern/doc/master/classTLatex.html) string. For example, `particleNameAsTLatex(13)` returns `#mu^{-}`. Note that the use of this function does not require ROOT (although to parse the result you would probably want to use ROOT).
- **`std::string absParticleNameAsTLatex(int pdgid)`**: Same as `particleNameAsTLatex`, but does not care whether the the PDG ID corresponds to a particle or its antiparticle. For example, `absParticleNameAsTLatex(13)` and `absParticleNameAsTLatex(-13)` both return `#mu^{#pm}`.

## [ParticleSort.hpp](https://raw.githubusercontent.com/DarkJets-hep/ParticleLevelDarkJet/main/Headers/ParticleSort.hpp)

This file contains utility functions for sorting.

Dependencies: Rivet

Functions:

- **`template<typename T1, typename T2> std::vector<std::pair<T1, T2>> sortMap(const std::map<T1, T2> &map)`**: Sorts `map` by value into an `std::vector` of `std::pairs`. This function is not related to Rivet, but is included here since I need it in my Rivet code.
- **`Rivet::Particles particlesByEnergy(Rivet::Particles particles)`**: Returns a vector of Rivet particles containing the same particles as `particles`, but sorted by energy.

## [GetEnvVars.hpp](https://raw.githubusercontent.com/DarkJets-hep/ParticleLevelDarkJet/main/Headers/GetEnvVars.hpp)

This file contains utility functions for reading environment variables. This isn't directly related to Rivet (so this file can be used without having Rivet installed), but is included here since I use it in my Rivet code.

Dependencies: None

Functions:

- **`int getIntFromEnvVar(const char *name, int def)`**: If the environment variable with name `name` exists and is a valid integer, returns that integer, otherwise returns `def`.
- **`double getDoubleFromEnvVar(const char *name, double def)`**: If the environment variable with name `name` exists and is a valid double, returns that double, otherwise returns `def`.
- **`template<typename String> String getStringFromEnvVar(const char *name, const String &def)`**: If the environment variable with name `name` exists, returns its contents as a string, otherwise returns `def`. This is a template in order to be able to use it both for `std::string` and `TString`. The template argument can be any type that a `char*` can be converted to.
- **`std::vector<int> getIntVectorFromEnvVar(const char *name, const std::vector<int> &def)`**: If the environment variable with name `name` exists and is a comma-seperated list of integers, returns those integers stored in a `std::vector<int>`, otherwise returns `def`.
